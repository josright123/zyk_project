#ifndef __DM9051_DEBUG_H
#define __DM9051_DEBUG_H

#include <stdio.h>
#include "debug/print_opts.h"

// Debug levels
typedef enum {
    DM9051_ETH_DEBUG_LEVEL_ERROR = 0,
    DM9051_ETH_DEBUG_LEVEL_DEBUG,
    DM9051_ETH_DEBUG_LEVEL_WARN,
    DM9051_ETH_DEBUG_LEVEL_INFO,
} dm9051_eth_debug_level_t;

#define LOG_LEVEL DM9051_ETH_DEBUG_LEVEL_DEBUG

/* debug definition, dbg_def~
 * Implementation of the debug handler
 */
#if 1
//inline void _dm9051_eth_debug_handler(dm9051_eth_debug_level_t level, const char *message) 
//{
//    const char *level_str;
//    switch (level) {
//        case DM9051_ETH_DEBUG_LEVEL_ERROR: level_str = "ERROR"; break;
//        case DM9051_ETH_DEBUG_LEVEL_WARN:  level_str = "WARN";  break;
//        case DM9051_ETH_DEBUG_LEVEL_INFO:  level_str = "INFO";  break;
//        case DM9051_ETH_DEBUG_LEVEL_DEBUG: level_str = "DEBUG"; break;
//        default:                           level_str = "UNKNOWN";
//    }
//	
//	if (level >= LOG_LEVEL)
//		printf("[%s] %s", level_str, message);	
//}
#define dm9051_eth_debug_handler(level, message) do { \
    const char *level_str; \
    switch (level) { \
        case DM9051_ETH_DEBUG_LEVEL_ERROR: level_str = "ERROR"; break; \
        case DM9051_ETH_DEBUG_LEVEL_WARN:  level_str = "WARN";  break; \
        case DM9051_ETH_DEBUG_LEVEL_INFO:  level_str = "INFO";  break; \
        case DM9051_ETH_DEBUG_LEVEL_DEBUG: level_str = "DEBUG"; break; \
        default:                           level_str = "UNKNOWN"; \
    } \
	if (level >= LOG_LEVEL) \
		printf("[%s] %s", level_str, message);	 \
} while(0)
#endif

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

//#define TASK_DM9051_DRIVER_DIAG(x)	do {printf x;} while(0) //(this is also in "lwip/arch.h")

//#define ESSENTIAL_DM9051_DEBUGF(headstr, message) do { \
//		char *hs = headstr; \
//		if (hs) \
//			printf(hs); \
//		TASK_DM9051_DRIVER_DIAG(message); \
//	} while(0)

//void fputc_dbg(uint16_t ch); //[_TASK_DM9051_DEBUGK()]
//#define LIST_DEBUGF(dbg, fmt) do { \
//	if (dbg) { \
//		char *p = fmt; \
//		while(*p) { \
//			fputc_dbg(*p++); \
//	}} \
//} while(0)

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
