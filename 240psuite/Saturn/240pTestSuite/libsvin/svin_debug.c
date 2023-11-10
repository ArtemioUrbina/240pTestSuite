#include <yaul.h>

#include <stdio.h>
#include <stdlib.h>

void 
_svin_debug_dump_to_lwram(uint8_t * buf, int offset, int size)
{
	uint8_t * p = (uint8_t *)0x20200000;
	for (int i =0; i<size; i++)
		p[i+offset] = buf[i];
}
