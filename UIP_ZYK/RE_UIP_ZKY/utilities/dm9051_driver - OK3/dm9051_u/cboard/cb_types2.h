//.#ifndef __CB_TYPES_H
//.#define __CB_TYPES_H

#define CB_TYPES_GET_FUNC(field)							cb_get_##field(void)
#define CB_TYPES_SET_FUNC(mtype, field, v)					cb_set_##field(const mtype v)

#if CB_TYPE == 0
	// - type 0, extern API
	
	//#define CB_TYPES_GET_CALL(field)
	#define GET_CSTATE(field)								cb_get_##field() //CB_TYPES_GET_CALL() //DM_GET_FIELDirqst()
	//#define CB_TYPES_SET_CALL(field, v)
	#define SET_CSTATE(field, v)							cb_set_##field(v) //CB_TYPES_SET_CALL(field, v) //DM_SET_FIELDirqst(v)

	//uint16_t DM_GET_FIELDirqst(void);
	//void DM_SET_FIELDirqst(uint16_t val);
#elif CB_TYPE == 1
	// - type 1, field
	static struct cbtype_data {
		uint16_t	irqst;
	} cb = {
		0x0000,
	};
#elif CB_TYPE == 2
	// - type 2, implement
	#undef CB_MACRO
	#define CB_MACRO(mtype, field)  \
		mtype CB_TYPES_GET_FUNC(field) { \
			return cb.field; \
		} \
		void CB_TYPES_SET_FUNC(mtype, field, v) { \
			cb.field = v;  \
		}

//		uint16_t DM_GET_FIELDirqst(void) {
//			return cb.irqst;
//		}
//		void DM_SET_FIELDirqst(uint16_t val) {
//			cb.irqst = val; 
//		}
		CB_MACRO(uint16_t, irqst);
#endif

#undef CB_TYPE
//.#endif //__CB_TYPES_H
