/**
  **************************************************************************
  * @file     dm_eth.c
  * @file     dm_eth_main.c
  * @version  v1.0.0
  * @date     2024-06-24
  * @brief    as eth.c
  **************************************************************************
  */
#include "uip-conf.h"
#include "uip.h"

#include "dm9051opts.h"
#include "cboard/dm9051_lw_mcu_default_IN.h"
#include "cboard/dm9051_cstate.h"

#include "cboard/dm9051_cboard_data_API.h"

#if defined(DM9051_DRIVER_INTERRUPT)
int flgSemaphore_r;
#endif

#if DM_ETH_DEBUG_MODE
uint16_t DM_ETH_ToCalc_rx_pointers(int state, uint16_t *mdra_rd_org, uint16_t *mdra_rdp)
{
	static uint16_t dummy_rwpa_wts /*,rwpa_wts, mdra_rds*/;

	//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
	dm9051_read_rx_pointers(&dummy_rwpa_wts, mdra_rdp);
	//DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));

	if (state == 0) {
		//rwpa_wts = *rwpa_wtp;
		//mdra_rds = *mdra_rdp;
		return 0;
	}

	//diffs = _wrpadiff(rwpa_wts, *rwpa_wtp);
	//diffs = _wrpadiff(mdra_rds, *mdra_rdp);
	return wrpadiff(*mdra_rd_org, *mdra_rdp);
}
#endif

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

int dm_eth_get_intr_event(int r) {
	//if (r)
	flgSemaphore_r = 0;
	return r;
}

int DM_ETH_ToGet_InterruptEvent(void)
{
	return dm_eth_get_intr_event(flgSemaphore_r);
}

void dm_eth_set_intr_event(void) {
	#ifdef DM9051_DRIVER_INTERRUPT

	flgSemaphore_r = 1;
//DM_ETHER_Receive_Task(NULL);

	#endif
}

void DM_ETH_InterruptHdlr(void)
{
	dm_eth_set_intr_event();
	inc_task_tryint();
//	freertos_task_clearpoll_event();
}

int32_t DM_ETH_Init(void) //DM9051_init(void)
{
    //DM9051_Configuration_NU();
	dm9051_boards_initialize();
	
	dm9051_init(&uip_ethaddr.addr[0]);
	return 0;
}

uint16_t DM_ETH_Input(void) //DM9051_rx(void)
{
	return dm9051_rx(uip_buf);
}

uint32_t DM_ETH_Output(void) //DM9051_tx(void)
{
	dm9051_tx(uip_buf, uip_len);
	return 0;
}
