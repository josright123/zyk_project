/**
  **************************************************************************
  * @file     netconf.h
  * @version  v2.0.0
  * @date     2020-11-02
  * @brief    This file contains all the functions prototypes for the netconf.c 
  *           file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NETCONF_H
#define __NETCONF_H

#include "lwip/err.h"

#ifdef __cplusplus
 extern "C" {
#endif
  
#define NUM_COUNT_MULTI	3 //10 //15 //30 //100

#define NUM_COUNT_CYC	(NUM_COUNT_MULTI * 500)
#define NUM_COUNT_LEDS	(NUM_COUNT_MULTI * 258) //126 //60 //30 //12 //6
#define NUM_PER_LED		(NUM_COUNT_LEDS/3)

  
/* Includes ------------------------------------------------------------------*/
void ethnode_config(ip4_addr_t *ipaddr, ip4_addr_t *netmask, ip4_addr_t *gw);
void tcpip_stack_init(void);
err_t rx_direct0(void);
err_t lwip_pkt_handle(void);
err_t rx_direct(struct netif *netif);
void rx_handler_direct(int i, int directFlag);
//void rx_handler_semaphore(void);
void lwip_periodic_handle(volatile uint32_t localtime);

//void ethernetif_set_link(void const *argument);

uint8_t netconf_is_link_up(void);
void netconf_set_link_down(void);
void netconf_set_link_up(void);

void net_task_freertos_init_led(uint16_t link, int link_change);
void net_task_freertos_link_handler_and_init_led(uint32_t ms);
void net_task_freertos_recv_and_led(int rx_flg, uint32_t ms);
void task_periodic_polling(void);

//void net_tcpip_thread(void);
void tcpip_thread_trip(const uint32_t xTicksToDelay); //to tcpip.c

void freertos_tasks_display(void);
void led_start(void);

#ifdef __cplusplus
}
#endif
    
#endif /* __NETCONF_H */



