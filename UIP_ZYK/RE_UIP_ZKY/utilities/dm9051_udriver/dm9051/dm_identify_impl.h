/**
 **************************************************************************
 * @file     _dm9051_env_identify_impl.h
 * @version  v1.0
 * @date     2024-09-27
 * @brief    header file of dm9051 environment identify
 **************************************************************************
 */
#ifndef __DM9051_ENVIDN_IMPL_H
#define __DM9051_ENVIDN_IMPL_H

/* APIs
 */
#define DM_TYPE 0
#include "dm_identify_types_define.h"

/* APIs.identify
 */
const uint8_t *identify_eth_mac(const uint8_t *macadr);
uint8_t *identify_tcpip_ip(uint8_t *ip4adr);
uint8_t *identify_tcpip_gw(uint8_t *ip4adr);
uint8_t *identify_tcpip_mask(uint8_t *ip4adr);
void trace_identified_eth_mac(int showf);

/* irqstate.identify
 */
#define ISTAT_IRQ_CFG (1 << 0)
#define ISTAT_IRQ_ENAB (1 << 1)
#define ISTAT_DM_IMR (1 << 2)
#define ISTAT_DM_RCR (1 << 3)

#define ISTAT_LOW_TRIGGER (1 << 4)
#define ISTAT_LOW_ACTIVE (1 << 5)

#define ISTAT_IRQ_NOW2 (1 << 6)
#define ISTAT_IRQ_NOW (1 << 7)
void identify_irq_stat(uint16_t bitflg);
void deidentify_irq_stat(uint16_t bitflg);
void trace_irq_stat(uint16_t bitflg);

/* APIs.identified
 */
// const uint8_t *identified_eth_mac(void);
// uint8_t *identified_tcpip_ip(void);
// uint8_t *identified_tcpip_gw(void);
// uint8_t *identified_tcpip_mask(void);
#define identified_eth_mac() GET_FIELD(final_mac)
#define identified_eth_ip() GET_FIELD(final_ip)
#define identified_eth_gw() GET_FIELD(final_gw)
#define identified_eth_mask() GET_FIELD(final_mask)

/* irqstate.identified
 */
//uint16_t identified_irq_stat(void);
#define	identified_irq_stat()	GET_CSTATE(irqst)

#endif //__DM9051_ENVIDN_IMPL_H
