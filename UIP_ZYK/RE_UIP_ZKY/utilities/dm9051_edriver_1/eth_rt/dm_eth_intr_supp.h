#include "identify/dm9051opts.h"



void DM_Eth_SemaphoreGiveBinary(void) {
#if 0
#if RX_HANDLER_ACCEPTED_TAKE_PLAIN_SEM || RX_HANDLER_ACCEPTED_TAKE_GENNY_SEM
	DM_ETH_RTOS_GIVE_SEMA();
#endif
#endif
}
int DM_Eth_SemaphoreTakeBinary(void) {
#if 0
	#if RX_HANDLER_ACCEPTED_TAKE_PLAIN_SEM || RX_HANDLER_ACCEPTED_TAKE_GENNY_SEM
	return DM_ETH_RTOS_TAKE_SEMA() == pdTRUE ? 1 : 0;
	#else
	return 1;
	#endif
#endif
	return 1;
}

#ifdef DRV_INTR_MODE
#if INTERRUPT_HANDLER_SUPPLIMENT

//.extern int flgSemaphore_r;

int xxx(void) {
	return 1;
}

static int nExpireCount_in; //save.

static void dm_eth_save_ec(void) {
	nExpireCount_in = nExpireCount;
}
static void dm_eth_statuslog_ec(void) {
	#undef printf
	#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_NETCONF_ON, /*SEMA_OFF*/ SEMA_ON, "  [ETH.POLLx.INT] ", (fmt, ##__VA_ARGS__))
	printf("[ec %d --> %d]\r\n", nExpireCount_in, nExpireCount);
	#undef printf
	#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_NETCONF_ON, /*SEMA_OFF*/ SEMA_ON, "  [[ETH] ] ", (fmt, ##__VA_ARGS__))
}

static int dm_eth_rxrdy_count(int nExpire)
{
	//#undef printf
	//#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_NETCONF_ON, SEMA_ON, "  [ETH.POLL5xx.INT] ", (fmt, ##__VA_ARGS__))
	#undef printf
	#define printf(fmt, ...) ORIGINAL_DM9051_DEBUGF("  [ETH] ", (fmt, ##__VA_ARGS__))
	//if (!_flgSemaphore_r) {
		//if (_xxx()) {
			if (flgSemaphore_r == 2) {
				/* while still NOT serviced
				 */
				//.DM_Eth_SemaphoreGiveBinary();
				/* while still NOT serviced
				 */
				return 0;
			}
				
			nExpireCount++;
			if (nExpireCount > nExpire && !nExpire_checking) {
				if (xxx()) {
					nExpire_checking = 1;
//					vTaskDelay(pdMS_TO_TICKS(1)); //(10)
				} else {
					nExpireCount = 0; //idle.
					nExpire_checking = 0; //state.
					printf("rxrdy_IDLE\r\n");
				}
			} else if (nExpireCount > nExpire && nExpire_checking) {
				if (xxx()) {
					if (nExpireCount > (nExpire+2)) {
					
						if (flgSemaphore_r == 1) {
							nExpireCount = 0; //fire.
							nExpire_checking = 0; //state.
							printk("\r\n");
							printf("Under ISR firing.. as restart... NOT rxrdy_FIRE\r\n");
						} else {
							nExpireCount = 0; //fire.
							nExpire_checking = 0; //state.

							flgSemaphore_r = 2; //printf("(set flgSemaphore_r = 2).dm_eth_rxrdy_count()\r\n");
							DM_Eth_SemaphoreGiveBinary(); //printf("rxrdy_FIRE\r\n");

							freertos_task_trypoll_event();
							if (isfreertos_task_firepoll_event()) {
								#if INTERRUPT_HANDLER_ON //when release 1 //accept solution to be released //ever to test with 0

								flgSemaphore_r = 1;
								DM_Eth_SemaphoreGiveBinary();

								#endif //INTERRUPT_HANDLER_ON
								freertos_task_clearpoll_event();
								printk("\r\n");
								printf("task_firepoll_event().. ON rxrdy_FIRE... and restart.. %d\r\n", event_fire_polling());
							}
							//.printf("Supplement rxrdy_FIRE for IRQ_Hdlr\r\n");
							return 1;
						}
						return 0; //.
					}
//					vTaskDelay(pdMS_TO_TICKS(1)); //(10)
				}
				else {
					nExpireCount = 0; //re-establish.
					nExpire_checking = 0; //state.
					printf("rxrdy_RE-ESTABLISH\r\n");
				}
			}
			return 0;
		//}
	//}
	#undef printf
	#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_NETCONF_ON, /*SEMA_OFF*/ SEMA_ON, "  [[ETH] ] ", (fmt, ##__VA_ARGS__))
}
#endif //INTERRUPT_HANDLER_SUPPLIMENT
#endif
 
 
int DM_ETH_InterruptHdlr_Supplement(void)
{
	#undef printf
	#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_NETCONF_ON, /*SEMA_OFF*/ SEMA_ON, "  [ETH.POLL.INT] ", (fmt, ##__VA_ARGS__))
	int ret = 0;
	#ifdef DRV_INTR_MODE
	#if INTERRUPT_HANDLER_SUPPLIMENT
	dm_eth_save_ec();
	//#ifdef DRV_INTR_MODE
		//if (!_flgSemaphore_r) {
			#if INTERRUPT_HANDLER_ON //when release 1 //IRQ_HANDLER_ACCEPTED //accept solution to be released
				//accept with watchdog polling.
				//#if 0
				//#endif

				//dm_eth_rxrdy_count(5); //to.test
			//ret = dm_eth_rxrdy_count(15); //to.first.default
			ret = dm_eth_rxrdy_count(75); //(50); //to.first.default (work.test, NOT stable, work.ok)
				//dm_eth_rxrdy_count(5000); // work.FAIL
			#else //!INTERRUPT_HANDLER_ON //test 0
				ret = dm_eth_rxrdy_count(0); // work.test
				//dm_eth_rxrdy_count(5); // work.ok
				//dm_eth_rxrdy_count(50); // work.test, NOT stable, work.ok
				//dm_eth_rxrdy_count(5000); // work.FAIL
			#endif //.INTERRUPT_HANDLER_ON //test.closed.
		//}
		//dm_eth_statuslog_ec();
	//#endif //DRV_INTR_MODE
	#endif //INTERRUPT_HANDLER_SUPPLIMENT
	#endif
	return ret;
	#undef printf
	#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_NETCONF_ON, /*SEMA_OFF*/ SEMA_ON, "[ETH] ", (fmt, ##__VA_ARGS__))
}

void DM_ETH_SHOW_CONSECUTIVE_Packets(void)
{
	#undef printf
	#define printf(fmt, ...) ORIGINAL_DM9051_DEBUGF("  [ETH] ", (fmt, ##__VA_ARGS__))
	if (isrSemaphore_src == 0x8888) {
		if (isrSemaphore_n >= 5) {
			printf("POLL.ISR %02x, recv %3d packet(s)\r\n", //post %02x -> %02x, 
				isrSemaphore_src, //flgSemaphore_isr_post & 0xff, (flgSemaphore_isr_post >> 8) & 0xff,
				isrSemaphore_n);
			isrSemaphore_n = 0;
		}
	}
	#undef printf
	#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_NETCONF_ON, /*SEMA_OFF*/ SEMA_OFF, "[NET] ", (fmt, ##__VA_ARGS__))
}

#if DM_ETH_DEBUG_MODE //[not essential], [only-dbg]
static void debug_packets(int n) {
	#undef printf
	#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_NETCONF_ON, /*SEMA_OFF*/ SEMA_ON, "[ETH] ", (fmt, ##__VA_ARGS__))
//	printf("(%d packets)\r\n", n);
}

static char *dm_int_type(void) {
	#ifdef DRV_INTR_MODE
		return "(INT)";
	#else
		return "(POLLING)";
	#endif
}

static char *dm_int_string(void) {
	#ifdef DRV_INTR_MODE
		return "interrupt";
	#else
		return "polling";
	#endif
}
#endif
