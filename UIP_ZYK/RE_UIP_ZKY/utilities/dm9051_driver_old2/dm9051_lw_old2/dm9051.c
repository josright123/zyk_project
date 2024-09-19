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
 * Date: 20240215 (Versions log please refer to 'dm9051_lw/info.txt')
 */

/* SPI master control is essential,
 * Do define spi specific definition, depend on your CPU's board support package library.
 * Here we had the definition header file with : "at32f415_spi.h"
 * for AT32F415 cpu.
 */
#include "dm9051opts.h"
#include "dm9051_lw.h"
#include "dm9051_lw_cboard.h"
#include "dm9051_lw_cspi.h"
#include "dm9051_lw_debug.h"
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))

#if 0
//	//static const uint8_t *impl_dm9051_init(const uint8_t *adr)
//	//{
//	//	return NULL;
//	//}
//	
//	#define DM9051_RX_RERXB(expression, handler) do { if ((expression)) { \
//	  handler;}} while(0)
//	#define DM9051_RX_BREAK(expression, handler) do { if ((expression)) { \
//	  handler;}} while(0)

//	#define	DM9051_Read_Rxb	DM9051_Read_Mem2X

//	//uint16_t impl_dm9051_rx(uint8_t *buff);
//	uint16_t impl_dm9051_rx0(uint8_t *buff)
//	{
//		return rx_len;
//	}

//	#define DM9051_TX_DELAY(expression, handler) do { if ((expression)) { \
//	  handler;}} while(0)

//	void impl_dm9051_tx0(uint8_t *buf, uint16_t len)
//	{
//	}
#endif

const uint8_t *dm9051_init(const uint8_t *adr)
{
	const uint8_t *mac;
	LOCK_TCPIP_COREx();
	impl_reset_pulse();
	mac = impl_dm9051_init(adr);
	ULOCK_TCPIP_COREx();
	return mac;
}

uint16_t dm9051_rx(uint8_t *buff)
{
	uint16_t len;
	LOCK_TCPIP_COREx();
	len = impl_dm9051_rx1(buff);
	ULOCK_TCPIP_COREx();
	return len;
}

void dm9051_tx(uint8_t *buf, uint16_t len)
{
	LOCK_TCPIP_COREx();
	impl_dm9051_tx1(buf, len);
	ULOCK_TCPIP_COREx();
}

//noused
uint16_t dm9051_isr_disab(void) {
	uint16_t isrs;
	LOCK_TCPIP_COREx();
	isrs = impl_dm9051_isr_disab();
	ULOCK_TCPIP_COREx();
	return isrs;
}

uint16_t dm9051_isr_enab(void) //static uint16_t dm9051_isr_clean(void)
{
	uint16_t isrs;
	LOCK_TCPIP_COREx();
	isrs = impl_dm9051_isr_enab();
	ULOCK_TCPIP_COREx();
	return isrs;
}

//uint16_t dm9051_read_chip_id(void)
//{
//	uint16_t id;
//	LOCK_TCPIP_COREx();
//	id = impl_read_chip_id();
//	ULOCK_TCPIP_COREx();
//	return id;
//}

void dm9051_read_rx_pointers(u16 *rwpa_wt, u16 *mdra_rd) {
	LOCK_TCPIP_COREx();
	impl_read_rx_pointers(rwpa_wt, mdra_rd);
	ULOCK_TCPIP_COREx();
}

//uint16_t dm9051_bmsr_update(void)
//{
//  uint16_t val;
//  LOCK_TCPIP_COREx();
//  val = impl_phy_read(PHY_STATUS_REG);
//  if (!(val & 0x4))
//	dm9051_link_log_reset();
//  ULOCK_TCPIP_COREx();
//  return val;
//}

uint16_t dm9051_err_hdlr(char *errstr, u32 invalue, u8 zerochk)
{
	u16 ret;
	LOCK_TCPIP_COREx();
	ret = impl_dm9051_err_hdlr(errstr, PINCOD, invalue, zerochk);
	ULOCK_TCPIP_COREx();
	return ret;
}
