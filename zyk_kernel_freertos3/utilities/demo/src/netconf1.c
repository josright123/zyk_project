/**
  **************************************************************************
  * @file     netconf1.c
  * @version  v2.0.0
  * @date     2020-11-02
  * @brief    network connection configuration
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

/* Includes ------------------------------------------------------------------*/
#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/priv/tcp_priv.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/tcpip.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "dm_eth.h"
#include "stdio.h"

#include "dm9051opts.h"
#include "dm9051_lw.h"
#include "dm9051_cboard.h"
#include "dm9051_lw_cint.h"
#include "ethernetif.h"
#include "netconf.h"
#include "../freertos_tasks_debug.h"
#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_NETCONF_ON, /*SEMA_OFF*/ SEMA_ON, "[N]", (fmt, ##__VA_ARGS__))

/* Private variables ---------------------------------------------------------*/
struct netif netif;

static uint8_t mac_address1[BOARD_SPI_COUNT][MAC_ADDR_LENGTH] = {
	{0, 0x60, 0x6e, 0x00, 0x00, 0x33},
	{0, 0x60, 0x6e, 0x00, 0x00, 0x35},
	{0, 0x60, 0x6e, 0x00, 0x00, 0x37},
};
#if LWIP_DHCP == 0
static uint8_t local_ip1[BOARD_SPI_COUNT][ADDR_LENGTH]   = {
	{192, 168, 6,  33},
	{192, 168, 6,  35},
	{192, 168, 6,  37},
};
static uint8_t local_gw1[BOARD_SPI_COUNT][ADDR_LENGTH]   = {
	{192, 168, 6,   1},
	{192, 168, 6,   1},
	{192, 168, 6,   1},
};
static uint8_t local_mask1[BOARD_SPI_COUNT][ADDR_LENGTH] = {
	{255, 255, 255, 0},
	{255, 255, 255, 0},
	{255, 255, 255, 0},
};
#endif

uint8_t netconf_is_link_up(void) {
  return netif_is_link_up(&netif);
}

void netconf_set_link_down(void) {
	netif_clear_flags(&netif, NETIF_FLAG_LINK_UP);
}
void netconf_set_link_up(void) {
	netif_set_flags(&netif, NETIF_FLAG_LINK_UP);
}

static void ethernetif_notify_conn_changed(struct netif *netif)
{
  /* note : this is function could be implemented in user file 
            when the callback is needed,
  */

  if (netif_is_link_up(netif)) {
    netif_set_up(netif);

#if LWIP_DHCP
    /*  creates a new dhcp client for this interface on the first call.
    note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
    the predefined regular intervals after starting the client.
    you can peek in the netif->dhcp struct for the actual dhcp status.*/
    dhcp_start(netif);
#endif
  }
  else
    netif_set_down(netif);
}

void ethernetif_update_config(struct netif *netif)
{
  ethernetif_notify_conn_changed(netif);
}

/**
  * @brief  initializes the lwip stack
  * @param  none
  * @retval none
  */
void ethnode_config(ip4_addr_t *ipaddr, ip4_addr_t *netmask, ip4_addr_t *gw)
{
  int pin = 0;
  uint8_t *pd;

#if LWIP_DHCP  //need DHCP server
  ipaddr->addr = 0;
  netmask->addr = 0;
  gw->addr = 0;
#else

  //pd = identify_tcpip_ip(NULL); //(local_ip1);
  pd = identify_tcpip_ip(local_ip1[pin]);
  IP4_ADDR(ipaddr, pd[0], pd[1], pd[2], pd[3]);
  pd = identify_tcpip_gw(local_gw1[pin]); //(local_gw1);
  IP4_ADDR(gw, pd[0], pd[1], pd[2], pd[3]);
  //pd = identify_tcpip_mask(NULL); //(local_mask1);
  pd = identify_tcpip_mask(local_mask1[pin]);
  IP4_ADDR(netmask, pd[0], pd[1], pd[2], pd[3]);
#endif

  lwip_set_mac_address(mac_address1[pin]);
}

void tcpip_stack_init(void)
{
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;
  uint8_t *pd;
 
  tcpip_init(NULL,NULL);

  /* - netif_add(struct netif *netif, struct ip_addr *ipaddr,
            struct ip_addr *netmask, struct ip_addr *gw,
            void *state, err_t (* init)(struct netif *netif),
            err_t (* input)(struct pbuf *p, struct netif *netif))
    
   Adds your network interface to the netif_list. Allocate a struct
  netif and pass a pointer to this structure as the first argument.
  Give pointers to cleared ip_addr structures when using DHCP,
  or fill them with sane numbers otherwise. The state pointer may be NULL.

  The init function pointer must point to a initialization function for
  your ethernet netif interface. The following code illustrates it's use.*/
  
printf("LWIP_Initalize.s1 node config\r\n"); //printk("\r\n");
  ethnode_config(&ipaddr, &netmask, &gw);
printf("LWIP_Initalize.s2 netif add\r\n"); //printf("\r\n");
  if(netif_add(&netif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_msging_pkt) == NULL) {
    while(1);
  }
printf("LWIP_Initalize.s3 ip (identified)\r\n"); //printf("\r\n");
  pd = identified_tcpip_ip();
  printf("eth_config ip %d.%d.%d.%d\r\n", pd[0], pd[1], pd[2], pd[3]);
  pd = identified_tcpip_gw();
  printf("eth_config gw %d.%d.%d.%d\r\n", pd[0], pd[1], pd[2], pd[3]);
  pd = identified_tcpip_mask();
  printf("eth_config mask %d.%d.%d.%d\r\n", pd[0], pd[1], pd[2], pd[3]);
printf("\r\n");
printk("\r\n");

  link_quick_check(); //xzcbfgngfnh..
  
  /*  Registers the default network interface.*/
  netif_set_default(&netif);

#if LWIP_DHCP
  /*  Creates a new DHCP client for this interface on the first call.
  Note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
  the predefined regular intervals after starting the client.
  You can peek in the netif->dhcp struct for the actual DHCP status.*/
  dhcp_start(&netif);
#endif

  /*  When the netif is fully configured this function must be called.*/
  netif_set_up(&netif);  
  
  /* Set the link callback function, this function is called on change of link status*/
  netif_set_link_callback(&netif, ethernetif_update_config);
  
  /* Create the Ethernet link handler thread */
//  xTaskCreate((TaskFunction_t)ethernetif_set_link, "ethernetif_set_link", 512, &netif, tskIDLE_PRIORITY + 3, &link_status_handler);
}
