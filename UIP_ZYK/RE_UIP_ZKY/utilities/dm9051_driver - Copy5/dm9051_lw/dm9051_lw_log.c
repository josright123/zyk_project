#include <stdlib.h>
#include "dm9051opts.h"
#include "dm9051_lw.h"
#include "dm9051_lw_log_types.h"
#include "dm9051_lw_log_inc.h"

//only for Lwip
//#include "../freertos_tasks_debug.h"
//#define printf(fmt, ...) TASK_DM9051_DEBUGF(0, SEMA_OFF, "[xx]", (fmt, ##__VA_ARGS__))

//#include "dm9051_lw_debug.h"
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_LW_LOG, (fmt, ##__VA_ARGS__))

#define MMALLOC_MAX_LEN2	250

#if !DM9051_DEBUG_ENABLE
#error "Davicom sanity, Please don't add this implement C file, because you did NOT enable debug packets dump in the 'dm9051opts.h',\
 Or to HAVE packets dump functions you can define DM9051_DEBUG_ENABLE to 1 in the 'dm9051opts.h'"
#endif

TX_MODLE_DECLARATION;
pkt_monitor_t tx_modle_keep = {
	0,
};
pkt_monitor_t tx_all_modle_keep = {
	0,
};

static const struct uip_eth_addr log_ethbroadcast = {{0xff, 0xff, 0xff, 0xff, 0xff, 0xff}};
#define	IsBroadcast(add)	(memcmp(add, log_ethbroadcast.addr, sizeof(struct uip_eth_addr))==0)
#define	IsMulticast(buf)	(((uint8_t *)(buf))[0] & 0x01)

static void sprint_hex_dump(const char *prefix_str, int rowsize, const void *buf, int seg_start, size_t len) { } //large program code.
static void sprint_arp_field_parse(const void *buf, int segpos, int segsize) { } // complexity program code
static void arp_counting_to_safty_tx(int to_must_safty) { }

void arp_unicast_safty_tx(const uint8_t *buf, size_t len) //(struct net_device *ndev, struct sk_buff *skb)
{
		sprint_hex_dump("CAS3 ", 32, buf, 0, len - 0); // print HEX
		sprint_arp_field_parse(buf, SEG_QUAN, len - SEG_QUAN);
}

void arp_counting_reach_disp_tx(const void *buf, size_t len)
{
		size_t x = (size_t) kkmin(32, (int) len);
		sprint_hex_dump("CAS0", 32, buf, 0, x); // a "CAS-no_unicast_arp_enter"
		sprint_arp_field_parse(buf, SEG_QUAN, len - SEG_QUAN);
}

static void arp_tx(const uint8_t *buf, size_t len, int fullcheck)
{
		if (!IsBroadcast(buf)) {
			/* [unicast-] */
			printf("  Tx a arp_uni-cast [CASE3 ]\r\n");
			arp_unicast_safty_tx(buf, len); //(ndev, skb);
		}

		if (fullcheck) {
			/* [full-display-] */
			arp_counting_reach_disp_tx(buf, len); //counting reach expire
			arp_counting_to_safty_tx(0); //counting reach expire, and rst 0
		} else {
			/* [partial-display-] */
			printf("  arp_counting_to_safty_tx [CAS1 ]\r\n");
			arp_counting_to_safty_tx(0); //counting reach expire, and rst 0
		}
}

static void protocol_tx(const uint8_t *buf, size_t len, int fullcheck)
{
	if (IS_ICMP)
		return;

	if (fullcheck) {
		if (IsMulticast(buf)) {
			if (IS_UDP)
				;
			else if (IPBUF->proto == UIP_PROTO_IGMP_JJ)
				; //printk("TTX %2d dm9 tx misc %3d multicast igmp\n", cb.tx_total.val, cb.misc_total_tx.val);
			else
				sprint_hex_dump("MISC", 32, buf, 0, 14+20+20);
		} else {
			if (IPBUF->proto == UIP_PROTO_TCP)
				sprint_hex_dump("MISC", 32, buf, 0, 14+20+20);
			else if (IS_UDP)
				sprint_hex_dump("MISC", 32, buf, 0, 14+20+20);
			else
				sprint_hex_dump("MISC", 32, buf, 0, 14+20+20);
		}
	}
}

void dm_check_tx(const uint8_t *buf, size_t len)
{
	if (IS_ARP) { //cb.arp_total_tx.val++;
		arp_tx(buf, len, DEF_MONITOR_NTX);
	}
	else if (IS_DHCPTX) {
		; //dhcp_tx(buf);
	} else { //cb.misc_total_tx.val++;
		protocol_tx(buf, len, DEF_MONITOR_NTX);
	}
}

int DBG_IS_ARP(void *dataload) {
	const uint8_t *buf = dataload;
	return IS_ARP;
}
int DBG_IS_TCP(void *dataload) {
	const uint8_t *buf = dataload;
	return IS_TCP;
}

int master_UDP_unknow = 0;
void dm9051_disp_tx(const uint8_t *buf, uint16_t len)
{
  static int count = 0;
  count++;

  if (IS_ICMP)
	return;

  if (IS_ARP)
	return;

  if (IS_UDP) {
	  if (IS_DHCPTX) {
		printf("---------------Sending DHCP total_tx(%d) Len %u\r\n", count, len);
		function_monitor_tx_all(HEAD_SPC, 0, NULL, buf, len);
	  }
	  return;
  }
  
  if (IS_TCP) {
	  function_monitor_tx_all(HEAD_SPC, 0, "<TCP>", buf, len);
	  return;
  }
}

int dm9051_disp_and_check_rx(const uint8_t *buf, uint16_t len)
{
  static int master_TCP_count = 0;

  if (IsMulticast(buf)) {
    if (IS_ARP)
		return 0;

	if (IS_DHCPRX) {
		printf("Receive DHCP pkt (%02x:%02x:%02x:%02x:%02x:%02x) len %d ---------------\r\n",
			buf[0],buf[1],buf[2],buf[3],buf[4],buf[5], len);

	printf("Your client ID: %d.%d.%d.%d\r\n", buf[0x3a], buf[0x3b], buf[0x3c], buf[0x3d]); //0x3a = 58
	printf("\r\n");
	return 0;
	}
	return 0;
  }

  if (IS_ARP)
	return 0; //uni-cast arp

  if (IS_ICMP)
		return 0;

  if (IS_IP) {
	if (IS_TCP) {
		if (IS_TCP) {
			master_TCP_count++;
			if (len > 70) { //add.
				printf("\r\n");
				printf("Receive unit-cast pkt. Protocol %s (%d), Listen port %d len %d (%d) ---------------\r\n", "TCP",
						IPBUF->proto, HTONS(TCPBUF->destport), len, master_TCP_count);
				function_monitor_rx(HEAD_SPC, NULL, buf, len);
			}
			return 0;
		}
	}
	return 0;
  }

  printf("Receive unit-cast UNKNOW pkt ---------------\r\n");
  function_monitor_rx(HEAD_SPC, "UNKNOW pkt <<rx  ", buf, len); //(buffer, l);
  return 1; //err
}

void dm9051_txlog_monitor_tx(int hdspc, const uint8_t *buffer, size_t len)
{
	if (tx_modle_keep.allow_num < tx_modle.allow_num) {
		tx_modle_keep.allow_num++;
#if LWIP_TESTMODE
		if (tx_modle_keep.allow_num == 1 || tx_modle_keep.allow_num == 5 || // because we do checksum offload teat, only meaning with packet1/packet9
			tx_modle_keep.allow_num == 9 || tx_modle_keep.allow_num == 13)
#endif
		do {
			#define HEAD_LEN	MMALLOC_MAX_LEN2 //3KB --> 1kb
			char *heads;
			int n;
			heads = (char *) malloc(HEAD_LEN); // note: memory allocation WITH <stdlib.h>!
			
			printf("  ### tx [ %lx = malloc( %d )  as a reference for headstr of _txlog_monitor_tx]\r\n",
				heads, HEAD_LEN);
			
			n = sprintf(heads, "%d/%d len %lu", tx_modle_keep.allow_num, tx_modle.allow_num, len);
			sprintf(heads, "%d/%d len %lu tx[%d]>>", tx_modle_keep.allow_num, tx_modle.allow_num, len, mstep_get_net_index());

			function_monitor_tx(hdspc, n, /*NULL*/ heads, buffer, len);
			free(heads);
		} while(0);
	}
}

void dm9051_rxlog_monitor_rx(int hdspc, char *cause_str, const uint8_t *buffer, size_t len)
{
	function_monitor_rx(hdspc, cause_str, buffer, len);
}

/*
 * TX log is design as: dm9051_txlog_monitor_tx_all()
 *   hdspc: 0 ,must print-out
 *          ~0, only print-out 'rx_modle.allow_num' times.
 * *Will exechange to ~0 must print-out, and others 'rx_modle.allow_num' times.
 */
void dm9051_txlog_monitor_tx_all(int hdspc, const uint8_t *buffer, size_t len)
{
	if (hdspc == 0) {
		bannerline_log();
		function_monitor_tx_all(0, 0, "[tx_all]", buffer, len);
	} else if (tx_all_modle_keep.allow_num < tx_all_modle.allow_num) {
		#define HEAD_LEN	MMALLOC_MAX_LEN2 //3KB --> 1kb
		char *heads;
		int n;
		tx_all_modle_keep.allow_num++;

		heads = (char *) malloc(HEAD_LEN); // note: memory allocation WITH <stdlib.h>!
		  n = sprintf(heads, "%d/%d len %lu", tx_all_modle_keep.allow_num, tx_all_modle.allow_num, len);
		  sprintf(heads, "txALL[%d]>>len %lu %d/%d", mstep_get_net_index(), len,
			tx_all_modle_keep.allow_num, tx_all_modle.allow_num);

		  bannerline_log();
		  function_monitor_tx_all(/*hdspc*/ tx_all_modle_keep.allow_num, n, heads, buffer, len);

		free(heads);
	}
}

uint16_t unitcast_UNKNOW_pkt = 0;

void dm9051_rx_unknow_pkt_inc_count(void) {
	unitcast_UNKNOW_pkt++;
}

void dm9051_rxlog_monitor_rx_all(int hdspc, const uint8_t *buffer, size_t len)
{
	function_monitor_rx_all(hdspc, NULL, buffer, len);
}

void dm9051_txlog_disp(uint8_t *buffer, int len)
{
#if DM9051_DEBUG_ENABLE
  dm9051_disp_tx(buffer, len);
#endif
}

void dm9051_rxlog_arp(void *payload, uint16_t tot_len, uint16_t len)
{
#if DM9051_DEBUG_ENABLE
  if (tot_len == LEN64) {
	if (DBG_IS_ARP(payload))
		;
  }
#endif
}

void dm9051_rxlog_ack(void *payload, uint16_t tot_len, char *result_str)
{
#if DM9051_DEBUG_ENABLE
  if (tot_len == 64) {
	  if (DBG_IS_TCP(payload)) {
		  printf(" .[debug process this 'ACK'] is %s\r\n", result_str);
	  }
  }
#endif
}
