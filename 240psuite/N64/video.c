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

#include "video.h"
#include "utils.h"

int current_resolution = 0;
int current_bitdepth = 0;
int current_buffers = 0;
int current_gamma = 0;
int current_antialias = 0;

volatile display_context_t __dc = 0;
int dW = 0;
int dH = 0;
int bD = 0;

int video_set = 0;
unsigned char *__screen_buffer = NULL;

// vblank callback so we can wait for vsync

/*
volatile int __frames;

void vblCallback(void)
{
    __frames++;
}
*/

void init_video()
{
	current_resolution = RESOLUTION_320x240;
	current_bitdepth = DEPTH_16_BPP;
	current_buffers = 2;
	current_gamma = GAMMA_NONE;
	current_antialias = ANTIALIAS_RESAMPLE; // ANTIALIAS_OFF doesn't work in 16BPP
	__screen_buffer = NULL;
	video_set = 0;
}

void set_video()
{
	if(video_set)
	{
		//unregister_VI_handler(vblCallback);
		FreeScreenBuffer();
		rdp_close();
		display_close();
		video_set = 0;
	}
	
	video_set = 0;
	display_init(current_resolution, current_bitdepth, current_buffers, current_gamma, current_antialias);
	//register_VI_handler(vblCallback);
	rdp_init();
	
	switch(current_resolution)
	{
		case RESOLUTION_320x240:
			dW = 320;
			dH = 240;
			break;
		case RESOLUTION_640x480:
			dW = 640;
			dH = 480;
			break;
		case RESOLUTION_256x240:
			dW = 256;
			dH = 240;
			break;
		case RESOLUTION_512x480:
			dW = 512;
			dH = 480;
			break;
	}
	
	if(current_bitdepth == DEPTH_16_BPP)
		bD = 2;
		
	if(current_bitdepth == DEPTH_32_BPP)
		bD = 4;
		
	video_set = 1;
	ClearScreen();
	CreateScreenBuffer();
}

void GetDisplay()
{	
	do
	{	__dc = display_lock();}
	while(!__dc);
}

int isNTSC()
{
	int tv; 
	
	tv = *(int *) 0x80000300;
	return tv;
}

void WaitVsync()
{
	//int nextframe;
	
	//nextframe = __frames + 1;
	display_show(__dc);
	//while (nextframe > __frames) ;
}

void CreateScreenBuffer()
{	
	FreeScreenBuffer();
		
#ifdef USE_N64MEM		
	__screen_buffer = n64_malloc(dW*dH*bD);
#else
	__screen_buffer = malloc(dW*dH*bD);
#endif

	if(__screen_buffer)
#ifdef USE_N64MEM		
		n64_memset(__screen_buffer, 0, dW*dH*bD);
#else
		memset(__screen_buffer, 0, dW*dH*bD);
#endif
}

void FreeScreenBuffer()
{
	if(__screen_buffer)
	{
#ifdef USE_N64MEM	
		n64_free(__screen_buffer);
#else
		free(__screen_buffer);
#endif
		__screen_buffer = NULL;
	}
}


void CopyScreen()
{	
	if(!__screen_buffer || !__dc)
		return;

#ifdef USE_N64MEM
    n64_memcpy(__screen_buffer, __safe_buffer[(__dc)-1], dW*dH*bD);
#else
	memcpy(__screen_buffer, __safe_buffer[(__dc)-1], dW*dH*bD);
#endif
}

void FillScreenFromBuffer()
{
	if(!__screen_buffer || !__dc)
		return;

#ifdef USE_N64MEM
    n64_memcpy(__safe_buffer[(__dc)-1], __screen_buffer, dW*dH*bD);
#else
	memcpy(__safe_buffer[(__dc)-1], __screen_buffer, dW*dH*bD);
#endif
}


