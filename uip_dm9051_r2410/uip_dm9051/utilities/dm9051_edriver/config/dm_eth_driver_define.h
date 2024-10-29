#ifndef __DM_ETH_DRVDEF_H__
#define	__DM_ETH_DRVDEF_H__
#include "../config/dm9051opts.h"

//[5] dependency
#ifdef freeRTOS_CONF
#define freeRTOS														freeRTOS_CONF
#else
#define freeRTOS														0
#endif

#if freeRTOS
#include "FreeRTOS.h" //..................nbnj......
#include "task.h"
#include "queue.h"
#include "semphr.h"
//#warning "freeRTOS is defined"
	
#else
//#warning "freeRTOS is NOT defined"
	
#endif
#endif //__DM_ETH_DRVDEF_H__
