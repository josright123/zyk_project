#ifndef __DM_IDEN_API_H__
#define __DM_IDEN_API_H__
#include "control/dm9051opts.h"

void inc_interrupt_count(void); //inc_task_tryint(void);
unsigned long get_interrupt_count(void);

#if DM_ETH_DEBUG_MODE
uint16_t wrpadiff(uint16_t rwpa_s, uint16_t rwpa_e);
void debug_diff_rx_pointers(int state, uint16_t rd_now);
void dm_eth_input_hexdump_reset(void);
void dm_eth_input_hexdump(const void *buf, size_t len);
#endif

/*
 * HCC: Hard Core Candidate (hcc)
 */

extern const struct eth_node_t node_candidate[1];

/* APIs.identify
 */
#define candidate_eth_mac() &node_candidate[0].mac_addresse[0]    //[pin_code]
#define candidate_eth_ip() &node_candidate[0].local_ipaddr[0]     //[pin_code]
#define candidate_eth_gw() &node_candidate[0].local_gwaddr[0]     //[pin_code]
#define candidate_eth_mask() &node_candidate[0].local_maskaddr[0] //[pin_code]
// const uint8_t *identify_eth_mac(const uint8_t *macadr);
// uint8_t *identify_tcpip_ip(uint8_t *ip4adr);
// uint8_t *identify_tcpip_gw(uint8_t *ip4adr);
// uint8_t *identify_tcpip_mask(uint8_t *ip4adr);
// void trace_identify_eth_mac(void);
#define identify_eth_mac(macadr) SET_FIELD(final_mac, macadr ? macadr : candidate_eth_mac())
#define identify_tcpip_ip(ip4adr) SET_FIELD(final_ip, ip4adr ? ip4adr : candidate_eth_ip())
#define identify_tcpip_gw(ip4adr) SET_FIELD(final_gw, ip4adr ? ip4adr : candidate_eth_gw())
#define identify_tcpip_mask(ip4adr) SET_FIELD(final_mask, ip4adr ? ip4adr : candidate_eth_mask())
#define trace_identify_eth_mac()                            \
  do                                                        \
  {                                                         \
    const uint8_t *mac = GET_FIELD(final_mac);              \
    printf("mac address %02x%02x%02x%02x%02x%02x\r\n",      \
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]); \
  } while (0)

/* APIs.identified
 */
// const uint8_t *identified_eth_mac(void);
// uint8_t *identified_tcpip_ip(void);
// uint8_t *identified_tcpip_gw(void);
// uint8_t *identified_tcpip_mask(void);
#define identified_eth_mac() GET_FIELD(final_mac)
#define identified_tcpip_ip() GET_FIELD(final_ip)
#define identified_tcpip_gw() GET_FIELD(final_gw)
#define identified_tcpip_mask() GET_FIELD(final_mask)

#endif //__DM_IDEN_API_H__
