#include "dm9051_cboard_data_API.h" //#include "dm9051_cboard_data_types.h"
#include "dm9051_cstate.h"
#include "../dm_eth.h"

#if defined (_DLW_AT32F437xx)
/* ------------------------------- AT configuration ----------------------------------------- */
// --------------------- AT ----------------------------
void dm9051_boards_initialize_AT(void)
{
	spi_add();
	#ifdef DM9051_DRIVER_INTERRUPT
	intr_add();
	#endif
}

	struct spi_pin_t {
		gpio_type *gpport;        		//= PORT;
		uint16_t pin;           		//= PIN;
		
		gpio_pins_source_type pinsrc;
		gpio_mux_sel_type muxsel;
	};
	struct spi_pin_set_t {
		crm_periph_clock_type gp_crmclk;
		gpio_mode_type gpio_mode;
		struct spi_pin_t gpio_pin;
	};

	struct spi_set_t {

	  //char *spi_name;
	  spi_type *spi_num;        		//= SPIPORT;
	  crm_periph_clock_type spi_crm_clk;	//= SPI_CRM_CLK;
		struct spi_pin_set_t pin_sck;
		struct spi_pin_set_t pin_miso;
		struct spi_pin_set_t pin_mosi;
		struct spi_pin_set_t pin_cs; /* //CS (e.g. PA15) */
		
	};
	
	struct spi_set_t spi_set = {
		
		SPI1,
		CRM_SPI1_PERIPH_CLOCK,
		{CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_MUX, {GPIOA, GPIO_PINS_5, GPIO_PINS_SOURCE5, GPIO_MUX_5, }, },
		{CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_MUX, {GPIOA, GPIO_PINS_6, GPIO_PINS_SOURCE6, GPIO_MUX_5}, },
		{CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_MUX, {GPIOA, GPIO_PINS_7, GPIO_PINS_SOURCE7, GPIO_MUX_5}, },
		{CRM_GPIOA_PERIPH_CLOCK, GPIO_MODE_OUTPUT, {GPIOA, GPIO_PINS_15, }, },

	};

static void gp_config(struct spi_pin_set_t *ps, gpio_pull_type gppull) {
  gpio_init_type gpio_init_struct;

  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_out_type  		= GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_drive_strength	= GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_mode			= ps->gpio_mode;

  gpio_init_struct.gpio_pull			= gppull; //GPIO_PULL_DOWN; GPIO_PULL_UP; //GPIO_PULL_NONE;
  gpio_init_struct.gpio_pins			= ps->gpio_pin.pin;
  gpio_init(ps->gpio_pin.gpport, &gpio_init_struct);
}

static void gp_mux(struct spi_pin_set_t *ps) {
 #if defined(_DLW_AT32F437xx)
		gpio_pin_mux_config(ps->gpio_pin.gpport, ps->gpio_pin.pinsrc, ps->gpio_pin.muxsel);
 #endif
}

static void spi_config(struct spi_set_t *ss) {
  spi_init_type spi_init_struct;

  crm_periph_clock_enable(
		ss->spi_crm_clk,
		TRUE);
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
  spi_init(
		ss->spi_num,
		&spi_init_struct);
  spi_enable(
		ss->spi_num,
		TRUE);
}

static void pin_cfg(struct spi_pin_set_t *ps, gpio_pull_type gppull) {
	/* enable the gpioa clock 
	 */
	crm_periph_clock_enable(ps->gp_crmclk, TRUE);
	gp_config(ps, gppull);
	if (ps->gpio_mode == GPIO_MODE_MUX)
		gp_mux(ps);
}

void spi_add(void) {
  spi_config(&spi_set);
	pin_cfg(&spi_set.pin_sck, GPIO_PULL_NONE);
	pin_cfg(&spi_set.pin_miso, GPIO_PULL_NONE);
	pin_cfg(&spi_set.pin_mosi, GPIO_PULL_NONE);
	pin_cfg(&spi_set.pin_cs, GPIO_PULL_NONE);
}

	struct irq_cfg_t {
		crm_periph_clock_type scfg_clk; //CRM_SCFG_PERIPH_CLOCK,
		scfg_port_source_type scfg_port_src; //SCFG_PORT_SOURCE_X
		scfg_pins_source_type scfg_pin_src; //SCFG_PINS_SOURCEX
	};
//	struct irq_line_t {
//		crm_periph_clock_type intr_crm_clk; //CRM_GPIOC_PERIPH_CLOCK,
//		uint32_t extline; //= LINE
//	};

	struct spi_intr_t {

	  //char *intr_gpio_name;
		//struct irq_line_t ln;
		crm_periph_clock_type intr_crm_clk; //CRM_GPIOC_PERIPH_CLOCK,
		uint32_t extline; //= LINE
		struct irq_cfg_t irq;
		struct spi_pin_set_t pin_intr; /* //INTR (e.g. PC7) */
		IRQn_Type irqn; //= EXINTn_m
		nvic_priority_group_type priority; //_group;

	};
	struct spi_intr_t spi_intr[1] = {
		
		//"extline pc7",
		CRM_GPIOC_PERIPH_CLOCK,
		EXINT_LINE_7,
		{CRM_SCFG_PERIPH_CLOCK, SCFG_PORT_SOURCE_GPIOC, SCFG_PINS_SOURCE7 },
		{CRM_GPIOC_PERIPH_CLOCK, GPIO_MODE_INPUT, {GPIOC, GPIO_PINS_7 } },
		EXINT9_5_IRQn,
		NVIC_PRIORITY_GROUP_0,

	};

static void intr_irqline_config(exint_polarity_config_type polarity) { //const struct extscfg_st *pexint_set, 
  exint_init_type exint_init_struct;

  crm_periph_clock_enable(spi_intr[0].irq.scfg_clk, TRUE); //src_intr_c7[0].intr_ck.scfg_clk
  crm_periph_clock_enable(spi_intr[0].intr_crm_clk, TRUE); //src_intr_c7[0].extline.intr_crm_clk, 
	
//#ifndef AT32F437xx
//  gpio_exint_line_config(scfg_port(), scfg_pin()); // SCFG_PORT_SOURCE_GPIOA, SCFG_PINS_SOURCE0
//#else
//#endif
  scfg_exint_line_config( // SCFG_PORT_SOURCE_GPIOC, SCFG_PINS_SOURCE7
		spi_intr[0].irq.scfg_port_src,
		spi_intr[0].irq.scfg_pin_src
		);

  exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_enable = TRUE;

  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_select = spi_intr[0].extline; //src_intr_c7[0].extline.extline; //pexint_set->extline.extline; // line_no;
  exint_init_struct.line_polarity = polarity; // EXINT_TRIGGER_RISING_EDGE/ EXINT_TRIGGER_FALLING_EDGE
  exint_init(&exint_init_struct);
}

#ifdef DM9051_DRIVER_INTERRUPT

void intr_add(void) {
	//log_intr_qpio_pin_config();
	pin_cfg(&spi_intr[0].pin_intr, GPIO_PULL_UP);

	intr_irqline_config(EXINT_TRIGGER_FALLING_EDGE); //[ops] //&devconf_at437_intr_c7.extend1, 
	
	identify_irq_stat(ISTAT_IRQ_CFG);
	trace_irq_stat(ISTAT_IRQ_CFG);
	identify_irq_stat(ISTAT_LOW_TRIGGER);
	trace_irq_stat(ISTAT_LOW_TRIGGER);
}
#endif //DM9051_DRIVER_INTERRUPT
#endif //_DLW_AT32F437xx

#if defined (_DLW_AT32F437xx)
// --------------------- AT ----------------------------
void AT_cint_disable_mcu_irq(void)
{
#ifdef DM9051_DRIVER_INTERRUPT
	deidentify_irq_stat(ISTAT_IRQ_ENAB);
	nvic_irq_disable(spi_intr[0].irqn); //(devconf_at437_intr_c7.extend1.extline.irqn);
#endif
}

void AT_cint_enable_mcu_irq(void)
{
#ifdef DM9051_DRIVER_INTERRUPT
	identify_irq_stat(ISTAT_IRQ_ENAB);
	trace_irq_stat(ISTAT_IRQ_ENAB);

	nvic_priority_group_config(spi_intr[0].priority); //devconf_at437_intr_c7.extend1.extline.priority //NVIC_PRIORITY_GROUP_0/NVIC_PRIORITY_GROUP_4 // or "NVIC_PRIORITY_GROUP_0"
	nvic_irq_enable(spi_intr[0].irqn, 1, 0); //devconf_at437_intr_c7.extend1.extline.irqn
#endif
}

/* IRQ handler support */
void AT_cint_exint9_5_handler(void)
{
	identify_irq_stat(ISTAT_IRQ_NOW);
	trace_irq_stat(ISTAT_IRQ_NOW);

	// add user's mcu irq-handler source code here.
	
	if(exint_flag_get(spi_intr[0].extline) != RESET) {
	
		identify_irq_stat(ISTAT_IRQ_NOW2);
		trace_irq_stat(ISTAT_IRQ_NOW2);
		DM_ETH_ToSet_InterruptEvent();

		exint_flag_clear(spi_intr[0].extline);
	}
	
	deidentify_irq_stat(ISTAT_IRQ_NOW | ISTAT_IRQ_NOW2);
}
#endif

#if defined (_DLW_M051xx)
/* ------------------------------- NU configuration ----------------------------------------- */
// --------------------- NU ----------------------------
#endif

#define pin_cs()	spi_set.pin_cs.gpio_pin //devconf[0].wire_cs

// --------------------- AT ----------------------------
#if defined (_DLW_AT32F437xx)
void AT_spi_cs_lo(void) {
	gpio_bits_reset(pin_cs().gpport, pin_cs().pin);
}
void AT_spi_cs_hi(void) {
	gpio_bits_set(pin_cs().gpport, pin_cs().pin);
}

#define spi_number() spi_set.spi_num //devconf[0].spidef.spi_num

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

/* dm9051_Hw_common implementation 
 * source code.
 */
//#include "dm9051_cboard_data_API.h"

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
