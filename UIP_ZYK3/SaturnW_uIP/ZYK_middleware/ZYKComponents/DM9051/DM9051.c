/***************************************************/
/* File: DM9051_spi_fun.c                           */
/* Author: DAVICOM                                 */
/***************************************************/
#include "DM9051.h"
#include "stdio.h"
#include "stdint.h"
#include "uip-conf.h"
#include "uip.h"
#include "sl_udelay.h"
#include "sl_spidrv_instances.h"
#include "sl_spidrv_usart_exp_config.h"
#include "em_usart.h"
#include "string.h"
#include "ZYKApp/RttPrint.h"
#define SPI_HANDLE                  sl_spidrv_usart_exp_handle;
#define CS_PORT SL_SPIDRV_USART_EXP_CS_PORT
#define CS_PIN  SL_SPIDRV_USART_EXP_CS_PIN
#define DM9051_RESET_PORT gpioPortB
#define DM9051_RESET_PIN 4

#ifdef DM9051_INT
#include "gpiointerrupt.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#define DM9051_INT_PORT  gpioPortA
#define DM9051_INT_PIN   8
#define DM9051_INT_NUM   8

extern SemaphoreHandle_t rx_semaphore;

#endif
/* Private function prototypes */
static uint16_t phy_read(uint32_t uReg);
static void phy_write(uint16_t uReg, uint16_t uValue);
static void phy_mode_set(uint32_t uMediaMode);

static void _DM9051_Delay(uint32_t uDelay);
static void _DM9051_Delay_ms(uint32_t uDelay);

static void DM9051_Configuration(void);

int DM9051_INT_FLAG;
static struct DM9051_eth DM9051_device;
/* -----------------------------------------------------*/
/* DM9051_Read_Reg()                                                                        */
/* SPI read command: bit7 = 0,                                                  */
/*                   bit[6:0] = Reg. offset addr                */
/* Disable auto SPI slave chip select.                  */
/*------------------------------------------------------*/
uint8_t DM9051_Read_Reg(uint8_t Reg_Off)
{
    uint8_t result;
    uint8_t tx_data[2];
    tx_data[0] = Reg_Off;
    tx_data[1] = 0;
    GPIO_PinOutClear(CS_PORT, CS_PIN);
    USART_SpiTransfer(USART0, tx_data[0]);
    result = USART_SpiTransfer(USART0, tx_data[1]);
    GPIO_PinOutSet(CS_PORT, CS_PIN);
    return result;
}

/* -----------------------------------------------------*/
/* DM9051_Write_Reg()                                                                       */
/* SPI write command: bit7 = 1,                                             */
/*                   bit[6:0] = Reg. offset addr        */
/* Disable auto SPI slave chip select.                  */
/*------------------------------------------------------*/
void DM9051_Write_Reg(uint8_t Reg_Off, uint8_t spi_data)
{
    uint8_t cmdaddr;
    uint8_t tx_data[2];
    cmdaddr = (Reg_Off | 0x80);
    tx_data[0] = cmdaddr;
    tx_data[1] = spi_data;
    GPIO_PinOutClear(CS_PORT, CS_PIN);
    USART_SpiTransfer(USART0, tx_data[0]);
    USART_SpiTransfer(USART0, tx_data[1]);
    GPIO_PinOutSet(CS_PORT, CS_PIN);
}

/* -----------------------------------------------------*/
/* DM9051_Read_Mem()                                                                        */
/*                                                                                                          */
/* DM9051 burst read command: SPI_RD_BURST = 0x72           */
/* Disable auto SPI slave chip select.                  */
/*------------------------------------------------------*/
void DM9051_Read_Mem(uint8_t *pu8data, uint32_t datalen)
{
  uint8_t tx_data[datalen+1];
  uint32_t i;

  // Read SPI_Data_Array back from the slave
  memset(tx_data, 0x00, sizeof(tx_data));
  tx_data[0] =SPI_RD_BURST;
  GPIO_PinOutClear(CS_PORT, CS_PIN);
  USART_SpiTransfer(USART0, tx_data[0]);
  for (i = 0; i< datalen;i++)
    pu8data[i] = USART_SpiTransfer(USART0, tx_data[i+1]);
  GPIO_PinOutSet(CS_PORT, CS_PIN);
}

/* -----------------------------------------------------*/
/* DM9051_Write_Mem()                                                                       */
/*                                                                                                          */
/* DM9051 burst write command: SPI_WR_BURST = 0xF8          */
/* Disable auto SPI slave chip select.                  */
/*------------------------------------------------------*/
void DM9051_Write_Mem(uint8_t *pu8data, uint32_t datalen)
{
    int i;
    uint8_t tx_data[datalen+1];
    memset(tx_data, 0x00, sizeof(tx_data));
    for (i = datalen; i > 0; i--) {
        tx_data[i] = pu8data[i - 1];
    }

    tx_data[0]= SPI_WR_BURST;
    GPIO_PinOutClear(CS_PORT, CS_PIN);
    for (i = 0; i< datalen+1;i++)
      USART_SpiTransfer(USART0, tx_data[i]);
    GPIO_PinOutSet(CS_PORT, CS_PIN);
}

/*********************************************************************************************************//**
  * @brief  Rx function.
  * @retval pbuf
  ***********************************************************************************************************/
uint16_t DM9051_RX(void)
{
    uint8_t rxbyte;

    uint16_t calc_MRR;
    uint16_t rx_len = 0;

    //printf("DM9051_rx \r\n");
    /* Check packet ready or not                                                                              */
    rxbyte = DM9051_Read_Reg(DM9051_MRCMDX);
    rxbyte = DM9051_Read_Reg(DM9051_MRCMDX);

    //printf("DM9051_rx process...1st byte = 0x%x\n",rxbyte);

    if ((rxbyte != 1) && (rxbyte != 0))
    {
        printf("rxbyte = %02X.\n", rxbyte);
        DM9051_TRACE2("DM9051 rx: rx error, stop device \n");
        //printf("tx_packet_count = %d \r\n",tx_packet_count);

        /* Reset RX FIFO pointer */
        DM9051_Write_Reg(DM9051_RCR, RCR_DEFAULT);  //RX disable
        DM9051_Write_Reg(DM9051_MPCR, 0x01);        //Reset RX FIFO pointer
        _DM9051_Delay(2);
        DM9051_Write_Reg(DM9051_RCR, (RCR_DEFAULT | RCR_RXEN));     //RX Enable

        /* restore receive interrupt                                                                            */
        //DM9051_device.imr_all |= IMR_PRM;
        //DM9051_device.imr_all = IMR_DEFAULT;
        //DM9051_Write_Reg(DM9051_IMR, DM9051_device.imr_all);

        return 0;
    }

    if (rxbyte)
    {
        uint16_t rx_status;
        uint8_t ReceiveData[4];

#ifdef FifoPointCheck
        calc_MRR = (DM9051_Read_Reg(DM9051_MRRH) << 8) + DM9051_Read_Reg(DM9051_MRRL);  //Save RX SRAM pointer
#endif  //FifoPointCheck

        DM9051_Read_Reg(DM9051_MRCMDX);     //dummy read

        DM9051_Read_Mem(ReceiveData, 4);

        rx_status = ReceiveData[0] + (ReceiveData[1] << 8);
        rx_len = ReceiveData[2] + (ReceiveData[3] << 8);

        //printf("ReceiveData[0] %x ReceiveData[1] %x ReceiveData[2] %x ReceiveData[3] %x \r\n", ReceiveData[0], ReceiveData[1], ReceiveData[2], ReceiveData[3]);
        //DM9051_TRACE("DM9051 rx: status %04x len %d \n", rx_status, rx_len);
#ifdef uIP_NOOS
        DM9051_Read_Mem((u8_t *)uip_buf, rx_len);
#else
        netif->rxb_count += rx_len;     //Count received bytes.

        /* allocate buffer           */
        p = pbuf_alloc(PBUF_LINK, rx_len, PBUF_RAM);

        if (p != NULL)
        {
            DM9051_Read_Mem((u8_t *)p->payload, rx_len);
        }
        else
        {
            DM9051_TRACE2("DM9051 rx: no pbuf \n");
            /* no pbuf, discard data from DM9051  */
            DM9051_Read_Mem(databyte, rx_len);
        }

#endif  //uIP_NOOS

#ifdef FifoPointCheck
        calc_MRR += (rx_len + 4);

        if (calc_MRR > 0x3fff) calc_MRR -= 0x3400;

        if (calc_MRR != ((DM9051_Read_Reg(DM9051_MRRH) << 8) + DM9051_Read_Reg(DM9051_MRRL)))
        {
            printf("DM9K MRR Error!!\n");
            printf("Predicut RX Read pointer = 0x%X, Current pointer = 0x%X\n", calc_MRR, ((DM9051_Read_Reg(DM9051_MRRH) << 8) + DM9051_Read_Reg(DM9051_MRRL)));

            DM9051_Write_Reg(DM9051_MRRH, (calc_MRR >> 8) & 0xff);
            DM9051_Write_Reg(DM9051_MRRL, calc_MRR & 0xff);
        }

#endif

        if ((rx_status & 0xbf00) || (rx_len < 0x40) || (rx_len > DM9051_PKT_MAX))
        {
            DM9051_TRACE2("rx error: status %04x, rx_len: %d \n", rx_status, rx_len);

            if (rx_status & 0x100)
            {
                DM9051_TRACE2("rx fifo error \n");
            }

            if (rx_status & 0x200)
            {
                DM9051_TRACE2("rx crc error \n");
            }

            if (rx_status & 0x8000)
            {
                DM9051_TRACE2("rx length error \n");
            }

            if (rx_len > DM9051_PKT_MAX)
            {
                DM9051_TRACE2("rx length too big \n");
                /* RESET device                                                                                     */
                //_DM9051_WriteReg(DM9051_NCR, NCR_RST);
                //_DM9051_Delay_ms(5);
            }

#ifndef uIP_NOOS
            pbuf_free(p);
            p = NULL;
#endif  //uIP_NOOS
            //rx_len =0;  // stone add if rx_status error clear rx_len = 0
        }
    }
    else
    {
        //DM9051_TRACE("DM9051 rx: No packets received. \n");
        /* clear packet received latch status*/
        /* restore receive interrupt*/
        //DM9051_device.imr_all |= IMR_PRM;
        //DM9051_device.imr_all = IMR_DEFAULT;
        //DM9051_Write_Reg(DM9051_IMR, DM9051_device.imr_all);
    }

    return rx_len;
}

/*********************************************************************************************************//**
  * @brief  Tx function.
  * @param  pbuf: buffer link list
  * @retval Always 0
  ***********************************************************************************************************/
#ifdef uIP_NOOS
    uint32_t DM9051_TX(void)
#else
    uint32_t DM9051_TX(struct netif *netif, struct pbuf *p)
#endif  //uIP_NOOS
{

    uint16_t calc_MWR;
    uint16_t SendLength;

    //printf("DM9051_tx()....\n");

#ifndef uIP_NOOS
    uint8_t err;

    netif->txb_count += p->tot_len;     //Count TX bytes.
    SendLength = p->tot_len;
    /* lock DM9051                                                                                            */
    OSSemPend(DM9051_sem_lock, 0, &err);
#else
    SendLength = uip_len;
#endif  //uIP_NOOS

    //tx_packet_count++;

    /* disable DM9051a interrupt                                                                              */
    //DM9051_Write_Reg(DM9051_IMR, IMR_PAR);

    while (DM9051_Read_Reg(DM9051_TCR) & DM9051_TCR_SET)
    {
        _DM9051_Delay(5);
    }

#ifdef FifoPointCheck
    calc_MWR = (DM9051_Read_Reg(DM9051_MWRH) << 8) + DM9051_Read_Reg(DM9051_MWRL);
    calc_MWR += SendLength;

    //printf("calc_MWR = 0x%X\n", calc_MWR);
    //  if(SendLength & 0x01) calc_MWR++;
    if (calc_MWR > 0x0bff) calc_MWR -= 0x0c00;

#endif


    /* Move data to DM9051 TX RAM                                                                             */
    //_DM9051_WriteRegAddr(DM9051_MWCMD);

    {
#ifdef uIP_NOOS
        //Write data to FIFO
        DM9051_Write_Reg(DM9051_TXPLL, uip_len & 0xff);
        DM9051_Write_Reg(DM9051_TXPLH, (uip_len >> 8) & 0xff);
        DM9051_Write_Mem((u8_t *)uip_buf, SendLength);
#else
        /* q traverses through linked list of pbuf's. This list MUST consist of a single packet ONLY            */
        struct pbuf *q;
        //uint16_t pbuf_index = 0;
        //uint8_t word[2], word_index = 0;
        q = p;
#if 0
        printf("Tx Read Pointer H = 0x%02X.\n", DM9051_Read_Reg(DM9051_TRPAH));
        printf("Tx Read Pointer L = 0x%02X.\n", DM9051_Read_Reg(DM9051_TRPAL));
        printf("DM9051_MWRH H = 0x%02X.\n", DM9051_Read_Reg(DM9051_MWRH));
        printf("DM9051_MWRL L = 0x%02X.\n", DM9051_Read_Reg(DM9051_MWRL));
#endif  //0

        //Write data to FIFO
        DM9051_Write_Reg(DM9051_TXPLL, q->tot_len & 0xff);
        DM9051_Write_Reg(DM9051_TXPLH, (q->tot_len >> 8) & 0xff);
        DM9051_Write_Mem((u8_t *)q->payload, q->tot_len);
#endif  //uIP_NOOS

        /* Issue TX polling command                                                                             */
        DM9051_Write_Reg(DM9051_TCR, TCR_TXREQ);    /* Cleared after TX complete */
    }

#if 0

    if (DM9051_device.packet_cnt == 0)
    {
        DM9051_TRACE("DM9051 tx: first packet \n");

        DM9051_device.packet_cnt ++;
        /* Set TX length to DM9051                                                                              */
        DM9051_Write_Reg(DM9051_TXPLL, p->tot_len & 0xff);
        DM9051_Write_Reg(DM9051_TXPLH, (p->tot_len >> 8) & 0xff);

        /* Issue TX polling command                                                                             */
        DM9051_Write_Reg(DM9051_TCR, TCR_TXREQ);    /* Cleared after TX complete                                */
    }
    else
    {
        DM9051_TRACE("DM9051 tx: second packet \n");
        DM9051_device.packet_cnt ++;
        DM9051_device.queue_packet_len = p->tot_len;
    }

#endif  //0

#if 0
    printf("Tx control Reg = 0x%02X.\n", DM9051_Read_Reg(DM9051_TCR));
    printf("Tx Read Pointer H = 0x%02X.\n", DM9051_Read_Reg(DM9051_TRPAH));
    printf("Tx Read Pointer L = 0x%02X.\n", DM9051_Read_Reg(DM9051_TRPAL));
    printf("DM9051_MWRH H = 0x%02X.\n", DM9051_Read_Reg(DM9051_MWRH));
    printf("DM9051_MWRL L = 0x%02X.\n", DM9051_Read_Reg(DM9051_MWRL));
    printf("Tx control Reg = 0x%02X.\n", DM9051_Read_Reg(DM9051_TCR));
    printf("ISR Reg = 0x%02X.\n", DM9051_Read_Reg(DM9051_ISR));
    printf("NSR Reg = 0x%02X.\n", DM9051_Read_Reg(DM9051_NSR));
#endif  //0

#ifdef FifoPointCheck

    if (calc_MWR != ((DM9051_Read_Reg(DM9051_MWRH) << 8) + DM9051_Read_Reg(DM9051_MWRL)))
    {
#ifndef uIP_NOOS
        netif->tx_reset++;
#endif  //uIP_NOOS

#if 0 //Point_Error_Reset

        while (ior(DM9051_REG_TCR) & DM9051_TCR_SET) udelay(5);

        dm9k_reset();
        /* unlock DM9051 device                                                                                   */
        OSSemPost(DM9051_sem_lock);
        return;
#endif
        printf("DM9K MWR Error!! calc_MWR = 0x%X, SendLength = 0x%x\n", calc_MWR, SendLength);
        printf("MWRH = 0x%X, MWRL = 0x%X\n", DM9051_Read_Reg(DM9051_MWRH), DM9051_Read_Reg(DM9051_MWRL));
#if 0
        DM9051_Write_Reg(DM9051_ISR, 0x80);    /* DIsable INT                                               */
        DM9051_INIT(netif);
        DM9051_Write_Reg(DM9051_ISR, 0x81);    /* Enable INT */
#endif  //0

        DM9051_Write_Reg(DM9051_MWRH, (calc_MWR >> 8) & 0xff);
        DM9051_Write_Reg(DM9051_MWRL, calc_MWR & 0xff);
    }

#endif

    /* enable DM9051a interrupt    */
    //DM9051_Write_Reg(DM9051_IMR, DM9051_device.imr_all);

#ifndef uIP_NOOS
    /* unlock DM9051 device                                                                                   */
    OSSemPost(DM9051_sem_lock);

    /* wait ack                                                                                               */
    //OSSemPend(DM9051_sem_ack, 0,&err);  //For TX interrupt
#endif  //uIP_NOOS

    //DM9051_TRACE("DM9051 tx done \n");

    return 0;
}

#ifdef DM9051_INT
uint8_t DM9051_IntPinstatus(void)
{
    return GPIO_PinInGet(DM9051_INT_PORT,DM9051_INT_PIN);
}
void DM9051_IRQEnable(void)
{
    GPIO_IntClear(1 << DM9051_INT_NUM);
    GPIO_IntEnable(1 << DM9051_INT_NUM);
}
void DM9051_Intenable(void)
{
  DM9051_device.imr_all = IMR_DEFAULT;      //Enable all interrupt.
  //DM9051_TRACE("DM9051 isr: imr_all %02x \n", DM9051_device.imr_all);
  DM9051_Write_Reg(DM9051_IMR, DM9051_device.imr_all);
}

void DM9051_Intdisable(void)
{
  /* disable receive interrupt                                                                            */
  DM9051_device.imr_all &= ~IMR_PRM;
  DM9051_Write_Reg(DM9051_IMR, DM9051_device.imr_all);

  DM9051_device.imr_all = IMR_OFF;        //Disable all interrupt.
  DM9051_Write_Reg(DM9051_IMR, DM9051_device.imr_all);
}
uint8_t DM9051_Intstatus(void)
{
  uint16_t int_status;
  //u8 temp;

  //printf("DM9051_isr \r\n");

  //DM9051_Write_Reg(DM9051_IMR, DM9051_IMR_OFF);     //Clear interrupt flag

  /* Got DM9051 interrupt status  */
  int_status = DM9051_Read_Reg(DM9051_ISR);

  DM9051_Write_Reg(DM9051_ISR, (uint8_t)int_status);    /* Clear ISR status                                          */

  /*if(int_status == 0x80)
  {
      DM9051_TRACE("DM9051 isr: int status %04x \n", int_status);
  }*/


  /* receive overflow */
  if ((int_status & ISR_ROS) || (int_status & ISR_ROOS)){
    DM9051_TRACE2("overflow or overflow counter overflow\r\n");
      return true;
  }
  /* Received the coming packet                                                                             */
  if (int_status & ISR_PRS)
  {
      return true;
  }
  else
    return false;
}
/*---------------------------------------------------------------------------------------------------------*/
/*  SPI0 IRQ Handler                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
 * @brief   This function handles EXTI interrupt.
 * @retval  None
 ************************************************************************************************************/
void DM9051_IRQHandler(uint8_t intNo)
{
  if (intNo != DM9051_INT_NUM) {
      return;
  }
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(rx_semaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/*********************************************************************************************************//**
  * @brief  Interrupt service routine.
  * @retval None
  ***********************************************************************************************************/
void DM9051_isr(void)
{
    uint16_t int_status;
    //u8 temp;

    //printf("DM9051_isr \r\n");

    //DM9051_Write_Reg(DM9051_IMR, DM9051_IMR_OFF);     //Clear interrupt flag

    /* Got DM9051 interrupt status  */
    int_status = DM9051_Read_Reg(DM9051_ISR);

    DM9051_Write_Reg(DM9051_ISR, (uint8_t)int_status);    /* Clear ISR status                                          */

    /*if(int_status == 0x80)
    {
        DM9051_TRACE("DM9051 isr: int status %04x \n", int_status);
    }*/

    /* receive overflow                                                                                       */
    if (int_status & ISR_ROS)
    {
        DM9051_TRACE2("overflow \n");
    }

    if (int_status & ISR_ROOS)
    {
        DM9051_TRACE2("overflow counter overflow \n");
    }

    /* Received the coming packet                                                                             */
    if (int_status & ISR_PRS)
    {
        //extern OS_EVENT* ethernetinput;

        uip_len = DM9051_RX();

        /* disable receive interrupt                                                                            */
        DM9051_device.imr_all &= ~IMR_PRM;
        DM9051_Write_Reg(DM9051_IMR, DM9051_device.imr_all);

        DM9051_device.imr_all = IMR_OFF;        //Disable all interrupt.
        DM9051_Write_Reg(DM9051_IMR, DM9051_device.imr_all);
        /* a frame has been received                                                                            */
        //OSSemPost(ethernetinput);
    }

#if 0

    /* Transmit Interrupt check                                                                               */
    if (int_status & ISR_PTS)
    {
        /* transmit done                                                                                        */
        int tx_status = DM9051_Read_Reg(DM9051_NSR);    /* Got TX status                                        */

        if (tx_status & (NSR_TX2END | NSR_TX1END))
        {
#if 0
            DM9051_device.packet_cnt --;

            if (DM9051_device.packet_cnt > 0)
            {
                DM9051_TRACE("DM9051 isr: tx second packet \n");

                /* transmit packet II                                                                               */
                /* Set TX length to DM9051                                                                          */
                DM9051_Write_Reg(DM9051_TXPLL, DM9051_device.queue_packet_len & 0xff);
                DM9051_Write_Reg(DM9051_TXPLH, (DM9051_device.queue_packet_len >> 8) & 0xff);

                /* Issue TX polling command                                                                         */
                DM9051_Write_Reg(DM9051_TCR, TCR_TXREQ);    /* Cleared after TX complete                            */
            }

#endif  //0
            /* unlock DM9051 device                                                                               */
            //OSSemPost(DM9051_sem_ack);  //For TX interrupt

        }
    }

#endif  //0
    DM9051_device.imr_all = IMR_DEFAULT;      //Enable all interrupt.
    //DM9051_TRACE("DM9051 isr: imr_all %02x \n", DM9051_device.imr_all);
    DM9051_Write_Reg(DM9051_IMR, DM9051_device.imr_all);
}
#endif //uIP_NOOS

/*********************************************************************************************************//**
  * @brief  Hardware related configuration.
  * @retval None
  ***********************************************************************************************************/
void DM9051_Configuration(void)
{
  GPIO_PinModeSet(CS_PORT, CS_PIN, gpioModePushPull, 1);
  GPIO_PinModeSet(DM9051_RESET_PORT, DM9051_RESET_PIN, gpioModePushPull, 0);
  sl_udelay_wait(1200);
  GPIO_PinOutSet(DM9051_RESET_PORT, DM9051_RESET_PIN);
  sl_udelay_wait(200);
#ifdef DM9051_INT
  GPIO_PinModeSet(DM9051_INT_PORT, DM9051_INT_PIN, gpioModeInputPullFilter, 1);
  GPIOINT_CallbackRegister(DM9051_INT_NUM,DM9051_IRQHandler);
  GPIO_ExtIntConfig(DM9051_INT_PORT, DM9051_INT_PIN, DM9051_INT_NUM, false, true, false);
#endif  //DM9051_INT
}

/*********************************************************************************************************//**
  * @brief  uS level delay function.
  * @param  uDelay: Delay time
  * @retval None
  ***********************************************************************************************************/
static void _DM9051_Delay(uint32_t uDelay)
{
    sl_udelay_wait(uDelay);
}

/*********************************************************************************************************//**
  * @brief  mS lebel delay function.
  * @param  uDelay: delay time based on ms.
  * @retval None
  ***********************************************************************************************************/
static void _DM9051_Delay_ms(uint32_t uDelay)
{
  sl_udelay_wait(uDelay*1000);
}

/*********************************************************************************************************//**
  * @brief  Read function of PHY.
  * @param  uReg: PHY register
  * @retval uData: Data of register
  ***********************************************************************************************************/
static uint16_t phy_read(uint32_t uReg)
{
    uint16_t uData;

    /* Fill the phyxcer register into REG_0C                                                                  */
    DM9051_Write_Reg(DM9051_EPAR, DM9051_PHY | uReg);
    DM9051_Write_Reg(DM9051_EPCR, 0xc);           /* Issue phyxcer read command                               */

    //_DM9051_Delay_ms(100);                        /* Wait read complete                                       */
    while (DM9051_Read_Reg(DM9051_EPCR) & 0x1)
    {
        _DM9051_Delay(1);
    };        //Wait complete

    DM9051_Write_Reg(DM9051_EPCR, 0x0);           /* Clear phyxcer read command                               */

    uData = (DM9051_Read_Reg(DM9051_EPDRH) << 8) | DM9051_Read_Reg(DM9051_EPDRL);

    return uData;
}

/*******************************************************************************
* Function Name  : phy_write
* Description    : Write a word to phyxcer
* Input          : - reg: reg
*                  - value: data
* Output         : None
* Return         : None
* Attention         : None
*******************************************************************************/
static void phy_write(uint16_t reg, uint16_t value)
{
    /* Fill the phyxcer register into REG_0C                                                                */
    DM9051_Write_Reg(DM9051_EPAR, DM9051_PHY | reg);

    /* Fill the written data into REG_0D & REG_0E                                                           */
    DM9051_Write_Reg(DM9051_EPDRL, (value & 0xff));
    DM9051_Write_Reg(DM9051_EPDRH, ((value >> 8) & 0xff));
    DM9051_Write_Reg(DM9051_EPCR, 0xa);    /* Issue phyxcer write command                                   */

    //_DM9051_Delay_ms(500);                       /* Wait write complete                                   */
    while (DM9051_Read_Reg(DM9051_EPCR) & 0x1)
    {
        _DM9051_Delay(1);
    };       //Wait complete

    DM9051_Write_Reg(DM9051_EPCR, 0x0);    /* Clear phyxcer write command                                   */
}

/*********************************************************************************************************//**
  * @brief  Set PHY mode.
  * @param  uMediaMode:
  *         @DM9051_AUTO Auto negotiation
  *         @DM9051_10MHD 10MHz, Half duplex
  *         @DM9051_10MFD 10MHz, Full duplex
  *         @DM9051_100MHD 100MHz, Half duplex
  *         @DM9051_100MFD 100MHz, Full duplex
  * @retval None
  ***********************************************************************************************************/
static void phy_mode_set(uint32_t uMediaMode)
{
    uint16_t phy_reg4 = 0x01e1, phy_reg0 = 0x1000;

    if (!(uMediaMode & DM9051_AUTO))
    {
        switch (uMediaMode)
        {
            case DM9051_10MHD:
            {
                phy_reg4 = 0x21;
                phy_reg0 = 0x0000;
                break;
            }

            case DM9051_10MFD:
            {
                phy_reg4 = 0x41;
                phy_reg0 = 0x1100;
                break;
            }

            case DM9051_100MHD:
            {
                phy_reg4 = 0x81;
                phy_reg0 = 0x2000;
                break;
            }

            case DM9051_100MFD:
            {
                phy_reg4 = 0x101;
                phy_reg0 = 0x3100;
                break;
            }

            case DM9051_10M:
            {
                phy_reg4 = 0x61;
                phy_reg0 = 0x1200;
                break;
            }
        }

        phy_write(4, phy_reg4);             /* Set PHY media mode                                               */

        phy_write(0, phy_reg0);             /* Tmp                                                              */
        _DM9051_Delay(10);
    }
}

/*********************************************************************************************************//**
  * @brief  DM9051 init function.
  * @retval -1 or 0
  ***********************************************************************************************************/
int32_t DM9051_INIT()
{
    int i, oft, lnk;
    uint32_t value;

    DM9051_Configuration();


    value  = DM9051_Read_Reg(DM9051_VIDL);
    value |= DM9051_Read_Reg(DM9051_VIDH) << 8;
    value |= DM9051_Read_Reg(DM9051_PIDL) << 16;
    value |= DM9051_Read_Reg(DM9051_PIDH) << 24;

    value  = DM9051_Read_Reg(DM9051_VIDL);
    value |= DM9051_Read_Reg(DM9051_VIDH) << 8;
    value |= DM9051_Read_Reg(DM9051_PIDL) << 16;
    value |= DM9051_Read_Reg(DM9051_PIDH) << 24;

    Zyk_printf("DM9051 id: 0x%x", value);
    DM9051_device.type  = TYPE_DM9051;
#ifdef FORCE_10M
    DM9051_device.mode    = DM9051_10M;
#else
    DM9051_device.mode    = DM9051_AUTO;
#endif  //FORCE_10M   
    DM9051_device.packet_cnt = 0;
    DM9051_device.queue_packet_len = 0;

    /* SRAM Tx/Rx pointer automatically return to start address                                               */
    /* Packet Transmitted, Packet Received                                                                    */
    DM9051_device.imr_all = IMR_PAR | IMR_PRM; //RX interrupt enable
    //DM9051_device.imr_all = IMR_DEFAULT;
    //DM9051_device.imr_all = 0x81;

#ifdef uIP_NOOS
    DM9051_device.dev_addr[0] = emacETHADDR0;
    DM9051_device.dev_addr[1] = emacETHADDR1;
    DM9051_device.dev_addr[2] = emacETHADDR2;
    DM9051_device.dev_addr[3] = emacETHADDR3;
    DM9051_device.dev_addr[4] = emacETHADDR4;
    DM9051_device.dev_addr[5] = emacETHADDR5;
#endif


    /* RESET device                                                                                           */
    DM9051_Write_Reg(DM9051_NCR, DM9051_REG_RESET);
    _DM9051_Delay(1); //1us
    DM9051_Write_Reg(DM9051_NCR, 0);

    /* RESET device                                                                                           */
    DM9051_Write_Reg(DM9051_NCR, DM9051_REG_RESET);
    _DM9051_Delay(1);
    _DM9051_Delay_ms(1);
    DM9051_Write_Reg(DM9051_NCR, 0);

    DM9051_Write_Reg(DM9051_GPCR, GPCR_GEP_CNTL);
    DM9051_Write_Reg(DM9051_GPR, 0x00);       //Power on PHY

    /* Set PHY                                                                                                */
    phy_mode_set(DM9051_device.mode);

    //_DM9051_Delay_ms(50000);
    //printf("PHY REG_0 = 0x%4x\n", phy_read(0));
    //printf("PHY REG_4 = 0x%4x\n", phy_read(4));
    /* set mac address                                                                                        */
    for (i = 0, oft = DM9051_PAR; i < 6; i++, oft++)
    {
        DM9051_Write_Reg(oft, DM9051_device.dev_addr[i]);
    }

    /* set multicast address      */
    for (i = 0; i < 8; i++)                                /* �M�� ���d�h���]�m */
        DM9051_Write_Reg(DM9051_MAR + i, (7 == i) ? 0x80 : 0x00); /* �t�]�m �s���] �]�m */

    /*
      for (i = 0, oft = DM9051_MAR; i < 8; i++, oft++)
      {
        DM9051_Write_Reg(oft, 0xff);
      }
    */

    DM9051_TRACE2("DM9051 MAC: ");

    for (i = 0, oft = DM9051_PAR; i < 6; i++, oft++)
    {
        DM9051_TRACE2("%X:", DM9051_Read_Reg(oft));
    }

    DM9051_TRACE2("\n");

    /* Activate DM9051                                                                                        */
    /* Setup DM9051 Registers */
    DM9051_Write_Reg(DM9051_NCR, NCR_DEFAULT);
    DM9051_Write_Reg(DM9051_TCR, TCR_DEFAULT);
    //DM9051_Write_Reg(DM9051_TCR, 0x20);                     //Disable underrun retry.
    DM9051_Write_Reg(DM9051_RCR, RCR_DEFAULT);
    DM9051_Write_Reg(DM9051_BPTR, BPTR_DEFAULT);
    //DM9051_Write_Reg(DM9051_FCTR, FCTR_DEAFULT);
    DM9051_Write_Reg(DM9051_FCTR, 0x3A);
    DM9051_Write_Reg(DM9051_FCR,  FCR_DEFAULT);
    //DM9051_Write_Reg(DM9051_FCR,  0x0);                 //Disable Flow_Control
    DM9051_Write_Reg(DM9051_SMCR, SMCR_DEFAULT);
    DM9051_Write_Reg(DM9051_TCR2, DM9051_TCR2_SET);
    //DM9051_Write_Reg(DM9051_TCR2, 0x80);
    DM9051_Write_Reg(DM9051_INTR, 0x1);

    /* Clear status */
    DM9051_Write_Reg(DM9051_NSR, NSR_CLR_STATUS);
    DM9051_Write_Reg(DM9051_ISR, ISR_CLR_STATUS);


    //if (DM9051_device.mode == DM9051_AUTO)
    if ((DM9051_device.mode == DM9051_AUTO) || (DM9051_device.mode == DM9051_10M))
    {
        //printf("DM9051_REG_1fh = 0x%X.\n", DM9051_Read_Reg(0x1f));
        while (!(phy_read(1) & 0x20))
        {
            /* autonegation complete bit                                                                          */
            _DM9051_Delay(2);
            //printf("PHY_REG00h = 0x%04X, PHY_REG01h = 0x%04X.\n", phy_read(0), phy_read(1));
            i++;

            if (i == 255)
            {
                DM9051_TRACE2("could not establish link \n");
                //LCD_ShowString(Line5, LEFT_WIDTH, LEFT_ALIGNMENT, Color_White, FONT_12, "Operation Mode: Connecting");
                //return 0;
                break;
            }
        }
    }

    /* see what we've got                                                                                     */
    lnk = phy_read(17) >> 12;
    DM9051_TRACE2("operating at ");

    switch (lnk)
    {
        case 1:
            DM9051_TRACE2("10M half duplex ");
            //LCD_ShowString(Line5, LEFT_WIDTH, LEFT_ALIGNMENT, Color_White, FONT_12, "Operation Mode: 10M half duplex");
            break;

        case 2:
            DM9051_TRACE2("10M full duplex ");
            //LCD_ShowString(Line5, LEFT_WIDTH, LEFT_ALIGNMENT, Color_White, FONT_12, "Operation Mode: 10M full duplex");
            break;

        case 4:
            DM9051_TRACE2("100M half duplex ");
            //LCD_ShowString(Line5, LEFT_WIDTH, LEFT_ALIGNMENT, Color_White, FONT_12, "Operation Mode: 100M half duplex");
            break;

        case 8:
            DM9051_TRACE2("100M full duplex ");
            //LCD_ShowString(Line5, LEFT_WIDTH, LEFT_ALIGNMENT, Color_White, FONT_12, "Operation Mode: 100M full duplex");
            break;

        default:
            DM9051_TRACE2("unknown: %d ", lnk);
            break;
    }

    DM9051_TRACE2("mode \n");

    DM9051_Write_Reg(DM9051_IMR, DM9051_device.imr_all);    /* Enable TX/RX interrupt mask                    */
    DM9051_Write_Reg(DM9051_RCR, (RCR_DEFAULT | RCR_RXEN));  /* Enable RX */

    return 0;
}
