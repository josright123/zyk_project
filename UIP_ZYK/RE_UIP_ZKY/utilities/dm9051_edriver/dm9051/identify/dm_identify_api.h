#ifndef __DM_IDEN_API_H__
#define __DM_IDEN_API_H__
#include "../dm9051opts.h"

void inc_interrupt_count(void); //inc_task_tryint(void);
unsigned long get_task_tryint(void);

//[dm9051_cstate.c]
uint16_t wrpadiff(uint16_t rwpa_s, uint16_t rwpa_e);

//[dm_eth_status.c]
#if DM_ETH_DEBUG_MODE
		void dm_eth_input_hexdump_reset(void);
		void dm_eth_input_hexdump(const void *buf, size_t len);
#endif

#endif //__DM_IDEN_API_H__
