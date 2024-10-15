#ifndef __DM9051_ETH_DEBUG_H
#define __DM9051_ETH_DEBUG_H
//#include "dm9051.h"
#include "../identify_opts.h"
#include "../eth_print_opts.h"
#include "dm9051_debug.h"

//#if freeRTOS
//#endif

#if !qprint //total
	#define TASK_DM9051_DEBUGF(mux, headstr, message)
	#define TASK_DM9051_DEBUGK(mux, fmt)
#elif !drv_print //section
	#define TASK_DM9051_DEBUGF(mux, headstr, message)
	#define TASK_DM9051_DEBUGK(mux, fmt)
#else //#elif PRINT_SEMA == SEMA_ON //(freeRTOS)
	#define TASK_DM9051_DEBUGF(mux, headstr, message) do { \
				TASK_DEBUGF(headstr, message); \
			} while(0)
	#define TASK_DM9051_DEBUGK(mux, fmt) do { \
				LIST_DEBUGF(PRINTK_ON, fmt); \
			} while(0)
#endif

#define ESSENTIAL_DM9051_DEBUGF(headstr, message) \
	do { \
		char *hs = headstr; \
		if (hs) \
			printf(hs); \
		TASK_DM9051_DRIVER_DIAG(message); \
	} while(0)

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

//#if !qprint //total
//#elif !drv_print //section
//#else //others
//#endif
#if !drv_print //section
#define	printf(fmt, ...)
#else //others
#define	printf(fmt, ...) printf("%s" fmt, PRINT_INFO, ##__VA_ARGS__)
//#define printf(fmt, ...) TASK_DM9051_DEBUGF(PRINT_SEMA, PRINT_DRV, (fmt, ##__VA_ARGS__))
#endif
	
#endif //__DM9051_ETH_DEBUG_H
