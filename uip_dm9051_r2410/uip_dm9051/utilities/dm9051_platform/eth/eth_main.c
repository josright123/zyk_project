/**
 **************************************************************************
 * @file      eth_main.c
 * @version   v1.0.1
 * @date      2024-06-24
 * 
 * @author   Joseph CHANG <joseph_chang@davicom.com.tw>
 * @copyright (c) 2023-2025 Davicom Semiconductor, Inc.
 * 
 * @brief     DM9051 Ethernet Driver Implementation
 * 
 * @details   This module implements the main functionality for the DM9051
 *           Ethernet controller, providing initialization, packet handling,
 *           and interrupt management.
 *           
 * @features  - Interrupt-driven packet processing
 *           - Configuration interface for IP settings
 *           - Debug support for packet analysis
 *           - Link status monitoring
 * 
 * @note      Last updated: 2024-09-05
 **************************************************************************
 */

#include "control/drv_control/conf_core.h"
#include "control/drv_control/dm9051_drv_debug.h"

/* Basic Type Definitions */
#include "eth_types.h"
//#define DM_DEBUG_TYPE 0
//#include "debug_types_define.h"
//#define DM_TYPE 0
//#include "dm_types_define.h"

/* eth_main api */
#include "eth_api.h"

/* Global state variables */
static volatile int flgSemaphore_r = 0;

/* Interrupt handler mapping */
#define DM_ETH_IRQHandler EXINT9_5_UserFunction

/* Debug Type Definitions */
#define DM_DEBUG_TYPE 10
#include "debug_types_define.h"
#define DM_DEBUG_TYPE 11
#include "debug_types_define.h"
#define DM_DEBUG_TYPE 20
#include "debug_types_define.h"
#define DM_DEBUG_TYPE 21
#include "debug_types_define.h"

/*-----------------------------------------------------------------------------
 * Public dm_impl Functions
 *-----------------------------------------------------------------------------*/

/* Type Definitions [Public dm_types Functions, belong to dm9051.c] */
#define DM_TYPE 1
#include "dm_types_define.h"
#define DM_TYPE 2
#include "dm_types_define.h"

/**
 * @brief  Interrupt service routine for Ethernet events
 * @note   Handles packet reception and updates interrupt statistics
 */
void DM_ETH_IRQHandler(void)
{
  flgSemaphore_r = 1;
  inc_interrupt_count();
  identify_irq_stat(ISTAT_IRQ_NOW2);
  trace_irq_stat(ISTAT_IRQ_NOW2);
  deidentify_irq_stat(ISTAT_IRQ_NOW2);
}

/**
 * @brief  Retrieves pending interrupt events
 * @return 1 if event pending, 0 otherwise
 */
int DM_ETH_GetInterruptEvent(void)
{
  if (flgSemaphore_r) {
    flgSemaphore_r = 0;
    return 1;
  }
  return 0;
}

/**
 * @brief  Prepares ISR for reset operation
 */
void DM_ETH_ToRst_ISR(void)
{
  dm9051_isr_enab();
  identify_irq_stat(ISTAT_IRQ_NOW2END);
}

/**
 * @brief  Initializes the Ethernet interface
 * @param  adr: MAC address pointer
 * @return Pointer to configured MAC address
 */
const uint8_t *DM_ETH_Init(const uint8_t *adr)
{
  struct board_init_type board_init_struct;
  flgSemaphore_r = 0;

  dm9051_boards_get_info(&board_init_struct);

#ifdef ETHERNET_INTERRUPT_MODE
  board_init_struct.interrrpt_mode = 1;
#endif
  dm9051_boards_initialize(&board_init_struct);
  return dm9051_init(adr);
}

/**
 * @brief  Handles packet reception
 * @param  bff: Buffer for received packet
 * @return Length of received packet
 */
uint16_t DM_ETH_Input(uint8_t *bff)
{
  uint16_t len = dm9051_rx(bff);
  dm_eth_input_hexdump(bff, len);
  return len;
}

/**
 * @brief  Handles packet transmission
 * @param  bff: Buffer containing packet to send
 * @param  len: Length of packet
 */
void DM_ETH_Output(uint8_t *bff, uint16_t len)
{
  dm9051_tx(bff, len);
}

/**
 * @brief  Network configuration functions
 */
const uint8_t *DM_ETH_Ip_Configuration(const uint8_t *ip)
{
	identify_tcpip_ip(ip);
	return dm_eth_show_identified_ip(ip ? "config ip" : "candidate ip");
}

const uint8_t *DM_ETH_Gw_Configuration(const uint8_t *ip)
{
	identify_tcpip_gw(ip);
	return dm_eth_show_identified_gw(ip ? "config gw" : "candidate gw");
}

const uint8_t *DM_ETH_Mask_Configuration(const uint8_t *ip)
{
  identify_tcpip_mask(ip);
  return identified_tcpip_mask();
}

/**
 * @brief  Network configuration retrieval functions
 */
uint8_t *DM_ETH_Ip_Configured(void)
{
  return identified_tcpip_ip();
}

uint8_t *DM_ETH_Gw_Configured(void)
{
  return identified_tcpip_gw();
}

uint8_t *DM_ETH_Mask_Configured(void)
{
  return identified_tcpip_mask();
}

/**
 * @brief  Debug function for RX pointer calculation
 */
uint16_t DM_ETH_ToCalc_rx_pointers(int state, const uint16_t *mdra_rd_org, uint16_t *mdra_rd_now)
{
  static uint16_t dummy_rwpa;
  dm9051_read_rx_pointers(&dummy_rwpa, mdra_rd_now);
  debug_diff_rx_pointers(state, *mdra_rd_now);
  return (state == 0) ? 0 : wrpadiff(*mdra_rd_org, *mdra_rd_now);
}

/**
 * @brief  Checks if link is up based on configured source
 * @param  stat: Status register values
 * @return 1 if link is up, 0 otherwise
 */
int DM_Eth_Regs_Info_Linkup(uint8_t *stat)
{
	enum link_state_t lst = LINK_STATE_SOURCE;
	if (lst == DM9051_CHECK_MAC) {
		return stat[1] & 0x40 ? 1 : 0;  /* NSR register */
	}
	else if (lst == DM9051_CHECK_PHY) {
		return stat[5] & 0x04 ? 1 : 0;  /* BMSR register */
	}
	return 1;
//#define DM9051_CHECK_MAC				1
//#define DM9051_CHECK_PHY				2
//#if LINK_STATE_SOURCE == DM9051_CHECK_MAC
//#endif
//#if LINK_STATE_SOURCE == DM9051_CHECK_PHY
//#endif
}

/**
 * @brief  Reads and processes register information (could periodic call)
 * @param  stat: Buffer for register values
 */
void DM_Eth_ReadRegsInfo(uint8_t *stat)
{
  dm9051_read_regs_info(stat);
  if (!DM_Eth_Regs_Info_Linkup(stat))
		/* Resets the hex dump state for input processing */
    dm_eth_input_hexdump_reset();
}

int dm_eth_polling_downup(void)
{
	static int link_stat = 0;
	uint8_t statdat[6];

	DM_Eth_ReadRegsInfo(statdat);
	#if 1
	operate_led3(VIA_NET, DM_Eth_Regs_Info_Linkup(statdat) ? LED_FLASH : LED_OFF);
	#endif
	if (DM_Eth_Regs_Info_Linkup(statdat) && !link_stat) {
		link_stat = 1;
		printf("(down to link up)\r\n");
		return 1;
	} else if (!DM_Eth_Regs_Info_Linkup(statdat) && link_stat) {
		link_stat = 0;
		printf("(up2down to link down)\r\n");
		return 0;
	}
	return 0;
}

#define PRINT_ETH_WITH_HEADER	0

//with head-str
static void eth_printf(char *str) {
	printf("%s", str);
}

//no head-str
static void eth_printkey(char *str) {
	printkey("%s", str);
}

const uint8_t *dm_eth_show_identified_ip(char *headtypestr)
{
	print_eth_configuration(
		headtypestr, 
		identified_tcpip_ip(), 
		PRINT_ETH_WITH_HEADER ? 
			eth_printf: 
			eth_printkey);
	return identified_tcpip_ip();
}

const uint8_t *dm_eth_show_identified_gw(char *headtypestr)
{
	print_eth_configuration(
		headtypestr, 
		identified_tcpip_gw(),
		PRINT_ETH_WITH_HEADER ? 
			eth_printf: 
			eth_printkey);
  return identified_tcpip_gw();
}

void dm_eth_show_app_help_info(char *contentStr)
{
	printkey("\r\n\r\n\r\n[%s mode] /ZYK_project /R2410 [uip_dm9051_r2410] %s\r\n", RX_MODE_STR, contentStr);
}

/**
 * @brief  Init to Demo button control led3
 */
void dm_eth_polling_button_init(enum operate_tag tag)
{
	config_button();
	if (tag == OPS_LED3)
		config_led3();
}

/**
 * @brief  Periodically to Demo button control led3
 */
void dm_eth_polling_button_ops(enum operate_tag tag)
{
	static int button_stat = 0;

	if (button_is_pressed() == USER_BUTTON) {
		if (!button_stat) {
			dm_eth_show_app_help_info("polling_button_pressed");
			button_stat = 1;
		}
		if (tag == OPS_LED3)
			operate_led3(VIA_BUTTON, LED_FLASH);
	} else {
		if (button_stat) { //release button
			dm_eth_show_identified_ip("display config ip");
			dm_eth_show_identified_gw("display config gw");
			button_stat = 0;
		}
		if (tag == OPS_LED3)
			operate_led3(VIA_BUTTON, LED_OFF);
	}
}
