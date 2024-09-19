#include <stdint.h>
#include "dm9051opts.h"

//dm9051_if.c
void board_conf_configuration(void)
{
}

//dm9051_lw_cspi.c
void cspi_read_regs(uint8_t reg, uint8_t *buf, uint16_t len, csmode_t csmode)
{
}

uint8_t cspi_read_reg(uint8_t reg) //static (todo)
{
	return 0;
}
void cspi_write_regs(uint8_t reg, u8 *buf, u16 len, csmode_t csmode)
{
}
void cspi_write_reg(uint8_t reg, uint8_t val)
{
}
uint8_t cspi_read_mem2x(void)
{
	return 0;
}
void cspi_read_mem(u8 *buf, u16 len)
{
}
void cspi_write_mem(u8 *buf, u16 len)
{
}

//dm9051_if.c
void dm_delay_us(uint32_t nus)
{
}
void dm_delay_ms(uint16_t nms)
{
}

//dm9051_if.c
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_init_setup (referred from .\objects\dm9051.o).
int dm9051_init_setup(uint16_t *id)
{
	return 0;
}
u16 ev_rxb(uint8_t rxb)
{
	return 0;
}
u16 ev_status(uint8_t rx_status)
{
	return 0;
}

//dm9051_if.c
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol hdlr_reset_process (referred from .\objects\dm9051.o).
const uint8_t *hdlr_reset_process(const uint8_t *macaddr, enable_t en)
{
	return 0;
}

//dm9051_if.c
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol display_identity (referred from .\objects\dm9051.o).
int display_identity(char *spiname, uint16_t id, uint8_t *ids, uint8_t id_adv, uint16_t idin, char *tail)
{
	return 0;
}

//dm9051_if.c
//D:\basic_2Git\dm9051_FreeRTOS\direct2yicheng55\freertos\SourceCode\zyk_f437_freertos\utilities\dm9051_driver\dm9051_lw\dm9051_lw_conf_types.h(117) : } spi_dev_t;
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_irq_exint_line (referred from .\objects\at32f435_437_int.o).
#include "dm9051_lw_cspi.h"
typedef struct {
	const spi_dev_t *devconf[BOARD_SPI_COUNT];
	const struct modscfg_st *intrconf[BOARD_SPI_COUNT];
} conf_list_t;
static conf_list_t DataObj;
uint32_t dm9051_irq_exint_line(int pin)
{
	return DataObj.intrconf[pin] ? DataObj.intrconf[pin]->extend->extline.extline : 0;
}

//dm9051_lw_cspi.c
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol gpio_pin_config (referred from .\objects\dm9051_lw_cint.o).
void gpio_pin_config(const gpio_t *gpio, gpio_pull_type gppull)
{
}

//dm9051_if.c
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_phycore_on (referred from .\objects\dm9051_bl_impl.o).
void dm9051_phycore_on(uint16_t nms)
{
}

//dm9051_lw_cspi.c
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol de_pin (referred from .\objects\dm9051_lw_cint.o).
int de_pin(const gpio_t *gpio)
{
	return 0;
}

//dm9051_lw_cspi_data.h
//dm9051_lw_cspi.c
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol mode_input (referred from .\objects\dm9051_lw_cint.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol mode_output (referred from .\objects\dm9051_lw_crst.o).
#define INP_DATA()	\
	GPIO_MODE_INPUT,
#define OUTP_DATA()	\
	GPIO_MODE_OUTPUT,
const gpio_mux_t mode_input = { INP_DATA()};
const gpio_mux_t mode_output = { OUTP_DATA()};

//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol devconf (referred from .\objects\dm9051_lw_conf.o).
//{.}

//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol mstep_conf_cpu_cs_ethernet (referred from .\objects\dm9051_lw_conf.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol mstep_conf_cpu_spi_ethernet (referred from .\objects\dm9051_lw_conf.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol mstep_conf_info (referred from .\objects\dm9051_lw_conf.o).

//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol mstep_spi_conf_name (referred from .\objects\dm9051.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol devconf (referred from .\objects\dm9051_lw_cint.o).

//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol mstep_spi_conf_name (referred from .\objects\dm9051_lw_clog.o).

//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol devconf (referred from .\objects\dm9051_lw_cint.o).

//dm9051_lw_cint_data.h
//dm9051_lw_cint.c
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol intr_gpio_mptr (referred from .\objects\netconf.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_eth_irq_disab (referred from .\objects\dm_eth.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_eth_irq_enab (referred from .\objects\dm_eth.o).
int intr_gpio_mptr(void)
{
	return 1;
}
void dm9051_eth_irq_disab(IRQn_Type irqn)
{
}
void dm9051_eth_irq_enab(IRQn_Type irqn, nvic_priority_group_type priority)
{
}

#if 0
//dm9051_lw_cint_data.h
//dm9051_lw_cint.c
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol devconf_at437_intr_a0 (referred from .\objects\dm9051_lw_cspi.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol devconf_at437_intr_c7 (referred from .\objects\dm9051_lw_cspi.o).
#endif

#if 0
linking...

.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cspi_read_reg (referred from .\objects\dm9051.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cspi_write_reg (referred from .\objects\dm9051.o).

.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cpin_poweron_reset (referred from .\objects\dm9051_bl_impl_if.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cspi_read_regs (referred from .\objects\dm9051_bl_impl_if.o).

.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cspi_read_mem (referred from .\objects\dm9051_bl_impl.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cspi_read_mem2x (referred from .\objects\dm9051_bl_impl.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cspi_write_mem (referred from .\objects\dm9051_bl_impl.o).

.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol devconf (referred from .\objects\dm9051_board_impl.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cpin_gpio_add (referred from .\objects\dm9051_board_impl.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cpin_rst_add (referred from .\objects\dm9051_board_impl.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol intr_add (referred from .\objects\dm9051_board_impl.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol spi_add (referred from .\objects\dm9051_board_impl.o).

/* dm9051_board_impl.c
 */
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol intr_gpio_mptr (referred from .\objects\netconf.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_eth_irq_disab (referred from .\objects\dm_eth.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_eth_irq_enab (referred from .\objects\dm_eth.o).

//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol de_enum (referred from .\objects\dm9051_board_impl.o).

.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_extline_irq_enable (referred from .\objects\dm9051_bl_impl.o)..
#endif

linking...
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cspi_read_reg (referred from .\objects\dm9051.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cspi_write_reg (referred from .\objects\dm9051.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol devconf (referred from .\objects\dm9051_board_impl.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cpin_gpio_add (referred from .\objects\dm9051_board_impl.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cpin_rst_add (referred from .\objects\dm9051_board_impl.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol intr_add (referred from .\objects\dm9051_board_impl.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol spi_add (referred from .\objects\dm9051_board_impl.o).

.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cpin_poweron_reset (referred from .\objects\dm9051_bl_impl_if.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cspi_read_regs (referred from .\objects\dm9051_bl_impl_if.o).

.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cspi_read_mem (referred from .\objects\dm9051_bl_impl.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cspi_read_mem2x (referred from .\objects\dm9051_bl_impl.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cspi_write_mem (referred from .\objects\dm9051_bl_impl.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_extline_irq_enable (referred from .\objects\dm9051_bl_impl.o).

linking...
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cspi_read_reg (referred from .\objects\dm9051.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cspi_write_reg (referred from .\objects\dm9051.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cpin_gpio_add (referred from .\objects\dm9051_board_impl.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cpin_rst_add (referred from .\objects\dm9051_board_impl.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol intr_add (referred from .\objects\dm9051_board_impl.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol spi_add (referred from .\objects\dm9051_board_impl.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cpin_poweron_reset (referred from .\objects\dm9051_bl_impl_if.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cspi_read_regs (referred from .\objects\dm9051_bl_impl_if.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cspi_read_mem (referred from .\objects\dm9051_bl_impl.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cspi_read_mem2x (referred from .\objects\dm9051_bl_impl.o).
.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cspi_write_mem (referred from .\objects\dm9051_bl_impl.o).
