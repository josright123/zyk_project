#ifndef __DM9051_LW_CBOARD_H__
#define __DM9051_LW_CBOARD_H__

#include "../dm9051_lw_mcu_default.h"
#include "dm9051_lw_conf_types.h"

#ifdef MCU_SPECIFY_CONFIG
#define app_config_board		1
#define driver_config_board		0
#else
#define app_config_board		0
#define driver_config_board		1
#endif

#if 0
//Location: "dm9051_board_impl.c"
//.#define cint_exint9_5_handler	EXINT9_5_IRQHandler

void gpio_pin_config(const gpio_t *gpio, gpio_pull_type gppull);
void spi_config(void);

void DataObj_store(int pin);

int de_enum(uint32_t data);
int de_pin(const gpio_t *gpio);
void list_extline(uint32_t exint_line);

int dm9051_irq_pincode(uint32_t exint_line);
uint32_t dm9051_irq_exint_line(int pin);
#endif

#endif //__DM9051_LW_CBOARD_H__
