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
#include "dm9051_env_identify.h"

#include "cboard/dm9051_cboard_data_API.h" //for void DM9051_Configuration(), dm_delay_ms()
// for cint_enable_mcu_irq()

#define PBUF_POOL_BUFSIZE (1514 + 4) //.2000	//.2000(tested) //dm,jj=1514

uint16_t impl_dm9051_rx(uint8_t *buff)
{
	uint8_t rxbyte, rx_status;
	uint8_t ReceiveData[4];
	uint16_t rx_len, pad = 0; // 8-bit
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
	pad = rx_len & 1;				  // 16-bit
	cspi_rx_read(buff, rx_len + pad); // 8/16-bit
	return rx_len;
}

uint16_t dm9051_rx(uint8_t *buff)
{
	uint16_t rx_len;
	// DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
	rx_len = impl_dm9051_rx(buff);
	// DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));
	return rx_len;
}

void dm9051_tx(uint8_t *buf, uint16_t len)
{
	// DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
	cspi_tx_write(buf, len);
	cspi_tx_req();
	// DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));
}

int display_identity(char *spiname, uint16_t id, uint8_t *ids, uint8_t id_adv, uint16_t idin, char *tail)
{
	DM_UNUSED_ARG(id);
	DM_UNUSED_ARG(id_adv);
	if (ids)
	{
	}
	printf("%s %s ::: Chip ID %04x\r\n", //"%s[%d]"
		   tail,						 //": Read device", 0,
		   spiname,
		   idin);
	return 0;
}

const uint8_t *impl_dm9051_init(const uint8_t *adr)
{
	uint16_t id;
	const uint8_t *mac = NULL;
	if (env_init_setup(&id))
	{
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
	// DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
	// DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));
	return impl_dm9051_init(adr);
}

uint16_t dm9051_read_control_status(void)
{
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

uint16_t dm9051_read_chip_id(void)
{
#if 0
	//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core)); //LOCK_TCPIP_COREx();
	//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core)); //ULOCK_TCPIP_COREx();
	return cspi_read_chip_id();
#else
	return 0x9051;
#endif
}
uint16_t dm9051_read_bmsr(void)
{
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

void dm9051_read_rx_pointers(uint16_t *rwpa_wt, uint16_t *mdra_rd)
{
	// LOCK_TCPIP_COREx();
	cspi_read_rx_pointers(rwpa_wt, mdra_rd);
	// ULOCK_TCPIP_COREx();
}

// uint16_t dm9051_isr_disab(void) {
//	DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core)); //LOCK_TCPIP_COREx();
//	return 0;
//	DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core)); //ULOCK_TCPIP_COREx();
// }

void dm9051_write_rst_isr(void)
{
	cspi_isr_enab();
}

/**
 * @brief  updates the link states
 * @param  headstr
 * @retval bmsr 7649: disconnect, 764d: connection, usually
 */
#define dm9051_set_flags(flg, set_flags)    \
	do                                      \
	{                                       \
		flg = (uint8_t)(flg | (set_flags)); \
	} while (0)
#define dm9051_clear_flags(flg, clr_flags)                     \
	do                                                         \
	{                                                          \
		flg = (uint8_t)(flg & (uint8_t)(~(clr_flags) & 0xff)); \
	} while (0)
#define dm9051_is_flag_set(flg, flag) ((flg & (flag)) != 0)

static uint8_t lw_flag;
#define DM9051_FLAG_LINK_UP 0x01U
#define link_flag() dm9051_is_flag_set(lw_flag, DM9051_FLAG_LINK_UP)
void dm9051_debug_flags(char *headstr)
{
	if (headstr)
	{
		printf("%s: %s\r\n", headstr, (lw_flag & DM9051_FLAG_LINK_UP) ? "linkup" : "linkdown");
		return;
	}
	printf("dm9051 %s\n", (lw_flag & DM9051_FLAG_LINK_UP) ? "linkup" : "linkdown");
}

void dm9051_update_flags(char *querystr, int linkup)
{
	if (linkup)
		dm9051_set_flags(lw_flag, DM9051_FLAG_LINK_UP);
	else
		dm9051_clear_flags(lw_flag, DM9051_FLAG_LINK_UP);

	dm9051_debug_flags(querystr);
}

uint16_t dm_update_phyread(char *queryStr)
{
	uint16_t bmsr = cspi_phy_read(PHY_STATUS_REG);
	if (!link_flag() && (bmsr & PHY_LINKED_BIT))
		dm9051_update_flags(queryStr, 1);
	else if (link_flag() && !(bmsr & PHY_LINKED_BIT))
		dm9051_update_flags(queryStr, 0);
	return bmsr;
}

void dm9051_probe_link(int nsr_poll)
{
	// DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
	// printf("DM9051 _dm_update_phyread: %d\r\n", nsr_poll);
	printf("dm9051_probe_link: %d\r\n", nsr_poll);
	while (--nsr_poll /*&& !(_dm_update_phyread("init_probe_link") & PHY_LINKED_BIT)*/)
	{
		uint16_t bmsr = dm_update_phyread("dm9051_probe_link");
		if (bmsr & PHY_LINKED_BIT)
			break;
		printf("dm9051_probe_link: %d, bmsr %04x\r\n", nsr_poll, bmsr);
		dm_delay_ms(10);
	}
	// printf("DM9051 _dm_update_phyread: %d\r\n", nsr_poll);
	// DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));
}

// ---------------------- cspi -------------------------------------------------------------
#include "cboard/dm9051_cstate.h"

void cspi_phycore_on(uint16_t nms)
{
	// u8 gpcr = cspi_read_reg(DM9051_GPCR);
	// cspi_write_reg(DM9051_GPCR, gpcr | 0x01); //bit-0
	cspi_write_reg(DM9051_GPR, 0x00); // Power on PHY
	dm_delay_ms(nms);
}

void cspi_ncr_reset(uint16_t nms)
{
	cspi_write_reg(DM9051_NCR, DM9051_REG_RESET); // iow(DM9051_NCR, NCR_RST);
	dm_delay_ms(nms);							  // dm9051_delay_in_core_process(nms, "_core_reset<>"); //dm_delay_ms(250); //CH390-Est-Extra
}

uint16_t cspi_phy_read(uint16_t uReg)
{
	int w = 0;
	uint16_t uData;

	cspi_write_reg(DM9051_EPAR, DM9051_PHY | uReg);
	cspi_write_reg(DM9051_EPCR, 0xc);
	dm_delay_us(1);
	while (cspi_read_reg(DM9051_EPCR) & 0x1)
	{
		dm_delay_us(1);
		if (++w >= 500) // 5
			break;
	} // Wait complete

	cspi_write_reg(DM9051_EPCR, 0x0);
	uData = (cspi_read_reg(DM9051_EPDRH) << 8) | cspi_read_reg(DM9051_EPDRL);

	return uData;
}

void cspi_phy_write(uint16_t reg, uint16_t value)
{
	int w = 0;

	cspi_write_reg(DM9051_EPAR, DM9051_PHY | reg);
	cspi_write_reg(DM9051_EPDRL, (value & 0xff));
	cspi_write_reg(DM9051_EPDRH, ((value >> 8) & 0xff));
	/* Issue phyxcer write command */
	cspi_write_reg(DM9051_EPCR, 0xa);
	dm_delay_us(1);
	while (cspi_read_reg(DM9051_EPCR) & 0x1)
	{
		dm_delay_us(1);
		if (++w >= 500) // 5
			break;
	} // Wait complete

	cspi_write_reg(DM9051_EPCR, 0x0);
}

void cspi_soft_default(void)
{
	uint8_t val = MBNDRY_WORD;					   // 16-bit
	cspi_write_reg(DM9051_MBNDRY, val);			   /* MemBound */
	cspi_write_reg(DM9051_PPCR, PPCR_PAUSE_COUNT); // iow(DM9051_PPCR, PPCR_SETTING); //#define PPCR_SETTING 0x08
	cspi_write_reg(DM9051_LMCR, LMCR_MODE1);
	cspi_write_reg(DM9051_INTR, INTR_ACTIVE_LOW); // interrupt active low
	identify_irq_stat(ISTAT_LOW_ACTIVE);
	trace_irq_stat(ISTAT_LOW_ACTIVE);
}

void cspi_core_reset(void)
{
	cspi_ncr_reset(2);
	cspi_phycore_on(25);
	cspi_soft_default();
}

uint16_t cspi_read_chip_id(void)
{
	uint8_t buff[2];
	cspi_read_regs(DM9051_PIDL, buff, 2, CS_EACH);
	return buff[0] | buff[1] << 8;
}

void cspi_vid_pid_revisions(uint8_t *ids, uint8_t *rev_ad)
{
	cspi_read_regs(DM9051_VIDL, ids, 5, CS_EACH); // OPT_CS(csmode)
	cspi_read_regs(0x5C, rev_ad, 1, CS_EACH);	  // OPT_CS(csmode)
}

uint16_t cspi_read_control_status(void)
{
	uint8_t buff[2];
	cspi_read_regs(DM9051_NCR, buff, 2, CS_EACH);
	return buff[0] | buff[1] << 8;
}

void cspi_read_rx_pointers(uint16_t *rwpa_wt, uint16_t *mdra_rd)
{
	*rwpa_wt = (uint16_t)cspi_read_reg(0x24) | (uint16_t)cspi_read_reg(0x25) << 8; // DM9051_RWPAL
	*mdra_rd = (uint16_t)cspi_read_reg(0x74) | (uint16_t)cspi_read_reg(0x75) << 8; // DM9051_MRRL;
}

#if (POLL_ON == POLL_ON_RXPADIFF)
uint16_t cspi_diff_rxpa(void)
{
	static uint16_t /*rwpa_wts,*/ s_mdra_rds = 0;
	uint16_t rwpa_wtp, mdra_rdp;

	cspi_read_rx_pointers(&rwpa_wtp, &mdra_rdp);

	if (!s_mdra_rds)
		s_mdra_rds = mdra_rdp;

	return wrpadiff(s_mdra_rds, mdra_rdp);
}
#endif

void cspi_set_par(const uint8_t *macadd)
{
	int i;
	for (i = 0; i < 6; i++)
		cspi_write_reg(DM9051_PAR + i, macadd[i]);
}

void cspi_set_mar(void)
{
	int i;
	for (i = 0; i < 8; i++)
		cspi_write_reg(DM9051_MAR + i, (i == 7) ? 0x80 : 0x00);
}

void cspi_set_recv(void)
{
#if 0
	cspi_write_reg(DM9051_FCR, FCR_DEFAULT); //iow(DM9051_FCR, FCR_FLOW_ENABLE);
	phy_write 04, flow
#endif
	identify_irq_stat(ISTAT_DM_IMR);
	trace_irq_stat(ISTAT_DM_IMR);
	cspi_write_reg(DM9051_IMR, IMR_PAR | IMR_PRM); // iow(DM9051_IMR, IMR_PAR | IMR_PTM | IMR_PRM);

	identify_irq_stat(ISTAT_DM_RCR);
	trace_irq_stat(ISTAT_DM_RCR);
	cspi_write_reg(DM9051_RCR, RCR_DEFAULT | RCR_RXEN); // dm9051_fifo_RX_enable();
	// trace_dm9051_set_recv();
}

void cspi_rx_mode(void)
{
	//=impl_dm9051_set_mar();
	cspi_set_mar();

	//=impl_dm9051_set_recv();
	cspi_set_recv();
}

const uint8_t *cspi_dm_start1(const uint8_t *adr)
{
	cint_enable_mcu_irq(); // impl_enable_mcu_irq();
	cspi_set_par(adr);
	cspi_rx_mode();
	return adr;
}

void cspi_rx_head(uint8_t *receivedata)
{
	cspi_read_mem(receivedata, 4);
	cspi_write_reg(DM9051_ISR, 0x80);
}

void cspi_rx_read(uint8_t *buff, uint16_t rx_len)
{
	cspi_read_mem(buff, rx_len);
	cspi_write_reg(DM9051_ISR, 0x80);
}

void cspi_tx_write(uint8_t *buf, uint16_t len)
{
	uint16_t pad = 0; // 8-bit
	cspi_write_reg(DM9051_TXPLL, len & 0xff);
	cspi_write_reg(DM9051_TXPLH, (len >> 8) & 0xff);
	pad = len & 1;					// 16-bit
	cspi_write_mem(buf, len + pad); // 8/16-bit
}

#define DM9051_TX_DELAY(expression, handler) \
	do                                       \
	{                                        \
		if ((expression))                    \
		{                                    \
			handler;                         \
		}                                    \
	} while (0)

void cspi_tx_req(void)
{
	cspi_write_reg(DM9051_TCR, TCR_TXREQ); /* Cleared after TX complete */
	DM9051_TX_DELAY((cspi_read_reg(DM9051_TCR) & TCR_TXREQ), dm_delay_us(5));
}

uint16_t cspi_isr_enab(void)
{
	uint16_t isrs;
	// DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
	isrs = cspi_read_reg(DM9051_ISR);
	cspi_write_reg(DM9051_ISR, (uint8_t)isrs);
	// DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));
	return isrs | (0xff << 8);
}

// ---------------------- env -------------------------------------------------------------

/* Only used, in [ env.c ] */
typedef uint8_t mac_t[MAC_ADDR_LENGTH];
typedef uint8_t ip_t[ADDR_LENGTH];

#define DM_TYPE 1
#include "dm_identify_types.h"

#define DM_TYPE 2
#include "dm_identify_types.h"

/*
 * candidate
 */
typedef struct eth_node_st
{
	uint8_t mac_addresse[MAC_ADDR_LENGTH];
	uint8_t local_ipaddr[ADDR_LENGTH];
	uint8_t local_gwaddr[ADDR_LENGTH];
	uint8_t local_maskaddr[ADDR_LENGTH];
} eth_node_t;

//(define is as rather than '_ETHERNET_COUNT', refer to as '_BOARD_SPI_COUNT' counter) define BOARD_SPI_COUNT 'N'
const eth_node_t node_candidate[1] = {
	{
		{
			0,
			0x60,
			0x6e,
			0x00,
			0x00,
			0x17,
		},
		{192, 168, 6, 17},
		{192, 168, 6, 1},
		{255, 255, 255, 0},
	}, /*
	   { \
		   {0, 0x60, 0x6e, 0x00, 0x01, 0x26,}, \
		   {192, 168, 6,  26}, \
		   {192, 168, 6,   1}, \
		   {255, 255, 255, 0}, \
	   }, \
	   { \
		   {0, 0x60, 0x6e, 0x00, 0x01, 0x25,}, \
		   {192, 168, 6,  25}, \
		   {192, 168, 6,   1}, \
		   {255, 255, 255, 0}, \
	   }, \
	   { \
		   {0, 0x60, 0x6e, 0x00, 0x01, 0xfe,}, \
		   {192, 168, 6,  66}, \
		   {192, 168, 6,   1}, \
		   {255, 255, 255, 0}, \
	   }, \
	   { \
		   {0, 0x60, 0x6e, 0x00, 0x01, 0xff,}, \
		   {192, 168, 6,  67}, \
		   {192, 168, 6,   1}, \
		   {255, 255, 255, 0}, \
	   }, \
	   */
};

/*
 * HCC: Hard Core Candidate (hcc)
 */
#define candidate_eth_mac() &node_candidate[0].mac_addresse[0]	  //[pin_code]
#define candidate_eth_ip() &node_candidate[0].local_ipaddr[0]	  //[pin_code]
#define candidate_eth_gw() &node_candidate[0].local_gwaddr[0]	  //[pin_code]
#define candidate_eth_mask() &node_candidate[0].local_maskaddr[0] //[pin_code]

const uint8_t *identify_eth_mac(const uint8_t *macadr)
{
	SET_FIELD(final_mac, macadr ? macadr : candidate_eth_mac()); // DM_SET_FIELDmac(macadr);
	return GET_FIELD(final_mac);
}

uint8_t *identify_tcpip_ip(uint8_t *ip4adr)
{
	SET_FIELD(final_ip, ip4adr ? ip4adr : candidate_eth_ip()); // DM_SET_FIELD_ips(dm.final_ip, ip4adr); //DM_SET_FIELD(ip_t ,ip, ip4adr ? ip4adr : candidate_eth_ip());
	return GET_FIELD(final_ip);								   // DM_GET_FIELD_ips(dm.final_ip); //return DM_GET_FIELD(ip_t, ip);
}
uint8_t *identify_tcpip_gw(uint8_t *ip4adr)
{
	SET_FIELD(final_gw, ip4adr ? ip4adr : candidate_eth_gw()); // DM_SET_FIELD_ips(dm.final_gw, ip4adr); //DM_SET_FIELD(ip_t ,gw, ip4adr ? ip4adr : candidate_eth_gw());
	return GET_FIELD(final_gw);								   // DM_GET_FIELD_ips(dm.final_gw); //return DM_GET_FIELD(ip_t, gw);
}
uint8_t *identify_tcpip_mask(uint8_t *ip4adr)
{
	SET_FIELD(final_mask, ip4adr ? ip4adr : candidate_eth_mask()); // DM_SET_FIELD_ips(dm.final_mask, ip4adr); //DM_SET_FIELD(ip_t ,mask, ip4adr ? ip4adr : candidate_eth_mask());
	return GET_FIELD(final_mask);								   // DM_GET_FIELD_ips(dm.final_mask); //return DM_GET_FIELD(ip_t, mask);
}

void trace_identified_eth_mac(int showf)
{
	if (showf)
	{
		const uint8_t *mac = GET_FIELD(final_mac);
		printf("mac address %02x%02x%02x%02x%02x%02x\r\n",
			   mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	}
}

// const uint8_t *identified_eth_mac(void) {
//	return GET_FIELD(final_mac); //DM_GET_FIELDmac(); //DM_GET_FIELD(mac_t, final_mac);
// }
// uint8_t *identified_tcpip_ip(void) {
//	return GET_FIELD(final_ip); //DM_GET_FIELD_ips(dm.final_ip); //return DM_GET_FIELD(ip_t, ip);
// }
// uint8_t *identified_tcpip_gw(void) {
//	return GET_FIELD(final_gw); //DM_GET_FIELD_ips(dm.final_gw); //return DM_GET_FIELD(ip_t, gw);
// }
// uint8_t *identified_tcpip_mask(void) {
//	return GET_FIELD(final_mask); //DM_GET_FIELD_ips(dm.final_mask); //return DM_GET_FIELD(ip_t, mask);
// }

int env_init_setup(uint16_t *id)
{
	uint8_t rev = cspi_read_reg(0x5c);
	*id = cspi_read_chip_id();
	if (*id == 0x9000)
		*id = 0x9051;

	// rev = cspi_read_reg(0x5c);

	if (*id != 0x9051)
	{
		printf("DM9051 not found, chipid: %04x\r\n", *id);
		printf("system stop\r\n");
		while (1)
			;
	}
	printf("\r\n"); // printk("\r\n");
	printf("DM9051 chip rev: %02x\r\n", rev);
	printf("DM9051 found: %04x\r\n", *id);
	return 1;
}

/*
 * cboard_env_func
 */
#define TIMES_TO_RST 10

static void dm9051_show_rxbstatistic(uint8_t *htc, int n)
{
	int i, j;

	//.dm9051_chipid(); //Also [test][test][test].init
	printf("SHW rxbStatistic, 254 wrngs\r\n");
	for (i = 0; i < (n + 2); i++)
	{
		if (!(i % 32) && i)
			printf("\r\n");
		if (!(i % 32) || !(i % 16))
			printf("%02x:", i);
		// if (!(i%16)) printf(" ");
		if (!(i % 8))
			printf(" ");
		if (i == 0 || i == 1)
		{
			printf("  ");
			continue;
		}
		j = i - 2;
		printf("%d ", htc[j]);
	}
	printf("\r\n");
}

static uint8_t ret_fire_time(uint8_t *histc, int csize, int i, uint8_t rxb)
{
	printf(" _dm9051f rxb %02x (times %2d)%c\r\n", rxb, histc[i], (histc[i] == 2) ? '*' : ' ');
	if (histc[i] >= TIMES_TO_RST)
	{
		dm9051_show_rxbstatistic(histc, csize);
		histc[i] = 1;
		return TIMES_TO_RST;
	}
	return 0;
}

uint16_t env_evaluate_rxb(uint8_t rxb)
{
	int i;
	static uint8_t histc[254] = {0}; // static int rff_c = 0 ...;
	uint8_t times = 1;

	for (i = 0; i < sizeof(histc); i++)
	{
		if (rxb == (i + 2))
		{
			histc[i]++;
			times = ret_fire_time(histc, sizeof(histc), i, rxb);
			if (times == 0) // As: Hdlr (times : 0 or TIMES_TO_RST)
				return 0;
			return env_err_rsthdlr("_dm9051f rxb error accumunation times : %u\r\n", times);
		}
	}
	return env_err_rsthdlr("dm9 impossible path error times : %u\r\n", times); // As: Hdlr (times : 1)
}

const uint8_t *env_reset_process(const uint8_t *macaddr, enable_t en)
{
	cspi_core_reset(); // As: printf("rstc %d ,because rxb %02x (is %d times)\r\n", rstc, rxbyte, times);

	if (en)
	{
		macaddr = cspi_dm_start1(macaddr);
	}
	return macaddr;
}

uint16_t env_err_rsthdlr(char *err_explain_str, uint32_t valuecode) //,int ret_cond //u8 zerochk
{
	printf(err_explain_str, valuecode); // or "0x%02x"
	env_reset_process(identified_eth_mac(), DM_TRUE);
	return 0;
}

//// ---------------------- data_impl -------------------------------------------------------------

// #define DM9051_MRCMDX       (0x70)  //Read_Mem2X
// #define DM9051_MRCMD        (0x72)  //Read_Mem
// #define DM9051_MWCMD        (0x78)  //Write_Mem

// #define OPC_REG_W       		0x80  // Register Write
// #define OPC_REG_R       		0x00  // Register Read

// #if defined (_DLW_M051xx)
////[NU cspi_io]
// #define	dm9051_spi_cmd_write(wb)		NU_spi_exc_data_head(wb)
// #define dm9051_spi_data_read() 			NU_spi_exc_data_read()
// #define	dm9051_spi_read_end()			NU_spi_exc_data_read_end()
// #define	dm9051_spi_data_write(wb)		NU_spi_exc_data_write(wb)
// #define	dm9051_spi_write_end()			NU_spi_exc_data_write_end()

// uint8_t NU_spi_data_read(uint8_t reg)
//{
//	uint8_t val;
//	dm9051_spi_cmd_write(reg | OPC_REG_R);
//	val = dm9051_spi_data_read();
//	dm9051_spi_read_end();
//	return val;
// }
// void NU_spi_data_write(uint8_t reg, uint8_t val)
//{
//	dm9051_spi_cmd_write(reg | OPC_REG_W);
//	dm9051_spi_data_write(val);
//	dm9051_spi_write_end();
// }

// uint8_t NU_spi_mem2x_read(void)
//{
//	uint8_t rxb;
//	rxb = cspi_read_reg(DM9051_MRCMDX);
//	rxb = cspi_read_reg(DM9051_MRCMDX);
//	return rxb;
// }

// void NU_spi_mem_read(uint8_t *buf, uint16_t len)
//{
//	int i;
//	dm9051_spi_cmd_write(DM9051_MRCMD | OPC_REG_R);
//	for(i=0; i<len; i++)
//		buf[i] = dm9051_spi_data_read();
//	dm9051_spi_read_end();
// }

// void NU_spi_mem_write(uint8_t *buf, uint16_t len)
//{
//	int i;
//	dm9051_spi_cmd_write(DM9051_MWCMD | OPC_REG_W);
//	for(i=0; i<len; i++)
//		dm9051_spi_data_write(buf[i]);
//	dm9051_spi_write_end();
// }
// #endif

// ---------------------- cstate -------------------------------------------------------------
#define CB_TYPE 0
#include "cboard/cb_types2.h"

#define CB_TYPE 1
#include "cboard/cb_types2.h"

#define CB_TYPE 2
#include "cboard/cb_types2.h"

/*
 * cboard_trace_irq_flow
 */

// uint16_t irqst = 0;

void deidentify_irq_stat(uint16_t bitflg)
{
	// irqst &= ~bitflg; //return DM_GET_FIELD(uint16_t, irqst);
	SET_CSTATE(irqst, GET_CSTATE(irqst) & ~bitflg); // DM_SET_FIELDirqst(DM_GET_FIELDirqst() & ~bitflg);
}
void identify_irq_stat(uint16_t bitflg)
{
	// irqst |= bitflg; //DM_SET_FIELD(uint16_t, irqst, DM_GET_FIELD(uint16_t, irqst) | bitflg); //store into dm9051optsex[i].read_chip_id
	SET_CSTATE(irqst, GET_CSTATE(irqst) | bitflg); // DM_SET_FIELDirqst(DM_GET_FIELDirqst() | bitflg);
}

void trace_irq_stat(uint16_t bitflg)
{
	char istat_term[22];
	switch (bitflg)
	{
	case ISTAT_IRQ_CFG:
		sprintf(istat_term, "[IRQ_CFG]");
		break;
	case ISTAT_IRQ_ENAB:
		sprintf(istat_term, "[IRQ_ENAB]");
		break;
	case ISTAT_DM_IMR:
		sprintf(istat_term, "(IMR.pr)");
		break;
	case ISTAT_DM_RCR:
		sprintf(istat_term, "(RCR.rxen)");
		break;
	case ISTAT_LOW_TRIGGER:
		sprintf(istat_term, "[IRQ_LOW_TRIGGER]");
		break;
	case ISTAT_LOW_ACTIVE:
		sprintf(istat_term, "(INTR.lo)");
		break;
	case ISTAT_IRQ_NOW2:
		sprintf(istat_term, "(INT %lu)", get_task_tryint());
		break;
	case ISTAT_IRQ_NOW:
	default:
		istat_term[0] = 0;
		break;
	}

	if (get_task_tryint() > 5 && (bitflg == ISTAT_IRQ_NOW || bitflg == ISTAT_IRQ_NOW2))
		return;

	printf("[irq_stat]:                   ------------> irqst= %02x on add %02x %s\r\n", identified_irq_stat(), bitflg, istat_term);
}

uint16_t identified_irq_stat(void)
{
	// return irqst;
	return GET_CSTATE(irqst);
}

//---------------------------------------

unsigned long dispc_int_active = 0; //, dispc_int_active_saved = 0;

void inc_task_tryint(void)
{
	dispc_int_active++;
	// at32_led_toggle(LED2);
}

unsigned long get_task_tryint(void)
{
	return dispc_int_active;
}

//---------------------------------------

uint16_t wrpadiff(uint16_t rwpa_s, uint16_t rwpa_e)
{
	return (rwpa_e >= rwpa_s) ? rwpa_e - rwpa_s : (rwpa_e + 0x4000 - 0xc00) - rwpa_s;
}

//---------------------------------------
/*
 * cboard_print_hex
 */

#define kkmin(a, b) (a < b) ? a : b

static void printf_space(int n)
{
	while (n--)
		printf("%c", ' ');
}

static int printf_space_init(size_t tlen)
{
	char cspace[16];
	int n = sprintf(cspace, "rxlen %4d", tlen);
#if 1
	printf_space(n);
	printf(" %s\r\n", cspace);
#endif
	return n;
}

static int printf_rxlen_head(size_t tlen, int nspc)
{
#if 1
	if (!nspc)
		nspc = printf_space_init(tlen);

	printf_space(nspc);
	return nspc;
#endif
}

void sprint_hex_dump0(int head_space, int titledn, char *prefix_str,
					  size_t tlen, int rowsize, const void *buf, int seg_start, size_t len, /*int useflg*/ int cast_lf) //, int must, int dgroup
{
	// #undef printf
	// #define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	// if (useflg) {
	int si, se, titlec = 0;
	int i, linelen, remaining = len; // hs, const eth_class_t *ec = &eclass[10];
	int nspace = 0;

	(void)head_space;
	//(void) tlen;

	si = seg_start;
	se = seg_start + len;
	for (i = si; i < se; i += rowsize)
	{
		// unsigned
		char linebuf[(12 * 3) + (3 * 16) + 1 + 32]; // here!

		nspace = printf_rxlen_head(tlen, nspace);

		linelen = kkmin(remaining, rowsize);
		remaining -= rowsize;
		do
		{
			const uint8_t *ptr = buf;
			int j;
			// unsigned char *
			int nb = 0;
			for (j = 0; j < linelen && (size_t)nb < sizeof(linebuf); j++)
			{
				if (j && !(j % 8))
					nb += snprintf(linebuf + nb, sizeof(linebuf) - nb, " ");
				if (((rowsize >> 1) != 8) && !(j % (rowsize >> 1)))
					nb += snprintf(linebuf + nb, sizeof(linebuf) - nb, " ");

				nb += snprintf(linebuf + nb, sizeof(linebuf) - nb, "%02x ", *(ptr + i + j));
			}
		} while (0);

		//.nspace = printf_rxlen_head(tlen, nspace);

#if 0
			hs = head_space;
			while(hs--)
#endif
		printf(" ");

		if (prefix_str)
		{
			printf("(%s) %.3x %s", prefix_str, i, linebuf); //"%s", ec->str //CHECK (XXX >> )
			while (titledn)
			{
				titledn--;
				prefix_str[titlec++] = ' ';
			}
		}
		else
		{
			printf("(dm9 xfer) %.3x %s", i, linebuf); //"%s", ec->str
		}

		if ((i + rowsize) < se)
			printf("\r\n");
		else
		{
			if (cast_lf)
				printf("\r\n");
#if 0
				if (IS_UDP) {
					//ptr
#if 0
					size_t ulen = tlen; // larger for with 4-bytes CRC
					ulen = UIP_LLH_LEN;
					ulen += HTONS(UDPBUF->udplen) - 8;
					ulen += sizeof(struct uip_udpip_hdr); // correct for without 4-bytes CRC (htons)

					if (cast_lf)
						printf("\r\n");

					printf(" ..SrcIP %d.%d.%d.%d", (IPBUF->srcipaddr[0] >> 0) & 0xff, (IPBUF->srcipaddr[0] >> 8),
						(IPBUF->srcipaddr[1] >> 0) & 0xff, (IPBUF->srcipaddr[1] >> 8));
					printf("  DestIP %d.%d.%d.%d", (IPBUF->destipaddr[0] >> 0) & 0xff, (IPBUF->destipaddr[0] >> 8),
						(IPBUF->destipaddr[1] >> 0) & 0xff, (IPBUF->destipaddr[1] >> 8));
					printf("  Len %d", ulen);
					printf("  (%5d -> %d Len %d)", UDPBUF->srcport, UDPBUF->destport, HTONS(UDPBUF->udplen) - 8);
					printf("\r\n");
#endif
				}
				if (IS_TCP) {
					size_t ulen = tlen; // larger for with 4-bytes CRC
					printf(" ..SrcIP %d.%d.%d.%d", (IPBUF->srcipaddr[0] >> 0) & 0xff, (IPBUF->srcipaddr[0] >> 8),
						(IPBUF->srcipaddr[1] >> 0) & 0xff, (IPBUF->srcipaddr[1] >> 8));
					printf("  DestIP %d.%d.%d.%d", (IPBUF->destipaddr[0] >> 0) & 0xff, (IPBUF->destipaddr[0] >> 8),
						(IPBUF->destipaddr[1] >> 0) & 0xff, (IPBUF->destipaddr[1] >> 8));
					printf("  Len %d", ulen);
					
					if (TCPBUF->flags == 0x18)
						printf("  (%5d -> %d) flags %02x (PSH, ACK)", HTONS(TCPBUF->srcport), HTONS(TCPBUF->destport), TCPBUF->flags);
					else
						printf("  (%5d -> %d) flags %02x", HTONS(TCPBUF->srcport), HTONS(TCPBUF->destport), TCPBUF->flags);
					
					printf("\r\n");
				}
#endif
			// else
			//	printf("\r\n");
		}
	}
	//}
	// #undef printf
	// #define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

/* print log
 */

#if DM_ETH_DEBUG_MODE
// void dm_eth_input_hexdump_reset(void) {
//	link_log_reset_allow_num = 0;
// }

int link_log_reset_allow_num = 0;
const int rx_modle_log_reset_allow_num = 3;
#define limit_len(n, nTP) ((n <= nTP) ? n : nTP)

void dm_eth_input_hexdump(const void *buf, size_t len)
{
#if 1
// int get_tcpip_thread_state(void);
// void set_tcpip_thread_state(int state); //.temp //extern int tcpip_thread_init;
#endif
	int titledn = 0;
	DM_UNUSED_ARG(buf);
	DM_UNUSED_ARG(len);

	if (link_log_reset_allow_num < rx_modle_log_reset_allow_num)
	{
		link_log_reset_allow_num++;
		// if (link_log_reset_allow_num == rx_modle_log_reset_allow_num && get_tcpip_thread_state() == 1) {
		// set_tcpip_thread_state(6);
		//}
		sprint_hex_dump0(2, titledn, "dm9 head   <<rx", len, 32, buf, 0, limit_len(len, 66), DM_TRUE);
	}
}
#endif

// ---------------------- xx -------------------------------------------------------------

// ---------------------- xx -------------------------------------------------------------
