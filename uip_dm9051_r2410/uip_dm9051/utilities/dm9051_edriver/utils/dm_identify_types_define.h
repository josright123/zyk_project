//.#ifndef __DM_TYPES2_H
//.#define __DM_TYPES2_H

//[dm9051_data.c]
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
	/*static*/ rtype dm_mget_##field(void)                                    \
	{ /*DM_GET_FIELDmac(void)*/                                               \
		return dm.field;                                                      \
	}                                                                         \
	/*void DM_TYPES_SET_FUNC(mtype, field, setval) {*/                        \
	/*	memcpy(dm9051optsex[mstep_get_net_index()].field, setval, adr_len); */ \
	/*}*/                                                                     \
	/*static*/ rtype dm_mset_##field(const mtype adr)                         \
	{ /*DM_SET_FIELDmac(const mtype adr)*/                                    \
		memcpy(dm.field, adr, adr_len);                                       \
		return dm.field;                                                      \
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

#endif

//
// extern
//

#if DM_TYPE == 0
/*
 * cstate trace_irq_flow
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

/* irqstate.identify
 */
// void deidentify_irq_stat(uint16_t bitflg);
// void identify_irq_stat(uint16_t bitflg);
#define deidentify_irq_stat(bitflg) SET_CSTATE(irqst, GET_CSTATE(irqst) & ~bitflg)
#define identify_irq_stat(bitflg) SET_CSTATE(irqst, GET_CSTATE(irqst) | bitflg)

/* irqstate.identified
 */
// uint16_t identified_irq_stat(void);
// void trace_irq_stat(uint16_t bitflg);
#define identified_irq_stat() GET_CSTATE(irqst)

#include "config/identify_opts.h"

#if IDENTIFY_PRINTF_IRQ_STATE
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
