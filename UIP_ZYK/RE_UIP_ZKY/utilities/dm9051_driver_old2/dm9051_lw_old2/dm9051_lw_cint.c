/**
 **************************************************************************
 * @file     dm9051_lw_cint.c
 * @version  v1.0
 * @date     2024-05-29
 * @brief    dm9051 lwip intr control program
 **************************************************************************
 *                       Copyright notice & Disclaimer
 *
 * The software Board Support Package (BSP) that is made available to
 * download from Artery official website is the copyrighted work of Artery.
 * Artery authorizes customers to use, copy, and distribute the BSP
 * software and its related documentation for the purpose of design and
 * development in conjunction with Artery microcontrollers. Use of the
 * software is governed by this copyright notice and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
 * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
 * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
 * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
 * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
 *
 **************************************************************************
 */
#include "dm9051opts.h"
#include "dm9051_lw.h"
#include "dm9051_lw_cboard.h"

// #include "dm9051_lw_conf_types.h"
#include "dm9051_lw_cspi.h"
#include "dm9051_lw_cint.h"
//#include "dm9051_lw_cint_data.h"
#include "dm_eth.h"

#if 1
#include "dm9051_lw_debug.h"
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
#endif

/* IRQ handler support */
void exint9_5_handler(void)
{
//#undef printf
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	//Correspond to main.c
	// net_task_freertos_recv_and_led(RX_HANDLER_FLG, NET_TASK_DLY),
	//only work with 1 spi_device (always  pincode = 0, in the working-flow).
	int pin;
	for (pin = 0; pin < ETHERNET_COUNT; pin++) {
		uint32_t exint_line = dm9051_irq_exint_line(pin);
	printf("pass a exint9_5_handler(void)\r\n");
		if (exint_line) { //[EXINT_LINE_5 ~ EXINT_LINE_9]
		
			if(exint_flag_get(exint_line) != RESET) {
			
				#if ETHERNET_INTERRUPT_MODE
				DM_ETH_IRQHandler();
				#endif
				exint_flag_clear(exint_line);
			}
			
		}
	}
//#undef printf
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

//[dm9051_lw_cint.c]
//[board /enab]
void dm9051_eth_irq_disab(IRQn_Type irqn)
{
	nvic_irq_disable(irqn);
}
//[board /disab]
void dm9051_eth_irq_enab(IRQn_Type irqn, nvic_priority_group_type priority)
{
  nvic_priority_group_config(priority);
  nvic_irq_enable(irqn, 1, 0); // nvic_irq_enable(EXINT9_5_IRQn, 1, 0); //i.e.= //_misc
}

//[int /Enab]
void irq_exint_enable(const struct extscfg_st *pexint_set)
{
  bannerline_log();
  printf("irq_priority[%d]: priority = 0x%x !\r\n", mstep_get_net_index(), pexint_set->extline.priority); // or "NVIC_PRIORITY_GROUP_0"
  printf("irq_enable[%d]: %s\r\n", mstep_get_net_index(), exint_enable_info());                           // pexint_set
  bannerline_log();
  dm9051_eth_irq_enab(pexint_set->extline.irqn, pexint_set->extline.priority);
  // NVIC_PRIORITY_GROUP_0/NVIC_PRIORITY_GROUP_4
}

void dm9051_extline_irq_enable(void)
{
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
//  printf("\r\n");
  if (intr_pointer())
  {
    const struct extscfg_st *pexint_set = (const struct extscfg_st *)intr_data_scfg(); //exint_scfg_ptr();

 //.list_extline(EXINT_LINE_NONE);
    printf(".dm9051_board_extline_irq (EXINT_LINE_%d, INT)\r\n", de_enum(dm9051_irq_exint_line(0)));

    if (pexint_set)
    {
      irq_exint_enable(pexint_set);
      // #if ETHERNET_INTERRUPT_MODE
      //	DEBUG_refresh_isr_check();
      // #endif
    }
  }
  else
    printf(".dm9051_board_extline_irq (POLLING)\r\n");
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}
