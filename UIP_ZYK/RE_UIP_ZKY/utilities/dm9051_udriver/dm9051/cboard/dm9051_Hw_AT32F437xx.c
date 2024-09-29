#include "cboard/dm9051_lw_mcu_default_IN.h" //#include "dm9051_env.h" of
#include "dm9051_cboard_data_types.h"
#include "dm9051_cstate.h"
#include "../dm_eth.h"

#if defined (_DLW_AT32F437xx)
/* ------------------------------- AT data ----------------------------------------- */
// --------------------- AT ----------------------------

//#define FIELD_SPIDEV(field)			devconf[0].field
//#define spi_number()						FIELD_SPIDEV(spidef.spi_num)
//#define pin_cs()								FIELD_SPIDEV(wire_cs)
//
//#define PTR_EXINTD(nextfield)		devconf[0].intr_cfg->nextfield
//#define scfg_info()							devconf[0].intr_cfg->scfg_inf
																//PTR_EXINTD(scfg_inf)
//#define intr_data_scfg()				((const struct extscfg_st *)&devconf[0].intr_cfg->extend1)
																//&PTR_EXINTD(extend1)
//#define intr_gpio_ptr()					((const pin_t *)&devconf[0].intr_cfg->option1.pin)
																//&PTR_EXINTD(option1.pin)

/* ------------------------------- AT configuration ----------------------------------------- */
// --------------------- AT ----------------------------
void dm9051_boards_initialize_AT(void)
{
	spi_add();
	#ifdef DM9051_DRIVER_INTERRUPT
	intr_add();
	#endif
}

void pin_gpio_config(const pin_t *pin, gpio_pull_type gppull) {
	const gpio_t *gpio = &pin->gpio;
	const gpio_mux_t *mux = pin->mux;
	
  gpio_init_type gpio_init_struct;
  crm_periph_clock_enable(gpio->gp_crmclk, TRUE); /* enable the gpioa clock */

  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_out_type  		= GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_drive_strength	= GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_mode			= mux->gpio_mode; //gmode
											//gpio->gpio_mode; //gpmode; //GPIO_MODE_INPUT;

  gpio_init_struct.gpio_pull			= gppull; //exint_cfg.gpio_pull; //GPIO_PULL_DOWN; GPIO_PULL_UP; //GPIO_PULL_NONE;
  gpio_init_struct.gpio_pins			= gpio->pin;
  gpio_init(gpio->gpport, &gpio_init_struct);
}

void pin_gpio_mux(const pin_t *pin) {
 #if defined(_DLW_AT32F437xx)
	const gpio_t *gpio = &pin->gpio;
	const gpio_mux_t *mux = pin->mux;
	
  if (mux->gpio_mode == GPIO_MODE_MUX)
		gpio_pin_mux_config(gpio->gpport, mux->pinsrc, mux->muxsel);
 #endif
}

void pin_config(const pin_t *pin, gpio_pull_type gppull)
{
	pin_gpio_config(pin, gppull);
	pin_gpio_mux(pin);
}

		#define MUX_DATA(src,muxto)	\
			GPIO_MODE_MUX, src, muxto,
		#define SPI_PINSTD(spiname,spinum,crmclk,iom)				{spiname, spinum, crmclk, iom}
		#define GPIO_PINMUX(gpport,pin,crmclk, ptrmux)				{{gpport,pin,crmclk}, ptrmux}
		#define GPIO_PINOUT(gpport,pin,crmclk, ptrmux)				{{gpport,pin,crmclk}, ptrmux}
		const gpio_mux_t src5_mux5 = { MUX_DATA(GPIO_PINS_SOURCE5, GPIO_MUX_5)};
		const gpio_mux_t src6_mux5 = { MUX_DATA(GPIO_PINS_SOURCE6, GPIO_MUX_5)};
		const gpio_mux_t src7_mux5 = { MUX_DATA(GPIO_PINS_SOURCE7, GPIO_MUX_5)};
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
		
		#define INP_DATA()	\
			GPIO_MODE_INPUT,
		#define OUTP_DATA()	\
			GPIO_MODE_OUTPUT,
				
		const gpio_mux_t mode_input = { INP_DATA()};
		const gpio_mux_t mode_output = { OUTP_DATA()};

		#define GPIO_PININ_INFO(inf,gpport,pin,crmclk, ptrmux)		{inf, {{gpport,pin,crmclk}, ptrmux}}		
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
	#ifdef DM9051_DRIVER_INTERRUPT
	//=devconf_at437_spi1("AT32F437", "sck/mi/mo/ pa5/pa6/pa7", "cs/ pa15", &devconf_at437_intr_c7), //Note: NULL got no interrupt, and could faii crash! Need debug.
	{
		"AT32F437",
		SPI_PINSTD("SPI1", SPI1, CRM_SPI1_PERIPH_CLOCK, IO_MUX_NULL),
		"sck/mi/mo/ pa5/pa6/pa7",
		GPIO_PINMUX(GPIOA, GPIO_PINS_5, CRM_GPIOA_PERIPH_CLOCK, &src5_mux5), /* //SCK */
		GPIO_PINMUX(GPIOA, GPIO_PINS_6, CRM_GPIOA_PERIPH_CLOCK, &src6_mux5),	/* //MISO */
		GPIO_PINMUX(GPIOA, GPIO_PINS_7, CRM_GPIOA_PERIPH_CLOCK, &src7_mux5),	/* //MOSI */
		"cs/ pa15",
		GPIO_PINOUT(GPIOA, GPIO_PINS_15, CRM_GPIOA_PERIPH_CLOCK, &mode_output), /* //(PA15) */
		&devconf_at437_intr_c7,
	},
	#else
	devconf_at437_spi1("AT32F437", "sck/mi/mo/ pa5/pa6/pa7", "cs/ pa15", NULL), //Note: NULL got no interrupt, and could faii crash! Need debug.
	#endif
};

void spi_config(void)
{
//#define FIELD_SPIDEV(field)			devconf[0].field
//#define spi_crm()					FIELD_SPIDEV(spidef.spi_crm_clk)

#if 1
  spi_init_type spi_init_struct;
  crm_periph_clock_enable(/*spi_crm()*/ devconf[0].spidef.spi_crm_clk, TRUE); //crm_spi_clk(_pinCode), CRM_SPI3_PERIPH_CLOCK/CRM_SPI2_PERIPH_CLOCK

  spi_default_para_init(&spi_init_struct);
  spi_init_struct.transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;
  spi_init_struct.master_slave_mode = SPI_MODE_MASTER;
  spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_8;
  //spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_LSB;
  spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_MSB;
  spi_init_struct.frame_bit_num = SPI_FRAME_8BIT;
  spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_LOW;
  //spi_init_struct.clock_phase = SPI_CLOCK_PHASE_2EDGE;
  spi_init_struct.clock_phase = SPI_CLOCK_PHASE_1EDGE;
  spi_init_struct.cs_mode_selection = SPI_CS_SOFTWARE_MODE;
  spi_init(/*spi_number()*/ devconf[0].spidef.spi_num, &spi_init_struct); //SPI2, _spi_num(_pinCode)
  spi_enable(/*spi_number()*/ devconf[0].spidef.spi_num, TRUE); //SPI2, _spi_num(_pinCode)
#endif
#if 0
  spi_init_type spi_init_struct;
  /* xxx spi-config xxx */
  crm_periph_clock_enable(CRM_SPI1_PERIPH_CLOCK, TRUE);
  spi_default_para_init(&spi_init_struct);
  
  spi_init_struct.transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;
  spi_init_struct.master_slave_mode = SPI_MODE_MASTER;
  spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_8;
  //spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_LSB;
	spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_MSB;
  spi_init_struct.frame_bit_num = SPI_FRAME_8BIT;
  spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_LOW;
  //spi_init_struct.clock_phase = SPI_CLOCK_PHASE_2EDGE;
	spi_init_struct.clock_phase = SPI_CLOCK_PHASE_1EDGE;
  spi_init_struct.cs_mode_selection = SPI_CS_SOFTWARE_MODE;
  spi_init(SPI1, &spi_init_struct);
  
  spi_enable(SPI1, TRUE);
#endif
}

//#define scfg_crm()					PTR_EXINTD(scfg_init.scfg_clk)
//#define exint_crm()					PTR_EXINTD(extend1.extline.intr_crm_clk)

//#define scfg_port()					PTR_EXINTD(scfg_init.scfg_port_src)
//#define scfg_pin()					PTR_EXINTD(scfg_init.scfg_pin_src)

static void intr_irqline_config(const struct extscfg_st *pexint_set, exint_polarity_config_type polarity)
{
  exint_init_type exint_init_struct;

  crm_periph_clock_enable(/*scfg_crm()*/ devconf[0].intr_cfg->scfg_init.scfg_clk, TRUE);  // CRM_SCFG_PERIPH_CLOCK
  crm_periph_clock_enable(/*exint_crm()*/devconf[0].intr_cfg->extend1.extline.intr_crm_clk, TRUE); // CRM_GPIOC_PERIPH_CLOCK

//#ifndef AT32F437xx
//  gpio_exint_line_config(scfg_port(), scfg_pin()); // SCFG_PORT_SOURCE_GPIOA, SCFG_PINS_SOURCE0
//#else
//#endif
  scfg_exint_line_config(
		/*scfg_port(), 
		scfg_pin()*/
		devconf[0].intr_cfg->scfg_init.scfg_port_src,
		devconf[0].intr_cfg->scfg_init.scfg_pin_src
		); // SCFG_PORT_SOURCE_GPIOC, SCFG_PINS_SOURCE7

  exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_enable = TRUE;

  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_select = pexint_set->extline.extline; // line_no;
  exint_init_struct.line_polarity = polarity;                  // EXINT_TRIGGER_RISING_EDGE/ EXINT_TRIGGER_FALLING_EDGE
  exint_init(&exint_init_struct);
}

//#define FIELD_SPIDEV(field)			devconf[0].field
//#define spi_number()						FIELD_SPIDEV(spidef.spi_num)
//#define pin_wire_sck()					FIELD_SPIDEV(wire_sck)
//#define pin_wire_mi()						FIELD_SPIDEV(wire_mi)
//#define pin_wire_mo()						FIELD_SPIDEV(wire_mo)
//#define pin_cs()								FIELD_SPIDEV(wire_cs)

void spi_add(void) {
  pin_config(/*&pin_wire_sck()*/ &devconf[0].wire_sck, GPIO_PULL_NONE); //,GPIO_MODE_MUX
  pin_config(/*&pin_wire_mi()*/ &devconf[0].wire_mi, GPIO_PULL_NONE); //,GPIO_MODE_MUX
  pin_config(/*&pin_wire_mo()*/ &devconf[0].wire_mo, GPIO_PULL_NONE); //,GPIO_MODE_MUX

  spi_config(); //(spi_port_ptr(_pinCode));
  pin_config(/*&pin_cs()*/ &devconf[0].wire_cs, GPIO_PULL_NONE);
}

#ifdef DM9051_DRIVER_INTERRUPT
#define intr_data_scfg()				((const struct extscfg_st *)&devconf[0].intr_cfg->extend1)
#define intr_gpio_ptr()					((const pin_t *)&devconf[0].intr_cfg->option1.pin)

void intr_add(void) {
	pin_config(intr_gpio_ptr(), GPIO_PULL_UP); //[ops]
	//=intr_gpio_pin_config(GPIO_PULL_UP);
	//log_intr_qpio_pin_config();

	intr_irqline_config(intr_data_scfg(), EXINT_TRIGGER_FALLING_EDGE); //[ops]
  //exint_polarity_config_type pol = EXINT_TRIGGER_FALLING_EDGE;
	
	identify_irq_stat(ISTAT_IRQ_CFG);
	trace_irq_stat(ISTAT_IRQ_CFG);
	identify_irq_stat(ISTAT_LOW_TRIGGER);
	trace_irq_stat(ISTAT_LOW_TRIGGER);
}
#endif
#endif

#if defined (_DLW_M051xx)
/* ------------------------------- NU configuration ----------------------------------------- */
// --------------------- NU ----------------------------
		void DM9051_Configuration_NU(void)
		{
			//NU_DM9051_SPI_Configuration();
			//NU_DM9051_Interript_Configuration();
			spi_add();
			intr_add();
		}

		//---------------------------------------
		void spi_add(void) {
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
		}

		void intr_add(void) {
		#ifdef DM9051_DRIVER_INTERRUPT
			identify_irq_stat(ISTAT_IRQ_CFG);
			trace_irq_stat(ISTAT_IRQ_CFG);
			identify_irq_stat(ISTAT_LOW_TRIGGER);
			trace_irq_stat(ISTAT_LOW_TRIGGER);

			// add user's mcu irq configuration code here.
		#endif
		}
#endif

#define pin_cs()	devconf[0].wire_cs

// --------------------- AT ----------------------------
#if defined (_DLW_AT32F437xx)
void AT_spi_cs_lo(void) {
	gpio_bits_reset(pin_cs().gpio.gpport, pin_cs().gpio.pin);
}
void AT_spi_cs_hi(void) {
	gpio_bits_set(pin_cs().gpio.gpport, pin_cs().gpio.pin);
}

#define spi_number() devconf[0].spidef.spi_num

uint8_t AT_spi_exc_data(uint8_t byte) { 
    while(spi_i2s_flag_get(spi_number(), SPI_I2S_TDBE_FLAG) == RESET);	//while(spi_i2s_flag_get(SPI2, SPI_I2S_TDBE_FLAG) == RESET);
    spi_i2s_data_transmit(spi_number(), byte);							//spi_i2s_data_transmit(SPI2, byte); //spi2 tx
    while(spi_i2s_flag_get(spi_number(), SPI_I2S_RDBF_FLAG) == RESET);	//while(spi_i2s_flag_get(SPI2, SPI_I2S_RDBF_FLAG) == RESET);
    return (uint8_t) spi_i2s_data_receive(spi_number());				//return (uint8_t) spi_i2s_data_receive(SPI2); //spi2 rx (rx_pad)
}
#endif

#if defined (_DLW_M051xx)
// --------------------- NU ----------------------------
//[NU cspi]
		void NU_spi_cs_lo(void) {
			SPI_SET_SS_LOW(SPI1);
		}
		void NU_spi_cs_hi(void) {
			SPI_SET_SS_HIGH(SPI1);
		}

		void NU_spi_exc_data_head(uint8_t byte) { 
				SPI_WRITE_TX0(SPI1, (uint32_t)byte);
				while (SPI_IS_BUSY(SPI1));
				SPI_READ_RX0(SPI1);
		}
		uint8_t NU_spi_exc_data_read(void) { 
			SPI_WRITE_TX0(SPI1, 0x0);           //Dummy for read register value.
			while (SPI_GET_RX_FIFO_EMPTY_FLAG(SPI1));
			return (SPI_READ_RX0(SPI1) & 0xFF);
		}
		void NU_spi_exc_data_read_end(void) { 
				while (SPI_IS_BUSY(SPI1));
				//Clear SPI TX FIFO
				SPI_ClearTxFIFO(SPI1);
		}
		void NU_spi_exc_data_write(uint8_t byte) { 
			while (SPI_GET_TX_FIFO_FULL_FLAG(SPI1));
			SPI_WRITE_TX0(SPI1, (uint32_t)byte);
		}
		void NU_spi_exc_data_write_end(void) { 
				while (SPI_IS_BUSY(SPI1));
				//Clear SPI RX FIFO
				SPI_ClearRxFIFO(SPI1);
		}
#endif

// ---------------------- data_impl -------------------------------------------------------------
#define DM9051_MRCMDX       (0x70)  //Read_Mem2X
#define DM9051_MRCMD        (0x72)  //Read_Mem
#define DM9051_MWCMD        (0x78)  //Write_Mem

#define OPC_REG_W       		0x80  // Register Write
#define OPC_REG_R       		0x00  // Register Read

#if defined (_DLW_AT32F437xx)
//[AT cspi_io]
#define dm9051_spi_command_write(rd)	AT_spi_exc_data(rd)
#define dm9051_spi_dummy_read()				AT_spi_exc_data(0)

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
	for(i=0; i<len; i++)
		buf[i] = dm9051_spi_dummy_read();
}
void AT_spi_mem_write(uint8_t *buf, uint16_t len)
{
	int i;
	dm9051_spi_command_write(DM9051_MWCMD | OPC_REG_W);
	for(i=0; i<len; i++)
		dm9051_spi_command_write(buf[i]);
}
#endif

#if defined (_DLW_M051xx)
//[NU cspi_io]
		#define	dm9051_spi_cmd_write(wb)		NU_spi_exc_data_head(wb)
		#define dm9051_spi_data_read() 			NU_spi_exc_data_read()
		#define	dm9051_spi_read_end()				NU_spi_exc_data_read_end()
		#define	dm9051_spi_data_write(wb)		NU_spi_exc_data_write(wb)
		#define	dm9051_spi_write_end()			NU_spi_exc_data_write_end()

		uint8_t NU_spi_data_read(uint8_t reg)
		{
			uint8_t val;
			dm9051_spi_cmd_write(reg | OPC_REG_R);
			val = dm9051_spi_data_read();
			dm9051_spi_read_end();
			return val;
		}
		void NU_spi_data_write(uint8_t reg, uint8_t val)
		{
			dm9051_spi_cmd_write(reg | OPC_REG_W);
			dm9051_spi_data_write(val);
			dm9051_spi_write_end();
		}

		uint8_t NU_spi_mem2x_read(void)
		{
			uint8_t val;
			dm9051_spi_cmd_write(DM9051_MRCMDX | OPC_REG_R);
			val = dm9051_spi_data_read();
			val = dm9051_spi_data_read();
			dm9051_spi_read_end();
			return val;
		}

		void NU_spi_mem_read(uint8_t *buf, uint16_t len)
		{
			int i;
			dm9051_spi_cmd_write(DM9051_MRCMD | OPC_REG_R);
			for(i=0; i<len; i++)
				buf[i] = dm9051_spi_data_read();
			dm9051_spi_read_end();
		}

		void NU_spi_mem_write(uint8_t *buf, uint16_t len)
		{
			int i;
			dm9051_spi_cmd_write(DM9051_MWCMD | OPC_REG_W);
			for(i=0; i<len; i++)
				dm9051_spi_data_write(buf[i]);
			dm9051_spi_write_end();
		}
#endif

#if defined (_DLW_AT32F437xx)
// --------------------- AT ----------------------------
void AT_cint_disable_mcu_irq(void)
{
#ifdef DM9051_DRIVER_INTERRUPT
  //if (intr_pointer()) {
    const struct extscfg_st *pexint_set = intr_data_scfg(); //exint_scfg_ptr();
    //if (pexint_set) {
	deidentify_irq_stat(ISTAT_IRQ_ENAB);
	nvic_irq_disable(pexint_set->extline.irqn);
	//}
  //}
#endif
}

void AT_cint_enable_mcu_irq(void)
{
#ifdef DM9051_DRIVER_INTERRUPT
  //if (intr_pointer()) {
    const struct extscfg_st *pexint_set = intr_data_scfg(); //exint_scfg_ptr();
    //if (pexint_set) {
	identify_irq_stat(ISTAT_IRQ_ENAB);
	trace_irq_stat(ISTAT_IRQ_ENAB);

	nvic_priority_group_config(pexint_set->extline.priority); //NVIC_PRIORITY_GROUP_0/NVIC_PRIORITY_GROUP_4 // or "NVIC_PRIORITY_GROUP_0"
	nvic_irq_enable(pexint_set->extline.irqn, 1, 0); //nvic_irq_enable(EXINT9_5_IRQn, 1, 0); //i.e.
    //}
  //}
#endif
}

/* IRQ handler support */
void AT_cint_exint9_5_handler(void)
{
	identify_irq_stat(ISTAT_IRQ_NOW);
	trace_irq_stat(ISTAT_IRQ_NOW);

	// add user's mcu irq-handler source code here.
	
	if(exint_flag_get(EXINT_LINE_7) != RESET) {
	
		identify_irq_stat(ISTAT_IRQ_NOW2);
		trace_irq_stat(ISTAT_IRQ_NOW2);
		DM_ETH_ToSet_InterruptEvent();

		exint_flag_clear(EXINT_LINE_7);
	}
	
	deidentify_irq_stat(ISTAT_IRQ_NOW | ISTAT_IRQ_NOW2);
}
#endif

/* dm9051_Hw_common implementation 
 * source code.
 */
#include "dm9051_cboard_data_API.h"

uint8_t cspi_read_reg(uint8_t reg) //static (todo)
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

#if defined (_DLW_AT32F437xx)
#define	spi_datas_read(r,b,l)		AT_spi_datas_read(r,b,l)
#define	spi_datas_write(r,b,l)	AT_spi_datas_write(r,b,l)
void AT_spi_datas_read(uint8_t reg, uint8_t *buf, uint16_t len) {
	int i;
	for(i=0; i < len; i++, reg++) {
		dm9051_spi_command_write(reg | OPC_REG_R);
		buf[i] = dm9051_spi_dummy_read();
	}
}
void AT_spi_datas_write(uint8_t reg, uint8_t *buf, uint16_t len) {
	int i;
	for (i = 0; i < len; i++, reg++) {
		dm9051_spi_command_write(reg | OPC_REG_W);
		dm9051_spi_command_write(buf[i]);
	}
}
#endif
#if defined (_DLW_M051xx)
#define	spi_datas_read(r,b,l)		NU_spi_datas_read(r,b,l)
#define	spi_datas_write(r,b,l)	NU_spi_datas_write(r,b,l)
void NU_spi_datas_read(uint8_t reg, uint8_t *buf, uint16_t len) {
	int i;
	for(i=0; i < len; i++, reg++) {
		dm9051_spi_cmd_write(reg | OPC_REG_R);
		buf[i] = dm9051_spi_data_read();
	}
	dm9051_spi_read_end();
}
void NU_spi_datas_write(uint8_t reg, uint8_t *buf, uint16_t len) {
	int i;
	for(i=0; i < len; i++, reg++) {
		dm9051_spi_cmd_write(reg | OPC_REG_W);
		dm9051_spi_data_write(buf[i]);
	}
	dm9051_spi_write_end();
}
#endif

void cspi_read_regs(uint8_t reg, uint8_t *buf, uint16_t len, csmode_t csmode)
{
	int i;

	if (csmode == CS_LONG) {
		dm9051if_cs_lo();
		spi_datas_read(reg, buf, len);
		dm9051if_cs_hi();
	}
	else { //CS_EACH
	  for(i=0; i < len; i++, reg++)
			buf[i] = cspi_read_reg(reg);
	}
}

void cspi_write_regs(uint8_t reg, uint8_t *buf, uint16_t len, csmode_t csmode)
{
  int i;

  if (csmode == CS_LONG) {
    dm9051if_cs_lo();
		spi_datas_write(reg, buf, len);
    dm9051if_cs_hi();
  }
  else { // CS_EACH
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
