/* define to prevent recursive inclusion -------------------------------------*/
#ifndef ___DM_ETH_DEBUG_H__
#define ___DM_ETH_DEBUG_H__
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "../identify_opts.h"
#include "../dm_eth_opts.h"

#if freeRTOS
//extern sys_mutex_t lock_dm9051_core;
//# define LOCK_TCPIP_COREx()     sys_mutex_lock(&lock_dm9051_core)
//# define ULOCK_TCPIP_COREx()   sys_mutex_unlock(&lock_dm9051_core)
//# else
//# define LOCK_TCPIP_COREx()	//empty test
//# define ULOCK_TCPIP_COREx()	//empty test

void dm_eth_create_mutex_print(void);

xSemaphoreHandle tasks_dm9051_debug_init(void);
BaseType_t tasks_dm9051_mutex_begin(int qpnt, enum sema_tag_t mux, xSemaphoreHandle *sema_p);
void tasks_dm9051_mutex_end(int qpnt, enum sema_tag_t mux, xSemaphoreHandle *sema_p);
#endif

#if qprint && (PRINT_SEMA == SEMA_ON)
extern xSemaphoreHandle mutex_print;
#endif

#endif //___DM_ETH_DEBUG_H__
