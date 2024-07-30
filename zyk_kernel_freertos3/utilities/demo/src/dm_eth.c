/**
  **************************************************************************
  * @file     dm_eth.c
  * @version  v1.0.0
  * @date     2024-06-24
  * @brief    as eth.c
  **************************************************************************
  */
//#include "at32f435_437_board.h"
//#include "at32f435_437_clock.h"
#include "stdio.h"
//#include "FreeRTOS.h"
//#include "task.h"
#include "ethernetif.h"
#include "netconf.h"
#include "dm9051opts.h"
#include "dm9051_lw.h"
#include "dm9051_cboard.h"
#include "dm9051_lw_cint.h"
#include "dm_eth.h"

#include "../freertos_tasks_debug.h"
#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_NETCONF_ON, /*SEMA_OFF*/ SEMA_ON, "[E]", (fmt, ##__VA_ARGS__))

#if 1
int link_log_reset_allow_num = 0;
const int rx_modle_log_reset_allow_num = 3;

void ethernetif_link_log_reset(void) {
	link_log_reset_allow_num = 0; // rx_modle_count[RX_ANY].allow_num = 0;
}

void ethernetif_link_log_reset_hexdump(const void *buf, size_t len) {
	#if 1
	int get_tcpip_thread_state(void);
	void set_tcpip_thread_state(int state); //.temp //extern int tcpip_thread_init;
	#endif
	LWIP_UNUSED_ARG(buf);
	LWIP_UNUSED_ARG(len);
	
	if (link_log_reset_allow_num < rx_modle_log_reset_allow_num) {
			link_log_reset_allow_num++;
			if (link_log_reset_allow_num == rx_modle_log_reset_allow_num && get_tcpip_thread_state() == 1) {
				set_tcpip_thread_state(6); //tcpip_thread_init = 6; //temp
			}
	}
}
#endif

int flgSemaphore_r = 0;
SemaphoreHandle_t xSemaphore_RX = NULL;
TaskHandle_t network_rx_handler;

static void New_Semaphore(void) {
	printk("\r\n");
	printk("\r\n");
	printf("DM_Eth_Initalize.s\r\n");
	xSemaphore_RX = xSemaphoreCreateBinary();
	xSemaphoreTake(xSemaphore_RX, ( TickType_t ) 0 );
	flgSemaphore_r = 0;
}

static void LWIP_Initalize(void) {
#if 1
	printf("LWIP_Initalize.s\r\n");
	tcpip_stack_init();
	httpd_init();
#else
  printf("UIP_Initalize.s\r\n");
#endif
}

//static void DM_Eth_Open(void) {
//#if 1
//	LWIP_Initalize();
//#else
//	UIP_Initalize();
//#endif
//}

static void DM_ETH_RXHandler(void* param) {
	(void) param;
	printf("DM_ETH_RXHandler.S delay %u\r\n", 0);
	while(1){
		if (flgSemaphore_r) // such as Semaphore taken successfully
		{
			rx_handler_direct(0, 0);  //(0, FALSE); //[Lwip] insteaded
			flgSemaphore_r = 0;
		}
		vTaskDelay(0); //vTaskDelay(pdMS_TO_TICKS(1));
	}
}

static void Open_Board(void)
{
}

static void Register_RXHandler(void) {
#ifdef DRV_INTR_MODE
	//if (intr_gpio_mptr())
	if (identified_irq_stat() & ISTAT_IRQ_CFG) //the h/w configured infra-structure
	{
	  printf("DM_ETH_RXHandler.create\r\n");
	  if(xTaskCreate((TaskFunction_t )DM_ETH_RXHandler,     
					 (const char*    )"Rx_task",   
					 (uint16_t       )512+128, 
					 (void*          )NULL,
					 (UBaseType_t    )NET_TASK_PRIO, //2
					 (TaskHandle_t*  )&network_rx_handler) != pdPASS) {
		printf("Rx task not be created. Error!\r\n");
	  }  
	}
#endif
}

void DM_Eth_Initialize(void)
{
  Open_Board();
  LWIP_Initalize(); //DM_Eth_Open();
  New_Semaphore();
	Register_RXHandler();

	//DM_Eth_Open();
}

uint32_t DM_Eth_Pid_and_bmsr(uint8_t link_up)
{
	uint32_t pid_bmsr;
	uint16_t link;

	pid_bmsr = dm9051_read_chip_id();
	pid_bmsr = (pid_bmsr << 16) | dm9051_bmsr_update();
	
	link = pid_bmsr & 0x0004 ? 1 : 0;
	if (!link_up && link)
		ethernetif_link_log_reset();
		
	return pid_bmsr;
}

void DM_ETH_Init(struct netif *netif, const uint8_t *adr)
{
  /* set MAC hardware to dm9051 */
  const uint8_t *pd;
  
  dm9051_boards_initialize();
  
  /* initialize */
  pd = dm9051_init(adr); //dm9051_init_nondual(lwip_set_MACaddr); //dm9051_init_dual(); //_dm9051_init(MACaddr);

  /* set MAC hardware address to lwip */
  netif->hwaddr[0] =  pd[0];
  netif->hwaddr[1] =  pd[1];
  netif->hwaddr[2] =  pd[2];
  netif->hwaddr[3] =  pd[3];
  netif->hwaddr[4] =  pd[4];
  netif->hwaddr[5] =  pd[5];
  
  printk("\r\n");
  printf("Out mac %02x:%02x:%02x:%02x:%02x:%02x\r\n", pd[0],pd[1],pd[2],pd[3],pd[4],pd[5]);
}

struct pbuf *DM_ETH_Input(struct netif *netif)
{
  struct pbuf *p, *q;
  u16_t len = 0;
  int l =0;
  uint8_t *buffer = get_ReceiveBuffer();

  (void)netif;

//  uint16_t rwpa_w, mdra_ingress;
//  dm9051_read_rx_pointers(&rwpa_w, &mdra_ingress);
  
  len = dm9051_rx(buffer);
  if (!len)
	  return NULL;
	  
  ethernetif_link_log_reset_hexdump(buffer, len); //set_tcpip_thread_state(6)

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

err_t DM_ETH_Output(struct netif *netif, struct pbuf *p)
{
  uint8_t *buffer;
  struct pbuf *q;
  int l = 0;
  (void) netif;

  buffer = get_TransmitBuffer();
  for(q = p; q != NULL; q = q->next)
  {
    memcpy((u8_t*)&buffer[l], q->payload, q->len);
    l = l + q->len;
  }
  dm9051_tx(buffer, (uint16_t) l);
  return ERR_OK;
}
 
void DM_ETH_IRQHandler(void)
{
	// Inside your ISR
	//BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	// Give the semaphore from the ISR
	//xSemaphoreGiveFromISR(xSemaphore_RX, &xHigherPriorityTaskWoken);
	// If xSemaphoreGiveFromISR unblocked a task of higher priority,
	// request a context switch before exiting the ISR
	//portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

	flgSemaphore_r = 1;
}

void DM_ETH_IRQEnable(void)
{
	dm9051_mcu_irq_enab();
	dm9051_isr_enab();
}

uint16_t DM_ETH_IRQDisable(void)
{
	dm9051_mcu_irq_disab();
	return dm9051_isr_disab();
}
