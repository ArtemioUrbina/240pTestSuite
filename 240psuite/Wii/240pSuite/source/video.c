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

 GXRModeObj Mode_240p;
 GXRModeObj Mode_480i;
 GXRModeObj Mode_264p;
 GXRModeObj Mode_528i;

GXRModeObj *vmodes[TOTAL_VMODES] = {
	&Mode_240p,
	&Mode_480i, 		
	&Mode_480i, 
	&Mode_264p,
	&Mode_528i,
	&Mode_528i,
	&TVNtsc480Prog,
	&TVNtsc480Prog
};

u32	ActiveFB	= 0;
u32 vmode		= INVALID_VIDEO;
u8 	IsPAL		= MODE_NTSC;
void *frameBuffer[2][2];
u8	offsetY		= 0;


GXRModeObj *mvmode	= NULL;
GXRModeObj *rmode   = NULL;
int W			    = 0;
int H			    = 0;
int dW			    = 0;
int dH			    = 0;

u8 VIDEO_HaveSCARTRGBCable()
{
	if(!mvmode)
		return 0;
		
	return(mvmode->viTVMode >> 2 == VI_EURGB60);
}

void InitVideo()
{
	int fb = 0;
	
	VIDEO_Init();
		
	mvmode = VIDEO_GetPreferredMode(NULL);

	Mode_240p = TVNtsc240Ds;
	Mode_480i = TVNtsc480IntDf;
	Mode_264p = TVPal264Ds;
	Mode_528i = TVPal528IntDf;

#ifdef WII_VERSION			
	/* Adjust SCART cable to output RGB and not YUV */	
	
	Mode_240p.viTVMode = VI_TVMODE_EURGB60_DS;
	Mode_480i.viTVMode = VI_TVMODE_EURGB60_INT;
	Mode_264p.viTVMode = VI_TVMODE_EURGB60_DS;
	Mode_528i.viTVMode = VI_TVMODE_EURGB60_INT;
	
	if(!(mvmode->viTVMode >> 2))
	{
		Mode_240p.viTVMode = VI_TVMODE(mvmode->viTVMode >> 2, VI_NON_INTERLACE);
		Mode_480i.viTVMode = VI_TVMODE(mvmode->viTVMode >> 2, VI_INTERLACE);
		Mode_264p.viTVMode = VI_TVMODE(mvmode->viTVMode >> 2, VI_NON_INTERLACE);
		Mode_528i.viTVMode = VI_TVMODE(mvmode->viTVMode >> 2, VI_INTERLACE);
	}	
#endif
	
	// Fix scanline start for PAL modes to line 25
	Mode_264p.viYOrigin = PAL_OFFSET;
	Mode_528i.viYOrigin = PAL_OFFSET;	
	
	for(fb = 0; fb < 2; fb++)
	{
		frameBuffer[fb][0] = NULL;
		frameBuffer[fb][1] = NULL;
	}
	
	InitFrameBuffers();
	SetVideoMode(VIDEO_240P);
	VIDEO_SetBlack(FALSE);
}

void RestoreVideo()
{
	DeleteFrameBuffers();	
	
	if(mvmode)
		VIDEO_Configure(mvmode);	
}

void SetVideoMode(u32 newmode)
{	
	if(newmode == vmode)
		return;
		
	if(newmode >= VIDEO_480P && !VIDEO_HaveComponentCable())
		newmode = VIDEO_240P;		
		
	if(newmode >= TOTAL_VMODES || newmode < 0)
		newmode = VIDEO_240P;	
		
	if(newmode >= VIDEO_288P && newmode <= VIDEO_576I && !Options.EnablePAL)
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
			IsPAL = MODE_NTSC;
			offsetY = 0;
			break;
		case VIDEO_480I:
		case VIDEO_480P:
			dW = 640;
			dH = 480;
			IsPAL = MODE_NTSC;
			offsetY = 0;
			break;
		case VIDEO_288P:
		case VIDEO_576I_A264:		
			dW = 320;
			dH = 264;
			offsetY = 12; // (264 - 240) / 2 -> to center all in PAL modes
			IsPAL = MODE_PAL;
			break;
		case VIDEO_576I:
			dW = 640;
			dH = 528;
			offsetY = 12;
			IsPAL = MODE_PAL;
			break;
	}	
		
	VIDEO_Configure(rmode);			
	VIDEO_SetNextFramebuffer(frameBuffer[IsPAL][ActiveFB]);			
	VIDEO_Flush();	
}

void InitFrameBuffers()
{	
	if(!frameBuffer[MODE_NTSC][0])
		frameBuffer[MODE_NTSC][0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(&TVNtsc480Prog));			
	
	if(!frameBuffer[MODE_NTSC][1])	
		frameBuffer[MODE_NTSC][1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(&TVNtsc480Prog));	
		
	if(!frameBuffer[MODE_PAL][0])
		frameBuffer[MODE_PAL][0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(&TVPal576ProgScale));	
		
	if(!frameBuffer[MODE_PAL][1])
		frameBuffer[MODE_PAL][1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(&TVPal576ProgScale));		
		
	CleanFB();	
}

void CleanFB()
{
	if(frameBuffer[MODE_PAL][0])
		VIDEO_ClearFrameBuffer(&TVPal576ProgScale, frameBuffer[MODE_PAL][0], COLOR_BLACK);		
	if(frameBuffer[MODE_PAL][1])
		VIDEO_ClearFrameBuffer(&TVPal576ProgScale, frameBuffer[MODE_PAL][1], COLOR_BLACK);		

	if(frameBuffer[MODE_NTSC][0])
		VIDEO_ClearFrameBuffer(&TVNtsc480Prog, frameBuffer[MODE_NTSC][0], COLOR_BLACK);		
	if(frameBuffer[MODE_NTSC][0])
		VIDEO_ClearFrameBuffer(&TVNtsc480Prog, frameBuffer[MODE_NTSC][1], COLOR_BLACK);		
}

void DeleteFrameBuffers()
{
	if(frameBuffer[MODE_NTSC][0])
	{
		free(MEM_K1_TO_K0(frameBuffer[MODE_NTSC][0]));
		frameBuffer[MODE_NTSC][0] = NULL;		
	}
	if(frameBuffer[MODE_NTSC][1])
	{
		free(MEM_K1_TO_K0(frameBuffer[MODE_NTSC][1]));
		frameBuffer[MODE_NTSC][1] = NULL;		
	}	
	if(frameBuffer[MODE_PAL][0])
	{
		free(MEM_K1_TO_K0(frameBuffer[MODE_PAL][0]));
		frameBuffer[MODE_PAL][0] = NULL;		
	}
	if(frameBuffer[MODE_PAL][1])
	{
		free(MEM_K1_TO_K0(frameBuffer[MODE_PAL][1]));
		frameBuffer[MODE_PAL][1] = NULL;		
	}	
}

