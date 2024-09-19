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
//#include <lwip/sys.h> //only for Lwip
#include "dm9051opts.h"
#include "dm9051_lw.h"
#include "dm9051_lw_cboard.h"
#include "dm9051_lw_cspi.h"
#include "dm9051_lw_debug.h"
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
#include "dm9051_bl_xxx.h"

#if freeRTOS
#include "lwip/sys.h"
#include "lwip/err.h"
#endif

#if freeRTOS
sys_mutex_t lock_dm9051_core;
#endif

//static 
const dm_dly_t dmf = {
#if freeRTOS
	uvTaskDelay, //here assign, define system's delay us function
	vTaskDelay, //here assign, define system's delay ms function
#else
	delay_us, //here assign, define system's delay us function
	delay_ms, //here assign, define system's delay ms function
#endif
};

#define	board_printf(format, args...) //int board_printf(const char *format, args...) { return 0; }

/* dm9051 delay times procedures */

void dm_delay_us(uint32_t nus)
{
	board_printf("test %d ,because rxb %02x (is %d times)\r\n", rstccc, rxbyteee, timesss);
	dmf.dly_us(nus); //delay_us(nus);
}

void dm_delay_ms(uint16_t nms)
{
	dmf.dly_ms(nms); //delay_ms(nms);
}

//static 
uint16_t impl_read_chip_id(void) {
	u8 buff[2];
	cspi_read_regs(DM9051_PIDL, buff, 2, CS_EACH);
	return buff[0] | buff[1] << 8;
}

//static
void impl_read_rx_pointers(u16 *rwpa_wt, u16 *mdra_rd) {
	*rwpa_wt = (uint32_t)DM9051_Read_Reg(0x24) | (uint32_t)DM9051_Read_Reg(0x25) << 8; //DM9051_RWPAL
	*mdra_rd = (uint32_t)DM9051_Read_Reg(0x74) | (uint32_t)DM9051_Read_Reg(0x75) << 8; //DM9051_MRRL;
}

//static
uint16_t impl_phy_read(uint16_t uReg)
{
	int w = 0;
	u16 uData;

#if 1
  //_CH390
  //if (uReg == PHY_STATUS_REG)
  //{
	//dm9051_phycore_on(0); //if (uReg == PHY_STATUS_REG)
  //}
  if (DM_GET_FIELD(uint16_t, read_chip_id) == 0x9151 && uReg == PHY_STATUS_REG)
	dm9051_phycore_on(0);
#endif

	DM9051_Write_Reg(DM9051_EPAR, DM9051_PHY | uReg);
	DM9051_Write_Reg(DM9051_EPCR, 0xc);
	dm_delay_us(1);
	while(DM9051_Read_Reg(DM9051_EPCR) & 0x1) {
		dm_delay_us(1);
		if (++w >= 500) //5
			break;
	} //Wait complete

	DM9051_Write_Reg(DM9051_EPCR, 0x0);
	uData = (DM9051_Read_Reg(DM9051_EPDRH) << 8) | DM9051_Read_Reg(DM9051_EPDRL);

	#if 0
	if (uReg == PHY_STATUS_REG) {
		if (uData  & PHY_LINKED_BIT)
			dm9051_set_flags(lw_flag, DM9051_FLAG_LINK_UP);
		else
			dm9051_clear_flags(lw_flag, DM9051_FLAG_LINK_UP);
	}
	#endif

	return uData;
}

void impl_reset_pulse(void)
{
	  DM9051_Poweron_Reset();
}

const uint8_t *impl_dm9051_init(const uint8_t *adr)
{
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	uint16_t id;
	const uint8_t *mac = NULL;

#if freeRTOS
	if (sys_mutex_new(&lock_dm9051_core) != ERR_OK) {
		printf("init, failed to create lock_dm9051_core\r\n");
	}
#endif /* freeRTOS */

	//mac = impl_dm9051_init(adr); =
	//
	//= dm9051_init_setup(); //has, init.
	//= _identify_eth_mac(adr); //has, display_toset_mac();
	//= hdlr_reset_process(); //has, dm9051_core_reset(); ..dm9051_start(mac);
	//
	if (dm9051_init_setup(&id)) {
#if 1 //need so 'display'
		display_identity("TempName"/*mstep_spi_conf_name()*/, 0, NULL, 0, id, ".(Rst.process)"); //printf(".(Rst.process[%d])\r\n", mstep_get_net_index());
#endif
		mac = hdlr_reset_process(identify_eth_mac(adr, 0), DM_TRUE);
//		rx_pointer_show("dm9051_start");
//		rx_isr_show("dm9051_err_hdlr");
		rx_pointers_isr_show("dm9051_start");
	} else {
#if 1 //need so 'display'
		display_identity("TempName"/*mstep_spi_conf_name()*/, 0, NULL, 0, id, ""); //printf(".(Rst.process[%d])\r\n", mstep_get_net_index());
		printf(": dm9051_init_setup[%d] ::: FAIL ID %04x\r\n", mstep_get_net_index(), DM_GET_FIELD(uint16_t, read_chip_id));
#endif
	}
	return mac;
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

#define DM9051_RX_BREAK(expression, handler) do { if ((expression)) { \
  handler;}} while(0)

uint8_t rx_pointers_isr_show(char *headstr) //= show_isr_and_rx_pointers()
{
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
//.#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_ETHERNETIF_ON, SEMA_OFF, "[cc]", (fmt, ##__VA_ARGS__))

	u16 rwpa_w, mdra_ingress;
	uint8_t rcr, isr;
	impl_read_rx_pointers(&rwpa_w, &mdra_ingress);
	rcr = DM9051_Read_Reg(DM9051_RCR);
	isr = DM9051_Read_Reg(DM9051_ISR);
	printf(": %s[%d] :: %s rwpa %04x / igrss %04x\r\n",
			headstr, mstep_get_net_index(), "TempName"/*mstep_spi_conf_name()*/, rwpa_w, mdra_ingress);
	printf(": %s[%d] :: %s rcr 0x%02x isr 0x%02x\r\n",
			headstr, mstep_get_net_index(), "TempName"/*mstep_spi_conf_name()*/, rcr, isr);
	return isr;

#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

/*static*/
u16 impl_dm9051_err_hdlr(char *errstr, int pincode, u32 invalue, u8 zerochk)
{
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	char buf[76];
	int space_size = 76, n = 0;
	if (zerochk && invalue == 0)
		return 0; //.printf(": NoError as %u\r\n", valuecode);

#if 0
	printf(errstr, pincode, invalue); //or "0x%02x"
#else
	n += snprintf(buf+n, space_size-n, "----- [.]");
	n += snprintf(buf+n, space_size-n, errstr, pincode, invalue);
	
	if (n >= 76) n = 75;
	buf[n] = 0;
	
	bannerline_log();
	printf("----- [.]");
	printf(errstr, pincode, invalue); //or "0x%02x"
	printf(buf);
#endif

	hdlr_reset_process(mstep_eth_mac(), OPT_CONFIRM(hdlr_confrecv)); //CH390 opts
	
#if 1
//	rx_pointer_show("dm9051_err_hdlr");
//	rx_isr_show("dm9051_err_hdlr");
	rx_pointers_isr_show("dm9051_err_hdlr");
#endif

	return 0;
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

static uint16_t buff_rx(uint8_t *buff)
{
	uint16_t rx_len = 0;
	uint8_t rxbyte, rx_status;
	uint8_t ReceiveData[4];
	
	rxbyte = DM9051_Read_Mem2X(); //DM9051_Read_Rxb(); //DM9051_Read_Reg(DM9051_MRCMDX);
	//DM9051_RXB_Basic(rxbyte); //(todo) Need sevice case.
	
	DM9051_RX_BREAK((rxbyte != 0x01 && rxbyte != 0), return ev_rxb(rxbyte));
	DM9051_RX_BREAK((rxbyte == 0), return 0);
		
	DM9051_Read_Mem(ReceiveData, 4);
	DM9051_Write_Reg(DM9051_ISR, 0x80);
	
	rx_status = ReceiveData[1];
	rx_len = ReceiveData[2] + (ReceiveData[3] << 8);
	
	//instead of : err_hdlr("_dm9051f rx_status error : 0x%02x\r\n", rx_status, 0)
	DM9051_RX_BREAK((rx_status & 0xbf), return ev_status(rx_status));
	//instead of : err_hdlr("_dm9051f rx_len error : %u\r\n", rx_len, 0));
	DM9051_RX_BREAK((rx_len > RX_POOL_BUFSIZE), return impl_dm9051_err_hdlr("_dm9051f[%d] rx_len error : %u\r\n", PINCOD, rx_len, 0));

	DM9051_Read_Mem(buff, rx_len);
	DM9051_Write_Reg(DM9051_ISR, 0x80);
	return rx_len;
}

uint16_t impl_dm9051_rx1(uint8_t *buff)
{
//	if (!dm9051_get_flags()) {
//		dm9051_unlink_inc();
//		dm9051_bmsr_update();

//		return 0;
//	}
	return buff_rx(buff);
}

#define DM9051_TX_DELAY(expression, handler) do { if ((expression)) { \
  handler;}} while(0)

void impl_dm9051_tx1(uint8_t *buf, uint16_t len)
{
	DM9051_Write_Reg(DM9051_TXPLL, len & 0xff);
	DM9051_Write_Reg(DM9051_TXPLH, (len >> 8) & 0xff);
	DM9051_Write_Mem(buf, len);
	DM9051_Write_Reg(DM9051_TCR, TCR_TXREQ); /* Cleared after TX complete */
	DM9051_TX_DELAY((DM9051_Read_Reg(DM9051_TCR) & TCR_TXREQ), dm_delay_us(5));
}

/* dm9051_if.c (bl_impl's subroutines))*/
char *display_identity_bannerline_title = NULL;
char *display_identity_bannerline_default =  ": Read device";


int display_identity(char *spiname, uint16_t id, uint8_t *ids, uint8_t id_adv, uint16_t idin, char *tail)
{
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))

	static uint8_t psh_ids1[ETHERNET_COUNT][5], psh_id_adv1[ETHERNET_COUNT];
	DM_UNUSED_ARG(id);
	if (ids) {
#if 0
		psh_id1[mstep_get_net_index()] = id;
#endif	
		memcpy(&psh_ids1[mstep_get_net_index()][0], ids, 5);
		psh_id_adv1[mstep_get_net_index()] = id_adv;
	} else {
//		 id = psh_id1[mstep_get_net_index()];
//		 memcpy(ids, &psh_ids1[mstep_get_net_index()][0], 5);
//		 id_adv = psh_id_adv1[mstep_get_net_index()];
	}

//display_ids(fstr, ids) + display_ida(fstr, id_adv)
	printf("%s[%d] %s ::: ids %02x %02x %02x %02x (%s) chip rev %02x, Chip ID %04x (CS_EACH_MODE)%s\r\n",
		display_identity_bannerline_title ? display_identity_bannerline_title : display_identity_bannerline_default,
		mstep_get_net_index(), spiname,
		psh_ids1[mstep_get_net_index()][0], psh_ids1[mstep_get_net_index()][1],
		psh_ids1[mstep_get_net_index()][2], psh_ids1[mstep_get_net_index()][3], 
		DM_GET_DESC(csmode_t, csmode), //dm9051opts_desccsmode()
		psh_id_adv1[mstep_get_net_index()], 
		idin, //psh_id1[mstep_get_net_index()],
		tail //ids ? "" : ".(Rst.process)"
		);
	return 0;
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
} //spiname

static int check_chip_id(uint16_t id) {

	if (id == (0x91001c00 >> 16)) {
		printf("---------------------- \r\n");
		printf("--- warn case %04x --- \r\n", id);
		printf("---------------------- \r\n");
	}

	return (id == (DM9051_ID >> 16)
			|| id == (DM9052_ID >> 16)
			#if 1
			|| id == (0x91511c00 >> 16)
			|| id == (0x91001c00 >> 16) /* Test CH 3 */
			#endif
		) ? 1 : 0;
}

int dm9051_init_setup(uint16_t *id)
{
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	uint8_t ids[5], id_adv;
	
	display_identity_bannerline_title = ": dm9051_init";

	*id = impl_read_chip_id();
	read_chip_revision(ids, &id_adv);

#if 0 //NO need so 'display'
	display_identity(mstep_spi_conf_name(), *id, ids, *id, id_adv);
#endif

	//display_chipmac();
	DM_SET_FIELD(uint16_t, read_chip_id, *id); //store into dm9051optsex[i].read_chip_id
	
	if (check_chip_id(*id)) {
//		display_eeprom_action();
		return 1;
	}

	return 0;
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

static int is_multicast_ether_addr(const uint8_t *addr)
{
	return 0x01 & addr[0];
}
static int is_zero_ether_addr(const uint8_t *addr)
{
	return (*(const uint16_t *)(addr + 0) |
			*(const uint16_t *)(addr + 2) |
			*(const uint16_t *)(addr + 4)) == 0;
}

static int is_valid_ether_addr(const uint8_t *addr)
{
	/* FF:FF:FF:FF:FF:FF is a multicast address so we don't need to
	 * explicitly check for it here. */
	return !is_multicast_ether_addr(addr) && !is_zero_ether_addr(addr);
}

static const uint8_t *internal_adr_logic(void)
{
	const uint8_t *adr;
	/*
	 * dm9051 start1 internal logic, i.e.
	 * Read par, if valid then it's from EEPROM
	 * otherwise, use Hard Core Candidate (hcc)
	 */
	//READ MAC_ADDR_LENGTH REGs //CCC
	uint8_t buff[6];
	cspi_read_regs(DM9051_PAR, buff, 6, CS_EACH);
	if (is_valid_ether_addr(buff)) {
//			adr = buff;
		adr = identify_eth_mac(buff, 0); /* Trick1 */
		printf(": EEPROM valid-mac[%d] ::: %02x %02x %02x %02x %02x %02x\r\n",
			mstep_get_net_index(), adr[0],adr[1],adr[2],adr[3],adr[4],adr[5]);
	} else {
		adr = identify_eth_mac(NULL, 0); /* Trick2 */
		printf(": Hard Core candidate-mac[%d] ::: %02x %02x %02x %02x %02x %02x\r\n",
			mstep_get_net_index(), adr[0],adr[1],adr[2],adr[3],adr[4],adr[5]);
	}
	return adr;
}

static void dm9051_core_reset(void)
{
	//u8 gpcr = DM9051_Read_Reg(DM9051_GPCR);
	//DM9051_Write_Reg(DM9051_GPCR, gpcr | 0x01); //GPC0(reserved), bit-0
		  
	if (OPT_CONFIRM(generic_core_rst)){
			#if 0
			printf("-----------------------------------------------------------------------------------------\r\n");
			printf("--------------------- write a long delay type procedure ,for core reset -----------------\r\n");
			printf("-----------------------------------------------------------------------------------------\r\n");
			#endif

		  //CHMODE
		  dm9051_ncr_reset(OPT_U8(hdir_x2ms)*2);
		  dm9051_phycore_on(250);
	} else {
		#if 0
		//DAVICOM
		  dm9051_phycore_on(5);
		  dm9051_ncr_reset(0);
		#else
		//CHMODE
		  dm9051_ncr_reset(2); //CHMODE-Est-Extra
		  dm9051_phycore_on(5);
		#endif
	}

	DM9051_Write_Reg(DM9051_MBNDRY, OPT_U8(iomode));
	DM9051_Write_Reg(DM9051_PPCR, PPCR_PAUSE_COUNT); //iow(DM9051_PPCR, PPCR_SETTING); //#define PPCR_SETTING 0x08
	DM9051_Write_Reg(DM9051_LMCR, LMCR_MODE1);
	DM9051_Write_Reg(DM9051_INTR, INTR_ACTIVE_LOW); // interrupt active low

	if (OPT_CONFIRM(generic_core_rst))
		;
	else {
		#if TEST_PLAN_MODE
		if (OPT_CONFIRM(rxmode_checksum_offload))
			DM9051_Write_Reg(DM9051_RCSSR, RCSSR_RCSEN);
		#else
		/* if (isrxmode_checksum_offload())
				DM9051_Write_Reg(DM9051_RCSSR, RCSSR_RCSEN); */
		#endif

		/* flow_control_config_init */
		if (OPT_CONFIRM(flowcontrolmode)) {
		  #if TEST_PLAN_MODE == 0
			printf("  core_reset: %s, fcr value %02x\r\n", DM_GET_DESC(enable_t, flowcontrolmode), FCR_DEFAULT_CONF);
		  #endif
			DM9051_Write_Reg(DM9051_FCR, FCR_DEFAULT_CONF);
		}

		#if TEST_PLAN_MODE
		test_plan_100mf(OPT_BMCR(bmcrmode), if_log); //= dm9051opts_bmcrmode_tbmcrmode()
		#endif
	}
}

const uint8_t *dm9051_start1(const uint8_t *adr)
{
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
//	display_baremac();
#if 1
	if (adr) {
		if (is_valid_ether_addr(adr)) {
			adr = identify_eth_mac(adr, 0); /* Trick0 */
//			printf(": APP ENTER-mac[%d] ::: %02x %02x %02x %02x %02x %02x\r\n",
//				mstep_get_net_index(), adr[0],adr[1],adr[2],adr[3],adr[4],adr[5]);
		} else {
			printf(": APP DEF-INVAL-mac[%d] ::: %02x %02x %02x %02x %02x %02x (Invalid)\r\n",
				mstep_get_net_index(), adr[0],adr[1],adr[2],adr[3],adr[4],adr[5]);
			adr = internal_adr_logic();
		}
	}
	else
		adr = internal_adr_logic();
#endif
//	display_mac_action(bare_mac_tbl[1], adr); //[1]= ": wr-bare device"

	dm9051_extline_irq_enable(); //_dm9051_board_irq_enable(NVIC_PRIORITY_GROUP_0);
	dm9051_mac_adr(adr);
	dm9051_rx_mode();
	return adr;
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

static void display_rw_mac(char *head, const uint8_t *adr)
{
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	int i;
	
	bannerline_log();
	printf(": %s[%d] :: eeprom[] ", head, mstep_get_net_index());
	for (i = 0; i < 3; i++) {
		uint16_t value = dm9051_eeprom_read(i);
		printf("%04x ", value);
	}
	printf("set chip-par[] %02x%02x%02x%02x%02x%02x\r\n", adr[0], adr[1], adr[2], adr[3], adr[4], adr[5]); //wr-par
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

const uint8_t *hdlr_reset_process(const uint8_t *macaddr, enable_t en)
{
	dm9051_core_reset(); //Can: printf("rstc %d ,because rxb %02x (is %d times)\r\n", rstc, rxbyte, times);

	if (en) { //As dm9051_init's whole operations. Only for _CH390
		macaddr = dm9051_start1(macaddr);

		display_rw_mac("dm9051_start", macaddr);
//		display_eeprom_action(bare_mac_tbl[0]);
//		display_mac_action(bare_mac_tbl[1], macaddr); //[1]= ": wr-bare device"

//		_rx_pointer_show("dm9051_start");
	}
	return macaddr;
}

#define	TIMES_TO_RST	10

static void dm9051_show_rxbstatistic(u8 *htc, int n)
{
	int i, j;

	//dm9051_show_id();
	printf("SHW rxbStatistic, 254 wrngs\r\n");
	for (i = 0 ; i < (n+2); i++) {
		if (!(i%32) && i) printf("\r\n");
		if (!(i%32) || !(i%16)) printf("%02x:", i);
		if (!(i%8)) printf(" ");
		if (i==0 || i==1) {
			printf("  ");
			continue;
		}
		j = i - 2;
		printf("%d ", htc[j]);
	}
	printf("\r\n");
}

static u8 ret_fire_time(u8 *histc, int csize, int i, u8 rxb)
{
	u16 rwpa_w, mdra_ingress;
	u8 times = (histc[i] >= TIMES_TO_RST) ? histc[i] : 0;
//	printf(" _dm9051f rxb %02x (times %2d)%c\r\n", rxb, histc[i], (histc[i]==2) ? '*': ' ');

//  rwpa_w = (uint32_t)DM9051_Read_Reg(0x24) | (uint32_t)DM9051_Read_Reg(0x25) << 8; //DM9051_RWPAL
//  mdra_ingress = (uint32_t)DM9051_Read_Reg(0x74) | (uint32_t)DM9051_Read_Reg(0x75) << 8; //DM9051_MRRL;
  impl_read_rx_pointers(&rwpa_w, &mdra_ingress);
  printf("%2d. rwpa %04x / igrss %04x, histc[rxb %02xh], times= %d\r\n",
		 histc[i], rwpa_w, mdra_ingress, rxb, times);

	if (times) { //if (histc[i] >= TIMES_TO_RST)
		dm9051_show_rxbstatistic(histc, csize);
		histc[i] = 1;
		//return times; (return TIMES_TO_RST;)
	}
	return times; //0;
}

//static 
u16 ev_rxb(uint8_t rxb)
{
	int i;
	static u8 histc[254] = { 0 }; //static int rff_c = 0 ...;
	u8 times = 1;

	for (i = 0 ; i < sizeof(histc); i++) {
		if (rxb == (i+2)) {
			histc[i]++;
			times = ret_fire_time(histc, sizeof(histc), i, rxb);
			return impl_dm9051_err_hdlr("_dm9051f[%d] : rxbErr %u times :: dm9051_core_reset()\r\n", PINCOD, times, 1); //As: Hdlr (times : TIMES_TO_RST or 0)
			                //: Read device[0] :::
		}
	}
	return impl_dm9051_err_hdlr(" _dm9051f[%d] rxb error times (No way!) : %u\r\n", PINCOD, times, 0); //As: Hdlr (times : 1, zerochk : 0)
}

//static
u16 ev_status(uint8_t rx_status)
{
	bannerline_log();
	printf(".(Err.status%2x) _dm9051f:", rx_status);
	if (rx_status & RSR_RF) printf(" runt-frame");

	if (rx_status & RSR_LCS) printf(" late-collision");
	if (rx_status & RSR_RWTO) printf(" watchdog-timeout");
	if (rx_status & RSR_PLE) printf(" physical-layer-err");
	if (rx_status & RSR_AE) printf(" alignment-err");
	if (rx_status & RSR_CE) printf(" crc-err");
	if (rx_status & RSR_FOE) printf(" rx-memory-overflow-err");
	bannerline_log();
	return impl_dm9051_err_hdlr("_dm9051f[%d] rx_status error : 0x%02x\r\n", PINCOD, rx_status, 0);
}

uint16_t impl_dm9051_isr_disab(void) //static uint16_t dm9051_isr_clean(void)
{
	uint16_t isrs = DM9051_Read_Reg(DM9051_ISR);
	return (isrs & ISR_PR) | (isrs << 8);
}

uint16_t impl_dm9051_isr_enab(void) //static uint16_t dm9051_isr_clean(void)
{
	uint16_t isrs = DM9051_Read_Reg(DM9051_ISR);
	if (isrs & 1) {
		DM9051_Write_Reg(DM9051_ISR, (uint8_t) isrs);
		isrs |= DM9051_Read_Reg(DM9051_ISR) << 8;
	}
	return isrs;
}
