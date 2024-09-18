/*
 * ZykAPP.c
 *
 *  Created on: Sep 13, 2024
 *      Author: jack
 */

#include "ZykApp.h"
#include "FreeRTOS.h"
#include "task.h"
#include "uIP_Task.h"

void Zyk_uIP_init(void)
{
  TaskHandle_t xHandle = NULL;
  xTaskCreate(vuIP_Task,  "uIP", configMINIMAL_STACK_SIZE * 3, NULL, tskIDLE_PRIORITY + 4, &xHandle);
  EFM_ASSERT(xHandle != NULL);
}

void Zyk_app_init(void)
{
  Zyk_uIP_init();
}
