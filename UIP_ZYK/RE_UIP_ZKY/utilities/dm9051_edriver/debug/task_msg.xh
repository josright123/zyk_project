#ifndef __DM_ETH_TSK_MSG_H__
#define __DM_ETH_TSK_MSG_H__
#include "../identify_opts.h"

//#if freeRTOS
//#endif
#if (qprint && PRINT_SEMA == SEMA_ON) //depend
//=#include "dm_eth_lock.h" //#include "dm9051_rtos.h"
	#define DM_ETH_MUTEX_CFG(expression, msg, sem, handler) do { if ((expression)) { \
	sem = handler; \
	if (!sem) { \
		printf("dm9051, failed to '%s'\r\n", msg); \
		while(1) \
			; /*Handle the error, e.g., or infinite loop*/ \
	} \
	}} while(0)
#else
	#define DM_ETH_MUTEX_CFG(expression, msg, sem, handler)
#endif

#if !qprint //total
	#define TASK_DM9051_DEBUGF(mux, headstr, message)
	#define TASK_DM9051_DEBUGK(mux, fmt)
#elif !ap_print //section
	#define TASK_DM9051_DEBUGF(mux, headstr, message)
	#define TASK_DM9051_DEBUGK(mux, fmt)
#elif PRINT_SEMA == SEMA_ON //(freeRTOS)
	#define TASK_DM9051_DEBUGF(mux, headstr, message) do { \
			if (tasks_dm9051_mutex_begin(qprint, mux, &mutex_print) == pdTRUE) { \
					TASK_DEBUGF(headstr, message); \
					tasks_dm9051_mutex_end(qprint, mux, &mutex_print); \
			} \
			} while(0)
	#define TASK_DM9051_DEBUGK(mux, fmt) do { \
				if (tasks_dm9051_mutex_begin(qprint, mux, &mutex_print) == pdTRUE) { \
					LIST_DEBUGF(PRINTK_ON, fmt); \
					tasks_dm9051_mutex_end(qprint, mux, &mutex_print); \
				} \
			} while(0)
#else
	#define TASK_DM9051_DEBUGF(mux, headstr, message) do { \
				TASK_DM9051_DRIVER_DIAG(message); \
			} while(0)
	#define TASK_DM9051_DEBUGK(mux, fmt) do { \
				LIST_DEBUGF(PRINTK_ON, fmt); \
			} while(0)
#endif
//-
void freertos_task_trypoll_inpkt(void);
void freertos_task_clearpoll_event(void);

#endif //__DM_ETH_TSK_MSG_H__
