/* define to prevent recursive inclusion -------------------------------------*/
#ifndef ___DM_ETH_DEBUG_H__
#define ___DM_ETH_DEBUG_H__
#include "dm9051opts.h"

typedef enum {
	SEMA_OFF = 0,
	SEMA_ON,
} sema_tag_t;

#if freeRTOS
//extern sys_mutex_t lock_dm9051_core;
//# define LOCK_TCPIP_COREx()     sys_mutex_lock(&lock_dm9051_core)
//# define ULOCK_TCPIP_COREx()   sys_mutex_unlock(&lock_dm9051_core)
//# else
//# define LOCK_TCPIP_COREx()	//empty test
//# define ULOCK_TCPIP_COREx()	//empty test
xSemaphoreHandle tasks_dm9051_debug_init(void);
BaseType_t tasks_dm9051_mutex_begin(int qpnt, sema_tag_t mux, xSemaphoreHandle *sema_p);
void tasks_dm9051_mutex_end(int qpnt, sema_tag_t mux, xSemaphoreHandle *sema_p);
#endif

extern xSemaphoreHandle mutex_print;
#endif //___DM_ETH_DEBUG_H__
