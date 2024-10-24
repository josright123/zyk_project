#ifndef __ETH_PRINT_OPTS_H
#define __ETH_PRINT_OPTS_H
#include "../config/dm_eth_opts.h"

#if freeRTOS
//enum sema_tag_t {
//	SEMA_OFF = 0,
//	SEMA_ON,
//};


//#define	PRINT_SEMA	SEMA_OFF	//ON to print with sema, OFF without sema.
#define	PRINT_DRV	" .drv " //"ap"
//#define PRINT_INFO	"[DM9051 INFO] "
#define PRINT_INFO	"[DM9051 DRV] "
#define PRINT_INFO_SP	" "

#define	PRINT_INFO_IRQ	"[DM9051 IRQ] "

#endif

#endif //__ETH_PRINT_OPTS_H
