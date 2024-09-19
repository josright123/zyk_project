#include "dm9051opts.h"
#include "dm9051_lw.h"
#include "dm9051_lw_cboard.h"
#include "dm9051_lw_cspi.h"
#include "dm9051_lw_debug.h"
//#include "../freertos_tasks_debug.h"
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))

void dm9051_link_to_hexdump(const void *buffer, size_t len) {
	if (dm9051_link_log_reset_hexdump(buffer, len)) 
		rx_pointers_isr_show("link_to_dump");
}
