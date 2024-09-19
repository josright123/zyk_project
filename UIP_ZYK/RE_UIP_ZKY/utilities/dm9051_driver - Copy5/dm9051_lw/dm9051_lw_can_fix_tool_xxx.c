#include "lwip/ip.h"
#include "lwip/udp.h"
//#include "lwip/tcp.h"
#include "lwip/prot/tcp.h"
#include "lwip/prot/ethernet.h"
#include "dm9051opts.h"
#include "dm9051_lw.h"
#include "dm9051_lw_cint.h"
#include "../freertos_tasks_debug.h"
#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_ETHERNETIF_ON, SEMA_OFF, "[E]", (fmt, ##__VA_ARGS__))

uint16_t dm9051_read_chip_id(void)
{
  return 0x9051;
}

uint16_t dm9051_bmsr_update(void)
{
  return 0x786d;
}

//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_read_rx_pointers (referred from .\objects\ethernetif.o).

//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_err_hdlr (referred from .\objects\ethernetif.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol printk (referred from .\objects\dm9051_lw_can_fix_tool_xxx.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_read_chip_id (referred from .\objects\ethernetif.o).

//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_irq_isr_disab (referred from .\objects\ethernetif.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_irq_isr_enab (referred from .\objects\ethernetif.o).

//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol gpio_pin_config (referred from .\objects\dm9051_board_impl.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_boards_initialize (referred from .\objects\main.o).

//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cpin_poweron_reset (referred from .\objects\dm9051_bl_impl_if.o).

//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DM9051_Read_Reg (referred from .\objects\dm9051_lw_clog.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cpin_poweron_reset (referred from .\objects\dm9051_bl_impl_if.o). //in _crst.c
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DM9051_Write_Reg (referred from .\objects\dm9051.o).

//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol rx_modle (referred from .\objects\dm9051_lw_cusart.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol rx_modle_count (referred from .\objects\dm9051_lw_cusart.o).

//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol first_log_get (referred from .\objects\dm9051_bl_impl.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol first_log_init (referred from .\objects\dm9051_bl_impl.o).

void check_link_up_can_fix(struct netif *netif, uint32_t id_bmsr)
{
  uint16_t rwpa_w, mdra_ingress;
  
	  //bannerline_log();
	  printk("\r\n");
	  printf("bmsr %04x. (Chip ID %04x) dm9051-link up %u.%u.%u.%u ...\r\n",
			id_bmsr & 0xffff, id_bmsr >> 16,
			ip4_addr1_16(netif_ip4_addr(netif)), 
			ip4_addr2_16(netif_ip4_addr(netif)),
			ip4_addr3_16(netif_ip4_addr(netif)), 
			ip4_addr4_16(netif_ip4_addr(netif)));
	  printf("ethernetif_link(): %s\r\n", intr_gpio_mptr() ? "intr_mode" : "poll_mode");
	  
	  dm9051_read_rx_pointers(&rwpa_w, &mdra_ingress);
	  printf("dm9051_restart rwpa %04x / igrss %04x\r\n", rwpa_w, mdra_ingress); //"(pin = %d) at", mstep_get_net_index(), 
	  printk("\r\n");
	  
	  bannerline_log();
	  if (rwpa_w == mdra_ingress) //condition!!
		printf("dm9051_restart rx pointers good\r\n"); //"(pin = %d)", mstep_get_net_index(), 
	  else
		dm9051_err_hdlr("_dm9051f linkup do rst : 0x%02x\r\n", DM9051_NUM_LINKUP_RST, 0); //xxxxxx

}
