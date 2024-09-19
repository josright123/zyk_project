//.#ifndef __DM_TYPES2_H
//.#define __DM_TYPES2_H

//[dm9051_data.c]
#if DM_TYPE == 0

#endif

#if DM_TYPE == 1

	#undef DM_MACRO
	#define DM_MACRO(mtype, field) \
		mtype field;

	static struct dmtype_data {
		DM_MACRO(mac_t, final_mac)
	//static mac_t final_mac;
		DM_MACRO(ip_t, final_ip)
		DM_MACRO(ip_t, final_gw)
		DM_MACRO(ip_t, final_mask)
	//	ip_t final_ip;
	//	ip_t final_gw;
	//	ip_t final_mask;
	} dm;

#elif DM_TYPE == 2

	//#when.s DM_TYPE == 0
	#define GET_FIELD(field)			dm_mget_##field()
	#define SET_FIELD(field, val)		dm_mset_##field(val)
	//#when.e DM_TYPE == 0

	#undef DM_RMACRO
	#define DM_RMACRO(rtype, mtype, field, adr_len)  \
		/*rtype DM_TYPES_GET_FUNC(mtype, field) {*/ \
		/*	return (rtype) dm9051optsex[mstep_get_net_index()].field;*/ \
		/*}*/ \
		static rtype dm_mget_##field(void) { /*DM_GET_FIELDmac(void)*/ \
			return dm.field; \
		} \
		/*void DM_TYPES_SET_FUNC(mtype, field, setval) {*/ \
		/*	memcpy(dm9051optsex[mstep_get_net_index()].field, setval, adr_len); */ \
		/*}*/ \
		static void dm_mset_##field(const mtype adr) { /*DM_SET_FIELDmac(const mtype adr)*/ \
			memcpy(dm.field, adr, adr_len); \
		}

	DM_RMACRO(uint8_t*,	mac_t, final_mac, MAC_ADDR_LENGTH)
//static void DM_SET_FIELDmac(const uint8_t *macadr){
//	memcpy(final_mac, macadr, sizeof(final_mac));
//}

//static uint8_t *DM_GET_FIELDmac(void) {
//	return final_mac;
//}
	DM_RMACRO(uint8_t*,	ip_t, final_ip, ADDR_LENGTH)
	DM_RMACRO(uint8_t*,	ip_t, final_gw, ADDR_LENGTH)
	DM_RMACRO(uint8_t*,	ip_t, final_mask, ADDR_LENGTH)
//	uint8_t *DM_GET_FIELD_ips(ip_t ipx) {
//		return ipx;
//	}

//	void DM_SET_FIELD_ips(ip_t ipx, const uint8_t *ipadr){
//		memcpy(ipx, ipadr, sizeof(ipx));
//	}

#endif

#undef DM_TYPE
//.#endif //__DM_TYPES2_H
