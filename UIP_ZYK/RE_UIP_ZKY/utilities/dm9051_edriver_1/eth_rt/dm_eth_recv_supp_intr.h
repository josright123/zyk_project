//#include "identify/dm9051opts.h"

//static int net_pkts_handle_intr(struct netif *netif) {
//	int n = 0;

//#if ETHERNET_INTERRUPT_MODE

//	while (ethernetif_input_intr(netif) == ERR_OK)
//		n++;
//#endif

//	return n;
//}

//void net_inp_interrupt(void)
//{
//#if ETHERNET_INTERRUPT_MODE
//	struct netif *netif = tcpip_stack_netif();

//	//if (isrs & ISR_PR) {
//	isrSemaphore_n = net_pkts_handle_intr(netif);
//	//}

//#endif
//}
