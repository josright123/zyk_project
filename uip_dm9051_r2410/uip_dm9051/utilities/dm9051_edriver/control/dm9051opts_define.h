/* 
 * dm9051_lw_driver_define.h
 */
#ifndef __DM9051_LW_DRIVER_DEF_H
#define __DM9051_LW_DRIVER_DEF_H
#include "control/dm9051opts.h"

//[1]
#ifdef DRV_INTR_MODE
#define	ETHERNET_INTERRUPT_MODE
#define RX_MODE_STR "interrupt"
#else
#define	ETHERNET_POLLING_MODE
#define RX_MODE_STR "polling"
#endif
 
#define DM_ETH_DEBUG_MODE										1

/* Check LINK state in source 
 * Use [NSR/NSR] 1
 * Use [BMSR]    2
 */
#define DM9051_CHECK_MAC										1
#define DM9051_CHECK_PHY										2

#define LINK_STATE_SOURCE										DM9051_CHECK_MAC

#endif //__DM9051_LW_DRIVER_DEF_H
