/**
  **************************************************************************
  * @file     dm_eth_status.c
  * @version  v1.0.0, v1.0.1
  * @date     2024-06-24, 2024-08-12
  * @brief    as particular range control for dm_eth.c
  **************************************************************************
  */
#include "dm9051_env.h"

#include "dm_eth/dm_eth.h"
#include "dm9051_debug.h" //for - xSemaphoreHandle mutex_print;

#include "cboard/dm9051_cstate.h"

#if DM_ETH_DEBUG_MODE
int link_log_reset_allow_num = 0;
const int rx_modle_log_reset_allow_num = 3;
#define	limit_len(n, nTP)	((n <= nTP) ? n : nTP)

void dm_eth_input_hexdump(const void *buf, size_t len) {
	#if 1
	//int get_tcpip_thread_state(void);
	//void set_tcpip_thread_state(int state); //.temp //extern int tcpip_thread_init;
	#endif
	int titledn = 0;
	DM_UNUSED_ARG(buf);
	DM_UNUSED_ARG(len);
	
	if (link_log_reset_allow_num < rx_modle_log_reset_allow_num) {
			link_log_reset_allow_num++;
			//if (link_log_reset_allow_num == rx_modle_log_reset_allow_num && get_tcpip_thread_state() == 1) {
				//set_tcpip_thread_state(6);
			//}
			sprint_hex_dump0(2, titledn, "dm9 head   <<rx", len, 32, buf, 0, limit_len(len, 66), DM_TRUE);
	}
}
#endif

#if 0
//void dm_eth_input_hexdump_reset(void) {
//	link_log_reset_allow_num = 0;
//}

#if RX_HANDLER_ACCEPTED_TAKE_PLAIN_SEM || RX_HANDLER_ACCEPTED_TAKE_GENNY_SEM
//SemaphoreHandle_t g_dm9051_interrupt_event;

//void DM_ETH_RTOS_GIVE_SEMA(void) {
//	#if RX_HANDLER_ACCEPTED_TAKE_PLAIN_SEM
//		xSemaphoreGive(&g_dm9051_interrupt_event);/* ????? */

//	#elif RX_HANDLER_ACCEPTED_TAKE_GENNY_SEM
//		sys_sem_signal(&g_dm9051_interrupt_event); //xxx
//	#endif
//}
//BaseType_t DM_ETH_RTOS_TAKE_SEMA(void) {
//	return xSemaphoreTake(g_dm9051_interrupt_event, portMAX_DELAY); //...
//}
#endif

#if freeRTOS
//xSemaphoreHandle mutex_print; //...........ghmgtg...
//xSemaphoreHandle tasks_dm9051_debug_init(void)
//{
//  xSemaphoreHandle sema;
//  sema = xSemaphoreCreateMutex(); // Create a mutex
//  return sema;
//}

//BaseType_t tasks_dm9051_mutex_begin(int qpnt, sema_tag_t mux, xSemaphoreHandle *sema_p)
//{
//	if (qpnt && (mux == SEMA_ON))
//		return xSemaphoreTake(*sema_p, portMAX_DELAY); //(_mutex_print,)
//	return pdFALSE; //pdTRUE;
//}

//void tasks_dm9051_mutex_end(int qpnt, sema_tag_t mux, xSemaphoreHandle *sema_p)
//{
//	if (qpnt && (mux == SEMA_ON))
//		xSemaphoreGive(*sema_p);
//	return;
//}
#endif /* freeRTOS */
#endif //0
