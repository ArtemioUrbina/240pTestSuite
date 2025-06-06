/* 
 * 240p Test Suite
 * Copyright (C)2014-2022 Artemio Urbina (Wii GX)
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


#ifdef WII_VERSION
#include "CheckRegion.h"
char wiiregion[100];
#endif

u8 DrawMenu = 0;
char **HelpData = GENERALHELP;
u8 EndProgram = 0;
u8 ChangeVideoEnabled = 1;

ImagePtr 	sd_b1 = NULL, sd_b2 = NULL;

void ShowMenu()
{
	int 		sel = 1, close = 0;
	ImagePtr	Back = NULL, cFB = NULL;
	
	cFB = CopyFrameBufferToImage();	
	if(cFB)
	{	
		cFB->r = 0x77;
		cFB->g = 0x77;
		cFB->b = 0x77;
	}
	Back = LoadImage(FLOATMENUIMG, 0);
	if(Back)
	{
		Back->x = (dW - Back->w) / 2;
		Back->y = (dH - Back->h) / 2;
		
		Back->alpha = 0xaa;
	}
	   
	while(!close && !EndProgram) 
	{		
		u8		r = 0xff;
		u8		g = 0xff;
		u8		b = 0xff;
		u8	 	c = 1;
		u16 	x = Back ? Back->x + 20 : 100;
		u16 	y = Back ? Back->y + 10 : 60;
		u32 	pressed = 0;
		char	videomode[50];
				
		StartScene();
		
		DrawImage(cFB);
		DrawImage(Back);

		DrawStringS(x, y, 0x00, 0xff, 0x00, VERSION_NUMBER); y += 3*fh;
		
		GetVideoModeStr(videomode, 1);
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Help"); y += fh; c++;
		if(ChangeVideoEnabled)
		{
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Video");
			DrawStringS(x+6*fw, y, r, sel == c ? 0 : g, sel == c ? 0 : b, videomode); y += fh; c++;
		}
		else
		{
			DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA,	sel == c ? 0x77 : 0xAA, "Video");
			DrawStringS(x+6*fw, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, videomode); y += fh; c++;		
		}
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Options"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Credits"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Close Menu"); y += 2* fh; c++;
		DrawStringS(x, y, r-0x40, sel == c ? 0 : g,	sel == c ? 0 : b, "Exit 240p Suite"); y += 2* fh;
		
#ifdef WII_VERSION
		u8 		battery;
		char 	level[30];
		
		battery = ControllerBattery(0);
		if(battery)
		{
			sprintf(level, "Wiimote batt: %d%%", battery > 100 ? 100 : battery);
			DrawStringS(x-10, y, battery < 25 ? 0xff : 0x00, 0x88, battery >= 25 ? 0xff : 0x00 , level); 
		}
#endif

		EndScene();		
		
		ControllerScan();

		pressed = Controller_ButtonsDown(0);
		
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
		
		if (pressed & PAD_BUTTON_B || pressed & PAD_BUTTON_START)
			close = 1;	
	
		if (pressed & PAD_BUTTON_A)
		{
			switch(sel)
			{
				case 1:
					HelpWindow(cFB);
					break;
				case 2:	
					if(ChangeVideoEnabled)
					{
						SelectVideoMode(cFB);
						if(Back)
						{
							Back->x = (dW - Back->w) / 2;
							Back->y = (dH - Back->h) / 2;
						}
					}
					break;
				case 3:
					ChangeOptions(cFB);
					break;
				case 4:	
					DrawCredits(cFB);
					break;
				case 5:
					close = 1;
					break;
				case 6:
					EndProgram = 1;
					close = 1;
					break;
				default:
					break;
			}
		}
	}
	
	HelpData = GENERALHELP;
	FreeImage(&Back);
	FreeImage(&cFB);

	return;
}


void ChangeOptions(ImagePtr title)
{	
	int 		sel = 1, close = 0;	
	ImagePtr	back;
	
	back = LoadImage(HELPIMG, 0);
	if(!back)
		return;
		
	back->alpha = 0xaa;
		
	while(!close && !EndProgram) 
	{		
		u8		r = 0xff;
		u8		g = 0xff;
		u8		b = 0xff;
		u8	 	c = 1;
		u16 	x = 50;
		u16 	y = 41;
		u16		OptPos = 160;
		u32 	pressed = 0, held = 0;
		char	intensity[80];
				
		StartScene();
				
		DrawImage(title);
		DrawImage(back);

		DrawStringS(x + 70, 42, 0x00, 0xff, 0x00, "General Options"); 
		
#ifdef WII_VERSION
		y += fh; 
		
		// option 1, TrapFilter
		DrawStringS(x + OptPos, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, Options.TrapFilter ? "ON" : "OFF");
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "VI Trap Filter (Composite):"); y += fh; c++;
#else
		y += 2*fh; 
#endif

		// option 2, FlickerFilter
		DrawStringS(x + OptPos, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, Options.FlickerFilter ? "ON" : "OFF");
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "GX Deflickering Filter:"); y += fh; c++;	
		
		/*
		// option 3, BilinearFiler		
		DrawStringS(x + OptPos, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, GetBilinearText(1));
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "GX Filter:"); y += fh; c++;
		*/
		
		// option 3, Active 480p
		if(VIDEO_HaveComponentCable())
		{
			DrawStringS(x + OptPos, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, Options.Activate480p ? "ON" : "OFF");
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Enable 480p Modes:"); y += fh; c++;
		}
		else
		{
			DrawStringS(x + OptPos, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, Options.Activate480p ? "ON" : "OFF");
			DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "Enable 480p Modes:"); y += fh; c++;
		}

		// Option 4
		DrawStringS(x + OptPos, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, Options.EnablePAL ? "ON" : "OFF");
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Enable PAL Modes:"); y += fh; c++;
		
		// Option 5
		if(Options.EnablePAL)
		{
			DrawStringS(x + OptPos, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, Options.EnablePALBG ? "ON" : "OFF"); 					
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Enable PAL Background:"); y += fh; c++;								
		}
		else
		{
			DrawStringS(x + OptPos, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, Options.EnablePALBG ? "ON" : "OFF"); 					
			DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "Enable PAL Background:"); y += fh; c++;
		}
		
		// Option 6
		{
			char BorderColor[100];
			
			sprintf(BorderColor, "[%X,%X,%X]", Options.PalBackR, Options.PalBackG, Options.PalBackB);
			if(Options.EnablePAL)
			{
				DrawStringS(x + OptPos, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, BorderColor);
				DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Change PAL Background:"); y += fh; c++;
			}
			else
			{
				DrawStringS(x + OptPos, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, BorderColor);
				DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "Change PAL Background:"); y += fh; c++;
			}

		}
		
		// Option 7
		{
			char palstart[20];
			
			sprintf(palstart, "%s", GetPalStartText());
			if(Options.EnablePAL)
			{
				DrawStringS(x + OptPos, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, palstart);
				DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "PAL starting line:"); y += fh; c++;
			}
			else
			{
				DrawStringS(x + OptPos, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, palstart);
				DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "PAL starting line:"); y += fh; c++;
			}
		}		
		
		// Option 8
		if(Options.EnablePAL)
		{
			DrawStringS(x + OptPos, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, Options.PALScale576 ? "ON" : "OFF");
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "V. Stretch to 288/576 in PAL:"); y += fh; c++;
		}
		else
		{
			DrawStringS(x + OptPos, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, Options.PALScale576 ? "ON" : "OFF");
			DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "V. Stretch to 288/576 in PAL:"); y += fh; c++;
		}
		
		// option 9, Stretch Horizontal
		DrawStringS(x + OptPos, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, Options.Enable720Stretch ? "ON" : "OFF");
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "H. Stretch to 720:"); y += fh; c++;

		
		// option 10 Scanline intensity
		sprintf(intensity, "%d%%", GetScanlineIntensity());
		if(vmode == VIDEO_480P_SL)
		{
			DrawStringS(x + OptPos, y, r, sel == c ? 0 : g, sel == c ? 0 : b, intensity); 
			DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "480p Scanline Intensity:"); y += fh; c++;
			
		// option 11, Scanline even/odd
			DrawStringS(x + OptPos, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, ScanlinesEven() ? "EVEN" : "ODD");
			DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "480p Scanlines:"); y += fh; c++;	
		}
		else
		{
			DrawStringS(x + OptPos, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, intensity);
			DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "480p Scanline Intensity:"); y += fh; c++;			
			
		// option 11, Scanline even/odd
			DrawStringS(x + OptPos, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, ScanlinesEven() ? "EVEN" : "ODD"); 					
			DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "480p Scanlines:"); y += fh; c++;	
		}

#ifdef WII_VERSION
		// option 12, SFC CC
		DrawStringS(x + OptPos, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, Options.SFCClassicController ? "ON" : "OFF");
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "SFC Classic Controller:"); y += fh; c++;
#endif

		// Option 13
		DrawStringS(x, y + fh, r-0x40, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu");

		r = g = b = r - 0x30;
		y += fh / 2;
		
		/*
		if(sel == 3)
			DrawStringS(x-15, y + 2*fh, r, g, b, GetBilinearText(0));
		*/
	
		if(Options.EnablePAL && Options.PALScale576)
		{
#ifdef WII_VERSION
			if(sel == 7)
#else
			if(sel+1 == 7)
#endif
				DrawStringS(x-15, y + 2*fh, r, g, b, "This setting has no effect in stretched modes");
		}
		
		if(Options.EnablePAL)
		{
#ifdef WII_VERSION	
			if(sel == 8)
#else
			if(sel+1 == 8)
#endif
				DrawStringS(x-15, y + 2*fh, r, g, b, "Vert. stretching disables 1:1 pixel mapping");
		}
		
#ifdef WII_VERSION	
			if(sel == 9)
#else
			if(sel+1 == 9)
#endif
				DrawStringS(x-15, y + 2*fh, r, g, b, "Horz. stretching disables 1:1 pixel mapping");

#ifdef WII_VERSION
		if(vmode == VIDEO_480P_SL && sel == 10)
		{
			if(ControllerType != ControllerGC && !Options.SFCClassicController)
				DrawStringS(x-15, y + 2*fh, r, g, b, "Adjust with + & - buttons or Left & Right");
			else
				DrawStringS(x-15, y + 2*fh, r, g, b, "Adjust with L & R triggers or Left & Right");
		}
#else
		if(vmode == VIDEO_480P_SL && sel+1 == 10)
			DrawStringS(x-15, y + 2*fh, r, g, b, "Adjust with L & R triggers or Left & Right");
#endif


#ifdef WII_VERSION
		if(vmode != VIDEO_480P_SL && (sel == 10 || sel == 11))
#else
		if(vmode != VIDEO_480P_SL && (sel+1 == 10 || sel+1 == 11))
#endif
			DrawStringS(x-15, y + 2*fh, r, g, b, "Scanlines are only available in\n480 Line Doubled mode");
			
#ifdef WII_VERSION
		if(sel == 12)
			DrawStringS(x-15, y + 2*fh, r, g, b, "Change Classic Controller Button map:\n [HOME] -> [+] and [- +] -> [L R]");
#endif

#ifdef WII_VERSION
		DrawStringS(x+60, 200, r, g, b, "Press HOME for help");
#else
		DrawStringS(x+60, 200, r, g, b, "Press START for help");
#endif

		EndScene();
		
		ControllerScan();

		pressed = Controller_ButtonsDown(0);
		held = Controller_ButtonsHeld(0);

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

#ifdef WII_VERSION
		if ( pressed & PAD_TRIGGER_R && sel == 10)
#else
		if ( pressed & PAD_TRIGGER_R && sel+1 == 10)
#endif
		{
			if(vmode == VIDEO_480P_SL)
				RaiseScanlineIntensity();
		}

#ifdef WII_VERSION
		if ( pressed & PAD_TRIGGER_L && sel == 10)
#else
		if ( pressed & PAD_TRIGGER_L && sel+1 == 10)
#endif		
		{
			if(vmode == VIDEO_480P_SL)
				LowerScanlineIntensity();
		}			

#ifdef WII_VERSION			
		if ( held & PAD_BUTTON_RIGHT && sel == 10)
#else
		if ( held & PAD_BUTTON_RIGHT && sel+1 == 10)
#endif		
		{
			if(vmode == VIDEO_480P_SL)
				RaiseScanlineIntensity();
		}
		
#ifdef WII_VERSION
		if ( held & PAD_BUTTON_LEFT && sel == 10)
#else
		if ( held & PAD_BUTTON_LEFT && sel+1 == 10)
#endif		
		{
			if(vmode == VIDEO_480P_SL)
				LowerScanlineIntensity();
		}
		
		if(pressed & PAD_BUTTON_START)
		{
#ifdef WII_VERSION
			HelpData = OPTIONSHELP;
#else
			HelpData = OPTIONSGCHELP;
#endif
			HelpWindow(title);
			HelpData = GENERALHELP;
		}
		
		if(pressed & PAD_BUTTON_B) 		
			close = 1;	
			
		if(held & PAD_TRIGGER_L && held & PAD_TRIGGER_R)
			Options = DefaultOptions;
	
		if(pressed & PAD_BUTTON_A)
		{
#ifdef WII_VERSION		
			switch(sel)
#else
			switch(sel + 1)
#endif
			{
#ifdef WII_VERSION
				case 1:
					Options.TrapFilter = !Options.TrapFilter;
					SetVideoMode(vmode);
					SetupGX();
					break;
#endif	
				case 2:
					Options.FlickerFilter = !Options.FlickerFilter;	
					SetVideoMode(vmode);
					SetupGX();
					break;
					/*
				case 3:
					SetBilinearOption(Options.BilinearFiler+1);
					SetVideoMode(vmode);
					SetupGX();
					
					FreeImage(&back);
					back = LoadImage(HELPIMG, 0);
					if(back)
						back->alpha = 0xaa;
					
					ReleaseFont();
					LoadFont();
					
					break;
					*/
				case 3:
					if(VIDEO_HaveComponentCable() && !(Options.Activate480p && vmode >= VIDEO_480P))
						Options.Activate480p = !Options.Activate480p;
					break;
				case 4:	
					Options.EnablePAL = !Options.EnablePAL;
					break;
				case 5:	
					if(Options.EnablePAL)
						Options.EnablePALBG = !Options.EnablePALBG;
					break;
				case 6:	
					if(Options.EnablePAL)
						ChangePALBackgroundColor(title);
					break;
				case 7:	
					if(Options.EnablePAL)
					{
						Set576iLine23Option(Options.PALline23+1);
						if(IsPAL)	
						{
							SetVideoMode(vmode);
							SetupGX();
						}
					}
					break;
				case 8:	
					if(Options.EnablePAL)
					{
						EnableStretchedPALModes(!Options.PALScale576);
						if(IsPAL)	
						{
							SetVideoMode(vmode);
							SetupGX();
						}
					}
					break;
				case 9:
					Options.Enable720Stretch = !Options.Enable720Stretch;
					if(Options.Enable720Stretch)
						ShowStretchWarning();
					SetVideoMode(vmode);
					break;
				case 10:
					break;
				case 11:
					if(vmode == VIDEO_480P_SL)
						ToggleScanlineEvenOdd();
					break;
				case 12:
#ifdef WII_VERSION
					Options.SFCClassicController = !Options.SFCClassicController;
					break;
				case 13:
#endif
					close = 1;
					break;
				default:
					break;
			}
		}
	}
	
	FreeImage(&back);

	return;
}

void SelectVideoMode(ImagePtr title)
{
	int 		sel = 1, close = 0, mode = vmode;
	ImagePtr	back;
	
	back = LoadImage(HELPIMG, 0);
	if(!back)
		return;
		
	back->alpha = 0xaa;
		
	sel = mode + 1;
	while(!close && !EndProgram) 
	{		
		u8		r = 0xff;
		u8		g = 0xff;
		u8		b = 0xff;
		u8	 	c = 1;	 
		u16 	x = 70;
		u16 	y = 42;
		u32 	pressed = 0;
				
		StartScene();
				
		DrawImage(title);
		DrawImage(back);

		DrawStringS(x - 20, y, 0x00, 0xff, 0x00, "Please select the desired video mode"); y += 3*fh; 
		
		mode = vmode;
			
		DrawStringS(x - 10, y + (mode * fh) + ((mode >= VIDEO_288P) ? fh/2 : 0) + ((mode >= VIDEO_480P_SL) ? fh/2 - 1: 0),
					0x00, 0xff, 0x00, ">"); 
		
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "240p"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "480i scaled 240p assets (NTSC)"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "480i mixed 480p/240p assets (1:1/NTSC)"); y += fh; c++;
		
		y += fh/2;
		if(Options.EnablePAL)
		{
			if(!Options.PALScale576)
			{
				DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "288p"); y += fh; c++;
				DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "576i scaled 264/240p assets (PAL)"); y += fh; c++;
				DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "576i 528/480/240p assets (1:1/PAL)"); y += fh; c++;
			}
			else
			{
				DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "288p (stretched)"); y += fh; c++;
				DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "576i scaled 264/240p assets (stretched/PAL)"); y += fh; c++;
				DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "576i 528/480/240p assets (stretched/PAL)"); y += fh; c++;
			}
		}
		else
		{
			if(!Options.PALScale576)
			{
				DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "288p"); y += fh; c++;
				DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "576i scaled 264/240p assets (PAL)"); y += fh; c++;
				DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "576i 528/480/240p assets (1:1/PAL)"); y += fh; c++;
			}
			else
			{
				DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "288p (stretched)"); y += fh; c++;
				DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "576i scaled 264/240p assets (stretched/PAL)"); y += fh; c++;
				DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "576i 528/480/240p assets (stretched/PAL)"); y += fh; c++;
			}
		}
		
		y += fh/2;
		if(Options.Activate480p && VIDEO_HaveComponentCable())
		{
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "480p scaled 240p assets & scanlines"); y += fh; c++;
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "480p mixed 480p/240p assets (1:1)"); y += fh; c++;
		}
		else
		{
			DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "480p scaled 240p assets & scanlines"); y += fh; c++;
			DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "480p mixed 480p/240p assets (1:1)"); y += fh; c++;
		}
			
		DrawStringS(x, y + fh, r-0x40, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu");
		
#ifdef WII_VERSION
		DrawStringS(x+40, 200, r, g, b, "Press HOME for help");
#else
		DrawStringS(x+40, 200, r, g, b, "Press START for help");
#endif
		EndScene();
		
		ControllerScan();

		pressed = Controller_ButtonsDown(0);
		
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

		if(pressed & PAD_BUTTON_START)
		{
			HelpData = VIDEOMODEHELP;
			HelpWindow(title);
			HelpData = GENERALHELP;
		}		
			
		if ( pressed & PAD_BUTTON_B )
			close = 1;	
	
		if (pressed & PAD_BUTTON_A)
		{	  
			switch(sel)
			{			
				case 1:
					SetVideoMode(VIDEO_240P);
					SetupGX();
					break;
				case 2:
					SetVideoMode(VIDEO_480I_A240);
					SetupGX();
					break;
				case 3:
					SetVideoMode(VIDEO_480I);
					SetupGX();
					break;
				case 4:
					if(Options.EnablePAL)
					{
						SetVideoMode(VIDEO_288P);
						SetupGX();
					}
					break;
				case 5:
					if(Options.EnablePAL)
					{
						SetVideoMode(VIDEO_576I_A264);
						SetupGX();
					}
					break;
				case 6:
					if(Options.EnablePAL)
					{
						SetVideoMode(VIDEO_576I);
						SetupGX();
					}
					break;
				case 7:
					if(Options.Activate480p && VIDEO_HaveComponentCable())
					{
						SetVideoMode(VIDEO_480P_SL);
						SetupGX();
					}
					break;
				case 8:
					if(Options.Activate480p && VIDEO_HaveComponentCable())
					{
						SetVideoMode(VIDEO_480P);
						SetupGX();
					}
					break;		
				case 9:
					if(Controller_ButtonsHeld(0) & PAD_TRIGGER_L)
						ShowVideoData();
					else
						close = 1;
					break;
				default:
					break;
			}
		}
	}
	FreeImage(&back);

	return;
}

void ChangePALBackgroundColor(ImagePtr title)
{
	int 		sel = 1, close = 0;	
	ImagePtr	back, block, blackblock;
	
	back = LoadImage(HELPIMG, 0);
	if(!back)
		return;
		
	block = LoadImage(WHITEIMG, 0);
	if(!block)
		return;
		
	blackblock = LoadImage(WHITEIMG, 0);
	if(!blackblock)
		return;
		
	block->x = 160;
	block->y = 92;
	block->w = 20;
	block->h = 20;
	
	blackblock->x = block->x - 1;
	blackblock->y = block->y - 1;
	blackblock->w = block->w + 2;
	blackblock->h = block->h + 2;
	blackblock->r = 0;
	blackblock->g = 0;
	blackblock->b = 0;
		
	back->alpha = 0xaa;
		
	while(!close && !EndProgram) 
	{		
		u8		r = 0xff;
		u8		g = 0xff;
		u8		b = 0xff;
		u8	 	c = 1;
		u16 	x = 80;
		u16 	y = 70;
		u32 	pressed = 0, held = 0;
		char	color[80];
		
		block->r = Options.PalBackR;
		block->g = Options.PalBackG;
		block->b = Options.PalBackB;

		StartScene();

		DrawImage(title);
		DrawImage(back);
		DrawImage(blackblock);
		DrawImage(block);

		DrawStringS(x - 20, y, 0x00, 0xff, 0x00, "PAL background color"); y += 2*fh; 
		
		sprintf(color, "Red:	0x%X", Options.PalBackR);
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, color); y += fh; c++;		
		sprintf(color, "Green:	0x%X", Options.PalBackG);
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, color); y += fh; c++;		
		sprintf(color, "Blue:	0x%X", Options.PalBackB);
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, color); y += fh; c++;	

		DrawStringS(x, y + fh, r, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Options Menu"); 	
		
		DrawStringS(x-40, y + 6*fh, r-0x30, g-0x30, b-0x30, "The background color is used to fill the screen");
		DrawStringS(x-40, y + 7*fh, r-0x30, g-0x30, b-0x30, "to the selected PAL resolution when needed");
		
#ifdef WII_VERSION
		if(sel != c)
		{
			if(ControllerType != ControllerGC && !Options.SFCClassicController)
				DrawStringS(x-40, y + 4*fh, r, g, b, "Adjust with + and - buttons or Left & Right");
			else
				DrawStringS(x-40, y + 4*fh, r, g, b, "Adjust with L and R triggers or Left & Right");
		}
#else
		if(sel != c)
			DrawStringS(x-40, y + 4*fh, r, g, b, "Adjust with L and R triggers or Left & Right");
#endif

		EndScene();
		
		ControllerScan();

		pressed = Controller_ButtonsDown(0);
		held = Controller_ButtonsHeld(0);
		
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
				case 4:
					close = 1;
					break;
			}
		}
		
		 
		if ((pressed & PAD_TRIGGER_L || held & PAD_BUTTON_LEFT) && sel == 1)
		{
			if(Options.PalBackR - 1 >= 0)
				Options.PalBackR --;
		}
		
		if ((held & PAD_TRIGGER_R || pressed & PAD_BUTTON_RIGHT) && sel == 1)
		{
			if(Options.PalBackR + 1 <= 255)
				Options.PalBackR ++;
		}
		
		if ((held & PAD_TRIGGER_L || pressed & PAD_BUTTON_LEFT) && sel == 2)
		{
			if(Options.PalBackG - 1 >= 0)
				Options.PalBackG --;
		}
		
		if ((held & PAD_TRIGGER_R || pressed & PAD_BUTTON_RIGHT) && sel == 2)
		{
			if(Options.PalBackG + 1 <= 255)
				Options.PalBackG ++;
		}
		
		if ((held & PAD_TRIGGER_L || pressed & PAD_BUTTON_LEFT) && sel == 3)
		{
			if(Options.PalBackB - 1 >= 0)
				Options.PalBackB --;
		}
		
		if ((held & PAD_TRIGGER_R || pressed & PAD_BUTTON_RIGHT) && sel == 3)
		{
			if(Options.PalBackB + 1 <= 255)
				Options.PalBackB ++;
		}
	}
	FreeImage(&back);		
	FreeImage(&block);
	FreeImage(&blackblock);
}

extern GXRModeObj *mvmode;
extern GXRModeObj *rmode;

void ShowVideoData()
{
	u16 	x = 80;
	u16 	y = 80;
	u32 	pressed = 0, done = 0;
	char	data[100];
	
	/*
	u8 	aa
	u16 	efbHeight
	u16 	fbWidth
	u8 	field_rendering
	u8 	sample_pattern [12][2]
	u8 	vfilter [7]
	u16 	viHeight
	u32 	viTVMode
	u16 	viWidth
	u16 	viXOrigin
	u16 	viYOrigin
	u16 	xfbHeight
	u32 	xfbMode
	*/
	while(!done)
	{
		x = 40;
		y = 40;
		
		StartScene();
		
		sprintf(data, "viTVMode: %X", rmode->viTVMode);
		DrawStringS(x, y, 0xff, 0xff,	0xff, data); y += fh;
		
		sprintf(data, "xfbMode: %X", rmode->xfbMode);
		DrawStringS(x, y, 0xff, 0xff,	0xff, data); y += fh;
				
		sprintf(data, "efbHeight: %u", rmode->efbHeight);
		DrawStringS(x, y, 0xff, 0xff,	0xff, data); y += fh;
				
		sprintf(data, "viHeight: %u", rmode->viHeight);
		DrawStringS(x, y, 0xff, 0xff,	0xff, data); y += fh;
				
		sprintf(data, "fbWidth: %u", rmode->fbWidth);
		DrawStringS(x, y, 0xff, 0xff,	0xff, data); y += fh;
						
		sprintf(data, "viWidth: %u", rmode->viWidth);
		DrawStringS(x, y, 0xff, 0xff,	0xff, data); y += fh;
				
		sprintf(data, "field_rendering: %u", rmode->field_rendering);
		DrawStringS(x, y, 0xff, 0xff,	0xff, data); y += fh;
				
		sprintf(data, "viXOrigin: %u", rmode->viXOrigin);
		DrawStringS(x, y, 0xff, 0xff,	0xff, data); y += fh;
				
		sprintf(data, "viYOrigin: %u", rmode->viYOrigin);
		DrawStringS(x, y, 0xff, 0xff,	0xff, data); y += fh;
		
		sprintf(data, "dW: %d", dW);
		DrawStringS(x, y, 0xff, 0xff,	0xff, data); y += fh;
		sprintf(data, "dH: %d", dH);
		DrawStringS(x, y, 0xff, 0xff,	0xff, data); y += fh;
				
		EndScene();		
			
		ControllerScan();
		pressed = Controller_ButtonsDown(0);
		
		if ( pressed & PAD_BUTTON_B ) 		
			done = 1;	
	}
}

void ShowVideoWarning(ImagePtr screen)
{	
	u32 		pressed = 0, done = 0;	
	ImagePtr	Back = NULL;
	
	Back = LoadImage(FLOATMENUIMG, 0);
	if(Back)
	{	
		Back->x = (dW - Back->w) / 2;
		Back->y = (dH - Back->h) / 2;
		
		Back->alpha = 0xaa;
	}
	
	while(!done)
	{			
		u8		r = 0xff;
		u8		g = 0xff;
		u8		b = 0xff;
		u16 	x = Back->x + 20;
		u16 	y = Back->y + 10;
				
		StartScene();
		
		DrawImage(screen);
		DrawImage(Back);

		DrawStringS(x, y, 0x00, 0xff, 0x00, VERSION_NUMBER); y += 2*fh;
		
		x -= 10;
		DrawStringS(x, y, r, 0, 0, "These settings"); y += fh;
		DrawStringS(x, y, r, 0, 0, "will be ignored:"); y += fh;
				
		if(OffsetH != 0)
		{
			DrawStringS(x, y, r, g, b, "- Horizontal Offset"); y += fh;
		}
		
		if(AspectRatio != 0)
		{
			DrawStringS(x, y, r, g, b, "- 16:9 Aspect Ratio"); y += fh;
		}
				
		DrawStringS(x, Back->y + fh*12, 0, g, b, "Press B to continue"); 
		
		EndScene();		
			
		ControllerScan();
		pressed = Controller_ButtonsDown(0);
		
		if ( pressed & PAD_BUTTON_B )
			done = 1;	
	}
	FreeImage(&Back);
}

void ShowStretchWarning()
{	
	DrawMessageBox(NULL, "Horizontal stretch renders geometry\npatterns useless");
}

void ShowPALBGWarning()
{	
	DrawMessageBox(NULL, "PAL Background disabled for monoscope");
}

void DrawMessageBox(ImagePtr scene, char *msg)
{	
	u32 		pressed = 0, done = 0;	
	ImagePtr	Back = NULL;
	
	if(scene)
	{	
		scene->r = 0x77;
		scene->g = 0x77;
		scene->b = 0x77;
	}
	
	Back = LoadImage(MSGIMG, 0);
	if(Back)
		Back->alpha = 0xaa;
	
	while(!done)
	{			
		u8		r = 0xff;
		u8		g = 0xff;
		u8		b = 0xff;
		u16 	x = Back->x + 50;
		u16 	y = Back->y + 90;
				
		StartScene();
		
		if(scene)
			DrawImage(scene);
		DrawImage(Back);

		DrawStringS(x, y, r, g, b, msg);
				
		DrawStringC(164, 0, g, b, "Press B to continue"); 
		
		EndScene();		
			
		ControllerScan();
		pressed = Controller_ButtonsDown(0);
		
		if ( pressed & PAD_BUTTON_B ) 
			done = 1;
	}
	
	FreeImage(&Back);
	if(scene)
	{
		scene->r = 0xff;
		scene->g = 0xff;
		scene->b = 0xff;
	}
}


void DrawCredits(ImagePtr Back)
{
	int 		done = 0, r, g, b;
	u32 		pressed = 0;
	char		data[50];
	ImagePtr	qr = NULL;
#ifdef WII_VERSION
	u8 			shopcode = 0;
	char		shop[100];	
	
	
	if (CONF_GetShopCode(&shopcode) >= 0) {
		sprintf(shop, "Shop: %s\n", (strlen(CONF_CountryCodes[shopcode]) ? CONF_CountryCodes[shopcode] : "Unknown Country"));
	} else {
		sprintf(shop, "Error getting shop code!\n");
	}
#endif

	if(Back)
	{
		r = Back->r;
		g = Back->b;
		b = Back->g;

		Back->r = 0x64;
		Back->g = 0x64;
		Back->b = 0x64;
	}
	
	qr = LoadImage(QRIMG, 0);
	if(qr) {
		qr->x = 260;
		qr->y = 165;
	}
	
	while(!done && !EndProgram)  
	{
		int x = 20, x2 = 180, y = 10, y2 = 0;

		StartScene();

		DrawImage(Back);
		DrawImage(qr);
		
		DrawStringS(x, y, 0x00, 0xff, 0x00, "Code and Patterns:"); y += fh; 
		DrawStringS(x, y, 0xff, 0xff, 0xff, "Artemio Urbina"); y += fh; 
		sprintf(data, "@Artemio (twitter)");

		DrawStringS(x, y, 0x00, 0xff, 0x00, "Support and suggestions:"); y += fh; 
		DrawStringS(x+5, y, 0xff, 0xff, 0xff, data); y += fh; 
		
		y += fh; 
		y2 = y;
		DrawStringS(x, y, 0x00, 0xff, 0x00, "SDK:"); y += fh; 
		DrawStringS(x+5, y, 0xff, 0xff, 0xff, "libogc2/devkitPPC"); y += fh;
		DrawStringS(x2, y2, 0x00, 0xff, 0x00, "Monoscope Pattern:"); y2 += fh; 
		DrawStringS(x2+5, y2, 0xff, 0xff, 0xff, "Keith Raney (@khmr33)"); y2 += fh;
		
		DrawStringS(x, y, 0x00, 0xff, 0x00, "Donna Art:"); y += fh; 
		DrawStringS(x+5, y, 0xff, 0xff, 0xff, "Jose Salot (@pepe_salot)"); y += fh;
		DrawStringS(x2, y2, 0x00, 0xff, 0x00, "Menu Pixel Art:"); y2 += fh; 
		DrawStringS(x2+5, y2, 0xff, 0xff, 0xff, "Asher"); y2 += fh;
		
		DrawStringS(x, y, 0x00, 0xff, 0x00, "Advisor:"); y += fh; 
		DrawStringS(x+5, y, 0xff, 0xff, 0xff, "Fudoh"); y += fh; 
		DrawStringS(x2, y2, 0x00, 0xff, 0x00, "Collaboration:"); y2 += fh; 
		DrawStringS(x2+5, y2, 0xff, 0xff, 0xff, "shmups regulars"); y2 += fh;
 
		DrawStringS(x, y, 0x00, 0xff, 0x00, "PAL testing:"); y += fh; 
		DrawStringS(x+5, y, 0xff, 0xff, 0xff, "Yamato"); y += fh; 
		DrawStringS(x2, y2, 0x00, 0xff, 0x00, "MDFourier help:"); y2 += fh; 
		DrawStringS(x2+5, y2, 0xff, 0xff, 0xff, "Extrems"); y2 += fh;
		
		DrawStringS(x2, y2, 0x00, 0xff, 0x00, "HCFR Data & Tests:"); y2 += fh;
		DrawStringS(x2+5, y2, 0xff, 0xff, 0xff, "Dan Mons (@_daemons)"); y2 += fh;
		DrawStringS(x2+5, y2, 0xff, 0xff, 0xff, "& Keith Raney"); y2 += fh;
		DrawStringS(x2, y2, 0x00, 0xff, 0x00, "160p test Suite:"); y2 += fh;
		DrawStringS(x2+5, y2, 0xff, 0xff, 0xff, "pinobatch"); y2 += fh;
#ifdef GC_VERSION
		DrawStringS(x, y, 0x00, 0xff, 0x00, "GameCube Tools:"); y += fh; 
		DrawStringS(x+5, y, 0xff, 0xff, 0xff, "Rolman"); y += fh;
#endif
		y += fh; 
		DrawStringS(x, y, 0x00, 0xff, 0x00, "Info on using this suite:"); y += fh; 
		DrawStringS(x+5, y, 0xff, 0xff, 0xff, "http://junkerhq.net/240p/"); y += 3*fh; 

		DrawStringS(x, y, 0x00, 0xba, 0xba, "This program is free Software.");	y += fh;
		DrawStringS(x, y, 0x00, 0xba, 0xba, "Source code is available under GPL.");  y += fh;
#ifdef WII_VERSION
		DrawStringS(x, y, 0x00, 0xba, 0xba, "Includes libcheckregion."); y += fh;
#endif
		DrawStringS(200, y, 0x00, 0xba, 0xba, "Dedicated to Elisa.");
		
		y = 10;
		
		DrawStringS(200, y, 0x0f, 0xff, 0xff, VERSION_NUMBER); y += fh;
		DrawStringS(200, y, 0x0f, 0xff, 0xff, VERSION_DATE); y += 2*fh;

#ifdef WII_VERSION
		DrawStringS(200, y, 0xee, 0xee, 0xee, shop); y += fh;
		DrawStringS(200, y, 0xee, 0xee, 0xee, wiiregion); y += fh;
#endif

		EndScene();
		
		ControllerScan();
		pressed = Controller_ButtonsDown(0);
				
		if (pressed & PAD_TRIGGER_R)
		{
			int 		check = 0;
			u32			press;
			ImagePtr	nish;
			
#ifdef WII_VERSION
			nish = LoadImage(NISHIMG, 0);
#else
			nish = LoadImage(NISHGCIMG, 0);
#endif
			if(nish)
			{
				while(!check) 
				{		
					check = 1;
					StartScene();
					DrawImage(nish);
					EndScene();
					ControllerScan();
					press = Controller_ButtonsHeld(0);
							
					if (press & PAD_TRIGGER_R)
						check =	0;
				}
				FreeImage(&nish);
			}
		}
		
		if (pressed & PAD_BUTTON_B)
			done =	1;
	}
	
	if(Back)
	{
		Back->r = r;
		Back->g = g;
		Back->b = b;
	}
	FreeImage(&qr);
}

void DrawIntro()
{
	u32				counter, frames = 60;
	int				delta;
	ImagePtr		black;

	black = LoadImage(BLACKIMG, 1);
	if(!black)
		return;

	black->alpha = 0xff;
	delta = 1;
	black->w = dW;
	black->h = dH;
	black->layer = 5.0f;
		
	for(counter = 0; counter < frames*2; counter ++)
	{
		black->alpha -= delta;
		if(black->alpha < 0x00)
			black->alpha = 0x00;

		StartScene();
	
		DrawStringS(120, 115, 0xff, 0xff, 0xff, "KORDAMP PRESENTS");
		DrawImage(black);
		
		EndScene();

		if(counter == frames)
			delta *= -1;
	}
	FreeImage(&black);
}

int SelectMenu(char *title, fmenudata *menu_data, int num_options, int selected_option)
{
	return(SelectMenuEx(title, menu_data, num_options, selected_option, NULL));
}

int SelectMenuEx(char *title, fmenudata *menu_data, int num_options, int selected_option, char **helpfile)
{
	int 		sel = selected_option, close = 0, value = MENU_CANCEL;
	ImagePtr	Back = NULL;
	
	Back = LoadImage(FLOATMENUIMG, 0);
	if(Back)
	{
		Back->x = (dW - Back->w) / 2;
		Back->y = (dH - Back->h) / 2;
		
		Back->alpha = 0xaa;
	}
	   
	while(!close) 
	{		
		u8		r = 0xff;
		u8		g = 0xff;
		u8		b = 0xff;
		u8	 	c = 1, i = 0;
		u16 	x = Back ? Back->x + 20 : 100;
		u16 	y = Back ? Back->y + 10 : 60;
		u32 	pressed = 0;
		
		StartScene();
		
		if(Back)
			DrawImage(Back);

		DrawStringS(x, y, 0x00, 0xff, 0x00, title); y += 3*fh;

		if(num_options > 6)
			y -= fh;	
		
		for(i = 0; i < num_options; i++)
		{
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, menu_data[i].option_text);
			y += fh; c++;		
		}
		
		y += fh;
		
		if(num_options <= 6)
			y += fh;
		
		DrawStringS(x+2*fw, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Close Menu");
		
		if(helpfile)
			DrawStringS(x-2*fw, y+2*fh, 0xff, 0xff, 0xff, "Press #YSTART#Y for help");

		EndScene();
		
		ControllerScan();

		pressed = Controller_ButtonsDown(0);
		
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

		if (pressed & PAD_BUTTON_B || 
			(!helpfile && pressed & PAD_BUTTON_START))
		{
			close = 1;	
			value = MENU_CANCEL;
		}
		
		if(helpfile && pressed & PAD_BUTTON_START)
		{
			HelpData = helpfile;
			HelpWindow(NULL);
			HelpData = GENERALHELP;
		}
	
		if (pressed & PAD_BUTTON_A)
		{	  
			close = 1;

			if(sel == c)
				value = MENU_CANCEL;
			else
				value = menu_data[sel - 1].option_value;
		}		
	}
	
	if(Back)
		FreeImage(&Back);

	return value;
}

void SD_blink_cycle(ImagePtr sd)
{
	static int blink_counter = 0;
	static int is_blinking = 0;
	
	if(!sd)
		return;
	
	if(sd_b1) {
		sd_b1->x = sd->x+16;
		sd_b1->y = sd->y+32;
	}
	
	if(sd_b2) {
		sd_b2->x = sd->x+16;
		sd_b2->y = sd->y+32;
	}
	
	blink_counter++;	
	if(sd_b1 && sd_b2 && blink_counter > 230)
	{
		if(!is_blinking)
		{
			if(rand() % 100 < 98)	// 2% chance every frame after 240
			{
				is_blinking = 1;
				blink_counter = 230;
				DrawImage(sd_b1);
			}
		}
		else
		{
			if(blink_counter >= 232 && blink_counter < 234)
				DrawImage(sd_b2);
				
			if(blink_counter >= 234 && blink_counter < 238)
				DrawImage(sd_b1);
	
			if(blink_counter >= 238)
			{	
				blink_counter = 0;
				is_blinking = 0;
			}
		}
	}
}

#ifdef WII_VERSION

void GetWiiRegion()
{
	s32 ret;
	char serialcode[SERIALCODE_SIZE] = "???\0";
	char model[MODEL_SIZE] = {0};
	
	ret = CONF_GetSerialCode(serialcode);
	switch (ret) {
		case CONF_CODE_JPN:
			// LJF - Confirmed
			sprintf(wiiregion, "Japanese Wii");
			break;
		case CONF_CODE_USA:
			// LU - Confirmed
			sprintf(wiiregion, "USA Wii");
			break;
		case CONF_CODE_EURH:
			// LEH - Confirmed
		case CONF_CODE_EURM:
			// LEM - Confirmed
		case CONF_CODE_EURF:
			// LEF - Confirmed
			sprintf(wiiregion, "European Wii");
			break;
		case CONF_CODE_KOR:
			// LKM - Confirmed
			sprintf(wiiregion, "Korean Wii");
			break;
		case CONF_CODE_AUS:
			// LAH - Confirmed
			sprintf(wiiregion, "n Australian Wii");
			break;
		case CONF_CODE_USAK:
			// KU - Confirmed (thanks LiNkZoR)
			sprintf(wiiregion, "new USA Wii");
			break;
		case CONF_CODE_EURHK:
			// KEH - Confirmed (thanks JohnyNyga)
		case CONF_CODE_EURMK:
			// KEM - Confirmed (thanks VampireLordAlucard)
		case CONF_CODE_EURFK:
			// KEF - Confirmed (thanks sureiya)
			sprintf(wiiregion, "new European Wii");
			break;
		case CONF_CODE_AUSK:
			// KAM - Confirmed (thanks Drexyl)
			sprintf(wiiregion, "new Australian Wii");
			break;
		case CONF_CODE_DLPH:
			// LE - Confirmed by myself
			sprintf(wiiregion, "Running Dolphin");
			break;
		case CONF_CODE_VJPNI:
			sprintf(wiiregion, "n 8GB Japanese Wii U");
			break;
		case CONF_CODE_VJPNO:
			sprintf(wiiregion, "32GB Wii U");
			break;
		case CONF_CODE_VUSAI:
			sprintf(wiiregion, "8GB USA Wii U");
			break;
		case CONF_CODE_VUSAO:
			sprintf(wiiregion, "32GB USA Wii U");
			break;
		case CONF_CODE_VEURHI:
		case CONF_CODE_VEURMI:
		case CONF_CODE_VEURFI:
			sprintf(wiiregion, "n 8GB European Wii U");
			break;
		case CONF_CODE_VEURHO:
		case CONF_CODE_VEURMO:
		case CONF_CODE_VEURFO:
			sprintf(wiiregion, "32GB European Wii U");
			break;
		case CONF_CODE_VKORI:
			sprintf(wiiregion, "n 8GB Korean Wii U");
			break;
		case CONF_CODE_VKORO:
			sprintf(wiiregion, "32GB Korean Wii U");
			break;
		case CONF_CODE_VAUSI:
			sprintf(wiiregion, "n 8GB Australian Wii U");
			break;
		case CONF_CODE_VAUSO:
			sprintf(wiiregion, "32GB Australian Wii U");
			break;
		case CONF_EBADVALUE:
			if (!CONF_GetModel(model))
				sprintf(wiiregion, "Unknown SC %s model %s", serialcode, model);
			else
				sprintf(wiiregion, "Unknown SC %s", serialcode);
			break;
		default:
			sprintf(wiiregion, "Error getting region");	
			break;
	}
}

#endif