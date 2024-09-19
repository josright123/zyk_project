#include "dm9051opts.h"
#include "dm9051_lw.h"
#include "dm9051_lw_cboard.h"
#include "dm9051_lw_cspi.h"
#include "dm9051_lw_cint.h"
#include "dm9051_lw_debug.h"
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
#include "dm9051_bl_xxx.h"

/* eeprom && phy */

uint16_t eeprom_read(uint16_t wordnum)
{
	u16 uData;
	do {
		int w = 0;
		DM9051_Write_Reg(DM9051_EPAR, wordnum);
		DM9051_Write_Reg(DM9051_EPCR, 0x4); // chip is reading
		dm_delay_us(1);
		while(DM9051_Read_Reg(DM9051_EPCR) & 0x1) {
			dm_delay_us(1);
			if (++w >= 500) //5
				break;
		} //Wait complete
		DM9051_Write_Reg(DM9051_EPCR, 0x0);
		uData = (DM9051_Read_Reg(DM9051_EPDRH) << 8) | DM9051_Read_Reg(DM9051_EPDRL);
	} while(0);
	return uData;
}

//static ; function "phy_write" was declared but never referenced.
void phy_write(uint16_t reg, uint16_t value)
{
	int w = 0;

	DM9051_Write_Reg(DM9051_EPAR, DM9051_PHY | reg);
	DM9051_Write_Reg(DM9051_EPDRL, (value & 0xff));
	DM9051_Write_Reg(DM9051_EPDRH, ((value >> 8) & 0xff));
	/* Issue phyxcer write command */
	DM9051_Write_Reg(DM9051_EPCR, 0xa);
	dm_delay_us(1);
	while(DM9051_Read_Reg(DM9051_EPCR) & 0x1){
		dm_delay_us(1);
		if (++w >= 500) //5
			break;
	} //Wait complete

	DM9051_Write_Reg(DM9051_EPCR, 0x0);
}

uint16_t dm9051_bmcr_update(void) {
  return impl_phy_read(PHY_CONTROL_REG);
}
uint16_t dm9051_link_update(void) {
  return impl_phy_read(PHY_STATUS_REG);
}
uint16_t dm9051_phy_read(uint32_t reg) {
  return impl_phy_read(reg);
}
void dm9051_phy_write(uint32_t reg, uint16_t value) {
  phy_write(reg, value);
}

uint16_t dm9051_eeprom_read(uint16_t word) {
  return eeprom_read(word);
}

static void dm9051_delay_in_core_process(uint16_t nms, char *zhead) //finally, dm9051_lw.c
{
	if (nms)
		printf(": dm9051_driver[%d] ::: %s delay %u ms.. : \r\n", mstep_get_net_index(), zhead, nms);

	dm_delay_ms(nms); //if (nms) //from James' advice! to be determined with a reproduced test cases!!
}

//.void dm9051_poweron_reset_pulse(void)
//.{
//.	  DM9051_Poweron_Reset();
//.}

void dm9051_phycore_on(uint16_t nms) {
	  DM9051_Write_Reg(DM9051_GPR, 0x00);		//Power on PHY
	  dm9051_delay_in_core_process(nms, "_phycore_on<>");
}

void dm9051_ncr_reset(uint16_t nms) {
	  DM9051_Write_Reg(DM9051_NCR, DM9051_REG_RESET); //iow(DM9051_NCR, NCR_RST);
	  dm9051_delay_in_core_process(nms, "_core_reset<>"); //dm_delay_ms(250); //CH390-Est-Extra
}

//void test_plan_mbndry(void)
//{
//	uint8_t isr0, isr1, mbndry0, mbndry1;
//	char *str0, *str1;

//	isr0 = DM9051_Read_Reg(DM9051_ISR);

//	mbndry0 = OPT_U8(iomode);
//	str0 = (mbndry0 & MBNDRY_BYTE) ? "(8-bit mode)" : "(16-bit mode)";
//	printf("  ACTION: Start.s Write the MBNDRY %02x %s\r\n", mbndry0, str0);

//	DM9051_Write_Reg(DM9051_MBNDRY, mbndry0);

//		mbndry1 = DM9051_Read_Reg(DM9051_MBNDRY);
//		str1 = (mbndry1 & MBNDRY_BYTE) ? "(8-bit mode)" : "(16-bit mode)";
//		if ((mbndry0 & MBNDRY_BYTE) == (mbndry1 & MBNDRY_BYTE))
//			printf("  ACTION: !And.e Read check MBNDRY %02x %s\r\n", mbndry1, str1);
//		else
//			printf("  ACTION: !But.e Read check MBNDRY %02x %s \"( read different )\"\r\n",
//				mbndry1, str1); //"(read diff, be an elder revision chip bit7 can't write)"

//	isr1 = DM9051_Read_Reg(DM9051_ISR);

//	printf("  RESET: ISR.read.s %02x %s\r\n", isr0, isr0 & 0x80 ? "(8-bit mode)" : "(16-bit mode)");
//	printf("  RESET: ISR.read.e %02x %s\r\n", isr1, isr1 & 0x80 ? "(8-bit mode)" : "(16-bit mode)");
//}

//static 
void read_chip_revision(u8 *ids, u8 *rev_ad) {
	cspi_read_regs(DM9051_VIDL, ids, 5, OPT_CS(csmode)); //dm9051opts_csmode_tcsmode()
	cspi_read_regs(0x5C, rev_ad, 1, OPT_CS(csmode)); //dm9051opts_csmode_tcsmode()
}

static void impl_dm9051_set_par(const u8 *macadd)
{
	int i;
	for (i=0; i<6; i++)
		DM9051_Write_Reg(DM9051_PAR+i, macadd[i]);
}

static void dm9051_set_mar(void)
{
	int i;
	for (i=0; i<8; i++)
		DM9051_Write_Reg(DM9051_MAR+i, (i == 7) ? 0x80 : 0x00);
}

static void dm9051_set_recv(void)
{
	#if 0
	DM9051_Write_Reg(_DM9051_FCR, _FCR_DEFAULT_CONF); Located in 'dm9051 core reset'!
	phy_write _04, _flow
	#endif

	DM9051_Write_Reg(DM9051_IMR, IMR_PAR | IMR_PRM); //(IMR_PAR | IMR_PTM | IMR_PRM);

	//#if _TEST_PLAN_MODE //#else //#endif
	if (OPT_U8(promismode)) {
		printf("SETRECV: ::: test rx_promiscous (write_reg, 0x05, (1 << 2))\r\n");
		DM9051_Write_Reg(DM9051_RCR, RCR_DEFAULT | RCR_PRMSC | RCR_RXEN); //promiscous
	}
	else
		DM9051_Write_Reg(DM9051_RCR, RCR_DEFAULT | RCR_RXEN); //dm9051_fifo_RX_enable();
}

//static 
void dm9051_mac_adr(const uint8_t *macadd) {
	impl_dm9051_set_par(macadd);
	//show_par();
}

//static 
void dm9051_rx_mode(void)
{
	dm9051_set_mar();
	//show_mar();

	dm9051_set_recv();
}
