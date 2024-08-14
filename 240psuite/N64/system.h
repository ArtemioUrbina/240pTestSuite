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

#endif