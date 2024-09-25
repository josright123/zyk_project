/*
 * Copyright (c) 2001, Adam Dunkels.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Adam Dunkels.
 * 4. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the uIP TCP/IP stack.
 *
 * $Id: main.c,v 1.16 2006/06/11 21:55:03 adam Exp $
 *
 */
/*------------------------------------------------------------*/
// main.c
// Modified by Spenser  2013/11
// Platform: M08G16 Cortex-M0
// Builder: Keil 4
/*------------------------------------------------------------*/
#include "stdio.h"

/* uIP include */
#include "uip.h"
#include "uip_arp.h"
#include "tapdev.h"
#include "dhcpc.h"
#include "timer.h"
/* FreeRTOS include */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#if 1
//#include "dm9051opts.h"
//#include "dm9051_lw.h"
//#include "cboard/dm9051_lw_mcu_default_IN.h"
#include "dm9051_env.h"

//#include "dm9051_cboard.h"
//#include "../dm9051_lw_mcu_default.h"
//#include "dm9051_lw_conf_types.h"
//#include "dm9051_lw_cint.h"

#else
//#include "DM9051.h"
//#include "RttPrintf.h"
#endif

#include "dhcpc.h"
#include "dm_eth/dm_eth.h" //#include "dm_eth.h"
//#include "_dm_eth.h"

//[version_0.ok]
//#define tapdev_send()		DM_ETH_Output((uint8_t *)uip_buf, uip_len) //dm9051_tx((uint8_t *)uip_buf, uip_len)
//#define tapdev_read()		DM_ETH_Input((uint8_t *)uip_buf) //_DM_ETH_RXHandler((uint8_t *)uip_buf) //dm9051_rx((uint8_t *)uip_buf)
//[version_1]
#define tapdev_init()		DM9051_init()
#define tapdev_send()		DM9051_tx()
#define tapdev_read()		DM9051_rx()
//#define	input_intr()	DM9051_rx()
#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

#ifndef NULL
    #define NULL (void *)0
#endif /* NULL */

#ifdef __DHCPC_H__
    struct timer dhcp_timer;
#endif

uint32_t LED_flag;
/*---------------------------------------------------------------------------*/
//uint16_t rx_wrapper_read(void) {
//	static int n = 0;
//	if (flgSemaphore_r) {
//		if (n == 0) {
//			//dm9051_isr_disab();
//			DM_ETH_IRQDisable();
//		}
//		uip_len = tapdev_read();
//		if (uip_len) {
//			u16_t wrapper_len = uip_len;
//			n++;
//			/* Interrupt, per increase 1 packet */
//			rcx_handler_direct();
//			return wrapper_len; //continue;
//		}
//		else {
//			flgSemaphore_r = 0;
//			if (n >= 3)
//				printf("tapdev_read exint9_5_handler(void) EXINT_LINE_%d, set flgSemaphore_r %d (conti %d packets)\r\n",
//						de_enum(dm9051_irq_exint_line(0)), flgSemaphore_r, n);
//			n = 0;
//			DM_ETH_IRQEnable();
//			//dm9051_isr_enab();
//		}
//	}
//	return 0;
//}

#if defined(DM9051_DRIVER_INTERRUPT)

uint16_t isrSemaphore_src;
int isrSemaphore_n = 0;

#if DM_ETH_DEBUG_MODE
//static
extern uint16_t wrpadiff(uint16_t rwpa_s, uint16_t rwpa_e);

static void debug_packets(int n) {
//	printf("(%d packets)\r\n", n);
}

static uint16_t DM_ETH_Diff_rx_pointers(int stamp, uint16_t *rwpa_wtp, uint16_t *mdra_rdp)
{
	static uint16_t /*rwpa_wts,*/ mdra_rds;

	//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
	dm9051_read_rx_pointers(rwpa_wtp, mdra_rdp);
	//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));

	if (stamp == 0) {
		//rwpa_wts = *rwpa_wtp;
		mdra_rds = *mdra_rdp;
		return 0;
	}

	//diffs = _wrpadiff(rwpa_wts, *rwpa_wtp);
	//diffs = _wrpadiff(mdra_rds, *mdra_rdp);
	return wrpadiff(mdra_rds, *mdra_rdp);
}
#endif

void diff_rx_pointers_s(uint16_t *pMdra_rds) {
#if DM_ETH_DEBUG_MODE
	uint16_t rwpa_wts /*mdra_rds*/;
	DM_ETH_Diff_rx_pointers(0, &rwpa_wts, pMdra_rds);
#endif
}

void diff_rx_pointers_e(uint16_t *pMdra_rds, int n) {
#if DM_ETH_DEBUG_MODE
	uint16_t rwpa_wt, mdra_rd, diff;

	diff = DM_ETH_Diff_rx_pointers(1, &rwpa_wt, &mdra_rd); //......................
	diff = wrpadiff(*pMdra_rds, mdra_rd);
	printf("INTR.mdra.s %02x%02x mdra.e %02x%02x diff %02x%02x (%d packets)\r\n",
		*pMdra_rds >> 8, *pMdra_rds & 0xff,
		mdra_rd >> 8, mdra_rd & 0xff,
		diff >> 8, diff & 0xff,
		n);
#endif
}

int input_intr(void)
{
	uip_len = tapdev_read();
	return (uip_len > 0) ? 1 : 0;
}

#endif

#if defined(DM9051_DRIVER_POLL)
.......... old version_0..
uint16_t DM_ETH_RXHandler_Poll(void)
{
	uip_len = tapdev_read();
	if (uip_len)
		/* Polling, per 1 packet */
		rcx_handler_direct();
	return uip_len;
}
#endif

/*---------------------------------------------------------------------------*/
#if 1 //0 [!!TEST]
void vuIP_Task(void *pvParameters)
{
	int i; //n = 0;
    const TickType_t xFrequency = 10;
    TickType_t xLastWakeTime = xTaskGetTickCount();

#ifndef __DHCPC_H__
    uip_ipaddr_t ipaddr={0,0};
#endif

    struct timer periodic_timer, arp_timer;

    /* FreeRTOS  task delay */
    timer_set(&periodic_timer, CLOCK_SECOND / 2); 		//500ms
    timer_set(&arp_timer, CLOCK_SECOND * 10);         // 10sec
	
	tapdev_init(); //DM_ETH_Init(&uip_ethaddr.addr[0]); //DM_Eth_Open();
	
    uip_init();
    uip_arp_init(); // Clear arp table.

#ifdef __DHCPC_H__ //if use fixed ip, #ifdef modify #ifndef
    // setup the dhcp renew timer the make the first request
    timer_set(&dhcp_timer, CLOCK_SECOND * 600);
    dhcpc_init(&uip_ethaddr, 6);
    //dhcpc_request();
#else //Fixed IP set
    uip_ipaddr(ipaddr, 10, 0, 2, 222);   //Host IP address
    uip_sethostaddr(ipaddr);
    uip_ipaddr(ipaddr, 10, 0, 205, 2);   //Default Gateway
    uip_setdraddr(ipaddr);
    uip_ipaddr(ipaddr, 255, 255, 255, 0); //Network Mask
    uip_setnetmask(ipaddr);
    /* Display system information */
    printf("\n---------------------------------------------\n");
    printf("Network chip: DAVICOM DM9051 \n");
    printf("MAC Address: %02X:%02X:%02X:%02X:%02X:%02X \n", uip_ethaddr.addr[0], uip_ethaddr.addr[1],
           uip_ethaddr.addr[2], uip_ethaddr.addr[3], uip_ethaddr.addr[4], uip_ethaddr.addr[5]);
    uip_gethostaddr(ipaddr);
    printf("Host IP Address: %d.%d.%d.%d \n", uip_ipaddr1(ipaddr), uip_ipaddr2(ipaddr), uip_ipaddr3(ipaddr), uip_ipaddr4(ipaddr));
    uip_getnetmask(ipaddr);
    printf("Network Mask: %d.%d.%d.%d \n", uip_ipaddr1(ipaddr), uip_ipaddr2(ipaddr), uip_ipaddr3(ipaddr), uip_ipaddr4(ipaddr));
    uip_getdraddr(ipaddr);
    printf("Gateway IP Address: %d.%d.%d.%d \n", uip_ipaddr1(ipaddr), uip_ipaddr2(ipaddr), uip_ipaddr3(ipaddr), uip_ipaddr4(ipaddr));
    printf("---------------------------------------------\n");
#endif
    httpd_init();

    while (1)
    {
#if 0
//[version_0.ok]
/* Interrupt */
//		if (intr_gpio_mptr()) {
//			if (DM_ETH_RXHandler())
//				continue;
//		}
/* Polling */
//		else {
//			if (DM_ETH_RXHandler_Poll())
//				continue;
//		}
#endif
#if 1
#if defined(DM9051_DRIVER_INTERRUPT)
//[version_1]
/* Interrupt */
		//if (DM_ETH_RXHandler())
		//	continue;
			
		//if (flgSemaphore_r == 1)
			//flgSemaphore_r = 0; //for next to direct no-limited change-in
		
		if (DM_ETH_InterruptEvent()) {

			isrSemaphore_src = 0x5555 >> 8;
			do { //[isrSemaphore_n = net_pkts_handle_intr(tcpip_stack_netif());]
				uint16_t mdra_rds;
				
				//printf("diff_rx_pointers_s()\r\n");
				diff_rx_pointers_s(&mdra_rds);

				isrSemaphore_n = 0;
				while (input_intr()) {
				
					dm_eth_input_hexdump(uip_buf, uip_len);
					#if 1
					//diff_rx_pointers_e(&mdra_rds, 1);
					#endif
					
					//= [original uip_processing code]
					if (BUF->type == htons(UIP_ETHTYPE_IP))
					{
						uip_input();        // uip_process(UIP_DATA)

						/* If the above function invocation resulted in data that
						should be sent out on the network, the global variable
						uip_len is set to a value > 0. */
						if (uip_len > 0)
						{
							uip_arp_out();
							tapdev_send();
						}
					}
					else if (BUF->type == htons(UIP_ETHTYPE_ARP))
					{
						uip_arp_arpin();

						/* If the above function invocation resulted in data that
						   should be sent out on the network, the global variable
						   uip_len is set to a value > 0. */
						if (uip_len > 0)
						{
							tapdev_send();
						}
					}
					isrSemaphore_n++;
				}

				if (isrSemaphore_n >= 3) {
					diff_rx_pointers_e(&mdra_rds, isrSemaphore_n);
					debug_packets(isrSemaphore_n);
				}
				
					#if 1
					//diff_rx_pointers_e(&mdra_rds, 0);
					#endif
			} while(0);
			
			//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
			cspi_isr_enab(); //DM_ETH_IRQEnable(); //dm9051_isr_enab();
			//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));
			//nExpireCount = 0; //= dm_eth_semaphore_renew();
		}
#else
//[version_1, to be continued.]
/* Polling */
		if (DM_ETH_RXHandler_Poll())
			continue;
#endif
#endif

		//uip_len = .. ; 
		//;[can insteaded, used for common-used of interrupt/polling]
        //if (uip_len > 0) {
		//	rcx_handler_direct();
        //} else
        if (timer_expired(&periodic_timer))
        {
            timer_reset(&periodic_timer);
            for (i = 0; i < UIP_CONNS; i++)
            {
                uip_periodic(i);
                /* If the above function invocation resulted in data that
                should be sent out on the network, the global variable
                uip_len is set to a value > 0. */
                if (uip_len > 0)
                {
                    uip_arp_out();
                    tapdev_send();
                }
            }

#if UIP_UDP

            for (i = 0; i < UIP_UDP_CONNS; i++)
            {
                uip_udp_periodic(i);
                /* If the above function invocation resulted in data that
                should be sent out on the network, the global variable
                uip_len is set to a value > 0. */
                if (uip_len > 0)
                {
                    uip_arp_out();
                    tapdev_send();
                }
            }

#endif /* UIP_UDP */

            /* Call the ARP timer function every 10 seconds. */
            if (timer_expired(&arp_timer))
            {
                timer_reset(&arp_timer);
                uip_arp_timer();
            }
        }

#ifdef __DHCPC_H__
        else if (timer_expired(&dhcp_timer))
        {
            // for now turn off the led when we start the dhcp process
            dhcpc_renew();
            timer_reset(&dhcp_timer);
        }

#endif // __DHCPC_H__
        else
        {
            /* task delay */
            vTaskDelayUntil(&xLastWakeTime, xFrequency);
        }
    }
}
#endif //[TEST]

/*---------------------------------------------------------------------------*/
void    uip_log(char *m)
{
    printf("uIP log message: %s\n", m);
}

void    resolv_found(char *name, u16_t *ipaddr)
{
    //u16_t *ipaddr2;

    if (ipaddr == NULL)
    {
        printf("Host '%s' not found.\n", name);
    }
    else
    {
        printf("Found name '%s' = %d.%d.%d.%d\n", name,
               htons(ipaddr[0]) >> 8,
               htons(ipaddr[0]) & 0xff,
               htons(ipaddr[1]) >> 8,
               htons(ipaddr[1]) & 0xff);
        /*    webclient_get("www.sics.se", 80, "/~adam/uip");*/
    }
}

#ifdef __DHCPC_H__
void    dhcpc_configured(const struct dhcpc_state *s)
{
    uip_ipaddr_t ipaddr={0,0};

    if (s->state == STATE_FAIL)
    {
		uint8_t *p;
		p = identify_tcpip_ip(NULL); //candidate_eth_ip();
        uip_ipaddr(ipaddr, p[0], p[1], p[2], p[3]);    //Host IP address
        uip_sethostaddr(ipaddr);
		p = identify_tcpip_gw(NULL); //candidate_eth_gw();
        uip_ipaddr(ipaddr, p[0], p[1], p[2], p[3]);     //Default Gateway
        uip_setdraddr(ipaddr);
		p = identify_tcpip_mask(NULL); //candidate_eth_mask();
        uip_ipaddr(ipaddr, p[0], p[1], p[2], p[3]); //Network Mask
        uip_setnetmask(ipaddr);
        printf("\n--Fixed IP address ---------------------\r\n");
    }
    else
    {
        LED_flag = 0;

        uip_sethostaddr(s->ipaddr);
        uip_setnetmask(s->netmask);
        uip_setdraddr(s->default_router);
        //  resolv_conf(s->dnsaddr);            // Now don't need DNS
        printf("\n--IP address setting from DHCP-----------\r\n");
    }

    /* Display system information */

    printf("Network chip: DAVICOM DM9051 \n");
    printf("MAC Address: %X:%X:%X:%X:%X:%X \n", uip_ethaddr.addr[0], uip_ethaddr.addr[1],
           uip_ethaddr.addr[2], uip_ethaddr.addr[3], uip_ethaddr.addr[4], uip_ethaddr.addr[5]);
    uip_gethostaddr(ipaddr);
    printf("Host IP Address: %d.%d.%d.%d \n", uip_ipaddr1(ipaddr), uip_ipaddr2(ipaddr), uip_ipaddr3(ipaddr), uip_ipaddr4(ipaddr));
    uip_getnetmask(ipaddr);
    printf("Network Mask: %d.%d.%d.%d \n", uip_ipaddr1(ipaddr), uip_ipaddr2(ipaddr), uip_ipaddr3(ipaddr), uip_ipaddr4(ipaddr));
    uip_getdraddr(ipaddr);
    printf("Gateway IP Address: %d.%d.%d.%d \n", uip_ipaddr1(ipaddr), uip_ipaddr2(ipaddr), uip_ipaddr3(ipaddr), uip_ipaddr4(ipaddr));
    printf("---------------------------------------------\r\n");
}
#endif /* __DHCPC_H__ */

void    smtp_done(unsigned char code)
{
    printf("SMTP done with code %d\n", code);
}
void    webclient_closed(void)
{
    printf("Webclient: connection closed\n");
}
void    webclient_aborted(void)
{
    printf("Webclient: connection aborted\n");
}
void    webclient_timedout(void)
{
    printf("Webclient: connection timed out\n");
}
void    webclient_connected(void)
{
    printf("Webclient: connected, waiting for data...\n");
}
void    webclient_datahandler(char *data, u16_t len)
{
    printf("Webclient: got %d bytes of data.\n", len);
}
/*---------------------------------------------------------------------------*/
