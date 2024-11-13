//.#ifndef __DM_TYPES3_H
//.#define __DM_TYPES3_H
/* #include "control/dbg_opts.h" */

/* Configuration Constants */
#define MAX_NODE_CANDIDATES    6
#define RX_BUFFER_START       0xC00
#define RX_BUFFER_END         0x4000
#define MAX_HEX_LINE_BUF      180
#define MAX_HEX_SEGMENT       32
#define DEFAULT_MDRA_RD       0x4000
#define TOTAL_DIFF_OFFSET     0x3400

/* Debug Configuration */
#define MAX_RX_LOG_ENTRIES    1
#define MIN_HEADER_LENGTH     14
#define MIN(a, b)             ((a < b) ? a : b)
#define LIMIT_LEN(n, nTP)     ((n <= nTP) ? n : nTP)

/*
 * DEBUG-SEC-10, static
 *  - essential static sub (or data)
 *  - non-debug static sub
 */
#if DM_DEBUG_TYPE == 10
/* Buffer Management */
static uint16_t wrpadiff(uint16_t rwpa_s, uint16_t rwpa_e)
{
    return (rwpa_e >= rwpa_s) ? 
           rwpa_e - rwpa_s : 
           (rwpa_e + RX_BUFFER_END - RX_BUFFER_START) - rwpa_s;
}

#if !DM_ETH_DEBUG_MODE
static void inc_interrupt_count(void)
{
}
static void dm_eth_input_hexdump_reset(void)
{
}
static void debug_diff_rx_pointers(int state, uint16_t rd_now)
{
}
static void dm_eth_input_hexdump(const void *buf, size_t len)
{
}
#endif //!DM_ETH_DEBUG_MODE
#endif //DM_DEBUG_TYPE 10

/*
 * DEBUG-SEC-11, static
 *  - debug static sub
 */
#if DM_DEBUG_TYPE == 11
#if DM_ETH_DEBUG_MODE

/* Interrupt Tracking */
static volatile unsigned long dispc_int_active = 0;

static void inc_interrupt_count(void)
{
    dispc_int_active++;
}

/* Hex Dump Implementation */
#if (defined(__DM9051_ETH_DEBUG_H) && drv_print)  || (defined(__DM9051_AP_DEBUG_H) && ap_print) //org 'drv_print'
static int room_printf_space(char *lineroom, int offset, int n)
{
    while (n--)
        offset += sprintf(lineroom + offset, "%c", ' ');
    return offset;
}

static int room_printf_space_init(char *lineroom, size_t tlen)
{
    int offset = 0;
    char textspace[16];
    int n = sprintf(textspace, "rxlen %4d", tlen);

    offset = room_printf_space(lineroom, offset, n);
    offset += sprintf(lineroom + offset, " %s", textspace);
    printf("%s\r\n", lineroom);

    return n;
}

static int room_printf_rxlen_head(char *lineroom, size_t tlen, int nspc)
{
    if (!nspc)
        nspc = room_printf_space_init(lineroom, tlen);

    room_printf_space(lineroom, 0, nspc);
    return nspc;
}
#endif

static void sprint_hex_dump0(int head_space, int titledn, char *prefix_str,
                           size_t tlen, int rowsize, const void *buf, 
                           int seg_start, size_t len, int cast_lf)
{
#if (defined(__DM9051_ETH_DEBUG_H) && drv_print)  || (defined(__DM9051_AP_DEBUG_H) && ap_print) //org 'drv_print'
    char lineroombuff[MAX_HEX_LINE_BUF];
    int print_linefeed_flag;
    int si = seg_start;
    int se = seg_start + len;
    int titlec = 0;
    int nspace = 0;
		int i;

    (void)head_space;

    for (i = si; i < se; i += rowsize)
    {
        char linebuf[(12 * 3) + (3 * 16) + 1 + 32];
        int remaining = len - (i - si);
        int linelen = MIN(remaining, rowsize);
        const uint8_t *ptr = buf;
        int nb = 0;
        int j;

        nspace = room_printf_rxlen_head(lineroombuff, tlen, nspace);
        
        /* Format hex values */
        for (j = 0; j < linelen && (size_t)nb < sizeof(linebuf); j++)
        {
            if (j && !(j % 8))
                nb += snprintf(linebuf + nb, sizeof(linebuf) - nb, " ");
            if (((rowsize >> 1) != 8) && !(j % (rowsize >> 1)))
                nb += snprintf(linebuf + nb, sizeof(linebuf) - nb, " ");

            nb += snprintf(linebuf + nb, sizeof(linebuf) - nb, "%02x ", *(ptr + i + j));
        }

        /* Format output line */
        nspace += sprintf(lineroombuff + nspace, " ");
        if (prefix_str) {
            nspace += sprintf(lineroombuff + nspace, "(%s) %.3x %s", prefix_str, i, linebuf);
            while (titledn) {
                titledn--;
                prefix_str[titlec++] = ' ';
            }
        } else {
            nspace += sprintf(lineroombuff + nspace, "(dm9 xfer) %.3x %s", i, linebuf);
        }

        /* Handle line endings */
        print_linefeed_flag = ((i + rowsize) < se) || cast_lf;
        printf("%s%s", lineroombuff, print_linefeed_flag ? "\r\n" : "");
    }
#endif
}

/* Debug Logging Interface */
static int link_log_reset_allow_num = 0;
static const int rx_modle_log_reset_allow_num = MAX_RX_LOG_ENTRIES;

static void dm_eth_input_hexdump_reset(void)
{
	// Jos like protect from always write-it.
	if (link_log_reset_allow_num) {
		link_log_reset_allow_num = 0;
	}
}

static void dm_eth_input_hexdump(const void *buf, size_t len)
{
    if (link_log_reset_allow_num >= rx_modle_log_reset_allow_num) {
        return;
    }

    link_log_reset_allow_num++;
    sprint_hex_dump0(2, 0, "dm9 head   <<rx", len, MAX_HEX_SEGMENT,
                    buf, 0, LIMIT_LEN(len, MIN_HEADER_LENGTH), DM_TRUE);
}

//uint16_t wrpadiff(uint16_t rwpa_s, uint16_t rwpa_e);
//void dm_eth_input_hexdump_reset(void);
//void dm_eth_input_hexdump(const void *buf, size_t len);

/* Debug Pointer Tracking */
#if (defined(__DM9051_ETH_DEBUG_H) && drv_print)  || (defined(__DM9051_AP_DEBUG_H) && ap_print) //org 'drv_print'
static int fifo_fifoTurn_n = 0;
static uint16_t fifo_premdra_rd = DEFAULT_MDRA_RD;
static uint16_t fifo_mdra_rd;
#endif

static void debug_diff_rx_pointers(int state, uint16_t rd_now)
{
#if (defined(__DM9051_ETH_DEBUG_H) && drv_print)  || (defined(__DM9051_AP_DEBUG_H) && ap_print) //org 'drv_print'
	if (fifo_premdra_rd == DEFAULT_MDRA_RD)
		fifo_mdra_rd = rd_now;

	if (state)
		fifo_fifoTurn_n++;

	if (rd_now < fifo_premdra_rd && (fifo_premdra_rd != DEFAULT_MDRA_RD))
	{
		uint16_t compos_totaldiff = (rd_now >= fifo_mdra_rd) ? TOTAL_DIFF_OFFSET : 0;
		uint16_t diff = wrpadiff(fifo_mdra_rd, rd_now);

		printf("(INT %lu) mdra s %02x%02x e %02x%02x dif %x (nrx %d) .eth\r\n",
			   get_interrupt_count(),
			   fifo_mdra_rd >> 8, fifo_mdra_rd & 0xff,
			   rd_now >> 8, rd_now & 0xff,
			   diff + compos_totaldiff,
			   fifo_fifoTurn_n);

		fifo_fifoTurn_n = 0;
		fifo_mdra_rd = rd_now;
	}
	fifo_premdra_rd = rd_now;
#endif
}
#endif //DM_ETH_DEBUG_MODE
#endif //DM_DEBUG_TYPE 11

/*
 * DEBUG-SEC-0, public
 *  - essential extern sub declaration (or extern data)
 */
#if DM_DEBUG_TYPE == 0
typedef void (*printkey_ptr)(char *);

//void diff_rx_s(void);
//void diff_rx_e(void);
//void print_eth_configuration(char *head, const uint8_t *ip, printkey_ptr printky);
//void ap_print_ipconfig(char *head, const uint8_t *mac, printkey_ptr printky);
//unsigned long get_interrupt_count(void);
#endif //DM_DEBUG_TYPE 0

/*
 * DEBUG-SEC-20, public
 *  - essential extern sub
 *  - non-debug extern sub
 */
#if DM_DEBUG_TYPE == 20
/* essential extern sub */
void print_eth_configuration(char *head, const uint8_t *ip, printkey_ptr printky)
{
#if rt_print | drv_print
	char buf[100];
  sprintf(buf, "%s %d.%d.%d.%d\r\n", head, ip[0], ip[1], ip[2], ip[3]);
	printky(buf);
#endif
}
void ap_print_ipconfig(char *head, const uint8_t *mac, printkey_ptr printky)
{
	#if ap_print //defined(__DM9051_AP_DEBUG_H) && ap_print
		uint8_t *addr;
		char buf[100];
		
		sprintf(buf, "%s\r\n", head);
		printky(buf);

		sprintf(buf, "  Network chip: DAVICOM DM9051 \r\n");
		printky(buf);
		sprintf(buf, "  MAC Address: %X:%X:%X:%X:%X:%X \r\n", mac[0], mac[1],
					 mac[2], mac[3], mac[4], mac[5]);
		printky(buf);
		addr = DM_ETH_Ip_Configured();
		sprintf(buf, "  Host IP Address: %d.%d.%d.%d \r\n", addr[0], addr[1], addr[2], addr[3]);
		printky(buf);
		addr = DM_ETH_Mask_Configured();
		sprintf(buf, "  Network Mask: %d.%d.%d.%d \r\n", addr[0], addr[1], addr[2], addr[3]);
		printky(buf);
		addr = DM_ETH_Gw_Configured();
		sprintf(buf, "  Gateway IP Address: %d.%d.%d.%d \r\n", addr[0], addr[1], addr[2], addr[3]);
		printky(buf);
//		printkey("%s\r\n", head);

//		printkey("Network chip: DAVICOM DM9051 \r\n");
//		printkey("MAC Address: %X:%X:%X:%X:%X:%X \r\n", mac[0], mac[1],
//					 mac[2], mac[3], mac[4], mac[5]);
//		addr = DM_ETH_Ip_Configured();
//		printkey("Host IP Address: %d.%d.%d.%d \r\n", addr[0], addr[1], addr[2], addr[3]);
//		addr = DM_ETH_Mask_Configured();
//		printkey("Network Mask: %d.%d.%d.%d \r\n", addr[0], addr[1], addr[2], addr[3]);
//		addr = DM_ETH_Gw_Configured();
//		printkey("Gateway IP Address: %d.%d.%d.%d \r\n", addr[0], addr[1], addr[2], addr[3]);
		
//		uip_ipaddr_t ipaddr={0,0};
//		printf("MAC Address: %X:%X:%X:%X:%X:%X \r\n", uip_ethaddr.addr[0], uip_ethaddr.addr[1],
//					 uip_ethaddr.addr[2], uip_ethaddr.addr[3], uip_ethaddr.addr[4], uip_ethaddr.addr[5]);
//		uip_gethostaddr(ipaddr);
//		printf("Host IP Address: %d.%d.%d.%d \r\n", uip_ipaddr1(ipaddr), uip_ipaddr2(ipaddr), uip_ipaddr3(ipaddr), uip_ipaddr4(ipaddr));
//		uip_getnetmask(ipaddr);
//		printf("Network Mask: %d.%d.%d.%d \r\n", uip_ipaddr1(ipaddr), uip_ipaddr2(ipaddr), uip_ipaddr3(ipaddr), uip_ipaddr4(ipaddr));
//		uip_getdraddr(ipaddr);
//		printf("Gateway IP Address: %d.%d.%d.%d \r\n", uip_ipaddr1(ipaddr), uip_ipaddr2(ipaddr), uip_ipaddr3(ipaddr), uip_ipaddr4(ipaddr));
		printky("-----------------------------------------\r\n");
	#endif
}

#if !DM_ETH_DEBUG_MODE
/* non-debug */
void diff_rx_s(void)
{
}
void diff_rx_e(void)
{
}

unsigned long get_interrupt_count(void)
{
    return 0;
}
#endif //!DM_ETH_DEBUG_MODE
#endif //DM_DEBUG_TYPE 20
/*
 * DEBUG-SEC-21, public
 *  - debug extern sub
 */
#if DM_DEBUG_TYPE == 21
#if DM_ETH_DEBUG_MODE
/* debug */
void diff_rx_pointers_s(uint16_t *pMdra_rds) {
	uint16_t dummy_rds= 0xc00;
	DM_ETH_ToCalc_rx_pointers(0, &dummy_rds, pMdra_rds);
}

void diff_rx_s(void)
{
	if (!fifoTurn_n)
		diff_rx_pointers_s(&gkeep_mdra_rds); //&mdra_rds
}

int fifoTurn_n = 0;
uint16_t gkeep_mdra_rds;

void diff_rx_pointers_e(uint16_t *pMdra_rds) {
	uint16_t mdra_rd, diff;
	diff = DM_ETH_ToCalc_rx_pointers(1, pMdra_rds, &mdra_rd); //......................

	do {
		static uint16_t premdra_rd = 0x4000;
		if (mdra_rd < premdra_rd && (premdra_rd != 0x4000)) {
			//uint16_t compos_totaldiff = (mdra_rd >= *pMdra_rds) ? 0x3400 : 0;
			diff += (mdra_rd >= *pMdra_rds) ? 0x3400 : 0;
			printf("[vuIP_Tasks] mdra.s %02x%02x e %02x%02x dif %02x%02x (nrx %d)\r\n",
				*pMdra_rds >> 8, *pMdra_rds & 0xff,
				mdra_rd >> 8, mdra_rd & 0xff,
				diff >> 8, diff & 0xff,
				fifoTurn_n);
			fifoTurn_n = 0;
		}
		premdra_rd = mdra_rd;
	} while(0);
}

void diff_rx_e(void)
{
	fifoTurn_n++;
	diff_rx_pointers_e(&gkeep_mdra_rds); //&mdra_rds
}

unsigned long get_interrupt_count(void)
{
    return dispc_int_active;
}
#endif //DM_ETH_DEBUG_MODE
#endif //DM_DEBUG_TYPE 21

#undef DM_DEBUG_TYPE
//.#endif //__DM_TYPES3_H
