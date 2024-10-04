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
#include "cboard/dm9051_Hw_api.h" //[dm9051_boards_initialize()/cint_exint9_5_handler()]
#include "dm_identify_impl.h" //[h file implement]

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
	uip_len = dm9051_rx(uip_buf);
	dm_eth_input_hexdump(uip_buf, uip_len); //"dm_eth_status.c"
	return uip_len;
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

void DM_ETH_IpConfiguration(uint8_t *ip, uint8_t *gw, uint8_t *mask)
{
    uip_ipaddr_t ipaddr;
    uint8_t *host_ip, *gateway, *netmask;

    // Set Host IP address
    host_ip = identify_tcpip_ip(ip);
    uip_ipaddr(&ipaddr, host_ip[0], host_ip[1], host_ip[2], host_ip[3]);
    uip_sethostaddr(ipaddr);

    // Set Default Gateway
    gateway = identify_tcpip_gw(gw);
    uip_ipaddr(&ipaddr, gateway[0], gateway[1], gateway[2], gateway[3]);
    uip_setdraddr(ipaddr);

    // Set Network Mask
    netmask = identify_tcpip_mask(mask);
    uip_ipaddr(&ipaddr, netmask[0], netmask[1], netmask[2], netmask[3]);
    uip_setnetmask(ipaddr);
}

// DM_Eth_GetStatus: cid/bmsr/ncr_nsr
void DM_Eth_ReadRegsInfo(uint8_t *stat)
{
	uint16_t cs;
	uint32_t pbm;

#if 0 // to do
	DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
#endif
	pbm = dm9051_read_bmsr();
	pbm |= dm9051_read_chip_id() << 16;
	cs = dm9051_read_control_status();
#if 0 // to do
	DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));
#endif

	stat[0] = cs & 0xff;
	stat[1] = (cs >> 8) & 0xff;

	stat[2] = (pbm >> 24) & 0xff;
	stat[3] = (pbm >> 16) & 0xff;
	stat[4] = (pbm >> 8) & 0xff;
	stat[5] = (pbm) & 0xff;
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
