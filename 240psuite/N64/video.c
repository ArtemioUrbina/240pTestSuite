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

// vblank callback so we can wait for vsync
void vblCallback(void)
{
    __frames++;
}

#ifndef DEBUG_BENCHMARK
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
#else
static uint64_t __frameStart = 0;
static uint64_t __idleStart = 0;
float __frameIdle = 0;
float __frameLen = 0;

void drawFrameLens()
{
	char str[100];
	
	sprintf(str, "Frame: %0.2fms Idle: %0.2fms", __frameLen, __frameIdle);
	drawStringB(80, 4, __frameLen > 16.715 ? 0xff : 0x00, __frameLen < 16.715 ? 0xff : 0x00, 0x00, str);
}

void getDisplay()
{	
	__dc = display_get();
	__frameStart = get_ticks_us();
}

void waitVsync()
{
	uint64_t nextFrame = __frames + 1, end = 0;
	
	if(__dc)
	{
		drawFrameLens();
		display_show(__dc);
		__dc = NULL;
	}
	
	__idleStart = get_ticks_us();
	while (nextFrame > __frames) ;
	end = get_ticks_us();
	__frameLen = (end - __frameStart)/1000.0f;
	__frameIdle = (end - __idleStart)/1000.0f;
}
#endif


