#if HELLO_DRIVER_INTERNAL && 0
/* Sanity3.
 */
//typedef enum {
//} iomux_t;
#define  IO_MUX_NULL                    	((uint16_t)0x0000) /*!< subordinate  */
#define  IO_MUX_PINREMAP                 	((uint16_t)0x0001) /*!< subordinate mode a (such as f413)*/
#define  IO_MUX_GPIOMUX                 	((uint16_t)0x0002) /*!< subordinate mode b (such as f437) */
#define  IO_CRM_CLOCK                 		((uint16_t)0x0100) /*!< subordinate mode c (such as f413 clock) */

typedef struct spihead_sel_st {
  char *spi_name;
  spi_type *spi_num;        		//= SPIPORT;
  crm_periph_clock_type spi_crm_clk;	//= SPI_CRM_CLK;
  uint16_t iomux;
} spihead_t;

#define GPIO_MODE_NULL		(gpio_mode_type)		0x0F
#define GPIO_PINSRC_NULL 	(gpio_pins_source_type)	0x0F //0
#define GPIO_MUX_NULL    	(gpio_mux_sel_type)		0x0F //0

typedef struct gpio_mux_st {
	gpio_mode_type gpio_mode;		//= type
	gpio_pins_source_type pinsrc;
	gpio_mux_sel_type muxsel;
} gpio_mux_t;

typedef struct gpio_sel_st {
	gpio_type *gpport;        		//= PORT;
	uint16_t pin;           		//= PIN;
	crm_periph_clock_type gp_crmclk;  //= CRM_CLK;
//	gpio_mode_type gpio_mode;
//	gpio_pins_source_type pinsrc;
//	gpio_mux_sel_type muxsel;
	const gpio_mux_t *pmux;
	uint16_t iomux_to_instead; //instead of 'iomux'
} gpio_t;

/*
 * interrupt,
 *  modscfg_st =
 *    struct extint_init_st
 *    struct extscfg_st *
 *      struct extline_st
 *    struct gp_set_st *
 */
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

typedef struct gp_set_st {
	const char *gp_info;
	const gpio_t gp;
} gp_set_t;

struct modscfg_st {
	const char *scfg_inf;
	struct extint_init_st scfg_init; //extint_init_t
	struct extscfg_st *extend; //struct linescfg_st *extend; //essential
	struct gp_set_st *option; //gp_set_t
};

typedef struct {
	char *info;
	spihead_t spidef;
	char *cpu_spi_info;	//cpu name is for the purpose to lead the pins, for easy recogition!
	gpio_t wire_sck;
	gpio_t wire_mi;
	gpio_t wire_mo;
	char *cpu_cs_info;
	gpio_t wire_cs;
	const struct modscfg_st *intr_cfg;
} spi_dev_t;

// -

extern const struct modscfg_st devconf_at437_intr_a0;
extern const struct modscfg_st devconf_at437_intr_c7;
extern const struct modscfg_st devconf_at403a_intr_c6;
extern const struct modscfg_st devconf_at403a_intr_c7;

extern const spi_dev_t devconf[BOARD_SPI_COUNT];
//extern int pin_code;

#endif //HELLO_DRIVER_INTERNAL
