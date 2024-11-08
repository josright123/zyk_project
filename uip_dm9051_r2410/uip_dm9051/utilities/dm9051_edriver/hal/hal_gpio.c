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
 * @author  Joseph CHANG
 * @version 1.0
 * @date    2024-11-05
 *
 * @api     Pin Configuration:
 *          - gpio_hal_stdpin_config()  : Standard pin configuration
 *          - gpio_hal_muxpin_config()  : Multiplexed pin configuration
 *
 *          Pin Control:
 *          - gpio_hal_stdpin_hi()      : Set pin high
 *          - gpio_hal_stdpin_lo()      : Set pin low
 *          - gpio_hal_stdpin_get()     : Read pin state
 **************************************************************************
 */

#include "control/drv_control/conf_core.h"

#if defined(_DLW_AT32F437xx)

#define cint_enable_mcu_irq_AT cint_enable_mcu_irq
#define	dm9051_hal_irqline HAL_IRQLine

// GPIO Data
struct gpio_config_t
cs = {
		GPIOA, GPIO_PINS_15, GPIO_PULL_NONE, CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_OUTPUT,
},
intr = {
		GPIOC, GPIO_PINS_7, GPIO_PULL_UP, CRM_GPIOC_PERIPH_CLOCK, GPIO_MODE_INPUT,
};

const struct interrupt_config_t intr_cset[1] = {
	{
		CRM_SCFG_PERIPH_CLOCK,
		CRM_GPIOC_PERIPH_CLOCK,
		SCFG_PORT_SOURCE_GPIOC,
		SCFG_PINS_SOURCE7,
		EXINT_LINE_7,
		NVIC_PRIORITY_GROUP_0,
		EXINT9_5_IRQn,
	}};

/**
 * @brief  Configures a standard GPIO pin
 * @param  gpio: Pointer to GPIO configuration structure
 * @note   This function handles:
 *         - Clock enabling
 *         - Pin mode configuration
 *         - Pull-up/down configuration
 *         - Drive strength setting
 */
void gpio_hal_stdpin_config(const struct gpio_config_t *gpio)
{
	gpio_init_type gpio_init_struct;

	/* Enable peripheral clock for selected GPIO port */
	crm_periph_clock_enable(gpio->clock, TRUE);

	/* Initialize GPIO structure with default values */
	gpio_default_para_init(&gpio_init_struct);

	/* Configure GPIO parameters */
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_mode = gpio->mode;
	gpio_init_struct.gpio_pull = gpio->pull;
	gpio_init_struct.gpio_pins = gpio->pin;

	/* Apply configuration */
	gpio_init(gpio->port, &gpio_init_struct);
}

/**
 * @brief  Configures a multiplexed GPIO pin
 * @param  gpiomux: Pointer to multiplexed GPIO configuration structure
 * @note   Extends standard configuration with multiplexing capabilities
 */
void gpio_hal_muxpin_config(const struct gpio_mux_t *gpiomux)
{
	/* Configure standard GPIO parameters first */
	gpio_hal_stdpin_config(&gpiomux->gpio);

#if defined(_DLW_AT32F437xx)
	/* Configure multiplexing if in MUX mode */
	if (gpiomux->gpio.mode == GPIO_MODE_MUX)
		gpio_pin_mux_config(gpiomux->gpio.port, gpiomux->source, gpiomux->mux);
#endif
}

/**
 * @brief  Sets GPIO pin to low state
 * @param  gpio: Pointer to GPIO configuration structure
 */
void gpio_hal_stdpin_lo(const struct gpio_config_t *gpio)
{
	gpio_bits_reset(gpio->port, gpio->pin);
}

/**
 * @brief  Sets GPIO pin to high state
 * @param  gpio: Pointer to GPIO configuration structure
 */
void gpio_hal_stdpin_hi(const struct gpio_config_t *gpio)
{
	gpio_bits_set(gpio->port, gpio->pin);
}

/**
 * @brief  Reads current state of GPIO pin
 * @param  gpio: Pointer to GPIO configuration structure
 * @return flag_status: SET if pin is high, RESET if pin is low
 */
flag_status gpio_hal_stdpin_get(const struct gpio_config_t *gpio)
{
	return gpio_input_data_bit_read(gpio->port, gpio->pin);
}

//[hw]
//static void configure_cirq(const struct interrupt_config_t *cf,
//						   exint_polarity_config_type polarity)
//{
//}

// Static function prototypes
void interrupt_config_init(const struct interrupt_config_t *cf)
{
	//=configure_cirq(config, EXINT_TRIGGER_FALLING_EDGE);
	#ifdef ETHERNET_INTERRUPT_MODE
	exint_polarity_config_type polarity = EXINT_TRIGGER_FALLING_EDGE;
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
	exint_init_struct.line_polarity = polarity;
	exint_init(&exint_init_struct);

	identify_irq_stat(ISTAT_IRQ_CFG);
	trace_irq_stat(ISTAT_IRQ_CFG);

	identify_irq_stat(ISTAT_LOW_TRIGGER);
	trace_irq_stat(ISTAT_LOW_TRIGGER);
	#endif
}

void cint_disable_mcu_irq_AT(void)
{
	deidentify_irq_stat(ISTAT_IRQ_ENAB);
	nvic_irq_disable(nvic_irqn());
}

void cint_enable_mcu_irq_AT(void)
{
	identify_irq_stat(ISTAT_IRQ_ENAB);
	trace_irq_stat(ISTAT_IRQ_ENAB);

	nvic_priority_group_config(nvic_prio());
	nvic_irq_enable(nvic_irqn(), 1, 0);
}

uint32_t dm9051_hal_irqline(void)
{
	return irq_line();
}

#if 1
//void dm9051if_gpio_lo(void) {
//	gpio_hal_stdpin_lo(&cs);
//}
//void dm9051if_gpio_hi(void) {
//	gpio_hal_stdpin_hi(&cs);
//}
#endif

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
		GPIO_PULL_NONE,
		CRM_GPIOD_PERIPH_CLOCK, // LED3_GPIO_CRM_CLK
		GPIO_MODE_OUTPUT,
},
	button = {
		GPIOA,		 // USER_BUTTON_PORT
		GPIO_PINS_0, // USER_BUTTON_PIN
		GPIO_PULL_DOWN,
		CRM_GPIOA_PERIPH_CLOCK, // USER_BUTTON_CRM_CLK
		GPIO_MODE_INPUT,
},
	diag = {
		GPIOA,
		GPIO_PINS_15,
		GPIO_PULL_NONE,
		CRM_GPIOA_PERIPH_CLOCK,
		GPIO_MODE_OUTPUT,
},
	inpt = {
		GPIOC,
		GPIO_PINS_7,
		GPIO_PULL_UP,
		CRM_GPIOC_PERIPH_CLOCK,
		GPIO_MODE_INPUT,
};

/**
 * @brief  LED3 control functions
 */
void config_led3(void)
{
	gpio_hal_stdpin_config(&led3);
}

void led3_on(void)
{
	gpio_hal_stdpin_lo(&led3);
}

void led3_off(void)
{
	gpio_hal_stdpin_hi(&led3);
}

/**
 * @brief  User button control functions
 */
void config_button(void)
{
	gpio_hal_stdpin_config(&button);
}

button_type button_is_pressed(void)
{
	return gpio_hal_stdpin_get(&button) == SET ? USER_BUTTON : NO_BUTTON;
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
#define NMS	250
static int control_slice_led3(uint32_t diff, uint32_t intvl_expire)
{
	if (diff >= intvl_expire)
	{
				switch (intvl_expire) {
					case (NMS):
						led3_off();
						break;
					case (2*NMS):
						//statime[trig_src] = 0;
						break;
				}
				return 1;
	}
	return 0;
}

void toggle_led3(int trig_src, button_type usr_button)
{
	static uint32_t statime[2] = { 0, 0};
	static uint32_t intvltime[2];
	
	if (usr_button == USER_BUTTON) {
			uint32_t now = dm_sys_now();
			if (!statime[trig_src]) {
				statime[trig_src] = now;
				intvltime[trig_src] = NMS;
				led3_on();
				return;
			}
			
			if (control_slice_led3(now - statime[trig_src], intvltime[trig_src])) {
				if (intvltime[trig_src] == (2*NMS))
					statime[trig_src] = 0;
				intvltime[trig_src] += NMS;
			}
			
			//if ((now - statime[trig_src]) >= intvltime[trig_src])
			//{
			//  if (intvltime[trig_src] == (NMS))
			//			led3_off();
			//  if (intvltime[trig_src] == (2*NMS))
			//			statime[trig_src] = 0;
			//	intvltime[trig_src] += NMS;
			//}
		} else {
			/* for only turn led off once by per operated trigger source! 
			 */
			if (statime[trig_src])
				led3_off();
			statime[trig_src] = 0;
	}
}

/**
 * @brief  Periodically to Demo button control led3
 */
void button_toggle_led3(void)
{
	toggle_led3(LED3_BY_BUTTON, button_is_pressed());
}

/**
 * @brief  Diagnostic pin control functions
 */
void config_diag(void)
{
	gpio_hal_stdpin_config(&diag);
}

void diag_lo(void)
{
	gpio_hal_stdpin_lo(&diag);
}

void diag_hi(void)
{
	gpio_hal_stdpin_hi(&diag);
}

/**
 * @brief  Input pin control functions
 */
void config_inpt(void)
{
	gpio_hal_stdpin_config(&inpt);
}

flag_status inpt_get(void)
{
	return gpio_hal_stdpin_get(&inpt);
}

#endif /* _DLW_AT32F437xx */
