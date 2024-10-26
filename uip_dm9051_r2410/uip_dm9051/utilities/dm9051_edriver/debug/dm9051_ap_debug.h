/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __DM9051_AP_DEBUG_H
#define __DM9051_AP_DEBUG_H

#include "config/ap_print_opts.h"
#include "config/print_opts.h"
#include "dbg_def.h"

#if !qprint //total
	#define TASK_DM9051_DEBUGF(headstr, message)
	#define TASK_DM9051_DEBUGK(fmt)
#elif !ap_print //section
	#define TASK_DM9051_DEBUGF(headstr, message)
	#define TASK_DM9051_DEBUGK(fmt)
//#elif PRINT_SEMA == SEMA_ON
//	#define TASK_DM9051_DEBUGF(headstr, message) do { \
//			if (print_mutex_lock_start(qprint, PRINT_SEMA) == pdTRUE) { \
//					ESSENTIAL_DM9051_DEBUGF(headstr, message); \
//					print_mutex_lock_end(qprint, PRINT_SEMA); \
//			} \
//			} while(0)
//	#define TASK_DM9051_DEBUGK(fmt) do { \
//				if (print_mutex_lock_start(qprint, PRINT_SEMA) == pdTRUE) { \
//					LIST_DEBUGF(PRINTK_ON, fmt); \
//					print_mutex_lock_end(qprint, PRINT_SEMA); \
//				} \
//			} while(0)
#else
	#define TASK_DM9051_DEBUGF(headstr, message) do { \
				TASK_DM9051_DRIVER_DIAG(message); \
			} while(0)
	#define TASK_DM9051_DEBUGK(fmt) do { \
				LIST_DEBUGF(PRINTK_ON, fmt); \
			} while(0)
#endif

#endif //__DM9051_AP_DEBUG_H
