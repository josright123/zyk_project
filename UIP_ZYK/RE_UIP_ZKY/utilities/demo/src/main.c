/**
  **************************************************************************
  * @file     main.c
  * @version  v2.0.6
  * @date     2022-03-11
  * @brief    main program
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to 
  * download from Artery official website is the copyrighted work of Artery. 
  * Artery authorizes customers to use, copy, and distribute the BSP 
  * software and its related documentation for the purpose of design and 
  * development in conjunction with Artery microcontrollers. Use of the 
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */
#include "at32f435_437_board.h"
#include "at32f435_437_clock.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"

#define NET_TASK_PRIO           		2 //FOR 'net_task'

//#include "lwip/opt.h"
//#include "uip_d/freertos_tasks_debug.h"

//const uint8_t *test_init(void);
//const uint8_t *test_init(const uint8_t *adr);

//const uint8_t *_dm9051_init(const uint8_t *adr);
//const uint8_t *dm9051_init1(const uint8_t *adr);
//const uint8_t *test_init1(const uint8_t *adr);

//#include "../dm_eth_api.h" //#include "dm_eth.h"

#if 0
  #include "../freertos_tasks_debug.h"
  #define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_MAIN_ON, /*SEMA_OFF*/ SEMA_ON, "[i]", (fmt, ##__VA_ARGS__))
#endif

#include "uIP_Task.h" //.void _vuIP_Task(void *pvParameters);

int Web_LED_FLASH = 1; // Default set 1 use freertos task control led, if set 0 web control

void network_task(void *pvParameters);
void create_network_task(void);

/** @addtogroup UTILITIES_examples
  * @{
  */
  
/** @addtogroup FreeRTOS_demo
  * @{
  */

TaskHandle_t network_handler;

//void vuIP_Task(void *pvParameters)
//{
//	(void) pvParameters;
	#if 1
	//do {
	//	const uint8_t *mac = dm9051_init1(NULL);
	//	const uint8_t *mac = _dm9051_init(NULL);
	//} while(0);
	#endif
//}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  system_clock_config();
  uart_print_init(115200);
  
  printf("\r\n");
  printf("\r\n");
  printf("\r\n");
  printf("[ZYK_project/re_zyk]\r\n");
  //tasks_dm9051_debug_init();
  
#if 0 //[TEST]
//  vuIP_Task(NULL);
//  do {
//	const uint8_t *mac;
//		//mac = test_init();
//		//mac = test_init(NULL);
//		//mac = test_init1(NULL);
//		//mac = dm9051_init1(NULL);
//		//mac = _dm9051_init(NULL);
//		
//		printf("mac %x\r\n", mac[0]);
//  } while(0);
#else

  /* enter critical */
  taskENTER_CRITICAL(); 

  create_network_task();
  
  /* exit critical */            
  taskEXIT_CRITICAL();      
              
  /* start scheduler */            
  vTaskStartScheduler(); 
#endif
}

#include "../debug/dm9051_ap_debug.h"
#define printf(fmt, ...) TASK_DM9051_DEBUGF(PRINT_SEMA, PRINT_INFO_AP, (fmt, ##__VA_ARGS__)) //PRINT_AP or "[MAIN] "
#define	printk(fmt, ...) TASK_DM9051_DEBUGK(PRINT_SEMA, (fmt, ##__VA_ARGS__))

//void task_periodic_polling(void)
//{
//  while(1)
//  {
//	vTaskDelay(pdMS_TO_TICKS(NET_TASK_DLY)); 
//  }
//}

void network_task(void *pvParameters)
{
  (void) pvParameters;

	dm_eth_create_mutex_print();
//if (_intr_gpio_mptr()) {
//  DM_Eth_Initialize();
//}
  vuIP_Task(NULL); //task_periodic_polling();
}

void create_network_task(void)
{
  if(xTaskCreate((TaskFunction_t )network_task,     
                 (const char*    )"net_task",   
                 (uint16_t       )512+128, 
                 (void*          )NULL,
                 (UBaseType_t    )NET_TASK_PRIO, //2
                 (TaskHandle_t*  )&network_handler) != pdPASS)
  {
    printf("Net_task created Error!\r\n");
  }
}

/**
  * @}
  */ 

/**
  * @}
  */ 
