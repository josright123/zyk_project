#include "dm9051_cboard_data_API.h"
#include "dm9051_cstate.h"
#include "../dm_eth.h"

#if defined(_DLW_AT32F437xx)
/* ------------------------------- AT configuration ----------------------------------------- */
// --------------------- AT ----------------------------
void dm9051_boards_initialize_AT(void)
{
	spi_add();
#ifdef DM9051_DRIVER_INTERRUPT
	intr_add();
#endif
}

struct spi_pin_t
{
	gpio_type *gpport; //= PORT;
	uint16_t pin;	   //= PIN;
	gpio_pins_source_type pinsrc;
	gpio_mux_sel_type muxsel;
};
struct spi_pin_set_t
{
	crm_periph_clock_type clock;
	gpio_mode_type gpio_mode;
	struct spi_pin_t gpio_pin;
};

struct spi_set_t
{
	// char *spi_desc;
	spi_type *spi;				 //= SPIPORT;
	crm_periph_clock_type clock; //= SPI_CRM_CLK;
	struct spi_pin_set_t pin_sck;
	struct spi_pin_set_t pin_miso;
	struct spi_pin_set_t pin_mosi;
	struct spi_pin_set_t pin_cs; /* //CS (e.g. PA15) */
};

static struct spi_set_t spi_set[1] = {
	{SPI1,
	CRM_SPI1_PERIPH_CLOCK,
	{CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_MUX, {GPIOA, GPIO_PINS_5, GPIO_PINS_SOURCE5, GPIO_MUX_5}},
	{CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_MUX, {GPIOA, GPIO_PINS_6, GPIO_PINS_SOURCE6, GPIO_MUX_5}},
	{CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_MUX, {GPIOA, GPIO_PINS_7, GPIO_PINS_SOURCE7, GPIO_MUX_5}},
	{CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_OUTPUT, {GPIOA, GPIO_PINS_15}}}
};

static void configure_gpio(struct spi_pin_set_t *ps, gpio_pull_type gppull)
{
	gpio_init_type gpio_init_struct;

	gpio_default_para_init(&gpio_init_struct);
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_mode = ps->gpio_mode;
	gpio_init_struct.gpio_pull = gppull; // GPIO_PULL_DOWN; GPIO_PULL_UP; //GPIO_PULL_NONE;
	gpio_init_struct.gpio_pins = ps->gpio_pin.pin;
	gpio_init(ps->gpio_pin.gpport, &gpio_init_struct);

#if defined(_DLW_AT32F437xx)
	if (ps->gpio_mode == GPIO_MODE_MUX)
		gpio_pin_mux_config(ps->gpio_pin.gpport, ps->gpio_pin.pinsrc, ps->gpio_pin.muxsel);
#endif
}

static void configure_spi(struct spi_set_t *ss)
{
	spi_init_type spi_init_struct;

	crm_periph_clock_enable(
		ss->clock,
		TRUE);
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
	spi_init(
		ss->spi,
		&spi_init_struct);
	spi_enable(
		ss->spi,
		TRUE);
}

static void configure_pin(struct spi_pin_set_t *ps, gpio_pull_type gppull)
{
	/* enable the gpio clock
	 */
	crm_periph_clock_enable(ps->clock, TRUE);
	configure_gpio(ps, gppull);
}

void spi_add(void)
{
	configure_spi(&spi_set[0]);
	configure_pin(&spi_set[0].pin_sck, GPIO_PULL_NONE);
	configure_pin(&spi_set[0].pin_miso, GPIO_PULL_NONE);
	configure_pin(&spi_set[0].pin_mosi, GPIO_PULL_NONE);
	configure_pin(&spi_set[0].pin_cs, GPIO_PULL_NONE);
}

struct irq_set_t
{
	crm_periph_clock_type clock; // CRM_SCFG_PERIPH_CLOCK,
	scfg_port_source_type scfg_port_src; // SCFG_PORT_SOURCE_X
	scfg_pins_source_type scfg_pin_src; // SCFG_PINS_SOURCEX
};

struct irqline_set_t
{
	crm_periph_clock_type clock; // CRM_GPIOC_PERIPH_CLOCK,
	uint32_t extline; //= LINE
};

struct interrupt_mode_t
{
	IRQn_Type irqn;					   //= EXINTn_m
	nvic_priority_group_type priority; //_group;
};

struct interrupt_set_t
{
	struct spi_pin_set_t gpio; /* //INTR (e.g. PC7) */
	struct irq_set_t irq;
	struct irqline_set_t irqline;
	struct interrupt_mode_t interrupt;
};

struct interrupt_set_t spi_intr[1] = {
	{
	{CRM_GPIOC_PERIPH_CLOCK, GPIO_MODE_INPUT, {GPIOC, GPIO_PINS_7}},
	{CRM_SCFG_PERIPH_CLOCK, SCFG_PORT_SOURCE_GPIOC, SCFG_PINS_SOURCE7},
	{CRM_GPIOC_PERIPH_CLOCK, EXINT_LINE_7},
	{EXINT9_5_IRQn, NVIC_PRIORITY_GROUP_0},
	},
};

static void configure_irq(struct interrupt_set_t *spiintr,
						  exint_polarity_config_type polarity)
{ // const struct extscfg_st *pexint_set,
	exint_init_type exint_init_struct;

	crm_periph_clock_enable(spiintr->irq.clock, TRUE);
	crm_periph_clock_enable(spiintr->irqline.clock, TRUE);

	// #ifndef AT32F437xx
	//   gpio_exint_line_config(scfg_port(), scfg_pin()); // SCFG_PORT_SOURCE_GPIOA, SCFG_PINS_SOURCE0
	// #else
	// #endif
	scfg_exint_line_config(spiintr->irq.scfg_port_src,
						   spiintr->irq.scfg_pin_src);

	exint_default_para_init(&exint_init_struct);
	exint_init_struct.line_enable = TRUE;
	exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
	exint_init_struct.line_select = spiintr->irqline.extline;
	exint_init_struct.line_polarity = polarity; // EXINT_TRIGGER_RISING_EDGE/ EXINT_TRIGGER_FALLING_EDGE
	exint_init(&exint_init_struct);
}

#ifdef DM9051_DRIVER_INTERRUPT

void intr_add(void)
{
	// log_intr_qpio_pin_config();
	configure_pin(&spi_intr[0].gpio, GPIO_PULL_UP);
	configure_irq(&spi_intr[0], EXINT_TRIGGER_FALLING_EDGE); //[ops] //&devconf_at437_intr_c7.extend1,

	identify_irq_stat(ISTAT_IRQ_CFG);
	trace_irq_stat(ISTAT_IRQ_CFG);
	identify_irq_stat(ISTAT_LOW_TRIGGER);
	trace_irq_stat(ISTAT_LOW_TRIGGER);
}
#endif // DM9051_DRIVER_INTERRUPT
#endif //_DLW_AT32F437xx

#if defined(_DLW_AT32F437xx)
// --------------------- AT ----------------------------
void AT_cint_disable_mcu_irq(void)
{
#ifdef DM9051_DRIVER_INTERRUPT
	deidentify_irq_stat(ISTAT_IRQ_ENAB);
	nvic_irq_disable(spi_intr[0].interrupt.irqn);
#endif
}

void AT_cint_enable_mcu_irq(void)
{
#ifdef DM9051_DRIVER_INTERRUPT
	identify_irq_stat(ISTAT_IRQ_ENAB);
	trace_irq_stat(ISTAT_IRQ_ENAB);

	nvic_priority_group_config(spi_intr[0].interrupt.priority);
	nvic_irq_enable(spi_intr[0].interrupt.irqn, 1, 0);
#endif
}

/* IRQ handler support */
void AT_cint_exint9_5_handler(void)
{
	identify_irq_stat(ISTAT_IRQ_NOW);
	trace_irq_stat(ISTAT_IRQ_NOW);

	// add user's mcu irq-handler source code here.

	if (exint_flag_get(spi_intr[0].irqline.extline) != RESET)
	{

		identify_irq_stat(ISTAT_IRQ_NOW2);
		trace_irq_stat(ISTAT_IRQ_NOW2);
		DM_ETH_ToSet_InterruptEvent();

		exint_flag_clear(spi_intr[0].irqline.extline);
	}

	deidentify_irq_stat(ISTAT_IRQ_NOW | ISTAT_IRQ_NOW2);
}
#endif

#define pin_cs() spi_set[0].pin_cs.gpio_pin // devconf[0].wire_cs
#define spi_number() spi_set[0].spi // devconf[0].spidef.spi_num

// --------------------- AT ----------------------------
#if defined(_DLW_AT32F437xx)
void AT_spi_cs_lo(void)
{
	gpio_bits_reset(pin_cs().gpport, pin_cs().pin);
}
void AT_spi_cs_hi(void)
{
	gpio_bits_set(pin_cs().gpport, pin_cs().pin);
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
#endif

// ---------------------- data_impl -------------------------------------------------------------
#define DM9051_MRCMDX (0x70) // Read_Mem2X
#define DM9051_MRCMD (0x72)	 // Read_Mem
#define DM9051_MWCMD (0x78)	 // Write_Mem

#define OPC_REG_W 0x80 // Register Write
#define OPC_REG_R 0x00 // Register Read

#if defined(_DLW_AT32F437xx)
//[AT cspi_io]
#define dm9051_spi_command_write(rd) AT_spi_exc_data(rd)
#define dm9051_spi_dummy_read() AT_spi_exc_data(0)

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
#endif

/* dm9051_Hw_common implementation
 * source code.
 */
// #include "dm9051_cboard_data_API.h"

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

#if defined(_DLW_AT32F437xx)
#define spi_datas_read(r, b, l) AT_spi_datas_read(r, b, l)
#define spi_datas_write(r, b, l) AT_spi_datas_write(r, b, l)
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
 * source code.
 */
#include "clock.h"
#define sys_now clock_time // or xTaskGetTickCount()

void dm_delay_us(uint32_t nus)
{
	uint32_t startTick = sys_now();
	while ((sys_now() - startTick) < ((nus + 999) / 1000))
	{
		// Wait until the specified delay has passed
	}
}

void dm_delay_ms(uint16_t nms)
{
	uint32_t startTick = sys_now();
	while ((sys_now() - startTick) < nms)
	{
		// Wait until the specified delay has passed
	}
}
