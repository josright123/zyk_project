#ifndef __DM9051_LW_USR_H
#define __DM9051_LW_USR_H

#define freeRTOS_CONF	1 //0
#define freeRTOS_ENABLE_MUTEX	0 //1 //0

#define MCU_SPECIFY_CONFIG

//#define intrAPP_READY
#undef intrAPP_READY //changed

#define _AT32F437xx

#if freeRTOS_CONF
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#endif

#endif //__DM9051_LW_USR_H
