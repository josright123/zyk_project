/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __DM9051_LW_DEBUG_H
#define __DM9051_LW_DEBUG_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "lwip/netif.h" //#include "lwip/err.h"
#include "dm9051.h"
#include "identify/dm_eth_rt_opts.h"
#include "identify/dm_eth_opts.h"
#include "../eth/dm_eth_api.h"
#include "../eth_rt/dm_eth_task_mut_print.h"

//[Re-directed:]
#define TASK_DM9051_DRIVER_DIAG(x)	do {printf x;} while(0) //(this is also in "lwip/arch.h")
#define ORIGINAL_DM9051_DEBUGF(headstr, message) \
	do { \
		char *hs = headstr; \
		if (hs) \
			printf(hs); \
		TASK_DM9051_DRIVER_DIAG(message); \
	} while(0)
	
//[Re-directed:]
//.#define DM9051_DRIVER_LOAD(x) do {printf x;} while(0)
//.#define TASK_DM9051_DEBUGF(debug, mux, headstr, message)	DM9051_DRIVER_LOAD(message)

	#define TASK_SEMAPHORE_NETCONF_OFF		0
	#define TASK_SEMAPHORE_NETCONF_ON			1
				
	#define TASK_SEMAPHORE_TCPOUT      		0 //1

	#define TASK_SEMAPHORE_ETHERNETIF_OFF	0
	#define TASK_SEMAPHORE_ETHERNETIF_ON	1
	
	#define TASK_SEMAPHORE_TX_THREAD    	0 //1

	#define PRINTK_OFF										0
	#define PRINTK_ON											1

	#if freeRTOS
		#define TASK_DEBUGF(debug, headstr, message) do { \
					if ( debug ) { \
						char *hs = headstr; \
						if (hs) \
							printf(hs); \
						TASK_DM9051_DRIVER_DIAG(message); \
					} \
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
				
	#if freeRTOS
		#define TASK_DM9051_DEBUGF(debug, mux, headstr, message) do { \
				if (tasks_dm9051_mutex_begin(qprint, mux, &mutex_print) == pdTRUE) { \
						TASK_DEBUGF(debug, headstr, message); \
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
		#define TASK_DM9051_DEBUGF(debug, mux, headstr, message) do { \
				if ( debug ) { \
						TASK_DM9051_DRIVER_DIAG(message); \
				}} while(0)
		#define TASK_DM9051_DEBUGK(mux, fmt) do { \
					LIST_DEBUGF(PRINTK_ON, fmt); \
				} while(0)
	#endif
		
//-

int freertos_task_tryDisplay(char *head, unsigned long base_countf, unsigned long base_count_value, unsigned long *dispp);  //OF DM9051_RTOS.H
void freertos_task_trypoll_inpkt(void);
			
void freertos_task_clearpoll_event(void);

#ifdef __cplusplus
}
#endif

#endif //__DM9051_LW_DEBUG_H
