#ifndef __DM_ETH_TSK_MAIN_H__
#define __DM_ETH_TSK_MAIN_H__

//[lwip]
void DM_Eth_LwipInitialize(void);

void DM_ETH_Init_W(struct netif *netif, const uint8_t *adr);
struct pbuf *DM_ETH_Input_W(void);
void DM_ETH_Output_W(struct pbuf *p);

void DM_ETH_ToRst_ISR_W(void);
void DM_Eth_ReadRegsInfo_W(uint8_t *stat);

uint16_t DM_ETH_ToCalc_rx_pointers_W(int state, uint16_t *mdra_rd_org, uint16_t *mdra_rdp);
uint16_t DM_ETH_Diff_rx_pointers_W(int stamp, uint16_t *rwpa_wtp, uint16_t *mdra_rdp);

void DM_Eth_Show_status(char *head, uint8_t *statdat, int force);

#endif //__DM_ETH_TSK_MAIN_H__
