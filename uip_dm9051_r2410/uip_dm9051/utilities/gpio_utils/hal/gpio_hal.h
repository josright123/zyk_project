#ifndef __DMGPIO_HAL_MCU_H
#define __DMGPIO_HAL_MCU_H

#define gpio_hal_init dm9051if_gpios_init
void gpio_hal_init(void);
#define gpio_hal_cs_lo dm9051if_cs_lo
#define gpio_hal_cs_hi dm9051if_cs_hi
void gpio_hal_cs_lo(void);
void gpio_hal_cs_hi(void);

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

	Usually, Cn find the expected included files below in main.c
*/
#error "Opps, Opts board to be define  \r\n \
		While NOT include the mcu-definition in the program code, in advance, previously. \r\n \
		Usually, Project find the expected included files below in Options for Target > C/C++ > Preprocessor Definitions > Add > _AT32F4xxxx \r\n \
		ex: _AT32F407xx,USE_STDPERIPH_DRIVER,AT_START_F407_V1 \r\n \
		ex: _AT32F437xx,USE_STDPERIPH_DRIVER,AT_START_F437_V1 \r\n \ "
#endif
#endif //_DLW_AT32F437xx

/* export funcrions
 */
//#define gpio_hal_diag_lo gpio_boards_diag_lo //_dm9051if_cs_lo
//#define gpio_hal_diag_hi gpio_boards_diag_hi //_dm9051if_cs_hi
//#define gpio_hal_inpt_get gpio_boards_inpt_get
/* Hw_implement funcrions
 */
//void gpio_hal_diag_lo(void);
//void gpio_hal_diag_hi(void);
//flag_status gpio_hal_inpt_get(void);
#endif //__DMGPIO_HAL_MCU_H
