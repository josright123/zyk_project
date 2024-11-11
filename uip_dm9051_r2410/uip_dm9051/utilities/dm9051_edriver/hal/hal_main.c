/**
 *******************************************************************************
 * @file    hal_main.c
 * @brief   Hardware Abstraction Layer for DM9051 Ethernet Controller
 * 
 * @details This file provides functions for SPI initialization and communication with
 *          the DM9051 Ethernet controller.
 * 
 * @version 1.0.0
 * @author  Joseph CHANG
 * @copyright (c) 2023-2025 Davicom Semiconductor, Inc.
 * @date    2024-11-10
 *******************************************************************************
 */

#include "control/drv_control/conf_core.h"
#include "control/drv_control/dm9051_eth_debug.h"

// SPI Configuration Structure
struct spi_config_t {
    spi_type *spi;                       // SPI instance
    crm_periph_clock_type clock;         // Peripheral clock type
    struct gpio_mux_t sck;               // SCK pin configuration
    struct gpio_mux_t miso;              // MISO pin configuration
    struct gpio_mux_t mosi;              // MOSI pin configuration
};

// SPI Function Prototypes
#define dm9051_spi_command_write AT_spi_xfer
#define dm9051_spi_dummy_read() AT_spi_xfer(0)
uint8_t AT_spi_xfer(uint8_t byte);

#define AT_spi_data_read spi_data_read
#define AT_spi_data_write spi_data_write
#define AT_spi_mem2x_read spi_mem2x_read
#define AT_spi_mem_read spi_mem_read
#define AT_spi_mem_write spi_mem_write
void AT_spi_data_read(uint8_t reg, uint8_t *pd);
void AT_spi_data_write(uint8_t reg, uint8_t val);
void AT_spi_mem2x_read(uint8_t *pd);
void AT_spi_mem_read(uint8_t *buf, uint16_t len);
void AT_spi_mem_write(uint8_t *buf, uint16_t len);

void AT_hal_init(void);
uint32_t AT_hal_tick_count(void);
void AT_hal_tick(void);
void ctick_delay_us(uint32_t nus);

// SPI Configuration Initialization
#define AT_spi_config_init dm9051if_spi_config
void AT_spi_config_init(const struct spi_config_t *config);

// SPI Command Definitions
#define DM9051_MRCMDX (0x70) // Read_Mem2X
#define DM9051_MRCMD (0x72)  // Read_Mem
#define DM9051_MWCMD (0x78)  // Write_Mem

#define spi_set() &spi_cset[0]
#define spi_number() spi_cset[0].spi

// SPI Configuration Set
static const struct spi_config_t spi_cset[1] = {
    {
        SPI1,
        CRM_SPI1_PERIPH_CLOCK,
        {{GPIOA, GPIO_PINS_5, GPIO_PULL_NONE, CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_MUX}, GPIO_PINS_SOURCE5, GPIO_MUX_5},
        {{GPIOA, GPIO_PINS_6, GPIO_PULL_NONE, CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_MUX}, GPIO_PINS_SOURCE6, GPIO_MUX_5},
        {{GPIOA, GPIO_PINS_7, GPIO_PULL_NONE, CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_MUX}, GPIO_PINS_SOURCE7, GPIO_MUX_5},
    }
};

#if defined(_DLW_AT32F437xx)
// SPI Initialization Function
void AT_hal_init(void)
{
    // Initialize SPI and GPIO configurations
    dm9051if_spi_config(spi_set());
    dm9051if_muxpin_config(spi_set().sck);
    dm9051if_muxpin_config(spi_set().miso);
    dm9051if_muxpin_config(spi_set().mosi);
    dm9051if_stdpin_config(&cs_gpio);
    dm9051if_stdpin_config(&intr_gpio);
    dm9051if_intr_config(intr_set());
}

// SPI Configuration Function
void AT_spi_config_init(const struct spi_config_t *config) {
    spi_init_type spi_init_struct;

    // Enable peripheral clock and initialize SPI
    crm_periph_clock_enable(config->clock, TRUE);
    spi_default_para_init(&spi_init_struct);
    spi_init_struct.transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;
    spi_init_struct.master_slave_mode = SPI_MODE_MASTER;
    spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_8;
    spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_MSB;
    spi_init_struct.frame_bit_num = SPI_FRAME_8BIT;
    spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_LOW;
    spi_init_struct.clock_phase = SPI_CLOCK_PHASE_1EDGE;
    spi_init(config->spi, &spi_init_struct);
    spi_enable(config->spi, TRUE);
}

// SPI Transfer Function
uint8_t AT_spi_xfer(uint8_t byte)
{
    while (spi_i2s_flag_get(spi_number(), SPI_I2S_TDBE_FLAG) == RESET);
    spi_i2s_data_transmit(spi_number(), byte);
    while (spi_i2s_flag_get(spi_number(), SPI_I2S_RDBF_FLAG) == RESET);
    return (uint8_t)spi_i2s_data_receive(spi_number());
}

// SPI Data Read Function
void AT_spi_data_read(uint8_t reg, uint8_t *pd)
{
    dm9051_spi_command_write(reg | OPC_REG_R);
    *pd = dm9051_spi_dummy_read();
}

// SPI Data Write Function
void AT_spi_data_write(uint8_t reg, uint8_t val) {
    dm9051_spi_command_write(reg | OPC_REG_W);
    dm9051_spi_command_write(val);
}

// Memory Read and Write Functions
void AT_spi_mem2x_read(uint8_t *pd) {
    dm9051_spi_command_write(DM9051_MRCMDX | OPC_REG_R);
    dm9051_spi_dummy_read();
    *pd = dm9051_spi_dummy_read();
}

void AT_spi_mem_read(uint8_t *buf, uint16_t len)
{
	uint16_t i;
    dm9051_spi_command_write(DM9051_MRCMD | OPC_REG_R);
    for (i = 0; i < len; i++)
        buf[i] = dm9051_spi_dummy_read();
}

void AT_spi_mem_write(uint8_t *buf, uint16_t len)
{
	uint16_t i;
    dm9051_spi_command_write(DM9051_MWCMD | OPC_REG_W);
    for (i = 0; i < len; i++)
        dm9051_spi_command_write(buf[i]);
}

// Tick Management
static uint32_t AT_HalTicks = 0;

uint32_t AT_hal_tick_count(void) {
    return AT_HalTicks;
}

void AT_hal_tick(void) {
    AT_HalTicks++;
}
#endif //_DLW_AT32F437xx

// Register Read/Write Functions
uint8_t cspi_read_reg(uint8_t reg)
{
    uint8_t val;
    dm9051if_stdpin_lo(&cs_gpio); // Chip select low
    spi_data_read(reg, &val);
    dm9051if_stdpin_hi(&cs_gpio); // Chip select high
    return val;
}

void cspi_write_reg(uint8_t reg, uint8_t val) {
    dm9051if_stdpin_lo(&cs_gpio); // Chip select low
    spi_data_write(reg, val);
    dm9051if_stdpin_hi(&cs_gpio); // Chip select high
}

// Additional Functions for Register Operations
void cspi_read_regs_long(uint8_t reg, uint8_t *buf, uint16_t len);
void cspi_read_regs_each(uint8_t reg, uint8_t *buf, uint16_t len);

void cspi_read_regs(uint8_t reg, uint8_t *buf, uint16_t len, csmode_t csmode) {
    if (csmode == CS_LONG) {
        cspi_read_regs_long(reg, buf, len);
    } else { // CS_EACH
        cspi_read_regs_each(reg, buf, len);
    }
}

void cspi_read_regs_long(uint8_t reg, uint8_t *buf, uint16_t len) {
	uint16_t i;
    dm9051if_stdpin_lo(&cs_gpio); // Chip select low
    for (i = 0; i < len; i++, reg++) {
        spi_data_read(reg, &buf[i]);
    }
    dm9051if_stdpin_hi(&cs_gpio); // Chip select high
}

void cspi_read_regs_each(uint8_t reg, uint8_t *buf, uint16_t len) {
	uint16_t i;
    for (i = 0; i < len; i++, reg++) {
        buf[i] = cspi_read_reg(reg);
    }
}

void cspi_write_regs(uint8_t reg, const uint8_t *buf, uint16_t len)
{
	uint16_t i;
    for (i = 0; i < len; i++, reg++) {
        cspi_write_reg(reg, buf[i]);
    }
}

// PHY Read/Write Functions
uint16_t cspi_phy_read(uint16_t uReg) {
    int w = 0;
    uint16_t uData;

    cspi_write_reg(DM9051_EPAR, DM9051_PHY | uReg);
    cspi_write_reg(DM9051_EPCR, 0xc);
    ctick_delay_us(1);
    while (cspi_read_reg(DM9051_EPCR) & 0x1) {
        ctick_delay_us(1);
        if (++w >= 500) break; // Timeout
    }

    cspi_write_reg(DM9051_EPCR, 0x0);
    uData = (cspi_read_reg(DM9051_EPDRH) << 8) | cspi_read_reg(DM9051_EPDRL);
    return uData;
}

void cspi_phy_write(uint16_t reg, uint16_t value) {
    int w = 0;

    cspi_write_reg(DM9051_EPAR, DM9051_PHY | reg);
    cspi_write_reg(DM9051_EPDRL, (value & 0xff));
    cspi_write_reg(DM9051_EPDRH, ((value >> 8) & 0xff));
    cspi_write_reg(DM9051_EPCR, 0xa);
    ctick_delay_us(1);
    while (cspi_read_reg(DM9051_EPCR) & 0x1) {
        ctick_delay_us(1);
        if (++w >= 500) break; // Timeout
    }

    cspi_write_reg(DM9051_EPCR, 0x0);
}

// RX Buffer Read Function
uint8_t cspi_read_rxb(void) {
    uint8_t rxb;
    dm9051if_stdpin_lo(&cs_gpio); // Chip select low
    spi_mem2x_read(&rxb);
    dm9051if_stdpin_hi(&cs_gpio); // Chip select high
    return rxb;
}

// TX Request Function
void cspi_tx_req(void) {
    cspi_write_reg(DM9051_TCR, TCR_TXREQ); // Cleared after TX complete
    DM9051_TX_DELAY((cspi_read_reg(DM9051_TCR) & TCR_TXREQ), ctick_delay_us(5));
}

// Memory Read/Write Functions
void cspi_read_mem(uint8_t *buf, uint16_t len)
{
    dm9051if_stdpin_lo(&cs_gpio); // Chip select low
    spi_mem_read(buf, len);
    dm9051if_stdpin_hi(&cs_gpio); // Chip select high
}

void cspi_write_mem(uint8_t *buf, uint16_t len)
{
    dm9051if_stdpin_lo(&cs_gpio); // Chip select low
    spi_mem_write(buf, len);
    dm9051if_stdpin_hi(&cs_gpio); // Chip select high
}

// Delay Functions
void ctick_delay_us(uint32_t nus)
{
    uint32_t start = dm_sys_now();
    while ((dm_sys_now() - start) < ((nus + 999) / 1000));
}

void ctick_delay_ms(uint16_t nms)
{
    uint32_t start = dm_sys_now();
    while ((dm_sys_now() - start) < nms);
}
