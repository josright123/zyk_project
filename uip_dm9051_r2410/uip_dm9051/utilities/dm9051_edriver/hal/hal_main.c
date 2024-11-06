/**
 **************************************************************************
 * @file     hal_main.c
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

// SPI Configuration Structure
struct spi_config_t
{
	spi_type *spi;
	crm_periph_clock_type clock;
	struct gpio_mux_t sck, miso, mosi; 
	/*struct gpio_config_t cs */
};

#define spi_config_init dm9051if_spi_config
void spi_config_init(const struct spi_config_t *config);


#define	AT_spi_exc_data	dm9051if_spi_xfer //dm9051if_exc_data //AT_spi_exc_data
uint8_t AT_spi_exc_data(uint8_t byte);

#define dm9051_spi_command_write dm9051if_spi_xfer //dm9051if_exc_data //AT_spi_exc_data
#define dm9051_spi_dummy_read() dm9051if_spi_xfer(0) //dm9051if_exc_data(0) //AT_spi_exc_data(0)

#define AT_spi_data_read spi_data_read
#define AT_spi_data_write spi_data_write
#define AT_spi_mem2x_read spi_mem2x_read
#define AT_spi_mem_read  spi_mem_read
#define AT_spi_mem_write  spi_mem_write

void dm9051_hal_init(void);
void dm9051_hal_tick(void);

#if defined(_DLW_AT32F437xx)

void dm_delay_us(uint32_t nus);

static const struct spi_config_t spi_cset[1] = {
	{
		SPI1,
		CRM_SPI1_PERIPH_CLOCK,
		{{GPIOA, GPIO_PINS_5, GPIO_PULL_NONE, CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_MUX}, GPIO_PINS_SOURCE5, GPIO_MUX_5},
		{{GPIOA, GPIO_PINS_6, GPIO_PULL_NONE, CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_MUX}, GPIO_PINS_SOURCE6, GPIO_MUX_5},
		{{GPIOA, GPIO_PINS_7, GPIO_PULL_NONE, CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_MUX}, GPIO_PINS_SOURCE7, GPIO_MUX_5},
	}};

#define spi_set() &spi_cset[0]
#define spi_number() spi_cset[0].spi

void dm9051_hal_init(void)
{	
	/* purpose: spi_add, and intr_add
	 */
	/* purpose: seperating cs_gpio_add, and intr_gpio_add
	 */
	dm9051if_spi_config(spi_set());
	dm9051if_muxpin_config(spi_set().sck);
	dm9051if_muxpin_config(spi_set().miso);
	dm9051if_muxpin_config(spi_set().mosi);
	dm9051if_stdpin_config(&cs);
	
	dm9051if_stdpin_config(&intr);
	dm9051if_intr_config(intr_set());
}

// Static function prototypes
//#define esck() (config->sck)
//#define emiso() (config->miso)
//#define emosi() (config->mosi)
void spi_config_init(const struct spi_config_t *ss)
{
	//=configure_cspi(config);
	spi_init_type spi_init_struct;

	crm_periph_clock_enable(ss->clock, TRUE);
	spi_default_para_init(&spi_init_struct);
	spi_init_struct.transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;
	spi_init_struct.master_slave_mode = SPI_MODE_MASTER;
	spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_8;
	spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_MSB;
	spi_init_struct.frame_bit_num = SPI_FRAME_8BIT;
	spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_LOW;
	spi_init_struct.clock_phase = SPI_CLOCK_PHASE_1EDGE;
	spi_init_struct.cs_mode_selection = SPI_CS_SOFTWARE_MODE;
	spi_init(ss->spi, &spi_init_struct);
	spi_enable(ss->spi, TRUE);
}

// ---------------------- hw_impl -------------------------------------------------------------

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
	dm9051if_stdpin_lo(&cs); //dm9051if_cs_lo();
	spi_data_read(reg, &val);
	dm9051if_stdpin_hi(&cs); //dm9051if_cs_hi();
	return val;
}

void cspi_write_reg(uint8_t reg, uint8_t val)
{
	dm9051if_stdpin_lo(&cs); //dm9051if_cs_lo();
	spi_data_write(reg, val);
	dm9051if_stdpin_hi(&cs); //dm9051if_cs_hi();
}

void cspi_read_regs(uint8_t reg, uint8_t *buf, uint16_t len, csmode_t csmode)
{
	int i;
	if (csmode == CS_LONG)
	{
		dm9051if_stdpin_lo(&cs); //dm9051if_cs_lo();
		for (i = 0; i < len; i++, reg++)
			spi_data_read(reg, &buf[i]);
		dm9051if_stdpin_hi(&cs); //dm9051if_cs_hi();
	}
	else
	{ // CS_EACH
		for (i = 0; i < len; i++, reg++)
			buf[i] = cspi_read_reg(reg);
	}
}

void cspi_write_regs(uint8_t reg, const uint8_t *buf, uint16_t len)
{
	int i;
	for (i = 0; i < len; i++, reg++)
		cspi_write_reg(reg, buf[i]);
}

uint16_t cspi_phy_read(uint16_t uReg)
{
	int w = 0;
	uint16_t uData;

	cspi_write_reg(DM9051_EPAR, DM9051_PHY | uReg);
	cspi_write_reg(DM9051_EPCR, 0xc);
	dm_delay_us(1);
	while (cspi_read_reg(DM9051_EPCR) & 0x1)
	{
		dm_delay_us(1);
		if (++w >= 500)
			break;
	} // Wait complete

	cspi_write_reg(DM9051_EPCR, 0x0);
	uData = (cspi_read_reg(DM9051_EPDRH) << 8) | cspi_read_reg(DM9051_EPDRL);

	return uData;
}

void cspi_phy_write(uint16_t reg, uint16_t value)
{
	int w = 0;

	cspi_write_reg(DM9051_EPAR, DM9051_PHY | reg);
	cspi_write_reg(DM9051_EPDRL, (value & 0xff));
	cspi_write_reg(DM9051_EPDRH, ((value >> 8) & 0xff));
	/* Issue phyxcer write command */
	cspi_write_reg(DM9051_EPCR, 0xa);
	dm_delay_us(1);
	while (cspi_read_reg(DM9051_EPCR) & 0x1)
	{
		dm_delay_us(1);
		if (++w >= 500)
			break;
	} // Wait complete

	cspi_write_reg(DM9051_EPCR, 0x0);
}

uint8_t cspi_read_rxb(void)
{
	uint8_t rxb;
	dm9051if_stdpin_lo(&cs); //dm9051if_cs_lo();
	spi_mem2x_read(&rxb);
	dm9051if_stdpin_hi(&cs); //dm9051if_cs_hi();
	return rxb;
}
void cspi_tx_req(void)
{
	cspi_write_reg(DM9051_TCR, TCR_TXREQ); /* Cleared after TX complete */
	DM9051_TX_DELAY((cspi_read_reg(DM9051_TCR) & TCR_TXREQ), dm_delay_us(5));
}

void cspi_read_mem(uint8_t *buf, uint16_t len)
{
	dm9051if_stdpin_lo(&cs); //dm9051if_cs_lo();
	spi_mem_read(buf, len);
	dm9051if_stdpin_hi(&cs); //dm9051if_cs_hi();
}
void cspi_write_mem(uint8_t *buf, uint16_t len)
{
	dm9051if_stdpin_lo(&cs); //dm9051if_cs_lo();
	spi_mem_write(buf, len);
	dm9051if_stdpin_hi(&cs); //dm9051if_cs_hi();
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
