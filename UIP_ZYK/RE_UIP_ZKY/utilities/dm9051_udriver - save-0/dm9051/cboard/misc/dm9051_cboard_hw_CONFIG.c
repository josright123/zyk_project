#include "dm9051_cboard_data_types.h"
#include "dm9051_cstate.h"
#include "../dm_eth.h"

#if defined (_DLW_AT32F437xx)
// --------------------- AT ----------------------------
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
 #ifdef AT32F437xx
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

void spi_config(void)
{
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
#if 0
  spi_init_type spi_init_struct;
  crm_periph_clock_enable(spi_crm(), TRUE); //crm_spi_clk(_pinCode), CRM_SPI3_PERIPH_CLOCK/CRM_SPI2_PERIPH_CLOCK

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
  spi_init(spi_number(), &spi_init_struct); //SPI2, _spi_num(_pinCode)
  spi_enable(spi_number(), TRUE); //SPI2, _spi_num(_pinCode)
#endif
}

	#ifdef DM9051_DRIVER_INTERRUPT
	//[cint]
	#define scfg_crm()					PTR_EXINTD(scfg_init.scfg_clk)
	#define scfg_port()					PTR_EXINTD(scfg_init.scfg_port_src)
	#define scfg_pin()					PTR_EXINTD(scfg_init.scfg_pin_src)
	#define exint_crm()					PTR_EXINTD(extend1.extline.intr_crm_clk)
	#endif
	
//[cint]
#ifdef DM9051_DRIVER_INTERRUPT
/* @brief  exint gpio
 */
//static 
void intr_gpio_pin_config(const pin_t *pin, gpio_pull_type pull) {
	pin_config(pin, pull); //=from gpio_pin_config(intr_gpio_ptr(), pull);
}

/* @brief  exint irqline
 */
//static 
void intr_irqline_config(const struct extscfg_st *pexint_set, exint_polarity_config_type polarity)
{
  exint_init_type exint_init_struct;

  crm_periph_clock_enable(scfg_crm(), TRUE);  // CRM_SCFG_PERIPH_CLOCK
  crm_periph_clock_enable(exint_crm(), TRUE); // CRM_GPIOC_PERIPH_CLOCK

#ifndef AT32F437xx
  gpio_exint_line_config(scfg_port(), scfg_pin()); // SCFG_PORT_SOURCE_GPIOA, SCFG_PINS_SOURCE0
#else
  scfg_exint_line_config(scfg_port(), scfg_pin()); // SCFG_PORT_SOURCE_GPIOC, SCFG_PINS_SOURCE7
#endif

  exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_enable = TRUE;

  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_select = pexint_set->extline.extline; // line_no;
  exint_init_struct.line_polarity = polarity;                  // EXINT_TRIGGER_RISING_EDGE/ EXINT_TRIGGER_FALLING_EDGE
  exint_init(&exint_init_struct);
}

//static void exint_config(const struct extscfg_st *pexint_set, exint_polarity_config_type pol) {
//      intr_irqline_config(pexint_set, pol);
//}

int de_enum(uint32_t data) {
	int n;
	
	for (n = 0; n < 16; n++)
		if (data & (1 << n))
			return n;
	return 999; //? unknow
}
int de_pin(const pin_t *pin) {
	return de_enum(pin->gpio.pin); //check uint16_t as to uint32_t is possible.
}
char *de_port_str(const pin_t *pin) {
	if (pin->gpio.gpport == GPIOA)
		return "GPIOA";
	if (pin->gpio.gpport == GPIOB)
		return "GPIOB";
	if (pin->gpio.gpport == GPIOC)
		return "GPIOC";
	if (pin->gpio.gpport == GPIOD)
		return "GPIOD";
	if (pin->gpio.gpport == GPIOE)
		return "GPIOE";
	if (pin->gpio.gpport == GPIOF)
		return "GPIOF";
	if (pin->gpio.gpport == GPIOG)
		return "GPIOG";
	if (pin->gpio.gpport == GPIOH)
		return "GPIOH";
	return "GPIO_UNKNOW";
}
#endif

void log_intr_qpio_pin_config(void) {
#ifdef DM9051_DRIVER_INTERRUPT
	printf("\r\n"); //printk("\r\n");
	//printf("Intr pin_config: %s, GPIO_PINS_%d\r\n", de_port_str(intr_gpio_ptr()), de_pin(intr_gpio_ptr())); //" %d", de_pin(&intr_gpio_ptr())
	//printf("scfg_config: %s\r\n", scfg_info());
	printf("interrupt pin: %s\r\n", scfg_info());
	//printf("\r\n"); //printk("\r\n");
#endif
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

// --------------------- AT ----------------------------
#if defined (_DLW_AT32F437xx)
#endif

#if defined (_DLW_AT32F437xx)
// --------------------- AT ----------------------------
//[AT cspi]
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
#endif

#if defined (_DLW_M051xx)
// --------------------- NU ----------------------------
void NU_cint_disable_mcu_irq(void)
{
#ifdef DM9051_DRIVER_INTERRUPT
	deidentify_irq_stat(ISTAT_IRQ_ENAB);
	
	// add user's mcu irq enable control code here.

#endif
}

void NU_cint_enable_mcu_irq(void)
{
#ifdef DM9051_DRIVER_INTERRUPT
	identify_irq_stat(ISTAT_IRQ_ENAB);
	trace_irq_stat(ISTAT_IRQ_ENAB);
	
	// add user's mcu irq enable control code here.

#endif
}

/* IRQ handler support */
void NU_cint_exint9_5_handler(void)
{
	
	identify_irq_stat(ISTAT_IRQ_NOW);
	trace_irq_stat(ISTAT_IRQ_NOW);

	// add user's mcu irq-handler source code here.
	
	//if(exint_flag_get(EXINT_LINE_7) != RESET) {
	
		identify_irq_stat(ISTAT_IRQ_NOW2);
		trace_irq_stat(ISTAT_IRQ_NOW2);
		DM_ETH_ToSet_InterruptEvent();

	//	exint_flag_clear(EXINT_LINE_7);
	//}
	
	deidentify_irq_stat(ISTAT_IRQ_NOW | ISTAT_IRQ_NOW2);
}
#endif

// --------------------- AT ----------------------------
#if defined (_DLW_AT32F437xx)
void cint_disable_mcu_irq(void)
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

void cint_enable_mcu_irq(void)
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
			DM_ETH_ToSet_InterruptEvent();
	
			exint_flag_clear(EXINT_LINE_7);
		}
//			exint_flag_clear(exint_line);
//		}
	
	deidentify_irq_stat(ISTAT_IRQ_NOW | ISTAT_IRQ_NOW2);
}
#endif

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
