#ifndef __DM9051_DEBUG_H
#define __DM9051_DEBUG_H

#include <stdio.h>
#include "debug/dbg_opts.h"

//ap
#define	PRINT_SEMA	SEMA_OFF
#define PRINT_INFO_AP	"[iAP] "
#define PRINT_INFO_APIN	"[APIN] "

//rt
#define	PRINT_SEMA_RT	SEMA_ON	//ON to print with sema, OFF without sema.
#define PRINT_INFO_RT	"[iRT]  "

//drv
#define PRINT_INFO	"[DRV] "
#define	PRINT_INFO_IRQ	"[IRQ] "

// Debug levels
typedef enum {
    DM9051_ETH_DEBUG_LEVEL_ERROR = 0,
    DM9051_ETH_DEBUG_LEVEL_DEBUG,
    DM9051_ETH_DEBUG_LEVEL_WARN,
    DM9051_ETH_DEBUG_LEVEL_INFO,
} dm9051_eth_debug_level_t;

#define LOG_LEVEL DM9051_ETH_DEBUG_LEVEL_DEBUG

const char *level_str_impl(dm9051_eth_debug_level_t level);

/* debug definition, dbg_def~
 * Implementation of the debug handler
 */
#define dm9051_eth_debug_handler(level, message) do { \
	if (level >= LOG_LEVEL) { \
		printf("[%s] %s", level_str_impl(level), message);	 \
    } \
} while(0)

//#define DM9051_LOG_LEVEL 3

//#if DM9051_LOG_LEVEL > 0
//#define DM9051_ERROR(fmt, ...) printf("DM9051 ERROR: " fmt "\n", ##__VA_ARGS__)
//#else
//#define DM9051_ERROR(fmt, ...)
//#endif

//#if DM9051_LOG_LEVEL > 1
//#define DM9051_WARN(fmt, ...) printf("DM9051 WARN: " fmt "\n", ##__VA_ARGS__)
//#else
//#define DM9051_WARN(fmt, ...)
//#endif

//#if DM9051_LOG_LEVEL > 2
//#define DM9051_INFO(fmt, ...) printf("DM9051 INFO: " fmt "\n", ##__VA_ARGS__)
//#else
//#define DM9051_INFO(fmt, ...)
//#endif

//#if DM9051_LOG_LEVEL > 3
//#define DM9051_DEBUF_LOG(fmt, ...) printf("DM9051 DEBUG_LOG: " fmt "\n", ##__VA_ARGS__)
//#else
//#define DM9051_DEBUF_LOG(fmt, ...)
//#endif

//void dm9051_print_regs(void);
//void dm9051_print_phy_regs(void);

#if 1
void fputc_dbg(uint16_t ch); //[_TASK_DM9051_DEBUGK()]
#define LIST_DEBUGF(dbg, fmt) do { \
	if (dbg) { \
		char *p = fmt; \
		while(*p) { \
			fputc_dbg(*p++); \
	}} \
} while(0)
#define	printkey(fmt, ...) \
	do { \
		char debug_msg[256]; \
		char *p = debug_msg; \
		snprintf(debug_msg, sizeof(debug_msg), fmt, ##__VA_ARGS__); \
		/*ESSENTIAL_DEBUGF(debug_msg);*/ \
		while(*p) \
			fputc_dbg(*p++); \
	} while(0)
#endif

//#define TASK_DM9051_DRIVER_DIAG(x)	do {printf x;} while(0) //(this is also in "lwip/arch.h")

//#define ESSENTIAL_DM9051_DEBUGF(headstr, message) do { \
//		char *hs = headstr; \
//		if (hs) \
//			printf(hs); \
//		printf message; /*TASK_DM9051_DRIVER_DIAG(message);*/  \
//	} while(0)

//#if !qprint //total [Re-directed:]
//#define ORIGINAL_DM9051_DEBUGF(headstr, message)
////#elif (PRINT_SEMA == SEMA_ON) //in case no this attribute.
//#else
//#define ORIGINAL_DM9051_DEBUGF(headstr, message) \
//	do { \
//		char *hs = headstr; \
//		if (hs) \
//			printf(hs); \
//		TASK_DM9051_DRIVER_DIAG(message); \
//	} while(0)
//#endif

#endif // __DM9051_DEBUG_H
