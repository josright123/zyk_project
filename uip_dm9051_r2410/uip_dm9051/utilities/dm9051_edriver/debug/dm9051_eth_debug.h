#ifndef __DM9051_ETH_DEBUG_H
#define __DM9051_ETH_DEBUG_H

#include "config/eth_print_opts.h"
#include "config/print_opts.h"
#include "dbg_def.h"

#if !qprint //total
	#define TASK_DM9051_DEBUGF(headstr, message)
	#define TASK_DM9051_DEBUGK(fmt)
#elif !drv_print //section
	#define TASK_DM9051_DEBUGF(headstr, message)
	#define TASK_DM9051_DEBUGK(fmt)
#else //#elif PRINT_SEMA == SEMA_ON //(freeRTOS)
	#define TASK_DM9051_DEBUGF(headstr, message) do { \
				ESSENTIAL_DM9051_DEBUGF(headstr, message); \
			} while(0)
	#define TASK_DM9051_DEBUGK(fmt) do { \
				LIST_DEBUGF(PRINTK_ON, fmt); \
			} while(0)
#endif

#if !drv_print //section
#define	printf(fmt, ...)
#else //others
#define	printf(fmt, ...) printf("%s" fmt, PRINT_INFO, ##__VA_ARGS__)
//#define printf(fmt, ...) TASK_DM9051_DEBUGF(PRINT_DRV, (fmt, ##__VA_ARGS__))
#endif
	
#endif //__DM9051_ETH_DEBUG_H
