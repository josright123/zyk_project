/**
  **************************************************************************
  * @file     dm_eth_pad.c
  * @version  v1.0.0
  * @date     2024-09-25
  * @brief    as particular range control relate to dm_eth.c
  **************************************************************************
  */
#include "dm9051_env.h"
#include "cboard/dm9051_cstate.h"

#include "../dm_eth.h"

#if freeRTOS
#include "FreeRTOS.h" //#include "task.h"
#endif

/* print log
 */

#if DM_ETH_DEBUG_MODE
//void dm_eth_input_hexdump_reset(void) {
//	link_log_reset_allow_num = 0;
//}

int link_log_reset_allow_num = 0;
const int rx_modle_log_reset_allow_num = 3;
#define	limit_len(n, nTP)	((n <= nTP) ? n : nTP)

void dm_eth_input_hexdump(const void *buf, size_t len) {
	#if 1
	//int get_tcpip_thread_state(void);
	//void set_tcpip_thread_state(int state); //.temp //extern int tcpip_thread_init;
	#endif
	int titledn = 0;
	DM_UNUSED_ARG(buf);
	DM_UNUSED_ARG(len);
	
	if (link_log_reset_allow_num < rx_modle_log_reset_allow_num) {
			link_log_reset_allow_num++;
			//if (link_log_reset_allow_num == rx_modle_log_reset_allow_num && get_tcpip_thread_state() == 1) {
				//set_tcpip_thread_state(6);
			//}
			sprint_hex_dump0(2, titledn, "dm9 head   <<rx", len, 32, buf, 0, limit_len(len, 66), DM_TRUE);
	}
}
#endif

/* freertos info/of uip_d
 */
#if freeRTOS
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
#endif
