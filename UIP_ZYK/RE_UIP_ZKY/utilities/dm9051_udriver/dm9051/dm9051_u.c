/*
 * Copyright (c) 2023-2025 Davicom Semiconductor, Inc.
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
 * This DM9051 Driver for LWIP tcp/ip stack
 * First veryification with : AT32F415
 *
 * Author: Joseph CHANG <joseph_chang@davicom.com.tw>
 * Date: 20230411
 * Date: 20230428 (V3)
 */
#include "stdint.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "dm9051_env.h"

#include "cboard/dm9051_cboard_data_API.h"	//for void DM9051_Configuration(), dm_delay_ms() 
//for cint_enable_mcu_irq()

#define PBUF_POOL_BUFSIZE                (1514+4) //.2000	//.2000(tested) //dm,jj=1514

uint16_t impl_dm9051_rx(uint8_t *buff)
{
	uint8_t rxbyte, rx_status;
	uint8_t ReceiveData[4];
	uint16_t rx_len, pad = 0; //8-bit
#if 0
	DM9051_RX_BREAK(!link_flag(), return evaluate_link());
#endif
	rxbyte = cspi_read_rxb();
	DM9051_RX_BREAK((rxbyte != 0x01 && rxbyte != 0), return env_evaluate_rxb(rxbyte));
	DM9051_RX_BREAK((rxbyte == 0), return 0);

	cspi_rx_head(ReceiveData);
	rx_status = ReceiveData[1];
	rx_len = ReceiveData[2] + (ReceiveData[3] << 8);
	DM9051_RX_BREAK((rx_status & (0xbf & ~RSR_PLE)), return env_err_rsthdlr("_dm9051f rx_status error : 0x%02x\r\n", rx_status));
	DM9051_RX_BREAK((rx_len > PBUF_POOL_BUFSIZE), return env_err_rsthdlr("_dm9051f rx_len error : %u\r\n", rx_len));
	pad = rx_len & 1; //16-bit
	cspi_rx_read(buff, rx_len+pad); //8/16-bit
	return rx_len;
}

uint16_t dm9051_rx(uint8_t *buff)
{
	uint16_t rx_len;
	//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
	rx_len = impl_dm9051_rx(buff);
	//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));
	return rx_len;
}

void dm9051_tx(uint8_t *buf, uint16_t len)
{
	//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
	cspi_tx_write(buf, len);
	cspi_tx_req();
	//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));
}

int display_identity(char *spiname, uint16_t id, uint8_t *ids, uint8_t id_adv, uint16_t idin, char *tail)
{
	DM_UNUSED_ARG(id);
	DM_UNUSED_ARG(id_adv);
	if (ids) {
	}
	printf("%s %s ::: Chip ID %04x\r\n", //"%s[%d]"
		tail, //": Read device", 0,
		spiname,
		idin
		);
	return 0;
}

const uint8_t *impl_dm9051_init(const uint8_t *adr)
{
	uint16_t id;
	const uint8_t *mac = NULL;
	if (env_init_setup(&id)) {
//	display_identity("SPI", 0, NULL, 0, id, "(DM9051 init)");
		mac = identify_eth_mac(adr);
		trace_identified_eth_mac(/*show 0*/ 1);
		mac = env_reset_process(mac, DM_TRUE);
		//_dm9051_probe_link(10);
	}
	printf("dm9051_init done\r\n");
	#ifdef DM9051_DRIVER_INTERRUPT
	printf("[interrupt]\r\n");
	#else
	printf("[polling]\r\n");
	#endif
	printf("\r\n");
	return mac;
}

const uint8_t *dm9051_init(const uint8_t *adr)
{
	//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
	//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));
	return impl_dm9051_init(adr);
}

uint16_t dm9051_read_control_status(void) {
#if 0
	uint16_t cs;
	//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core)); //LOCK_TCPIP_COREx();
	cs = cspi_read_control_status();
	//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core)); //ULOCK_TCPIP_COREx();
	return cs;
#else
	return 0x4000;
#endif
}

uint16_t dm9051_read_chip_id(void) {
#if 0
	//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core)); //LOCK_TCPIP_COREx();
	//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core)); //ULOCK_TCPIP_COREx();
	return cspi_read_chip_id();
#else
	return 0x9051;
#endif
}
uint16_t dm9051_read_bmsr(void) {
#if 0
	uint16_t val;
	//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core)); //LOCK_TCPIP_COREx();
	val = cspi_phy_read(PHY_STATUS_REG);
	//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core)); //ULOCK_TCPIP_COREx();
	return val;
#else
	return 0x786d;
#endif
}

void dm9051_read_rx_pointers(uint16_t *rwpa_wt, uint16_t *mdra_rd) {
	//LOCK_TCPIP_COREx();
	cspi_read_rx_pointers(rwpa_wt, mdra_rd);
	//ULOCK_TCPIP_COREx();
}

//uint16_t dm9051_isr_disab(void) {
//	DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core)); //LOCK_TCPIP_COREx();
//	return 0;
//	DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core)); //ULOCK_TCPIP_COREx();
//}


/**
  * @brief  updates the link states
  * @param  headstr
  * @retval bmsr 7649: disconnect, 764d: connection, usually
  */
#define dm9051_set_flags(flg, set_flags)     do { flg = (uint8_t)(flg | (set_flags)); } while(0)
#define dm9051_clear_flags(flg, clr_flags)   do { flg = (uint8_t)(flg & (uint8_t)(~(clr_flags) & 0xff)); } while(0)
#define dm9051_is_flag_set(flg, flag)        ((flg & (flag)) != 0)

static uint8_t lw_flag;
#define DM9051_FLAG_LINK_UP				0x01U
#define	link_flag()	dm9051_is_flag_set(lw_flag, DM9051_FLAG_LINK_UP)
void dm9051_debug_flags(char *headstr) {
	if (headstr) {
		printf("%s: %s\r\n", headstr, (lw_flag & DM9051_FLAG_LINK_UP) ? "linkup" : "linkdown");
		return;
	}
	printf("dm9051 %s\n", (lw_flag & DM9051_FLAG_LINK_UP) ? "linkup" : "linkdown");
}

void dm9051_update_flags(char *querystr, int linkup) {
	if (linkup)
		dm9051_set_flags(lw_flag, DM9051_FLAG_LINK_UP);
	else
		dm9051_clear_flags(lw_flag, DM9051_FLAG_LINK_UP);

	dm9051_debug_flags(querystr);
}

uint16_t dm_update_phyread(char *queryStr) {
	uint16_t bmsr = cspi_phy_read(PHY_STATUS_REG); 
	if (!link_flag() && (bmsr & PHY_LINKED_BIT))
		dm9051_update_flags(queryStr, 1);
	else if (link_flag() && !(bmsr & PHY_LINKED_BIT))
		dm9051_update_flags(queryStr, 0);
	return bmsr;
}

void dm9051_probe_link(int nsr_poll)
{
	//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
	//printf("DM9051 _dm_update_phyread: %d\r\n", nsr_poll);
		printf("dm9051_probe_link: %d\r\n", nsr_poll);
		while (--nsr_poll /*&& !(_dm_update_phyread("init_probe_link") & PHY_LINKED_BIT)*/) {
			uint16_t bmsr = dm_update_phyread("dm9051_probe_link");
			if (bmsr & PHY_LINKED_BIT)
				break;
			printf("dm9051_probe_link: %d, bmsr %04x\r\n", nsr_poll, bmsr);
			dm_delay_ms(10);
		}
	//printf("DM9051 _dm_update_phyread: %d\r\n", nsr_poll);
	//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));
}
