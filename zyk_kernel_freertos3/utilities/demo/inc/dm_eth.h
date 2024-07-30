#ifndef __DM_ETH_H__
#define __DM_ETH_H__

typedef enum {FALSE = 0, TRUE = !FALSE} confirm_state;

extern SemaphoreHandle_t xSemaphore_RX;
extern int flgSemaphore_r;

void DM_Eth_Initialize(void);
//.static void LWIP_Initalize(void);

//.static void DM_ETH_RXHandler(void* param);
//.err_t DM_ETH_Output(struct netif *netif, struct pbuf *p);
void DM_ETH_IRQHandler(void);

void DM_ETH_IRQEnable(void);
uint16_t DM_ETH_IRQDisable(void);

#endif //__DM_ETH_H__
