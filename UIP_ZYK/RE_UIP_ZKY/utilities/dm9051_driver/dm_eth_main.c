/**
  **************************************************************************
  * @file     dm_eth.c
  * @file     dm_eth_main.c
  * @version  v1.0.0
  * @date     2024-06-24
  * @brief    as eth.c
  **************************************************************************
  */
//#include "stdio.h"
//#include "DM9051_M051.h"
//#include "uip.h"
#include "uip-conf.h"
#include "uip.h"

#include "dm9051opts.h"
#include "cboard/dm9051_lw_mcu_default_IN.h"
#include "cboard/dm9051_cstate.h"
//#include "dm_eth/dm_eth.h"

#include "cboard/dm9051_cboard_data_API.h"

// return : 0, no error
//			1, last packet
//			2, No packet data (error)
//int ethernetif_input_intr(uint16_t *max_allow_len) {
//	uip_len = DM9051_rx();
//  return 0;
//}
//static int net_pkts_handle_intr(void) {
//		ret = ethernetif_input_intr(&max_allow_len); n++;
//	return n;
//}
//void net_inp_interrupt(void) {
//		isrSemaphore_n = net_pkts_handle_intr();
//}
//void DM_ETHER_Receive_Task(void* param) {
//	if (_flgSemaphore_r == 1) {
//		net_inp_interrupt(); //ethernetif_inp_interrupt0();
//	}
//}

int flgSemaphore_r = 0;

void dm_eth_intr_event(void)
{
	#ifdef DM9051_DRIVER_INTERRUPT

	flgSemaphore_r = 1;
//DM_ETHER_Receive_Task(NULL);

	#endif
}

void DM_ETH_InterruptHdlr(void)
{
	dm_eth_intr_event();
	inc_task_tryint();
//	freertos_task_clearpoll_event();
}

int32_t DM9051_init(void)
{
    //DM9051_Configuration_NU();
	dm9051_boards_initialize();
	
	dm9051_init(&uip_ethaddr.addr[0]);
	return 0;
}

uint16_t DM9051_rx(void)
{
	return dm9051_rx(uip_buf);
}

uint32_t DM9051_tx(void)
{
	dm9051_tx(uip_buf, uip_len);
	return 0;
}
