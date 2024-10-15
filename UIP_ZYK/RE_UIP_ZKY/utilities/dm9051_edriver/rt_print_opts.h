#ifndef __PRINT_OPTS_H
#define __PRINT_OPTS_H
#include "../dm_eth_opts.h"

#if freeRTOS
//enum sema_tag_t {
//	SEMA_OFF = 0,
//	SEMA_ON,
//};

#define	PRINT_SEMA	SEMA_ON	//ON to print with sema, OFF without sema.
#define	PRINT_RT	"[RT] " //"rt"	
#define	PRINT_AP	"[ap] " //"ap"
#define PRINT_INFO_RT	"[DM9051 RT] "
#define PRINT_INFO_AP	"[DM9051 AP] "

#endif

#endif //__PRINT_OPTS_H
