/**
  **************************************************************************
  * @file     dm_eth_status.c
  * @version  v1.0.0, v1.0.1
  * @date     2024-06-24, 2024-08-12
  * @brief    as particular range control for dm_eth.c
  **************************************************************************
  */
#include "../eth_rt/dm_eth_task_mut_print.h"

#if RX_HANDLER_ACCEPTED_TAKE_PLAIN_SEM || RX_HANDLER_ACCEPTED_TAKE_GENNY_SEM
SemaphoreHandle_t g_dm9051_interrupt_event;

void DM_ETH_RTOS_GIVE_SEMA(void) {
	#if RX_HANDLER_ACCEPTED_TAKE_PLAIN_SEM
		xSemaphoreGive(&g_dm9051_interrupt_event);/* ????? */

	#elif RX_HANDLER_ACCEPTED_TAKE_GENNY_SEM
		sys_sem_signal(&g_dm9051_interrupt_event); //xxx
	#endif
}
BaseType_t DM_ETH_RTOS_TAKE_SEMA(void) {
	return xSemaphoreTake(g_dm9051_interrupt_event, portMAX_DELAY); //...
}
#endif

#if freeRTOS
/* Declare the mutex handle globally so it can be accessed in protect spi sync.
 */
xSemaphoreHandle mutex_print; //............hjmhjm.............

//[dm9051_rtos.c - print mutex]
/* Declare the mutex handle globally so it can be accessed in different tasks
 */
xSemaphoreHandle tasks_dm9051_debug_init(void)
{
  xSemaphoreHandle sema;
  sema = xSemaphoreCreateMutex(); // Create a mutex
  return sema;
}

BaseType_t tasks_dm9051_mutex_begin(int qpnt, sema_tag_t mux, xSemaphoreHandle *sema_p)
{
	if (qpnt && (mux == SEMA_ON))
		return xSemaphoreTake(*sema_p, portMAX_DELAY); //(_mutex_print,)
	return pdFALSE; //pdTRUE;
}

void tasks_dm9051_mutex_end(int qpnt, sema_tag_t mux, xSemaphoreHandle *sema_p)
{
	if (qpnt && (mux == SEMA_ON))
		xSemaphoreGive(*sema_p); //(_mutex_print);
	return;
}
#endif /* freeRTOS */
