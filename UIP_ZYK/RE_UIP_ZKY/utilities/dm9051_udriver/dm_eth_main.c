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

//#include "dm9051_env.h"
#include "dm9051_lw.h"
#include "cboard/dm9051_Hw_api.h"

//#if defined(DM9051_DRIVER_INTERRUPT)
int flgSemaphore_r;
//#endif

int dm_eth_get_intr_event(int r)
{
	flgSemaphore_r = 0;
	return r;
}

void dm_eth_set_intr_event(void)
{
	//#ifdef DM9051_DRIVER_INTERRUPT
	flgSemaphore_r = 1;
	//#endif
}

int DM_ETH_ToGet_InterruptEvent(void)
{
	return dm_eth_get_intr_event(flgSemaphore_r);
}

void DM_ETH_ToSet_InterruptEvent(void)
{
	dm_eth_set_intr_event();
	inc_task_tryint();
	// freertos_task_clearpoll_event();
}

void DM_ETH_InterruptHdlr(void)
{
	cint_exint9_5_handler();
}

int32_t DM_ETH_Init(void) // DM9051_init(void)
{
	// DM9051_Configuration_NU();
	dm9051_boards_initialize();

	dm9051_init(&uip_ethaddr.addr[0]);
	return 0;
}

uint16_t DM_ETH_Input(void) // DM9051_rx(void)
{
	return dm9051_rx(uip_buf);
}

uint32_t DM_ETH_Output(void) // DM9051_tx(void)
{
	dm9051_tx(uip_buf, uip_len);
	return 0;
}

void DM_ETH_ToRst_ISR(void)
{
	dm9051_write_rst_isr();
}

#if DM_ETH_DEBUG_MODE
uint16_t DM_ETH_ToCalc_rx_pointers(int state, uint16_t *mdra_rd_org, uint16_t *mdra_rdp)
{
	static uint16_t dummy_rwpa;

	// DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
	dm9051_read_rx_pointers(&dummy_rwpa, mdra_rdp);
	// DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));

	if (state == 0)
		return 0;

	return wrpadiff(*mdra_rd_org, *mdra_rdp);
}
#endif
