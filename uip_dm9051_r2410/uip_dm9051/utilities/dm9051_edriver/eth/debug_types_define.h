//.#ifndef __DM_TYPES3_H
//.#define __DM_TYPES3_H

#if DM_ETH_DEBUG_MODE

#if DM_DEBUG_TYPE == 1
static volatile unsigned long dispc_int_active = 0;
void inc_interrupt_count(void);
unsigned long get_interrupt_count(void);
#endif

#if DM_DEBUG_TYPE == 2
/* Interrupt Tracking */
void inc_interrupt_count(void)
{
    dispc_int_active++;
}

unsigned long get_interrupt_count(void)
{
    return dispc_int_active;
}
#endif

#undef DM_DEBUG_TYPE
#endif
//.#endif //__DM_TYPES3_H
