#include "lwip/ip.h"
#include "lwip/udp.h"
//#include "lwip/tcp.h"
#include "lwip/prot/tcp.h"
#include "lwip/prot/ethernet.h"
#include "dm9051opts.h"

int impl_clear_checksum_field(uint8_t *buf, uint16_t len)
{
	//.
	struct eth_hdr *ethhdr = (struct eth_hdr *) buf;
	uint16_t type = ethhdr->type;
		
	if (type == PP_HTONS(ETHTYPE_IP)) {
		struct ip_hdr *iphdr = (struct ip_hdr *) (buf + 14);
		struct udp_hdr *udphdr;
		struct tcp_hdr *tcphdr;
		
		switch (IPH_PROTO(iphdr)) {
		  case IP_PROTO_UDP:
			memset(buf, 0xff, 6);
			buf[6] = 0;
			buf[7] = 0;
			buf[8] = 0;
			buf[9] = 0;
			buf[10] = 0;
			buf[11] = 0x22;

			IPH_CHKSUM(iphdr) = 0x1111;
			//MIB2_STATS_INC(mib2.ipindelivers);
			//udp_input(p, inp);
			udphdr = (struct udp_hdr *) (buf + 14 + 20);
			//udphdr->chksum = 0xffff;
			udphdr->chksum = 0x2222;
			return 1;
			//break;
		  case IP_PROTO_TCP:
			memset(buf, 0xff, 6);
			buf[6] = 0;
			buf[7] = 0;
			buf[8] = 0;
			buf[9] = 0;
			buf[10] = 0;
			buf[11] = 0x33;

			IPH_CHKSUM(iphdr) = 0x1111;
			//MIB2_STATS_INC(mib2.ipindelivers);
			//tcp_input(p, inp);
			tcphdr = (struct tcp_hdr *) (buf + 14 + 20);
			tcphdr->chksum = 0xffff; //0x3333;
			return 1;
			//break;
		}
	}
	return 0;
}
