#ifndef __DM_ETH_H__
#define __DM_ETH_H__
#include "../config/dm9051opts.h"

#if ETHERNET_INTERRUPT_MODE
int DM_ETH_GetInterruptEvent(void);
//void DM_ETH_SetInterruptEvent(void);
void DM_ETH_ToRst_ISR(void);
#endif

#if DM_ETH_DEBUG_MODE
uint16_t DM_ETH_ToCalc_rx_pointers(int state, uint16_t *mdra_rd_org, uint16_t *mdra_rdp);
#endif

//[uip]
const uint8_t *DM_ETH_Init(const uint8_t *adr);
uint16_t DM_ETH_Input(uint8_t *bff);
void DM_ETH_Output(uint8_t *bff, uint16_t len);
//void DM_ETH_IpConfiguration(uint8_t *ip, uint8_t *gw, uint8_t *mask);
uint8_t *DM_ETH_Ip_Configuration(const uint8_t *ip);
uint8_t *DM_ETH_Gw_Configuration(const uint8_t *ip);
uint8_t *DM_ETH_Mask_Configuration(const uint8_t *ip);
uint8_t *DM_ETH_Ip_Configured(void);
uint8_t *DM_ETH_Gw_Configured(void);
uint8_t *DM_ETH_Mask_Configured(void);
void DM_Eth_ReadRegsInfo(uint8_t *stat);
int DM_Eth_Regs_Info_Linkup(uint8_t *stat);
//void DM_Eth_Input_HexDumpReset(uint8_t *stat);
extern int flgSemaphore_r;

#if 0
//#if 0
//	//[Lwip]
//	void DM_ETH_Init(struct netif *netif, const uint8_t *adr);
//	struct pbuf *DM_ETH_Input(void);
//	err_t DM_ETH_Output(struct netif *netif, struct pbuf *p);
//	
//	#if ETHERNET_INTERRUPT_MODE
//	int DM_ETH_InterruptHdlr_Supplement(void);
//	#endif
//	void DM_ETH_SHOW_CONSECUTIVE_Packets(void);
//	void DM_Eth_Probe_link(int nsr_poll);
//	void DM_Eth_Poll(void);

//	void DM_Eth_CreateRecvTask(void); //dm_eth_b.h
//#endif

//#if 0
//void net_inp_interrupt(void);
//void net_inp_poll(void);
//#endif

//void dm_eth_poll_event(void); //for eventually, call to 'net_inp_poll()'
//extern SemaphoreHandle_t _xSemaphore_RX;
//extern SemaphoreHandle_t _g_dm9051_interrupt_event;
//extern int _flgSemaphore_r;
//static void dm_eth_config_interrupt_polling_log(void);
//.void LWIP_Initalize(void);
//static void UIP_Initalize(void);
//void DM_Eth_SemaphoreNewbinary(void);
//int DM_Eth_SemaphoreTakeBinary(void);
//static void dm_eth_semaphore_renew(void);
/*uint16_t DM_ETH_IRQEnable(void);*/
/*uint16_t DM_ETH_IRQDisable(void);*/
#endif //0
#endif //__DM_ETH_H__
