#ifndef __DM_ETH_H__
#define __DM_ETH_H__
#include "control/dm9051opts.h"

//[uip support version]
const uint8_t *DM_ETH_Init(const uint8_t *adr);
uint16_t DM_ETH_Input(uint8_t *bff);
void DM_ETH_Output(uint8_t *bff, uint16_t len);
const uint8_t *DM_ETH_Ip_Configuration(const uint8_t *ip);
const uint8_t *DM_ETH_Gw_Configuration(const uint8_t *ip);
const uint8_t *DM_ETH_Mask_Configuration(const uint8_t *ip);
uint8_t *DM_ETH_Ip_Configured(void);
uint8_t *DM_ETH_Gw_Configured(void);
uint8_t *DM_ETH_Mask_Configured(void);
int DM_Eth_Regs_Info_Linkup(uint8_t *stat);
void DM_Eth_ReadRegsInfo(uint8_t *stat);
int dm_eth_polling_downup(void);
void dm_eth_show_app_help_info(char *contentStr);
const uint8_t *dm_eth_show_identified_ip(char *headtypestr);
const uint8_t *dm_eth_show_identified_gw(char *headtypestr);

enum operate_tag {OPS_LED3 = 0, };
void dm_eth_polling_button_init(enum operate_tag tag);
void dm_eth_polling_button_ops(enum operate_tag tag);

#ifdef ETHERNET_INTERRUPT_MODE
int DM_ETH_GetInterruptEvent(void);
void DM_ETH_ToRst_ISR(void);
#endif

uint16_t DM_ETH_ToCalc_rx_pointers(int state, const uint16_t *mdra_rd_org, uint16_t *mdra_rdp);

#endif //__DM_ETH_H__
