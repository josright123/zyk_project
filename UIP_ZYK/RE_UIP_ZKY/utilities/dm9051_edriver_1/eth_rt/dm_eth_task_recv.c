/**
  **************************************************************************
  * @file     dm_eth.c
  * @file     dm_eth_main.c
  * @version  v1.0.0
  * @date     2024-06-24
  * @brief    as eth.c
  **************************************************************************
  */
#include "cmsis_compiler.h"               /* CMSIS compiler specific defines */
#include "ethernetif.h"
#include "netconf.h" //#include "lwip/netif.h" //#include "lwip/err.h"

#include "dm_eth_task_main.h"
void DM_Eth_SemaphoreNewbinary_Log(void); //dm_eth_mutex.h
void DM_ETHER_Receive_Task_head_log(void); //dm_eth_mutex.h
void DM_ETHER_Receive_Task(void* param); //dm_eth_task_recv.h
#include "dm9051_rt_debug.h"
#define printk printf
//#include "dm_eth_lock.h" //#include "dm9051_rtos.h"

TaskHandle_t network_rx_handler;

uint16_t isrSemaphore_src;
int isrSemaphore_n = 0; //extern int flgSemaphore_n;
extern int flgSemaphore_r;

#include "dm_eth_intr_supp.h" //#include "dm_eth_intr_supp.h"
#include "dm_eth_recv_supp_intr.h"
#include "dm_eth_recv_supp_poll.h"

void diff_rx_pointers_s(uint16_t *pMdra_rds) {
#if DM_ETH_DEBUG_MODE
	uint16_t rwpa_wts /*mdra_rds*/;
	DM_ETH_Diff_rx_pointers_W(0, &rwpa_wts, pMdra_rds);
#endif
}

void diff_rx_pointers_e(uint16_t *pMdra_rds, int n) {
#if DM_ETH_DEBUG_MODE
	uint16_t rwpa_wt, mdra_rd, diff;
	if (n >= 3) {
		diff = DM_ETH_Diff_rx_pointers_W(1, &rwpa_wt, &mdra_rd);
		diff = wrpadiff(*pMdra_rds, mdra_rd);
		printf("INTR.mdra.s %02x%02x mdra.e %02x%02x diff %02x%02x (%d packets)\r\n",
			*pMdra_rds >> 8, *pMdra_rds & 0xff,
			mdra_rd >> 8, mdra_rd & 0xff,
			diff >> 8, diff & 0xff,
			n);
		debug_packets(n);
	}
#endif
}

void DM_Eth_CreateRecvTask(void)
{
	//#undef printf
	//#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_NETCONF_ON, /*SEMA_OFF*/ SEMA_ON, "[ETH] ", (fmt, ##__VA_ARGS__))

//	DM_Eth_SemaphoreNewbinary();
	DM_Eth_SemaphoreNewbinary_Log();
	
	printf("Create.S DM_ETHER_Receive_Task\r\n");

	if(xTaskCreate((TaskFunction_t )DM_ETHER_Receive_Task,     
				 (const char*    )"Rx_task",   
				 (uint16_t       )512+256, //is to stacksize larger.
				 (void*          )tcpip_stack_netif(), //netif, //NULL,
				 (UBaseType_t    )NET_TASK_PRIO, //2
				 (TaskHandle_t*  )&network_rx_handler) != pdPASS) {
		printf("Rx task not be created. Error!\r\n");
	}
}

void DM_ETHER_Receive_Task_Loop(void) {
	if (DM_Eth_SemaphoreTakeBinary()) {
		if (flgSemaphore_r == 1) {
			flgSemaphore_r = 0; //for next to direct no-limited change-in

			//[net_inp_interrupt()]
			//uint16_t isrs;
			//isrs = 0x5555;
			//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
			//= uint16_t isrs = dm9051_isr_disab();
			//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));
			//= uint16_t isrs = DM_ETH_IRQDisable(); //dm9051_isr_disab();

			isrSemaphore_src = 0x5555 >> 8;
	
				//[net_pkts_handle_intr()] //[isrSemaphore_n = net_pkts_handle_intr(tcpip_stack_netif());]
				do {
					uint16_t mdra_rds;
					struct pbuf *p;

					diff_rx_pointers_s(&mdra_rds);

					//=while(input_intr());
					isrSemaphore_n = 0;
					do {
						p = input_intr();
						if (p) {
							p = process_input_pbuf(p);
							if (p) {
								isrSemaphore_n++;
							}
						}
					} while(p);

					diff_rx_pointers_e(&mdra_rds, isrSemaphore_n);
				} while(0);

				DM_ETH_ToRst_ISR_W();
				//=
				//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
				//cspi_isr_enab(); //DM_ETH_IRQEnable(); //dm9051_isr_enab();
				//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));
			
			dm_eth_semaphore_renew();
		}
		if (flgSemaphore_r == 2) {
			flgSemaphore_r = 0; //for next to direct no-limited change-in
			net_inp_poll(); //ethernet_inp_poll0(); //printf("[flgSemaphore_r == 2]._net_inp_poll(netif)\r\n");
			dm_eth_semaphore_renew();
		}
	}
	freertos_task_tryDisplay("Receive_Task.while", 1, 5000000, NULL /*&dispc_int*/);
}

void DM_ETHER_Receive_Task(void* param) {
	(void) param; //struct netif *netif = param;

	printf("Created.S DM_ETHER_Receive_Task\r\n");
	DM_ETHER_Receive_Task_head_log();
	printf("Created.E DM_ETHER_Receive_Task\r\n");

	while(1) {
		
		DM_ETHER_Receive_Task_Loop();
		vTaskDelay(0); //vTaskDelay(pdMS_TO_TICKS(1));
	}
}
