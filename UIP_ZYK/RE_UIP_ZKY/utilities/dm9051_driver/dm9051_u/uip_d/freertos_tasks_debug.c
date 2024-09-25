//
// freertos_tasks_debug.c
//
#include "stdio.h"
#include <string.h>
//#include "dm9051opts.h"

#define freeRTOS_CONF	1 //...
#include "FreeRTOS.h" //#include "task.h"

#include "semphr.h" //.....
//#include "freertos_tasks_debug.h"

#define LIST_BASE_COUNT	30000

#define LIST_MULT_COUNT	(LIST_BASE_COUNT *3)

unsigned long dispc = 0;

int freertos_task_tryDisplay(int tryDisp2, int tryDisp3) {
		
		if (dispc % LIST_BASE_COUNT) {
			int m, cc, mc = 0;
			char pcWriteBuffer[160]; //[86];
			char pcCalcBuffer[30]; //[260];
			char *p, *q, c;
			
			dispc++;
			
			if (!(dispc % LIST_BASE_COUNT)) {
				printf("\r\n");
				printf("%u (%d) display_freertos_tasks..\r\n", dispc, dispc / LIST_BASE_COUNT); // " [%d]", _tcpip_thread_init
				vTaskList(pcWriteBuffer);
				m = strlen(pcWriteBuffer);
				p = pcWriteBuffer;
				do {
					memset(pcCalcBuffer, 0, sizeof(pcCalcBuffer));
					q = pcCalcBuffer;
					do {
						c = *q++ = *p++;
					} while(c != 0x0A && c != 0);
					printf(pcCalcBuffer);
					cc = strlen(pcCalcBuffer);
					if (cc > mc)
						mc = cc;
				} while(p < (pcWriteBuffer + m));
				printf("vTaskList need %d bytes (by-strlen)\r\n", m);
				printf("vTaskList max row %d bytes (by-strlen)\r\n", mc);
				printf("\r\n");
			}
			return 1;
		}
		return 0;
}

void freertos_task_trylist(void) {
	//int get_tcpip_thread_state(void);
	//void set_tcpip_thread_state(int state);

	//if (get_tcpip_thread_state() == 6) {
		/*if (*/ freertos_task_tryDisplay(7, 7); /*)*/
			//set_tcpip_thread_state(8); //tcpip_thread_init = 8;
	//}
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
//	printf("[%d] %s", pntdat[i].n, pntdat[i].pntbff);
//  }
//  #endif
