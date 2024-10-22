/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __DM9051_LW_DEBUG_H
#define __DM9051_LW_DEBUG_H

#ifdef __cplusplus
 extern "C" {
#endif
//#include "lwip/netif.h" //#include "lwip/err.h"
//#include "../config/dm_eth_opts.h" to be later
//.#include "dm9051.h"
#include "config/identify_opts.h"
#include "config/dm_eth_opts.h" //#include "../config/dm_ap_opts.h"
#include "config/ap_print_opts.h" //#include "../config/rt_print_opts.h"
				
	//#define TASK_SEMAPHORE_TCPOUT      		0 //1
	//#define TASK_SEMAPHORE_TX_THREAD    	0 //1
#include "dm9051_debug.h"

	#define PRINTK_OFF										0
	#define PRINTK_ON											1

//#include "utils/dm9051_mut_print.h"
/* Only need for 'rt' application
 */
//#include "utils/task_msg.h"

#if !qprint //total
	#define TASK_DM9051_DEBUGF(headstr, message)
	#define TASK_DM9051_DEBUGK(fmt)
#elif !ap_print //section
	#define TASK_DM9051_DEBUGF(headstr, message)
	#define TASK_DM9051_DEBUGK(fmt)
//#elif PRINT_SEMA == SEMA_ON
//	#define TASK_DM9051_DEBUGF(headstr, message) do { \
//			if (print_mutex_lock_start(qprint, PRINT_SEMA) == pdTRUE) { \
//					TASK_DEBUGF(headstr, message); \
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

//[Re-directed:]
#if !qprint //total
#define ORIGINAL_DM9051_DEBUGF(headstr, message)
//[Re-directed:]
//#elif (PRINT_SEMA == SEMA_ON) in case no this attribute.
#else
#define ORIGINAL_DM9051_DEBUGF(headstr, message) \
	do { \
		char *hs = headstr; \
		if (hs) \
			printf(hs); \
		TASK_DM9051_DRIVER_DIAG(message); \
	} while(0)
#endif

#if 1
	void fputc_dbg(uint16_t ch); //[_TASK_DM9051_DEBUGK()]

	#define LIST_DEBUGF(dbg, fmt) do { \
		if (dbg) { \
			char *p = fmt; \
			while(*p) { \
				fputc_dbg(*p++); \
		}} \
	} while(0)
#endif

#ifdef __cplusplus
}
#endif

#endif //__DM9051_LW_DEBUG_H
