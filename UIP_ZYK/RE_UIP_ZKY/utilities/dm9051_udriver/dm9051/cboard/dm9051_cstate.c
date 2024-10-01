#include "stdio.h"
#include "dm9051opts.h"
#include "cboard/dm9051_lw_mcu_default_IN.h"
#include "dm9051_cstate.h"

#define CB_TYPE 0
#include "cb_types2.h"

#define CB_TYPE 1
#include "cb_types2.h"

#define CB_TYPE 2
#include "cb_types2.h"

/*
 * cboard_trace_irq_flow
 */

// uint16_t irqst = 0;

void deidentify_irq_stat(uint16_t bitflg)
{
	// irqst &= ~bitflg; //return DM_GET_FIELD(uint16_t, irqst);
	SET_CSTATE(irqst, GET_CSTATE(irqst) & ~bitflg); // DM_SET_FIELDirqst(DM_GET_FIELDirqst() & ~bitflg);
}
void identify_irq_stat(uint16_t bitflg)
{
	//	char istat_term[22];
	// irqst |= bitflg; //DM_SET_FIELD(uint16_t, irqst, DM_GET_FIELD(uint16_t, irqst) | bitflg); //store into dm9051optsex[i].read_chip_id
	SET_CSTATE(irqst, GET_CSTATE(irqst) | bitflg); // DM_SET_FIELDirqst(DM_GET_FIELDirqst() | bitflg);

	//	switch(bitflg) {
	//		case ISTAT_IRQ_CFG:
	//			sprintf(istat_term, "(IRQ_CFG)");
	//			break;
	//		case ISTAT_IRQ_ENAB:
	//			sprintf(istat_term, "(IRQ_ENAB)");
	//			break;
	//		case ISTAT_DM_IMR:
	//			sprintf(istat_term, "(IMR.pr)");
	//			break;
	//		case ISTAT_DM_RCR:
	//			sprintf(istat_term, "(RCR.rxen)");
	//			break;
	//		case ISTAT_LOW_TRIGGER:
	//			sprintf(istat_term, "(IRQ_LOW_TRIGGER)");
	//			break;
	//		case ISTAT_LOW_ACTIVE:
	//			sprintf(istat_term, "(INTR.lo)");
	//			break;
	//		case ISTAT_IRQ_NOW2:
	//			sprintf(istat_term, "(INT %lu)", get_task_tryint());
	//			break;
	//		default:
	//			istat_term[0] = 0;
	//			break;
	//	}
	//	printf("[irq_stat]:                   ------------> irqst= %02x on add %02x %s\r\n", identified_irq_stat(), bitflg, istat_term);
}

void trace_irq_stat(uint16_t bitflg)
{
	char istat_term[22];
	switch (bitflg)
	{
	case ISTAT_IRQ_CFG:
		sprintf(istat_term, "[IRQ_CFG]");
		break;
	case ISTAT_IRQ_ENAB:
		sprintf(istat_term, "[IRQ_ENAB]");
		break;
	case ISTAT_DM_IMR:
		sprintf(istat_term, "(IMR.pr)");
		break;
	case ISTAT_DM_RCR:
		sprintf(istat_term, "(RCR.rxen)");
		break;
	case ISTAT_LOW_TRIGGER:
		sprintf(istat_term, "[IRQ_LOW_TRIGGER]");
		break;
	case ISTAT_LOW_ACTIVE:
		sprintf(istat_term, "(INTR.lo)");
		break;
	case ISTAT_IRQ_NOW2:
		sprintf(istat_term, "(INT %lu)", get_task_tryint());
		break;
	case ISTAT_IRQ_NOW:
	default:
		istat_term[0] = 0;
		break;
	}

	if (get_task_tryint() > 5 && (bitflg == ISTAT_IRQ_NOW || bitflg == ISTAT_IRQ_NOW2))
		return;

	printf("[irq_stat]:                   ------------> irqst= %02x on add %02x %s\r\n", identified_irq_stat(), bitflg, istat_term);
}

uint16_t identified_irq_stat(void)
{
	// return irqst;
	return GET_CSTATE(irqst);
}
// uint16_t DM_GET_FIELD_irqst(void) {
//	return irqst;
// }

// void trace_dm9051_set_recv(void); //trace
// void trace_dm9051_set_recv(void) {
// #ifdef DM9051_DRIVER_INTERRUPT
//	trace_irq_stat(ISTAT_DM_IMR);
//	trace_irq_stat(ISTAT_DM_RCR);
// #endif
// }

unsigned long dispc_int_active = 0, dispc_int_active_saved = 0;

void inc_task_tryint(void)
{
	dispc_int_active++;
	// at32_led_toggle(LED2);
}

unsigned long get_task_tryint(void)
{
	return dispc_int_active;
}

unsigned long get_task_tryint_saved(void)
{
	return dispc_int_active_saved;
}

void do_task_tryint_saved(void)
{
	dispc_int_active_saved = dispc_int_active;
}

// #if POLL_ON == POLL_ON_RXPADIFF //(DM_ETH_DEBUG_MODE || (POLL_ON == POLL_ON_RXPADIFF)) && 1
uint16_t wrpadiff(uint16_t rwpa_s, uint16_t rwpa_e)
{
	return (rwpa_e >= rwpa_s) ? rwpa_e - rwpa_s : (rwpa_e + 0x4000 - 0xc00) - rwpa_s;
}
// #endif

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

void sprint_hex_dump0(int head_space, int titledn, char *prefix_str,
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
			// unsigned char *
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
			// else
			//	printf("\r\n");
		}
	}
	//}
	// #undef printf
	// #define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

/* print log
 */

#if DM_ETH_DEBUG_MODE
// void dm_eth_input_hexdump_reset(void) {
//	link_log_reset_allow_num = 0;
// }

int link_log_reset_allow_num = 0;
const int rx_modle_log_reset_allow_num = 3;
#define limit_len(n, nTP) ((n <= nTP) ? n : nTP)

void dm_eth_input_hexdump(const void *buf, size_t len)
{
#if 1
// int get_tcpip_thread_state(void);
// void set_tcpip_thread_state(int state); //.temp //extern int tcpip_thread_init;
#endif
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
