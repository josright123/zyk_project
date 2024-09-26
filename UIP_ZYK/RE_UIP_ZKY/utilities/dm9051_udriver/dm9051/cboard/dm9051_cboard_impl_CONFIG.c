//#include "lwip/sys.h" //for lwip's sys_now()
#include "dm9051_cboard_data_types.h"
#include "dm9051_cstate.h"

#include "../dm_eth.h"

/* ------------------------------- NU configuration ----------------------------------------- */
// --------------------- NU ----------------------------

void NU_DM9051_SPI_Configuration(void);
void NU_DM9051_Interript_Configuration(void);

void DM9051_Configuration_NU(void)
{
	NU_DM9051_SPI_Configuration();
	NU_DM9051_Interript_Configuration();
}

void NU_DM9051_SPI_Configuration(void)
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

void NU_DM9051_Interript_Configuration(void) {
#ifdef DM9051_DRIVER_INTERRUPT
	identify_irq_stat(ISTAT_IRQ_CFG);
	trace_irq_stat(ISTAT_IRQ_CFG);
	identify_irq_stat(ISTAT_LOW_TRIGGER);
	trace_irq_stat(ISTAT_LOW_TRIGGER);

	// add user's mcu irq configuration code here.
#endif
}

//void NU_spi_add(void){
//	NU_DM9051_SPI_Configuration();
//}
//	
//void NU_intr_add(void) {
//	NU_DM9051_Interript_Configuration();
//}

//void NU_DM9051_Interript_Configuration(void) {}

/* ------------------------------- AT configuration ----------------------------------------- */
// --------------------- AT ----------------------------

void dm9051_boards_initialize(void)
{
	spi_add();
	intr_add();
}

	/* used in struct data declaration */
	#define GPIO_PINMUX(gpport,pin,crmclk, ptrmux)				{{gpport,pin,crmclk}, ptrmux}
	#define GPIO_PINOUT(gpport,pin,crmclk, ptrmux)				{{gpport,pin,crmclk}, ptrmux}

	#define SPI_PINSTD(spiname,spinum,crmclk,iom)				{spiname, spinum, crmclk, iom}

	//#define GPIO_PININ(gpport,pin,crmclk, ptrmux)				{gpport,pin,crmclk,}, ptrmux
	#define GPIO_PININ_INFO(inf,gpport,pin,crmclk, ptrmux)		{inf, {{gpport,pin,crmclk}, ptrmux}}

	//#define GPIO_PINNORM(gpport,pin,crmclk, ptrmux)			{gpport,pin,crmclk, ptrmux} //used (AT32F413/415)
	
	#define MUX_DATA(src,muxto)	\
		GPIO_MODE_MUX, src, muxto,
	#define INP_DATA()	\
		GPIO_MODE_INPUT,
	#define OUTP_DATA()	\
		GPIO_MODE_OUTPUT,

	const gpio_mux_t src1_mux6 = { MUX_DATA(GPIO_PINS_SOURCE1, GPIO_MUX_6)};
	const gpio_mux_t src2_mux5 = { MUX_DATA(GPIO_PINS_SOURCE2, GPIO_MUX_5)};
	const gpio_mux_t src4_mux6 = { MUX_DATA(GPIO_PINS_SOURCE4, GPIO_MUX_6)};
	const gpio_mux_t src5_mux5 = { MUX_DATA(GPIO_PINS_SOURCE5, GPIO_MUX_5)};
	const gpio_mux_t src6_mux5 = { MUX_DATA(GPIO_PINS_SOURCE6, GPIO_MUX_5)};
	const gpio_mux_t src7_mux5 = { MUX_DATA(GPIO_PINS_SOURCE7, GPIO_MUX_5)};
	const gpio_mux_t mode_input = { INP_DATA()};
	const gpio_mux_t mode_output = { OUTP_DATA()};

	const struct modscfg_st devconf_at437_intr_c7 = { //devconf_intr_c7,
		"pc7",
		{CRM_SCFG_PERIPH_CLOCK, SCFG_PORT_SOURCE_GPIOC, SCFG_PINS_SOURCE7},
		{ /*(essential) &pe_c7*/
			"extline pc7",
			{ CRM_GPIOC_PERIPH_CLOCK, EXINT_LINE_7, EXINT9_5_IRQn, NVIC_PRIORITY_GROUP_0}, // correspond to and with PC7
		},
		GPIO_PININ_INFO("gpio pc7", GPIOC, GPIO_PINS_7, CRM_GPIOC_PERIPH_CLOCK, &mode_input),
	};

	const struct spi_dev_t devconf[1] = {
	//#if defined(AT32F437xx)
	//#endif
	#if defined(_DLW_AT32F437xx)
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
	#ifdef DM9051_DRIVER_INTERRUPT
			devconf_at437_spi1("AT32F437", "sck/mi/mo/ pa5/pa6/pa7", "cs/ pa15", &devconf_at437_intr_c7), //Note: NULL got no interrupt, and could faii crash! Need debug.
	#else
			devconf_at437_spi1("AT32F437", "sck/mi/mo/ pa5/pa6/pa7", "cs/ pa15", NULL), //Note: NULL got no interrupt //DM9051A BENCH BOARD V1.0
	#endif
	#endif
				
	//#if defined (AT32F413xx) || defined (AT32F415xx)
	//#endif
	#if defined(_DLW_AT32F415xx) || defined(_DLW_AT32F413xx)
	#endif
	};

	#define FIELD_SPIDEV(field)			devconf[0].field
	#define spi_number()						FIELD_SPIDEV(spidef.spi_num)
	#define pin_wire_sck()					FIELD_SPIDEV(wire_sck)
	#define pin_wire_mi()						FIELD_SPIDEV(wire_mi)
	#define pin_wire_mo()						FIELD_SPIDEV(wire_mo)
	#define pin_cs()								FIELD_SPIDEV(wire_cs)

	#ifdef DM9051_DRIVER_INTERRUPT
	//[cint]
	#define intr_pointer()				devconf[0].intr_cfg	//FIELD_SPIDEV(intr_cfg)
	#define intr_data_scfg()			&devconf[0].intr_cfg->extend1 //PTR_EXINTD(extend)

	#define PTR_EXINTD(nextfield)		devconf[0].intr_cfg->nextfield
	#define intr_gpio_ptr()				((const pin_t *)(&PTR_EXINTD(option1.pin)))
//	#define scfg_info()					devconf[0].intr_cfg->scfg_inf

//	#define scfg_crm()					PTR_EXINTD(scfg_init.scfg_clk)
//	#define scfg_port()					PTR_EXINTD(scfg_init.scfg_port_src)
//	#define scfg_pin()					PTR_EXINTD(scfg_init.scfg_pin_src)

//	#define exint_crm()					PTR_EXINTD(extend1.extline.intr_crm_clk)
	#endif

	void spi_add(void) //=== pins_config(); //total_eth_count++;
	{
	  #if 0
	  //.#ifndef AT32F437xx
	  /* Setting of Non-f437 */
	  if (spi_number() == SPI1) {
		  if  (spi_iomux() & IO_CRM_CLOCK) {

			#if !defined(AT32F437xx) || defined(AT32F413xx) || defined(AT32F415xx) || \
				defined(AT32F403Axx) || defined(AT32F403xx) || defined(AT32F407xx)
			crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE); //Non-f437,iomux-config
			gpio_pin_remap_config(SWJTAG_GMUX_010, TRUE); //Non-f437,iomux-config
			#endif //.
		  }
		  if  (spi_iomux() & IO_MUX_PINREMAP) {

			#if !defined(AT32F437xx) || defined(AT32F413xx) || defined(AT32F415xx) || \
				defined(AT32F403Axx) || defined(AT32F403xx) || defined(AT32F407xx)
			  gpio_pin_remap_config(SPI1_MUX_01, TRUE); //Non-f437,remap
			#endif //.
		  }
	  }
	  //.#endif
	  #endif

	  pin_config(&pin_wire_sck(), GPIO_PULL_NONE); //,GPIO_MODE_MUX
	  pin_config(&pin_wire_mi(), GPIO_PULL_NONE); //,GPIO_MODE_MUX
	  pin_config(&pin_wire_mo(), GPIO_PULL_NONE); //,GPIO_MODE_MUX

	  spi_config(); //(spi_port_ptr(_pinCode));
	  pin_config(&pin_cs(), GPIO_PULL_NONE);
	}

	void intr_add(void)
	{
	#ifdef DM9051_DRIVER_INTERRUPT
	  exint_polarity_config_type pol = EXINT_TRIGGER_FALLING_EDGE;
	  const struct modscfg_st *intr = intr_pointer();

	  if (intr) {
		intr_gpio_pin_config(intr_gpio_ptr(), GPIO_PULL_UP); //[ops] &intr->option1.pin
		if (intr_data_scfg()) { //&devconf[0].intr_cfg->extend1

			identify_irq_stat(ISTAT_IRQ_CFG);
			trace_irq_stat(ISTAT_IRQ_CFG);

			log_intr_qpio_pin_config();

			intr_irqline_config(intr_data_scfg(), pol); //[ops]

			if (pol == EXINT_TRIGGER_FALLING_EDGE) {
			  identify_irq_stat(ISTAT_LOW_TRIGGER);
			  trace_irq_stat(ISTAT_LOW_TRIGGER);
			}
			//else if (pol == EXINT_TRIGGER_RISING_EDGE) {
			// identify_irq_stat(ISTAT_HIGH_TRIGGER);
			// trace_irq_stat(ISTAT_HIGH_TRIGGER);
			//}
		}
	  } else {
		printf("\r\n");
		printf("[polling]\r\n");
	  }
	#endif
	}

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

	/* IRQ handler support */
	void cint_exint9_5_handler(void)
	{
		// add user's mcu irq handler such as EINT0_IRQHandler/EINT1_IRQHandler, and
		//	Let it call this "cint_exint9_5_handler()" subroutine,
		//	Put some control code here to maintain the mcu's INTERRUPT for
		//	allow further cycllic interrupt-in.
	
		//[EXINT_LINE_5 ~ EXINT_LINE_9]
//		uint32_t exint_line = EXINT_LINE_7;
		
		identify_irq_stat(ISTAT_IRQ_NOW);
		trace_irq_stat(ISTAT_IRQ_NOW);
	
//		if(exint_flag_get(exint_line) != RESET) {
			if(exint_flag_get(EXINT_LINE_7) != RESET) {
			
				identify_irq_stat(ISTAT_IRQ_NOW2);
				trace_irq_stat(ISTAT_IRQ_NOW2);
				DM_ETH_InterruptHdlr();
		
				exint_flag_clear(EXINT_LINE_7);
			}
//			exint_flag_clear(exint_line);
//		}
		
		deidentify_irq_stat(ISTAT_IRQ_NOW | ISTAT_IRQ_NOW2);
	}

#if 1
#include "clock.h"
#define sys_now	clock_time //or xTaskGetTickCount()

void dm_delay_us(uint32_t nus)
{
	uint32_t startTick = sys_now();
  while ((sys_now() - startTick) < ((nus + 999)/ 1000)) {
    // Wait until the specified delay has passed
  }
}

void dm_delay_ms(uint16_t nms)
{
	uint32_t startTick = sys_now();
  while ((sys_now() - startTick) < nms) {
    // Wait until the specified delay has passed
  }
}
#endif
