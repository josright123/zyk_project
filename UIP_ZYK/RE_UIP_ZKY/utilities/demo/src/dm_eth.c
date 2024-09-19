/**
  **************************************************************************
  * @file     dm_eth.c
  * @version  v1.0.0
  * @date     2024-06-24
  * @brief    as eth.c
  **************************************************************************
  */
#include "stdio.h"
//#include "FreeRTOS.h"
//#include "task.h"
#include "uip.h"
#include "uip_arp.h"
#include "dm9051opts.h"
#include "dm9051_lw.h"
#include "dm9051_cboard.h"
#include "../dm9051_lw_mcu_default.h"
#include "dm9051_lw_conf_types.h"
#include "dm9051_lw_cint.h"
//#include "ethernetif.h" //we are using 'uip', now!
#include "dm_eth.h"

#if 0
#include "../freertos_tasks_debug.h"
#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_NETCONF_ON, /*SEMA_OFF*/ SEMA_ON, "[N]", (fmt, ##__VA_ARGS__))
#endif

int flgSemaphore_r = 0;
#if 0
SemaphoreHandle_t xSemaphore_RX = NULL;
TaskHandle_t network_rx_handler;

static void DM_Eth_NewSema(void) {
	printk("\r\n");
	printk("\r\n");
	printf("DM_Eth_Initalize.s\r\n");
	xSemaphore_RX = xSemaphoreCreateBinary();
	xSemaphoreTake(xSemaphore_RX, ( TickType_t ) 0 );
	flgSemaphore_r = 0;
}
#endif

//static void LWIP_Initalize(void) {
//	//[uip] no compiler article files, -if 0 instead
//	//	printf("LWIP_Initalize.s\r\n");
//	//	tcpip_stack_init();
//	//	httpd_init();
//}

static void UIP_Initalize(void) {
	printf("UIP_Initalize.s\r\n");
}

void DM_Eth_Open(void) {
	#if 0
	LWIP_Initalize();
	#else
	UIP_Initalize();
	#endif
}

#define tapdev_send()		DM_ETH_Output((uint8_t *)uip_buf, uip_len)
#define tapdev_read()		DM_ETH_Input((uint8_t *)uip_buf) //_DM_ETH_RXHandler((uint8_t *)uip_buf) //dm9051_rx((uint8_t *)uip_buf)
#define BUF					((struct uip_eth_hdr *)&uip_buf[0])

void rcx_handler_direct(void)
{
	if (BUF->type == htons(UIP_ETHTYPE_IP))
	{
		uip_input();
		/* If the above function invocation resulted in data that
		should be sent out on the network, the global variable
		uip_len is set to a value > 0. */
		if (uip_len > 0)
		{
			uip_arp_out();
			tapdev_send();
		}
	}
	else if (BUF->type == htons(UIP_ETHTYPE_ARP))
	{
		uip_arp_arpin();
		/* If the above function invocation resulted in data that
		   should be sent out on the network, the global variable
		   uip_len is set to a value > 0. */
		if (uip_len > 0)
		{
			tapdev_send();
		}
	}
}

uint16_t DM_ETH_RXHandler(void) 
{
	static int n = 0;
	if (flgSemaphore_r) {
		if (n == 0) {
			//dm9051_isr_disab();
			DM_ETH_IRQDisable();
		}
		uip_len = tapdev_read();
		if (uip_len) {
			u16_t wrapper_len = uip_len;
			n++;
			/* Interrupt, per increase 1 packet */
			rcx_handler_direct();
			return wrapper_len; //continue;
		}
		else {
			flgSemaphore_r = 0;
			if (n >= 3)
				printf("tapdev_read exint9_5_handler(void) EXINT_LINE_%d, set flgSemaphore_r %d (conti %d packets)\r\n",
						de_enum(dm9051_irq_exint_line(0)), flgSemaphore_r, n);
			n = 0;
			DM_ETH_IRQEnable();
			//dm9051_isr_enab();
		}
	}
	return 0;
}

uint16_t DM_ETH_RXHandler_Poll(void)
{
	uip_len = tapdev_read();
	if (uip_len)
		/* Polling, per 1 packet */
		rcx_handler_direct();
	return uip_len;
}

#if 0
uint16_t _DM_ETH_RXHandler(uint8_t *buf)
{
	(void)buf;
	return 0;
}
static void _DM_ETH_RXHandler(void* param)
{
	(void) param;
	while(1){
		if (rx_wrapper_read())
			continue;
//		if (flgSemaphore_r) // such as Semaphore taken successfully
//		{
//			rx_handler_direct(0, FALSE); //[uip] insteaded
//			flgSemaphore_r = 0;
//		}
		vTaskDelay(0); //vTaskDelay(pdMS_TO_TICKS(1));
	}
}

static void DM_Eth_CreateRxTask(void) {
	//if (_intr_gpio_mptr())
	if (identified_irq_stat() & ISTAT_IRQ_CFG) //the h/w configured infra-structure
	{
	  printf("_DM_ETH_RXHandler.create\r\n");
	  if(xTaskCreate((TaskFunction_t )_DM_ETH_RXHandler,     
					 (const char*    )"Rx_task",   
					 (uint16_t       )512+128, 
					 (void*          )NULL,
					 (UBaseType_t    )NET_TASK_PRIO, //2
					 (TaskHandle_t*  )&network_rx_handler) != pdPASS) {
		printf("Rx task not be created. Error!\r\n");
	  }  
	}
}

void DM_Eth_Initialize(void)
{
	DM_Eth_NewSema();
	
	DM_Eth_CreateRxTask();

	//DM_Eth_Open();
}
#endif

void DM_ETH_Init(const uint8_t *adr)
{
	printf("DM9051_Configuration.s\r\n");
	dm9051_boards_initialize();
	
	printf("DM_ETH_Init.s\r\n");
#if 0

    tapdev_init(); // DM9051 init
#else

	do {
		const uint8_t *mac = dm9051_init(adr); //(NULL);
		if (!mac) {
			printf("Out mac NULL, Error!\r\n");
			printf("while(1);\r\n");
			while(1);
		}
		printf("In mac %02x:", adr[0]);
		printf(" %02x:", adr[1]);
		printf(" %02x:", adr[2]);
		printf(" %02x:", adr[3]);
		printf(" %02x:", adr[4]);
		printf(" %02x\r\n", adr[5]);

		printf("Out mac %02x:", mac[0]);
		printf(" %02x:", mac[1]);
		printf(" %02x:", mac[2]);
		printf(" %02x:", mac[3]);
		printf(" %02x:", mac[4]);
		printf(" %02x\r\n", mac[5]);
	} while(0);
#endif
}

uint16_t DM_ETH_Input(uint8_t *buf)
{
	return dm9051_rx(buf);
}

#if 0
err_t DM_ETH_Output(struct netif *netif, struct pbuf *p)
{
  struct pbuf *q;
  int l = 0;

  uint8_t *buffer = get_TransmitBuffer();
  for(q = p; q != NULL; q = q->next)
  {
    memcpy((u8_t*)&buffer[l], q->payload, q->len);
    l = l + q->len;
  }
  dm9051_tx(buffer, (uint16_t) l);
  return ERR_OK;
}
#else
void DM_ETH_Output(uint8_t *buf, uint16_t len)
{
  dm9051_tx(buf, len);
}
#endif
 
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
