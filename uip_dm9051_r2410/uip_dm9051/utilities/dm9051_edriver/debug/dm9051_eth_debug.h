#ifndef __DM9051_ETH_DEBUG_H
#define __DM9051_ETH_DEBUG_H

//#include "print_eth_def.h"
#include "dbg_opts.h"
#include "dbg_def.h"

//#if !qprint //total
//	#define TASK_DM9051_DEBUGF(headstr, message)
//	#define TASK_DM9051_DEBUGK(fmt)
//#elif !drv_print //section
//	#define TASK_DM9051_DEBUGF(headstr, message)
//	#define TASK_DM9051_DEBUGK(fmt)
//#else //#elif PRINT_SEMA == SEMA_ON //(freeRTOS)
//	#define TASK_DM9051_DEBUGF(headstr, message) do { \
//				ESSENTIAL_DM9051_DEBUGF(headstr, message); \
//			} while(0)
//	#define TASK_DM9051_DEBUGK(fmt) do { \
//				LIST_DEBUGF(PRINTK_ON, fmt); \
//			} while(0)
//#endif

#if drv_print
#define printf(fmt, ...) do { \
            char debug_msg[256]; \
            snprintf(debug_msg, sizeof(debug_msg), "%s" fmt, PRINT_INFO, ##__VA_ARGS__); \
			dm9051_eth_debug_handler(LOG_LEVEL, debug_msg); \
		} while(0)
#define	printk	printkey
#else //others
#define	printf(fmt, ...)
#define	printk(fmt, ...)
#endif

#endif //__DM9051_ETH_DEBUG_H
