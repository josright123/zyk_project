/**
 **************************************************************************
 * @file     dm_eth_task_main.c
 * @version  v1.0.1
 * @date     2024-10-08
 * @brief    DM9051 Ethernet driver main file (or referred to be as eth.c)
 **************************************************************************
 *
 * To restructure and improve the file to enhance readability, maintainability,
 * and potentially performance.
 * Last updated: 20240905
 *
 */
#include "cmsis_compiler.h"               /* CMSIS compiler specific defines */
	//[lwip]
#include "lwip/pbuf.h"
#include "lwip/netif.h" //#include "lwip/err.h"
#include "ethernetif.h"
#include "netconf.h"
#include "dm9051_rt_debug.h"
#define printk printf

#include "dm_eth_lock.h" //#include "dm9051_rtos.h"
xSemaphoreHandle lock_dm9051_core;

#if freeRTOS
/* Declare the mutex handle globally so it can be accessed in protect spi sync.
 */
//extern xSemaphoreHandle lock_dm9051_core;

static xSemaphoreHandle mutex_create_start(void) {
	xSemaphoreHandle sema;
	sema = xSemaphoreCreateMutex();
	//if (!sema)
	//  printf("init, failed to create a sema\r\n"); //(ERR_MEM)
	return sema;
}
		
static void dm_eth_create_mutexes(void) {
  DM_ETH_MUTEX_CFG((freeRTOS), "'lock_dm9051' be created\r\n", lock_dm9051_core, mutex_create_start());
  DM_ETH_MUTEX_CFG((freeRTOS & qprint), "'mutex_print' be created\r\n", mutex_print, tasks_dm9051_debug_init()); //how to disable!
}

static int inHandlerMode (void);
static void sys_mutex_lock_start(xSemaphoreHandle *sema_p)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	if(inHandlerMode())
	{
		xSemaphoreTakeFromISR(*sema_p, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
	else
	{
		xSemaphoreTake(*sema_p, portMAX_DELAY);
	}
}

static void sys_mutex_unlock_end(xSemaphoreHandle *sema_p)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	if(inHandlerMode())
	{
		xSemaphoreGiveFromISR(*sema_p, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
	else
	{
		xSemaphoreGive(*sema_p);
	}
}

/* Determine whether we are in thread mode or handler mode.
 */
static int inHandlerMode (void)
{
  return __get_IPSR() != 0;
}
#endif /* freeRTOS */



void DM_Eth_LwipInitialize(void)
{
	dm_eth_create_mutexes();
	tcpip_stack_init();
	httpd_init();
}

	//[lwip]
void DM_ETH_Init_W(struct netif *netif, const uint8_t *adr)
{
	const uint8_t *pd;

	#if DM_ETH_DEBUG_MODE
	if (adr) {
		printf("\r\n");
		printf("DM_ETH_Init:\r\n");
		printf("DM_ETH_Init: mac %02x:%02x:%02x:%02x:%02x:%02x\r\n", adr[0], adr[1], adr[2], adr[3], adr[4], adr[5]);
	}
	#endif

	DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core)); 
	pd = DM_ETH_Init(adr); //dm9051_init_nondual(lwip_set_MACaddr); //dm9051_init_dual(); //_dm9051_init(MACaddr);
	DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));

	netif->hwaddr[0] =  pd[0];
	netif->hwaddr[1] =  pd[1];
	netif->hwaddr[2] =  pd[2];
	netif->hwaddr[3] =  pd[3];
	netif->hwaddr[4] =  pd[4];
	netif->hwaddr[5] =  pd[5];

	#if DM_ETH_DEBUG_MODE
	printf("DM_ETH_Init: out netif->hwaddr %02x:%02x:%02x:%02x:%02x:%02x\r\n", pd[0], pd[1], pd[2], pd[3], pd[4], pd[5]);
	#endif
}

struct pbuf *DM_ETH_Input_W(void) //DM9051_rx(void)
{
  struct pbuf *p, *q;
  u16_t len = 0;
  int l =0;
  uint8_t *buffer = get_ReceiveBuffer();

  DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
  len = DM_ETH_Input(buffer); //uip_len = DM_ETH_Input(uip_buf); //dm9051_rx(uip_buf);
  DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));
  if (!len)
	  return NULL;
	
  /* We allocate a pbuf chain of pbufs from the pool. */
  p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
  if (p != NULL)
  {
    for (q = p; q != NULL; q = q->next)
    {
      memcpy((u8_t*)q->payload, (u8_t*)&buffer[l], q->len);
      l = l + q->len;
    }
  }
  return p;
}

void DM_ETH_Output_W(struct pbuf *p) // DM9051_tx(void)
{
//	dm9051_tx(uip_buf, uip_len);
//	return 0;
  uint8_t *buffer;
  struct pbuf *q;
  int l = 0;

  buffer = get_TransmitBuffer();
  for(q = p; q != NULL; q = q->next)
  {
    memcpy((u8_t*)&buffer[l], q->payload, q->len);
    l = l + q->len;
  }
  DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
  DM_ETH_Output(buffer, (uint16_t) l); //dm9051_tx
  DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));
  //return ERR_OK;
}

// Reset functionality
void DM_ETH_ToRst_ISR_W(void)
{
  DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
	DM_ETH_ToRst_ISR(); //cspi_isr_enab(); // dm9051_write_rst_isr();
  DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));
}

// DM_Eth_GetStatus: cid/bmsr/ncr_nsr
void DM_Eth_ReadRegsInfo_W(uint8_t *stat)
{
  DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
	DM_Eth_ReadRegsInfo(stat);
  DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));
}

// Debug functionality
#if DM_ETH_DEBUG_MODE
uint16_t DM_ETH_ToCalc_rx_pointers_W(int state, uint16_t *mdra_rd_org, uint16_t *mdra_rdp)
{
	uint16_t diff;
  DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
	diff = DM_ETH_ToCalc_rx_pointers(state, mdra_rd_org, mdra_rdp);
  DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));
	return diff;
}
#endif

#if DM_ETH_DEBUG_MODE
/*static*/ //extern uint16_t wrpadiff(uint16_t rwpa_s, uint16_t rwpa_e);

uint16_t DM_ETH_Diff_rx_pointers_W(int stamp, uint16_t *rwpa_wtp, uint16_t *mdra_rdp)
{
	static uint16_t /*rwpa_wts,*/ mdra_rds;

	DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core));
	
	//cspi_read_rx_pointers(rwpa_wtp, mdra_rdp);
	DM_ETH_ToCalc_rx_pointers(0, rwpa_wtp, mdra_rdp);
	
	DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core));

	if (stamp == 0) {
		//rwpa_wts = *rwpa_wtp;
		mdra_rds = *mdra_rdp;
		return 0;
	}

	//diffs = _wrpadiff(rwpa_wts, *rwpa_wtp);
	//diffs = _wrpadiff(mdra_rds, *mdra_rdp);
	return wrpadiff(mdra_rds, *mdra_rdp);
}
#endif

#define SD_100M	0x20
#define SD_10M	0x10
#define SD_FULL	0x02
#define SD_HALF	0x01
static uint8_t speed_duplex = 0;

void DM_Eth_Show_status(char *head, uint8_t *statdat, int force)
{
#undef printf
#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_NETCONF_ON, SEMA_ON, " [XIF] ", (fmt, ##__VA_ARGS__))
	//uint8_t statdat[6];
	uint8_t speeddupd = 0; //_low_level_get_id_bmsr(netif, statdat);
	
	if (DM_Eth_Regs_Info_Linkup(statdat)) {
		//do {
		if (statdat[0] & 0x08)
			speeddupd |= SD_FULL;
		else
			speeddupd |= SD_HALF;
		if (statdat[1] & 0x80)
			speeddupd |= SD_10M;
		else
			speeddupd |= SD_100M;

		if ((speeddupd != speed_duplex) || force) {
			speed_duplex = speeddupd;

			printf("%s Link up as %s %s\r\n",
				head, //"Eth Status"
				speeddupd & SD_100M ? "100M" : speeddupd & SD_10M ? "10M" : "UN_SPEED",
				speeddupd & SD_FULL ? "Full" : speeddupd & SD_HALF ? "Half" : "UN_DUPLEX"
				);
		}
		//} whilw(0);
	}
}
