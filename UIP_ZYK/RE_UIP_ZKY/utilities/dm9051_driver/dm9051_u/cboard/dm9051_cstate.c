#include "stdio.h"
//#include "dm9051opts.h"
//#include "dm9051_lw.h"
#include "cboard/dm9051_lw_mcu_default_IN.h"
#include "dm9051_cstate.h"

#define CB_TYPE		0
#include "cb_types2.h"

#define CB_TYPE		1
#include "cb_types2.h"

#define CB_TYPE		2
#include "cb_types2.h"

/*
 * cboard_trace_irq_flow
 */

//uint16_t irqst = 0;

void deidentify_irq_stat(uint16_t bitflg) {
	//irqst &= ~bitflg; //return DM_GET_FIELD(uint16_t, irqst);
	SET_CSTATE(irqst, GET_CSTATE(irqst) & ~bitflg); //DM_SET_FIELDirqst(DM_GET_FIELDirqst() & ~bitflg);
}
void identify_irq_stat(uint16_t bitflg) {
//	char istat_term[22];
	//irqst |= bitflg; //DM_SET_FIELD(uint16_t, irqst, DM_GET_FIELD(uint16_t, irqst) | bitflg); //store into dm9051optsex[i].read_chip_id
	SET_CSTATE(irqst, GET_CSTATE(irqst) | bitflg); //DM_SET_FIELDirqst(DM_GET_FIELDirqst() | bitflg);

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
		case ISTAT_IRQ_NOW:
		default:
			istat_term[0] = 0;
			break;
	}
	
	if (get_task_tryint() > 5 && (bitflg == ISTAT_IRQ_NOW || bitflg == ISTAT_IRQ_NOW2))
		return;
		
	printf("[irq_stat]:                   ------------> irqst= %02x on add %02x %s\r\n", identified_irq_stat(), bitflg, istat_term);
}

uint16_t identified_irq_stat(void) {
	//return irqst;
	return GET_CSTATE(irqst);
}
//uint16_t DM_GET_FIELD_irqst(void) {
//	return irqst;
//}

//void trace_dm9051_set_recv(void); //trace
//void trace_dm9051_set_recv(void) {
//#ifdef DRV_INTR_MODE
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
