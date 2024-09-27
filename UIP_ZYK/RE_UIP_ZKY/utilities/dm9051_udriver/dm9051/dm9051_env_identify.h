/**
  **************************************************************************
  * @file     dm9051_env_identify.h
  * @version  v1.0
  * @date     2024-09-27
  * @brief    header file of dm9051 environment identify
  **************************************************************************
  */
#ifndef __DM9051_ENVIDN_H
#define __DM9051_ENVIDN_H

#define DM_TYPE		0
#include "dm_identify_types.h"

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

#endif //__DM9051_ENVIDN_H
