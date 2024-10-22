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
#include <stdio.h> /* formats */
#include <stdarg.h>
#include <string.h>
#include <stdlib.h> /* strtol() */

#include "config/conf.h"
#include "config/conf_core.h"
#include "utils/dm9051_eth_debug.h"

// Constants and Definitions
#define PBUF_POOL_BUFSIZE (1514 + 4) //.2000	//.2000(tested)

// Forward declarations
static uint16_t impl_dm9051_rx(uint8_t *buff);
static const uint8_t *impl_dm9051_init(const uint8_t *adr);
static void cspi_core_reset(void);
static const uint8_t *cspi_dm_start1(const uint8_t *adr);
static void cspi_set_par(const uint8_t *macadd);
static void cspi_rx_mode(void);

uint16_t dm9051_rx(uint8_t *buff)
{
	uint16_t rx_len;
	rx_len = impl_dm9051_rx(buff);
	return rx_len;
}

void dm9051_tx(uint8_t *buf, uint16_t len)
{
	cspi_tx_write(buf, len);
	cspi_tx_req();
}

const uint8_t *dm9051_init(const uint8_t *adr)
{
	const uint8_t *mac = impl_dm9051_init(adr);

#ifdef DM9051_DRIVER_INTERRUPT
	printf("[interrupt] %s successfully\r\n\r\n", __func__);
#else
	printf("[polling] %s successfully\r\n\r\n", __func__);
#endif
	return mac;
}

// uint16_t dm9051_isr_disab(void) {
//	return 0;
// }

uint16_t cspi_isr_enab(void) // read and/then write
{
	uint16_t isrs;
	isrs = cspi_read_reg(DM9051_ISR);
	cspi_write_reg(DM9051_ISR, (uint8_t)isrs);
	return isrs | (0xff << 8);
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

uint16_t cspi_read_chip_id(void)
{
	uint8_t buff[2];
	cspi_read_regs(DM9051_PIDL, buff, 2, CS_EACH);
	return buff[0] | buff[1] << 8;
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

void cspi_read_regs_info(uint8_t *stat)
{
	uint16_t cs;
	uint32_t pbm;

	pbm = cspi_phy_read(PHY_STATUS_REG); // pbm = dm9051_read_bmsr();
	pbm |= cspi_read_chip_id() << 16;	 // dm9051_read_chip_id() << 16;
	cs = cspi_read_control_status();	 // dm9051_read_control_status();

	stat[0] = cs & 0xff;
	stat[1] = (cs >> 8) & 0xff;
	stat[2] = (pbm >> 24) & 0xff;
	stat[3] = (pbm >> 16) & 0xff;
	stat[4] = (pbm >> 8) & 0xff;
	stat[5] = (pbm) & 0xff;
}

// void cspi_vid_pid_revisions(uint8_t *ids, uint8_t *rev_ad)
//{
//	cspi_read_regs(DM9051_VIDL, ids, 5, CS_EACH);
//	cspi_read_regs(0x5C, rev_ad, 1, CS_EACH);
// }

// #if (POLL_ON == POLL_ON_RXPADIFF)
// uint16_t cspi_diff_rxpa(void)
//{
//	static uint16_t /*rwpa_wts,*/ s_mdra_rds = 0;
//	uint16_t rwpa_wtp, mdra_rdp;
//	cspi_read_rx_pointers(&rwpa_wtp, &mdra_rdp);
//	if (!s_mdra_rds)
//		s_mdra_rds = mdra_rdp;
//	return wrpadiff(s_mdra_rds, mdra_rdp);
// }
// #endif

// ---------------------- cspi -------------------------------------------------------------

void cspi_phycore_on(uint16_t nms)
{
	// u8 gpcr = cspi_read_reg(DM9051_GPCR);
	// cspi_write_reg(DM9051_GPCR, gpcr | 0x01);
	cspi_write_reg(DM9051_GPR, 0x00); // Power on PHY
	dm_delay_ms(nms);
}

void cspi_ncr_reset(uint16_t nms)
{
	cspi_write_reg(DM9051_NCR, DM9051_NCR_RESET); // iow(NCR_RST);
	dm_delay_ms(nms); // dm9051_delay_in_core_process(nms, "_core_reset<>"); //dm_delay_ms(250); //CH-Est-Extra
}

void cspi_soft_default(void)
{
	uint8_t val = MBNDRY_WORD;					   // 16-bit
	cspi_write_reg(DM9051_MBNDRY, val);			   /* MemBound */
	cspi_write_reg(DM9051_PPCR, PPCR_PAUSE_COUNT); // iow(PPCR_SETTING);
	cspi_write_reg(DM9051_LMCR, LMCR_MODE1);
	cspi_write_reg(DM9051_INTR, INTR_ACTIVE_LOW); // interrupt active low
	identify_irq_stat(ISTAT_LOW_ACTIVE);
	trace_irq_stat(ISTAT_LOW_ACTIVE);
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
	cspi_write_reg(DM9051_FCR, FCR_DEFAULT); //iow(FCR_FLOW_ENABLE);
	phy_write 04, flow
	#endif
	identify_irq_stat(ISTAT_DM_IMR);
	trace_irq_stat(ISTAT_DM_IMR);
	cspi_write_reg(DM9051_IMR, IMR_PAR | IMR_PRM); // iow(IMR, IMR_PAR | IMR_PTM | IMR_PRM);

	identify_irq_stat(ISTAT_DM_RCR);
	trace_irq_stat(ISTAT_DM_RCR);
	cspi_write_reg(DM9051_RCR, RCR_DEFAULT | RCR_RXEN); // dm9051_fifo_RX_enable();
														// trace_dm9051_set_recv();
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

void cspi_tx_req(void)
{
	cspi_write_reg(DM9051_TCR, TCR_TXREQ); /* Cleared after TX complete */
	DM9051_TX_DELAY((cspi_read_reg(DM9051_TCR) & TCR_TXREQ), dm_delay_us(5));
}

// ---------------------- env -------------------------------------------------------------

int env_init_setup(uint16_t *id)
{
	uint8_t rev = cspi_read_reg(0x5c);
	*id = cspi_read_chip_id();
	if (*id == 0x9000)
		*id = 0x9051;

	if (*id != 0x9051)
	{
		printf("DM9051 not found, chipid: %04x\r\n", id);
		return 0;
	}

	printf("\r\n"); // printf("DM9051 chip rev: %02x\r\n", rev);
	printf("DM9051 found: %04x\r\n", *id);
	return 1;
}

/*
 * cboard_env_func
 */
#define TIMES_TO_RST 10

static void dm9051_show_rxbstatistic(uint8_t *htc, int n)
{
	int i;
#if (drv_print && PRINT_SEMA == SEMA_ON) // depend
	int j;
#endif

	printf("SHW rxbStatistic, 254 wrngs\r\n");
	for (i = 0; i < (n + 2); i++)
	{
		if (!(i % 32) && i)
			printf("\r\n");
		if (!(i % 32) || !(i % 16))
			printf("%02x:", i);
		if (!(i % 8))
			printf(" ");
		if (i == 0 || i == 1)
		{
			printf("  ");
			continue;
		}
#if (drv_print && PRINT_SEMA == SEMA_ON) // depend
		j = i - 2;
#endif
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
	return env_err_rsthdlr3("dm9 impossible path error times : %u\r\n", times); // As: Hdlr (times : 1)
}

static const uint8_t *env_reset_process(const uint8_t *macaddr)
{
	cspi_core_reset();
	return cspi_dm_start1(macaddr);
}

void err_callback(char *explain_str, uint32_t err_code) {
	char bff[180];
	sprintf(bff, explain_str, err_code);
	printf("%s", bff);
}

uint16_t env_err_rsthdlr(char *err_explain_str, uint32_t valuecode)
{
	//printf(err_explain_str, valuecode);
	char bff[180];
	sprintf(bff, err_explain_str, valuecode);
	printf("%s", bff);
	
	env_reset_process(identified_eth_mac());
	return 0;
}

uint16_t env_err_rsthdlr1(void (*callback)(char *, uint32_t), char *explain_str, uint32_t code)
{
    callback(explain_str, code);
    env_reset_process(identified_eth_mac());
    return 0;
}

uint16_t env_err_rsthdlr2(int sz)
{
    DM_UNUSED_ARG(sz);
    env_reset_process(identified_eth_mac());
    return 0;
}

uint16_t env_err_rsthdlr3(const char *format, ...)
{
	char bff[180];
	int val;
	va_list args;
	va_start(args, format);

	val = va_arg(args, int);
	sprintf(bff, format, val);
	
	printf("%s", bff);
	//vaf.fmt = format;
	//vaf.va = &args;
	//printf("%pV\r\n", &vaf); //ops

	va_end(args);
	env_reset_process(identified_eth_mac());
	return 0;
}

#if 0
uint16_t proc_err_rsthdlr3(char *message)
{
	printf(message);
	
    env_reset_process(identified_eth_mac());
    return 0;
}

void netdev_printk(const char *level, const struct net_device *dev,
		   const char *format, ...)
{
	struct va_format vaf;
	va_list args;

	va_start(args, format);

	vaf.fmt = format;
	vaf.va = &args;

	__netdev_printk(level, dev, &vaf);

	va_end(args);
}
#endif

// Debug functionality
#if DM_ETH_DEBUG_MODE
void debug_diff_rx_pointers(int state, uint16_t rd_now) {
	static int drp_fifoTurn_n = 0;
	static uint16_t drp_premdra_rd = 0x4000;
	static uint16_t drp_mdra_rd;
	uint16_t compos_totaldiff;
	uint16_t diff;

	if (drp_premdra_rd == 0x4000)
		drp_mdra_rd = rd_now; //~return;

	if (state)
		drp_fifoTurn_n++;
	if (rd_now < drp_premdra_rd && (drp_premdra_rd != 0x4000)) {
		/*uint16_t*/ compos_totaldiff = (rd_now >= drp_mdra_rd) ? 0x3400 : 0;
		/*uint16_t*/ diff = wrpadiff(drp_mdra_rd, rd_now);

		printf("mdra s %02x%02x e %02x%02x dif %x (nrx %d) .eth\r\n",
			drp_mdra_rd >> 8, drp_mdra_rd & 0xff,
			rd_now >> 8, rd_now & 0xff,
			diff + compos_totaldiff,
			drp_fifoTurn_n);
		drp_fifoTurn_n = 0;

		drp_mdra_rd = rd_now; //~return;
	}

	drp_premdra_rd = rd_now;
}
#endif

// ---------------------- xx -------------------------------------------------------------

// ---------------------- xx -------------------------------------------------------------

static uint16_t impl_dm9051_rx(uint8_t *buff)
{
	uint8_t rxbyte, rx_status;
	uint8_t ReceiveData[4];
	uint16_t rx_len, pad = 0; // 8-bit

	#if 0
	DM9051_RX_BREAK(!link_flag(), return evaluate_link());
	#endif
	rxbyte = cspi_read_rxb();
	DM9051_RX_BREAK((rxbyte != 0x01 && rxbyte != 0),
					return env_evaluate_rxb(rxbyte));
	DM9051_RX_BREAK((rxbyte == 0),
					return 0);
	cspi_rx_head(ReceiveData);
	rx_status = ReceiveData[1];
	rx_len = ReceiveData[2] + (ReceiveData[3] << 8);
	DM9051_RX_BREAK((rx_status & (0xbf & ~RSR_PLE)),
					return env_err_rsthdlr1(err_callback, "_dm9051f rx_status error : 0x%02x\r\n", rx_status));
#if !drv_print
	DM9051_RX_BREAK((rx_len > PBUF_POOL_BUFSIZE), return env_err_rsthdlr("_dm9051f rx_len error : %u\r\n", rx_len));
#else
	DM9051_RX_BREAK((rx_len > PBUF_POOL_BUFSIZE), return env_err_rsthdlr2(printf("_dm9051f rx_len error : %u\r\n", rx_len)));
#endif
	pad = rx_len & 1;				  // 16-bit
	cspi_rx_read(buff, rx_len + pad); // 8/16-bit
	return rx_len;
}

static const uint8_t *impl_dm9051_init(const uint8_t *adr)
{
	uint16_t id;
	const uint8_t *mac = NULL;

	// Initialize and check device ID
	if (!env_init_setup(&id))
	{
		printf("system stop\r\n");
		while (1)
			;
	}

	// Identify and set MAC address
	mac = identify_eth_mac(adr);

	// Display identified MAC address (if trace is enabled)
	trace_identify_eth_mac();

	// Perform reset process
	#if 0
	return env_reset_process(mac);
	#else
	//[Testing]
  env_err_rsthdlr("env_err_rsthdlr to reset_process(mac): %d\r\n", 25);
  env_err_rsthdlr1(err_callback, "env_err_rsthdlr1 to reset_process(mac): %d\r\n", 25);
#if drv_print
	env_err_rsthdlr2(printf("env_err_rsthdlr2 to reset_process(mac): %d\r\n", 25));
#endif
	env_err_rsthdlr3("env_err_rsthdlr3 to reset_process(mac): %d\r\n", 25);
	return mac;
	#endif
}

static void cspi_core_reset(void)
{
	cspi_ncr_reset(2);
	cspi_phycore_on(25);
	cspi_soft_default();
}

static const uint8_t *cspi_dm_start1(const uint8_t *adr)
{
	#ifdef DM9051_DRIVER_INTERRUPT
	cint_enable_mcu_irq(); // impl_enable_mcu_irq();
	#endif
	cspi_set_par(adr);
	cspi_rx_mode();
	return adr;
}

static void cspi_set_par(const uint8_t *macadd)
{
	int i;
	for (i = 0; i < 6; i++)
		cspi_write_reg(DM9051_PAR + i, macadd[i]);
}

static void cspi_rx_mode(void)
{
	cspi_set_mar();
	cspi_set_recv();
}
