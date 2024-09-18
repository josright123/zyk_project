#include "stdint.h"
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __DM9051_REG_H
#define __DM9051_REG_H

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

#define uIP_NOOS             /* uIP driver, if not for LwIP driver */ //jerry add
//#define FORCE_10M
#define FifoPointCheck
//#define DM9051_INT

#define DM9051_MSG              1
#define DM9051_DEBUG            1
#define Rx_Int_enable                       1

#define DM9051_PHY              (0x40)    /* PHY address 0x01 */

/* Private typedef */
enum DM9051_PHY_mode
{
    DM9051_10MHD   = 0,
    DM9051_100MHD  = 1,
    DM9051_10MFD   = 4,
    DM9051_100MFD  = 5,
    DM9051_10M     = 6,
    DM9051_AUTO    = 8,
    DM9051_1M_HPNA = 0x10
};

enum DM9051_TYPE
{
    TYPE_DM9051E,
    TYPE_DM9051A,
    TYPE_DM9051B,
    TYPE_DM9051
};

struct DM9051_eth
{
    enum DM9051_TYPE type;
    enum DM9051_PHY_mode mode;

    uint8_t  imr_all;

    uint8_t  packet_cnt;                    /* packet I or II                                                 */
    uint16_t queue_packet_len;              /* queued packet (packet II)                                      */

    /* interface address info.                                                                                */
    uint8_t  dev_addr[6];                   /* hw address                                                     */

};


#if (DM9051_DEBUG == 1)
#define DM9051_TRACE  printf
#else
#define DM9051_TRACE(...)
#endif

#if (DM9051_MSG == 1)
#define DM9051_TRACE2  printf
#else
#define DM9051_TRACE2(...)
#endif

/* Exported constants --------------------------------------------------------------------------------------*/
#define DM9000_ID           (0x90000A46)    /* DM9051A ID                                                   */
#define DM9051_ID           (0x90510A46)
#define DM9051_PKT_MAX      (1536)          /* Received packet max size                                     */
#define DM9051_PKT_RDY      (0x01)          /* Packet ready to receive                                      */

#define DM9051_NCR          (0x00)
#define DM9051_NSR          (0x01)
#define DM9051_TCR          (0x02)
#define DM9051_TSR1         (0x03)
#define DM9051_TSR2         (0x04)
#define DM9051_RCR          (0x05)
#define DM9051_RSR          (0x06)
#define DM9051_ROCR         (0x07)
#define DM9051_BPTR         (0x08)
#define DM9051_FCTR         (0x09)
#define DM9051_FCR          (0x0A)
#define DM9051_EPCR         (0x0B)
#define DM9051_EPAR         (0x0C)
#define DM9051_EPDRL        (0x0D)
#define DM9051_EPDRH        (0x0E)
#define DM9051_WCR          (0x0F)

#define DM9051_PAR          (0x10)
#define DM9051_MAR          (0x16)

#define DM9051_GPCR         (0x1e)
#define DM9051_GPR          (0x1f)
#define DM9051_TRPAL        (0x22)
#define DM9051_TRPAH        (0x23)
#define DM9051_RWPAL        (0x24)
#define DM9051_RWPAH        (0x25)

#define DM9051_VIDL         (0x28)
#define DM9051_VIDH         (0x29)
#define DM9051_PIDL         (0x2A)
#define DM9051_PIDH         (0x2B)

#define DM9051_CHIPR        (0x2C)
#define DM9051_TCR2         (0x2D)
#define DM9051_OTCR         (0x2E)
#define DM9051_SMCR         (0x2F)

#define DM9051_ETCR         (0x30)    /* early transmit control/status register                             */
#define DM9051_CSCR         (0x31)    /* check sum control register                                         */
#define DM9051_RCSSR        (0x32)    /* receive check sum status register                                  */

#define DM9051_INTR                  0x39
#define DM9051_MPCR              0x55
#define DM9051_MRCMDX       (0x70)
#define DM9051_MRCMD        (0x72)
#define DM9051_MRRL         (0x74)
#define DM9051_MRRH         (0x75)
#define DM9051_MWCMDX       (0x76)
#define DM9051_MWCMD        (0x78)
#define DM9051_MWRL         (0x7A)
#define DM9051_MWRH         (0x7B)
#define DM9051_TXPLL        (0x7C)
#define DM9051_TXPLH        (0x7D)
#define DM9051_ISR          (0x7E)
#define DM9051_IMR          (0x7F)

#define DM9000_MRCMDX       (0xF0)
#define DM9000_MRCMD        (0xF2)
#define DM9000_MRRL         (0xF4)
#define DM9000_MRRH         (0xF5)
#define DM9000_MWCMDX       (0xF6)
#define DM9000_MWCMD        (0xF8)
#define DM9000_MWRL         (0xFA)
#define DM9000_MWRH         (0xFB)
#define DM9000_TXPLL        (0xFC)
#define DM9000_TXPLH        (0xFD)
#define DM9000_ISR          (0xFE)
#define DM9000_IMR          (0xFF)

#define CHIPR_DM9051A       (0x19)
#define CHIPR_DM9051B       (0x1B)

#define DM9051_REG_RESET     0x01
#define DM9051_IMR_OFF       0x80
#define DM9051_TCR2_SET      0x90   //one packet
#define DM9051_RCR_SET       0x31
#define DM9051_BPTR_SET      0x37
#define DM9051_FCTR_SET      0x38
#define DM9051_FCR_SET       0x28
#define DM9051_TCR_SET       0x01


#define NCR_EXT_PHY         (1 << 7)
#define NCR_WAKEEN          (1 << 6)
#define NCR_FCOL            (1 << 4)
#define NCR_FDX             (1 << 3)
#define NCR_LBK             (3 << 1)
#define NCR_RST             (1 << 0)
#define NCR_DEFAULT                  0x0                        // Disable Wakeup

#define NSR_SPEED           (1 << 7)
#define NSR_LINKST          (1 << 6)
#define NSR_WAKEST          (1 << 5)
#define NSR_TX2END          (1 << 3)
#define NSR_TX1END          (1 << 2)
#define NSR_RXOV            (1 << 1)
#define NSR_CLR_STATUS      (NSR_WAKEST | NSR_TX2END | NSR_TX1END)

#define TCR_TJDIS           (1 << 6)
#define TCR_EXCECM          (1 << 5)
#define TCR_PAD_DIS2        (1 << 4)
#define TCR_CRC_DIS2        (1 << 3)
#define TCR_PAD_DIS1        (1 << 2)
#define TCR_CRC_DIS1        (1 << 1)
#define TCR_TXREQ           (1 << 0)        //Start TX
#define TCR_DEFAULT                  0x0

#define TSR_TJTO            (1 << 7)
#define TSR_LC              (1 << 6)
#define TSR_NC              (1 << 5)
#define TSR_LCOL            (1 << 4)
#define TSR_COL             (1 << 3)
#define TSR_EC              (1 << 2)

#define RCR_WTDIS           (1 << 6)
#define RCR_DIS_LONG        (1 << 5)
#define RCR_DIS_CRC         (1 << 4)
#define RCR_ALL             (1 << 3)
#define RCR_RUNT            (1 << 2)
#define RCR_PRMSC           (1 << 1)
#define RCR_RXEN            (1 << 0)
#define RCR_DEFAULT     (RCR_DIS_LONG | RCR_DIS_CRC)

#define RSR_RF              (1 << 7)
#define RSR_MF              (1 << 6)
#define RSR_LCS             (1 << 5)
#define RSR_RWTO            (1 << 4)
#define RSR_PLE             (1 << 3)
#define RSR_AE              (1 << 2)
#define RSR_CE              (1 << 1)
#define RSR_FOE             (1 << 0)

#define BPTR_DEFAULT                 0x3f
#define FCTR_DEAFULT                 0x38
#define FCR_DEFAULT                  0xFF
#define SMCR_DEFAULT                 0x0

//#define FCTR_HWOT(ot)       ((ot & 0xF ) << 4 )
//#define FCTR_LWOT(ot)       (ot & 0xF )

#define IMR_PAR             (1 << 7)
#define IMR_LNKCHGI         (1 << 5)
#define IMR_UDRUN                       (1 << 4)
#define IMR_ROOM            (1 << 3)
#define IMR_ROM             (1 << 2)
#define IMR_PTM             (1 << 1)
#define IMR_PRM             (1 << 0)
#define IMR_FULL                        (IMR_PAR | IMR_LNKCHGI | IMR_UDRUN | IMR_ROOM | IMR_ROM | IMR_PTM | IMR_PRM)
#define IMR_OFF                          IMR_PAR
#define IMR_DEFAULT                 (IMR_PAR | IMR_PRM | IMR_PTM)

#define ISR_ROOS            (1 << 3)
#define ISR_ROS             (1 << 2)
#define ISR_PTS             (1 << 1)
#define ISR_PRS             (1 << 0)
#define ISR_CLR_STATUS      (0x80 | 0x3F)

#define EPCR_REEP           (1 << 5)
#define EPCR_WEP            (1 << 4)
#define EPCR_EPOS           (1 << 3)
#define EPCR_ERPRR          (1 << 2)
#define EPCR_ERPRW          (1 << 1)
#define EPCR_ERRE           (1 << 0)

#define GPCR_GEP_CNTL       (1 << 0)

#define MAX_BUFFER_SIZE          1600           // Maximum buffer Master will send

#define SPI_WR_BURST                 0xF8;
#define SPI_RD_BURST                 0x72;
#define SPI_READ                     0x03
#define SPI_WRITE                0x04

//#define DM9051_IRQHandler                   EINT0_IRQHandler
//#define  SPI_WRITE_BUFFER  0x05        // Send a series of bytes from the Master to the Slave
//#define  SPI_READ_BUFFER   0x06        // Send a series of bytes from the Slave to the Master

uint8_t DM9051_Read_Reg(uint8_t Reg_Off);
void DM9051_Write_Reg(uint8_t Reg_Off, uint8_t spi_data);
void DM9051_Read_Mem(uint8_t *pu8data, uint32_t datalen);
void DM9051_Write_Mem(uint8_t *pu8data, uint32_t datalen);

int32_t DM9051_INIT(void);
uint16_t DM9051_RX(void);
uint32_t DM9051_TX(void);
#ifdef DM9051_INT
void DM9051_IRQEnable(void);
void DM9051_isr(void);
uint8_t DM9051_Intstatus(void);
void DM9051_Intdisable(void);
void DM9051_Intenable(void);
uint8_t DM9051_IntPinstatus(void);
#endif
#ifdef __cplusplus
}
#endif

#endif /* __DM9051_REG_H -----------------------------------------------------------------------------------*/
