/**
 **************************************************************************
 * @file     dm_eth_pad.c
 * @version  v1.0.0
 * @date     2024-09-25
 * @brief    as particular range control relate to dm_eth.c
 **************************************************************************
 */
//#include "stdio.h"
//#include <string.h>

#include "dm_eth_opts.h"
//#include "dm_eth_api.h"

#if freeRTOS
#include "FreeRTOS.h" //#include "task.h"

/* freertos info/of uip_d
 */
#define LIST_BASE_COUNT 30000

#define LIST_MULT_COUNT (LIST_BASE_COUNT * 3)

unsigned long dispc = 0;

static int freertos_task_tryDisplay(int tryDisp2, int tryDisp3)
{

	if (dispc % LIST_BASE_COUNT)
	{
		int m, cc, mc = 0;
		char pcWriteBuffer[160]; //[86];
		char pcCalcBuffer[30];	 //[260];
		char *p, *q, c;

		dispc++;

		if (!(dispc % LIST_BASE_COUNT))
		{
			printf("\r\n");
			printf("%u (%d) display_freertos_tasks..\r\n", dispc, dispc / LIST_BASE_COUNT); // " [%d]", _tcpip_thread_init
			vTaskList(pcWriteBuffer);
			m = strlen(pcWriteBuffer);
			p = pcWriteBuffer;
			do
			{
				memset(pcCalcBuffer, 0, sizeof(pcCalcBuffer));
				q = pcCalcBuffer;
				do
				{
					c = *q++ = *p++;
				} while (c != 0x0A && c != 0);
				printf(pcCalcBuffer);
				cc = strlen(pcCalcBuffer);
				if (cc > mc)
					mc = cc;
			} while (p < (pcWriteBuffer + m));
			printf("vTaskList need %d bytes (by-strlen)\r\n", m);
			printf("vTaskList max row %d bytes (by-strlen)\r\n", mc);
			printf("\r\n");
		}
		return 1;
	}
	return 0;
}

void freertos_task_trylist(void)
{
	// int get_tcpip_thread_state(void);
	// void set_tcpip_thread_state(int state);

	// if (get_tcpip_thread_state() == 6) {
	/*if (*/ freertos_task_tryDisplay(7, 7); /*)*/
											 // set_tcpip_thread_state(8); //tcpip_thread_init = 8;
	//}
}
#endif
