/**
 * freertos_tasks_debug.c
 */
#include "stdio.h"
#define headSTR "[DBG] "
#define headSTR2 " [TASK.DBG] "
#include "dm9051opts.h"
#include "dm9051_lw.h"
#include "cboard/dm9051_lw_mcu_default_IN.h"
#include "dm9051_debug.h" //#include "../freertos_tasks_debug.h"
#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_NETCONF_ON, /*SEMA_OFF*/ SEMA_ON, headSTR, (fmt, ##__VA_ARGS__))
#define	printk(fmt, ...) TASK_DM9051_DEBUGK(SEMA_ON, (fmt, ##__VA_ARGS__))
#define print_begin()
#define print_end()
#define print_prelist	printf

#include "cboard/dm9051_cstate.h"

//#define LIST_BASE_COUNT	30000
//#define LIST_MULT_COUNT	(LIST_BASE_COUNT *3)
//unsigned long list_mult_count = base_count * 3;
//unsigned long base_count_value = base_count_value * 3;
//.unsigned long dispc = 0;

#if freeRTOS
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
#endif

void print_list(void) {
	#if freeRTOS
	char tempBuf[280];
	char pcTaskList[160]; //[86];
	vTaskList(pcTaskList);
	snTaskList(tempBuf, sizeof(tempBuf), pcTaskList);
	printk(tempBuf);
	#endif
}

static unsigned long dispp_poll = 0;
static unsigned long dispc_int = 0;
static unsigned long dispc_tcpip = 0;
static unsigned long dispc_idle = 0;

static void print_task_list(unsigned long disppc, unsigned long base_count_value, char *head) {
//if (*disppc == base_count_value)
//{
	printk("\r\n");
	print_begin();
	print_prelist("%lu /%lu - %s [intc %lu]\r\n", disppc, base_count_value, head, dispc_int);

	print_list();

	print_end();
//}
}

#if 0
static unsigned long dispc_fire_polling = 0
static unsigned long event_fire_polling(void) {
	return ++dispc_fire_polling;
}
static unsigned long get_fire_polling(void) {
	return dispc_fire_polling;
}
#endif

static unsigned long dispc_poll_active = 0;
#if 0
int isfreertos_task_firepoll_event(void) {
	return (dispc_poll_active >= 1) ? 1 : 0;
	//return (dispc_poll_active >= 10) ? 1 : 0;
}
#endif

//void freertos_task_clearpoll_event(void) {
//	dispc_poll_active = 0;
//}

#ifdef DM9051_DRIVER_INTERRUPT
void freertos_task_trypoll_event(void) {
	dispc_poll_active++;
}
#endif

static unsigned long dispc_poll_inpkt = 0;
void freertos_task_trypoll_inpkt(void) {
	dispc_poll_inpkt++;
}

int freertos_task_tryDisplay(char *head, unsigned long base_countf, unsigned long base_count_value, unsigned long *dispp) {
	#undef printf
	#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_NETCONF_ON, /*SEMA_OFF*/ SEMA_ON, headSTR2, (fmt, ##__VA_ARGS__))
		(void)dispp;

		if (base_countf == 0) {
			dispp_poll++;
			if (dispp_poll == base_count_value) { //if (!(dispp_poll % base_count_value)) //if (dispp_poll == base_count_value)
				print_task_list(dispp_poll, base_count_value, head);
				return 1;
			}
			if (!(dispp_poll % base_count_value)) {
				#undef printf
				#define printf(fmt, ...) ORIGINAL_DM9051_DEBUGF(headSTR2, (fmt, ##__VA_ARGS__))
				printf("[%8lu(%4lu, %4lu), tcpip %4lu, INT %4lum(%4lu) IDLE %luw]\r\n",
					dispp_poll, dispc_poll_inpkt, dispc_poll_active, dispc_tcpip/1,
					dispc_int/1000000, get_task_tryint(), dispc_idle/10000);
				return 1;
			}
			return 0;
			//return !(dispp_poll % base_count_value) ? 1 : 0;
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

//typedef struct spnt_st {
//	int n;
//	char pntbff[80];
//} spnt_t;

//extern int pntdatc;
//extern spnt_t pntdat[3];

//  #if 1
//  bannerline_log();
//  bannerline_log();
//  for (i = 0; i < pntdatc; i++) {
//		printf("[%d] %s", pntdat[i].n, pntdat[i].pntbff);
//  }
//  #endif
