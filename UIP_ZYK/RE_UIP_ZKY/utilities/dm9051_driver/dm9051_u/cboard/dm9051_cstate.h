#ifndef __DM9051_CSTATE_H
#define __DM9051_CSTATE_H

#define ISTAT_IRQ_CFG		(1 << 0)
#define ISTAT_IRQ_ENAB		(1 << 1)
#define ISTAT_DM_IMR		(1 << 2)
#define ISTAT_DM_RCR		(1 << 3)

#define ISTAT_LOW_TRIGGER	(1 << 4)
#define ISTAT_LOW_ACTIVE	(1 << 5)

#define ISTAT_IRQ_NOW2		(1 << 6)
#define ISTAT_IRQ_NOW		(1 << 7) //not used.

#define	ISTAT_DM_ISR_CLR	(1 << 8)

void deidentify_irq_stat(uint16_t bitflg);

void identify_irq_stat(uint16_t bitflg);
void trace_irq_stat(uint16_t bitflg);

uint16_t identified_irq_stat(void);

void inc_task_tryint(void);
unsigned long get_task_tryint(void);
unsigned long get_task_tryint_saved(void);
void do_task_tryint_saved(void);
	
void sprint_hex_dump0(int head_space, int titledn, char *prefix_str,
			size_t tlen, int rowsize, const void *buf, int seg_start, size_t len, /*int useflg*/ int cast_lf);

#endif //__DM9051_CSTATE_H
