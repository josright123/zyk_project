/**
 **************************************************************************
 * @file     dm_gpio_hal.c
 * The use of structures for SPI and interrupt configurations allows
 *	for easy modification if needed.
 * There are separate functions for SPI initialization (spi_add) and
 *	interrupt initialization (intr_add).
 * The code includes status tracking functions (identify_irq_stat, trace_irq_stat)
 *	for debugging purposes.
 **************************************************************************
 */
//#include "control/drv/conf_core.h"
//#include "control/drv/dm9051_eth_debug.h"
#include "dm_gpio_hal.h"

void dm_gpio_hal_init(void);

#if defined(_DLW_AT32F437xx)
/* ------------------------------- AT32F437 configuration data ----------------------------------------- */

const struct gpio_config_t gpio_out[1] = {
		{GPIOA, GPIO_PINS_15, (gpio_pins_source_type)0, (gpio_mux_sel_type)0, CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_OUTPUT},
};
const struct gpio_config_t gpio_in[1] = {
		{GPIOC, GPIO_PINS_7, (gpio_pins_source_type)0, (gpio_mux_sel_type)0, CRM_GPIOC_PERIPH_CLOCK, GPIO_MODE_INPUT},
};

//#define diag_gpio_port() gpio_out[0].port //calling use
//#define diag_gpio_pin() gpio_out[0].pin //calling use
//#define diag_gpio_lo() RESET //calling use
//#define diag_gpio_hi() SET //calling use

static void gpio_config_init(void);

static void configure_cgpio(const struct gpio_config_t *ps, gpio_pull_type gppull)
{
	gpio_init_type gpio_init_struct;

	/* enable the gpio clock
	 */
	crm_periph_clock_enable(ps->clock, TRUE);
	gpio_default_para_init(&gpio_init_struct);
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_mode = ps->mode;
	gpio_init_struct.gpio_pull = gppull; // GPIO_PULL_DOWN; GPIO_PULL_UP; //GPIO_PULL_NONE;
	gpio_init_struct.gpio_pins = ps->pin;
	gpio_init(ps->port, &gpio_init_struct);
	#if defined(_DLW_AT32F437xx)
	if (ps->mode == GPIO_MODE_MUX)
		gpio_pin_mux_config(ps->port, ps->source, ps->mux);
	#endif
}

static void gpio_config_init(void) //(const struct gpio_xxx_t *config, int n)
{
	configure_cgpio(gpio_out, GPIO_PULL_NONE); //configure_cpin()
	configure_cgpio(gpio_in, GPIO_PULL_UP); //configure_cpin()
}

#endif
