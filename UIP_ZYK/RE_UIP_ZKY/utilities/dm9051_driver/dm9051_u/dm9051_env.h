/**
  **************************************************************************
  * @file     dm9051_env.h
  * @version  v1.0
  * @date     2023-04-28
  * @brief    header file of dm9051 environment config
  **************************************************************************
  */
#ifndef __DM9051_ENV_H
#define __DM9051_ENV_H

#include "dm9051opts.h"
#include "cboard/dm9051_lw_mcu_default_IN.h"
#include "dm9051_lw.h"

#define DM_TYPE		0
#include "dm_types2.h"

/* APIs
 */
const uint8_t *identify_eth_mac(const uint8_t *macadr);
uint8_t *identify_tcpip_ip(uint8_t *ip4adr);
uint8_t *identify_tcpip_gw(uint8_t *ip4adr);
uint8_t *identify_tcpip_mask(uint8_t *ip4adr);

void trace_identified_eth_mac(int showf);

#define	identified_eth_mac()	GET_FIELD(final_mac)
#define	identified_eth_ip()		GET_FIELD(final_ip)
#define	identified_eth_gw()		GET_FIELD(final_gw)
#define	identified_eth_mask()	GET_FIELD(final_mask)
//const uint8_t *identified_eth_mac(void);
//uint8_t *identified_tcpip_ip(void);
//uint8_t *identified_tcpip_gw(void);
//uint8_t *identified_tcpip_mask(void);

int env_init_setup(uint16_t *id);

uint16_t env_evaluate_rxb(uint8_t rxb);
const uint8_t *env_reset_process(const uint8_t *macaddr, enable_t en);
uint16_t env_err_rsthdlr(char *err_explain_str, uint32_t valuecode);

#endif //__DM9051_ENV_H
