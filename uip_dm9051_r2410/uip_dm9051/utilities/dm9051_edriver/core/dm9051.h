/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __DM9051_H
#define __DM9051_H
#include "config/dm9051opts.h"
//#include "../config/identify_opts.h"
//#include "dm9051_hal.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Private constants ---------------------------------------------------------------------------------------*/
#define DM9051_PHY (0x40) /* PHY address 0x01 */

// #define PHY_ADDRESS       (0x01)       	/*!< relative to at32 board */
#define PHY_CONTROL_REG (0x00)  /*!< basic mode control register */
#define PHY_STATUS_REG (0x01)   /*!< basic mode status register */
#define PHY_LINKED_BIT (0x0004) /*!< link status */

/* Exported constants --------------------------------------------------------------------------------------*/
#define DM9051_ID (0x90510A46) /* DM9051A ID                                               */
#define DM9052_ID (0x90550A46) /* DM9051A ID                                               */
#define DM9051_PKT_MAX (1536)  /* Received packet max size                                     */
#define DM9051_PKT_RDY (0x01)  /* Packet ready to receive                                      */

#define DM9051_NCR (0x00)
#define DM9051_NSR (0x01)
#define DM9051_TCR (0x02)
#define DM9051_TSR1 (0x03)
#define DM9051_TSR2 (0x04)
#define DM9051_RCR (0x05)
#define DM9051_RSR (0x06)
#define DM9051_ROCR (0x07)
#define DM9051_BPTR (0x08)
#define DM9051_FCTR (0x09)
#define DM9051_FCR (0x0A)
#define DM9051_EPCR (0x0B)
#define DM9051_EPAR (0x0C)
#define DM9051_EPDRL (0x0D)
#define DM9051_EPDRH (0x0E)
#define DM9051_WCR (0x0F)

#define DM9051_PAR (0x10)
#define DM9051_MAR (0x16)

#define DM9051_GPCR (0x1e)
#define DM9051_GPR (0x1f)
#define DM9051_TRPAL (0x22)
#define DM9051_TRPAH (0x23)
#define DM9051_RWPAL (0x24)
#define DM9051_RWPAH (0x25)

#define DM9051_VIDL (0x28)
#define DM9051_VIDH (0x29)
#define DM9051_PIDL (0x2A)
#define DM9051_PIDH (0x2B)

#define DM9051_CHIPR (0x2C)
#define DM9051_TCR2 (0x2D)
#define DM9051_OTCR (0x2E)
#define DM9051_SMCR (0x2F)

#define DM9051_ETCR (0x30)  /* early transmit control/status register                             */
#define DM9051_CSCR (0x31)  /* check sum control register                                         */
#define DM9051_RCSSR (0x32) /* receive check sum status register                                  */

#define DM9051_PBCR (0x38)
#define DM9051_INTR (0x39)
#define DM9051_PPCR (0x3D)
#define DM9051_MPCR (0x55)
#define DM9051_LMCR (0x57)
#define DM9051_MBNDRY (0x5E)
#define DM9051_MRCMDX (0x70)
#define DM9051_MRCMD (0x72)
#define DM9051_MRRL (0x74)
#define DM9051_MRRH (0x75)
#define DM9051_MWCMDX (0x76)
#define DM9051_MWCMD (0x78)
#define DM9051_MWRL (0x7A)
#define DM9051_MWRH (0x7B)
#define DM9051_TXPLL (0x7C)
#define DM9051_TXPLH (0x7D)
#define DM9051_ISR (0x7E)
#define DM9051_IMR (0x7F)

#define OPC_REG_W 0x80 // Register Write
#define OPC_REG_R 0x00 // Register Read

#define CHIPR_DM9051A (0x19)
#define CHIPR_DM9051B (0x1B)

#define DM9051_NCR_RESET (0x01)
#define DM9051_IMR_OFF (0x80)
#define DM9051_TCR2_SET (0x90) /* set one packet */
#define DM9051_RCR_SET (0x31)
#define DM9051_BPTR_SET (0x37)
#define DM9051_FCTR_SET (0x38)
#define DM9051_FCR_SET (0x28)
  // #define DM9051_TCR_SET    (0x01)

#define NCR_EXT_PHY (1 << 7)
#define NCR_WAKEEN (1 << 6)
#define NCR_FCOL (1 << 4)
#define NCR_FDX (1 << 3)
#define NCR_LBK (3 << 1)
#define NCR_RST (1 << 0)
#define NCR_DEFAULT (0x0) /* Disable Wakeup */

#define NSR_SPEED (1 << 7)
#define NSR_LINKST (1 << 6)
#define NSR_WAKEST (1 << 5)
#define NSR_TX2END (1 << 3)
#define NSR_TX1END (1 << 2)
#define NSR_RXOV (1 << 1)
#define NSR_RXRDY (1 << 0)
#define NSR_CLR_STATUS (NSR_WAKEST | NSR_TX2END | NSR_TX1END)

#define TCR_TJDIS (1 << 6)
#define TCR_EXCECM (1 << 5)
#define TCR_PAD_DIS2 (1 << 4)
#define TCR_CRC_DIS2 (1 << 3)
#define TCR_PAD_DIS1 (1 << 2)
#define TCR_CRC_DIS1 (1 << 1)
#define TCR_TXREQ (1 << 0) /* Start TX */
#define TCR_DEFAULT (0x0)

#define TSR_TJTO (1 << 7)
#define TSR_LC (1 << 6)
#define TSR_NC (1 << 5)
#define TSR_LCOL (1 << 4)
#define TSR_COL (1 << 3)
#define TSR_EC (1 << 2)

#define RCR_WTDIS (1 << 6)
#define RCR_DIS_LONG (1 << 5)
#define RCR_DIS_CRC (1 << 4)
#define RCR_ALL (1 << 3)
#define RCR_RUNT (1 << 2)
#define RCR_PRMSC (1 << 1)
#define RCR_RXEN (1 << 0)
#define RCR_DEFAULT (RCR_DIS_LONG | RCR_DIS_CRC)

#define RSR_RF (1 << 7)
#define RSR_MF (1 << 6)
#define RSR_LCS (1 << 5)
#define RSR_RWTO (1 << 4)
#define RSR_PLE (1 << 3)
#define RSR_AE (1 << 2)
#define RSR_CE (1 << 1)
#define RSR_FOE (1 << 0)

#define INTR_ACTIVE_LOW (1 << 0)

#define BPTR_DEFAULT (0x3f)
#define FCTR_DEAFULT (0x38)
#define FCR_DEFAULT (0xFF)
#define FCR_DEFAULT1 (0x39)
#define FCR_DEFAULT_CONF FCR_DEFAULT
#define SMCR_DEFAULT (0x0)
#define PBCR_MAXDRIVE (0x44)

/* 0x31 */
// #define _DM9051_CSCR      (0x31)    /* check sum control register                                         */
#define TCSCR_UDPCS_ENABLE (1 << 2)
#define TCSCR_TCPCS_ENABLE (1 << 1)
#define TCSCR_IPCS_ENABLE (1 << 0)
/* 0x32 */
// #define _DM9051_RCSSR     (0x32)    /* receive check sum status register                                  */
#define RCSSR_UDPS (1 << 7)
#define RCSSR_TCPS (1 << 6)
#define RCSSR_IPS (1 << 5)
#define RCSSR_UDPP (1 << 4)
#define RCSSR_TCPP (1 << 3)
#define RCSSR_IPP (1 << 2)
#define RCSSR_RCSEN (1 << 1) // Receive Checksum Checking Enable
#define RCSSR_DCSE (1 << 0)  // Discard Checksum Error Packet
/* rxb */
#define RXB_ERR (1 << 1)
#define RXB_RDY (1 << 0)

/* 0x3D */
/* Pause Packet Control Register - default = 1 */
#define PPCR_PAUSE_COUNT 0x08

/* 0x57 */
/* LEDMode Control Register - LEDMode1 */
/* Value 0x81 : bit[7] = 1, bit[2] = 0, bit[1:0] = 01b */
#define LMCR_NEWMOD (1 << 7)
#define LMCR_TYPED1 (1 << 1)
#define LMCR_TYPED0 (1 << 0)
#define LMCR_MODE1 (LMCR_NEWMOD | LMCR_TYPED0)

/* 0x5E */
#define MBNDRY_WORD 0
#define MBNDRY_BYTE (1 << 7)
  // #define MBNDRY_DEFAULTx	MBNDRY_WORD //MBNDRY_BYTE

#define ISR_PR (1 << 0)

#define IMR_PAR (1 << 7)
#define IMR_PRM (1 << 0)

  //[API]uip
  const uint8_t *dm9051_init(const uint8_t *adr);
  uint16_t dm9051_rx(uint8_t *buff);
  void dm9051_tx(uint8_t *buf, uint16_t len);

  //[API]lwip
  // uint16_t dm9051_read_chip_id(void);
  // uint16_t dm9051_read_bmsr(void);
  // uint16_t dm9051_read_control_status(void);
  // void dm9051_read_rx_pointers(uint16_t *rwpa_wt, uint16_t *mdra_rd);
  // void dm9051_probe_link(int nsr_poll);
  // void dm9051_write_rst_isr(void);

#if 0
  int link_flag(void);
  uint16_t evaluate_link(void);
  unsigned long get_fire_polling(void);
#endif

  //[API]env.h
  int env_init_setup(uint16_t *id);
  uint16_t env_evaluate_rxb(uint8_t rxb);
  uint16_t env_err_rsthdlr(char *err_explain_str, uint32_t valuecode);
  uint16_t env_err_rsthdlr1(void (*callback)(char *, uint32_t), char *explain_str, uint32_t code);
  uint16_t env_err_rsthdlr2(int sz);
  uint16_t env_err_rsthdlr3(const char *format, ...); //uint16_t env_err_rsthdlr3(fmt, ...);

	void debug_diff_rx_pointers(int state, uint16_t rd_now);

#if 0
  //#define _env_err_rsthdlr3(fmt, ...) proc_err_rsthdlr3(fmt, ##__VA_ARGS__)
  #define proc_err_rsthdlr3(message) \
	return printf(message)
    //_env_reset_process(identified_eth_mac());
    //return 0
	
  //uint16_t proc_err_rsthdlr3(char *message);
#endif

  uint16_t cspi_phy_read(uint16_t uReg);
  void cspi_phy_write(uint16_t reg, uint16_t value); //[function "phy_write" was available but could never referenced.]
  // uint16_t eeprom_read(uint16_t uWord);

  uint16_t cspi_read_chip_id(void);
  uint16_t cspi_read_control_status(void);

  void cspi_read_rx_pointers(uint16_t *rwpa_wt, uint16_t *mdra_rd);
  uint16_t cspi_diff_rxpa(void);

  void cspi_read_regs_info(uint8_t *stat);

  uint16_t cspi_isr_enab(void); // read and/then write

  void cspi_vid_pid_revisions(uint8_t *ids, uint8_t *rev_ad);
  void read_chip_revision(uint8_t *ids, uint8_t *rev_ad);
  void impl_read_par(uint8_t *buff);

  void cspi_phycore_on(uint16_t nms);
  void cspi_ncr_reset(uint16_t nms);
  void cspi_soft_default(void);

  // void _cspi_core_reset(void);
  // const uint8_t *cspi_dm_start1(const uint8_t *adr);
  // void cspi_set_par(const uint8_t *macadd);
  void cspi_set_mar(void);
  void cspi_set_recv(void);
  // void cspi_rx_mode(void); // ethernetif.c (of _lwip_set_mac_address())
  void cspi_rx_head(uint8_t *receivedata);
  void cspi_rx_read(uint8_t *buff, uint16_t rx_len);
  void cspi_tx_write(uint8_t *buf, uint16_t len);
  void cspi_tx_req(void);

//
// #include "dm_identify_impl.h"
//

/* APIs
 */
//#define DM_TYPE 0
//#include "identify/dm_identify_types_define.h"

//  struct eth_node_t
//  {
//    uint8_t mac_addresse[MAC_ADDR_LENGTH];
//    uint8_t local_ipaddr[ADDR_LENGTH];
//    uint8_t local_gwaddr[ADDR_LENGTH];
//    uint8_t local_maskaddr[ADDR_LENGTH];
//  };

  /*
   * HCC: Hard Core Candidate (hcc)
   */

  extern const struct eth_node_t node_candidate[1];

/* APIs.identify
 */
#define candidate_eth_mac() &node_candidate[0].mac_addresse[0]    //[pin_code]
#define candidate_eth_ip() &node_candidate[0].local_ipaddr[0]     //[pin_code]
#define candidate_eth_gw() &node_candidate[0].local_gwaddr[0]     //[pin_code]
#define candidate_eth_mask() &node_candidate[0].local_maskaddr[0] //[pin_code]
// const uint8_t *identify_eth_mac(const uint8_t *macadr);
// uint8_t *identify_tcpip_ip(uint8_t *ip4adr);
// uint8_t *identify_tcpip_gw(uint8_t *ip4adr);
// uint8_t *identify_tcpip_mask(uint8_t *ip4adr);
// void trace_identify_eth_mac(void);
#define identify_eth_mac(macadr) SET_FIELD(final_mac, macadr ? macadr : candidate_eth_mac())
#define identify_tcpip_ip(ip4adr) SET_FIELD(final_ip, ip4adr ? ip4adr : candidate_eth_ip())
#define identify_tcpip_gw(ip4adr) SET_FIELD(final_gw, ip4adr ? ip4adr : candidate_eth_gw())
#define identify_tcpip_mask(ip4adr) SET_FIELD(final_mask, ip4adr ? ip4adr : candidate_eth_mask())
#define trace_identify_eth_mac()                            \
  do                                                        \
  {                                                         \
    const uint8_t *mac = GET_FIELD(final_mac);              \
    printf("mac address %02x%02x%02x%02x%02x%02x\r\n",      \
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]); \
  } while (0)

/* APIs.identified
 */
// const uint8_t *identified_eth_mac(void);
// uint8_t *identified_tcpip_ip(void);
// uint8_t *identified_tcpip_gw(void);
// uint8_t *identified_tcpip_mask(void);
#define identified_eth_mac() GET_FIELD(final_mac)
#define identified_tcpip_ip() GET_FIELD(final_ip)
#define identified_tcpip_gw() GET_FIELD(final_gw)
#define identified_tcpip_mask() GET_FIELD(final_mask)

#ifdef __cplusplus
}
#endif

#endif //__DM9051_H
