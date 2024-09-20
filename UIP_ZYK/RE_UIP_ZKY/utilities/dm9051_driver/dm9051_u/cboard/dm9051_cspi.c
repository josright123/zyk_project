/**
  **************************************************************************
  * @file     dm9051_cspi.c
  * @version  v1.0
  * @date     2023-04-28
  * @brief    dm9051 config program
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
#include "dm9051opts.h"							//for, POLL_ON == POLL_ON_RXPADIFF
#include "dm9051_lw_mcu_default_IN.h"
#include "dm9051_lw.h"

#include "dm9051_cboard_data_API.h"	//for cint_enable_mcu_irq()

#include "dm9051_cstate.h"

//#if POLL_ON == POLL_ON_RXPADIFF //(DM_ETH_DEBUG_MODE || (POLL_ON == POLL_ON_RXPADIFF)) && 1
uint16_t wrpadiff(uint16_t rwpa_s, uint16_t rwpa_e) {
	return (rwpa_e >= rwpa_s) ? rwpa_e - rwpa_s : (rwpa_e + 0x4000 - 0xc00) - rwpa_s;
}
//#endif

#define dm9051if_cs_lo() spi_cs_lo()
#define dm9051if_cs_hi() spi_cs_hi()

#if 1
uint8_t cspi_read_reg(uint8_t reg) //static (todo)
{
	uint8_t val;
	dm9051if_cs_lo();
	val = AT_spi_data_read(reg);
	dm9051if_cs_hi();
	return val;
}

void cspi_write_reg(uint8_t reg, uint8_t val)
{
	dm9051if_cs_lo();
	AT_spi_data_write(reg, val);
	dm9051if_cs_hi();
}

void cspi_read_regs(uint8_t reg, uint8_t *buf, uint16_t len, csmode_t csmode)
{
	int i;

	if (csmode == CS_LONG) {
		#if 0
		dm9051if_cs_lo();
//		for(i=0; i < len; i++, reg++) {
//			dm9051_spi_command_write(reg | OPC_REG_R);
//			buf[i] = dm9051_spi_dummy_read();
//		}

		for(i=0; i < len; i++, reg++) {
			dm9051_spi_cmd_write(reg | OPC_REG_R);
			buf[i] = dm9051_spi_data_read();
		}
		dm9051_spi_read_end();
		dm9051if_cs_hi();
		#endif
		for(i=0; i < len; i++, reg++)
			buf[i] = cspi_read_reg(reg);
	}
	else { //CS_EACH
	  for(i=0; i < len; i++, reg++)
			buf[i] = cspi_read_reg(reg);
	}
}

void cspi_write_regs(uint8_t reg, uint8_t *buf, uint16_t len, csmode_t csmode)
{
  int i;

  if (csmode == CS_LONG)
  {
	#if 0
    dm9051if_cs_lo();
    for (i = 0; i < len; i++, reg++) {
      dm9051_spi_command_write(reg | OPC_REG_W);
      dm9051_spi_command_write(buf[i]);
    }
    dm9051if_cs_hi();
	#endif
    for (i = 0; i < len; i++, reg++)
      cspi_write_reg(reg + i, buf[i]);
  }
  else { // CS_EACH
    for (i = 0; i < len; i++, reg++)
      cspi_write_reg(reg + i, buf[i]);
  }
}

uint8_t cspi_read_mem2x(void)
{
	#if 1
	uint8_t rxb;
	dm9051if_cs_lo();
	rxb = AT_spi_mem2x_read();
	dm9051if_cs_hi();
	return rxb;
	#else
	return NU_spi_mem2x_read();
	#endif
}
void cspi_read_mem(uint8_t *buf, uint16_t len)
{
	dm9051if_cs_lo();
	AT_spi_mem_read(buf, len);
	dm9051if_cs_hi();
}
void cspi_write_mem(uint8_t *buf, uint16_t len)
{
	dm9051if_cs_lo();
	AT_spi_mem_write(buf, len);
	dm9051if_cs_hi();
}
#endif

void cspi_phycore_on(uint16_t nms) {
//u8 gpcr = cspi_read_reg(DM9051_GPCR);
//cspi_write_reg(DM9051_GPCR, gpcr | 0x01); //bit-0
	  cspi_write_reg(DM9051_GPR, 0x00);		//Power on PHY
	  dm_delay_ms(nms);
}

void cspi_ncr_reset(uint16_t nms) {
	  cspi_write_reg(DM9051_NCR, DM9051_REG_RESET); //iow(DM9051_NCR, NCR_RST);
	  dm_delay_ms(nms); //dm9051_delay_in_core_process(nms, "_core_reset<>"); //dm_delay_ms(250); //CH390-Est-Extra
}

uint16_t cspi_phy_read(uint16_t uReg)
{
	int w = 0;
	uint16_t uData;
	
	cspi_write_reg(DM9051_EPAR, DM9051_PHY | uReg);
	cspi_write_reg(DM9051_EPCR, 0xc);
	dm_delay_us(1);
	while(cspi_read_reg(DM9051_EPCR) & 0x1) {
		dm_delay_us(1);
		if (++w >= 500) //5
			break;
	} //Wait complete
	
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
	while(cspi_read_reg(DM9051_EPCR) & 0x1){
		dm_delay_us(1);
		if (++w >= 500) //5
			break;
	} //Wait complete

	cspi_write_reg(DM9051_EPCR, 0x0);    
}

void cspi_soft_default(void)
{
	  uint8_t val = MBNDRY_WORD; //16-bit
	  cspi_write_reg(DM9051_MBNDRY, val); /* MemBound */
	  cspi_write_reg(DM9051_PPCR, PPCR_PAUSE_COUNT); //iow(DM9051_PPCR, PPCR_SETTING); //#define PPCR_SETTING 0x08
	  cspi_write_reg(DM9051_LMCR, LMCR_MODE1);
	  cspi_write_reg(DM9051_INTR, INTR_ACTIVE_LOW); //interrupt active low
	  identify_irq_stat(ISTAT_LOW_ACTIVE);
	  trace_irq_stat(ISTAT_LOW_ACTIVE);
}

void cspi_core_reset(void)
{
	  cspi_ncr_reset(2);
	  cspi_phycore_on(25);
	  cspi_soft_default();
}

uint16_t cspi_read_chip_id(void) {
	uint8_t buff[2];
	cspi_read_regs(DM9051_PIDL, buff, 2, CS_EACH);
	return buff[0] | buff[1] << 8;
}

void cspi_vid_pid_revisions(uint8_t *ids, uint8_t *rev_ad) {
	cspi_read_regs(DM9051_VIDL, ids, 5, CS_EACH); //OPT_CS(csmode)
	cspi_read_regs(0x5C, rev_ad, 1, CS_EACH); //OPT_CS(csmode)
}

uint16_t cspi_read_control_status(void) {
	uint8_t buff[2];
	cspi_read_regs(DM9051_NCR, buff, 2, CS_EACH);
	return buff[0] | buff[1] << 8;
}

void cspi_read_rx_pointers(uint16_t *rwpa_wt, uint16_t *mdra_rd) {
	*rwpa_wt = (uint16_t)cspi_read_reg(0x24) | (uint16_t)cspi_read_reg(0x25) << 8; //DM9051_RWPAL
	*mdra_rd = (uint16_t)cspi_read_reg(0x74) | (uint16_t)cspi_read_reg(0x75) << 8; //DM9051_MRRL;
}

#if (POLL_ON == POLL_ON_RXPADIFF)
uint16_t cspi_diff_rxpa(void) {
	static uint16_t /*rwpa_wts,*/ s_mdra_rds = 0;
	uint16_t rwpa_wtp, mdra_rdp;
	
	cspi_read_rx_pointers(&rwpa_wtp, &mdra_rdp);
	
	if (!s_mdra_rds)
		s_mdra_rds = mdra_rdp;
	
	return wrpadiff(s_mdra_rds, mdra_rdp);
}
#endif

void cspi_set_par(const uint8_t *macadd) {
	int i;
	for (i=0; i<6; i++)
		cspi_write_reg(DM9051_PAR+i, macadd[i]);
}

void cspi_set_mar(void) {
	int i;
	for (i=0; i<8; i++)
		cspi_write_reg(DM9051_MAR+i, (i == 7) ? 0x80 : 0x00);
}

void cspi_set_recv(void)
{
#if 0
	cspi_write_reg(DM9051_FCR, FCR_DEFAULT); //iow(DM9051_FCR, FCR_FLOW_ENABLE);
	phy_write 04, flow
#endif
	identify_irq_stat(ISTAT_DM_IMR);
	trace_irq_stat(ISTAT_DM_IMR);
	cspi_write_reg(DM9051_IMR, IMR_PAR | IMR_PRM); //iow(DM9051_IMR, IMR_PAR | IMR_PTM | IMR_PRM);

	identify_irq_stat(ISTAT_DM_RCR);
	trace_irq_stat(ISTAT_DM_RCR);
	cspi_write_reg(DM9051_RCR, RCR_DEFAULT | RCR_RXEN); //dm9051_fifo_RX_enable();
	//trace_dm9051_set_recv();
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
	cint_enable_mcu_irq(); //impl_enable_mcu_irq();
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
	uint16_t pad = 0; //8-bit
	cspi_write_reg(DM9051_TXPLL, len & 0xff);
	cspi_write_reg(DM9051_TXPLH, (len >> 8) & 0xff);
	pad = len & 1; //16-bit
	cspi_write_mem(buf, len + pad); //8/16-bit
}

#define DM9051_TX_DELAY(expression, handler) do { if ((expression)) { \
  handler;}} while(0)

void cspi_tx_req(void)
{
	cspi_write_reg(DM9051_TCR, TCR_TXREQ); /* Cleared after TX complete */
	DM9051_TX_DELAY((cspi_read_reg(DM9051_TCR) & TCR_TXREQ), dm_delay_us(5));
}

uint16_t cspi_isr_enab(void) {
	uint16_t isrs;
	//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
	isrs = cspi_read_reg(DM9051_ISR);
	cspi_write_reg(DM9051_ISR, (uint8_t) isrs);
	//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));
	return isrs | (0xff << 8);
}
