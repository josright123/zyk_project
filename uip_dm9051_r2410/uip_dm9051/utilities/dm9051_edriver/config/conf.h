/* conf.h
 */
#ifndef __DM_CONF_H__
#define	__DM_CONF_H__

/* domain */
#include "cdef.h"

/* hal, such as designed a MCU */
#include "../hal/dm9051_hal.h"

/* common util */
#define DM_TYPE 0
#include "utils/dm_identify_types_define.h"

/* all common */
//#include "dbg_opts.h"

/* all common, such define the 'freeRTOS' */
#include "dm9051opts.h" //"dm_eth_opts.h" //

#endif //__DM_CONF_H__
