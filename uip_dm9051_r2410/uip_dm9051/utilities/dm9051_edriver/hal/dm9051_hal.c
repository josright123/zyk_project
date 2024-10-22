/**
 **************************************************************************
 * The use of structures for SPI and interrupt configurations allows
 *	for easy modification if needed.
 * There are separate functions for SPI initialization (spi_add) and
 *	interrupt initialization (intr_add).
 * The code includes status tracking functions (identify_irq_stat, trace_irq_stat)
 *	for debugging purposes.
 **************************************************************************
 */
#include "../config/conf.h"
#include "../config/conf_core.h"
#include "utils/dm9051_eth_debug.h"

void dm9051_hal_init(void);

#if defined(_DLW_AT32F437xx)
/* ------------------------------- AT32F437 configuration ----------------------------------------- */
struct gpio_mux_t
{
	gpio_type *port;
	uint16_t pin;
	gpio_pins_source_type source;
	gpio_mux_sel_type mux;
	crm_periph_clock_type clock;
	gpio_mode_type mode;
};

// SPI Configuration Structure
struct spi_config_t
{
	spi_type *spi;
	crm_periph_clock_type clock;
	struct gpio_mux_t sck, miso, mosi, cs;
};

// Interrupt Configuration Structure
struct interrupt_config_t
{
	struct gpio_mux_t gpio;
	struct
	{
		scfg_port_source_type port_source;
		scfg_pins_source_type pin_source;
		crm_periph_clock_type clock;
	} scfg;
	struct
	{
		uint32_t line;
		crm_periph_clock_type clock;
	} exti;
	//struct
	//{
	//} nvic;
	IRQn_Type irqn;
	nvic_priority_group_type priority_group;
};

#define cs_gpio_port() spi_cset[0].cs.port // #define pin_cs() spi_cset[0].cs
#define cs_gpio_pin() spi_cset[0].cs.pin
#define spi_number() spi_cset[0].spi

static const struct spi_config_t spi_cset[1] = {
	{
		SPI1,
		CRM_SPI1_PERIPH_CLOCK,
		{GPIOA, GPIO_PINS_5, GPIO_PINS_SOURCE5, GPIO_MUX_5, CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_MUX},
		{GPIOA, GPIO_PINS_6, GPIO_PINS_SOURCE6, GPIO_MUX_5, CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_MUX},
		{GPIOA, GPIO_PINS_7, GPIO_PINS_SOURCE7, GPIO_MUX_5, CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_MUX},
		{GPIOA, GPIO_PINS_15, (gpio_pins_source_type)0, (gpio_mux_sel_type)0, CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_OUTPUT},
	}};

static /*const*/ struct interrupt_config_t spi_cintr[1] = {
	{
		{GPIOC, GPIO_PINS_7, (gpio_pins_source_type)0, (gpio_mux_sel_type)0, CRM_GPIOC_PERIPH_CLOCK, GPIO_MODE_INPUT},
		{SCFG_PORT_SOURCE_GPIOC, SCFG_PINS_SOURCE7, CRM_SCFG_PERIPH_CLOCK},
		{EXINT_LINE_7, CRM_GPIOC_PERIPH_CLOCK},
		EXINT9_5_IRQn,
		NVIC_PRIORITY_GROUP_0,
	}};

static void spi_config_init(const struct spi_config_t *config);
static void interrupt_config_init(const struct interrupt_config_t *config);

void dm9051_hal_init(void) //for AT32F437
{
	//to, spi_cintr[0].exti = 
	spi_cintr[0].gpio.port = GPIOC;
	spi_cintr[0].gpio.pin = GPIO_PINS_7;
	spi_cintr[0].gpio.clock = CRM_GPIOC_PERIPH_CLOCK;
	spi_cintr[0].exti.line = EXINT_LINE_7;
	spi_cintr[0].exti.clock = CRM_GPIOC_PERIPH_CLOCK;
	spi_cintr[0].scfg.clock = CRM_SCFG_PERIPH_CLOCK;
	spi_cintr[0].irqn = EXINT9_5_IRQn;
	spi_cintr[0].priority_group = NVIC_PRIORITY_GROUP_0;
	//to, spi_cintr[0].gpio = {GPIOC, GPIO_PINS_7, (gpio_pins_source_type)0, (gpio_mux_sel_type)0, CRM_GPIOC_PERIPH_CLOCK, GPIO_MODE_INPUT};
	
	/* spi_add();
	 */
	spi_config_init(spi_cset);

	/* intr_add();
	 */
	#ifdef DM9051_DRIVER_INTERRUPT
	interrupt_config_init(spi_cintr);
	#endif
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

static void configure_cpin(const struct gpio_mux_t *ps, gpio_pull_type gppull)
{
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

// #if 1
//	=configure_xirq_pin(&spi_intr[0].gpio, GPIO_PULL_UP);
//	=configure_xirq_body(&spi_intr[0], EXINT_TRIGGER_FALLING_EDGE);
static void configure_cirq(const struct interrupt_config_t *cf, gpio_pull_type gppull,
						   exint_polarity_config_type polarity)
{
	exint_init_type exint_init_struct;

	/* enable the gpio clock
	 */

	// [Not all devices essential]
	configure_cpin(&cf->gpio, gppull);

	// [Not all devices essential]
	// log_intr_qpio_pin_config();

	/* config irq
	 */

	crm_periph_clock_enable(cf->scfg.clock, TRUE);
	crm_periph_clock_enable(cf->exti.clock, TRUE);

	scfg_exint_line_config(cf->scfg.port_source,
						   cf->scfg.pin_source);

	exint_default_para_init(&exint_init_struct);
	exint_init_struct.line_enable = TRUE;
	exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
	exint_init_struct.line_select = cf->exti.line;
	exint_init_struct.line_polarity = polarity;
	exint_init(&exint_init_struct);

	identify_irq_stat(ISTAT_IRQ_CFG);
	trace_irq_stat(ISTAT_IRQ_CFG);

	identify_irq_stat(ISTAT_LOW_TRIGGER);
	trace_irq_stat(ISTAT_LOW_TRIGGER);
}
// #endif

// Static function prototypes
static void spi_config_init(const struct spi_config_t *config)
{
	configure_cspi(config);
	configure_cpin(&config->sck, GPIO_PULL_NONE);
	configure_cpin(&config->miso, GPIO_PULL_NONE);
	configure_cpin(&config->mosi, GPIO_PULL_NONE);
	configure_cpin(&config->cs, GPIO_PULL_NONE);
}

// Static function prototypes
static void interrupt_config_init(const struct interrupt_config_t *config)
{
	configure_cirq(config, GPIO_PULL_UP, EXINT_TRIGGER_FALLING_EDGE);
}

/* IRQ handler support */
//int cint_exint9_5_handler_AT(void)
//{
//	// add user's mcu irq-handler source code here.
//	if (exint_flag_get(spi_cintr[0].exti.line) != RESET)
//	{
//		exint_flag_clear(spi_cintr[0].exti.line);
//		return 1;
//		// DM_ETH_SetInterruptEvent();
//	} else {
//		identify_irq_stat(ISTAT_IRQ_NOT_NOW);
//		trace_irq_stat(ISTAT_IRQ_NOT_NOW);
//	}
//	return 0;
//}

// ---------------------- hw_impl -------------------------------------------------------------
//[hw]
//void cint_disable_mcu_irq_AT(void);
//void cint_enable_mcu_irq_AT(void);
//int cint_exint9_5_handler_AT(void);
//void AT_spi_cs_lo(void);
//void AT_spi_cs_hi(void);
//uint8_t AT_spi_exc_data(uint8_t byte);

// ... AT32F437xx specific interrupt enable/disable functions ...

void cint_disable_mcu_irq_AT(void)
{
	deidentify_irq_stat(ISTAT_IRQ_ENAB);
	nvic_irq_disable(spi_cintr[0].irqn);
}

void cint_enable_mcu_irq_AT(void)
{
	identify_irq_stat(ISTAT_IRQ_ENAB);
	trace_irq_stat(ISTAT_IRQ_ENAB);

	nvic_priority_group_config(spi_cintr[0].priority_group);
	nvic_irq_enable(spi_cintr[0].irqn, 1, 0);
}

void AT_spi_cs_lo(void)
{
	gpio_bits_reset(cs_gpio_port(), cs_gpio_pin());
}
void AT_spi_cs_hi(void)
{
	gpio_bits_set(cs_gpio_port(), cs_gpio_pin());
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
//[impl]
//[AT cspi_io]
#define DM9051_MRCMDX (0x70) // Read_Mem2X
#define DM9051_MRCMD (0x72)	 // Read_Mem
#define DM9051_MWCMD (0x78)	 // Write_Mem

uint8_t AT_spi_data_read(uint8_t reg);
void AT_spi_data_write(uint8_t reg, uint8_t val);
uint8_t AT_spi_mem2x_read(void);
void AT_spi_mem_read(uint8_t *buf, uint16_t len);
void AT_spi_mem_write(uint8_t *buf, uint16_t len);

uint8_t AT_spi_data_read(uint8_t reg)
{
	dm9051_spi_command_write(reg | OPC_REG_R);
	return dm9051_spi_dummy_read();
}
void AT_spi_data_write(uint8_t reg, uint8_t val)
{
	dm9051_spi_command_write(reg | OPC_REG_W);
	dm9051_spi_command_write(val);
}

uint8_t AT_spi_mem2x_read(void)
{
	dm9051_spi_command_write(DM9051_MRCMDX | OPC_REG_R);
	dm9051_spi_dummy_read();
	return dm9051_spi_dummy_read();
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

#if defined(_DLW_AT32F437xx)
void AT_spi_datas_read(uint8_t reg, uint8_t *buf, uint16_t len)
{
	int i;
	for (i = 0; i < len; i++, reg++)
	{
		dm9051_spi_command_write(reg | OPC_REG_R);
		buf[i] = dm9051_spi_dummy_read();
	}
}
void AT_spi_datas_write(uint8_t reg, uint8_t *buf, uint16_t len)
{
	int i;
	for (i = 0; i < len; i++, reg++)
	{
		dm9051_spi_command_write(reg | OPC_REG_W);
		dm9051_spi_command_write(buf[i]);
	}
}
#endif

/* dm9051_Hw_common implementation
 * source code.
 */
uint8_t cspi_read_reg(uint8_t reg) // static (todo)
{
	uint8_t val;
	dm9051if_cs_lo();
	val = spi_data_read(reg);
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
		spi_datas_read(reg, buf, len);
		dm9051if_cs_hi();
	}
	else
	{ // CS_EACH
		for (i = 0; i < len; i++, reg++)
			buf[i] = cspi_read_reg(reg);
	}
}

void cspi_write_regs(uint8_t reg, uint8_t *buf, uint16_t len, csmode_t csmode)
{
	int i;

	if (csmode == CS_LONG)
	{
		dm9051if_cs_lo();
		spi_datas_write(reg, buf, len);
		dm9051if_cs_hi();
	}
	else
	{ // CS_EACH
		for (i = 0; i < len; i++, reg++)
			cspi_write_reg(reg + i, buf[i]);
	}
}

uint8_t cspi_read_mem2x(void)
{
	uint8_t rxb;
	dm9051if_cs_lo();
	rxb = spi_mem2x_read();
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

/* dm9051_Hw_common delay funcrions
 * Select ether [0] or [1], or otherwise coding user's sys_now().
 * Need a tick counter in the system
 *  - mcu ticks
 *  - freertos
 *  - lwip
 *  - uip
 */
#if 0
//(0)[in case mcu ticks]
#include "main.h"
#define	sys_now	main_tick_count
#elif freeRTOS
//(1)in case freertos
// #include "lwip/sys.h"
#define sys_now xTaskGetTickCount
#else
//(2)[in case of lwip]
//(3)[in case of uip]
#define sys_now sys_now	   // to check lwip
#include "clock.h"
#define sys_now clock_time // or check uip
#endif

void dm_delay_us(uint32_t nus)
{
	uint32_t start = sys_now();
	while ((sys_now() - start) < ((nus + 999) / 1000))
		;
}

void dm_delay_ms(uint16_t nms)
{
	uint32_t start = sys_now();
	while ((sys_now() - start) < nms)
		;
}
