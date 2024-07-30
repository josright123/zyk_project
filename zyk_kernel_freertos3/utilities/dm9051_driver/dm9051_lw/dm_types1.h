//.#ifndef __DM_TYPES_H
//.#define __DM_TYPES_H

#define DM_TYPES_GET_CALL(mtype, field)			    dm9051opts_get_##mtype##field()
#define DM_TYPES_SET_CALL(mtype, field, sv)     dm9051opts_set_##mtype##field(sv)
#define DM_TYPES_GETDESC_CALL(mtype, field)     dm9051opts_desc_##field()

#define DM_TYPES_GET_FUNC(mtype, field)			    dm9051opts_get_##mtype##field(void)
#define DM_TYPES_SET_FUNC(mtype, field, sv)     dm9051opts_set_##mtype##field(const mtype sv)
#define DM_TYPES_GETDESC_FUNC(mtype, field)		  dm9051opts_desc_##field(void)

// - type 0, call in the application program
#define DM_GET_FIELD                            DM_TYPES_GET_CALL
#define DM_SET_FIELD                            DM_TYPES_SET_CALL
#define DM_GET_DESC                             DM_TYPES_GETDESC_CALL

#if DM_TYPE == 0

	// - type 0, extern API
	#undef DM_MACRO
	#define DM_MACRO(rtype, mtype, field) \
		rtype DM_TYPES_GET_FUNC(mtype, field); \
		char *DM_TYPES_GETDESC_FUNC(mtype, field); \
		void DM_TYPES_SET_FUNC(mtype, field, setval);

	//uint8_t* DM_TYPES_GET_FUNC(mac_t, final_mac);
  //char *DM_TYPES_GETDESC_FUNC(mac_t, final_mac);
  //void DM_TYPES_SET_FUNC(mac_t, final_mac, sv);
  //uint8_t* DM_TYPES_GET_FUNC(ip_t, ip);
  //char *DM_TYPES_GETDESC_FUNC(ip_t, ip);
  //void DM_TYPES_SET_FUNC(ip_t, ip, sv);
  //uint8_t* DM_TYPES_GET_FUNC(ip_t, gw);
  //char *DM_TYPES_GETDESC_FUNC(ip_t, gw);
  //void DM_TYPES_SET_FUNC(ip_t, gw, sv);
  //uint8_t* DM_TYPES_GET_FUNC(ip_t, mask);
  //char *DM_TYPES_GETDESC_FUNC(ip_t, mask);
  //void DM_TYPES_SET_FUNC(ip_t, mask, sv);

  DM_MACRO(uint8_t*,  mac_t, final_mac)
  DM_MACRO(uint8_t*,  ip_t, ip)
  DM_MACRO(uint8_t*,  ip_t, gw)
  DM_MACRO(uint8_t*,  ip_t, mask)

  //uint16_t DM_TYPES_GET_FUNC(uint16_t, read_chip_id);
  //char *DM_TYPES_GETDESC_FUNC(uint16_t, read_chip_id);
  //void DM_TYPES_SET_FUNC(uint16_t, read_chip_id, sv);
  //uint16_t DM_TYPES_GET_FUNC(uint16_t, irqst);
  //char *DM_TYPES_GETDESC_FUNC(uint16_t, irqst);
  //void DM_TYPES_SET_FUNC(uint16_t, irqst, sv);
  //DM_MACRO(uint16_t,  uint16_t, read_chip_id)
  //DM_MACRO(uint16_t, uint16_t, irqst)
#elif DM_TYPE == 1

	// - type 1, field
	#undef DM_MACRO
	#define DM_MACRO(rtype, mtype, field) \
		mtype field; \
		char *desc##field;

	//mac_t final_mac;
  //char *descfinal_mac;
  //ip_t ip;
  //char *descip;
  //ip_t gw;
  //char *descgw;
  //ip_t mask;
  //char *descmask;
  DM_MACRO(uint8_t*,  mac_t, final_mac)
  DM_MACRO(uint8_t*,  ip_t, ip)
  DM_MACRO(uint8_t*,  ip_t, gw)
  DM_MACRO(uint8_t*,  ip_t, mask)

  //uint16_t read_chip_id;
  //char *descread_chip_id;
  //uint16_t irqst;
  //char *descirqst;
  //DM_MACRO(uint16_t,  uint16_t, read_chip_id)
  //DM_MACRO(uint16_t,  uint16_t, irqst)
#elif DM_TYPE == 2

	// - type 2, implement
		
	#undef DM_RMACRO
	#define DM_RMACRO(rtype, mtype, field, adr_len)  \
		rtype DM_TYPES_GET_FUNC(mtype, field) { \
			return (rtype) dm9051optsex[mstep_get_net_index()].field; \
		} \
		char *DM_TYPES_GETDESC_FUNC(mtype, field) { \
			return dm9051optsex[mstep_get_net_index()].desc##field; \
		} \
		void DM_TYPES_SET_FUNC(mtype, field, setval) { \
			memcpy(dm9051optsex[mstep_get_net_index()].field, setval, adr_len); \
		}

  //uint8_t* DM_TYPES_GET_FUNC(mac_t, final_mac) {
  //  return (uint8_t*) dm9051optsex[mstep_get_net_index()].final_mac;
  //}
  //char *DM_TYPES_GETDESC_FUNC(mac_t, final_mac) {
  //  return dm9051optsex[mstep_get_net_index()].descfinal_mac;
  //}
  //void DM_TYPES_SET_FUNC(mac_t, final_mac, sv) {
  //  memcpy(dm9051optsex[mstep_get_net_index()].final_mac, sv, MAC_ADDR_LENGTH);
  //}

  DM_RMACRO(uint8_t*, mac_t, final_mac, MAC_ADDR_LENGTH)

  /*uint8_t* DM_TYPES_GET_FUNC(ip_t, ip) {
    return (uint8_t*) dm9051optsex[mstep_get_net_index()].ip;
  }
  char *DM_TYPES_GETDESC_FUNC(ip_t, ip) {
    return dm9051optsex[mstep_get_net_index()].descip;
  }
  void DM_TYPES_SET_FUNC(ip_t, ip, sv) {
    memcpy(dm9051optsex[mstep_get_net_index()].ip, sv, ADDR_LENGTH);
  }

  uint8_t* DM_TYPES_GET_FUNC(ip_t, gw) {
    return (uint8_t*) dm9051optsex[mstep_get_net_index()].gw;
  }
  char *DM_TYPES_GETDESC_FUNC(ip_t, gw) {
    return dm9051optsex[mstep_get_net_index()].descgw;
  }
  void DM_TYPES_SET_FUNC(ip_t, gw, sv) {
    memcpy(dm9051optsex[mstep_get_net_index()].gw, sv, ADDR_LENGTH);
  }

  uint8_t* DM_TYPES_GET_FUNC(ip_t, mask) {
    return (uint8_t*) dm9051optsex[mstep_get_net_index()].mask;
  }
  char *DM_TYPES_GETDESC_FUNC(ip_t, mask) {
    return dm9051optsex[mstep_get_net_index()].descmask;
  }
  void DM_TYPES_SET_FUNC(ip_t, mask, sv) {
    memcpy(dm9051optsex[mstep_get_net_index()].mask, sv, ADDR_LENGTH);
  }*/

  DM_RMACRO(uint8_t*, ip_t, ip, ADDR_LENGTH)
  DM_RMACRO(uint8_t*, ip_t, gw, ADDR_LENGTH)
  DM_RMACRO(uint8_t*, ip_t, mask, ADDR_LENGTH)

  #undef DM_MACRO
  #define DM_MACRO(rtype, mtype, field)  \
    rtype DM_TYPES_GET_FUNC(mtype, field) { \
      return (rtype)(dm9051optsex[mstep_get_net_index()].field); \
    } \
    char *DM_TYPES_GETDESC_FUNC(mtype, field) { \
      return (dm9051optsex[mstep_get_net_index()].desc##field); \
    } \
    void DM_TYPES_SET_FUNC(mtype, field, setval) { \
      dm9051optsex[mstep_get_net_index()].field = setval; \
    }

  //uint16_t DM_TYPES_GET_FUNC(uint16_t, read_chip_id) {
  //  return (uint16_t) dm9051optsex[mstep_get_net_index()].read_chip_id;
 // }
  //char *DM_TYPES_GETDESC_FUNC(uint16_t, read_chip_id) {
  //  return dm9051optsex[mstep_get_net_index()].descread_chip_id;
  //}
  //void DM_TYPES_SET_FUNC(uint16_t, read_chip_id, sv) {
  //  dm9051optsex[mstep_get_net_index()].read_chip_id = sv;
  //}

  //uint16_t DM_TYPES_GET_FUNC(uint16_t, irqst) {
  //  return (uint16_t) dm9051optsex[mstep_get_net_index()].irqst;
  //}
  //char *DM_TYPES_GETDESC_FUNC(uint16_t, irqst) {
  //  return dm9051optsex[mstep_get_net_index()].descirqst;
  //}
  //void DM_TYPES_SET_FUNC(uint16_t, irqst, sv) {
  //  dm9051optsex[mstep_get_net_index()].irqst = sv;
  //}
#endif

DM_MACRO(uint16_t,  uint16_t, read_chip_id)
DM_MACRO(uint16_t,  uint16_t, irqst)


//dm_types1.h
#if 0
DM_MACRO(enable_t,	enable_t, test_plan_log)
DM_MACRO(uint8_t,	uint8_t, iomode)
DM_MACRO(csmode_t,	csmode_t, csmode)
DM_MACRO(bmcrmode_t, bmcrmode_t, bmcrmode)
DM_MACRO(uint8_t,	uint8_t, promismode)
DM_MACRO(enable_t,	enable_t, rxtypemode)
DM_MACRO(enable_t,	enable_t, rxmode_checksum_offload)
DM_MACRO(enable_t,	enable_t, flowcontrolmode)
DM_MACRO(enable_t,	enable_t, onlybytemode)
DM_MACRO(uint8_t,	uint8_t, hdir_x2ms)
DM_MACRO(enable_t,	enable_t, hdlr_confrecv)
DM_MACRO(enable_t,	enable_t, tx_endbit)
DM_MACRO(enable_t,	enable_t, generic_core_rst)
#endif

#undef DM_TYPE
//.#endif //__DM_TYPES_H
