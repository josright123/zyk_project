//
// dm9051opts.c
//
#include "dm9051opts.h"
#include "dm9051_lw.h"

#if 0
u8 gfirst_log[ETHERNET_COUNT];

void first_log_init(void)
{
	int i;
	for (i = 0; i < ETHERNET_COUNT; i++)
		gfirst_log[i] = 1; //Can know the first time reset the dm9051 chip!
}

u8 first_log_get(int i)
{
	u8 if_log = 
		gfirst_log[i];
	gfirst_log[i] = 0; //first_log_clear(i);
	return if_log;
}
#endif
