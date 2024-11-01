/**
 **************************************************************************
 * @file gpio_hal.c
 **************************************************************************
 */
#include "gpio_hal.h"

#if defined(_DLW_AT32F437xx)

// GPIO Structure
struct gpio_config_t
{
	gpio_type *port;
	uint16_t pin;
	gpio_pins_source_type source;
	gpio_mux_sel_type mux;
	crm_periph_clock_type clock;
	gpio_mode_type mode;
};

// GPIO Definitions
struct gpio_define_t
{
	struct gpio_config_t diag, inpt;
};

// GPIO Data
const struct gpio_define_t gpio_set[1] = {{
		{GPIOA, GPIO_PINS_15, (gpio_pins_source_type)0, (gpio_mux_sel_type)0, CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_OUTPUT},
		{GPIOC, GPIO_PINS_7, (gpio_pins_source_type)0, (gpio_mux_sel_type)0, CRM_GPIOC_PERIPH_CLOCK, GPIO_MODE_INPUT},
	}};

#define ecs() (gpio_set[0].diag)
#define eint() (gpio_set[0].inpt)

void gpio_hal_cs_lo(void)
{
	gpio_bits_reset(ecs().port, ecs().pin); //(gpio_type *gpioport, uint16_t gpiopin, flag_status level);
}
void gpio_hal_cs_hi(void)
{
	gpio_bits_set(ecs().port, ecs().pin); //(gpio_type *gpioport, uint16_t gpiopin, flag_status level);
}

/* Static function, as is configure_cpin()
 */
static void configure_cgpio(const struct gpio_config_t *ps, gpio_pull_type gppull) {
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

void gpio_hal_init(void)
{
	configure_cgpio(&ecs(), GPIO_PULL_NONE);
	configure_cgpio(&eint(), GPIO_PULL_UP); //for interrupt configure, Not all devices essential]
}

//void gpio_hal_diag_lo(void)
//{
//	gpio_bits_reset(ecs().port, ecs().pin); //(gpio_type *gpioport, uint16_t gpiopin, flag_status level);
//}
//void gpio_hal_diag_hi(void)
//{
//	gpio_bits_set(ecs().port, ecs().pin); //(gpio_type *gpioport, uint16_t gpiopin, flag_status level);
//}

//flag_status gpio_hal_inpt_get(void)
//{
//	return gpio_input_data_bit_read(eint().port, eint().pin); //(gpio_type *gpioport, uint16_t gpiopin);
//}
#endif //_DLW_AT32F437xx
