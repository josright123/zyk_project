/**
 **************************************************************************
 * @file     hal_maim.c (dm9051_hal.c)
 * The use of structures for SPI and interrupt configurations allows
 *	for easy modification if needed.
 * There are separate functions for SPI initialization (spi_add) and
 *	interrupt initialization (intr_add).
 * The code includes status tracking functions (identify_irq_stat, trace_irq_stat)
 *	for debugging purposes.
 **************************************************************************
 */
#include "control/drv/conf_core.h"
#include "control/drv/dm9051_eth_debug.h"

#define cint_enable_mcu_irq_AT cint_enable_mcu_irq


#define	AT_spi_exc_data	dm9051if_exc_data //AT_spi_exc_data
uint8_t AT_spi_exc_data(uint8_t byte);

#define dm9051_spi_command_write dm9051if_exc_data //AT_spi_exc_data
#define dm9051_spi_dummy_read() dm9051if_exc_data(0) //AT_spi_exc_data(0)

#define AT_spi_data_read spi_data_read
#define AT_spi_data_write spi_data_write
#define AT_spi_mem2x_read spi_mem2x_read
#define AT_spi_mem_read  spi_mem_read
#define AT_spi_mem_write  spi_mem_write

#define	dm9051_hal_irqline HAL_IRQLine

void dm9051_hal_init(void);
void dm9051_hal_tick(void);

#if defined(_DLW_AT32F437xx)

// SPI Configuration Structure
struct spi_config_t
{
	spi_type *spi;
	crm_periph_clock_type clock;
	struct gpio_mux_t sck, miso, mosi; 
	/* cs */
};

// Interrupt Configuration Structure
struct interrupt_config_t
{
	/*struct gpio_mux_t gpio; */
	crm_periph_clock_type scfgclock;
	crm_periph_clock_type clock;
	scfg_port_source_type port_source;
	scfg_pins_source_type pin_source;
	uint32_t line;
	nvic_priority_group_type priority_group;
	IRQn_Type irqn;
};

static const struct spi_config_t spi_cset[1] = {
	{
		SPI1,
		CRM_SPI1_PERIPH_CLOCK,
		{{GPIOA, GPIO_PINS_5, GPIO_PULL_NONE, CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_MUX}, GPIO_PINS_SOURCE5, GPIO_MUX_5},
		{{GPIOA, GPIO_PINS_6, GPIO_PULL_NONE, CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_MUX}, GPIO_PINS_SOURCE6, GPIO_MUX_5},
		{{GPIOA, GPIO_PINS_7, GPIO_PULL_NONE, CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_MUX}, GPIO_PINS_SOURCE7, GPIO_MUX_5},
	}};

static const struct interrupt_config_t intr_cset[1] = {
	{
		CRM_SCFG_PERIPH_CLOCK,
		CRM_GPIOC_PERIPH_CLOCK,
		SCFG_PORT_SOURCE_GPIOC,
		SCFG_PINS_SOURCE7,
		EXINT_LINE_7,
		NVIC_PRIORITY_GROUP_0,
		EXINT9_5_IRQn,
	}};

// GPIO Data
struct gpio_config_t
cs = {
		GPIOA, GPIO_PINS_15, GPIO_PULL_NONE, CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_OUTPUT,
},
intr = {
		GPIOC, GPIO_PINS_7, GPIO_PULL_UP, CRM_GPIOC_PERIPH_CLOCK, GPIO_MODE_INPUT,
};

#define dm9051if_cs_lo() dm9051if_gpio_lo(&cs)
#define dm9051if_cs_hi() dm9051if_gpio_hi(&cs) //gpio_hal_stdpin_hi(&cs)

#define spi_set() &spi_cset[0]
#define spi_number() spi_cset[0].spi
	
#define intr_set() &intr_cset[0]
#define irq_line() intr_cset[0].line
#define nvic_irqn() intr_cset[0].irqn
#define nvic_prio() intr_cset[0].priority_group

static void spi_config_init(const struct spi_config_t *config);
static void interrupt_config_init(const struct interrupt_config_t *config);

void dm9051_hal_init(void)
{	
	/* purpose: spi_add, and intr_add
	 */
	/* purpose: seperating cs_gpio_add, and intr_gpio_add
	 */
	spi_config_init(spi_set());
	dm9051if_gpio_config(&cs);
	
	interrupt_config_init(intr_set());
	dm9051if_gpio_config(&intr);
}

uint32_t dm9051_hal_irqline(void)
{
	return irq_line();
}

static void configure_cspi(const struct spi_config_t *ss)
{
	spi_init_type spi_init_struct;

	crm_periph_clock_enable(ss->clock, TRUE);
	spi_default_para_init(&spi_init_struct);
	spi_init_struct.transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;
	spi_init_struct.master_slave_mode = SPI_MODE_MASTER;
	spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_8;
	// spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_LSB;
	spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_MSB;
	spi_init_struct.frame_bit_num = SPI_FRAME_8BIT;
	spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_LOW;
	// spi_init_struct.clock_phase = SPI_CLOCK_PHASE_2EDGE;
	spi_init_struct.clock_phase = SPI_CLOCK_PHASE_1EDGE;
	spi_init_struct.cs_mode_selection = SPI_CS_SOFTWARE_MODE;
	spi_init(ss->spi, &spi_init_struct);
	spi_enable(ss->spi, TRUE);
}

static void configure_cirq(const struct interrupt_config_t *cf,
						   exint_polarity_config_type polarity)
{
	exint_init_type exint_init_struct;

	/* config irq
	 */
	crm_periph_clock_enable(cf->scfgclock, TRUE);
	crm_periph_clock_enable(cf->clock, TRUE);

	scfg_exint_line_config(cf->port_source,
						   cf->pin_source);

	exint_default_para_init(&exint_init_struct);
	exint_init_struct.line_enable = TRUE;
	exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
	exint_init_struct.line_select = cf->line;
	exint_init_struct.line_polarity = polarity;
	exint_init(&exint_init_struct);

	identify_irq_stat(ISTAT_IRQ_CFG);
	trace_irq_stat(ISTAT_IRQ_CFG);

	identify_irq_stat(ISTAT_LOW_TRIGGER);
	trace_irq_stat(ISTAT_LOW_TRIGGER);
}

// Static function prototypes
//#define esck() (config->sck)
//#define emiso() (config->miso)
//#define emosi() (config->mosi)
static void spi_config_init(const struct spi_config_t *config)
{
	configure_cspi(config);
	dm9051if_spi_pin_config(&config->sck);
	dm9051if_spi_pin_config(&config->miso);
	dm9051if_spi_pin_config(&config->mosi);
}

// Static function prototypes
static void interrupt_config_init(const struct interrupt_config_t *config)
{
	#ifdef DM9051_DRIVER_INTERRUPT
	configure_cirq(config, EXINT_TRIGGER_FALLING_EDGE);
	#endif
}

// ---------------------- hw_impl -------------------------------------------------------------
//[hw]
void cint_disable_mcu_irq_AT(void)
{
	deidentify_irq_stat(ISTAT_IRQ_ENAB);
	nvic_irq_disable(nvic_irqn());
}

void cint_enable_mcu_irq_AT(void)
{
	identify_irq_stat(ISTAT_IRQ_ENAB);
	trace_irq_stat(ISTAT_IRQ_ENAB);

	nvic_priority_group_config(nvic_prio());
	nvic_irq_enable(nvic_irqn(), 1, 0);
}

uint8_t AT_spi_exc_data(uint8_t byte)
{
	while (spi_i2s_flag_get(spi_number(), SPI_I2S_TDBE_FLAG) == RESET)
		;
	spi_i2s_data_transmit(spi_number(), byte);
	while (spi_i2s_flag_get(spi_number(), SPI_I2S_RDBF_FLAG) == RESET)
		;
	return (uint8_t)spi_i2s_data_receive(spi_number());
}

// ---------------------- data_impl -------------------------------------------------------------
//[io]
#define DM9051_MRCMDX (0x70) // Read_Mem2X
#define DM9051_MRCMD (0x72)	 // Read_Mem
#define DM9051_MWCMD (0x78)	 // Write_Mem

void AT_spi_data_read(uint8_t reg, uint8_t *pd);
void AT_spi_data_write(uint8_t reg, uint8_t val);
void AT_spi_mem2x_read(uint8_t *pd);
void AT_spi_mem_read(uint8_t *buf, uint16_t len);
void AT_spi_mem_write(uint8_t *buf, uint16_t len);

void AT_spi_data_read(uint8_t reg, uint8_t *pd)
{
	dm9051_spi_command_write(reg | OPC_REG_R);
	*pd = dm9051_spi_dummy_read();
}
void AT_spi_data_write(uint8_t reg, uint8_t val)
{
	dm9051_spi_command_write(reg | OPC_REG_W);
	dm9051_spi_command_write(val);
}

void AT_spi_mem2x_read(uint8_t *pd)
{
	dm9051_spi_command_write(DM9051_MRCMDX | OPC_REG_R);
	dm9051_spi_dummy_read();
	*pd = dm9051_spi_dummy_read();
}
void AT_spi_mem_read(uint8_t *buf, uint16_t len)
{
	int i;
	dm9051_spi_command_write(DM9051_MRCMD | OPC_REG_R);
	for (i = 0; i < len; i++)
		buf[i] = dm9051_spi_dummy_read();
}
void AT_spi_mem_write(uint8_t *buf, uint16_t len)
{
	int i;
	dm9051_spi_command_write(DM9051_MWCMD | OPC_REG_W);
	for (i = 0; i < len; i++)
		dm9051_spi_command_write(buf[i]);
}
#endif //_DLW_AT32F437xx

/* dm9051_Hw_common implementation
 * source code.
 */
uint8_t cspi_read_reg(uint8_t reg) // static (todo)
{
	uint8_t val;
	dm9051if_cs_lo();
	spi_data_read(reg, &val);
	dm9051if_cs_hi();
	return val;
}

void cspi_write_reg(uint8_t reg, uint8_t val)
{
	dm9051if_cs_lo();
	spi_data_write(reg, val);
	dm9051if_cs_hi();
}

void cspi_read_regs(uint8_t reg, uint8_t *buf, uint16_t len, csmode_t csmode)
{
	int i;
	if (csmode == CS_LONG)
	{
		dm9051if_cs_lo();
		for (i = 0; i < len; i++, reg++)
			spi_data_read(reg, &buf[i]);
		dm9051if_cs_hi();
	}
	else
	{ // CS_EACH
		for (i = 0; i < len; i++, reg++)
			buf[i] = cspi_read_reg(reg);
	}
}

//	if (csmode == CS_LONG)
//	{
//		dm9051if_cs_lo();
//		for (i = 0; i < len; i++, reg++)
//			spi_data_write(reg, buf[i]);
//		dm9051if_cs_hi();
//	}
//	else
//	{ // CS_EACH
//	}
void cspi_write_regs(uint8_t reg, const uint8_t *buf, uint16_t len)
{
	int i;
	for (i = 0; i < len; i++, reg++)
		cspi_write_reg(reg, buf[i]);
}

uint8_t cspi_read_rxb(void)
{
	uint8_t rxb;
	dm9051if_cs_lo();
	spi_mem2x_read(&rxb);
	dm9051if_cs_hi();
	return rxb;
}
void cspi_read_mem(uint8_t *buf, uint16_t len)
{
	dm9051if_cs_lo();
	spi_mem_read(buf, len);
	dm9051if_cs_hi();
}
void cspi_write_mem(uint8_t *buf, uint16_t len)
{
	dm9051if_cs_lo();
	spi_mem_write(buf, len);
	dm9051if_cs_hi();
}

uint32_t dm9051HalTicks = 0;

uint32_t dm9051_hal_tick_count(void)
{
	return dm9051HalTicks;
}

void dm9051_hal_tick(void)
{
	dm9051HalTicks++;
}

void dm_delay_us(uint32_t nus)
{
	uint32_t start = dm_sys_now();
	while ((dm_sys_now() - start) < ((nus + 999) / 1000))
		;
}

void dm_delay_ms(uint16_t nms)
{
	uint32_t start = dm_sys_now();
	while ((dm_sys_now() - start) < nms)
		;
}
