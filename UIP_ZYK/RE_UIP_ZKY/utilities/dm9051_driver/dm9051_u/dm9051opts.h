#ifndef __DM9051_OPTS_H
#define __DM9051_OPTS_H
#include "stdint.h"
#include <string.h>

#define	ETHERNET_INTERRUPT_MODE							1 // Please keep define to 1, INT or POLLING by _data.h
#define DM_ETH_DEBUG_MODE										0
#define qprint															0 //1 // on/off

#include "dm9051_lw_usr_default.h"
#include "dm9051_lw_driver_define.h"

typedef enum {
  DM_FALSE = 0,
  DM_TRUE = !DM_FALSE,
} enable_t;

#define DM_UNUSED_ARG(x)						(void)x

#define DM9051_RX_BREAK(expression, handler) do { if ((expression)) { \
  handler;}} while(0)

//#define DM9051_HEXDUMP_RESET(expression, handler) do { if ((expression)) { \
//	(handler);}} while(0)

//[API]
const uint8_t *dm9051_init(const uint8_t *adr);
uint16_t dm9051_rx(uint8_t *buff);
void dm9051_tx(uint8_t *buf, uint16_t len);

int link_flag(void);
void dm9051_probe_link(int nsr_poll);
void impl_dm9051_set_recv(void);

//void lwip_set_mac_address_IN_DRV(const uint8_t* macadd);
uint16_t env_evaluate_rxb(uint8_t rxb);
uint16_t env_err_rsthdlr(char *err_explain_str, uint32_t valuecode);
uint16_t evaluate_link(void);

void spi_add(void);
void intr_add(void);
	
unsigned long get_fire_polling(void);

#endif //__DM9051_OPTS_H
