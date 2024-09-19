/**
  **************************************************************************
  * @file     dm9051_env.c ,dm9051_config.c ,at32_emac.c
  * @version  v1.0
  * @date     2023-04-28
  * @brief    dm9051 config program
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
#include "stdio.h"
//#include "dm9051opts.h"
//#include "dm9051_lw.h"
//#include "cboard/dm9051_lw_mcu_default_IN.h"
//#include "cboard/dm9051_cstate.h"
#include "dm9051_env.h"

#include "dm_eth/dm_eth.h"
#include "dm9051_debug.h"
#define	printk(fmt, ...) TASK_DM9051_DEBUGK(SEMA_ON, (fmt, ##__VA_ARGS__))

/* Only used, in [ env.c ] */
typedef uint8_t mac_t[MAC_ADDR_LENGTH];
typedef uint8_t ip_t[ADDR_LENGTH];

#define DM_TYPE		1
#include "dm_types2.h"

#define DM_TYPE		2
#include "dm_types2.h"

const uint8_t *identify_eth_mac(const uint8_t *macadr, int showflg) {
	const uint8_t *mac;
	SET_FIELD(final_mac, macadr); //DM_SET_FIELDmac(macadr);

	mac = identified_eth_mac();
	if (showflg)
		printf("mac address %02x%02x%02x%02x%02x%02x\r\n",
				mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	return mac;
}

uint8_t *identified_eth_mac(void) {
	return GET_FIELD(final_mac); //DM_GET_FIELDmac(); //DM_GET_FIELD(mac_t, final_mac);
}

uint8_t *identify_tcpip_ip(uint8_t *ip4adr) {
	SET_FIELD(final_ip, ip4adr); //DM_SET_FIELD_ips(dm.final_ip, ip4adr); //DM_SET_FIELD(ip_t ,ip, ip4adr ? ip4adr : candidate_eth_ip());
	return GET_FIELD(final_ip); //DM_GET_FIELD_ips(dm.final_ip); //return DM_GET_FIELD(ip_t, ip);
}
uint8_t *identify_tcpip_gw(uint8_t *ip4adr) {
	SET_FIELD(final_gw, ip4adr); //DM_SET_FIELD_ips(dm.final_gw, ip4adr); //DM_SET_FIELD(ip_t ,gw, ip4adr ? ip4adr : candidate_eth_gw());
	return GET_FIELD(final_gw); //DM_GET_FIELD_ips(dm.final_gw); //return DM_GET_FIELD(ip_t, gw);
}
uint8_t *identify_tcpip_mask(uint8_t *ip4adr) {
	SET_FIELD(final_mask, ip4adr); //DM_SET_FIELD_ips(dm.final_mask, ip4adr); //DM_SET_FIELD(ip_t ,mask, ip4adr ? ip4adr : candidate_eth_mask());
	return GET_FIELD(final_mask); //DM_GET_FIELD_ips(dm.final_mask); //return DM_GET_FIELD(ip_t, mask);
}

uint8_t *identified_tcpip_ip(void) {
	return GET_FIELD(final_ip); //DM_GET_FIELD_ips(dm.final_ip); //return DM_GET_FIELD(ip_t, ip);
}
uint8_t *identified_tcpip_gw(void) {
	return GET_FIELD(final_gw); //DM_GET_FIELD_ips(dm.final_gw); //return DM_GET_FIELD(ip_t, gw);
}
uint8_t *identified_tcpip_mask(void) {
	return GET_FIELD(final_mask); //DM_GET_FIELD_ips(dm.final_mask); //return DM_GET_FIELD(ip_t, mask);
}

int env_init_setup(uint16_t *id)
{
	uint8_t rev = cspi_read_reg(0x5c);
	*id = cspi_read_chip_id();
	if (*id == 0x9000)
		*id = 0x9051;

	//rev = cspi_read_reg(0x5c);

	if (*id != 0x9051) {
		printf("DM9051 not found, chipid: %04x\r\n", *id);
		printf("system stop\r\n");
		while(1);
	}
	printf("\r\n"); //printk("\r\n");
	printf("DM9051 chip rev: %02x\r\n", rev);
	printf("DM9051 found: %04x\r\n", *id);
	return 1;
}

/*
 * cboard_env_func
 */
#define	TIMES_TO_RST	10

static void dm9051_show_rxbstatistic(uint8_t *htc, int n)
{
	int i, j;
	
	//.dm9051_chipid(); //Also [test][test][test].init
	printf("SHW rxbStatistic, 254 wrngs\r\n");
	for (i = 0 ; i < (n+2); i++) {
		if (!(i%32) && i) printf("\r\n");
		if (!(i%32) || !(i%16)) printf("%02x:", i);
		//if (!(i%16)) printf(" ");
		if (!(i%8)) printf(" ");
		if (i==0 || i==1) {
			printf("  ");
			continue;
		}
		j = i - 2;
		printf("%d ", htc[j]);
	}
	printf("\r\n");
}

static uint8_t ret_fire_time(uint8_t *histc, int csize, int i, uint8_t rxb) {
	printf(" _dm9051f rxb %02x (times %2d)%c\r\n", rxb, histc[i], (histc[i]==2) ? '*': ' ');
	if (histc[i] >= TIMES_TO_RST) {
		dm9051_show_rxbstatistic(histc, csize);
		histc[i] = 1;
		return TIMES_TO_RST;
	}
	return 0;
}

uint16_t env_evaluate_rxb(uint8_t rxb)
{
	int i;
	static uint8_t histc[254] = { 0 }; //static int rff_c = 0 ...;
	uint8_t times = 1;
	
	for (i = 0 ; i < sizeof(histc); i++) {
		if (rxb == (i+2)) {
			histc[i]++;
			times = ret_fire_time(histc, sizeof(histc), i, rxb);
			if (times == 0) //As: Hdlr (times : 0 or TIMES_TO_RST)
				return 0;
			return env_err_rsthdlr("_dm9051f rxb error accumunation times : %u\r\n", times); 
		}
	}
	return env_err_rsthdlr("dm9 impossible path error times : %u\r\n", times); //As: Hdlr (times : 1)
}

const uint8_t *env_reset_process(const uint8_t *macaddr, enable_t en)
{
	cspi_core_reset(); //As: printf("rstc %d ,because rxb %02x (is %d times)\r\n", rstc, rxbyte, times);
	
	if (en) {
		macaddr = cspi_dm_start1(macaddr);
	}
	return macaddr;
}

uint16_t env_err_rsthdlr(char *err_explain_str, uint32_t valuecode) //,int ret_cond //u8 zerochk
{
	printf(err_explain_str, valuecode); //or "0x%02x"
	env_reset_process(identified_eth_mac(), DM_TRUE);
	return 0;
}

/*
 * cboard_print_hex
 */

#define kkmin(a, b) (a < b) ? a : b
			
static void printf_space(int n) {
	while (n--)
		printf("%c", ' ');
}

static int printf_space_init(size_t tlen) {
	char cspace[16];
	int n = sprintf(cspace, "rxlen %4d", tlen);
#if 1
	printf_space(n);
	printf(" %s\r\n", cspace);
#endif
	return n;
}

static int printf_rxlen_head(size_t tlen, int nspc) {
#if 1
			if (!nspc)
				nspc = printf_space_init(tlen);

			printf_space(nspc);
			return nspc;
#endif
}

void sprint_hex_dump0(int head_space, int titledn, char *prefix_str,
		    size_t tlen, int rowsize, const void *buf, int seg_start, size_t len, /*int useflg*/ int cast_lf) //, int must, int dgroup
{
//#undef printf
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	//if (useflg) {
		int si, se, titlec = 0;
		int i, linelen, remaining = len; //hs, const eth_class_t *ec = &eclass[10];
		int nspace = 0;
		
		(void) head_space;
		//(void) tlen;

		si = seg_start;
		se = seg_start + len;
		for (i = si; i < se; i += rowsize) {
			//unsigned
			char linebuf[(12 * 3) + (3 * 16) + 1 + 32]; //here!
			
			nspace = printf_rxlen_head(tlen, nspace);

			linelen = kkmin(remaining, rowsize);
			remaining -= rowsize;
			do {
				const uint8_t *ptr = buf;
				int j;
				//unsigned char *
				int nb = 0;
				for (j=0; j< linelen && (size_t) nb < sizeof(linebuf); j++) {
					if (j && !(j%8))
						nb += snprintf(linebuf+nb, sizeof(linebuf)-nb, " ");
					if (((rowsize>>1)!=8) && !(j%(rowsize>>1)))
						nb += snprintf(linebuf+nb, sizeof(linebuf)-nb, " ");

					nb += snprintf(linebuf+nb, sizeof(linebuf)-nb, "%02x ", *(ptr + i + j));
				}
			} while(0);

			//.nspace = printf_rxlen_head(tlen, nspace);

#if 0
			hs = head_space;
			while(hs--)
#endif
				printf(" ");

			if (prefix_str) {
				printf("(%s) %.3x %s", prefix_str, i, linebuf); //"%s", ec->str //CHECK (XXX >> )
				while (titledn) {
					titledn--;
					prefix_str[titlec++] = ' ';
				}
			} else {
				printf("(dm9 xfer) %.3x %s", i, linebuf); //"%s", ec->str
			}

			if ((i+rowsize) < se)
				printf("\r\n");
			else {
				if (cast_lf)
					printf("\r\n");
#if 0
				if (IS_UDP) {
					//ptr
					#if 0
					size_t ulen = tlen; // larger for with 4-bytes CRC
					ulen = UIP_LLH_LEN;
					ulen += HTONS(UDPBUF->udplen) - 8;
					ulen += sizeof(struct uip_udpip_hdr); // correct for without 4-bytes CRC (htons)

					if (cast_lf)
						printf("\r\n");

					printf(" ..SrcIP %d.%d.%d.%d", (IPBUF->srcipaddr[0] >> 0) & 0xff, (IPBUF->srcipaddr[0] >> 8),
						(IPBUF->srcipaddr[1] >> 0) & 0xff, (IPBUF->srcipaddr[1] >> 8));
					printf("  DestIP %d.%d.%d.%d", (IPBUF->destipaddr[0] >> 0) & 0xff, (IPBUF->destipaddr[0] >> 8),
						(IPBUF->destipaddr[1] >> 0) & 0xff, (IPBUF->destipaddr[1] >> 8));
					printf("  Len %d", ulen);
					printf("  (%5d -> %d Len %d)", UDPBUF->srcport, UDPBUF->destport, HTONS(UDPBUF->udplen) - 8);
					printf("\r\n");
					#endif
				}
				if (IS_TCP) {
					size_t ulen = tlen; // larger for with 4-bytes CRC
					printf(" ..SrcIP %d.%d.%d.%d", (IPBUF->srcipaddr[0] >> 0) & 0xff, (IPBUF->srcipaddr[0] >> 8),
						(IPBUF->srcipaddr[1] >> 0) & 0xff, (IPBUF->srcipaddr[1] >> 8));
					printf("  DestIP %d.%d.%d.%d", (IPBUF->destipaddr[0] >> 0) & 0xff, (IPBUF->destipaddr[0] >> 8),
						(IPBUF->destipaddr[1] >> 0) & 0xff, (IPBUF->destipaddr[1] >> 8));
					printf("  Len %d", ulen);
					
					if (TCPBUF->flags == 0x18)
						printf("  (%5d -> %d) flags %02x (PSH, ACK)", HTONS(TCPBUF->srcport), HTONS(TCPBUF->destport), TCPBUF->flags);
					else
						printf("  (%5d -> %d) flags %02x", HTONS(TCPBUF->srcport), HTONS(TCPBUF->destport), TCPBUF->flags);
					
					printf("\r\n");
				}
#endif
				//else
				//	printf("\r\n");
			}
		}
	//}
//#undef printf
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}
