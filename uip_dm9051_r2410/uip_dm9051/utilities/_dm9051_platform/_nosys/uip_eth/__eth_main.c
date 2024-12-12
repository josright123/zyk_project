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

#if 1 //gpio example
#if defined(_DLW_AT32F437xx)

/*-----------------------------------------------------------------------------
 * GPIO Pin Configurations and Control Functions
 *
 * Example Implementations
 * The file includes two concrete GPIO implementations:
 * LED3 Pin
 *  .Output pin (PD14)
 *  .No pull-up/down
 *  .Functions: config_led3(), led3_lo(), led3_hi()
 * BUTTON Pin
 *  .Input pin (PA0)
 *  .Pull-down enabled
 *  .Functions: config_button(), button_is_pressed()
 * DIAG Pin
 *  .Output pin (PA15)
 *  .No pull-up/down
 *  .Functions: config_diag(), diag_lo(), diag_hi()
 * INPT Pin
 *  .Input pin (PC7)
 *  .Pull-up enabled
 *  .Functions: config_inpt(), inpt_get()
 *-----------------------------------------------------------------------------*/
/**
 * @brief GPIO configuration structures for commonly used pins
 */
struct gpio_config_t
	led3 = {
		GPIOD,		  // LED3_GPIO
		GPIO_PINS_14, // LED3_PIN
		CRM_GPIOD_PERIPH_CLOCK, // LED3_GPIO_CRM_CLK
		{
			GPIO_PULL_NONE,
			GPIO_MODE_OUTPUT,
		}
},
	button = {
		GPIOA,		 // USER_BUTTON_PORT
		GPIO_PINS_0, // USER_BUTTON_PIN
		CRM_GPIOA_PERIPH_CLOCK, // USER_BUTTON_CRM_CLK
		{
			GPIO_PULL_DOWN,
			GPIO_MODE_INPUT,
		}
},
	diag = {
		GPIOA,
		GPIO_PINS_15,
		CRM_GPIOA_PERIPH_CLOCK,
		{
			GPIO_PULL_NONE,
			GPIO_MODE_OUTPUT,
		}
},
	inpt = {
		GPIOC,
		GPIO_PINS_7,
		CRM_GPIOC_PERIPH_CLOCK,
		{
			GPIO_PULL_UP,
			GPIO_MODE_INPUT,
		}
};

/**
 * @brief  LED3 control functions
 */
void config_led3(void)
{
	dm9051if_stdpin_config(&led3);
}

void led3_toggle(void)
{
	led3.port->odt ^= led3.pin; //gpio_stdpin_toggle
}

void led3_on(void)
{
	dm9051if_stdpin_lo(&led3); //gpio_stdpin_lo
}

void led3_off(void)
{
	dm9051if_stdpin_hi(&led3); //gpio_stdpin_hi
}

/**
 * @brief  User button control functions
 */
void config_button(void)
{
	dm9051if_stdpin_config(&button);
}

button_type button_is_pressed(void)
{
	return (gpio_stdpin_get(&button) == SET) ? USER_BUTTON : NO_BUTTON;
}

/**
 * @brief  Periodically to Demo toggle led3
 */
#define NMS 250

struct led_control_t {
    uint32_t start_time;
    uint32_t interval;
    confirm_state is_active; //bool is_active;
    //sled_ops_state state;
};

static struct led_control_t led_control[2] = {0};

void led_start_alloc(trigger_type trigger, uint32_t now)
{
	struct led_control_t *ctrl = &led_control[trigger];

	ctrl->start_time = now;

	trigger++;
	if (trigger > VIA_NET)
		trigger = VIA_BUTTON;
	ctrl = &led_control[trigger];
	ctrl->start_time = now + (NMS >> 1);
}

void operate_led3(trigger_type trigger, led_ops_state ops)
{
	//static uint32_t statime[2] = {0, 0};
	//static uint32_t intvltime[2];
	struct led_control_t *ctrl = &led_control[trigger];
	uint32_t elapsed;

	if (ops == LED_FLASH)
	{
		uint32_t now = dm9051_boards_heartbeat_now();
		if (!ctrl->is_active) //(!statime[trigger])
		{
			led_start_alloc(trigger, now); //ctrl->start_time = now; //statime[trigger] = now;
			ctrl->interval = NMS; //intvltime[trigger] = NMS;
			ctrl->is_active = TRUE;
			led3_toggle(); //led3_on
			return;
		}

		elapsed = now - ctrl->start_time; //statime[trigger];
		if (elapsed >= ctrl->interval) //intvltime[trigger]
		{
			if (ctrl->interval == NMS) //intvltime[trigger]
				led3_toggle(); //led3_off
			if (ctrl->interval == (NMS * 2))
				ctrl->is_active = FALSE; //statime[trigger] = 0;
			ctrl->interval += NMS;
		}
	}
	else
	{
		/* for only turn led off once by per operated trigger source!
		 */
		if (ctrl->is_active) { //statime[trigger]
			led3_off();
			ctrl->is_active = FALSE; //statime[trigger] = 0;
		}
	}
}

/**
 * @brief  Diagnostic pin control functions
 */
void config_diag(void)
{
	dm9051if_stdpin_config(&diag);
}

void diag_lo(void)
{
	dm9051if_stdpin_lo(&diag); //gpio_stdpin_lo
}

void diag_hi(void)
{
	dm9051if_stdpin_hi(&diag); //gpio_stdpin_hi
}

/**
 * @brief  Input pin control functions
 */
void config_inpt(void)
{
	dm9051if_stdpin_config(&inpt);
}

flag_status inpt_get(void)
{
	return gpio_stdpin_get(&inpt);
}

#endif /* _DLW_AT32F437xx */
#endif

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

void DM_ETH_IRQInit(void)
{
  flgSemaphore_r = 0;
}

/**
 * @brief  Prepares ISR for reset operation
 */
//void DM_ETH_ToRst_ISR(void)
//{
//  dm9051_isr_enab();
//  identify_irq_stat(ISTAT_IRQ_NOW2END);
//}

/**
 * @brief  Initializes the Ethernet interface
 * @param  adr: MAC address pointer
 * @return Pointer to configured MAC address
 */
//const uint8_t *DM_ETH_Init(const uint8_t *adr)
//{
  //flgSemaphore_r = 0;

  //dm9051_boards_initialize(); //(&board_init_struct);
  //return dm9051_init(adr);
//}

/**
 * @brief  Handles packet reception
 * @param  bff: Buffer for received packet
 * @return Length of received packet
 */
//uint16_t DM_ETH_Input(uint8_t *bff)
//{
//  uint16_t len = dm9051_rx(bff);
//  dm_eth_input_hexdump(bff, len);
//  return len;
//}

////.#if LWIP_PTP
//uint16_t DM_ETH_PTP_Input(uint8_t *bff, uint8_t *ts_bff)
//{
//  uint16_t len = dm9051_rx(bff);
//  dm_eth_input_hexdump(bff, len);
//  return len;
//}
////#endif

/**
 * @brief  Handles packet transmission
 * @param  bff: Buffer containing packet to send
 * @param  len: Length of packet
 */
//void DM_ETH_Output(uint8_t *bff, uint16_t len)
//{
//  dm9051_tx(bff, len);
//}

////.#if LWIP_PTP
//void DM_ETH_PTP_Output(uint8_t *bff, uint16_t len, uint8_t *ts_bff)
//{
//  dm9051_tx(bff, len);
//  //dm9051_get_tx_time(ts_bff);
//}

//void DM_ETH_PTP_HW_TIMESTAMP_Output(uint8_t *bff, uint16_t len, uint8_t *ts_bff)
//{
//  dm9051_tx(bff, len); //[send with hw-timestamp]
//  //dm9051_get_tx_time(ts_bff);
//}
////#endif

/**
 * @brief  Network configuration functions
 */
const uint8_t *DM_ETH_Ip_Configuration(const uint8_t *ip)
{
//	static uint8_t ip_printag = 0x1;

//	if (ip_printag & 0x01) {
//		ip_printag &= ~0x01;
//		identify_tcpip_ip(ip);
//		return dm_eth_show_identified_ip(ip ? "config ip" : "candidate ip");
//	}
	return identify_tcpip_ip(ip);
}

const uint8_t *DM_ETH_Gw_Configuration(const uint8_t *ip)
{
//	static uint8_t gw_printag = 0x1;

//	if (gw_printag & 0x01) {
//		gw_printag &= ~0x01;
//		identify_tcpip_gw(ip);
//		return dm_eth_show_identified_gw(ip ? "config gw" : "candidate gw");
//	}
	return identify_tcpip_gw(ip);
}

const uint8_t *DM_ETH_Mask_Configuration(const uint8_t *ip)
{
  return identify_tcpip_mask(ip);
  //return identified_tcpip_mask();
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
#if DM_ETH_DEBUG_MODE
uint16_t DM_ETH_ToCalc_rx_pointers(int state, const uint16_t *mdra_rd_org, uint16_t *mdra_rd_now)
{
  static uint16_t dummy_rwpa;
  dm9051_read_rx_pointers(&dummy_rwpa, mdra_rd_now);
  debug_diff_rx_pointers(state, *mdra_rd_now);
  return (state == 0) ? 0 : wrpadiff(*mdra_rd_org, *mdra_rd_now);
}
#endif

/**
 * @brief  Checks if link is up based on configured source
 * @param  stat: Status register values
 * @return 1 if link is up, 0 otherwise
 */
int DM_Eth_Info_Linkup(uint8_t *stat)
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
void DM_Eth_Read_Info(uint8_t *stat)
{
  dm9051_read_regs_info(stat);
  if (!DM_Eth_Info_Linkup(stat))
		/* Resets the hex dump state for input processing */
    dm_eth_input_hexdump_reset();
}

int dm_eth_polling_downup(void)
{
	static int link_stat = 0;
	uint8_t statdat[6];

	DM_Eth_Read_Info(statdat);
	#if 1
	operate_led3(VIA_NET, DM_Eth_Info_Linkup(statdat) ? LED_FLASH : LED_OFF);
	#endif
	if (DM_Eth_Info_Linkup(statdat) && !link_stat) {
		link_stat = 1;
		printf("(down to link up)\r\n");
		return 1;
	} else if (!DM_Eth_Info_Linkup(statdat) && link_stat) {
		link_stat = 0;
		printf("(up2down to link down)\r\n");
		return 0;
	}
	return 0;
}

void dm_eth_show_app_help_info(char *drv_modeS, char *statusS, char *dateS)
{
	//printf
	printkey("\r\n\r\n\r\n[%s mode] /ZYK_project /R2410 [uip_dm9051_r2410] %s %s\r\n", drv_modeS, statusS, dateS);
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
			dm_eth_show_app_help_info(RX_MODE_STR, "polling_button_pressed", __DATE__);
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
