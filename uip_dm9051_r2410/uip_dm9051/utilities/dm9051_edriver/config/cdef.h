/* def.h
 */
#ifndef __DM_DEF_H__
#define	__DM_DEF_H__
#include <stdint.h>

/* domain */
#define MAC_ADDR_LENGTH                  		(6)
#define ADDR_LENGTH                      		(4)

/* struct define */
  struct eth_node_t
  {
    uint8_t mac_addresse[MAC_ADDR_LENGTH];
    uint8_t local_ipaddr[ADDR_LENGTH];
    uint8_t local_gwaddr[ADDR_LENGTH];
    uint8_t local_maskaddr[ADDR_LENGTH];
  };

	/* types define : [ identify.h/identify.c ] */
	typedef uint8_t mac_t[MAC_ADDR_LENGTH];
	typedef uint8_t ip_t[ADDR_LENGTH];

/* types define */
  typedef enum
  {
    CS_EACH = 0,
    CS_LONG,
  } csmode_t;

/* domain */
#define	uipAPP_CONF	1
#define	lwipAPP_CONF	2

/* hal */
#include "../hal/dm9051_hal.h" //such as designed a MCU
  
/* eth_main */
//#define	DM_ETH_IRQHandler	EXINT9_5_UserFunction	//EXINT9_5_IRQHandler
	
#endif //__DM_DEF_H__
