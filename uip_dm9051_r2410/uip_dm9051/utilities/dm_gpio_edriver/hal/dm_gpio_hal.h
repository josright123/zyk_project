#ifndef __DMGPIO_HAL_MCU_H
#define __DMGPIO_HAL_MCU_H

// #define _DLW_AT32F415xx
// #define _DLW_AT32F437xx
// #define _DLW_M051xx
#define _DLW_AT32F437xx

#if defined(_DLW_M051xx)
#include "M051Series.h"
#elif defined(_DLW_AT32F437xx)
#include "at32f435_437_board.h" //mcu's board
#include "at32f435_437_clock.h" //Also mcu's clock
#elif defined(_DLW_AT32F415xx)
#include "at32f415_board.h" //mcu's board
#include "at32f415_clock.h" //Also mcu's clock
#elif defined(_DLW_AT32F413xx)
#include "at32f413_board.h" //mcu's board
#include "at32f413_clock.h" //Also mcu's clock
#elif defined(_DLW_AT32F403Axx) || defined(_DLW_AT32F403xx) || defined(_DLW_AT32F407xx)
#include "at32f403a_407_board.h" //mcu's board
#include "at32f403a_407_clock.h" //Also mcu's clock
#else
/*
	#error "opts board to be define"
	While NOT include the mcu-definition in the program code, in advance, previously.
	Add your board's board & clock header files here! Refer to above cases.

	Usually, Cn find the expected included files below in main.c
*/
#error "Opps, Opts board to be define  \r\n \
		While NOT include the mcu-definition in the program code, in advance, previously. \r\n \
		Usually, Project find the expected included files below in Options for Target > C/C++ > Preprocessor Definitions > Add > _AT32F4xxxx \r\n \
		ex: _AT32F407xx,USE_STDPERIPH_DRIVER,AT_START_F407_V1 \r\n \
		ex: _AT32F437xx,USE_STDPERIPH_DRIVER,AT_START_F437_V1 \r\n \ "
#endif

// GPIO Configuration Structure
struct gpio_config_t
{
	gpio_type *port;
	uint16_t pin;
	gpio_pins_source_type source;
	gpio_mux_sel_type mux;
	crm_periph_clock_type clock;
	gpio_mode_type mode;
};

  //
  // #include "dm9051_hal_api.h"
  //

#define dm_gpio_hal_init dm_gpio_initialize

/* Hw_common funcrions
 */
void dm_gpio_hal_init(void);

/* Calling example:
 *   cqpio_write(diag_gpio_port(), diag_gpio_pin(), diag_gpio_lo());
 *   cqpio_write(diag_gpio_port(), diag_gpio_pin(), diag_gpio_hi());
 *   cqpio_read(in_gpio_port(), in_gpio_pin());
 */
#define diag_gpio_port() gpio_out[0].port //calling use
#define diag_gpio_pin() gpio_out[0].pin //calling use
#define diag_gpio_lo() RESET //calling use
#define diag_gpio_hi() SET //calling use
#define in_gpio_port() gpio_in[0].port //calling use
#define in_gpio_pin() gpio_in[0].pin //calling use

extern const struct gpio_config_t gpio_out[1];
extern const struct gpio_config_t gpio_in[1];

void cqpio_write(gpio_type *port, uint16_t pin, flag_status lev);
flag_status cqpio_read(gpio_type *port, uint16_t pin);
  
#endif //__DMGPIO_HAL_MCU_H
