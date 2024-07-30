/**
 * @file
 * Ethernet Interface Skeleton
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/*
 * This file is a skeleton for developing Ethernet network interface
 * drivers for lwIP. Add code to the low_level functions and do a
 * search-and-replace for the word "ethernetif" to replace it with
 * something that better describes your network interface.
 */
#if 1
#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include "netif/etharp.h"
#include "netif/ppp/pppoe.h"
#include "lwip/err.h"
#include "lwip/tcpip.h"

//#include "mutex_jj0.h"
#include "dm9051opts.h" //TRANS_CONN/mstep_eth_mac/mstep_get_net_index
#include "dm9051_lw.h" //dm9051_mac_adr/dm9051_init/check_chip_id/dm9051_tx
#include "dm9051_cboard.h"
//#include "dm9051_lw_cint.h"
//#include "dm9051_lw_can_fix_tool_xxx.h"
#include "ethernetif.h"
#include "dm_eth.h"
#include "netconf.h"
//#include "at32_dm.h"

#include <string.h>

#include "../freertos_tasks_debug.h"
#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_ETHERNETIF_ON, SEMA_ON, "[T]", (fmt, ##__VA_ARGS__))

extern struct netif netif;
//struct netif *lwip_netif= &netif;

/* TCP and ARP timeouts */
//volatile int tcp_end_time, arp_end_time;

/* Define those to better describe your network interface. */
#define IFNAME0 'a'
#define IFNAME1 't'

#define OVERSIZE_LEN			PBUF_POOL_BUFSIZE //#define _PBUF_POOL_BUFSIZE 1514 //defined in "lwipopts.h" (JJ20201006)
#define RXBUFF_OVERSIZE_LEN		(OVERSIZE_LEN+2)
union {
	uint8_t rx;
	uint8_t tx;
} EthBuff[RXBUFF_OVERSIZE_LEN]; //[Single Task project.] not occupied by concurrently used.

uint8_t *get_ReceiveBuffer(void)
{
	return &EthBuff[0].rx;
}

uint8_t *get_TransmitBuffer(void)
{
	return &EthBuff[0].tx;
}

/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif
{
  struct eth_addr *ethaddr;
  /* Add whatever per-interface state that is needed here. */
  int unused;
};

/* Forward declarations. */
err_t  ethernetif_input(struct netif *netif);

#define EMAC_RXBUFNB        10
#define EMAC_TXBUFNB        10

const uint8_t *lwip_set_MACaddr; //MACaddr[6];
//emac_dma_desc_type  DMARxDscrTab[EMAC_RXBUFNB], DMATxDscrTab[EMAC_TXBUFNB];
//uint8_t Rx_Buff[EMAC_RXBUFNB][EMAC_MAX_PACKET_LENGTH], Tx_Buff[EMAC_TXBUFNB][EMAC_MAX_PACKET_LENGTH];

/**
 * Setting the MAC address.
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
void lwip_set_mac_address(const uint8_t* macadd)
{
	lwip_set_MACaddr = macadd;
//  MACaddr[0] = macadd[0];
//  MACaddr[1] = macadd[1];
//  MACaddr[2] = macadd[2];
//  MACaddr[3] = macadd[3];
//  MACaddr[4] = macadd[4];
//  MACaddr[5] = macadd[5];

#if 0
  //emac_local_address_set(macadd);
  //.ldm9051_mac_adr((const uint8_t *)macadd); //lwip set mac
#endif
}

int n_verify_id = 0;

//static uint16_t drviver_init(int pin) {
//	printf("drviver_init(pin = %d)\r\n", pin);
//	mstep_set_net_index(pin); 
//	return dm9051_init(_mstep_eth_mac()); //driver set mac
//}
//void dm9051_init_dual(void) {
//	n_verify_id = TRANS_DUAL(drviver_init);
//}

//static uint16_t drviver_init_nondual(uint8_t *macadr) {
//	printf("drviver_init(pin = %d)\r\n", mstep_get_net_index());
//	return dm9051_init(macadr);
//}
const uint8_t *dm9051_init_nondual(const uint8_t *hwaddr) { //(uint8_t* macadr)
	const uint8_t *mac;
	//.init
//	printf("drviver_init(pin = %d)\r\n", mstep_get_net_index());

//printf("\r\n");
//printf("In mac %02x:%02x:%02x:%02x:%02x:%02x\r\n", hwaddr[0],hwaddr[1],hwaddr[2],hwaddr[3],hwaddr[4],hwaddr[5]);
	//mac = dm9051_init(NULL);
	mac = dm9051_init(hwaddr); //in fact = mstep_eth_mac() //driver set mac
	//.id = drviver_init_nondual(macadr); //TRANS_NONDUAL(drviver_init_nondual, macadr);

	if (mac)
	 n_verify_id++;
	 
	return mac; //[TEMP]
}

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void
low_level_init(struct netif *netif)
{
  const uint8_t *pd;
  /* set MAC hardware address length */
  netif->hwaddr_len = ETHARP_HWADDR_LEN;

  /* maximum transfer unit */
  netif->mtu = 1500;

  /* device capabilities */
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;
  
  /* Enable MAC and DMA transmission and reception */
  //emac_start();
  
#if 1
  #if JOS_MUTEX_LO_INIT /* lo init */
  bannerline_log();
  printf("Sem: Lo init.s\r\n");
  while(1) {
	if (josSemaphoreDoOwn(1, "dm9051_init_nondual", /*lwip_sys_mutex*/ lwip_sys_mutexbinary)) { /* getmsg_tx, tx */
  #else
  LOCK_TCPIP_CORE();
  #endif
  
  /* set MAC hardware to dm9051 */
  pd = dm9051_init_nondual(lwip_set_MACaddr); //dm9051_init_dual(); //_dm9051_init(MACaddr);
  
  printk("\r\n");
  printf("Out mac %02x:%02x:%02x:%02x:%02x:%02x\r\n", pd[0],pd[1],pd[2],pd[3],pd[4],pd[5]);

  /* set MAC hardware address to lwip */
  netif->hwaddr[0] =  pd[0]; //= mstep_eth_mac()[0]
  netif->hwaddr[1] =  pd[1];
  netif->hwaddr[2] =  pd[2];
  netif->hwaddr[3] =  pd[3];
  netif->hwaddr[4] =  pd[4];
  netif->hwaddr[5] =  pd[5];

  #if JOS_MUTEX_LO_INIT
	  josSemaphoreDoYield(1, "dm9051_init_nondual", /*lwip_sys_mutex*/ lwip_sys_mutexbinary); //getmsg_tx
	  break;
	}
	vTaskDelay(pdMS_TO_TICKS(1));
  }
  printf("Sem: Lo init.e\r\n");
  bannerline_log();
//  _tcpip_thread_init = 3;
  #else
  UNLOCK_TCPIP_CORE();
  #endif
#endif
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
static err_t
low_level_output(struct netif *netif, struct pbuf *p)
{
err_t DM_ETH_Output(struct netif *netif, struct pbuf *p); //temp here!

  return DM_ETH_Output(netif, p);
}

#if 0
const int rx_modle_log_reset_allow_num = 3;
int link_log_reset_allow_num = 0;

void ethernetif_link_log_reset(void) {
	link_log_reset_allow_num = 0; // rx_modle_count[RX_ANY].allow_num = 0;
}

void ethernetif_link_log_reset_hexdump(const void *buf, size_t len) {
	#if 1
	int get_tcpip_thread_state(void);
	void set_tcpip_thread_state(int state); //.temp //extern int tcpip_thread_init;
	#endif
	LWIP_UNUSED_ARG(buf);
	LWIP_UNUSED_ARG(len);
	
	if (link_log_reset_allow_num < rx_modle_log_reset_allow_num) {
			link_log_reset_allow_num++;
			if (link_log_reset_allow_num == rx_modle_log_reset_allow_num && get_tcpip_thread_state() == 1) {
				set_tcpip_thread_state(6); //tcpip_thread_init = 6; //temp
			}
	}
}
#endif

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *
low_level_input(struct netif *netif)
{
  struct pbuf *p, *q;
  u16_t len = 0;
  int l =0;
  uint8_t *buffer = get_ReceiveBuffer();
  (void)netif;
//  uint16_t rwpa_w, mdra_ingress;

// .rx_pointers_isr_show("dm9051_rx.in");
//  dm9051_read_rx_pointers(&rwpa_w, &mdra_ingress);
  
  len = dm9051_rx(buffer);
//if (len) {
//  printf("RX.in ---LEN %u -----------rwpa_w.%4x mdra_ingress.%4x\r\n", len, rwpa_w, mdra_ingress);
//  printf("RX.out ---LEN %u -----------\r\n", len);
//}
  if (!len)
	  return NULL;
	  
  ethernetif_link_log_reset_hexdump(buffer, len);

  /* We allocate a pbuf chain of pbufs from the pool. */
  p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

  if (p != NULL)
  {
    for (q = p; q != NULL; q = q->next)
    {
      memcpy((u8_t*)q->payload, (u8_t*)&buffer[l], q->len);
      l = l + q->len;
    }
  }
  return p;
}

err_t
tcpip_msging_pkt(struct pbuf *p, struct netif *inp)
{
  err_t res;

//#if _JOS_MUTEX /* rx */
//  _josSemaphoreDoOwn("rx_putmsg", _lwip_sys_mutex); /* rx */
//#endif

  res = tcpip_input(p, inp);

//#if _JOS_MUTEX
//  _josSemaphoreDoYield(1, res == ERR_OK ? "rx_putmsg" : "rx_null_msg", _lwip_sys_mutex);
//#endif

  return res;
}

/**
  * @brief  updates the link states
  * @param  none
  * @retval link state 0: disconnect, 1: connection
  */
int link_startup_update_flg = 0;
int link_change_flg;

uint32_t low_level_get_id_bmsr(void)
{
	uint16_t id, bmsr;
	
#if JOS_MUTEX_LINK /* link_update */
	if (josSemaphoreDoOwn(1, "link", lwip_sys_mutex)) { /* link_update, link */
#else
	LOCK_TCPIP_CORE();
#endif

  /* Read PHY_BSR*/
  id = dm9051_read_chip_id();
  bmsr = dm9051_bmsr_update();

#if JOS_MUTEX_LINK
		josSemaphoreDoYield(0, "link", lwip_sys_mutex); //link_update
	}
#else
	UNLOCK_TCPIP_CORE();
#endif
	return (id << 16) | bmsr;
}

uint16_t ethernetif_link(struct netif *netif)
{
  uint32_t id_bmsr;
  uint16_t regvalue;

  id_bmsr = low_level_get_id_bmsr();
  regvalue = id_bmsr & 0x0004 ? 1 : 0;

  #if 1
	link_change_flg = 0;
	/* Check whether the netif link down and the PHY link is up */
	if(!netif_is_link_up(netif) && (regvalue))
	{
	  /* network cable is connected */ 
	  netif_set_link_up(netif);      
//	  printf("%04x dm9051-link up %u.%u.%u.%u ............ ethernetif_link( ip...) ............ _tcpip_thread_init %d\r\n",
//			bmsr,
//			ip4_addr1_16(netif_ip4_addr(netif)), 
//			ip4_addr2_16(netif_ip4_addr(netif)),
//			ip4_addr3_16(netif_ip4_addr(netif)), 
//			ip4_addr4_16(netif_ip4_addr(netif)),
//			_tcpip_thread_init);

	  link_startup_update_flg = 1;
	  link_change_flg = 1;
//	  dm9051_link_log_reset();
	  ethernetif_link_log_reset();
	  
	  //.check_link_up_can_fix(netif, id_bmsr);

	}
	else if(netif_is_link_up(netif) && (!regvalue))
	{
	  /* network cable is dis-connected */
	  netif_set_link_down(netif);
//	  printf("%04x dm9051-link down _tcpip_thread_init %d\r\n", bmsr, _tcpip_thread_init);
	  printk("\r\n"); // bannerline_log();
	  printf("bmsr %04lx. (Chip ID %04lx) dm9051-link down\r\n", id_bmsr & 0xffff, id_bmsr >> 16);
	  link_startup_update_flg = 1;
	  link_change_flg = 1;
	}
  #endif
  return regvalue;
}

void link_quick_check(void)
{
	uint32_t id_bmsr = low_level_get_id_bmsr();

	if (id_bmsr & 0x0004)
		netconf_set_link_down();
	else
		netconf_set_link_up();
	ethernetif_link(&netif);
#if 0
	//uint16_t regvalue = ethernetif_link(&netif);
#endif
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
int pbufin = 0;
err_t
ethernetif_input(struct netif *netif)
{
  err_t err;
  struct pbuf *p = NULL;

#if JOS_MUTEX_RX /* rx */
  if (josSemaphoreDoOwn(/*0, 1*/ 1, "rx", lwip_sys_mutex)) { /* rx_putmsg, rx */
#else
  LOCK_TCPIP_CORE();
#endif


//printf("low_level_input ---p.in %x -------------------------------\n", p);
	/* move received packet into a new pbuf */
	p = low_level_input(netif);
//if (p)
//  printf("low_level_input(%d) ---p.out 0x%x -------------------------------\r\n", ++pbufin, p);

#if JOS_MUTEX_RX
	josSemaphoreDoYield(/*1,*/ 0, "rx_putmsg", lwip_sys_mutex);
  }
#else
  UNLOCK_TCPIP_CORE();
#endif

  /* no packet could be read, silently ignore this */
  if (p == NULL) return ERR_MEM;

  err = netif->input(p, netif);
  if (err != ERR_OK)
  {
    LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
    pbuf_free(p);
    p = NULL;
  }
//_tcpip_thread_init = 3;
  return err;
}

err_t ethernetif_inp(struct netif *netif)
{
	int n = 0;

	#if ETHERNET_INTERRUPT_MODE
	uint16_t isrs = DM_ETH_IRQDisable(); //dm9051_isr_disab();
	
	if (!(isrs & ISR_PR))
		printf("IRQ: dm9051_isr %02x, It is %s\r\n",
			isrs >> 8, isrs & ISR_PR ? "PR packet found" : "No packet found");
	#endif

	while (ethernetif_input(netif) == ERR_OK)
		n++;

	#if ETHERNET_INTERRUPT_MODE
	if ((!(isrs & ISR_PR)) || n > 1)
		printf("IRQ: dm9051_isr %02x, It is recv %d packet(s) via %s\r\n", isrs >> 8, n, "a-info" /*exint_enable_info()*/);
	
	DM_ETH_IRQEnable(); //dm9051_isr_enab();
	#endif

	return ERR_OK;
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */

err_t
ethernetif_init(struct netif *netif)
{
  struct ethernetif *ethernetif;

  LWIP_ASSERT("netif != NULL", (netif != NULL));

//  printf("netif_add.init %u.%u.%u.%u ............ netif_add( ip...) done ............\r\n",
//		ip4_addr1_16(netif_ip4_addr(netif)), 
//		ip4_addr2_16(netif_ip4_addr(netif)),
//		ip4_addr3_16(netif_ip4_addr(netif)), 
//		ip4_addr4_16(netif_ip4_addr(netif)));

  ethernetif = mem_malloc(sizeof(struct ethernetif));
  if (ethernetif == NULL)
  {
    LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_init: out of memory\n"));
    return ERR_MEM;
  }

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  /*
   * Initialize the snmp variables and counters inside the struct netif.
   * The last argument should be replaced with your link speed, in units
   * of bits per second.
   */
  NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, 100000000);

  netif->state = ethernetif;
  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;
  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
  netif->output = etharp_output;
  netif->linkoutput = low_level_output;

  ethernetif->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);

	  /* initialize the hardware */
	  low_level_init(netif);

  return ERR_OK;
}

//typedef struct{
//u32 length;
//u32 buffer;
//emac_dma_desc_type *descriptor;
//}FrameTypeDef;
/*******************************************************************************
* Function Name  : emac_rxpkt_chainmode
* Description    : Receives a packet.
* Input          : None
* Output         : None
* Return         : frame: farme size and location
*******************************************************************************/
//FrameTypeDef emac_rxpkt_chainmode(void)
//{
//	FrameTypeDef frame;;
//	return frame;
//}
#endif
