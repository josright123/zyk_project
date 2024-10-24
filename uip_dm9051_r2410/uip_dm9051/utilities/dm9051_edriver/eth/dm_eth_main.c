/**
 **************************************************************************
 * @file     dm_eth_main.c
 * @version  v1.0.1
 * @date     2024-06-24
 * @brief    DM9051 Ethernet driver main file (or referred to be as eth.c)
 **************************************************************************
 *
 * To restructure and improve the file to enhance readability, maintainability,
 * and potentially performance.
 * Last updated: 2024-09-05
 */
#include "config/conf.h"
#include "config/conf_core.h"
#if 0
#include "utils/dm9051_eth_debug.h"
#endif

#define	DM_ETH_IRQHandler	EXINT9_5_UserFunction	//EXINT9_5_IRQHandler

// Configuration flags
// #define DM_ETH_USE_INTERRUPTS 1
// #define DM_ETH_DEBUG_MODE 0

// Interrupt event flag
// #if defined(DM9051_DRIVER_INTERRUPT)
volatile int flgSemaphore_r = 0;
// #endif

int DM_ETH_GetInterruptEvent(void)
{
#if 0
	return DM_ETH_RTOS_TAKE_SEMA();
#endif
#if 0
	//[Very bad, that stop the furhter interrupt cycle.]
		int event = flgSemaphore_r;
		flgSemaphore_r = 0;
		return event;
#endif
	//[Very good, check if it stop the furhter interrupt cycle or NOT (experiment OK).]
		if (flgSemaphore_r) {
			flgSemaphore_r = 0;
			return 1;
		}
		return 0;
}

/* DM_ETH_InterruptHdlr
 */
void DM_ETH_IRQHandler(void)
{
	//identify_irq_stat(ISTAT_IRQ_NOW);
	//trace_irq_stat(ISTAT_IRQ_NOW);
	//if (cint_exint9_5_handler())
	//{
		flgSemaphore_r = 1;
		//DM_ETH_RTOS_GIVE_SEMA();
		inc_interrupt_count();
		identify_irq_stat(ISTAT_IRQ_NOW2);
		trace_irq_stat(ISTAT_IRQ_NOW2);
	//}
	deidentify_irq_stat(ISTAT_IRQ_NOW2);
}

// Reset functionality
void DM_ETH_ToRst_ISR(void)
{
	cspi_isr_enab();
	identify_irq_stat(ISTAT_IRQ_NOW2END);
}

const uint8_t *DM_ETH_Init(const uint8_t *adr)
{
	dm9051_boards_initialize();
	return dm9051_init(adr);
}

uint16_t DM_ETH_Input(uint8_t *bff)
{
	uint16_t len = dm9051_rx(bff);
	dm_eth_input_hexdump(bff, len);
	return len;
}

void DM_ETH_Output(uint8_t *bff, uint16_t len)
{
	dm9051_tx(bff, len);
}

//void DM_ETH_IpConfiguration(uint8_t *ip, uint8_t *gw, uint8_t *mask){
//	identify_tcpip_ip(ip);
//	identify_tcpip_gw(gw);
//	identify_tcpip_mask(mask);
//}
uint8_t *DM_ETH_Ip_Configuration(const uint8_t *ip)
{
	printf("config ip %d.%d.%d.%d\r\n", ip[0], ip[1], ip[2], ip[3]); //first-line.
	return identify_tcpip_ip(ip);
}
uint8_t *DM_ETH_Gw_Configuration(const uint8_t *ip)
{
	printf("config gw %d.%d.%d.%d\r\n", ip[0], ip[1], ip[2], ip[3]);
	return identify_tcpip_gw(ip);
}
uint8_t *DM_ETH_Mask_Configuration(const uint8_t *ip)
{
	return identify_tcpip_mask(ip);
}

uint8_t *DM_ETH_Ip_Configured(void) {
	return identified_tcpip_ip();
}

uint8_t *DM_ETH_Gw_Configured(void) {
	return identified_tcpip_gw();
}

uint8_t *DM_ETH_Mask_Configured(void) {
	return identified_tcpip_mask();
}

#if DM_ETH_DEBUG_MODE
uint16_t DM_ETH_ToCalc_rx_pointers(int state, const uint16_t *mdra_rd_org, uint16_t *mdra_rd_now)
{
	static uint16_t dummy_rwpa;
	cspi_read_rx_pointers(&dummy_rwpa, mdra_rd_now);
	debug_diff_rx_pointers(state, *mdra_rd_now);
	return (state == 0) ? 0 : wrpadiff(*mdra_rd_org, *mdra_rd_now);
}
#endif

static void DM_Eth_Input_HexDumpReset(uint8_t *stat)
{
#if DM_ETH_DEBUG_MODE
	dm_eth_input_hexdump_reset();
#endif
}

//DM_Eth_StatusLink
int DM_Eth_Regs_Info_Linkup(uint8_t *stat) {
	#if LINK_STATE_SOURCE == DM9051_CHECK_MAC
	  return stat[1] & 0x40 ? 1 : 0; //'nsr'
	#endif
	#if LINK_STATE_SOURCE == DM9051_CHECK_PHY
	  return stat[5] & 0x04 ? 1 : 0; //'bmsr'
	#endif
}

void DM_Eth_ReadRegsInfo(uint8_t *stat)
{
	/* cid/bmsr/ncr_nsr
	 */
	cspi_read_regs_info(stat);
	if (!DM_Eth_Regs_Info_Linkup(stat))
		DM_Eth_Input_HexDumpReset(stat);
}
