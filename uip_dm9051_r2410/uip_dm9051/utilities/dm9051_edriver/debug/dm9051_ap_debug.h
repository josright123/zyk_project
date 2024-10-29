/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __DM9051_AP_DEBUG_H
#define __DM9051_AP_DEBUG_H

#include "print_ap_def.h"
#include "dbg_opts.h"
#include "dbg_def.h"

#if ap_print
#define printf(fmt, ...) do { \
            char debug_msg[256]; \
            snprintf(debug_msg, sizeof(debug_msg), "%s" fmt, PRINT_INFO_AP, ##__VA_ARGS__); \
			dm9051_eth_debug_handler(LOG_LEVEL, debug_msg); \
		} while(0)
#define	printk	printkey
#else //others
#define	printf(fmt, ...)
#define	printk(fmt, ...)
#endif

//#if !qprint //total
//	#define TASK_DM9051_DEBUGF(headstr, message)
//	#define TASK_DM9051_DEBUGK(fmt)
//#elif !ap_print //section
//	#define TASK_DM9051_DEBUGF(headstr, message)
//	#define TASK_DM9051_DEBUGK(fmt)
////#elif PRINT_SEMA == SEMA_ON
////	#define TASK_DM9051_DEBUGF(headstr, message) do { \
////			if (print_mutex_lock_start(qprint, PRINT_SEMA) == pdTRUE) { \
////					ESSENTIAL_DM9051_DEBUGF(headstr, message); \
////					print_mutex_lock_end(qprint, PRINT_SEMA); \
////			} \
////			} while(0)
////	#define TASK_DM9051_DEBUGK(fmt) do { \
////				if (print_mutex_lock_start(qprint, PRINT_SEMA) == pdTRUE) { \
////					LIST_DEBUGF(PRINTK_ON, fmt); \
////					print_mutex_lock_end(qprint, PRINT_SEMA); \
////				} \
////			} while(0)
//#else
//	#define TASK_DM9051_DEBUGF(headstr, message) do { \
//				TASK_DM9051_DRIVER_DIAG(message); \
//			} while(0)
//	#define TASK_DM9051_DEBUGK(fmt) do { \
//				LIST_DEBUGF(PRINTK_ON, fmt); \
//			} while(0)
//#endif

#endif //__DM9051_AP_DEBUG_H
