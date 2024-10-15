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

void inc_interrupt_count(void); //inc_task_tryint(void);
unsigned long get_task_tryint(void);

/* APIs
 */
#define DM_TYPE 0
#include "dm_identify_types_define.h"

struct eth_node_t
{
	uint8_t mac_addresse[MAC_ADDR_LENGTH];
	uint8_t local_ipaddr[ADDR_LENGTH];
	uint8_t local_gwaddr[ADDR_LENGTH];
	uint8_t local_maskaddr[ADDR_LENGTH];
};

/*
 * HCC: Hard Core Candidate (hcc)
 */
 
extern const struct eth_node_t node_candidate[1];

/* APIs.identify
 */
#define candidate_eth_mac() &node_candidate[0].mac_addresse[0]	  //[pin_code]
#define candidate_eth_ip() &node_candidate[0].local_ipaddr[0]	  //[pin_code]
#define candidate_eth_gw() &node_candidate[0].local_gwaddr[0]	  //[pin_code]
#define candidate_eth_mask() &node_candidate[0].local_maskaddr[0] //[pin_code]
//const uint8_t *identify_eth_mac(const uint8_t *macadr);
//uint8_t *identify_tcpip_ip(uint8_t *ip4adr);
//uint8_t *identify_tcpip_gw(uint8_t *ip4adr);
//uint8_t *identify_tcpip_mask(uint8_t *ip4adr);
//void trace_identify_eth_mac(void);
#define	identify_eth_mac(macadr)	SET_FIELD(final_mac, macadr ? macadr : candidate_eth_mac())
#define	identify_tcpip_ip(ip4adr)	SET_FIELD(final_ip, ip4adr ? ip4adr : candidate_eth_ip())
#define	identify_tcpip_gw(ip4adr)	SET_FIELD(final_gw, ip4adr ? ip4adr : candidate_eth_gw())
#define	identify_tcpip_mask(ip4adr)	SET_FIELD(final_mask, ip4adr ? ip4adr : candidate_eth_mask())
#define	trace_identify_eth_mac()	do {	\
		const uint8_t *mac = GET_FIELD(final_mac); \
		printf("mac address %02x%02x%02x%02x%02x%02x\r\n", \
				 mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]); \
	} while(0)

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

/*
 * ---------------------- cstate -------------------------------------------------------------
 */

/*
 * cstate trace_irq_flow
 */
	
#define ISTAT_IRQ_CFG (1 << 0)
#define ISTAT_IRQ_ENAB (1 << 1)
#define ISTAT_DM_IMR (1 << 2)
#define ISTAT_DM_RCR (1 << 3)

#define ISTAT_LOW_TRIGGER (1 << 4)
#define ISTAT_LOW_ACTIVE (1 << 5)

#define ISTAT_IRQ_NOW2 (1 << 6)
#define ISTAT_IRQ_NOW (1 << 7)

/* irqstate.identify
 */
//void deidentify_irq_stat(uint16_t bitflg);
//void identify_irq_stat(uint16_t bitflg);
#define deidentify_irq_stat(bitflg)	SET_CSTATE(irqst, GET_CSTATE(irqst) & ~bitflg)
#define	identify_irq_stat(bitflg)	SET_CSTATE(irqst, GET_CSTATE(irqst) | bitflg)

/* irqstate.identified
 */
//uint16_t identified_irq_stat(void);
//void trace_irq_stat(uint16_t bitflg);
#define	identified_irq_stat()	GET_CSTATE(irqst)
#define	trace_irq_stat(bitflg) do { \
	char istat_term[22]; \
	switch (bitflg) \
	{ \
	case ISTAT_IRQ_CFG: \
		sprintf(istat_term, "[IRQ_CFG]"); \
		break; \
	case ISTAT_IRQ_ENAB: \
		sprintf(istat_term, "[IRQ_ENAB]"); \
		break; \
	case ISTAT_DM_IMR: \
		sprintf(istat_term, "(IMR.pr)"); \
		break; \
	case ISTAT_DM_RCR: \
		sprintf(istat_term, "(RCR.rxen)"); \
		break; \
	case ISTAT_LOW_TRIGGER: \
		sprintf(istat_term, "[IRQ_LOW_TRIGGER]"); \
		break; \
	case ISTAT_LOW_ACTIVE: \
		sprintf(istat_term, "(INTR.lo)"); \
		break; \
	case ISTAT_IRQ_NOW2: \
		sprintf(istat_term, "(INT %lu)", get_task_tryint()); \
		break; \
	case ISTAT_IRQ_NOW: \
	default: \
		istat_term[0] = 0; \
		break; \
	} \
	if (get_task_tryint() > 5 && (bitflg == ISTAT_IRQ_NOW || bitflg == ISTAT_IRQ_NOW2)) \
		/*return;*/ break; \
	printf("[irq_stat]:                   ------------> irqst= %02x on add %02x %s\r\n", \
					GET_CSTATE(irqst), bitflg, istat_term); \
} while(0)

//[dm9051_cstate.c]
uint16_t wrpadiff(uint16_t rwpa_s, uint16_t rwpa_e);
//[dm_eth_status.c]
#if DM_ETH_DEBUG_MODE
		void dm_eth_input_hexdump_reset(void);
		void dm_eth_input_hexdump(const void *buf, size_t len);
#endif

#endif //__DM9051_ENVIDN_IMPL_H
