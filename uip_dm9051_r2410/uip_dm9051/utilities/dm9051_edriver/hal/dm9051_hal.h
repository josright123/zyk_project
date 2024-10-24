#ifndef __DM9051_HAL_MCU_H
#define __DM9051_HAL_MCU_H

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

  //
  // #include "dm9051_hal_api.h"
  //

#define dm9051_hal_init dm9051_boards_initialize

/* Hw_common funcrions
 */
void dm9051_hal_init(void);
void dm_delay_us(uint32_t nus);
void dm_delay_ms(uint16_t nms);

#if defined(_DLW_AT32F437xx)
#define AT_spi_data_read spi_data_read
#define AT_spi_data_write spi_data_write
#define AT_spi_mem2x_read spi_mem2x_read
#define AT_spi_mem_read  spi_mem_read
#define AT_spi_mem_write  spi_mem_write

  void spi_add(void);
  void intr_add(void);

//#define cint_exint9_5_handler_AT cint_exint9_5_handler
#define cint_disable_mcu_irq_AT  cint_disable_mcu_irq
#define cint_enable_mcu_irq_AT cint_enable_mcu_irq
#define AT_spi_cs_lo dm9051if_cs_lo
#define AT_spi_cs_hi dm9051if_cs_hi
#define	AT_spi_exc_data	dm9051if_exc_data //AT_spi_exc_data
#define dm9051_spi_command_write dm9051if_exc_data //AT_spi_exc_data
#define dm9051_spi_dummy_read() dm9051if_exc_data(0) //AT_spi_exc_data(0)
void cint_disable_mcu_irq_AT(void);
void cint_enable_mcu_irq_AT(void);
int cint_exint9_5_handler_AT(void);
void AT_spi_cs_lo(void);
void AT_spi_cs_hi(void);
uint8_t AT_spi_exc_data(uint8_t byte);
#endif

#if defined(_DLW_M051xx)
  //[impl]
  void DM9051_Configuration_NU(void); // NU
  uint8_t NU_spi_data_read(uint8_t reg);
  void NU_spi_data_write(uint8_t reg, uint8_t val);
  uint8_t NU_spi_mem2x_read(void);
  void NU_spi_mem_read(uint8_t *buf, uint16_t len);
  void NU_spi_mem_write(uint8_t *buf, uint16_t len);
#define DM9051_Configuration_NU	dm9051_boards_initialize
#define spi_data_read(reg,pd) NU_spi_data_read(reg,pd)
#define spi_data_write(reg, val) NU_spi_data_write(reg, val)
#define spi_mem2x_read(pd) NU_spi_mem2x_read(pd)
#define spi_mem_read(buf, len) NU_spi_mem_read(buf, len)
#define spi_mem_write(buf, len) NU_spi_mem_write(buf, len)

  //[hw]
  int NU_cint_exint9_5_handler(void);
  void NU_cint_disable_mcu_irq(void);
  void NU_cint_enable_mcu_irq(void);
  void NU_spi_cs_lo(void);
  void NU_spi_cs_hi(void);
  void NU_spi_exc_data_head(uint8_t byte);
  uint8_t NU_spi_exc_data_read(void);
  void NU_spi_exc_data_read_end(void);
  void NU_spi_exc_data_write(uint8_t byte);
  void NU_spi_exc_data_write_end(void);
//#define cint_exint9_5_handler() NU_cint_exint9_5_handler()
#define cint_disable_mcu_irq() NU_cint_disable_mcu_irq()
#define cint_enable_mcu_irq() NU_cint_enable_mcu_irq()
#define dm9051if_cs_lo() NU_spi_cs_lo()
#define dm9051if_cs_hi() NU_spi_cs_hi()
#endif

//#define spi_datas_read(r, b, l) AT_spi_datas_read(r, b, l)
//#define spi_datas_write(r, b, l) AT_spi_datas_write(r, b, l)

/* dm9051_Hw_common implementation
 * source code.
 */
//#define cspi_read_rxb cspi_read_mem2x
#define cspi_read_mem2x cspi_read_rxb

uint8_t cspi_read_reg(uint8_t reg);
void cspi_write_reg(uint8_t reg, uint8_t val);
void cspi_read_regs(uint8_t reg, uint8_t *buf, uint16_t len, csmode_t csmode);
//void cspi_write_regs(uint8_t reg, uint8_t *buf, uint16_t len, csmode_t csmode);
uint8_t cspi_read_mem2x(void);
void cspi_read_mem(uint8_t *buf, uint16_t len);
void cspi_write_mem(uint8_t *buf, uint16_t len);
  
#endif //__DM9051_HAL_MCU_H
