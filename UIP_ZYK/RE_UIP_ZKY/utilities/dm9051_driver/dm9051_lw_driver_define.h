/* 
 * dm9051_lw_driver_define.h
 */
#ifndef __DM9051_LW_DRIVER_DEF_H
#define __DM9051_LW_DRIVER_DEF_H

#include "dm9051opts.h" //for keil-c editor to recognize designed macro define!

//[0]
#ifdef DRV_INTR_MODE
#define INTERRUPT_HANDLER_ON										1 //0, 1 when 'DRV_INTR_MODE', accept INT active (default active low) trigger.
#define INTERRUPT_HANDLER_SUPPLIMENT						0 //0, 1 when 'DRV_INTR_MODE', poll suppliment.
#define INTERRUPT_HANDLER_SUPPLIMENT_RECV				0 //0, 1 when fire_polling do receive.
#else
#define INTERRUPT_HANDLER_SUPPLIMENT_RECV				1 //application fixed to 1.
#endif

//[1]
#ifdef DRV_INTR_MODE
#define	DM9051_DRIVER_INTERRUPT
#else
#define	DM9051_DRIVER_POLL
#endif

#define POLL_ON_FORCE			(1 << 0)
#define POLL_ON_LINK			(1 << 1)  //WILL BE LATE, OR NOT EMPTYING RXFIFO
#define POLL_ON_RXPKT			(1 << 2)
#define POLL_ON_RXPADIFF	(1 << 3)
#define	POLL_ON						POLL_ON_FORCE //POLL_ON_RXPKT

//[2]
#if 1
/*  C(2, 0), C(2, 1)
 * [plain/genny]= [0/0, 1/0, 0/1]
 */
#define RX_HANDLER_ACCEPTED_TAKE_PLAIN_SEM			0 //[plain/genny]= [0/0, 1/0, 0/1]
#define RX_HANDLER_ACCEPTED_TAKE_GENNY_SEM			0
#endif

//[3]
#define MAC_ADDR_LENGTH                  		(6)
#define ADDR_LENGTH                      		(4)

//[4]
#if 1
/*  Check LINK state in source 
 * Use [NSR/NSR] 1
 * Use [BMSR]    2
 */
#define DM9051_CHECK_MAC										1
#define DM9051_CHECK_PHY										2

#define LINK_STATE_SOURCE										DM9051_CHECK_MAC
#endif

#endif //__DM9051_LW_DRIVER_DEF_H
