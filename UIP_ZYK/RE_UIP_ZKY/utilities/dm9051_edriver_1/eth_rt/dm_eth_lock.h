/* define to prevent recursive inclusion -------------------------------------*/
#ifndef ___DM_ETH_LOCK_H__
#define ___DM_ETH_LOCK_H__
#include "identify/dm_eth_opts.h"

#define DM_ETH_MUTEX_CFG(expression, msg, sem, handler) do { if ((expression)) { \
	/*printf("%s", msg);*/ \
	sem = handler; \
	if (!sem) { \
		printf("dm9051, failed to '%s'\r\n", msg); \
		while(1); /*Handle the error, e.g., by entering an infinite loop*/ \
		/*return NULL;*/ \
	} \
	}} while(0)

//[dm9051_rtos.c]
#if freeRTOS
	#define DM9051_MUTEX_OPS(expression, handler) do { if ((expression)) { \
		(handler); \
		}} while(0)
#else
	#define DM9051_MUTEX_OPS(expression, handler)
#endif
	
//static xSemaphoreHandle mutex_create_start(void);
//static void sys_mutex_lock_start(xSemaphoreHandle *sema_p);
//static void sys_mutex_unlock_end(xSemaphoreHandle *sema_p);

#endif //___DM_ETH_LOCK_H__
