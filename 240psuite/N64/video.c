/* 
 * 240p Test Suite for Nintendo 64
 * Copyright (C)2016 Artemio Urbina
 *
 * This file is part of the 240p Test Suite
 *
 * The 240p Test Suite is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The 240p Test Suite is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	02111-1307	USA
 */

#include "system.h"
#include "video.h"

#ifdef DEBUG_BENCHMARK
#include "font.h"
#endif

unsigned int dW = 320;
unsigned int dH = 240;

volatile display_context_t __dc = NULL;
volatile uint64_t __frames = 0;

#ifndef DEBUG_BENCHMARK
void vblCallback(void)
{
    __frames++;
}

void getDisplay()
{	
	__dc = display_get();
}

void waitVsync()
{
	uint64_t nextFrame = __frames + 1;
	
	if(__dc)
	{
		display_show(__dc);
		__dc = NULL;
	}
	
	while (nextFrame > __frames) ;
}

void resetVideo()
{
}
#else
#define N64_FRAME_LEN 16.715
static uint64_t __frameStart = 0;
static uint64_t __idleStart = 0;
static uint64_t __lastVbl = 0;
float __frameIdle = N64_FRAME_LEN, __minIdle = N64_FRAME_LEN;
float __frameLen = 0;
float __vblLen = 0;

void vblCallback(void)
{
	uint64_t now = get_ticks_us();
	
	__vblLen = (now - __lastVbl)/1000.0f;
	__lastVbl = now;
    __frames++;
}

void drawFrameLens()
{
	char str[100];
	
	if(__frameIdle < __minIdle)
		__minIdle = __frameIdle;
	
	sprintf(str, "ALL:%0.3f IDL:%0.3f/%0.3f VBL:%0.3f", __frameLen, __frameIdle, __minIdle, __vblLen);
	drawStringB(10, 4, __frameIdle < 4.0 ? 0xff : 0x00, __frameIdle > 4.0 ? 0xff : 0x00, 0x00, str);
}

void getDisplay()
{	
	__frameStart = get_ticks_us();
	__dc = display_get();
}

void waitVsync()
{
	uint64_t nextFrame = __frames + 1;
	
	if(__dc)
	{
		drawFrameLens();
		display_show(__dc);
		__dc = NULL;
	}
	
	__idleStart = get_ticks_us();
	while (nextFrame > __frames) ;
	__frameLen = (__lastVbl - __frameStart)/1000.0f;
	__frameIdle = (__lastVbl - __idleStart)/1000.0f;
}

void resetVideo()
{
	__minIdle = N64_FRAME_LEN;
}
#endif


