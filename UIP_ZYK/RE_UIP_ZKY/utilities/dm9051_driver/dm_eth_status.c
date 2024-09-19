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

#define	limit_len(n, nTP)	((n <= nTP) ? n : nTP)

#if DM_ETH_DEBUG_MODE
int link_log_reset_allow_num = 0;
const int rx_modle_log_reset_allow_num = 3;

void dm_eth_input_hexdump_reset(void) {
	link_log_reset_allow_num = 0; // rx_modle_count[RX_ANY].allow_num = 0;
}

void dm_eth_input_hexdump(const void *buf, size_t len) {
	#if 1
	int get_tcpip_thread_state(void);
	void set_tcpip_thread_state(int state); //.temp //extern int tcpip_thread_init;
	#endif
	int titledn = 0;
	DM_UNUSED_ARG(buf);
	DM_UNUSED_ARG(len);
	
	if (link_log_reset_allow_num < rx_modle_log_reset_allow_num) {
			link_log_reset_allow_num++;
			if (link_log_reset_allow_num == rx_modle_log_reset_allow_num && get_tcpip_thread_state() == 1) {
				set_tcpip_thread_state(6); //tcpip_thread_init = 6; //temp
			}
			sprint_hex_dump0(2, titledn, "dm9 head   <<rx", len, 32, buf, 0, limit_len(len, 66), DM_TRUE);
	}
}
#endif

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

//linking...

//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol mutex_print (referred from .\objects\freertos_tasks_debug.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol tasks_dm9051_mutex_begin (referred from .\objects\freertos_tasks_debug.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol tasks_dm9051_mutex_end (referred from .\objects\freertos_tasks_debug.

//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DM_Eth_Initialize (referred from .\objects\main.o).o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DM_Eth_Create_mutexes (referred from .\objects\main.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DM_Eth_Create_Receive_Task (referred from .\objects\main.o).

//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DM_ETH_Init (referred from .\objects\ethernetif.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DM_ETH_Input (referred from .\objects\ethernetif.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DM_ETH_Output (referred from .\objects\ethernetif.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DM_Eth_GetStatus (referred from .\objects\ethernetif.o).

//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DM_ETH_InterruptHdlr (referred from .\objects\dm9051_cboard_data_import.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DM_ETH_InterruptHdlr_Supplement (referred from .\objects\netconf0.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DM_ETH_SHOW_CONSECUTIVE_Packets (referred from .\objects\netconf0.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DM_Eth_StatusLink (referred from .\objects\netconf0.o).

//Not enough information to list image symbols.
//Not enough information to list load addresses in the image map.
//Finished: 2 information, 0 warning and 14 error messages.


#if freeRTOS
/* Declare the mutex handle globally so it can be accessed in protect spi sync.
 */
xSemaphoreHandle mutex_print; //...........ghmgtg...
/*BaseType_t mutex_pdbool = pdFALSE;*/

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

//linking... dm_eth.c
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DM_Eth_Create_Receive_Task (referred from .\objects\main.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DM_Eth_Create_mutexes (referred from .\objects\main.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DM_Eth_Initialize (referred from .\objects\main.o).

//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DM_ETH_InterruptHdlr_Supplement (referred from .\objects\netconf0.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DM_ETH_Init (referred from .\objects\ethernetif.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DM_ETH_Input (referred from .\objects\ethernetif.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DM_ETH_Output (referred from .\objects\ethernetif.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DM_ETH_SHOW_CONSECUTIVE_Packets (referred from .\objects\netconf0.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DM_Eth_GetStatus (referred from .\objects\ethernetif.o).
/*.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DM_Eth_StatusLink (referred from .\objects\netconf0.o).*/

//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DM_ETH_InterruptHdlr (referred from .\objects\dm9051_cboard_data_import.o).
//(Total 11 items)

//linking... dm_eth_status.c
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm_eth_input_hexdump_reset (referred from .\objects\ethernetif.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm_eth_input_hexdump (referred from .\objects\dm_eth.o).
//(Total 2 items)
