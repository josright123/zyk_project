#ifndef __DM9051_OPTS_H
#define __DM9051_OPTS_H
#include "stdint.h"
#include "stdio.h"
#include <string.h>

/* __DRV_H
 */

#define DRV_INTR_MODE // define 'DRV_INTR_MODE' to configure the mcu EXTINT
//#undef DRV_INTR_MODE

#define FORCE_MODE_10M
//#undef FORCE_MODE_10M

#define FORCE_CHKSUM_OFFLOAD
//#undef FORCE_CHKSUM_OFFLOAD

/* __USR_H
 */

#include "dm9051opts_define.h"

#endif //__DM9051_OPTS_H
