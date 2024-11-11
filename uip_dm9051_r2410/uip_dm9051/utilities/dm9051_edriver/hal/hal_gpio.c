/**
 **************************************************************************
 * @file    hal_gpio.c
 * @brief   GPIO Hardware Abstraction Layer implementation for AT32F437
 *
 * @details This module provides hardware abstraction for General Purpose
 *          Input/Output (GPIO) operations on the AT32F437 microcontroller.
 *          It includes pin configuration, multiplexing, and control operations.
 *
 * @note    Supported Features:
 *          - Standard and multiplexed pin configurations
 *          - Input/Output operations with configurable pull-up/down
 *          - Pin state control and reading
 *
 * @version 1.0
 * @author  Joseph CHANG
 * @copyright (c) 2023-2025 Davicom Semiconductor, Inc.
 * @date    2024-11-05
 *
 * @api     Pin Configuration:
 *          - AT_hal_stdpin_config()  : Standard pin configuration
 *           (AT_hal_muxpin_config()  : Multiplexed pin configuration)
 *
 *          Pin Control:
 *          - AT_hal_stdpin_lo()      : Set pin high
 *          - AT_hal_stdpin_hi()      : Set pin low
 *          - AT_hal_stdpin_get()     : Read pin state
 **************************************************************************
 */

#include "control/drv_control/conf_core.h"

#if defined(_DLW_AT32F437xx)

//#define gpio_stdpin_lo AT_hal_stdpin_lo
//#define gpio_stdpin_hi AT_hal_stdpin_hi
//#define dm9051if_stdpin_lo			_AT_hal_stdpin_lo
//#define dm9051if_stdpin_hi			_AT_hal_stdpin_hi
//#define AT_hal_stdpin_lo gpio_stdpin_lo
//#define AT_hal_stdpin_hi gpio_stdpin_hi

#define AT_hal_irqline HAL_IRQLine

// ---------------------- data_impl -----------------------------------------------------------
// GPIO Data
const struct gpio_config_t
	cs_gpio = {
		GPIOA,
		GPIO_PINS_15,
		CRM_GPIOA_PERIPH_CLOCK,
		{
			GPIO_PULL_NONE,
			GPIO_MODE_OUTPUT,
		}
},
	intr_gpio = {
		GPIOC,
		GPIO_PINS_7,
		CRM_GPIOC_PERIPH_CLOCK,
		{
			GPIO_PULL_UP,
			GPIO_MODE_INPUT,
		}
};

const struct interrupt_pack_t intr_cset[1] = {{
		{
			CRM_SCFG_PERIPH_CLOCK,
			CRM_GPIOC_PERIPH_CLOCK,
			SCFG_PORT_SOURCE_GPIOC,
			SCFG_PINS_SOURCE7,
			EXINT_LINE_7,
			NVIC_PRIORITY_GROUP_0,
			EXINT9_5_IRQn,
		},
		EXINT_TRIGGER_FALLING_EDGE,
	}};

// ---------------------- hw_impl -------------------------------------------------------------
/**
 * @brief  Configures a standard GPIO pin
 * @param  gpio: Pointer to GPIO configuration structure
 * @note   This function handles:
 *         - Clock enabling
 *         - Pin mode configuration
 *         - Pull-up/down configuration
 *         - Drive strength setting
 */
void AT_hal_stdpin_config(const struct gpio_config_t *gpio)
{
	gpio_init_type gpio_init_struct;

	/* Enable peripheral clock for selected GPIO port */
	crm_periph_clock_enable(gpio->clock, TRUE);

	/* Initialize GPIO structure with default values */
	gpio_default_para_init(&gpio_init_struct);

	/* Configure GPIO parameters */
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_mode = gpio->pinland.mode;
	gpio_init_struct.gpio_pull = gpio->pinland.pull;
	gpio_init_struct.gpio_pins = gpio->pin;

	/* Apply configuration */
	gpio_init(gpio->port, &gpio_init_struct);

	/* Configure multiplexing if in MUX mode */
#if defined(_DLW_AT32F437xx)
	if (gpio->pinland.mode == GPIO_MODE_MUX)
		gpio_pin_mux_config(gpio->port, gpio->pinland.source, gpio->pinland.mux);
#endif
}

/**
 * @brief  Configures a multiplexed GPIO pin
 * @param  gpiomux: Pointer to multiplexed GPIO configuration structure
 * @note   Extends standard configuration with multiplexing capabilities
 */
//void AT_hal_muxpin_config(const struct gpio_config_t *gpio)
//{
	/* Configure standard GPIO parameters first */
//	dm9051if_stdpin_config(gpio);

	/* Configure multiplexing if in MUX mode */
//	if (gpio->mode == GPIO_MODE_MUX)
//		gpio_pin_mux_config(gpio->port, gpio->pinland.source, gpio->pinland.mux);
//}

/**
 * @brief  Sets GPIO pin to low state
 * @param  gpio: Pointer to GPIO configuration structure
 */
void AT_hal_stdpin_lo(const struct gpio_config_t *gpio)
{
	gpio_bits_reset(gpio->port, gpio->pin);
}

/**
 * @brief  Sets GPIO pin to high state
 * @param  gpio: Pointer to GPIO configuration structure
 */
void AT_hal_stdpin_hi(const struct gpio_config_t *gpio)
{
	gpio_bits_set(gpio->port, gpio->pin);
}

/**
 * @brief  Reads current state of GPIO pin
 * @param  gpio: Pointer to GPIO configuration structure
 * @return flag_status: SET if pin is high, RESET if pin is low
 */
flag_status AT_hal_stdpin_get(const struct gpio_config_t *gpio)
{
	return gpio_input_data_bit_read(gpio->port, gpio->pin);
}

// Static function prototypes
void AT_interrupt_config_init(const struct interrupt_pack_t *pack)
{
#ifdef ETHERNET_INTERRUPT_MODE
	const struct interrupt_config_t *cf = &(pack->cf);
	exint_polarity_config_type pol = pack->polarity; //EXINT_TRIGGER_FALLING_EDGE;
	exint_init_type exint_init_struct;

	/* config irq
	 */
	crm_periph_clock_enable(cf->scfgclock, TRUE);
	crm_periph_clock_enable(cf->clock, TRUE);

	scfg_exint_line_config(cf->port_source,
						   cf->pin_source);

	exint_default_para_init(&exint_init_struct);
	exint_init_struct.line_enable = TRUE;
	exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
	exint_init_struct.line_select = cf->line;
	exint_init_struct.line_polarity = pol;
	exint_init(&exint_init_struct);

	identify_irq_stat(ISTAT_IRQ_CFG);
	trace_irq_stat(ISTAT_IRQ_CFG);

	identify_irq_stat(ISTAT_LOW_TRIGGER);
	trace_irq_stat(ISTAT_LOW_TRIGGER);
#endif
}

void AT_hal_disable_mcu_irq(void)
{
	deidentify_irq_stat(ISTAT_IRQ_ENAB);
	nvic_irq_disable(nvic_irqn());
}

void AT_hal_enable_mcu_irq(void)
{
	identify_irq_stat(ISTAT_IRQ_ENAB);
	trace_irq_stat(ISTAT_IRQ_ENAB);

	nvic_priority_group_config(nvic_prio());
	nvic_irq_enable(nvic_irqn(), 1, 0);
}

uint32_t AT_hal_irqline(void)
{
	return irq_line();
}

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
	return gpio_stdpin_get(&button) == SET ? USER_BUTTON : NO_BUTTON;
}

/**
 * @brief  Init to Demo button control led3
 */
void button_toggle_led3_init(void)
{
	config_button();
	config_led3();
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

void toggle_led3(trigger_type trigger, led_ops_state ops)
{
	//static uint32_t statime[2] = {0, 0};
	//static uint32_t intvltime[2];
	struct led_control_t *ctrl = &led_control[trigger];
	uint32_t elapsed;

	if (ops == LED_FLASH)
	{
		uint32_t now = dm_sys_now();
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
 * @brief  Periodically to Demo button control led3
 */
void button_toggle_led3(void)
{
	toggle_led3(VIA_BUTTON, button_is_pressed() == USER_BUTTON ? LED_FLASH : LED_OFF);
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

// Configure and use LED3
//config_led3();
//led3_toggle();
//led3_on();
//led3_off();

//config_button();
//button_is_pressed();
