#ifndef __DM9051_RTOS_SYS_ARCH__
#define __DM9051_RTOS_SYS_ARCH__
#include "dm9051opts.h"

typedef enum {
	SEMA_OFF = 0,
	SEMA_ON,
} sema_tag_t;

#if freeRTOS
#include "semphr.h"
xSemaphoreHandle mutex_create_start(void);
void sys_mutex_lock_start(xSemaphoreHandle *sema_p);
void sys_mutex_unlock_end(xSemaphoreHandle *sema_p);

extern xSemaphoreHandle mutex_print;
#endif

#if freeRTOS
/** The global semaphore to lock the stack. */
//extern sys_mutex_t lock_dm9051_core;
//# define LOCK_TCPIP_COREx()     sys_mutex_lock(&lock_dm9051_core)
//# define ULOCK_TCPIP_COREx()   sys_mutex_unlock(&lock_dm9051_core)
//# else
//# define LOCK_TCPIP_COREx()	//empty test
//# define ULOCK_TCPIP_COREx()	//empty test
#endif

//.printk
void fputc_dbg(uint16_t ch); //[printk()]

xSemaphoreHandle tasks_dm9051_debug_init(void);
BaseType_t tasks_dm9051_mutex_begin(sema_tag_t mux, xSemaphoreHandle *sema_p);
void tasks_dm9051_mutex_end(sema_tag_t mux, xSemaphoreHandle *sema_p);

void freertos_task_tryint(void);
int freertos_task_tryDisplay(char *head, unsigned long base_countf, unsigned long base_count_value, unsigned long *dispp);

#endif //__DM9051_RTOS_SYS_ARCH__
