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

#include "control/drv/conf_core.h"

#if defined(_DLW_AT32F437xx)

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
 * @brief  Periodically to Demo button control led3
 */
void button_toggle_led3(void)
{
	//:led3_toggle(250) or :led3_toggle(0)
	uint16_t nms = (button_is_pressed() == USER_BUTTON) ? 250 : 0;
	
	//:led3_toggle(uint16_t nms)
	do {
		static uint32_t state_time;
		
		if (nms == 0) {
			state_time = 0;
			led3_off();
			return;
		}
		
		if (nms == 250) {
			if (!state_time) {
				state_time = dm_sys_now();
				led3_on();
				return;
			}
			if ((dm_sys_now() - state_time) > (2*nms)) {
				state_time = 0;
			} else
			if ((dm_sys_now() - state_time) > nms) {
				led3_off();
			}
		}
	} while(0);
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
