/**
 **************************************************************************
 * @file     task_msg.c
 * @version  v1.0.0
 * @date     2024-10-12
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
//#include "lwip/pbuf.h"
//#include "lwip/netif.h" //#include "lwip/err.h"
//#include "ethernetif.h"
//#include "netconf.h"

//#include "../dm_rt.h"
//#include "../eth_rt/dm_rt_api.h"
#include "identify/dm_identify_api.h"

#if 1
	#include "dm9051_ap_debug.h"
	//#define headSTR "[DBG] "
	//#define headSTR2 " [TASK.DBG] "
	#define printf(fmt, ...) TASK_DM9051_DEBUGF(PRINT_SEMA, PRINT_INFO_AP, (fmt, ##__VA_ARGS__)) //PRINT_INFO_AP or headSTR
	#define	printk(fmt, ...) TASK_DM9051_DEBUGK(PRINT_SEMA, (fmt, ##__VA_ARGS__))

	#define print_begin()
	#define print_end()
	#define print_prelist	printf
#endif

#if (qprint && PRINT_SEMA == SEMA_ON) //depend
xSemaphoreHandle mutex_print; //............hjmhjm.............
#endif

#if freeRTOS
//[dm9051_rtos.c - print mutex]
/* Declare the mutex handle globally so it can be accessed in different tasks
 */		
void dm_eth_create_mutex_print(void) {
  DM_ETH_MUTEX_CFG(
		(qprint && PRINT_SEMA == SEMA_ON),
		"'mutex_print' be created\r\n",
		mutex_print,
		tasks_dm9051_debug_init()); //how to disable!
}

xSemaphoreHandle tasks_dm9051_debug_init(void)
{
  xSemaphoreHandle sema;
  sema = xSemaphoreCreateMutex(); // Create a mutex
  return sema;
}

BaseType_t tasks_dm9051_mutex_begin(int qpnt, enum sema_tag_t mux, xSemaphoreHandle *sema_p)
{
	if (!qpnt)
		return pdFALSE;
	return (mux == SEMA_ON) ? xSemaphoreTake(*sema_p, portMAX_DELAY) : pdTRUE;
}

void tasks_dm9051_mutex_end(int qpnt, enum sema_tag_t mux, xSemaphoreHandle *sema_p)
{
	DM_UNUSED_ARG(qpnt);
	if (mux == SEMA_ON)
		xSemaphoreGive(*sema_p); //(_mutex_print);
	return;
}
#endif /* freeRTOS */

unsigned long dispc_poll_inpkt = 0;

void freertos_task_trypoll_inpkt(void) {
	dispc_poll_inpkt++;
}

unsigned long dispc_poll_active = 0;

#if 0
int isfreertos_task_firepoll_event(void) {
	return (dispc_poll_active >= 1) ? 1 : 0;
	//return (dispc_poll_active >= 10) ? 1 : 0;
}
#endif

void freertos_task_clearpoll_event(void) {
	dispc_poll_active = 0;
}

#ifdef DM9051_DRIVER_INTERRUPT
void freertos_task_trypoll_event(void) {
	dispc_poll_active++;
}
#endif

//#define headSTR "[TASK.LOG] "

static char *sn_memcpy(char *calcbuf, size_t size_calcbuf, char *dstBuf, size_t size_dstBuf, char *srcBuf) {
	char *q, c;
	memset(calcbuf, 0, size_calcbuf);
	q = calcbuf;
	do {
		c = *q++ = *srcBuf++;
	} while(c != 0x0A && c != 0);
	snprintf(dstBuf + strlen(dstBuf), size_dstBuf - strlen(dstBuf), "%s%s", PRINT_INFO_AP, calcbuf); //PRINT_AP or headSTR
	return srcBuf;
}
		
static void snTaskList(char *dstbuf, size_t size_dstbuf, char *pcList) {
	char *p = pcList;
	int m = strlen(pcList);
	memset(dstbuf, 0, size_dstbuf);
		do {
			char calbuf[30]; //[260];
			p = sn_memcpy(calbuf, sizeof(calbuf), dstbuf, size_dstbuf, p);
		} while(p < (pcList + m));
	dstbuf[size_dstbuf - 1] = 0;
}

static void print_list(void) {
	char tempBuf[280];
	char pcTaskList[160]; //[86];
	vTaskList(pcTaskList);
	snTaskList(tempBuf, sizeof(tempBuf), pcTaskList);
	printk(tempBuf);
}

static void print_task_list_R(unsigned long disppc, unsigned long base_count_value, char *head, unsigned long int_rcv_task_cnt) {
	printk("\r\n");
	print_begin();
	print_prelist("%lu /%lu - %s [int_task_poll %lu]\r\n", disppc, base_count_value, head, int_rcv_task_cnt);

	print_list();

	print_end();
}

static unsigned long dispp_poll = 0;
unsigned long dispc_int_rcv_task_loop = 0;

static unsigned long dispc_tcpip = 0;
static unsigned long dispc_idle = 0;

int freertos_task_tryDisplay_R(char *head, unsigned long base_countf, unsigned long base_count_value, unsigned long *dispp) {
		(void)dispp;

		if (base_countf == 0) {
			dispp_poll++;
//			if (dispp_poll <= 10) {
//				printf("%s: [%u] print_task_list_R().pre_counting...\r\n", head, dispp_poll);
//				if (dispp_poll == 10) {
//				  printf("%s: [%u] print_task_list_R()\r\n", head, dispp_poll);
//				  print_task_list_R(dispp_poll, base_count_value, head, dispc_int_rcv_task_loop);
//				}
//				return 1;
//			}
			
//			if (dispp_poll == (base_count_value % 5)) { //if (!(dispp_poll % base_count_value)) //if (dispp_poll == base_count_value)
//				
//				printf("%s: [%u] print_task_list_R()\r\n", head, dispp_poll);
//				print_task_list_R(dispp_poll, base_count_value, head, dispc_int_rcv_task_loop);
//				return 1;
//			}
			
			if (!(dispp_poll % base_count_value)) {
				printf("while count %8lu(%4lu, %4lu), tcpip %4lu, INT %4lum(%4lu) [IDLE %luw] .\"%s\", basecount %d\r\n",
					dispp_poll, dispc_poll_inpkt, dispc_poll_active, dispc_tcpip/1,
					dispc_int_rcv_task_loop/1000000, get_task_tryint(), dispc_idle/10000,
					head, base_count_value);
				return 1;
			}
			return 0;
		} else if (base_countf == 1) {
			dispc_int_rcv_task_loop++;
			if (dispc_int_rcv_task_loop == base_count_value)
				print_task_list_R(dispc_int_rcv_task_loop, base_count_value, head, dispc_int_rcv_task_loop);
			return !(dispc_int_rcv_task_loop % base_count_value) ? 1 : 0;
		}
		else if (base_countf == 2) {
			dispc_tcpip++;
		#if 0
			if (dispc_tcpip == base_count_value)
				print_task_list_R(dispc_tcpip, base_count_value, head);
		#endif
			return !(dispc_tcpip % base_count_value) ? 1 : 0;
		}
		else if (base_countf == 3) {
			dispc_idle++;
		#if 0
			if (dispc_idle == base_count_value)
				print_task_list_R(dispc_idle, base_count_value, head);
		#endif
			return !(dispc_idle % base_count_value) ? 1 : 0;
		}

		return 0;
}
