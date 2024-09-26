#ifndef __DM_ETH_H__
#define __DM_ETH_H__

//uint16_t rx_wrapper_read(void);

//extern SemaphoreHandle_t xSemaphore_RX;
//typedef QueueHandle_t SemaphoreHandle_t;
//extern QueueHandle_t xSemaphore_RX;
#if defined(DM9051_DRIVER_INTERRUPT)
extern int flgSemaphore_r;
#endif

//void DM_Eth_Open(void);

//void DM_ETH_Init(const uint8_t *adr);

//uint16_t DM_ETH_RXHandler(void) ;
//uint16_t DM_ETH_RXHandler_Poll(void);
////uint16_t _DM_ETH_RXHandler(uint8_t *buf);

////.void LWIP_Initalize(void);
////.void UIP_Initalize(void);
////.void _DM_ETH_RXHandler(void* param);

//uint16_t DM_ETH_Input(uint8_t *buf);
////.err_t DM_ETH_Output(struct netif *netif, struct pbuf *p);
//void DM_ETH_Output(uint8_t *buf, uint16_t len);

//void DM_ETH_IRQHandler(void);

//void DM_ETH_IRQEnable(void);
//uint16_t DM_ETH_IRQDisable(void);

int32_t DM9051_init(void);
uint16_t DM9051_rx(void);
uint32_t DM9051_tx(void);

#endif //__DM_ETH_H__
