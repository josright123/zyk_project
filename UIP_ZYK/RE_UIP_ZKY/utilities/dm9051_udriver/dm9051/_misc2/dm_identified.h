#ifndef __DM9051_ENVIDNFED_H
#define __DM9051_ENVIDNFED_H

/* APIs
 */
#define identified_eth_mac() GET_FIELD(final_mac)
#define identified_eth_ip() GET_FIELD(final_ip)
#define identified_eth_gw() GET_FIELD(final_gw)
#define identified_eth_mask() GET_FIELD(final_mask)
// const uint8_t *identified_eth_mac(void);
// uint8_t *identified_tcpip_ip(void);
// uint8_t *identified_tcpip_gw(void);
// uint8_t *identified_tcpip_mask(void);

/* irqstate
 */
uint16_t identified_irq_stat(void);

#endif //__DM9051_ENVIDNFED_H
