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
#include "ZYKComponents/DM9051/DM9051.h"
#include "dhcpc.h"
#include "ZYKApp/RttPrint.h"
#define BUF ((struct uip_eth_hdr *)&uip_buf[0])
#ifdef DM9051_INT
#include "semphr.h"
extern SemaphoreHandle_t rx_semaphore;
#endif

#ifndef NULL
    #define NULL (void *)0
#endif /* NULL */

#ifdef __DHCPC_H__
    struct timer dhcp_timer;
#endif

uint32_t LED_flag;
/*---------------------------------------------------------------------------*/
void vuIP_Task(void *pvParameters)
{

    int i;

    struct timer periodic_timer, arp_timer;

    /* FreeRTOS  task delay */
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 10;
    xLastWakeTime = xTaskGetTickCount();

    timer_set(&periodic_timer, CLOCK_SECOND / 2); 		//500ms
    timer_set(&arp_timer, CLOCK_SECOND * 10);         // 10sec

    tapdev_init(); // DM9051 init

    uip_init();
    uip_arp_init(); // Clear arp table.
#ifdef __DHCPC_H__ //if use fixed ip, #ifdef modify #ifndef
    // setup the dhcp renew timer the make the first request
    timer_set(&dhcp_timer, CLOCK_SECOND * 600);
    dhcpc_init(&uip_ethaddr, 6);
    //dhcpc_request();
#else //Fixed IP set
    uip_ipaddr_t ipaddr={0,0};
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

#ifdef DM9051_INT
    tapdev_intenable();
#endif
    while (1)
    {
#ifdef DM9051_INT
    if (xSemaphoreTake(rx_semaphore, portMAX_DELAY) == pdTRUE)
    {



rep:
        uip_len = tapdev_read();
#else
        uip_len = tapdev_read();
#endif


        if (uip_len > 0)
        {
            //printf("Rx length = %d.\n", uip_len);
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

        }
        else if (timer_expired(&periodic_timer))
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
#ifdef DM9051_INT
     if(tapdev_read_intpinstatus()==0)
     goto rep;
    }
#endif
   }
}
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
        uip_ipaddr(ipaddr, 192, 168, 7, 51);    //Host IP address
        uip_sethostaddr(ipaddr);
        uip_ipaddr(ipaddr, 192, 168, 7, 1);     //Default Gateway
        uip_setdraddr(ipaddr);
        uip_ipaddr(ipaddr, 255, 255, 255, 0); //Network Mask
        uip_setnetmask(ipaddr);
        printf("\n--Fixed IP address ---------------------\n");
    }
    else
    {
        LED_flag = 0;

        uip_sethostaddr(s->ipaddr);
        uip_setnetmask(s->netmask);
        uip_setdraddr(s->default_router);
        //  resolv_conf(s->dnsaddr);            // Now don't need DNS
        printf("\n--IP address setting from DHCP-----------\n");
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
    printf("---------------------------------------------\n");
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
