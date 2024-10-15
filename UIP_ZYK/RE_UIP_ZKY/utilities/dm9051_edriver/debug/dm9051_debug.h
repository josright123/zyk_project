#ifndef __DM9051_DEBUG_H
#define __DM9051_DEBUG_H

#include <stdio.h>

#define DM9051_DEBUG_LEVEL 3

#if DM9051_DEBUG_LEVEL > 0
#define DM9051_ERROR(fmt, ...) printf("DM9051 ERROR: " fmt "\n", ##__VA_ARGS__)
#else
#define DM9051_ERROR(fmt, ...)
#endif

#if DM9051_DEBUG_LEVEL > 1
#define DM9051_WARN(fmt, ...) printf("DM9051 WARN: " fmt "\n", ##__VA_ARGS__)
#else
#define DM9051_WARN(fmt, ...)
#endif

#if DM9051_DEBUG_LEVEL > 2
#define DM9051_INFO(fmt, ...) printf("DM9051 INFO: " fmt "\n", ##__VA_ARGS__)
#else
#define DM9051_INFO(fmt, ...)
#endif

//void dm9051_print_regs(void);
//void dm9051_print_phy_regs(void);

#define TASK_DM9051_DRIVER_DIAG(x)	do {printf x;} while(0) //(this is also in "lwip/arch.h")

#define TASK_DEBUGF(headstr, message) do { \
			char *hs = headstr; \
			if (hs) \
				printf(hs); \
			TASK_DM9051_DRIVER_DIAG(message); \
		} while(0)

//#define TASK_DM9051_DRIVER_SDIAG(bf,off,x)	do { off += sprintf(bf+off, x);} while(0)

#endif // __DM9051_DEBUG_H
