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
#include <stdlib.h>
#include "controller.h"
#include "video.h"
#include "image.h"
#include "font.h"
#include "options.h"
#include "help.h"
#include "menu.h"

#include "patterns.h"
#include "tests.h"


#ifdef WII_VERSION
#include "CheckRegion.h"
#endif

// Default options
struct options_st Options = DEFAULT_OPTIONS;
struct options_st DefaultOptions = DEFAULT_OPTIONS;

void TestPatternsMenu(ImagePtr title, ImagePtr sd);
void VideoPatternsMenu(ImagePtr title, ImagePtr sd);
void SoundTestMenu(ImagePtr title, ImagePtr sd);
void DrawMenuFooter(u8 r, u8 g, u8 b);

#ifdef WII_VERSION
s8 HWButton = -1; 
void WiiResetPressed();
void WiiPowerPressed();
void WiimotePowerPressed(s32 chan);
#endif

int main(int argc, char **argv) 
{
	int 		close = 0;	
	ImagePtr 	Back = NULL, sd = NULL;
	u8   		sel = 1;

#ifdef WII_VERSION
	SYS_SetResetCallback(WiiResetPressed);
	SYS_SetPowerCallback(WiiPowerPressed);
	WPAD_SetPowerButtonCallback(WiimotePowerPressed);
#endif

	ControllerInit();

	LoadOptions();
	
	InitVideo();	

	if(!InitGX())
		return 0;
		
	SetupGX();

    LoadFont();
	LoadScanlines();
	
	DrawIntro();
	
	Back = LoadImage(BACKIMG, 0);
	if(!Back)
    {
        ReleaseFont();
		EndProgram = 1;
    }	
    sd = LoadImage(SDIMG, 0);
    if(!sd)
    {
        ReleaseFont();
		EndProgram = 1;
    }   
	
#ifdef WII_VERSION	
	GetWiiRegion();
	
	if(OffsetH || AspectRatio)
	{
		sd->x = 250;
		sd->y = 80;	
		ShowVideoWarning(sd);
	}
#endif
	
	sd->x = 200;
    sd->y = 80;		
		
	while(!close && !EndProgram) 
	{
		u8      r = 0xff;
		u8      g = 0xff;
		u8      b = 0xff;
		u8   	c = 1;				    					   
		u16     x = 60;
		u16     y = 80;
        u32     pressed = 0;
		char 	res[100];
		
		StartScene();
		        
		DrawImage(Back);
        DrawImage(sd);
		
        DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Test Patterns"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Video tests"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Sound tests"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Hardware tools"); y += 4*fh; c++;
		
		GetVideoModeStr(res, 1);
		if(ChangeVideoEnabled)
		{
			DrawStringS(x, y, r-0x40, sel == c ? 0 : g, sel == c ? 0 : b, "Video");
			DrawStringS(x+6*fw, y, r-0x40, sel == c ? 0 : g, sel == c ? 0 : b, res); y += fh; c++;
		}
		else
		{
			DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA,	sel == c ? 0x77 : 0xAA, "Video");
			DrawStringS(x+6*fw, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, res); y += fh; c++;		
		}
		DrawStringS(x, y, r-0x40, sel == c ? 0 : g, sel == c ? 0 : b, "Options"); y += fh; c++;
		DrawStringS(x, y, r-0x40, sel == c ? 0 : g, sel == c ? 0 : b, "Help"); y += fh; c++;
		DrawStringS(x, y, r-0x40, sel == c ? 0 : g, sel == c ? 0 : b, "Credits"); 
		
		DrawMenuFooter(r, g, b);
		
		EndScene();
		
        ControllerScan();
		
		pressed = Controller_ButtonsDown(0);

#ifdef WII_VERSION
		if(HWButton != -1)
			EndProgram = 1;
#endif
			
		if ( pressed & PAD_BUTTON_START ) 		
			DrawMenu = 1;	
		
		if ( pressed & PAD_BUTTON_UP )
		{
			sel --;
			if(sel < 1)
				sel = c;		
		}
		
		if ( pressed & PAD_BUTTON_DOWN )
		{
			sel ++;
			if(sel > c)
				sel = 1;	
		}

        if (pressed & PAD_BUTTON_A)
		{		
			switch(sel)
			{
				case 1:
					TestPatternsMenu(Back, sd);
					break;
				case 2:
					VideoPatternsMenu(Back, sd);
					break;
				case 3:
					SoundTestMenu(Back, sd);
					break;
				case 4:
					break;
				case 5:	
					if(ChangeVideoEnabled)
					{
						Back->alpha = 0xaa;
						SelectVideoMode(Back);
						Back->alpha = 0xff;
					}
					break;
				case 6:
					{
						Back->alpha = 0xaa;
						ChangeOptions(Back);
						Back->alpha = 0xff;
					}
					break;
				case 7:
					HelpWindow(Back);
					break;
				case 8:
					DrawCredits(Back);
					break;								
			} 									
		}
	}
		
	SaveOptions();
	
	FreeImage(&Back);
	ReleaseFont();	
	ReleaseScanlines();
	
	EndGX();
	RestoreVideo();
	
#ifdef WII_VERSION
	if(HWButton != -1)
		SYS_ResetSystem(HWButton, 0, 0);
#endif

	return 0;
}

void TestPatternsMenu(ImagePtr title, ImagePtr sd)
{
	int			sel = 1, close = 0;		
	
	while(!close && !EndProgram) 
	{		
		u8      r = 0xff;
		u8      g = 0xff;
		u8      b = 0xff;
		u8   	c = 1;
		u16     x = 40;
		u16     y = 55;
        u32     pressed = 0;
				
		StartScene();
		        
		DrawImage(title);
        DrawImage(sd);

		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Pluge"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Color Bars"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "EBU Color Bars"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "SMPTE Color Bars"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Color Bars with Gray Scale"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Color Bleed Check"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Monoscope"); y += fh; c++;		
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Grid"); y += fh; c++;
		if(vmode != VIDEO_240P)
		{
			DrawStringS(x, y, sel == c ? 0x77 : 0xAA, 
					sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "Grid 224p"); y += fh; c++;
		}
		else
		{
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Grid 224p"); y += fh; c++;   
		}
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Gray Ramp"); y += fh; c++;		
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "White & RGB Screens"); y += fh; c++;				
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "100 IRE"); y += fh; c++;				
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Sharpness"); y += fh; c++;	
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Overscan"); y += fh; c++;	
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Convergence"); y += fh; c++;	
		DrawStringS(x, y, r-0x40, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); y += fh; 

		DrawMenuFooter(r, g, b);
				
		EndScene();		
        
        ControllerScan();

		pressed = Controller_ButtonsDown(0);
		
		if ( pressed & PAD_BUTTON_START ) 		
			DrawMenu = 1;	

		if ( pressed & PAD_BUTTON_UP )
	    {
		    sel --;
		    if(sel < 1)
			    sel = c;		
	    }
	    
	    if ( pressed & PAD_BUTTON_DOWN )
	    {
		    sel ++;
		    if(sel > c)
			    sel = 1;	
	    }			
			
		if ( pressed & PAD_BUTTON_B ) 		
			close = 1;	
	
		if (pressed & PAD_BUTTON_A)
		{            
			switch(sel)
			{				
				case 1:
					DrawPluge();
					break;				
				case 2:
					DrawColorBars();
					break;
				case 3:
					DrawEBUColorBars();
					break;
				case 4:
					DrawSMPTEColorBars();
					break;
				case 5:
					Draw601ColorBars();
					break;
				case 6:
					DrawColorBleed();
					break;
				case 7:
					DrawMonoscope();
					break;	
				case 8:
					DrawGrid();
					break;	
				case 9:
					if(vmode == VIDEO_240P)
						DrawGrid224();
					break;
				case 10:
					DrawGrayRamp();
					break;
				case 11:
					DrawWhiteScreen();
					break;					
				case 12:
					Draw100IRE();
					break;				
				case 13:
					DrawSharpness();
					break;				
				case 14:
					DrawOverscan();
					break;				
				case 15:
					DrawConvergence();
					break;				
				case 16:
					close = 1;
					break;
			} 			            										
		}
	}

	return;
}

void VideoPatternsMenu(ImagePtr title, ImagePtr sd)
{
	int 	sel = 1, close = 0;		
	
	while(!close && !EndProgram) 
	{		
		u8      r = 0xff;
		u8      g = 0xff;
		u8      b = 0xff;
		u8   	c = 1;				    					   
		u16     x = 40;
		u16     y = 70;
        u32     pressed = 0;
				
		StartScene();
		        
		DrawImage(title);
        DrawImage(sd);

		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Drop Shadow Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Striped Sprite Test"); y += fh; c++;    
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Lag Test"); y += fh; c++;
        DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Timing & Reflex Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Scroll Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Grid Scroll Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Horiz/Vert Stripes"); y += fh; c++;    
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Checkerboard"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Diagonal test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Backlit Zone Test"); y += fh; c++;
		if(vmode == VIDEO_480P || vmode == VIDEO_480P_SL)
		{
			DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, 
				IsPAL ? "Alternating 288p/576i Test" : "Alternating 240p/480i Test"); y += fh; c++;
		}
		else
		{
			DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, 
				IsPAL ? "Alternating 288p/576i Test" : "Alternating 240p/480i Test"); y += fh; c++;
		}
		DrawStringS(x, y, r-0x40, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); y += fh; 

		DrawMenuFooter(r, g, b);		
				
		EndScene();		
        
        ControllerScan();

		pressed = Controller_ButtonsDown(0);
		
		if ( pressed & PAD_BUTTON_START ) 		
			DrawMenu = 1;	

		if ( pressed & PAD_BUTTON_UP )
	    {
		    sel --;
		    if(sel < 1)
			    sel = c;		
	    }
	    
	    if ( pressed & PAD_BUTTON_DOWN )
	    {
		    sel ++;
		    if(sel > c)
			    sel = 1;	
	    }			
			
		if ( pressed & PAD_BUTTON_B ) 		
			close = 1;	
	
		if (pressed & PAD_BUTTON_A)
		{            
			switch(sel)
			{				
				case 1:	
					DropShadowTest();
					break;
				case 2:
					StripedSpriteTest();
					break;
				case 3:
					PassiveLagTest();
					break;					
				case 4:
					TimingReflexTest();
					break;					
				case 5:
					ScrollTest();
					break;
				case 6:
					GridScrollTest();
					break;
				case 7:
					DrawStripes();
					break;
				case 8:
					DrawCheckBoard();
					break;			
				case 9:
					DiagonalPatternTest();
					break;
				case 10:
					LEDZoneTest();
					break;
				case 11:
					if(vmode != VIDEO_480P && vmode != VIDEO_480P_SL)
						Alternate240p480i();					
					break;		
				case 12:
					close = 1;
					break;			
			} 			            										
		}
	}

	return;
}

void SoundTestMenu(ImagePtr title, ImagePtr sd)
{
	int 	sel = 1, close = 0;
#ifdef WII_VERSION
	u8		mdf_exists = 0, aet_exists = 0;
	
	mdf_exists = FileExists("mdf.raw");
	aet_exists = FileExists("aet.raw");
#endif
	
	while(!close && !EndProgram) 
	{		
		u8      r = 0xff;
		u8      g = 0xff;
		u8      b = 0xff;
		u8   	c = 1;
		u16     x = 60;
		u16     y = 90;
        u32     pressed = 0;
				
		StartScene();
		        
		DrawImage(title);
        DrawImage(sd);

		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Sount Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Audio Sync Test"); y += fh; c++;
#ifdef WII_VERSION
		if(mdf_exists)
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "MDFourier");
		else
			DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "MDFourier");
		y += fh; c++;    
		if(aet_exists)
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Audio Equipment Test");
		else
			DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "Audio Equipment Test");
		y += 2*fh; c++;
#endif
		y += fh;
		DrawStringS(x, y, r-0x40, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); y += fh; 

		DrawMenuFooter(r, g, b);		
				
		EndScene();		
        
        ControllerScan();

		pressed = Controller_ButtonsDown(0);
		
		if ( pressed & PAD_BUTTON_START ) 		
			DrawMenu = 1;	

		if ( pressed & PAD_BUTTON_UP )
	    {
		    sel --;
		    if(sel < 1)
			    sel = c;		
	    }
	    
	    if ( pressed & PAD_BUTTON_DOWN )
	    {
		    sel ++;
		    if(sel > c)
			    sel = 1;	
	    }			
			
		if ( pressed & PAD_BUTTON_B ) 		
			close = 1;	
	
		if (pressed & PAD_BUTTON_A)
		{            
			switch(sel)
			{				
				case 1:	
					SoundTest();
					break;
				case 2:
					AudioSyncTest();
					break;
#ifdef WII_VERSION		
				case 3:
					if(mdf_exists)
						MDFourier();
					break;
				case 4:
					if(aet_exists)
						AudioEquipmentTest();
					break;	
#endif
				case 5:
					close = 1;
					break;			
			} 			            										
		}
	}

	return;
}

void DrawMenuFooter(u8 r, u8 g, u8 b)
{
	char res[100];
	
	GetVideoModeStr(res, 0);
	DrawStringS(120, 200, r-0x40, g, b, res);
	
	if(VIDEO_HaveComponentCable())		
		DrawStringS(250, 215, r, g,	 b, "YPbPr");

#ifdef WII_VERSION		
	DrawStringS(35, 200, r, g, b, wiiregion);
#endif				
}



#ifdef WII_VERSION
/**
 * Callback for the reset button on the Wii.
 */
void WiiResetPressed()
{
	HWButton = SYS_RETURNTOMENU;
}
 
/**
 * Callback for the power button on the Wii.
 */
void WiiPowerPressed()
{
	HWButton = SYS_POWEROFF;
}
 
/**
 * Callback for the power button on the Wiimote.
 * @param[in] chan The Wiimote that pressed the button
 */
void WiimotePowerPressed(s32 chan)
{
	HWButton = SYS_POWEROFF;
}

#endif
