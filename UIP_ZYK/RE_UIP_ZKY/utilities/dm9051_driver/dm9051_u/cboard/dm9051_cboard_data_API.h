/**
  **************************************************************************
  * @file     dm9051_cboard.h
  * @version  v1.0
  * @date     2024-08-29
  * @brief    header file of dm9051 board config program.
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */
#ifndef __DM9051_CBOARD_H
#define __DM9051_CBOARD_H

#ifdef __cplusplus
 extern "C" {
#endif

//#include "dm9051opts.h"
//#include "cboard/dm9051_lw_mcu_default_IN.h"
//#include "dm9051_lw.h"

/** @addtogroup dm9051_cboard
  * @{
  */

//struct spi_dev_t;
//extern const struct spi_dev_t devconf[1];

void spi_cs_lo(void);
void spi_cs_hi(void);
	 
uint8_t spi_exc_data(uint8_t byte);
	 
void NU_spi_exc_data_head(uint8_t byte);
uint8_t NU_spi_exc_data_read(void);
void NU_spi_exc_data_read_end(void);
void NU_spi_exc_data_write(uint8_t byte);
void NU_spi_exc_data_write_end(void);

//uint8_t NU_spi_data_read(uint8_t reg);
//uint8_t NU_spi_mem2x_read(void);
uint8_t AT_spi_data_read(uint8_t reg);
void AT_spi_data_write(uint8_t reg, uint8_t val);
uint8_t AT_spi_mem2x_read(void);
void AT_spi_mem_read(uint8_t *buf, uint16_t len);
void AT_spi_mem_write(uint8_t *buf, uint16_t len);

void cint_exint9_5_handler(void);

//void dm9051_boards_initialize(void);
#define dm9051_boards_initialize() \
	spi_add(); \
	intr_add()

void DM9051_Configuration(void);

void cint_disable_mcu_irq(void);
void cint_enable_mcu_irq(void);

void dm_delay_us(uint32_t nus);
void dm_delay_ms(uint16_t nms);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif //__DM9051_CBOARD_H
