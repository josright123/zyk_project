#ifndef __HAL_API_MCU_H
#define __HAL_API_MCU_H

// option: _DLW_AT32F415xx
// option: _DLW_AT32F437xx
// option: _DLW_M051xx
#ifndef _DLW_AT32F437xx
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

	Usually, Find the expected included files below in main.c
*/
#error "Opps, Opts board to be define  \r\n \
		While NOT include the mcu-definition in the program code, in advance, previously. \r\n \
		Usually, Project find the expected included files below in Options for Target > C/C++ > Preprocessor Definitions > Add > _AT32F4xxxx \r\n \
		ex: _AT32F407xx,USE_STDPERIPH_DRIVER,AT_START_F407_V1 \r\n \
		ex: _AT32F437xx,USE_STDPERIPH_DRIVER,AT_START_F437_V1 \r\n \ "
#endif
#endif //_DLW_AT32F437xx

// Interrupt Configuration Structure
struct interrupt_config_t
{
	/*struct gpio_config_t gpio; */
	crm_periph_clock_type scfgclock;
	crm_periph_clock_type clock;
	scfg_port_source_type port_source;
	scfg_pins_source_type pin_source;
	uint32_t line;
	nvic_priority_group_type priority_group;
	IRQn_Type irqn;
};

// general programable input/output Structure
struct gpio_config_t
{
	gpio_type *port;
	uint16_t pin;
	gpio_pull_type pull;
	crm_periph_clock_type clock;
	gpio_mode_type mode;
};

struct gpio_mux_t
{
	struct gpio_config_t gpio;
	gpio_pins_source_type source;
	gpio_mux_sel_type mux;
};

/* hal_intr api
 */
extern const struct interrupt_config_t intr_cset[1];
#define intr_set() &intr_cset[0]
#define irq_line() intr_cset[0].line
#define nvic_irqn() intr_cset[0].irqn
#define nvic_prio() intr_cset[0].priority_group

#define interrupt_config_init dm9051if_intr_config
void interrupt_config_init(const struct interrupt_config_t *config);

/* hal_gpio api
 */
extern struct gpio_config_t cs;
extern struct gpio_config_t intr;
#define gpio_hal_muxpin_config dm9051if_muxpin_config
void gpio_hal_muxpin_config(const struct gpio_mux_t *gpiomux);

#define gpio_hal_stdpin_config dm9051if_stdpin_config //..........
void gpio_hal_stdpin_config(const struct gpio_config_t *gpio); //..........

#define gpio_hal_stdpin_lo dm9051if_stdpin_lo
#define gpio_hal_stdpin_hi dm9051if_stdpin_hi
//#define dm9051if_gpio_lo dm9051if_stdpin_lo
//#define dm9051if_gpio_hi dm9051if_stdpin_lo
//void dm9051if_gpio_lo(void);
//void dm9051if_gpio_hi(void);
void gpio_hal_stdpin_lo(const struct gpio_config_t *gpio);
void gpio_hal_stdpin_hi(const struct gpio_config_t *gpio);

flag_status gpio_hal_stdpin_get(const struct gpio_config_t *gpio);

void config_led3(void);
void led3_on(void);
void led3_off(void);
void config_button(void);
button_type button_is_pressed(void);
void config_diag(void);
void diag_lo(void);
void diag_hi(void);
void config_inpt(void);
flag_status inpt_get(void);

void button_toggle_led3_init(void);
void button_toggle_led3(void);

#define LED3_BY_BUTTON	0
#define LED3_BY_LINK		1
void toggle_led3(int trig_src, button_type usr_button);

/* hal_main api
 */
#define dm9051_hal_init dm9051_boards_initialize
void dm9051_hal_init(void);

#define dm9051_hal_tick dm9051_boards_heartbeat_tick
void dm9051_hal_tick(void);

#define dm9051_hal_tick_count dm_sys_now
uint32_t dm9051_hal_tick_count(void);

#endif //__HAL_API_MCU_H
