/*
 * RttPrint.h
 *
 *  Created on: Jul 10, 2024
 *      Author: jack
 */

#ifndef ZYK_MIDDLEWARE_ZYKAPP_RTTPRINT_H_
#define ZYK_MIDDLEWARE_ZYKAPP_RTTPRINT_H_

#ifdef __cplusplus
extern "C" {
#endif
int RttPrintf(const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#define printf RttPrintf
#define Zyk_printf RttPrintf

#endif /* ZYK_MIDDLEWARE_ZYKAPP_RTTPRINT_H_ */
