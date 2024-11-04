/*
 * eth or ap
 */

#if 1
	#include "control/conf.h" //#include "control/drv/conf_core.h"
	#include "control/drv/dm9051_eth_debug.h"
#else
	#include "control/conf.h" //#include "control/ap/conf_ap.h"
	#include "control/ap/dm9051_ap_debug.h"
#endif

/*
 * global
 */

#define DM_TYPE 1
#include "dm_types_define.h"

#define DM_TYPE 2
#include "dm_types_define.h"

/*
 * candidate
 */

const struct eth_node_t node_candidate[1] = {
	{
		{0, 0x60, 0x6e, 0x00, 0x00, 0x17},
		{192, 168, 6, 17},
		{192, 168, 6, 1},
		{255, 255, 255, 0},
	}, /*
	   { \
		   {0, 0x60, 0x6e, 0x00, 0x01, 0x26,}, \
		   {192, 168, 6,  26}, \
		   {192, 168, 6,   1}, \
		   {255, 255, 255, 0}, \
	   }, \
	   { \
		   {0, 0x60, 0x6e, 0x00, 0x01, 0x25,}, \
		   {192, 168, 6,  25}, \
		   {192, 168, 6,   1}, \
		   {255, 255, 255, 0}, \
	   }, \
	   { \
		   {0, 0x60, 0x6e, 0x00, 0x01, 0xfe,}, \
		   {192, 168, 6,  66}, \
		   {192, 168, 6,   1}, \
		   {255, 255, 255, 0}, \
	   }, \
	   { \
		   {0, 0x60, 0x6e, 0x00, 0x01, 0xff,}, \
		   {192, 168, 6,  67}, \
		   {192, 168, 6,   1}, \
		   {255, 255, 255, 0}, \
	   }, \
	   */
};

/*
 * dbg_def info-function
 */
const char *level_str_impl(dm9051_eth_debug_level_t level)
{
	const char *level_str;
	switch (level) {
		case DM9051_ETH_DEBUG_LEVEL_ERROR: level_str = "ERROR"; break;
		case DM9051_ETH_DEBUG_LEVEL_WARN:  level_str = "WARN";  break;
		case DM9051_ETH_DEBUG_LEVEL_INFO:  level_str = "INFO";  break;
		case DM9051_ETH_DEBUG_LEVEL_DEBUG: level_str = "DEBUG"; break;
		default:                           level_str = "UNKNOWN";
	} \
	return level_str;
}

//---------------------------------------

unsigned long dispc_int_active = 0; //, dispc_int_active_saved = 0;

void inc_interrupt_count(void)
{
	dispc_int_active++;
}

unsigned long get_interrupt_count(void)
{
	return dispc_int_active;
}

//---------------------------------------

uint16_t wrpadiff(uint16_t rwpa_s, uint16_t rwpa_e)
{
	return (rwpa_e >= rwpa_s) ? rwpa_e - rwpa_s : (rwpa_e + 0x4000 - 0xc00) - rwpa_s;
}

//---------------------------------------

// Debug functionality
#if DM_ETH_DEBUG_MODE
void debug_diff_rx_pointers(int state, uint16_t rd_now) {
#if drv_print
	static int drp_fifoTurn_n = 0;
	static uint16_t drp_premdra_rd = 0x4000;
	static uint16_t drp_mdra_rd;
	uint16_t compos_totaldiff, diff;

	if (drp_premdra_rd == 0x4000)
		drp_mdra_rd = rd_now; //~return;

	if (state)
		drp_fifoTurn_n++;
	if (rd_now < drp_premdra_rd && (drp_premdra_rd != 0x4000)) {
		/*uint16_t*/ compos_totaldiff = (rd_now >= drp_mdra_rd) ? 0x3400 : 0;
		/*uint16_t*/ diff = wrpadiff(drp_mdra_rd, rd_now);
		printf("(INT %lu) mdra s %02x%02x e %02x%02x dif %x (nrx %d) .eth\r\n",
			get_interrupt_count(),
			drp_mdra_rd >> 8, drp_mdra_rd & 0xff,
			rd_now >> 8, rd_now & 0xff,
			diff + compos_totaldiff,
			drp_fifoTurn_n);

		drp_fifoTurn_n = 0;
		drp_mdra_rd = rd_now; //~return;
	}
	drp_premdra_rd = rd_now;
#endif
}
#endif

//---------------------------------------
/*
 * cboard_print_hex
 */

#define kkmin(a, b) (a < b) ? a : b

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

static void sprint_hex_dump0(int head_space, int titledn, char *prefix_str,
							 size_t tlen, int rowsize, const void *buf, int seg_start, size_t len, int cast_lf)
{
	char lineroombuff[180];
	int print_linefeed_flag;
	int si, se, titlec = 0;
	int i, linelen, remaining = len; // hs, const eth_class_t *ec = &eclass[10];
	int nspace = 0;

	(void)head_space;

	si = seg_start;
	se = seg_start + len;
	for (i = si; i < se; i += rowsize)
	{
		char linebuf[(12 * 3) + (3 * 16) + 1 + 32]; // here!

		nspace = room_printf_rxlen_head(lineroombuff, tlen, nspace);
		linelen = kkmin(remaining, rowsize);
		remaining -= rowsize;
		do
		{
			const uint8_t *ptr = buf;
			int j;
			int nb = 0;
			for (j = 0; j < linelen && (size_t)nb < sizeof(linebuf); j++)
			{
				if (j && !(j % 8))
					nb += snprintf(linebuf + nb, sizeof(linebuf) - nb, " ");
				if (((rowsize >> 1) != 8) && !(j % (rowsize >> 1)))
					nb += snprintf(linebuf + nb, sizeof(linebuf) - nb, " ");

				nb += snprintf(linebuf + nb, sizeof(linebuf) - nb, "%02x ", *(ptr + i + j));
			}
		} while (0);

		nspace += sprintf(lineroombuff+nspace, " ");

		if (prefix_str)
		{
			nspace += sprintf(lineroombuff+nspace, "(%s) %.3x %s", prefix_str, i, linebuf);
			while (titledn)
			{
				titledn--;
				prefix_str[titlec++] = ' ';
			}
		}
		else
		{
			nspace += sprintf(lineroombuff+nspace, "(dm9 xfer) %.3x %s", i, linebuf);
		}

		print_linefeed_flag = 0;
		if ((i + rowsize) < se)
			print_linefeed_flag = 1;
		else
		{
			if (cast_lf)
				print_linefeed_flag = 1;
#if 0
				if (IS_UDP) {
#if 0
					size_t ulen = tlen; // larger for with 4-bytes CRC
					ulen = UIP_LLH_LEN;
					ulen += HTONS(UDPBUF->udplen) - 8;
					ulen += sizeof(struct uip_udpip_hdr); // correct for without 4-bytes CRC (htons)

					if (cast_lf)
						printf("\r\n");

					printf(" ..SrcIP %d.%d.%d.%d", (IPBUF->srcipaddr[0] >> 0) & 0xff, (IPBUF->srcipaddr[0] >> 8),
						(IPBUF->srcipaddr[1] >> 0) & 0xff, (IPBUF->srcipaddr[1] >> 8));
					printf("  DestIP %d.%d.%d.%d", (IPBUF->destipaddr[0] >> 0) & 0xff, (IPBUF->destipaddr[0] >> 8),
						(IPBUF->destipaddr[1] >> 0) & 0xff, (IPBUF->destipaddr[1] >> 8));
					printf("  Len %d", ulen);
					printf("  (%5d -> %d Len %d)", UDPBUF->srcport, UDPBUF->destport, HTONS(UDPBUF->udplen) - 8);
					printf("\r\n");
#endif
				}
				if (IS_TCP) {
					size_t ulen = tlen; // larger for with 4-bytes CRC
					printf(" ..SrcIP %d.%d.%d.%d", (IPBUF->srcipaddr[0] >> 0) & 0xff, (IPBUF->srcipaddr[0] >> 8),
						(IPBUF->srcipaddr[1] >> 0) & 0xff, (IPBUF->srcipaddr[1] >> 8));
					printf("  DestIP %d.%d.%d.%d", (IPBUF->destipaddr[0] >> 0) & 0xff, (IPBUF->destipaddr[0] >> 8),
						(IPBUF->destipaddr[1] >> 0) & 0xff, (IPBUF->destipaddr[1] >> 8));
					printf("  Len %d", ulen);
					
					if (TCPBUF->flags == 0x18)
						printf("  (%5d -> %d) flags %02x (PSH, ACK)", HTONS(TCPBUF->srcport), HTONS(TCPBUF->destport), TCPBUF->flags);
					else
						printf("  (%5d -> %d) flags %02x", HTONS(TCPBUF->srcport), HTONS(TCPBUF->destport), TCPBUF->flags);
					
					printf("\r\n");
				}
#endif
		}
		if (print_linefeed_flag)
			printf("%s\r\n", lineroombuff);
		else
			printf("%s", lineroombuff);
	}
}

/* print log
 */

#if DM_ETH_DEBUG_MODE
int link_log_reset_allow_num = 0;
const int rx_modle_log_reset_allow_num = 1; //3;
#define limit_len(n, nTP) ((n <= nTP) ? n : nTP)

void dm_eth_input_hexdump_reset(void) {
	if (link_log_reset_allow_num) //Jos like protect from always write-it.
		link_log_reset_allow_num = 0;
}

void dm_eth_input_hexdump(const void *buf, size_t len)
{
	int titledn = 0;
	if (link_log_reset_allow_num < rx_modle_log_reset_allow_num)
	{
		link_log_reset_allow_num++;
		sprint_hex_dump0(2, titledn, "dm9 head   <<rx", len, 32, buf, 0, 
			limit_len(len, 14) /*limit_len(len, 66)*/,
			DM_TRUE);
	}
}
#endif
