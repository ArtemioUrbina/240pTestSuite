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
#include "menu.h"

int current_resolution = 0;
int current_bitdepth = 0;
int current_buffers = 0;
int current_gamma = 0;
int current_antialias = 0;
int useNTSC = 1;
int EnablePAL = 0;
int EnableDivot = 0;
int EnableDither = 0;

volatile display_context_t __dc = 0;
int dW = 0;
int dH = 0;
int bD = 0;

int video_set = 0;
unsigned char *__screen_buffer = NULL;

// vblank callback so we can wait for vsync

volatile int __frames;

void vblCallback(void)
{
    __frames++;
}

int GetFrameCount()
{
	return __frames;
}

void reset_video_vars()
{
	current_resolution = RESOLUTION_320x240;
	current_bitdepth = DEPTH_16_BPP;
	current_buffers = 2;
	current_gamma = GAMMA_NONE;
	current_antialias = ANTIALIAS_RESAMPLE; // ANTIALIAS_OFF doesn't work in 16BPP
	EnablePAL = 0;
	EnableDivot = 1;
	EnableDither = 0;
	useNTSC = 1;
}

void reset_video()
{
	reset_video_vars();
	set_video();
	
	// Better to clean up
	ChangedVideoFormat = 1;
	ChangedResolution = 1;
}

void init_video()
{
	__screen_buffer = NULL;

	reset_video_vars();
	video_set = 0;
}

void set_video()
{
	if(video_set)
	{
		FreeScreenBuffer();
		
		rdp_close();
		unregister_VI_handler(vblCallback);
		display_close();
	}
	
	video_set = 0;
	
	display_init_ex(useNTSC, current_resolution, current_bitdepth, current_buffers, current_gamma, current_antialias);
	register_VI_handler(vblCallback);
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
	
	GetDisplay();
	WaitVsync();
	
	CreateScreenBuffer();
}

void GetDisplay()
{	
	do
	{	__dc = display_lock();}
	while(!__dc);
	
#ifdef DEBUG_BENCHMARK
	start_counter();
#endif
	ClearScreen();
}

int isNTSC()
{
	int tv; 
	
	tv = *(int *) 0x80000300;
	return tv;
}

void WaitVsync()
{
	int nextframe;
	
#ifdef DEBUG_BENCHMARK
	Draw_counter(10, 10, end_counter());
#endif
	nextframe = __frames + 1;
	display_show(__dc);
	while (nextframe > __frames) ;
}

void GetVideoModeStr(char *res, int shortdesc)
{
	if(!shortdesc)
	{
		switch(current_resolution)
		{
			case RESOLUTION_320x240:
				sprintf(res, "Video: 240p");				
				break;		
			case RESOLUTION_640x480:
				sprintf(res, "Video: 480i (Scaling disabled)");
				break;
			case RESOLUTION_256x240:
				sprintf(res, "Video: 256x240");
				break;
			case RESOLUTION_512x480:
				sprintf(res, "Video: 512x480");
				break;
		}
	}
	else
	{
		switch(current_resolution)
		{
			case RESOLUTION_320x240:
				sprintf(res, "[240p]");				
				break;
			case RESOLUTION_640x480:
				sprintf(res, "[480i]");
				break;
			case RESOLUTION_256x240:
				sprintf(res, "[256]");
				break;
			case RESOLUTION_512x480:
				sprintf(res, "[512]");
				break;
		}

	}
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

void DarkenScreenBuffer(int amount)
{
	if(!__screen_buffer || !__dc)
		return;

	if(bD == 2)
	{
		uint16_t *screen = (uint16_t *)__screen_buffer;
		for(int y = 0; y < dH; y++)
		{
			for(int x = 0; x < dW; x++)
			{
				uint16_t cur_color = screen[x + (y * dW)];

				/* Get current color */
				uint32_t cr = ((cur_color >> 11) & 0x1F);
				uint32_t cg = ((cur_color >> 6) & 0x1F);
				uint32_t cb = ((cur_color >> 1) & 0x1F);
				cr = (cr << 3) | (cr >> 2);
				cg = (cg << 3) | (cg >> 2);
				cb = (cb << 3) | (cb >> 2);
				
				if(cr > amount)
					cr -= amount;
				else
					cr = 0;
				if(cg > amount)
					cg -= amount;
				else
					cg = 0;
				if(cb > amount)
					cb -= amount;
				else
					cb = 0;

				screen[x + (y * dW)] = (uint16_t)graphics_make_color(cr, cg, cb, 255);
			}
		}
	}
	
	if(bD == 4)
	{
		uint32_t *screen = (uint32_t *)__screen_buffer;
		for(int y = 0; y < dH; y++)
		{
			for(int x = 0; x < dW; x++)
			{
				uint32_t cur_color = screen[x + (y * dW)];

				/* Get current color */
				uint32_t cr = (cur_color >> 24) & 0xFF;
				uint32_t cg = (cur_color >> 16) & 0xFF;
				uint32_t cb = (cur_color >> 8) & 0xFF;
				
				if(cr > amount)
					cr -= amount;
				else
					cr = 0;
				if(cg > amount)
					cg -= amount;
				else
					cg = 0;
				if(cb > amount)
					cb -= amount;
				else
					cb = 0;

				screen[x + (y * dW)] = (uint32_t)graphics_make_color(cr, cg, cb, 255);
			}
		}
	}
}


