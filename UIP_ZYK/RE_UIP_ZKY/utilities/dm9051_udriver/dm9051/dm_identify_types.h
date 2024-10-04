//.#ifndef __DM_TYPES2_H
//.#define __DM_TYPES2_H

//[dm9051_data.c]
#if DM_TYPE == 0

/* [ identify.h/identify.c ] */
typedef uint8_t mac_t[MAC_ADDR_LENGTH];
typedef uint8_t ip_t[ADDR_LENGTH];

#define GET_FIELD(field) dm_mget_##field() //call-use
#define SET_FIELD(field, val) dm_mset_##field(val) //call-use
		
#undef DM_AMACRO
#define DM_AMACRO(rtype, mtype, field) \
		rtype dm_mget_##field(void); \
		void dm_mset_##field(const mtype adr);

		DM_AMACRO(uint8_t *, mac_t, final_mac);
		DM_AMACRO(uint8_t *, ip_t, final_ip);
		DM_AMACRO(uint8_t *, ip_t, final_gw);
		DM_AMACRO(uint8_t *, ip_t, final_mask);
	
#define GET_CSTATE(field) cb_get_##field() //call-use
#define SET_CSTATE(field, v) cb_set_##field(v) //call-use

#define CB_TYPES_GET_CSTATE_FX(field) cb_get_##field(void)
#define CB_TYPES_SET_CSTATE_FX(mtype, field, v) cb_set_##field(const mtype v)

#undef CB_MACRO
#define CB_MACRO(mtype, field)              \
		mtype CB_TYPES_GET_CSTATE_FX(field);							\
		void CB_TYPES_SET_CSTATE_FX(mtype, field, v);

	CB_MACRO(uint16_t, irqst);

#endif

#if DM_TYPE == 1

#undef DM_MACRO
#define DM_MACRO(mtype, field) \
	mtype field;

static struct dmtype_data
{
	DM_MACRO(mac_t, final_mac)
	// static mac_t final_mac;
	DM_MACRO(ip_t, final_ip)
	DM_MACRO(ip_t, final_gw)
	DM_MACRO(ip_t, final_mask)
	//	ip_t final_ip;
	//	ip_t final_gw;
	//	ip_t final_mask;
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
	/*rtype DM_TYPES_GET_FUNC(mtype, field) {*/                               \
	/*	return (rtype) dm9051optsex[mstep_get_net_index()].field;*/            \
	/*}*/                                                                     \
	/*static*/ rtype dm_mget_##field(void)                                        \
	{ /*DM_GET_FIELDmac(void)*/                                               \
		return dm.field;                                                      \
	}                                                                         \
	/*void DM_TYPES_SET_FUNC(mtype, field, setval) {*/                        \
	/*	memcpy(dm9051optsex[mstep_get_net_index()].field, setval, adr_len); */ \
	/*}*/                                                                     \
	/*static*/ void dm_mset_##field(const mtype adr)                              \
	{ /*DM_SET_FIELDmac(const mtype adr)*/                                    \
		memcpy(dm.field, adr, adr_len);                                       \
	}

// static void DM_SET_FIELDmac(const uint8_t *macadr){
//	memcpy(final_mac, macadr, sizeof(final_mac));
// }

// static uint8_t *DM_GET_FIELDmac(void) {
//	return final_mac;
// }
DM_RMACRO(uint8_t *, mac_t, final_mac, MAC_ADDR_LENGTH)
DM_RMACRO(uint8_t *, ip_t, final_ip, ADDR_LENGTH)
DM_RMACRO(uint8_t *, ip_t, final_gw, ADDR_LENGTH)
DM_RMACRO(uint8_t *, ip_t, final_mask, ADDR_LENGTH)
//	uint8_t *DM_GET_FIELD_ips(ip_t ipx) {
//		return ipx;
//	}

//	void DM_SET_FIELD_ips(ip_t ipx, const uint8_t *ipadr){
//		memcpy(ipx, ipadr, sizeof(ipx));
//	}

#define CB_TYPES_GET_CSTATE_FUNC(field) cb_get_##field(void)
#define CB_TYPES_SET_CSTATE_FUNC(mtype, field, v) cb_set_##field(const mtype v)

#undef CB_MACRO
#define CB_MACRO(mtype, field)              \
	mtype CB_TYPES_GET_CSTATE_FUNC(field)          \
	{                                       \
		return cb.field;                    \
	}                                       \
	void CB_TYPES_SET_CSTATE_FUNC(mtype, field, v) \
	{                                       \
		cb.field = v;                       \
	}
	
CB_MACRO(uint16_t, irqst);

#endif

#undef DM_TYPE
//.#endif //__DM_TYPES2_H
