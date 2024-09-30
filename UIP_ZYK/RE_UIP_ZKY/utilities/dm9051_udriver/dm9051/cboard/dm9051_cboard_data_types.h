/*dm9051_cboard_data_types.h
 */
#ifndef __DM9051_CBOARD_DT_H__
#define __DM9051_CBOARD_DT_H__

//#include "dm9051opts.h"
//#include "cboard/dm9051_lw_mcu_default_IN.h"
//#include "dm9051_lw.h"

#if defined (_DLW_AT32F437xx)
#if 1
//[AT cspi]
	#define  IO_MUX_NULL                    	((uint16_t)0x0000) /*!< subordinate  */
	#define  IO_MUX_PINREMAP                 	((uint16_t)0x0001) /*!< subordinate mode a (such as f413)*/
	#define  IO_MUX_GPIOMUX                 	((uint16_t)0x0002) /*!< subordinate mode b (such as f437) */
	#define  IO_CRM_CLOCK                 		((uint16_t)0x0100) /*!< subordinate mode c (such as f413 clock) */
	
	struct pin_t;

	typedef struct gpio_st {
		gpio_type *gpport;        		//= PORT;
		uint16_t pin;           		//= PIN;
		crm_periph_clock_type gp_crmclk;  //= CRM_CLK;
		//const gpio_mux_t *pmux;
	} gpio_t;

	typedef struct gpio_mux_st {
		gpio_mode_type gpio_mode;		//= type
		gpio_pins_source_type pinsrc;
		gpio_mux_sel_type muxsel;
	} gpio_mux_t;

	struct pin_t {
			const gpio_t			gpio;
			const gpio_mux_t	*mux;
	};

	typedef struct spihead_st {
	  char *spi_name;
	  spi_type *spi_num;        		//= SPIPORT;
	  crm_periph_clock_type spi_crm_clk;	//= SPI_CRM_CLK;
	  uint16_t iomux;
	} spihead_t;

	typedef struct gp_set_st {
		const char *gp_info;
		const struct pin_t pin;
		//const gpio_t gpio; //gp;
		//const gpio_mux_t	*mux;
	} gp_set_t;

	struct extint_init_t {
		crm_periph_clock_type scfg_clk;
	#if !defined(AT32F437xx)
		gpio_port_source_type scfg_port_src;
		gpio_pins_source_type scfg_pin_src;
	#elif defined(AT32F437xx)
		scfg_port_source_type scfg_port_src; //SCFG_PORT_SOURCE_X
		scfg_pins_source_type scfg_pin_src; //SCFG_PINS_SOURCEX
	#endif
	};

	struct extscfg_st { //struct linescfg_st
		const char *irq_enable_inf;
		struct extline_ck_t {
			crm_periph_clock_type scfg_clk;
			scfg_port_source_type scfg_port_src; //SCFG_PORT_SOURCE_X
			scfg_pins_source_type scfg_pin_src; //SCFG_PINS_SOURCEX
		} intr_ck;
		struct extline_t {
			crm_periph_clock_type intr_crm_clk; //CRM_GPIOC_PERIPH_CLOCK,
			uint32_t extline; //= LINE
			IRQn_Type irqn; //= EXINTn_m
			nvic_priority_group_type priority; //_group;
		} extline;
	};

	struct modscfg_st {
		const char *scfg_inf;
		struct extint_init_t scfg_init; //extint_init_t
		const char *scfg_inf0;
		struct extscfg_st extend1; //struct linescfg_st *extend; //essential
		const char *scfg_inf1;
		struct gp_set_st option1; //gp_set_t
	};

	struct spi_dev_t {
		char *info;
		spihead_t spidef;
		char *cpu_spi_info;	//cpu name is for the purpose to lead the pins, for easy recogition!
		struct pin_t wire_sck;
		struct pin_t wire_mi;
		struct pin_t wire_mo;
		char *cpu_cs_info;
		struct pin_t wire_cs;
		//const struct modscfg_st *intr_cfg;
	};

	typedef enum {
		CS_EACH = 0,
		CS_LONG,
	} csmode_t;
	
	void spi_add(void);
	void intr_add(void);
	
	void pin_config(const struct pin_t *pin, gpio_pull_type gppull);
	void spi_config(void);
	void intr_gpio_pin_config(const struct pin_t *pin, gpio_pull_type pull);
	void intr_irqline_config(const struct extscfg_st *pexint_set, exint_polarity_config_type polarity);
	void log_intr_qpio_pin_config(void);
#endif
#endif
#endif //__DM9051_CBOARD_DT_H__
	
