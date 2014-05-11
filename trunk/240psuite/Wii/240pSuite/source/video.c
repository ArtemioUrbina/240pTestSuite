/* 
 * 240p Test Suite
 * Copyright (C)2014 Artemio Urbina (Wii GX)
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 
#include <stdio.h>
#include "video.h"
#include "options.h"

GXRModeObj *vmodes[TOTAL_VMODES] = {
	&TVNtsc240Ds,
	&TVNtsc480IntDf, 		// used to beTVNtsc480Int	
	&TVNtsc480IntDf, 
	&TVNtsc480Prog,
	&TVNtsc480Prog
};

u32	ActiveFB = 0;
void *frameBuffer[TOTAL_VMODES][2];
u32 vmode           = VIDEO_240P;
GXRModeObj *rmode   = NULL;
int VideoInit       = 0;
int W			    = 320;
int H			    = 240;
int dW			    = 320;
int dH			    = 240;

void SetVideoMode(u32 newmode)
{
	if(newmode >= VIDEO_480P && !VIDEO_HaveComponentCable())
		newmode = VIDEO_240P;		
		
	if(newmode >= TOTAL_VMODES || newmode < 0)
		newmode = VIDEO_240P;	

	if(newmode >= VIDEO_480P && !Options.Activate480p )
		newmode = VIDEO_240P;
		
	vmode = newmode;
	
	rmode = vmodes[vmode];
	
	switch(vmode)
	{
		case VIDEO_240P:
		case VIDEO_480I_A240:
		case VIDEO_480P_SL:
			dW = 320;
			dH = 240;
			break;
		case VIDEO_480I:
		case VIDEO_480P:
			dW = 640;
			dH = 480;
			break;
	}

	if(!VideoInit)
	{
		InitFrameBuffers();		
		VIDEO_SetBlack(FALSE);
	}
		
	VIDEO_ClearFrameBuffer(rmode, frameBuffer[vmode][ActiveFB], 0);
	VIDEO_Configure(rmode);		
	VIDEO_SetNextFramebuffer(frameBuffer[vmode][ActiveFB]);		
	VIDEO_Flush();

	VIDEO_WaitVSync();
	/*
	if(vmode == VIDEO_480I_A240 || vmode == VIDEO_480I) 
		VIDEO_WaitVSync();
		*/
}

void InitFrameBuffers()
{
	int mode = 0;
	
	for(mode = 0; mode < TOTAL_VMODES; mode++)		
	{
		frameBuffer[mode][0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(vmodes[mode]));	
		VIDEO_ClearFrameBuffer(vmodes[mode], frameBuffer[mode][0], 0);		
		frameBuffer[mode][1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(vmodes[mode]));			
		VIDEO_ClearFrameBuffer(vmodes[mode], frameBuffer[mode][1], 0);		
	}

	VideoInit = 1;
}

