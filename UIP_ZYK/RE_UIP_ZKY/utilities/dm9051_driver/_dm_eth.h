#ifndef __DM_ETH_H__
#define __DM_ETH_H__

//#include "lwip/netif.h"
//#include "dm9051opts.h"

#include "dm_eth_user_default.h"
#include "dm_eth_driver_define.h"

void DM_ETH_Init(struct netif *netif, const uint8_t *adr);
struct pbuf *DM_ETH_Input(void);
err_t DM_ETH_Output(struct netif *netif, struct pbuf *p);
int DM_Eth_LinkBitStatus(uint8_t *stat);
void DM_Eth_ReadRegsInfo(uint8_t *stat);
void DM_Eth_Input_HexDumpReset(uint8_t *stat);
#if ETHERNET_INTERRUPT_MODE
extern void DM_ETH_InterruptHdlr(void);
int DM_ETH_InterruptHdlr_Supplement(void);
#endif
void DM_ETH_SHOW_CONSECUTIVE_Packets(void);
void DM_Eth_Probe_link(int nsr_poll);
void DM_Eth_Poll(void);

void DM_Eth_LwipInitialize(void);
void DM_Eth_CreateRecvTask(void); //dm_eth_b.h
//[dm_eth_status.c]
#if DM_ETH_DEBUG_MODE
void dm_eth_input_hexdump_reset(void);
void dm_eth_input_hexdump(const void *buf, size_t len);
#endif

#if 0
void net_inp_interrupt(void);
void net_inp_poll(void);
#endif

//void dm_eth_poll_event(void); //for eventually, call to 'net_inp_poll()'

//extern SemaphoreHandle_t _xSemaphore_RX;
//extern SemaphoreHandle_t _g_dm9051_interrupt_event;
//extern int _flgSemaphore_r;
//static void dm_eth_config_interrupt_polling_log(void);
//static void dm_eth_create_mutexes(void);
//.void LWIP_Initalize(void);
//static void UIP_Initalize(void);
//void DM_Eth_SemaphoreNewbinary(void);
//int DM_Eth_SemaphoreTakeBinary(void);
//static void dm_eth_semaphore_renew(void);
/*uint16_t DM_ETH_IRQEnable(void);*/
/*uint16_t DM_ETH_IRQDisable(void);*/

#endif //__DM_ETH_H__
