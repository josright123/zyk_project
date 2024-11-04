/**
 **************************************************************************
 * @file hal_gpio.c (gpio_utils.c)
 **************************************************************************
 */
#include "control/drv/conf_core.h" //#include "hal_api.h" //"gpio_utils.h"

#if defined(_DLW_AT32F437xx)

//void configure_cgpio(gpio);
void gpio_hal_stdpin_config(const struct gpio_config_t *gpio)
{
	gpio_init_type gpio_init_struct;

	/* enable the gpio clock
	 */
	crm_periph_clock_enable(gpio->clock, TRUE);
	gpio_default_para_init(&gpio_init_struct);
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_mode = gpio->mode;
	gpio_init_struct.gpio_pull = gpio->pull; // GPIO_PULL_DOWN; GPIO_PULL_UP; //GPIO_PULL_NONE;
	gpio_init_struct.gpio_pins = gpio->pin;
	gpio_init(gpio->port, &gpio_init_struct);
}

//void configure_cgpio_mux(gpiomux);
void gpio_hal_muxpin_config(const struct gpio_mux_t *gpiomux)
{
	gpio_hal_stdpin_config(&gpiomux->gpio); //configure_cgpio(&gpiomux->gpio);
	
	#if defined(_DLW_AT32F437xx)
	if (gpiomux->gpio.mode == GPIO_MODE_MUX)
		gpio_pin_mux_config(gpiomux->gpio.port, gpiomux->source, gpiomux->mux);
	#endif
}

void gpio_hal_stdpin_lo(const struct gpio_config_t *gpio)
{
	gpio_bits_reset(gpio->port, gpio->pin);
}
void gpio_hal_stdpin_hi(const struct gpio_config_t *gpio)
{
	gpio_bits_set(gpio->port, gpio->pin);
}

flag_status gpio_hal_stdpin_get(const struct gpio_config_t *gpio)
{
	return gpio_input_data_bit_read(gpio->port, gpio->pin);
}

//============================================
// GPIO Data
// Customization gpio declaration define and
// Customization gpio operation control.
//============================================
struct gpio_config_t
diag = {
		GPIOA, GPIO_PINS_15, GPIO_PULL_NONE, CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_OUTPUT,
},
inpt = {
		GPIOC, GPIO_PINS_7, GPIO_PULL_UP, CRM_GPIOC_PERIPH_CLOCK, GPIO_MODE_INPUT,
};

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

void config_inpt(void)
{
	gpio_hal_stdpin_config(&inpt);
}

flag_status inpt_get(void)
{
	return gpio_hal_stdpin_get(&inpt);
}

/* general programable input/output function
 */
//void configure_cgpio(const struct gpio_config_t *gpio) {
//	gpio_init_type gpio_init_struct;

//	/* enable the gpio clock
//	 */
//	crm_periph_clock_enable(gpio->clock, TRUE);
//	gpio_default_para_init(&gpio_init_struct);
//	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
//	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
//	gpio_init_struct.gpio_mode = gpio->mode;
//	gpio_init_struct.gpio_pull = gpio->pull; // GPIO_PULL_DOWN; GPIO_PULL_UP; //GPIO_PULL_NONE;
//	gpio_init_struct.gpio_pins = gpio->pin;
//	gpio_init(gpio->port, &gpio_init_struct);
//}

/* Export configure_cmux function, is configure_cgpio() plus the mux config
 */
//void configure_cgpio_mux(const struct gpio_mux_t *gpiomux)
//{
//	configure_cgpio(&gpiomux->gpio);
//	
//	#if defined(_DLW_AT32F437xx)
//	if (gpiomux->gpio.mode == GPIO_MODE_MUX)
//		gpio_pin_mux_config(gpiomux->gpio.port, gpiomux->source, gpiomux->mux);
//	#endif
//}

//#define ecs() (cs) //(gpio_set[0].cs)
//#define cintr() (intr) //(gpio_set[0].intr)

//.void gpio_hal_init(void)
//.{
//.	configure_cgpio(&cs);
//.	configure_cgpio(&intr); //for interrupt configure, Not all devices essential]
//.}

//void gpio_hal_diag_lo(void)
//{
//	gpio_bits_reset(ecs().port, ecs().pin); //(gpio_type *gpioport, uint16_t gpiopin, flag_status level);
//}
//void gpio_hal_diag_hi(void)
//{
//	gpio_bits_set(ecs().port, ecs().pin); //(gpio_type *gpioport, uint16_t gpiopin, flag_status level);
//}

//flag_status gpio_hal_intr_get(void)
//{
//	return gpio_input_data_bit_read(eint().port, eint().pin); //(gpio_type *gpioport, uint16_t gpiopin);
//}
#endif //_DLW_AT32F437xx
