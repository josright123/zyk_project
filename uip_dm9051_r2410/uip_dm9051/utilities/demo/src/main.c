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

//#include "control/cdef.h"
#include "control/drv_control/conf_core.h"
#include "nosys/nosys_control/conf_ap.h"
#include "nosys/nosys_control/dm9051_ap_debug.h"

#if freeRTOS
#error "freeRTOS check defined, WRONG CONDITION!"
#else
#warning "freeRTOS is exactly NOT NEED defined"
#endif

#define NET_TASK_PRIO           		2 //FOR 'net_task'
#include "uIP_Task.h" //.void _vuIP_Task(void *pvParameters);

#include "main.h"

void main_tick_handler(void);

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
  
  dm_eth_show_app_help_info(__DATE__); //printkey("\r\n\r\n\r\n/ZYK_project /R2410 [uip_dm9051_r2410] %s\r\n", __DATE__);

  /* enter critical */
  taskENTER_CRITICAL(); 

  create_network_task();
  
  /* exit critical */            
  taskEXIT_CRITICAL();      
              
  /* start scheduler */            
  vTaskStartScheduler(); 
}

//uint32_t mainTicks = 0;

//uint32_t main_tick_count(void)
//{
//	return mainTicks;
//}

void main_tick_handler(void)
{
	//mainTicks++;
	dm9051_boards_heartbeat_tick();
	xPortSysTickHandler(); //SysTick_Handler_from_main(); //xPortSysTickHandler(); 
}

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

#if 0 
	//[uip no-need]
	dm_eth_create_mutex_print();
#endif
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
