/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Simon Goldschmidt
 *
 */
#ifndef LWIP_HDR_LWIPOPTS_H
#define LWIP_HDR_LWIPOPTS_H

/* main controls */
#define RX_HANDLER_DIS					0
#define RX_HANDLER_FLG					1 //0

/* httpd control macros, httpd.c */

#define JOS_MUTEX_INIT					1

#define NET_TASK_PRIO           		2 //FOR 'net_task'
#define NET_TASK_DLY           			1 //3
	#define JOS_MUTEX_LO_INIT			0 //1 //1 //1 //?
	#define JOS_MUTEX_LINK				0 //1 //0 //1 //1
	#define JOS_MUTEX_RX				0 //1 //1 //.0 //.1 //0
	
#define TCPIP_THREAD_PRIO       		2 //3 //FOR 'tcpip_thread'
#define	TCPIP_THREAD_DLY				1 //3 //1
#define JOS_PRINTF_TX_MUST1				1
	#define JOS_MUTEX_TX				0 //1 //1 //.1 //0 (to inside _net_task)
	#define LWIP_TCPIP_CORE_LOCKING     0 //1 //0

#define JOS_MUTEX_CODE					1 //.0

#define HTTPD_INIT_CODE 				1 //1
#define HTTPD_CALL_FS					1 //0

/* httpd control macros, httpd.c */

#define LWIP_HTTPD_CUSTOM_FILES       	0 /* artery 0, */  /* x2web 0, also could be defined in "httpd_opts.h" */ //0

/* Note:
 *
 *		x2web 1
 *		HTTPD_USE_CUSTOM_FSDATA				1
 *		-- with ../lwip_web2403v2_freelw/httpd.c
 *		-- with ../lwip_web2403v2_freelw/fs_jjdynamic.c
 *
 *		artery 0
 *		HTTPD_USE_CUSTOM_FSDATA				0
 *		-- with app/httpd/httpd.c
 *		-- with app/httpd/fs.c
 */
#if 1
//#define HTTPD_JJ_DYNAMIC_FSDATA			1  /* x2web 1 */ //1
#define HTTPD_USE_CUSTOM_FSDATA				1  //0 //1  /* x2web 1, used in "httpd_opts.h" */ //0 //1
#else
//#define HTTPD_USE_CUSTOM_FSDATA_FS		0
#define HTTPD_USE_CUSTOM_FSDATA				0
#endif

//#define LWIP_TESTMODE                   	0

#define LWIP_IPV4                         	1
//#define LWIP_TIMERS                     	0

//#define LWIP_CHECKSUM_ON_COPY           	1
//#define TCP_CHECKSUM_ON_COPY_SANITY_CHECK	1
//#define TCP_CHECKSUM_ON_COPY_SANITY_CHECK_FAIL(printfmsg) LWIP_ASSERT("TCP_CHECKSUM_ON_COPY_SANITY_CHECK_FAIL", 0)

/* We link to special sys_arch.c (for basic non-waiting API layers unit tests) */
#define NO_SYS                           0
#define SYS_LIGHTWEIGHT_PROT             1
#define LWIP_NETCONN                     !NO_SYS
#define LWIP_SOCKET                      !NO_SYS
#define LWIP_NETCONN_FULLDUPLEX          LWIP_SOCKET
#define LWIP_NETBUF_RECVINFO             1
#define LWIP_HAVE_LOOPIF                 1
//#define TCPIP_THREAD_TEST

/* Enable DHCP to test it, disable UDP checksum to easier inject packets */
#define LWIP_DHCP                        0

/* Minimal changes to opt.h required for tcp unit tests: */
/* TCP Maximum segment size. */

#define TCP_MSS                          (1500 - 40)	  /* TCP_MSS = (Ethernet MTU - IP header size - TCP header size) */
/* MEM_ALIGNMENT: should be set to the alignment of the CPU for which
   lwIP is compiled. 4 byte alignment -> define MEM_ALIGNMENT to 4, 2
   byte alignment -> define MEM_ALIGNMENT to 2. */
#define MEM_ALIGNMENT           4
#define MEM_SIZE                         (20*1024)
#define TCP_SND_QUEUELEN                 (6 * TCP_SND_BUF)/TCP_MSS
#define MEMP_NUM_TCP_SEG                 TCP_SND_QUEUELEN
#define TCP_SND_BUF                      (2 * TCP_MSS)
#define TCP_WND                          (2 * TCP_MSS)
#define LWIP_WND_SCALE                   0
#define TCP_RCV_SCALE                    0
#define PBUF_POOL_SIZE                   10 /* pbuf tests need ~200KByte */
/* PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. */
#define PBUF_POOL_BUFSIZE                2000
//#define TCP_QUEUE_OOSEQ         0

/* ---------- TCP options ---------- */
#define LWIP_TCP                         1
#define TCP_TTL                          255

/* Enable IGMP and MDNS for MDNS tests */
#define LWIP_IGMP                        1
#define LWIP_MDNS_RESPONDER              1
#define LWIP_NUM_NETIF_CLIENT_DATA       (LWIP_MDNS_RESPONDER)

/* Minimal changes to opt.h required for etharp unit tests: */
#define ETHARP_SUPPORT_STATIC_ENTRIES    1


/* LwIP 2.1.2 */
//#define MEMP_NUM_SYS_TIMEOUT            (LWIP_NUM_SYS_TIMEOUT_INTERNAL + 8)
#define MEMP_NUM_SYS_TIMEOUT            10

/* MIB2 stats are required to check IPv4 reassembly results */
//#define MIB2_STATS                       1
#define MIB2_STATS                       0

/* netif tests want to test this, so enable: */
//#define LWIP_NETIF_EXT_STATUS_CALLBACK  1

/* Check lwip_stats.mem.illegal instead of asserting */
#define LWIP_MEM_ILLEGAL_FREE(msg)       /* to nothing */

#define LWIP_NETIF_LINK_CALLBACK        1
#define LWIP_UDP                        1
#define TCPIP_MBOX_SIZE                 20
#define DEFAULT_TCP_RECVMBOX_SIZE       20
#define DEFAULT_ACCEPTMBOX_SIZE         20
#define LWIP_TCPIP_TIMEOUT              1
#define LWIP_SO_RCVTIMEO                 1

/**
 * MEMP_NUM_TCP_PCB: the number of simultaneously active TCP connections.
 * (requires the LWIP_TCP option)
 */
#define MEMP_NUM_TCP_PCB                 20

/**
 * MEMP_NUM_PBUF: the number of memp struct pbufs (used for PBUF_ROM and PBUF_REF).
 * If the application sends a lot of data out of ROM (or other static memory),
 * this should be set high.
 */
#define MEMP_NUM_PBUF                    20

//.JJ //#define CHECKSUM_BY_HARDWARE 
#ifdef CHECKSUM_BY_HARDWARE
  /* CHECKSUM_GEN_IP==0: Generate checksums by hardware for outgoing IP packets.*/
  #define CHECKSUM_GEN_IP                 0
  /* CHECKSUM_GEN_UDP==0: Generate checksums by hardware for outgoing UDP packets.*/
  #define CHECKSUM_GEN_UDP                0
  /* CHECKSUM_GEN_TCP==0: Generate checksums by hardware for outgoing TCP packets.*/
  #define CHECKSUM_GEN_TCP                0 
  /* CHECKSUM_CHECK_IP==0: Check checksums by hardware for incoming IP packets.*/
  #define CHECKSUM_CHECK_IP               0
  /* CHECKSUM_CHECK_UDP==0: Check checksums by hardware for incoming UDP packets.*/
  #define CHECKSUM_CHECK_UDP              0
  /* CHECKSUM_CHECK_TCP==0: Check checksums by hardware for incoming TCP packets.*/
  #define CHECKSUM_CHECK_TCP              0
  /* CHECKSUM_CHECK_ICMP==0: Check checksums by hardware for incoming ICMP packets.*/
  #define CHECKSUM_GEN_ICMP               0
#else
  /* CHECKSUM_GEN_IP==1: Generate checksums in software for outgoing IP packets.*/
  #define CHECKSUM_GEN_IP                 1
  /* CHECKSUM_GEN_UDP==1: Generate checksums in software for outgoing UDP packets.*/
  #define CHECKSUM_GEN_UDP                1
  /* CHECKSUM_GEN_TCP==1: Generate checksums in software for outgoing TCP packets.*/
  #define CHECKSUM_GEN_TCP                1
  /* CHECKSUM_CHECK_IP==1: Check checksums in software for incoming IP packets.*/
  #define CHECKSUM_CHECK_IP               1
  /* CHECKSUM_CHECK_UDP==1: Check checksums in software for incoming UDP packets.*/
  #define CHECKSUM_CHECK_UDP              1
  /* CHECKSUM_CHECK_TCP==1: Check checksums in software for incoming TCP packets.*/
  #define CHECKSUM_CHECK_TCP              1
  /* CHECKSUM_CHECK_ICMP==1: Check checksums by hardware for incoming ICMP packets.*/
  #define CHECKSUM_GEN_ICMP               1
#endif

#endif /* LWIP_HDR_LWIPOPTS_H */
