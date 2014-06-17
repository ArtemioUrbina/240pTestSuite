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

GXRModeObj TVPal264DsFull =
{
    VI_TVMODE_PAL_DS,       // viDisplayMode
    640,             // fbWidth
    264,             // efbHeight
    284,             // xfbHeight
    (VI_MAX_WIDTH_PAL - 640)/2,         // viXOrigin
    PAL_OFFSET,        // viYOrigin
    640,             // viWidth
    572,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_FALSE,        // aa

    // sample points arranged in increasing Y order
	{
		{6,6},{6,6},{6,6},  // pix 0, 3 sample points, 1/12 units, 4 bits each
		{6,6},{6,6},{6,6},  // pix 1
		{6,6},{6,6},{6,6},  // pix 2
		{6,6},{6,6},{6,6}   // pix 3
	},

    // vertical filter[7], 1/64 units, 6 bits each
	{
		 0,         // line n-1
		 0,         // line n-1
		21,         // line n
		22,         // line n
		21,         // line n
		 0,         // line n+1
		 0          // line n+1
	}
};

// Starts in line 23 1/2 and end in line 308. Lines 309 and 310 don't show.
// GC starts at 23

GXRModeObj TVPal576IntDfFull =
{
    VI_TVMODE_PAL_INT,      // viDisplayMode
    640,             // fbWidth
    528,             // efbHeight
    572,             // xfbHeight when using 576 2 lines are not drawn
    (VI_MAX_WIDTH_PAL - 640)/2,         // viXOrigin
    PAL_OFFSET-2, // (VI_MAX_HEIGHT_PAL - 576)/2,        // viYOrigin
    640,             // viWidth
    574,             // viHeight, when using 576 2 lines are not drawn
    VI_XFBMODE_DF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_FALSE,        // aa

    // sample points arranged in increasing Y order
	{
		{6,6},{6,6},{6,6},  // pix 0, 3 sample points, 1/12 units, 4 bits each
		{6,6},{6,6},{6,6},  // pix 1
		{6,6},{6,6},{6,6},  // pix 2
		{6,6},{6,6},{6,6}   // pix 3
	},
    // vertical filter[7], 1/64 units, 6 bits each
	{
		 8,         // line n-1
		 8,         // line n-1
		10,         // line n
		12,         // line n
		10,         // line n
		 8,         // line n+1
		 8          // line n+1
	}
};

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

s8	OffsetH			= 0;
s8	AspectRatio		= 0;

void InitVideo()
{
	int fb = 0;
	
	VIDEO_Init();
		
	mvmode = VIDEO_GetPreferredMode(NULL);

	Mode_240p = TVNtsc240Ds;
	Mode_480i = TVNtsc480IntDf;
	Mode_264p = TVPal264Ds;
	Mode_528i = TVPal528IntDf;
	
	Set576iLine23Option(Options.PALline23);

#ifdef WII_VERSION		
	CONF_GetDisplayOffsetH(&OffsetH);
	AspectRatio = CONF_GetAspectRatio();
		
	/* Adjust SCART cable to output RGB and not YUV */		
	if(!VIDEO_HaveComponentCable())
	{
		if(CONF_GetEuRGB60() > 0 || CONF_GetVideo() == CONF_VIDEO_PAL)
		{
			Mode_240p.viTVMode = VI_TVMODE_EURGB60_DS;
			Mode_480i.viTVMode = VI_TVMODE_EURGB60_INT;	  		  	  
		}		
	}
	  
	if(CONF_GetVideo() == CONF_VIDEO_PAL ||
		CONF_GetVideo() == CONF_VIDEO_MPAL)
		Options.EnablePAL = 1;			
#endif

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
	if(newmode >= TOTAL_VMODES || newmode < 0)
		newmode = VIDEO_240P;	
		
	// check 480p safety
	if(newmode == VIDEO_480P && !VIDEO_HaveComponentCable() && !Options.Activate480p)
		newmode = VIDEO_240P;		
		
	if(newmode == VIDEO_480P_SL && !VIDEO_HaveComponentCable() && !Options.Activate480p)
		newmode = VIDEO_240P;				
		
	// check PAL safety
	if(newmode >= VIDEO_288P && newmode <= VIDEO_576I && !Options.EnablePAL)
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
		
	ActiveFB ^= 1;  	
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

void EnableStretchedPALModes(int enable)
{
	Options.PALScale576 = enable;	
	
	if(enable)
	{
		Mode_264p = TVPal264DsFull;
		Mode_528i = TVPal576IntDfFull;
	}
	else
	{
		Mode_264p = TVPal264Ds;
		Mode_528i = TVPal528IntDf;
		Set576iLine23Option(Options.PALline23);
	}	
}

char *GetPalStartText()
{
	switch(Options.PALline23)
	{
		case PAL_LINE23HALF:
#ifdef WII_VERSION
			return("23 1/2");
#else
			return("23");
#endif
		case PAL_LINE24:
			return("24");
		case PAL_CENTERED:
			return("Centered");
		case PAL_BOTTOM:
			return("Bottom");
	}
	return("");
}

void Set576iLine23Option(s8 set)
{	
	if(Options.PALScale576)
		return;
		
	if(set > PAL_BOTTOM)
		set = PAL_LINE23HALF;
		
	switch(set)
	{
		case PAL_LINE23HALF:
			Mode_264p.viYOrigin = PAL_OFFSET;
			Mode_528i.viYOrigin = PAL_OFFSET - 2;
			break;
		case PAL_LINE24:
			Mode_264p.viYOrigin = PAL_OFFSET + 1;
			Mode_528i.viYOrigin = PAL_OFFSET - 1;	
			break;
		case PAL_CENTERED:
			Mode_264p.viYOrigin = (VI_MAX_HEIGHT_PAL/2 - 264)/2; // starts in 32
			Mode_528i.viYOrigin = (VI_MAX_HEIGHT_PAL - 528)/2;	// starts in 35
			break;
		case PAL_BOTTOM:
			Mode_264p.viYOrigin = VI_MAX_HEIGHT_PAL/2 - 264 - 1; // Max output line in 264 mode is 306, starts in 43
			Mode_528i.viYOrigin = VI_MAX_HEIGHT_PAL - 528;	// max ouput is 310, last line befor pulses, starts in 47
			break;
	}
			
	Options.PALline23 = set;
}

void GetVideoModeStr(char *res, int shortdesc)
{
	if(!shortdesc)
	{
		switch(vmode)
		{
			case VIDEO_240P:
				sprintf(res, "Video: 240p");				
				break;			
			case VIDEO_480I_A240:
				sprintf(res, "Video: 480i (scaled 240p)");
				break;
			case VIDEO_480I:
				sprintf(res, "Video: 480i (Scaling disabled)");
				break;
			case VIDEO_288P:
				if(Options.PALScale576)
					sprintf(res, "Video: 288p (stretched)");				
				else
					sprintf(res, "Video: 288p");				
				break;			
			case VIDEO_576I_A264:
				if(Options.PALScale576)
					sprintf(res, "Video: 576i (scaled 264p stretched)");
				else
					sprintf(res, "Video: 576i (scaled 264p)");
				break;			
			case VIDEO_576I:
				if(Options.PALScale576)
					sprintf(res, "Video: 576i (no scaling stretched)");
				else
					sprintf(res, "Video: 576i (Scaling disabled)");
				break;
			case VIDEO_480P:
				sprintf(res, "Video: 480p (Scaling disabled)");
				break;
			case VIDEO_480P_SL:
				sprintf(res, "Video: 480p (scaled 240p)");
				break;				
		}
	}
	else
	{
		switch(vmode)
		{
			case VIDEO_240P:
				sprintf(res, "[240p]");				
				break;			
			case VIDEO_480I_A240:
				sprintf(res, "[480i LD]");
				break;
			case VIDEO_480I:
				sprintf(res, "[480i 1:1]");
				break;
			case VIDEO_288P:
				if(Options.PALScale576)
					sprintf(res, "[288p STR]");				
				else
					sprintf(res, "[288p]");				
				break;			
			case VIDEO_576I_A264:
				if(Options.PALScale576)
					sprintf(res, "[576i LD/S]");
				else
					sprintf(res, "[576i LD]");				
				break;
			case VIDEO_576I:
				if(Options.PALScale576)
					sprintf(res, "[576i STR]");
				else
					sprintf(res, "[576i 1:1]");
				break;
			case VIDEO_480P:
				sprintf(res, "[480p 1:1]");
				break;
			case VIDEO_480P_SL:
				sprintf(res, "[480p LD]");
				break;				
		}

	}
}
