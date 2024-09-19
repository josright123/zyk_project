/***************************************************/
/* File: DM9051_spi_fun.c                           */
/* Author: DAVICOM                                 */
/***************************************************/
//#include "dm9051opts.h"
//#include "dm9051_lw.h"

#define uIP_NOOS             /* uIP driver, if not for LwIP driver */ //jerry add
//#include "DM9051.h"

#include "stdio.h"
#include "stdint.h"
#include "uip-conf.h"
#include "uip.h"
//#include "sl_udelay.h"
//#include "sl_spidrv_instances.h"
//#include "RttPrintf.h"

#define DM9051_RESET_PORT gpioPortB
#define DM9051_RESET_PIN 3

int DM9051_INT_FLAG;

//void DM9051_Configuration(void) {
//printf("DM9051_Configuration.s\r\n");
//	dm9051_boards_initialize();
//#ifdef DM9051_INT
//#endif
//}

//int32_t DM9051_INIT(void)
//{
//	const uint8_t *mac;
//	//printf("In mac %02x:%02x:%02x:%02x:%02x:%02x\r\n", 0,0,0,0,0,0);
//	printf("In mac %02x:", 0);
//	printf(" %02x:", 0);
//	printf(" %02x:", 0);
//	printf(" %02x:", 0);
//	printf(" %02x:", 0);
//	printf(" %02x\r\n", 0);
//	mac = _dm9051_init(NULL);
//	//printf("Out mac %02x:%02x:%02x:%02x:%02x:%02x\r\n", mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
//	printf("Out mac %02x:", mac[0]);
//	printf(" %02x:", mac[1]);
//	printf(" %02x:", mac[2]);
//	printf(" %02x:", mac[3]);
//	printf(" %02x:", mac[4]);
//	printf(" %02x\r\n", mac[5]);
//	return 0;
//}

//uint16_t DM9051_RX(void)
//{
//	return _dm9051_rx((uint8_t *)uip_buf);
//}

//#ifdef uIP_NOOS
//    uint32_t DM9051_TX(void)
//#else
//    uint32_t DM9051_TX(struct netif *netif, struct pbuf *p) ...
//#endif //uIP_NOOS
//{
//	dm9051_tx((uint8_t *)uip_buf, uip_len);
//    return 0;
//}

#ifdef DM9051_INT
//void DM9051_IRQHandler(void)
//{
//	//    if (GPIO_GET_INT_FLAG(PD, BIT2)){
//	//        GPIO_CLR_INT_FLAG(PD, BIT2);
//	//        DM9051_INT_FLAG = 1;
//	//    }
//}
#endif

//static void _DM9051_Delay(uint32_t uDelay)
//{
////    sl_udelay_wait(uDelay);
//}

//static void _DM9051_Delay_ms(uint32_t uDelay)
//{
////  sl_udelay_wait(uDelay*1000);
//}
