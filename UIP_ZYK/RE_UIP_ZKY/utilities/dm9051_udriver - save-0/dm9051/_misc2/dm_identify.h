/**
 **************************************************************************
 * @file     _dm9051_env_identify.h
 * @version  v1.0
 * @date     2024-09-27
 * @brief    header file of dm9051 environment identify
 **************************************************************************
 */
#ifndef __DM9051_ENVIDN_H
#define __DM9051_ENVIDN_H

/* APIs
 */
const uint8_t *identify_eth_mac(const uint8_t *macadr);
uint8_t *identify_tcpip_ip(uint8_t *ip4adr);
uint8_t *identify_tcpip_gw(uint8_t *ip4adr);
uint8_t *identify_tcpip_mask(uint8_t *ip4adr);
void trace_identified_eth_mac(int showf);

/* irqstate
 */
void identify_irq_stat(uint16_t bitflg);
void deidentify_irq_stat(uint16_t bitflg);
void trace_irq_stat(uint16_t bitflg);

#endif //__DM9051_ENVIDN_H
