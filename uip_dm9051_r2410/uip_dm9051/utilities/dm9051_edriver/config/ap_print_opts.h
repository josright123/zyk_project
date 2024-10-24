#ifndef __ETH_PRINT_OPTS_H
#define __ETH_PRINT_OPTS_H
#include "../config/dm_eth_opts.h"

#if freeRTOS
//enum sema_tag_t {
//	SEMA_OFF = 0,
//	SEMA_ON,
//};


#define	PRINT_SEMA	SEMA_OFF //SEMA_ON	//ON to print with sema, OFF without sema.
#define	PRINT_AP	"[ap] " //"ap"
#define PRINT_INFO_AP	"[DM9051 AP] "
#define PRINT_INFO_APIN	"[DM9051 APIN] "

#define	PRINT_INFO_IRQ	"[DM9051 IRQ] "

#endif

#endif //__ETH_PRINT_OPTS_H
