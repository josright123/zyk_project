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
#define DM9051_MUTEX_CFG(expression, msg, sem, handler) do { if ((expression)) { \
	printf("%s", msg); \
	sem = handler; \
	if (!sem) { \
	  printf("dm9051, failed to '%s'\r\n", msg); \
	  while(1); /*Handle the error, e.g., by entering an infinite loop*/ \
	  /*return NULL;*/ \
	} \
  }} while(0)
#warning "freeRTOS is defined"
#else
#define DM9051_MUTEX_CFG(expression, msg, sem, handler)
#warning "freeRTOS is NOT defined"
#endif

//#if freeRTOS
//#else
//#endif

