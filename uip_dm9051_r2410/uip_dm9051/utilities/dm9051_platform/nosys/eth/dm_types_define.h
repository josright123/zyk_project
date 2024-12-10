//.#ifndef __DM_TYPES2_H
//.#define __DM_TYPES2_H

#if DM_TYPE == 0

/* [ identify.h/identify.c ] */
//#define GET_FIELD(field) dm_mget_##field()		   // call-use
//#define SET_FIELD(field, val) dm_mset_##field(val) // call-use

//#undef DM_AMACRO
//#define DM_AMACRO(rtype, mtype, field) \
//	rtype dm_mget_##field(void);       \
//	rtype dm_mset_##field(const mtype adr);

//#define GET_CSTATE(field) cb_get_##field()	   // call-use
//#define SET_CSTATE(field, v) cb_set_##field(v) // call-use

//#define CB_TYPES_GET_CSTATE_FX(field) cb_get_##field(void)
//#define CB_TYPES_SET_CSTATE_FX(mtype, field, v) cb_set_##field(const mtype v)

//#undef CB_MACRO
//#define CB_MACRO(mtype, field)           \
//	mtype CB_TYPES_GET_CSTATE_FX(field); \
//	mtype CB_TYPES_SET_CSTATE_FX(mtype, field, v);

/* APIs.identify
 */
//#define candidate_eth_mac() &node_candidate[0].mac_addresse[0]    //[pin_code]
#define candidate_eth_ip() &node_candidate[0].local_ipaddr[0]     //[pin_code]
#define candidate_eth_gw() &node_candidate[0].local_gwaddr[0]     //[pin_code]
#define candidate_eth_mask() &node_candidate[0].local_maskaddr[0] //[pin_code]
// const uint8_t *identify_eth_mac(const uint8_t *macadr);
// uint8_t *identify_tcpip_ip(uint8_t *ip4adr);
// uint8_t *identify_tcpip_gw(uint8_t *ip4adr);
// uint8_t *identify_tcpip_mask(uint8_t *ip4adr);
// void trace_identify_eth_mac(void);
//#define identify_eth_mac(macadr) SET_FIELD(final_mac, macadr ? macadr : candidate_eth_mac())
#define identify_tcpip_ip(ip4adr) SET_FIELD(final_ip, ip4adr ? ip4adr : candidate_eth_ip())
#define identify_tcpip_gw(ip4adr) SET_FIELD(final_gw, ip4adr ? ip4adr : candidate_eth_gw())
#define identify_tcpip_mask(ip4adr) SET_FIELD(final_mask, ip4adr ? ip4adr : candidate_eth_mask())

/* APIs.identified
 */
// const uint8_t *identified_eth_mac(void);
// uint8_t *identified_tcpip_ip(void);
// uint8_t *identified_tcpip_gw(void);
// uint8_t *identified_tcpip_mask(void);
//#define identified_eth_mac() GET_FIELD(final_mac)
#define identified_tcpip_ip() GET_FIELD(final_ip)
#define identified_tcpip_gw() GET_FIELD(final_gw)
#define identified_tcpip_mask() GET_FIELD(final_mask)

#if 1
//	DM_AMACRO(uint8_t *, mac_t, final_mac);
	DM_AMACRO(uint8_t *, ip_t, final_ip);
	DM_AMACRO(uint8_t *, ip_t, final_gw);
	DM_AMACRO(uint8_t *, ip_t, final_mask);

//	CB_MACRO(uint16_t, irqst);

	/* HCC: Hard Core Candidate (hcc)
	 */
	extern const struct eth_node_t node_candidate[1];

	/* irqstate.identify
	 */
//.	const char *level_str_impl(dm9051_eth_debug_level_t level);
#endif

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
//const char *level_str_impl(dm9051_eth_debug_level_t level)
//{
//    static const char* const LEVEL_STRINGS[] = {
//        "ERROR", //[DM9051_ETH_DEBUG_LEVEL_ERROR] = 
//        "DEBUG", //[DM9051_ETH_DEBUG_LEVEL_DEBUG] = 
//        "WARN", //[DM9051_ETH_DEBUG_LEVEL_WARN]  = 
//        "INFO", //[DM9051_ETH_DEBUG_LEVEL_INFO]  = 
//    };

//    return (level < sizeof(LEVEL_STRINGS)/sizeof(LEVEL_STRINGS[0]) && 
//            LEVEL_STRINGS[level]) ? LEVEL_STRINGS[level] : "UNKNOWN";
//}
#endif

#if DM_TYPE == 0

#endif

#undef DM_TYPE
//.#endif //__DM_TYPES2_H
