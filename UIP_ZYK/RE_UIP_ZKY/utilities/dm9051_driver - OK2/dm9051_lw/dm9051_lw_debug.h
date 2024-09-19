/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __DM9051_LW_DEBUG_H
#define __DM9051_LW_DEBUG_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdio.h"
// #include <stdint.h>
#include "dm9051_rtos.h" //=dm9051_rtos.h

/**
 * @file dm9051_lw.h
 * @brief Header file for the DM9051 driver in the lwIP TCP/IP stack.
 *
 * This file contains the definitions and macros used for debugging the DM9051 driver.
 * It provides options to enable or disable debugging messages based on different debug levels.
 */
//#ifndef DM9051_DRIVER_DIAG
#define DM9051_DRIVER_HEAD(x) do {printf x;} while(0)
#define DM9051_DRIVER_LOAD(x) do {printf x;} while(0)
#define DM9051_DRIVER_DIAG(x) do {printf x;} while(0)
//#else
//#define DM9051_DRIVER_DIAG(x)
//#endif
//#define DM9051_DRIVER_DIAG1(x) do {printf x;} while(0)

#define DM9051_DRIVER_DEBUG

#ifdef DM9051_DRIVER_DEBUG
	#define DM9051_LW_CONF            0  //1
  #define DM9051_LW_LOG               0 //1

  #define DM9051_TRACE_DEBUG_OFF      0
  #define DM9051_TRACE_DEBUG_ON       1


	#define DM9051_DBG_ON              0x01
	#define DM9051_DBG_OFF             0x00
	#define DM9051_DBG_TYPES_ON        0x01
	#define DM9051_DBG_TYPES_OFF       0x00
	#define DM9051_DBG_MIN_LEVEL       0x00
	#define DM9051_DBG_LEVEL           0x03

	#define LwHeadSTR "[Lw] "

	/**
	 * @brief Macro for printing debug messages.
	 *
	 * This macro is used to print debug messages based on the specified debug level.
	 * It checks if the debug flag is enabled, the debug types are enabled, and the debug level is within the minimum level.
	 * If all conditions are met, it calls the DM9051_DRIVER_DIAG macro to print the debug message.
	 *
	 * @param debug The debug flag.
	 * @param message The debug message to be printed.
	 */
	#define DM9051_DEBUGF(debug, message) do { \
			if ( \
				((debug) & DM9051_DBG_ON) && \
				((debug) & DM9051_DBG_TYPES_ON) && \
				(DM9051_DBG_LEVEL >= DM9051_DBG_MIN_LEVEL)) { \
				printf(LwHeadSTR); \
				/*DM9051_DRIVER_HEAD(headSTR);*/ \
				/*DM9051_DRIVER_DIAG(headSTR);*/ \
				DM9051_DRIVER_DIAG(message); \
			}} while(0)
	#define DM9051_DEBUGK(debug, message) do { \
			if ( \
				((debug) & DM9051_DBG_ON) && \
				((debug) & DM9051_DBG_TYPES_ON) && \
				(DM9051_DBG_LEVEL >= DM9051_DBG_MIN_LEVEL)) { \
				/*DM9051_DRIVER_HEAD(headSTR);*/ \
				/*DM9051_DRIVER_DIAG(headSTR);*/ \
				DM9051_DRIVER_LOAD(message); \
			}} while(0)
	#define printk_lw_debug(fmt, ...) DM9051_DEBUGK(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
#else //DM9051_DRIVER_DEBUG
	#define DM9051_DEBUGF(debug, message)
	#define DM9051_DEBUGK(debug, message)
	#define printk_lw_debug(fmt, ...)
#endif //DM9051_DRIVER_DEBUG

// example usage: DM9051_DEBUGF(DM9051_DBG_ON, ("dm9051_lw: %s: %d\n", __func__, __LINE__));
// example usage: DM9051_DEBUGF(DM9051_LW_CONF, ("dm9051_lw: %s: %d\n", __func__, __LINE__));

#define FREE_TASKS_DEBUG

#ifdef FREE_TASKS_DEBUG
	/**
	 * @brief Macro for printing debug messages.
	 */
	#define TASK_DM9051_DRIVER_DIAG(x)	do {printf x;} while(0) //this is also in "lwip/arch.h"
	//#define TASK_DM9051_DRV_DIAG(x)		do {printf x;} while(0) //this is also in "lwip/arch.h"
	//#define TASK_DM9051_LIST_DIAG(x)	do {printf x;} while(0) //this is also in "lwip/arch.h"
	#define LIST_DEBUGF(dbg, fmt) do { \
		if (dbg) { \
			char *p = fmt; \
			while(*p) { \
				fputc_dbg(*p++); \
		}} \
	} while(0)

	#if freeRTOS_CONF
		extern BaseType_t mutex_pdbool;

		#define TASK_BEGIN_DEBUGF(mux) \
				mutex_pdbool = tasks_dm9051_mutex_begin(mux, &mutex_print)
		#define TASK_END_DEBUGF(mux) \
				if (mutex_pdbool) \
					tasks_dm9051_mutex_end(mux, &mutex_print); \
				mutex_pdbool = pdFALSE
		#define TASK_DEBUGF(debug, headstr, message) do { \
					if (mutex_pdbool) { \
					if ( debug ) { \
						char *hs = headstr; \
						if (hs) \
							printf(hs); \
						TASK_DM9051_DRIVER_DIAG(message); \
					}} \
				} while(0)
	#endif
#endif //FREE_TASKS_DEBUG

#ifdef FREE_TASKS_DEBUG
	#define TASK_DM9051_RX              1
	#define TASK_DM9051_TX          	1

	#define TASK_SEMAPHORE_ETHERNETIF_OFF	0
	#define TASK_SEMAPHORE_ETHERNETIF_ON	1
	//#define TASK_SEMAPHORE_ETHERNETIF		0

	#define TASK_SEMAPHORE_AT32_DM		1 //1
	#define TASK_SEMAPHORE_TCPOUT      	0 //1
	#define TASK_SEMAPHORE_HEX_DUMP		1
	
	#define TASK_SEMAPHORE_TX_THREAD    0  //1
	
	#define TASK_SEMAPHORE_MAIN_OFF     0
	#define TASK_SEMAPHORE_MAIN_ON     	1
	//#define TASK_SEMAPHORE_MAIN      	0
	
	#define TASK_SEMAPHORE_NETCONF_OFF	0
	#define TASK_SEMAPHORE_NETCONF_ON	1

	#define TASK_SEMAPHORE_DOOWN_OFF    0
	#define TASK_SEMAPHORE_DOOWN_ON	    1
	//#define TASK_SEMAPHORE_DOOWN      1

	#define PRINTK_OFF					0
	#define PRINTK_ON					1

	#if freeRTOS_CONF
		#define TASK_DM9051_DEBUGF(debug, mux, headstr, message) do { \
				TASK_BEGIN_DEBUGF(mux); \
				TASK_DEBUGF(debug, headstr, message); \
				TASK_END_DEBUGF(mux); \
				/*if (tasks_dm9051_mutex_begin(mux) == pdTRUE) {*/ \
						/*if ( debug ) {*/ \
								/*char *hs = headstr;*/ \
								/*if (hs)*/ \
									/*printf(hs);*/ \
								/*TASK_DM9051_DRIVER_DIAG(message);*/ \
						/*}*/ \
						/*tasks_dm9051_mutex_end(mux);*/ \
				/*}*/ \
				} while(0)
		#define printk(fmt) do { \
				TASK_BEGIN_DEBUGF(SEMA_ON); \
				LIST_DEBUGF(PRINTK_ON, fmt); \
				TASK_END_DEBUGF(SEMA_ON); \
				/*if (tasks_dm9051_mutex_begin(SEMA_ON) == pdTRUE) {*/ \
						/*if ( PRINTK_ON ) {*/ \
							/*char *p = fmt;*/ \
							/*while(*p) {*/ \
								/*fputc_dbg(*p++);*/ \
							/*}*/ \
						/*}*/ \
						/*tasks_dm9051_mutex_end(SEMA_ON);*/ \
						/*}*/ \
				} while(0)
	#else
		#define TASK_DM9051_DEBUGF(debug, mux, headstr, message) do { \
				if ( debug ) { \
						TASK_DM9051_DRIVER_DIAG(message); \
				}} while(0)
		#define printk(fmt) do { \
					LIST_DEBUGF(PRINTK_ON, fmt); \
				} while(0)
	#endif
#else //FREE_TASKS_DEBUG
	#define TASK_DM9051_DEBUGF(debug, mux, headstr, message)
#endif //FREE_TASKS_DEBUG

#ifdef __cplusplus
}
#endif

#endif //__DM9051_LW_DEBUG_H
