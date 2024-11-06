/**
 * DM9051 Implementation
 * 
 * This file contains debug and utility functions for the DM9051 Ethernet controller
 */

/* Configuration Selection */
#if 0
//#include "control/conf.h"
//#include "control/drv/dm9051_eth_debug.h"
#else
#include "control/conf.h"
#include "control/ap/dm9051_ap_debug.h"
#endif

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

#if DM_ETH_DEBUG_MODE
/* Buffer Management */
uint16_t wrpadiff(uint16_t rwpa_s, uint16_t rwpa_e)
{
    return (rwpa_e >= rwpa_s) ? 
           rwpa_e - rwpa_s : 
           (rwpa_e + RX_BUFFER_END - RX_BUFFER_START) - rwpa_s;
}

/* Debug Pointer Tracking */
#if (defined(__DM9051_ETH_DEBUG_H) && drv_print)  || (defined(__DM9051_AP_DEBUG_H) && ap_print) //org 'drv_print'
static int fifo_fifoTurn_n = 0;
static uint16_t fifo_premdra_rd = DEFAULT_MDRA_RD;
static uint16_t fifo_mdra_rd;
#endif

void debug_diff_rx_pointers(int state, uint16_t rd_now)
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

void dm_eth_input_hexdump_reset(void)
{
	// Jos like protect from always write-it.
	if (link_log_reset_allow_num) {
		link_log_reset_allow_num = 0;
	}
}

void dm_eth_input_hexdump(const void *buf, size_t len)
{
    if (link_log_reset_allow_num >= rx_modle_log_reset_allow_num) {
        return;
    }

    link_log_reset_allow_num++;
    sprint_hex_dump0(2, 0, "dm9 head   <<rx", len, MAX_HEX_SEGMENT,
                    buf, 0, LIMIT_LEN(len, MIN_HEADER_LENGTH), DM_TRUE);
}
#endif
