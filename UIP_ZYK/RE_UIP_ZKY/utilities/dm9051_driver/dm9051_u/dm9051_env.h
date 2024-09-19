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

/* APIs
 */
const uint8_t *identify_eth_mac(const uint8_t *macadr, int showflg);
uint8_t *identified_eth_mac(void);
uint8_t *identify_tcpip_ip(uint8_t *ip4adr);
uint8_t *identify_tcpip_gw(uint8_t *ip4adr);
uint8_t *identify_tcpip_mask(uint8_t *ip4adr);
uint8_t *identified_tcpip_ip(void);
uint8_t *identified_tcpip_gw(void);
uint8_t *identified_tcpip_mask(void);

int env_init_setup(uint16_t *id);

const uint8_t *env_reset_process(const uint8_t *macaddr, enable_t en);
	
void sprint_hex_dump0(int head_space, int titledn, char *prefix_str,
			size_t tlen, int rowsize, const void *buf, int seg_start, size_t len, /*int useflg*/ int cast_lf);

#endif //__DM9051_ENV_H
