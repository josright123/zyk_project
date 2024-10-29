#ifndef __DM9051_OPTS_H
#define __DM9051_OPTS_H
#include "stdint.h"
#include "stdio.h"
#include <string.h>

/* __DRV_H
 */

#define DRV_INTR_MODE /* define 'DRV_INTR_MODE' to configure the mcu EXTINT */
//#undef DRV_INTR_MODE

#define FORCE_MODE_10M
//#undef FORCE_MODE_10M

#define FORCE_CHKSUM_OFFLOAD
//#undef FORCE_CHKSUM_OFFLOAD

/* __USR_H
 */
 
#define ETHERNET_INTERRUPT_MODE 1 // Please keep define to 1, INT or POLLING by _data.h
#define DM_ETH_DEBUG_MODE 1       // 0

#include "dm9051_driver_define.h" //#include "identify/dm9051_driver_define.h"

/* __SYS_H
 */
 
#define	application_Type	uipAPP_CONF

#define freeRTOS_CONF	1 //0 //1 //1 //1 //0

#include "dm_eth_driver_define.h" //#include "identify/dm_eth_driver_define.h"

#endif //__DM9051_OPTS_H
