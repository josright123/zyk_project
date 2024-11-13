//.#ifndef __DM_TYPES2_H
//.#define __DM_TYPES2_H

#if DM_TYPE == 0

/* [ identify.h/identify.c ] */
#define GET_FIELD(field) dm_mget_##field()		   // call-use
#define SET_FIELD(field, val) dm_mset_##field(val) // call-use

#undef DM_AMACRO
#define DM_AMACRO(rtype, mtype, field) \
	rtype dm_mget_##field(void);       \
	rtype dm_mset_##field(const mtype adr);

DM_AMACRO(uint8_t *, mac_t, final_mac);
DM_AMACRO(uint8_t *, ip_t, final_ip);
DM_AMACRO(uint8_t *, ip_t, final_gw);
DM_AMACRO(uint8_t *, ip_t, final_mask);

#define GET_CSTATE(field) cb_get_##field()	   // call-use
#define SET_CSTATE(field, v) cb_set_##field(v) // call-use

#define CB_TYPES_GET_CSTATE_FX(field) cb_get_##field(void)
#define CB_TYPES_SET_CSTATE_FX(mtype, field, v) cb_set_##field(const mtype v)

#undef CB_MACRO
#define CB_MACRO(mtype, field)           \
	mtype CB_TYPES_GET_CSTATE_FX(field); \
	mtype CB_TYPES_SET_CSTATE_FX(mtype, field, v);

CB_MACRO(uint16_t, irqst);

/* HCC: Hard Core Candidate (hcc)
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

/* irqstate.identify
 */
const char *level_str_impl(dm9051_eth_debug_level_t level);

#endif

#if DM_TYPE == 1

#undef DM_MACRO
#define DM_MACRO(mtype, field) \
	mtype field;

static struct dmtype_data
{
	//  mac_t final_mac;
	//	ip_t final_ip;
	//	ip_t final_gw;
	//	ip_t final_mask;
	DM_MACRO(mac_t, final_mac)
	DM_MACRO(ip_t, final_ip)
	DM_MACRO(ip_t, final_gw)
	DM_MACRO(ip_t, final_mask)
} dm;

static struct cbtype_data
{
	uint16_t irqst;
} cb = {
	0x0000,
};

/* Network Configuration */
const struct eth_node_t node_candidate[1] = {
	{
		{0, 0x60, 0x6e, 0x00, 0x00, 0x17},
		{192, 168, 6, 17},
		{192, 168, 6, 1},
		{255, 255, 255, 0},
	}, 
	/*
	{
	 {0, 0x60, 0x6e, 0x00, 0x01, 0x25,},
	 {192, 168, 6,  25},
	 {192, 168, 6,   1},
	 {255, 255, 255, 0},
	},
	   */
	// ... other nodes can be uncommented and added here
};

#elif DM_TYPE == 2

#undef DM_RMACRO
#define DM_RMACRO(rtype, mtype, field, adr_len)                               \
	rtype dm_mget_##field(void)                                    \
	{                                               						\
		return dm.field;                                                      \
	}                                                                         \
	rtype dm_mset_##field(const mtype adr)                         \
	{                                  \
		memcpy(dm.field, adr, adr_len);                                       \
		return dm.field;                                                      \
	}

DM_RMACRO(uint8_t *, mac_t, final_mac, MAC_ADDR_LENGTH)
DM_RMACRO(uint8_t *, ip_t, final_ip, ADDR_LENGTH)
DM_RMACRO(uint8_t *, ip_t, final_gw, ADDR_LENGTH)
DM_RMACRO(uint8_t *, ip_t, final_mask, ADDR_LENGTH)

#define CB_TYPES_GET_CSTATE_FUNC(field) cb_get_##field(void)
#define CB_TYPES_SET_CSTATE_FUNC(mtype, field, v) cb_set_##field(const mtype v)

#undef CB_MACRO
#define CB_MACRO(mtype, field)                      \
	mtype CB_TYPES_GET_CSTATE_FUNC(field)           \
	{                                               \
		return cb.field;                            \
	}                                               \
	mtype CB_TYPES_SET_CSTATE_FUNC(mtype, field, v) \
	{                                               \
		cb.field = v;                               \
		return v;                                   \
	}

CB_MACRO(uint16_t, irqst);

/* Debug Level Implementation */
const char *level_str_impl(dm9051_eth_debug_level_t level)
{
    static const char* const LEVEL_STRINGS[] = {
        "ERROR", //[DM9051_ETH_DEBUG_LEVEL_ERROR] = 
        "DEBUG", //[DM9051_ETH_DEBUG_LEVEL_DEBUG] = 
        "WARN", //[DM9051_ETH_DEBUG_LEVEL_WARN]  = 
        "INFO", //[DM9051_ETH_DEBUG_LEVEL_INFO]  = 
    };

    return (level < sizeof(LEVEL_STRINGS)/sizeof(LEVEL_STRINGS[0]) && 
            LEVEL_STRINGS[level]) ? LEVEL_STRINGS[level] : "UNKNOWN";
}
#endif

#if DM_TYPE == 0
/*
 * trace_irq_flow
 */

#define ISTAT_IRQ_CFG (1 << 0)
#define ISTAT_IRQ_ENAB (1 << 1)
#define ISTAT_DM_IMR (1 << 2)
#define ISTAT_DM_RCR (1 << 3)

#define ISTAT_LOW_TRIGGER (1 << 4)
#define ISTAT_LOW_ACTIVE (1 << 5)

#define ISTAT_IRQ_NOW (1 << 6)
#define ISTAT_IRQ_NOT_NOW (1 << 7)

#define ISTAT_IRQ_NOW2 (1 << 8)
#define ISTAT_IRQ_NOW2END (1 << 9)

// void deidentify_irq_stat(uint16_t bitflg);
// void identify_irq_stat(uint16_t bitflg);
#define deidentify_irq_stat(bitflg) SET_CSTATE(irqst, GET_CSTATE(irqst) & ~bitflg)
#define identify_irq_stat(bitflg) SET_CSTATE(irqst, GET_CSTATE(irqst) | bitflg)
// uint16_t identified_irq_stat(void);
#define identified_irq_stat() GET_CSTATE(irqst)

/* irqstate.identified
 */
#include "control/dbg_opts.h"

#if IDENTIFY_PRINTF_IRQ_STATE
// void trace_irq_stat(uint16_t bitflg);
#define trace_irq_stat(bitflg)                                             \
	do                                                                     \
	{                                                                      \
		char istat_term[22];                                               \
		switch (bitflg)                                                    \
		{                                                                  \
		case ISTAT_IRQ_CFG:                                                \
			sprintf(istat_term, "[IRQ_CFG]");                              \
			break;                                                         \
		case ISTAT_IRQ_ENAB:                                               \
			sprintf(istat_term, "[IRQ_ENAB]");                             \
			break;                                                         \
		case ISTAT_DM_IMR:                                                 \
			sprintf(istat_term, "(IMR.pr)");                               \
			break;                                                         \
		case ISTAT_DM_RCR:                                                 \
			sprintf(istat_term, "(RCR.rxen)");                             \
			break;                                                         \
		case ISTAT_LOW_TRIGGER:                                            \
			sprintf(istat_term, "[IRQ_LOW_TRIGGER]");                      \
			break;                                                         \
		case ISTAT_LOW_ACTIVE:                                             \
			sprintf(istat_term, "(INTR.lo)");                              \
			break;                                                         \
		case ISTAT_IRQ_NOW:                                                \
			sprintf(istat_term, "(ISrvRoutine)");                          \
			break;                                                         \
		case ISTAT_IRQ_NOT_NOW:                                            \
			sprintf(istat_term, "(ISrvRoutine NOT match)");                \
			break;                                                         \
		case ISTAT_IRQ_NOW2:                                               \
			sprintf(istat_term, "(INT %lu)", get_interrupt_count());       \
			break;                                                         \
		case ISTAT_IRQ_NOW2END:                                            \
			sprintf(istat_term, "(INT.End)");                              \
			break;                                                         \
		default:                                                           \
			istat_term[0] = 0;                                             \
			break;                                                         \
		}                                                                  \
		if (get_interrupt_count() > 5 && bitflg == ISTAT_IRQ_NOW)          \
			;                                                              \
		else if (get_interrupt_count() > 5 && bitflg == ISTAT_IRQ_NOW2)    \
		{                                                                  \
			if (!(get_interrupt_count() % 25))                             \
				printf("%s--> %s\r\n", PRINT_INFO_IRQ, istat_term);              \
		}                                                                  \
		else if (get_interrupt_count() > 5 && bitflg == ISTAT_IRQ_NOW2END) \
			printf("%s--> %s\r\n", PRINT_INFO_IRQ, istat_term);                  \
		else                                                               \
			printf("%s--> irqst= %02x on add-bit %02x %s\r\n",   \
				   PRINT_INFO_IRQ, GET_CSTATE(irqst), bitflg, istat_term);                 \
	} while (0)
#else
#define trace_irq_stat(bitflg)
#endif

#endif

#undef DM_TYPE
//.#endif //__DM_TYPES2_H
