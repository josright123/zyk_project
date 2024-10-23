/* identify_opts.h
 * 
 * Top config
 * Middle settings
 * Lower subroutines
 */
#ifndef __IDENTIFY_OPTS_H__
#define	__IDENTIFY_OPTS_H__

#define ap_print					1 //section
#define rt_print					0 //section
#define drv_print					0 //1 //section

//#define qprint					1 //1 //total	// print-out // on/off
#define qprint 						(ap_print | rt_print | drv_print)

#define IDENTIFY_PRINTF_IRQ_STATE	1 //0

#endif //__IDENTIFY_OPTS_H__
