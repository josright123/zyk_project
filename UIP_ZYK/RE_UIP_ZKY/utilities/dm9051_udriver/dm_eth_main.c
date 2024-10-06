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
#include "uip-conf.h"
#include "uip.h"
#include "dm9051_lw.h"
#include "cboard/dm9051_Hw_api.h"
#include "dm_identify_impl.h"

// Configuration flags
// #define DM_ETH_USE_INTERRUPTS 1
// #define DM_ETH_DEBUG_MODE 0

// Interrupt event flag
// #if defined(DM9051_DRIVER_INTERRUPT)
// #endif
static volatile int flgSemaphore_r = 0;

// Function prototypes
static void dm_eth_update_ip_config(const uint8_t *ip, const uint8_t *gw, const uint8_t *mask);

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

int32_t DM_ETH_Init(void) // DM9051_init(void)
{
	// DM9051_Configuration_NU();
	dm9051_boards_initialize();
	dm9051_init(&uip_ethaddr.addr[0]);
	return 0;
}

uint16_t DM_ETH_Input(void) // DM9051_rx(void)
{
	uip_len = dm9051_rx(uip_buf);
	dm_eth_input_hexdump(uip_buf, uip_len); //"dm_eth_status.c"
	return uip_len;
}

uint32_t DM_ETH_Output(void) // DM9051_tx(void)
{
	dm9051_tx(uip_buf, uip_len);
	return 0;
}

// Reset functionality
void DM_ETH_ToRst_ISR(void)
{
	cspi_isr_enab(); // dm9051_write_rst_isr();
}

void DM_ETH_IpConfiguration(uint8_t *ip, uint8_t *gw, uint8_t *mask)
{
	dm_eth_update_ip_config(
		identify_tcpip_ip(ip),
		identify_tcpip_gw(gw),
		identify_tcpip_mask(mask));
}

// DM_Eth_GetStatus: cid/bmsr/ncr_nsr
void DM_Eth_ReadRegsInfo(uint8_t *stat)
{
	uint16_t cs;
	uint32_t pbm;

	//	DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
	//	DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));
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

// Debug functionality
#if DM_ETH_DEBUG_MODE
uint16_t DM_ETH_ToCalc_rx_pointers(int state, uint16_t *mdra_rd_org, uint16_t *mdra_rdp)
{
	static uint16_t dummy_rwpa;
	// DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
	// DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));
	cspi_read_rx_pointers(&dummy_rwpa, mdra_rdp);
	return (state == 0) ? 0 : wrpadiff(*mdra_rd_org, *mdra_rdp);
}
#endif

static void dm_eth_update_ip_config(const uint8_t *ip, const uint8_t *gw, const uint8_t *mask)
{
	uip_ipaddr_t ipaddr;

	uip_ipaddr(&ipaddr, ip[0], ip[1], ip[2], ip[3]);
	uip_sethostaddr(ipaddr);
	uip_ipaddr(&ipaddr, gw[0], gw[1], gw[2], gw[3]);
	uip_setdraddr(ipaddr);
	uip_ipaddr(&ipaddr, mask[0], mask[1], mask[2], mask[3]);
	uip_setnetmask(ipaddr);
}
