#ifndef __DM9051_OPTS_H
#define __DM9051_OPTS_H
#include "stdint.h"
#include <string.h>

#define	ETHERNET_INTERRUPT_MODE							1 // Please keep define to 1, INT or POLLING by _data.h
#define DM_ETH_DEBUG_MODE										1 // 0
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

//[API]uip dm9051.h
const uint8_t *dm9051_init(const uint8_t *adr);
uint16_t dm9051_rx(uint8_t *buff);
void dm9051_tx(uint8_t *buf, uint16_t len);

//[API]lwip dm9051.h
uint16_t dm9051_read_chip_id(void);
void dm9051_read_rx_pointers(uint16_t *rwpa_wt, uint16_t *mdra_rd);
uint16_t dm9051_read_bmsr(void);
uint16_t dm9051_read_control_status(void);
void dm9051_probe_link(int nsr_poll);
void dm9051_write_rst_isr(void);

#endif //__DM9051_OPTS_H
