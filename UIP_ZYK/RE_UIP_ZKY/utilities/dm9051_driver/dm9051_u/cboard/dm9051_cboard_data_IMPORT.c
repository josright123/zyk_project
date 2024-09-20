//#include "lwip/sys.h"								//for lwip's sys_now()
#include "dm9051_cboard_data_types.h"
#include "dm9051_cstate.h"

/* ------------------------------- NU configuration ----------------------------------------- */

/*********************************************************************************************************//**
  * @brief  Hardware related configuration.
  * @retval None
  ***********************************************************************************************************/
void DM9051_Configuration(void)
{
#if 0
    /* Enable SPI1 peripheral clock */
    CLK_EnableModuleClock(SPI1_MODULE);
    /* Select HCLK as the clock source of SPI1 */
    CLK_SetModuleClock(SPI1_MODULE, CLK_CLKSEL1_SPI1_S_HCLK, MODULE_NoMsk);
    /* Reset IP */
    SYS_ResetModule(SPI1_RST);
    /* Setup SPI1 multi-function pins */
    SYS->P0_MFP = SYS_MFP_P04_SPISS1 | SYS_MFP_P05_MOSI_1 | SYS_MFP_P06_MISO_1 | SYS_MFP_P07_SPICLK1;
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init SPI                                                                                                */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Configure SPI1 as a master, SPI clock rate 200 KHz,
         clock idle low, 32-bit transaction, drive output on falling clock edge and latch input on rising edge. */
    SPI_Open(SPI1, SPI_MASTER, SPI_MODE_0, 8, 25000000);
    /* Disable the automatic hardware slave selection function. Select the SPI1_SS pin and configure as low-active. */
    SPI_DisableAutoSS(SPI1);
    SPI_EnableFIFO(SPI1, 3, 3);
#endif
}

/* ------------------------------- AT configuration ----------------------------------------- */
const struct spi_dev_t devconf[1] = {
//#if defined(AT32F437xx)
//#endif
#ifdef _DLW_AT32F437xx
		#define devconf_at437_spi1(info, spi_setting_name, cs_setting_name, intrcfg) \
			{ \
				info, \
				SPI_PINSTD("SPI1", SPI1, CRM_SPI1_PERIPH_CLOCK, IO_MUX_NULL), \
				spi_setting_name, \
				GPIO_PINMUX(GPIOA, GPIO_PINS_5, CRM_GPIOA_PERIPH_CLOCK, &src5_mux5), /* //SCK */ \
				GPIO_PINMUX(GPIOA, GPIO_PINS_6, CRM_GPIOA_PERIPH_CLOCK, &src6_mux5),	/* //MISO */ \
				GPIO_PINMUX(GPIOA, GPIO_PINS_7, CRM_GPIOA_PERIPH_CLOCK, &src7_mux5),	/* //MOSI */ \
				cs_setting_name, \
				GPIO_PINOUT(GPIOA, GPIO_PINS_15, CRM_GPIOA_PERIPH_CLOCK, &mode_output), /* //(PA15) */ \
				intrcfg, \
			}
#ifdef DRV_INTR_MODE
		devconf_at437_spi1("AT32F437", "sck/mi/mo/ pa5/pa6/pa7", "cs/ pa15", &devconf_at437_intr_c7), //Note: NULL got no interrupt, and could faii crash! Need debug.
#else
		devconf_at437_spi1("AT32F437", "sck/mi/mo/ pa5/pa6/pa7", "cs/ pa15", NULL), //Note: NULL got no interrupt, and could faii crash! Need debug.
																					//DM9051A BENCH BOARD V1.0
#endif
#endif
			
//#if defined (AT32F413xx) || defined (AT32F415xx)
//#endif
#ifdef _DLW_AT32F415xx
#endif
};

#define DM9051_MRCMDX       (0x70)  //Read_Mem2X
#define DM9051_MRCMD        (0x72)  //Read_Mem
#define DM9051_MWCMD        (0x78)  //Write_Mem

#define OPC_REG_W       		0x80  // Register Write
#define OPC_REG_R       		0x00  // Register Read

//[NU cspi]
//#define	dm9051_spi_cmd_write(wb)		NU_spi_exc_data_head(wb)

//#define dm9051_spi_data_read() 			NU_spi_exc_data_read()
//#define	dm9051_spi_read_end()			NU_spi_exc_data_read_end()

//#define	dm9051_spi_data_write(wb)		NU_spi_exc_data_write(wb)
//#define	dm9051_spi_write_end()			NU_spi_exc_data_write_end()

//void NU_spi_cs_lo(void) {
//	SPI_SET_SS_LOW(SPI1);
//}
//void NU_spi_exc_data_head(uint8_t byte) { 
//    SPI_WRITE_TX0(SPI1, (uint32_t)byte);
//    while (SPI_IS_BUSY(SPI1));
//    SPI_READ_RX0(SPI1);
//}
//uint8_t NU_spi_exc_data_read(void) { 
//	SPI_WRITE_TX0(SPI1, 0x0);           //Dummy for read register value.
//	while (SPI_GET_RX_FIFO_EMPTY_FLAG(SPI1));
//	return (SPI_READ_RX0(SPI1) & 0xFF);
//}
//void NU_spi_exc_data_read_end(void) { 
//    while (SPI_IS_BUSY(SPI1));
//    //Clear SPI TX FIFO
//    SPI_ClearTxFIFO(SPI1);
//}

//void NU_spi_exc_data_write(uint8_t byte) { 
//	while (SPI_GET_TX_FIFO_FULL_FLAG(SPI1));
//	SPI_WRITE_TX0(SPI1, (uint32_t)byte);
//}
//void NU_spi_exc_data_write_end(void) { 
//    while (SPI_IS_BUSY(SPI1));
//    //Clear SPI RX FIFO
//    SPI_ClearRxFIFO(SPI1);
//}

//[AT cspi]
#define dm9051_spi_command_write(rd) spi_exc_data(rd)
#define dm9051_spi_dummy_read() spi_exc_data(0)

void spi_cs_lo(void) {
	gpio_bits_reset(pin_cs().gpio.gpport, pin_cs().gpio.pin); //cs.gpport->clr = cs.pin;
	//SPI_SET_SS_LOW(SPI1);
}

void spi_cs_hi(void) {
	gpio_bits_set(pin_cs().gpio.gpport, pin_cs().gpio.pin); //cs.gpport->scr = cs.pin;
	//SPI_SET_SS_HIGH(SPI1);
}

uint8_t spi_exc_data(uint8_t byte) {
  while(spi_i2s_flag_get(spi_number(), SPI_I2S_TDBE_FLAG) == RESET);	//while(spi_i2s_flag_get(SPI2, SPI_I2S_TDBE_FLAG) == RESET);
  spi_i2s_data_transmit(spi_number(), byte);							//spi_i2s_data_transmit(SPI2, byte); //spi2 tx
  while(spi_i2s_flag_get(spi_number(), SPI_I2S_RDBF_FLAG) == RESET);	//while(spi_i2s_flag_get(SPI2, SPI_I2S_RDBF_FLAG) == RESET);
  return (uint8_t) spi_i2s_data_receive(spi_number());				//return (uint8_t) spi_i2s_data_receive(SPI2); //spi2 rx (rx_pad)
}

//[NU cspi_io]
//uint8_t NU_spi_data_read(uint8_t reg);
//	uint8_t val;
//	dm9051_spi_cmd_write(reg | OPC_REG_R);
//	val = dm9051_spi_data_read();
//	dm9051_spi_read_end();
//	return val;
//uint8_t NU_spi_mem2x_read(void)
//{
//	uint8_t rxb;
//	rxb = cspi_read_reg(DM9051_MRCMDX);
//	rxb = cspi_read_reg(DM9051_MRCMDX);
//	return rxb;
//}

//[AT cspi_io]
uint8_t AT_spi_data_read(uint8_t reg)
{
	uint8_t val;
	dm9051_spi_command_write(reg | OPC_REG_R);
	val = dm9051_spi_dummy_read();
//	dm9051_spi_cmd_write(reg | OPC_REG_R);
//	val = dm9051_spi_data_read();
//	dm9051_spi_read_end();
	return val;
}

void AT_spi_data_write(uint8_t reg, uint8_t val)
{
	dm9051_spi_command_write(reg | OPC_REG_W);
	dm9051_spi_command_write(val);
//	dm9051_spi_cmd_write(reg | OPC_REG_W);
//	dm9051_spi_data_write(val);
//	dm9051_spi_write_end();
}

uint8_t AT_spi_mem2x_read(void)
{
	uint8_t rxb;
	dm9051_spi_command_write(DM9051_MRCMDX | OPC_REG_R);
	rxb = dm9051_spi_dummy_read();
	rxb = dm9051_spi_dummy_read();
	return rxb;
}

void AT_spi_mem_read(uint8_t *buf, uint16_t len)
{
	int i;
	dm9051_spi_command_write(DM9051_MRCMD | OPC_REG_R);
	for(i=0; i<len; i++)
		buf[i] = dm9051_spi_dummy_read();
//	dm9051_spi_cmd_write(DM9051_MRCMD | OPC_REG_R);
//	for(i=0; i<len; i++)
//		buf[i] = dm9051_spi_data_read();
//	dm9051_spi_read_end();
}

void AT_spi_mem_write(uint8_t *buf, uint16_t len)
{
	int i;
	dm9051_spi_command_write(DM9051_MWCMD | OPC_REG_W);
	for(i=0; i<len; i++)
		dm9051_spi_command_write(buf[i]);
//	dm9051_spi_cmd_write(DM9051_MWCMD | OPC_REG_W);
//	for(i=0; i<len; i++)
//		dm9051_spi_data_write(buf[i]);
//	dm9051_spi_write_end();
}

#if 1
//#define	board_printf(format, args...)

//static void dm_eth_delay_us( uint32_t nus ) {
//}
//static void dm_eth_delay_ms( uint32_t nms ) {
//}

/*#include "stm32f4xx.h"  // Adjust the header file according to your STM32 series

volatile uint32_t msTicks = 0;  // Counter for milliseconds

void SysTick_Handler(void) {
    msTicks++;  // Increment the tick counter
}

void SysTick_Init(void) {
    // Configure SysTick to interrupt every millisecond
    SysTick_Config(SystemCoreClock / 1000);
}

void Delay(uint32_t ms) {
    uint32_t startTick = msTicks;
    while ((msTicks - startTick) < ms) {
        // Wait until the specified delay has passed
    }
}

int main(void) {
    SysTick_Init();  // Initialize SysTick

    while (1) {
        // Your main code here
        Delay(1000);  // Delay for 1000 milliseconds (1 second)
    }
}*/

#define sys_now	clock_time //or xTaskGetTickCount()

void dm_delay_us(uint32_t nus)
{
	/*void delay_us(uint32_t nus);
	board_printf("test %d ,because rxb %02x (is %d times)\r\n", rstccc, rxbyteee, timesss);
#if freeRTOS
	vTaskDelay(pdMS_TO_TICKS((nus + 999)/ 1000));
#else
	delay_us(nus);
#endif
	*/
	uint32_t startTick = sys_now();
  while ((sys_now() - startTick) < ((nus + 999)/ 1000)) {
    // Wait until the specified delay has passed
  }
}

void dm_delay_ms(uint16_t nms)
{
	/*void delay_ms(uint16_t nms);
#if freeRTOS
	vTaskDelay(nms);
#else
	delay_ms((uint16_t)nms);
#endif
	*/
	uint32_t startTick = sys_now();
  while ((sys_now() - startTick) < nms) {
    // Wait until the specified delay has passed
  }
}
#endif
