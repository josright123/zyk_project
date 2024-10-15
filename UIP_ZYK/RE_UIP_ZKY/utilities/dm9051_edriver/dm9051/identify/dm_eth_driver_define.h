#include "../dm_eth_opts.h"

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
#warning "freeRTOS is defined"
	
#else
#warning "freeRTOS is NOT defined"
	
#endif
