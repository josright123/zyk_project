/*
 * RttPrintf.c
 *
 *  Created on: Jul 10, 2024
 *      Author: jack
 */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "SEGGER_RTT.h"
#include "sl_hal_sysrtc.h"
#include "RttPrint.h"
#define LOG_ZYK "[Zyk ]"
#define ZYK_LOG_MESSAGE_MAX_SIZE 64
static size_t Zyk_AddTimeStampAndPrefixStr(char * logBuffer, const char * prefix, size_t maxSize);

int RttPrintf(const char *format, ...)
{
    char print_buffer[ZYK_LOG_MESSAGE_MAX_SIZE];
    va_list args;
    va_start(args, format);
    size_t prefixLen = Zyk_AddTimeStampAndPrefixStr(print_buffer, LOG_ZYK, ZYK_LOG_MESSAGE_MAX_SIZE);
    size_t len       = vsnprintf(print_buffer + prefixLen, sizeof print_buffer - prefixLen, format, args);
    if (len >= sizeof print_buffer - prefixLen)
    {
        print_buffer[sizeof print_buffer - 1] = '\0';
    }

    va_end(args);
    size_t sz;
    sz = strlen(print_buffer);
    SEGGER_RTT_WriteNoLock(0, print_buffer, sz);

    const char * newline = "\r\n";
    sz                   = strlen(newline);
    SEGGER_RTT_WriteNoLock(0, newline, sz);
    return true;
}

static size_t Zyk_AddTimeStampAndPrefixStr(char * logBuffer, const char * prefix, size_t maxSize)
{
    uint64_t bootTime     = sl_hal_sysrtc_get_counter();
    uint16_t milliseconds = bootTime % 1000;
    uint32_t totalSeconds = bootTime / 1000;
    uint8_t seconds       = totalSeconds % 60;
    totalSeconds /= 60;
    uint8_t minutes = totalSeconds % 60;
    uint32_t hours  = totalSeconds / 60;
    return snprintf(logBuffer, maxSize, "[%02lu:%02u:%02u.%03u]%s", hours, minutes, seconds, milliseconds, prefix);
}
