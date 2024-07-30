/**
 * @brief Macro for printing debug messages.
 *
 * @Function printf re-directed.
 *
 * @Usage include header file and re-define printf.
 *
 *		(1)	#include "../freertos_tasks_debug.h"
 *		(2) #define printf(fmt, ...) _TASK_DM9051_DEBUGF(TASK_DM9051_RX, headstr, (fmt, ##__VA_ARGS__))
 *
 * @param whether does message to be printed.
 *
 *		Define 'TASK_DM9051_RX' to be 1 to print-out message, 0 to mask the print-out.
 *		This is defined in this file reading "freertos_tasks_debug.h" right now.
 */

/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __FREERTOS_TASKS_DEBUG_H
#define __FREERTOS_TASKS_DEBUG_H
#include "stdint.h"
#include <string.h>
#include "stdio.h"
//.#include "../dm9051_lw_usr_default.h"

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum {
	SEMA_OFF = 0,
	SEMA_ON,
} sema_tag_t;

#define DM9051_DRIVER_DEBUG

#ifdef DM9051_DRIVER_DEBUG
	#define TASK_DM9051_DRIVER_DIAG(x)	do {printf x;} while(0) //this is also in "lwip/arch.h"
	#define TASK_DM9051_DRV_DIAG(x)		do {printf x;} while(0) //this is also in "lwip/arch.h"

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
	
	/**
	 * @brief Macro for printing debug messages.
	 *
	 * This macro is used to print debug messages based on the specified debug level.
	 * It checks if the debug flag is enabled, the debug types are enabled, and the debug level is within the minimum level.
	 * If all conditions are met, it calls the TASK_DM9051_DRIVER_DIAG macro to print the debug message.
	 *
	 * @param debug The debug flag.
	 * @param message The debug message to be printed.
	 */

	// main.c
	// #include "semphr.h" // Include the header file for FreeRTOS semaphore
	// // Declare the mutex handle globally so it can be accessed in different tasks
	// SemaphoreHandle_t mutex_print;

	// int main(void)
	// {
	// 	// ...

	// 	// Create a mutex
	// 	mutex_print = xSemaphoreCreateMutex();

	// 	if (mutex_print == NULL) {
	// 		printf("Mutex could not be created.\r\n");
	// 		while(1); // Handle the error, e.g., by entering an infinite loop
	// 	}

	// 	// ...

	// 	/* start scheduler */
	// 	vTaskStartScheduler();
	// }

	#if 	freeRTOS_CONF
	#include "semphr.h" //.....
	extern SemaphoreHandle_t mutex_print;
	#define TASK_DM9051_DEBUGF(debug, mux, headstr, message) do { \
			if (tasks_dm9051_mutex_begin(mux) == pdTRUE) { \
					if ( debug ) { \
							char *hs = headstr; \
							if (hs) \
								printf(hs); \
							TASK_DM9051_DRIVER_DIAG(message); \
					} \
					tasks_dm9051_mutex_end(mux); \
			}} while(0)

//	#define TASK_DM9051_DBGF(debug, mux, message) do {
//			if (tasks_dm9051_mutex_begin(mux) == pdTRUE) {
//					if ( debug ) {
//							TASK_DM9051_DRV_DIAG(message);
//					}
//					tasks_dm9051_mutex_end(mux);
//			}} while(0)
	#define TASK_DM9051_DBGF(debug, mux, message) do { \
			if (tasks_dm9051_mutex_begin(mux) == pdTRUE) { \
					if ( debug ) { \
							TASK_DM9051_DRV_DIAG(message); \
					} \
					tasks_dm9051_mutex_end(mux); \
			}} while(0)
	#else
	#define TASK_DM9051_DEBUGF(debug, mux, headstr, message) do { \
					if ( debug ) { \
							TASK_DM9051_DRIVER_DIAG(message); \
					}} while(0)
	#endif
#else
	#define TASK_DM9051_DEBUGF(debug, mux, headstr, message)
#endif

/**
 * @file freertos_tasks_debug.h
 *
 * This file contains the definitions and macros used for debugging the freertos printf function.
 */

void tasks_dm9051_debug_init(void);

#if 	freeRTOS_CONF
BaseType_t tasks_dm9051_mutex_begin(sema_tag_t mux);
void tasks_dm9051_mutex_end(sema_tag_t mux);
#endif

//[printk()]

#if 1
#define printk printf
#else
void fputc_dbg(uint16_t ch);
#define printk(fmt) do { /*TASK_DM9051_DBGF(TASK_SEMAPHORE_ETHERNETIF_ON, SEMA_OFF, fmt) */ \
			if (tasks_dm9051_mutex_begin(SEMA_OFF) == pdTRUE) { \
					if ( TASK_SEMAPHORE_ETHERNETIF_ON ) { \
						/*TASK_DM9051_DRV_DIAG(fmt);= */ \
						char *p = fmt; \
						while(*p) { \
							fputc_dbg(*p++); \
						} \
					} \
					tasks_dm9051_mutex_end(SEMA_OFF); \
			}} while(0)
#endif

#ifdef __cplusplus
}
#endif

#endif //__FREERTOS_TASKS_DEBUG_H
