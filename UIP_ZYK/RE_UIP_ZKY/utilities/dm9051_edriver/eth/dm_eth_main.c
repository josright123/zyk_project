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
 * Last updated: 20240905
 *
 */
#include "dm9051.h"
#include "identify/dm_identify_api.h"

#if 0
//[macro define to run by macro-definition!]
#include "print/dm9051_eth_debug.h"
#endif

// Configuration flags
// #define DM_ETH_USE_INTERRUPTS 1
// #define DM_ETH_DEBUG_MODE 0

// Interrupt event flag
// #if defined(DM9051_DRIVER_INTERRUPT)
// #endif

volatile int flgSemaphore_r = 0;

int DM_ETH_GetInterruptEvent(void)
{
	int event = flgSemaphore_r;
	flgSemaphore_r = 0;
	return event;
}

void DM_ETH_InterruptEvent(void) // DM_ETH_InterruptHdlr
{
	if (cint_exint9_5_handler())
	{
		flgSemaphore_r = 1; // dm_eth_set_intr_event(); //DM_ETH_SetInterruptEvent();
		inc_interrupt_count();
		// freertos_task_clearpoll_event();
	}
}

const uint8_t *DM_ETH_Init(const uint8_t *adr) // DM9051_init(void)
{
	// DM9051_Configuration_NU();
	dm9051_boards_initialize();
	return dm9051_init(adr);
}

uint16_t DM_ETH_Input(uint8_t *bff) // DM9051_rx(void)
{
	uint16_t len = dm9051_rx(bff);
	dm_eth_input_hexdump(bff, len); //"dm_eth_status.c"
	return len;
}

void DM_ETH_Output(uint8_t *bff, uint16_t len) // DM9051_tx(void)
{
	dm9051_tx(bff, len);
}

// Reset functionality
void DM_ETH_ToRst_ISR(void)
{
	cspi_isr_enab(); // dm9051_write_rst_isr();
}

//void DM_ETH_IpConfiguration(uint8_t *ip, uint8_t *gw, uint8_t *mask)
//{
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

// Debug functionality
#if DM_ETH_DEBUG_MODE
uint16_t DM_ETH_ToCalc_rx_pointers(int state, uint16_t *mdra_rd_org, uint16_t *mdra_rdp)
{
	static uint16_t dummy_rwpa;
	cspi_read_rx_pointers(&dummy_rwpa, mdra_rdp);
	return (state == 0) ? 0 : wrpadiff(*mdra_rd_org, *mdra_rdp);
}
#endif

static void DM_Eth_Input_HexDumpReset(uint8_t *stat)
{
#if DM_ETH_DEBUG_MODE
	// =DM9051_HEXDUMP_RESET(DM_Eth_StatusLink(stat), dm_eth_input_hexdump_reset());
	#if 0
	if (DM_Eth_StatusLink(stat))
	#endif
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

// DM_Eth_GetStatus: cid/bmsr/ncr_nsr
void DM_Eth_ReadRegsInfo(uint8_t *stat)
{
	cspi_read_regs_info(stat);

#if 1
	//[in link-down, always execute HexDumpRest() is also of cause OK.]
	//if (!netif_is_link_up(netif))
	if (!DM_Eth_Regs_Info_Linkup(stat))
		DM_Eth_Input_HexDumpReset(stat);
#endif
}
