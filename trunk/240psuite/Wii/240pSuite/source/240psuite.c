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

void TestPatternsMenu(ImagePtr title, ImagePtr sd);

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
	InitVideo();	
		
	ControllerInit();

	if(!InitGX())
		return 0;
		
	SetupGX();

    LoadFont();
	LoadScanlines();
	
	LoadOptions();
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
    
    sd->x = 221;
    sd->y = 94;
				
#ifdef WII_VERSION
	if(Options.ShowWiiRegion)
		GetWiiRegion();
#endif
	
	while(!close && !EndProgram) 
	{        
        char	res[40];
		u8      r = 0xff;
		u8      g = 0xff;
		u8      b = 0xff;
		u8   	c = 1;				    					   
		u16     x = 40;
		u16     y = 55;
        u32     pressed = 0;
		
		StartScene();
		        
		DrawImage(Back);
        DrawImage(sd);
		
        DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Test Patterns"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Drop Shadow Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Striped Sprite Test"); y += fh; c++;    
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Lag Test"); y += fh; c++;
        DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Manual Lag Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Scroll Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Grid Scroll Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Horizontal Stripes"); y += fh; c++;    
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Checkerboard"); y += fh; c++;
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
        DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Sound Test"); y += fh; c++;
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
				sprintf(res, "Video: 288p");				
				break;			
			case VIDEO_576I_A264:
				sprintf(res, "Video: 576i (scaled 264p)");
				break;
			case VIDEO_576I:
				sprintf(res, "Video: 576i (Scaling disabled)");
				break;
			case VIDEO_480P:
				sprintf(res, "Video: 480p (Scaling disabled)");
				break;
			case VIDEO_480P_SL:
				sprintf(res, "Video: 480p (scaled 240p)");
				break;				
		}
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, res); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Options"); 
		
		if(VIDEO_HaveComponentCable())		
			DrawStringS(215, 225, r, g,	 b, "Component");
		if(VIDEO_HaveSCARTRGBCable())
			DrawStringS(215, 225, r, g,	 b, "SCART RGB");		

#ifdef WII_VERSION
		if(Options.ShowWiiRegion)
			DrawStringS(215, 215, r, g, b, wiiregion);
#endif				
		
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
					DropShadowTest();
					break;
				case 3:
					StripedSpriteTest();
					break;
				case 4:
					PassiveLagTest();
					break;					
				case 5:
					LagTest();
					break;					
				case 6:
					ScrollTest();
					break;
				case 7:
					GridScrollTest();
					break;
				case 8:
					DrawStripes();
					break;
				case 9:
					DrawCheckBoard();
					break;				
				case 10:
					LEDZoneTest();
					break;
				case 11:
					if(vmode != VIDEO_480P && vmode != VIDEO_480P_SL)
						Alternate240p480i();					
					break;					
				case 12:
					SoundTest();
					break;				
				case 13:
					SelectVideoMode(Back);					
					break;
				case 14:
					ChangeOptions(Back);
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
	int 			sel = 1, close = 0;		
	
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
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "SMPTE Color Bars"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Color Bars with Gray Scale"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Color Bleed Check"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Grid"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Linearity"); y += fh; c++;		
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Gray Ramp"); y += fh; c++;		
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "White Screen"); y += fh; c++;				
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "100 IRE"); y += fh; c++;				
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Sharpness"); y += fh; c++;	
		DrawStringS(x, y + fh, r, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); y += fh; 

		if(VIDEO_HaveComponentCable())		
			DrawStringS(215, 225, r, g,	 b, "Component");		
		if(VIDEO_HaveSCARTRGBCable())
			DrawStringS(215, 225, r, g,	 b, "SCART RGB");		
		
#ifdef WII_VERSION
		if(Options.ShowWiiRegion)
			DrawStringS(215, 215.0f, r, g, b, wiiregion);
#endif				
				
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
					DrawSMPTEColorBars();
					break;
				case 4:
					Draw601ColorBars();
					break;
				case 5:
					DrawColorBleed();
					break;
				case 6:
					DrawGrid();
					break;					
				case 7:
					DrawLinearity();
					break;										
				case 8:
					DrawGrayRamp();
					break;
				case 9:
					DrawWhiteScreen();
					break;					
				case 10:
					Draw100IRE();
					break;				
				case 11:
					DrawSharpness();
					break;				
				case 12:
					close = 1;
					break;
			} 			            										
		}
	}

	return;
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
	HWButton = SYS_POWEROFF_STANDBY;
}
 
/**
 * Callback for the power button on the Wiimote.
 * @param[in] chan The Wiimote that pressed the button
 */
void WiimotePowerPressed(s32 chan)
{
	HWButton = SYS_POWEROFF_STANDBY;
}

#endif
