/**
 **************************************************************************
 * @file     dm_eth_pad.c
 * @version  v1.0.0
 * @date     2024-09-25
 * @brief    as particular range control relate to dm_eth.c
 **************************************************************************
 */
#define headSTR "[DBG] "
#define headSTR2 " [TASK.DBG] "
#include "dm9051_rt_debug.h"
#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_NETCONF_ON, /*SEMA_OFF*/ SEMA_ON, headSTR, (fmt, ##__VA_ARGS__))
#define	printk(fmt, ...) TASK_DM9051_DEBUGK(SEMA_ON, (fmt, ##__VA_ARGS__))
#define print_begin()
#define print_end()
#define print_prelist	printf

#if freeRTOS
#include "FreeRTOS.h" //#include "task.h"

/* freertos info/of uip_d
 */
//#define LIST_BASE_COUNT 30000

//#define LIST_MULT_COUNT (LIST_BASE_COUNT * 3)

//unsigned long dispc = 0;

//int freertos_task_tryDisplay(int tryDisp2, int tryDisp3)
//{

//	if (dispc % LIST_BASE_COUNT)
//	{
//		int m, cc, mc = 0;
//		char pcWriteBuffer[160]; //[86];
//		char pcCalcBuffer[30];	 //[260];
//		char *p, *q, c;

//		dispc++;

//		if (!(dispc % LIST_BASE_COUNT))
//		{
//			printf("\r\n");
//			printf("%u (%d) display_freertos_tasks..\r\n", dispc, dispc / LIST_BASE_COUNT); // " [%d]", _tcpip_thread_init
//			vTaskList(pcWriteBuffer);
//			m = strlen(pcWriteBuffer);
//			p = pcWriteBuffer;
//			do
//			{
//				memset(pcCalcBuffer, 0, sizeof(pcCalcBuffer));
//				q = pcCalcBuffer;
//				do
//				{
//					c = *q++ = *p++;
//				} while (c != 0x0A && c != 0);
//				printf(pcCalcBuffer);
//				cc = strlen(pcCalcBuffer);
//				if (cc > mc)
//					mc = cc;
//			} while (p < (pcWriteBuffer + m));
//			printf("vTaskList need %d bytes (by-strlen)\r\n", m);
//			printf("vTaskList max row %d bytes (by-strlen)\r\n", mc);
//			printf("\r\n");
//		}
//		return 1;
//	}
//	return 0;
//}

//void freertos_task_trylist(void)
//{
//	// if (get_tcpip_thread_state() == 6) {
//		freertos_task_tryDisplay(7, 7);
//	//}
//}

static unsigned long dispp_poll = 0;
static unsigned long dispc_int = 0;
static unsigned long dispc_tcpip = 0;
static unsigned long dispc_idle = 0;

static unsigned long dispc_poll_active = 0;

static unsigned long dispc_poll_inpkt = 0;

void freertos_task_clearpoll_event(void) {
	dispc_poll_active = 0;
}

#ifdef DM9051_DRIVER_INTERRUPT
void freertos_task_trypoll_event(void) {
	dispc_poll_active++;
}
#endif

void freertos_task_trypoll_inpkt(void) {
	dispc_poll_inpkt++;
}

static char *sn_memcpy(char *calcbuf, size_t size_calcbuf, char *dstBuf, size_t size_dstBuf, char *srcBuf) {
	char *q, c;
	memset(calcbuf, 0, size_calcbuf);
	q = calcbuf;
	do {
		c = *q++ = *srcBuf++;
	} while(c != 0x0A && c != 0);
	snprintf(dstBuf + strlen(dstBuf), size_dstBuf - strlen(dstBuf), "%s%s", headSTR, calcbuf);
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

void print_list(void) {
	#if freeRTOS
	char tempBuf[280];
	char pcTaskList[160]; //[86];
	vTaskList(pcTaskList);
	snTaskList(tempBuf, sizeof(tempBuf), pcTaskList);
	printk(tempBuf);
	#endif
}

static void print_task_list(unsigned long disppc, unsigned long base_count_value, char *head) {
	printk("\r\n");
	print_begin();
	print_prelist("%lu /%lu - %s [intc %lu]\r\n", disppc, base_count_value, head, dispc_int);

	print_list();

	print_end();
}

int freertos_task_tryDisplay(char *head, unsigned long base_countf, unsigned long base_count_value, unsigned long *dispp) {
	#undef printf
	#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_NETCONF_ON, /*SEMA_OFF*/ SEMA_ON, headSTR2, (fmt, ##__VA_ARGS__))
		(void)dispp;

		if (base_countf == 0) {
			dispp_poll++;
			if (dispp_poll <= 10) {
				printf("%s: [%u] print_task_list().pre_counting...\r\n", head, dispp_poll);
				if (dispp_poll == 10) {
				  printf("%s: [%u] print_task_list()\r\n", head, dispp_poll);
				  print_task_list(dispp_poll, base_count_value, head);
				}
				return 1;
			}
			if (dispp_poll == (base_count_value % 5)) { //if (!(dispp_poll % base_count_value)) //if (dispp_poll == base_count_value)
				
				printf("%s: [%u] print_task_list()\r\n", head, dispp_poll);
				print_task_list(dispp_poll, base_count_value, head);
				return 1;
			}
			if (!(dispp_poll % base_count_value)) {
				#undef printf
				#define printf(fmt, ...) ORIGINAL_DM9051_DEBUGF(headSTR2, (fmt, ##__VA_ARGS__))
				printf("[%8lu(%4lu, %4lu), tcpip %4lu, INT %4lum(%4lu) IDLE %luw] .\"%s\", basecount %d\r\n",
					dispp_poll, dispc_poll_inpkt, dispc_poll_active, dispc_tcpip/1,
					dispc_int/1000000, get_task_tryint(), dispc_idle/10000,
					head, base_count_value);
				return 1;
			}
			return 0;
		} else if (base_countf == 1) {
			dispc_int++;
			if (dispc_int == base_count_value)
				print_task_list(dispc_int, base_count_value, head);
			return !(dispc_int % base_count_value) ? 1 : 0;
		}
		else if (base_countf == 2) {
			dispc_tcpip++;
		#if 0
			if (dispc_tcpip == base_count_value)
				print_task_list(dispc_tcpip, base_count_value, head);
		#endif
			return !(dispc_tcpip % base_count_value) ? 1 : 0;
		}
		else if (base_countf == 3) {
			dispc_idle++;
		#if 0
			if (dispc_idle == base_count_value)
				print_task_list(dispc_idle, base_count_value, head);
		#endif
			return !(dispc_idle % base_count_value) ? 1 : 0;
		}

		return 0;
}

extern int inp_poll_start_r;

//extern int flgSemaphore_r;
//void DM_Eth_SemaphoreNewbinary(void) {
//	flgSemaphore_r = 0;

//	#if RX_HANDLER_ACCEPTED_TAKE_PLAIN_SEM || RX_HANDLER_ACCEPTED_TAKE_GENNY_SEM
//	DM9051_MUTEX_CF((freeRTOS), g_dm9051_interrupt_event, xSemaphoreCreateBinary());
//	//_xSemaphoreTake(_g_dm9051_interrupt_event, ( TickType_t ) 0 ); //extra.with 0
//	#endif
//}

void DM_Eth_SemaphoreNewbinary_Log(void) {
	#if RX_HANDLER_ACCEPTED_TAKE_PLAIN_SEM || RX_HANDLER_ACCEPTED_TAKE_GENNY_SEM
	if (g_dm9051_interrupt_event == NULL) {
		printf("Semaphore creation_1 failed\r\n");
        while (1);
    }
	printf("'g_dm9051_interrupt_event' be created\r\n");
	#endif
}

#if DM_ETH_DEBUG_MODE
static char *dm_int_string(void) {
	#ifdef DRV_INTR_MODE
		return "interrupt";
	#else
		return "polling";
	#endif
}

static char *dm_int_type(void) {
	#ifdef DRV_INTR_MODE
		return "(INT)";
	#else
		return "(POLLING)";
	#endif
}

static void dm_eth_config_interrupt_polling_log(void) {
	printf("(Control. config.)\r\n");
	printf("%s\r\n", dm_int_string());
#if defined(DRV_INTR_MODE) && INTERRUPT_HANDLER_ON
	printf("Interrupt.Hdlr On\r\n");
#endif
#if defined(DRV_INTR_MODE) && INTERRUPT_HANDLER_SUPPLIMENT //when '_DRV_INTR_MODE', accept INT active (default active low) trigger.
	printf("%s\r\n", RX_HANDLER_ACCEPTED_TAKE_PLAIN_SEM ? "PLAIN SEMAPHORE_BINARY" : "NO PLAIN SEMAPHORE");
	printf("%s\r\n", RX_HANDLER_ACCEPTED_TAKE_GENNY_SEM ? "JENNY SEMAPHORE_BINARY" : "NO JENNY SEMAPHORE");
#endif
	printk("\r\n");
}
#endif

void DM_ETHER_Receive_Task_head_log(void) {
	printf("Receive_Task.S delay %u\r\n", 0);

	#if DM_ETH_DEBUG_MODE
	dm_eth_config_interrupt_polling_log();
	printf("receive.Task.creation\r\n");
	printf("Receive_Task.for %s\r\n", dm_int_type());
	#else
		#ifdef DRV_INTR_MODE
			printf("Receive_Task.for %s\r\n", "interrupt"); //dm_int_type() return "interrupt";
		#else
			printf("Receive_Task.for %s\r\n", "polling"); //dm_int_type() return "polling";
		#endif
	#endif
//	if (identified_irq_stat() & ISTAT_IRQ_CFG) {
//		printk("\r\n");
//		log_intr_qpio_pin_config();
//		printk("\r\n");
//	}
	if (!(identified_irq_stat() & ISTAT_IRQ_CFG)) {
		//printk("\r\n");
		printf("Poll_Task.inp_poll.start = 1\r\n");
		printk("\r\n");
		inp_poll_start_r = 1;
	}
}
#endif //freertos
