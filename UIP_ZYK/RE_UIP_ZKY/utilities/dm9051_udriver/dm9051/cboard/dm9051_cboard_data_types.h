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

	typedef struct spihead_st {
	  char *spi_name;
	  spi_type *spi_num;        		//= SPIPORT;
	  crm_periph_clock_type spi_crm_clk;	//= SPI_CRM_CLK;
	  uint16_t iomux;
	} spihead_t;

	typedef struct gpio_st {
		gpio_type *gpport;        		//= PORT;
		uint16_t pin;           		//= PIN;
		crm_periph_clock_type gp_crmclk;  //= CRM_CLK;
		//const gpio_mux_t *pmux;
	} gpio_t;

	//typedef struct gpio_mmux_st {
	//} gpio_mmux_t;
	typedef struct gpio_mux_st {
		gpio_mode_type gpio_mode;		//= type
		gpio_pins_source_type pinsrc;
		gpio_mux_sel_type muxsel;
	} gpio_mux_t;

	typedef struct pin_st {
			const gpio_t			gpio;
			const gpio_mux_t	*mux;
	} pin_t;

	typedef struct gp_set_st {
		const char *gp_info;
		const pin_t pin;
		//const gpio_t gpio; //gp;
		//const gpio_mux_t	*mux;
	} gp_set_t;

	typedef struct extint_init_st {
		crm_periph_clock_type scfg_clk;
	#if !defined(AT32F437xx)
		gpio_port_source_type scfg_port_src;
		gpio_pins_source_type scfg_pin_src;
	#elif defined(AT32F437xx)
		scfg_port_source_type scfg_port_src; //SCFG_PORT_SOURCE_X
		scfg_pins_source_type scfg_pin_src; //SCFG_PINS_SOURCEX
	#endif
	} extint_init_t;

	struct extscfg_st { //struct linescfg_st
		const char *irq_enable_inf;
		struct extline_st {
			crm_periph_clock_type intr_crm_clk; //CRM_GPIOC_PERIPH_CLOCK,
			uint32_t extline; //= LINE
			IRQn_Type irqn; //= EXINTn_m
			nvic_priority_group_type priority; //_group;
		} extline;
	};

	struct modscfg_st {
		const char *scfg_inf;
		struct extint_init_st scfg_init; //extint_init_t
		struct extscfg_st extend1; //struct linescfg_st *extend; //essential
		struct gp_set_st option1; //gp_set_t
	};

	struct spi_dev_t {
		char *info;
		spihead_t spidef;
		char *cpu_spi_info;	//cpu name is for the purpose to lead the pins, for easy recogition!
		pin_t wire_sck;
		pin_t wire_mi;
		pin_t wire_mo;
		char *cpu_cs_info;
		pin_t wire_cs;
		const struct modscfg_st *intr_cfg;
	};

	typedef enum {
		CS_EACH = 0,
		CS_LONG,
	} csmode_t;
#endif
#endif
#endif //__DM9051_CBOARD_DT_H__
	
