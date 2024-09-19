/*
 * Copyright (c) 2023-2025 Davicom Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This DM9051 Driver for LWIP tcp/ip stack
 * First veryification with : AT32F415
 *
 * Author: Joseph CHANG <joseph_chang@davicom.com.tw>
 * Date: 20230411
 * Date: 20230428 (V3)
 * Date: 20240215 (Versions log please refer to 'dm9051_lw/info.txt')
 */
/*
 * Usa as part of sys_arch.c for FreeRTOS.
 */
#include "stdio.h"
#include "cmsis_compiler.h"               /* CMSIS compiler specific defines */
//#include "dm9051opts.h"
#include "dm9051_rtos.h" //=dm9051_rtos.h

#if freeRTOS
/* Declare the mutex handle globally so it can be accessed in protect spi sync.
 */
static int inHandlerMode (void);
xSemaphoreHandle mutex_create_start(void)
{
	xSemaphoreHandle sema;
	sema = xSemaphoreCreateMutex();
	//if (!sema)
	//  printf("init, failed to create a sema\r\n"); //(ERR_MEM)
	return sema;
}

void sys_mutex_lock_start(xSemaphoreHandle *sema_p)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	if(inHandlerMode())
	{
		xSemaphoreTakeFromISR(*sema_p, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
	else
	{
		xSemaphoreTake(*sema_p, portMAX_DELAY);
	}
}

void sys_mutex_unlock_end(xSemaphoreHandle *sema_p)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	if(inHandlerMode())
	{
		xSemaphoreGiveFromISR(*sema_p, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
	else
	{
		xSemaphoreGive(*sema_p);
	}
}

/* Determine whether we are in thread mode or handler mode.
 */
static int inHandlerMode (void)
{
  return __get_IPSR() != 0;
}
#endif /* freeRTOS */

#if freeRTOS
/* Declare the mutex handle globally so it can be accessed in different tasks
 */
xSemaphoreHandle tasks_dm9051_debug_init(void)
{
  xSemaphoreHandle sema;
  sema = xSemaphoreCreateMutex(); // Create a mutex
  return sema;
}

BaseType_t tasks_dm9051_mutex_begin(sema_tag_t mux, xSemaphoreHandle *sema_p)
{
	if (mux == SEMA_ON)
		return xSemaphoreTake(*sema_p, portMAX_DELAY); //(_mutex_print,)
	return pdTRUE;
}

void tasks_dm9051_mutex_end(sema_tag_t mux, xSemaphoreHandle *sema_p)
{
	if (mux == SEMA_ON)
		xSemaphoreGive(*sema_p); //(_mutex_print);
	return;
}
#endif /* freeRTOS */
