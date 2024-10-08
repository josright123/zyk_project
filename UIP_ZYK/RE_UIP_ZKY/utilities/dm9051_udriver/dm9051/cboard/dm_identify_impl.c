#include "dm9051opts.h"
#include "dm9051.h"
#include "dm_identify_impl.h"

#define DM_TYPE 1
#include "dm_identify_types_define.h"

#define DM_TYPE 2
#include "dm_identify_types_define.h"

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

//---------------------------------------

unsigned long dispc_int_active = 0; //, dispc_int_active_saved = 0;

void inc_interrupt_count(void)
{
	dispc_int_active++;
	// at32_led_toggle(LED2);
}

unsigned long get_task_tryint(void)
{
	return dispc_int_active;
}

//---------------------------------------

uint16_t wrpadiff(uint16_t rwpa_s, uint16_t rwpa_e)
{
	return (rwpa_e >= rwpa_s) ? rwpa_e - rwpa_s : (rwpa_e + 0x4000 - 0xc00) - rwpa_s;
}

//---------------------------------------
/*
 * cboard_print_hex
 */

#define kkmin(a, b) (a < b) ? a : b

static void printf_space(int n)
{
	while (n--)
		printf("%c", ' ');
}

static int printf_space_init(size_t tlen)
{
	char cspace[16];
	int n = sprintf(cspace, "rxlen %4d", tlen);
#if 1
	printf_space(n);
	printf(" %s\r\n", cspace);
#endif
	return n;
}

static int printf_rxlen_head(size_t tlen, int nspc)
{
#if 1
	if (!nspc)
		nspc = printf_space_init(tlen);

	printf_space(nspc);
	return nspc;
#endif
}

static void sprint_hex_dump0(int head_space, int titledn, char *prefix_str,
							 size_t tlen, int rowsize, const void *buf, int seg_start, size_t len, /*int useflg*/ int cast_lf) //, int must, int dgroup
{
	// #undef printf
	// #define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	// if (useflg) {
	int si, se, titlec = 0;
	int i, linelen, remaining = len; // hs, const eth_class_t *ec = &eclass[10];
	int nspace = 0;

	(void)head_space;
	//(void) tlen;

	si = seg_start;
	se = seg_start + len;
	for (i = si; i < se; i += rowsize)
	{
		// unsigned
		char linebuf[(12 * 3) + (3 * 16) + 1 + 32]; // here!

		nspace = printf_rxlen_head(tlen, nspace);

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
		//.nspace = printf_rxlen_head(tlen, nspace);

#if 0
		hs = head_space;
		while(hs--)
#endif
		printf(" ");

		if (prefix_str)
		{
			printf("(%s) %.3x %s", prefix_str, i, linebuf); //"%s", ec->str //CHECK (XXX >> )
			while (titledn)
			{
				titledn--;
				prefix_str[titlec++] = ' ';
			}
		}
		else
		{
			printf("(dm9 xfer) %.3x %s", i, linebuf); //"%s", ec->str
		}

		if ((i + rowsize) < se)
			printf("\r\n");
		else
		{
			if (cast_lf)
				printf("\r\n");
#if 0
				if (IS_UDP) {
					//ptr
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
	}
	//}
	// #undef printf
	// #define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

/* print log
 */

#if DM_ETH_DEBUG_MODE
int link_log_reset_allow_num = 0;
const int rx_modle_log_reset_allow_num = 3;
#define limit_len(n, nTP) ((n <= nTP) ? n : nTP)

void dm_eth_input_hexdump_reset(void) {
	if (link_log_reset_allow_num) //Jos like protect from always write-it.
		link_log_reset_allow_num = 0;
}

void dm_eth_input_hexdump(const void *buf, size_t len)
{
	int titledn = 0;
	DM_UNUSED_ARG(buf);
	DM_UNUSED_ARG(len);

	if (link_log_reset_allow_num < rx_modle_log_reset_allow_num)
	{
		link_log_reset_allow_num++;
		// if (link_log_reset_allow_num == rx_modle_log_reset_allow_num && get_tcpip_thread_state() == 1) {
		// set_tcpip_thread_state(6);
		//}
		sprint_hex_dump0(2, titledn, "dm9 head   <<rx", len, 32, buf, 0, limit_len(len, 66), DM_TRUE);
	}
}
#endif
