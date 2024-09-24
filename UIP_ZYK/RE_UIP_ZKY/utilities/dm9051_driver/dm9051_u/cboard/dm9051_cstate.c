#include "stdio.h"
#include "cboard/dm9051_lw_mcu_default_IN.h"
#include "dm9051_cstate.h"

#if 0
 #define CB_TYPE		0
 #include "cb_types2.h"

 #define CB_TYPE		1
 #include "cb_types2.h"

 #define CB_TYPE		2
 #include "cb_types2.h"
#endif

/*
 * cboard_trace_irq_flow
 */

//uint16_t irqst = 0;
//uint16_t DM_GET_FIELD_irqst(void) {
//	return irqst;
//}

uint16_t irqstx = 0;
uint16_t identified_irq_stat(void) {
	return irqstx;
	//return GET_CSTATE(irqst);
}

void deidentify_irq_stat(uint16_t bitflg) {
	irqstx &= ~bitflg;
	//SET_CSTATE(irqst, GET_CSTATE(irqst) & ~bitflg);
}
void identify_irq_stat(uint16_t bitflg) {
	irqstx |= bitflg;
	//SET_CSTATE(irqst, GET_CSTATE(irqst) | bitflg);
}

void trace_irq_stat(uint16_t bitflg) {
	char istat_term[22];
	switch(bitflg) {
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
		case ISTAT_DM_ISR_CLR:
			sprintf(istat_term, "(ISR.pr, clr, INT-pin Hi)");
			break;
		case ISTAT_IRQ_NOW:
		default:
			istat_term[0] = 0;
			break;
	}
	
	if (get_task_tryint() > 10 && 
		(bitflg == ISTAT_IRQ_NOW || 
		bitflg == ISTAT_IRQ_NOW2 || 
		bitflg == ISTAT_DM_ISR_CLR))
		return;
		
	printf("[irq_stat]:                   ------------> irqst= %02x on add %02x %s\r\n", identified_irq_stat(), bitflg, istat_term);
}

//void trace_dm9051_set_recv(void); //trace
//void trace_dm9051_set_recv(void) {
//#ifdef DM9051_DRIVER_INTERRUPT
//	trace_irq_stat(ISTAT_DM_IMR);
//	trace_irq_stat(ISTAT_DM_RCR);
//#endif
//}

unsigned long dispc_int_active = 0, dispc_int_active_saved = 0;

void inc_task_tryint(void) {
	dispc_int_active++;
	//at32_led_toggle(LED2);
}

unsigned long get_task_tryint(void) {
	return dispc_int_active;
}

unsigned long get_task_tryint_saved(void) {
	return dispc_int_active_saved;
}

void do_task_tryint_saved(void) {
	dispc_int_active_saved = dispc_int_active;
}

/*
 * cboard_print_hex
 */

#define kkmin(a, b) (a < b) ? a : b
			
static void printf_space(int n) {
	while (n--)
		printf("%c", ' ');
}

static int printf_space_init(size_t tlen) {
	char cspace[16];
	int n = sprintf(cspace, "rxlen %4d", tlen);
#if 1
	printf_space(n);
	printf(" %s\r\n", cspace);
#endif
	return n;
}

static int printf_rxlen_head(size_t tlen, int nspc) {
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
//#undef printf
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	//if (useflg) {
		int si, se, titlec = 0;
		int i, linelen, remaining = len; //hs, const eth_class_t *ec = &eclass[10];
		int nspace = 0;
		
		(void) head_space;
		//(void) tlen;

		si = seg_start;
		se = seg_start + len;
		for (i = si; i < se; i += rowsize) {
			//unsigned
			char linebuf[(12 * 3) + (3 * 16) + 1 + 32]; //here!
			
			nspace = printf_rxlen_head(tlen, nspace);

			linelen = kkmin(remaining, rowsize);
			remaining -= rowsize;
			do {
				const uint8_t *ptr = buf;
				int j;
				//unsigned char *
				int nb = 0;
				for (j=0; j< linelen && (size_t) nb < sizeof(linebuf); j++) {
					if (j && !(j%8))
						nb += snprintf(linebuf+nb, sizeof(linebuf)-nb, " ");
					if (((rowsize>>1)!=8) && !(j%(rowsize>>1)))
						nb += snprintf(linebuf+nb, sizeof(linebuf)-nb, " ");

					nb += snprintf(linebuf+nb, sizeof(linebuf)-nb, "%02x ", *(ptr + i + j));
				}
			} while(0);

			//.nspace = printf_rxlen_head(tlen, nspace);

#if 0
			hs = head_space;
			while(hs--)
#endif
				printf(" ");

			if (prefix_str) {
				printf("(%s) %.3x %s", prefix_str, i, linebuf); //"%s", ec->str //CHECK (XXX >> )
				while (titledn) {
					titledn--;
					prefix_str[titlec++] = ' ';
				}
			} else {
				printf("(dm9 xfer) %.3x %s", i, linebuf); //"%s", ec->str
			}

			if ((i+rowsize) < se)
				printf("\r\n");
			else {
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
				//else
				//	printf("\r\n");
			}
		}
	//}
//#undef printf
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}
