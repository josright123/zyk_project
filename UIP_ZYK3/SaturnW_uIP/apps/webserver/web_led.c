#include "uip.h"
#include "timer.h"
#include "web_led.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "sl_udelay.h"

char sCurrLED[4] = {'o', 'n', 0};
char sCurrP05[6] = {'o', 'f', 'f', 0};



int CurrTmp = 0;
int CurrHum = 0;

uint32_t LED_timer_flag;


void Delay(uint32_t times)
{
    while (times--)
    {
        uint32_t i;

        for (i = 0; i < 0xffff; i++);
    }
}

void Set_LED_mode(char lkkcode)
{
    //int i;

    if (lkkcode == ('0')) // LED off
    {


    }
    else if (lkkcode == '1')   // LED on
    {


    }
    else if (lkkcode == '2')  // LED Flash
    {

        //for(i = 0 ; i< 30 ; ++i)
        {

        }
    }
    else if (lkkcode == '3')
    {

    }
}
