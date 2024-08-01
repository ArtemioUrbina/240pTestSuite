#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>

#define SUITE_NUM_BUFFERS 2

extern int isPAL;

void initN64();
void drawSysData();
int getUsedRAM();

// These taken from libdragon examples
uint32_t myrand(void);

// RANDN(n): generate a random number from 0 to n-1
#define RANDN(n) ({ \
	__builtin_constant_p((n)) ? \
		(myrand()%(n)) : \
		(uint32_t)(((uint64_t)myrand() * (n)) >> 32); \
})

#endif