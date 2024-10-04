#include "dm9051opts.h" //#include "_dm9051_env.h"
#include "dm9051_lw.h"

#include "dm_identify_impl.h" //[h file implement]
/* Only used, in [ env.c ] */
//typedef uint8_t mac_t[MAC_ADDR_LENGTH];
//typedef uint8_t ip_t[ADDR_LENGTH];

#define DM_TYPE 1
#include "dm_identify_types_define.h"

#define DM_TYPE 2
#include "dm_identify_types_define.h"

/*
 * candidate
 */

typedef struct eth_node_st
{
	uint8_t mac_addresse[MAC_ADDR_LENGTH];
	uint8_t local_ipaddr[ADDR_LENGTH];
	uint8_t local_gwaddr[ADDR_LENGTH];
	uint8_t local_maskaddr[ADDR_LENGTH];
} eth_node_t;

//(define is as rather than '_ETHERNET_COUNT', refer to as '_BOARD_SPI_COUNT' counter) define BOARD_SPI_COUNT 'N'
const eth_node_t node_candidate[1] = {
	{
		{
			0,
			0x60,
			0x6e,
			0x00,
			0x00,
			0x17,
		},
		{192, 168, 6, 17},
		{192, 168, 6, 1},
		{255, 255, 255, 0},
	}, /*
	   { \
		   {0, 0x60, 0x6e, 0x00, 0x01, 0x26,}, \
		   {192, 168, 6,  26}, \
		   {192, 168, 6,   1}, \
		   {255, 255, 255, 0}, \
	   }, \
	   { \
		   {0, 0x60, 0x6e, 0x00, 0x01, 0x25,}, \
		   {192, 168, 6,  25}, \
		   {192, 168, 6,   1}, \
		   {255, 255, 255, 0}, \
	   }, \
	   { \
		   {0, 0x60, 0x6e, 0x00, 0x01, 0xfe,}, \
		   {192, 168, 6,  66}, \
		   {192, 168, 6,   1}, \
		   {255, 255, 255, 0}, \
	   }, \
	   { \
		   {0, 0x60, 0x6e, 0x00, 0x01, 0xff,}, \
		   {192, 168, 6,  67}, \
		   {192, 168, 6,   1}, \
		   {255, 255, 255, 0}, \
	   }, \
	   */
};

/*
 * HCC: Hard Core Candidate (hcc)
 */
 
#define candidate_eth_mac() &node_candidate[0].mac_addresse[0]	  //[pin_code]
#define candidate_eth_ip() &node_candidate[0].local_ipaddr[0]	  //[pin_code]
#define candidate_eth_gw() &node_candidate[0].local_gwaddr[0]	  //[pin_code]
#define candidate_eth_mask() &node_candidate[0].local_maskaddr[0] //[pin_code]

const uint8_t *identify_eth_mac(const uint8_t *macadr)
{
	return SET_FIELD(final_mac, macadr ? macadr : candidate_eth_mac()); // DM_SET_FIELDmac(macadr);
	//return GET_FIELD(final_mac);
}

uint8_t *identify_tcpip_ip(uint8_t *ip4adr)
{
	return SET_FIELD(final_ip, ip4adr ? ip4adr : candidate_eth_ip()); // DM_SET_FIELD_ips(dm.final_ip, ip4adr); //DM_SET_FIELD(ip_t ,ip, ip4adr ? ip4adr : candidate_eth_ip());
	//return GET_FIELD(final_ip);								   // DM_GET_FIELD_ips(dm.final_ip); //return DM_GET_FIELD(ip_t, ip);
}
uint8_t *identify_tcpip_gw(uint8_t *ip4adr)
{
	return SET_FIELD(final_gw, ip4adr ? ip4adr : candidate_eth_gw()); // DM_SET_FIELD_ips(dm.final_gw, ip4adr); //DM_SET_FIELD(ip_t ,gw, ip4adr ? ip4adr : candidate_eth_gw());
	//return GET_FIELD(final_gw);								   // DM_GET_FIELD_ips(dm.final_gw); //return DM_GET_FIELD(ip_t, gw);
}
uint8_t *identify_tcpip_mask(uint8_t *ip4adr)
{
	return SET_FIELD(final_mask, ip4adr ? ip4adr : candidate_eth_mask()); // DM_SET_FIELD_ips(dm.final_mask, ip4adr); //DM_SET_FIELD(ip_t ,mask, ip4adr ? ip4adr : candidate_eth_mask());
	//return GET_FIELD(final_mask);								   // DM_GET_FIELD_ips(dm.final_mask); //return DM_GET_FIELD(ip_t, mask);
}

void trace_identified_eth_mac(int showf)
{
	if (showf)
	{
		const uint8_t *mac = GET_FIELD(final_mac);
		printf("mac address %02x%02x%02x%02x%02x%02x\r\n",
			   mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	}
}

// const uint8_t *identified_eth_mac(void) {
//	return GET_FIELD(final_mac); //DM_GET_FIELDmac(); //DM_GET_FIELD(mac_t, final_mac);
// }
// uint8_t *identified_tcpip_ip(void) {
//	return GET_FIELD(final_ip); //DM_GET_FIELD_ips(dm.final_ip); //return DM_GET_FIELD(ip_t, ip);
// }
// uint8_t *identified_tcpip_gw(void) {
//	return GET_FIELD(final_gw); //DM_GET_FIELD_ips(dm.final_gw); //return DM_GET_FIELD(ip_t, gw);
// }
// uint8_t *identified_tcpip_mask(void) {
//	return GET_FIELD(final_mask); //DM_GET_FIELD_ips(dm.final_mask); //return DM_GET_FIELD(ip_t, mask);
// }

/*
 * ---------------------- cstate -------------------------------------------------------------
 */

//#define CB_TYPE 0
//#include "cb_types2.h"

//#define CB_TYPE 1
//#include "cb_types2.h"

//#define CB_TYPE 2
//#include "cb_types2.h"

/*
 * cboard_trace_irq_flow
 */

// uint16_t irqst = 0;

void deidentify_irq_stat(uint16_t bitflg)
{
	// irqst &= ~bitflg; //return DM_GET_FIELD(uint16_t, irqst);
	SET_CSTATE(irqst, GET_CSTATE(irqst) & ~bitflg); // DM_SET_FIELDirqst(DM_GET_FIELDirqst() & ~bitflg);
}
void identify_irq_stat(uint16_t bitflg)
{
	// irqst |= bitflg; //DM_SET_FIELD(uint16_t, irqst, DM_GET_FIELD(uint16_t, irqst) | bitflg); //store into dm9051optsex[i].read_chip_id
	SET_CSTATE(irqst, GET_CSTATE(irqst) | bitflg); // DM_SET_FIELDirqst(DM_GET_FIELDirqst() | bitflg);
}

void trace_irq_stat(uint16_t bitflg)
{
	char istat_term[22];
	switch (bitflg)
	{
	case ISTAT_IRQ_CFG:
		sprintf(istat_term, "[IRQ_CFG]");
		break;
	case ISTAT_IRQ_ENAB:
		sprintf(istat_term, "[IRQ_ENAB]");
		break;
	case ISTAT_DM_IMR:
		sprintf(istat_term, "(IMR.pr)");
		break;
	case ISTAT_DM_RCR:
		sprintf(istat_term, "(RCR.rxen)");
		break;
	case ISTAT_LOW_TRIGGER:
		sprintf(istat_term, "[IRQ_LOW_TRIGGER]");
		break;
	case ISTAT_LOW_ACTIVE:
		sprintf(istat_term, "(INTR.lo)");
		break;
	case ISTAT_IRQ_NOW2:
		sprintf(istat_term, "(INT %lu)", get_task_tryint());
		break;
	case ISTAT_IRQ_NOW:
	default:
		istat_term[0] = 0;
		break;
	}

	if (get_task_tryint() > 5 && (bitflg == ISTAT_IRQ_NOW || bitflg == ISTAT_IRQ_NOW2))
		return;

	printf("[irq_stat]:                   ------------> irqst= %02x on add %02x %s\r\n", identified_irq_stat(), bitflg, istat_term);
}
