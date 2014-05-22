/* 
 * 240p Test Suite
 * Copyright (C)2011 Artemio Urbina
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

#include <kos.h>
#include <stdlib.h>
#include <dc/sound/sound.h>
#include <dc/sound/sfxmgr.h>

#include "image.h"
#include "font.h"
#include "vmodes.h"
#include "vmu.h"

#include "controller.h"
#include "tests.h"
#include "patterns.h"

#include "help.h"
#include "menu.h"
#include "settings.h"

/* romdisk */
extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);
KOS_INIT_FLAGS(INIT_DEFAULT);

int	region = 0;

void TestPatternsMenu(ImagePtr title, ImagePtr sd);
void DrawCredits(ImagePtr back);
void DrawIntro();

int main(void)
{
	int 			done = 0, sel = 1, joycnt = 0;
	uint16			pressed, start = 1;
	ImagePtr		title, sd;
	controller	*st;

	/* init kos	*/
	vcable = vid_check_cable();
	if(vcable != CT_VGA)
		vid_set_mode(DM_320x240_NTSC, PM_RGB565); // PM_RGB555 PM_RGB565 PM_RGB888

	pvr_init_defaults();
	region = flashrom_get_region();

	// Disable deflicker filter, 
	if(PVR_GET(PVR_SCALER_CFG) != 0x400)
	{
		dbglog(DBG_KDEBUG, "Disabling pvr deflicker filter for 240p tests\n");
		PVR_SET(PVR_SCALER_CFG, 0x400);
	}

		// Turn off texture dithering
	if(PVR_GET(PVR_FB_CFG_2) != 0x00000001)
	{
		dbglog(DBG_KDEBUG, "Disabling pvr dithering for 240p tests\n");
		PVR_SET(PVR_FB_CFG_2, 0x00000001);
	}

	 // Default to 480p when VGA is connected.
	 if(vcable == CT_VGA)
		ChangeResolution(NATIVE_640);

start:
	// Check cable again in case it was changed on the fly
	vcable = vid_check_cable();

	if(!settings.drawborder) // Draw back video signal limits?
		vid_border_color(0, 0, 0);
	else
		vid_border_color(255, 255, 255);

	if(!settings.drawpvrbg)
		pvr_set_bg_color(0.0f, 0.0f, 0.0f);
	else
		pvr_set_bg_color(0.0f, 1.0f, 0.0f);
		
	InitImages();
	LoadFont();
	LoadScanlines();
	title = LoadKMG("/rd/back.kmg.gz", 1);
	sd = LoadKMG("/rd/SD.kmg.gz", 0);
	if(sd)
	{
		sd->x = 221;
		sd->y = 94;
	}
	
	if(start)
	{
		DrawIntro();
		start = 0;
	}
	while(!done && !EndProgram) 
	{
		char	res[40];
		float 	r = 1.0f;
		float 	g = 1.0f;
		float 	b = 1.0f;
		int   	c = 1;				    
		float 	x = 40.0f;
		float 	y = 55.0f;
#ifdef USE_FFTW
		maple_device_t *sip = NULL;
#endif
				
		StartScene();
		DrawImage(title);
		DrawImage(sd);
		
#ifdef USE_FFTW
		sip = maple_enum_type(0, MAPLE_FUNC_MICROPHONE);
#endif

		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Test Patterns"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Drop Shadow Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Striped Sprite Test"); y += fh; c++;    
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Lag Test"); y += fh; c++;
#ifdef USE_FFTW
		if(sip)
		{
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Microphone Lag Test"); y += fh; c++;
		}
		else
		{
#endif
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Manual Lag Test"); y += fh; c++;
#ifdef USE_FFTW
		}
#endif
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Scroll Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Grid Scroll Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Horizontal Stripes"); y += fh; c++;    
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Checkerboard"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Backlit Zone Test"); y += fh; c++;
		if(vcable == CT_VGA)
		{
			DrawStringS(x, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, "Alternating 240p/480i Test"); y += fh; c++;
		}
		else
		{
			DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Alternating 240p/480i Test"); y += fh; c++;
		}
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Sound Test"); y += fh; c++;

		switch(vmode)
		{
			case NATIVE_320:
				if(vcable != CT_VGA)
					sprintf(res, "Video: 240p");
				else
					sprintf(res, "Video: 480p linedoubled 240p");
				break;
			case NATIVE_640:
				sprintf(res, "Video: 480i no scaling/240p assets");
				break;
			case FAKE_640:
				sprintf(res, "Video: 480i scaled 240p");
				break;
			case NATIVE_640_FS:
				if(vcable != CT_VGA)
					sprintf(res, "Video: 480i no scaling/mixed assets");
				else
					sprintf(res, "Video: 480p no scaling/mixed assets");
				break;
			case FAKE_640_SL:
				sprintf(res, "Video: 480p %s scanlines %0.0f%%", ScanlinesEven() ? "even" : "odd", GetScanlineIntensity());
				break;
		}
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, res); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Help"); y += fh; c++;
		DrawStringS(x, y + fh, r, sel == c ? 0 : g, sel == c ? 0 : b, "Credits"); y += fh; 

#ifdef VGA_SETTINGS
		if(vmode == NATIVE_640_FS && vcable == CT_VGA)
		{
			c++;
			DrawStringS(x, y +fh, r, sel == c ? 0 : g,	sel == c ? 0 : b, "VGA Settings"); 
		}    
#endif

		r = 0.8f;
		g = 0.8f;
		b = 0.8f;
		switch(vcable)
		{
			case CT_RGB:
				DrawStringS(265.0f, 225.0f, r, g, b, "RGB");
				break;
			case CT_VGA:
				DrawStringS(265.0f, 225.0f, r, g, b, "VGA");
				break;
			case CT_COMPOSITE:
				DrawStringS(235.0f, 225.0f, r, g, b, "Composite");
				break;
		}
		
		switch(region)
		{
			case FLASHROM_REGION_UNKNOWN:
				DrawStringS(265.0f, 215.0f, r, g, b, "??????");
				break;
			case FLASHROM_REGION_JAPAN:
				DrawStringS(265.0f, 215.0f, r, g, b, "Japan");
				break;
			case FLASHROM_REGION_US:
				DrawStringS(265.0f, 215.0f, r, g, b, "USA");
				break;
			case FLASHROM_REGION_EUROPE:
				DrawStringS(265.0f, 215.0f, r, g, b, "Europe");
				break;
		}
		
		EndScene();
		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_START)
				ShowMenu(GENERALHELP);

			if (pressed & CONT_X)
				ToggleScanlineEvenOdd();

			if (pressed & CONT_X && pressed & CONT_Y)
			{
				settings.drawborder = !settings.drawborder;
				settings.drawpvrbg = !settings.drawpvrbg;
				FreeImage(&title);		
				FreeImage(&sd);		
				ReleaseScanlines();
				ReleaseFont();
				goto start;
			}

			if (pressed & CONT_DPAD_RIGHT && st->buttons & CONT_Y)
				RaiseScanlineIntensity();

			if (pressed & CONT_DPAD_LEFT && st->buttons & CONT_Y)
				LowerScanlineIntensity();

			if (pressed & CONT_DPAD_UP)
			{
				sel --;
				if(sel < 1)
					sel = c;				
			}
			
			if (pressed & CONT_DPAD_DOWN)
			{
				sel ++;
				if(sel > c)
					sel = 1;				
			}
			
			if(st->joyy != 0)
			{
				if(++joycnt > 5)
				{
					if(st->joyy > 0)
						sel ++;
					if(st->joyy < 0)
						sel --;
		
					if(sel < 1)
						sel = c;
					if(sel > c)
						sel = 1;					
					joycnt = 0;
				}
			}
			else
				joycnt = 0;
			
			if (pressed & CONT_A)
			{
				switch(sel)
				{
					case 1:
						TestPatternsMenu(title, sd);
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
#ifdef USE_FFTW
						if(sip)
							SIPLagTest();
						else
#endif
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
						if(vcable != CT_VGA)
						{
							FreeImage(&title);		
							FreeImage(&sd);		
							Alternate240p480i();
							ReleaseScanlines();
							ReleaseFont();
							goto start;
						}
						break;
					case 12:
						SoundTest();
						break;
					case 13:
					/*
						ChangeResolution();
						FreeImage(&title);		
						FreeImage(&sd);		
						ReleaseScanlines();
						ReleaseFont();
						// we need to reload textures and stuff..
						// not pretty, but "clean"
						goto start;
					*/
						break;
					case 14:
						HelpWindow(GENERALHELP, title);
						break;
					case 15:
						DrawCredits(title);
						break;
					case 16:
						//Settings(title);
						TestVideoMode();
						break;          						
				} 					
				updateVMU("240p Test", "", 1);				
			}
		}
		updateVMU("240p Test", "", 0);
	}

	updateVMU(" Goodbye ", " m(_ _)m ", 1);

	FreeImage(&title);		
	FreeImage(&sd);		
	ReleaseScanlines();
	ReleaseFont();
	CleanImages();
#ifndef SERIAL
	arch_menu();
#endif
	return 0;
}

void TestPatternsMenu(ImagePtr title, ImagePtr sd)
{
	int 			done = 0, sel = 1, joycnt = 0;
	uint16			pressed;		
	controller		*st;

	while(!done && !EndProgram) 
	{		
		float 	r = 1.0f;
		float 	g = 1.0f;
		float 	b = 1.0f;
		int			c = 1;				
		float 	x = 40.0f;
		float 	y = 55.0f;
				
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
		DrawStringS(x, y + fh, r, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); y += fh; 

		r = 0.8f;
		g = 0.8f;
		b = 0.8f;
		switch(vcable)
		{
			case CT_RGB:
				DrawStringS(265.0f, 225.0f, r, g,	b, "RGB");
				break;
			case CT_VGA:
				DrawStringS(265.0f, 225.0f, r, g,	b, "VGA");
				break;
			case CT_COMPOSITE:
				DrawStringS(215.0f, 225.0f, r, g,	b, "Composite");
				break;
		}
		
		switch(region)
		{
			case FLASHROM_REGION_UNKNOWN:
				DrawStringS(265.0f, 215.0f, r, g, b, "??????");
				break;
			case FLASHROM_REGION_JAPAN:
				DrawStringS(265.0f, 215.0f, r, g, b, "Japan");
				break;
			case FLASHROM_REGION_US:
				DrawStringS(265.0f, 215.0f, r, g, b, "USA");
				break;
			case FLASHROM_REGION_EUROPE:
				DrawStringS(265.0f, 215.0f, r, g, b, "Europe");
				break;
		}
		
		EndScene();

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_START)
				ShowMenu(GENERALHELP);

			if (pressed & CONT_DPAD_RIGHT && st->buttons & CONT_Y)
				RaiseScanlineIntensity();

			if (pressed & CONT_DPAD_LEFT && st->buttons & CONT_Y)
				LowerScanlineIntensity();

			if (pressed & CONT_DPAD_UP)
			{
				sel --;
				if(sel < 1)
					sel = c;				
			}
		
			if (pressed & CONT_DPAD_DOWN)
			{
				sel ++;
				if(sel > c)
					sel = 1;				
			}
		
			if(st->joyy != 0)
			{
				if(++joycnt > 5)
				{
					if(st->joyy > 0)
						sel ++;
					if(st->joyy < 0)
						sel --;
		
					if(sel < 1)
						sel = c;
					if(sel > c)
						sel = 1;					
					joycnt = 0;
				}
			}
			else
				joycnt = 0;
		
			if (pressed & CONT_B)			
				done = 1;			
		
			if (pressed & CONT_A)
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
						done = 1;
						break;
				} 												
				updateVMU("Patterns", "", 1);
			}			
		}

		updateVMU("Patterns", "", 0);
	}

	return;
}

void DrawCredits(ImagePtr back)
{
	int 		done = 0;
	uint16		pressed, counter = 1;		
	char 		name[50], title[50];
	controller	*st;

	updateVMU("	Credits", "", 1);
	while(!done && !EndProgram) 
	{
		int x = 30, y = 52;

		StartScene();
		DrawImage(back);

		if(counter == 1)
                {
                        sprintf(title, "Code and Patterns:");
                        sprintf(name, "Artemio Urbina");
                }
                if(counter == 60*2)
                {
                        sprintf(title, "Support and suggestions:");
                        sprintf(name, "aurbina@junkerhq.net");
                }
                if(counter == 60*4)
                        sprintf(name, "@Artemio (twitter)");
                if(counter == 60*8)
                        counter = 0;

		DrawStringS(x, y, 0.0, 1.0, 0.0, title); y += fh; 
		DrawStringS(x+5, y, 1.0, 1.0, 1.0, name); y += fh; 
		DrawStringS(x, y, 0.0, 1.0, 0.0, "SDK:"); y += fh; 
		DrawStringS(x+5, y, 1.0, 1.0, 1.0, "KallistiOS"); y += fh; 
		DrawStringS(x, y, 0.0, 1.0, 0.0, "SDK Assistance:"); y += fh; 
		DrawStringS(x+5, y, 1.0, 1.0, 1.0, "BlueCrab"); y += fh; 
		DrawStringS(x, y, 0.0, 1.0, 0.0, "Menu Pixel Art:"); y += fh; 
		DrawStringS(x+5, y, 1.0, 1.0, 1.0, "Asher"); y += fh; 		
		DrawStringS(x, y, 0.0, 1.0, 0.0, "Advisor:"); y += fh; 
		DrawStringS(x+5, y, 1.0, 1.0, 1.0, "Fudoh"); y += fh; 
		DrawStringS(x, y, 0.0, 1.0, 0.0, "Collaboration:"); y += fh; 
		DrawStringS(x+5, y, 1.0, 1.0, 1.0, "Konsolkongen & shmups regulars"); y += fh; 
		DrawStringS(x, y, 0.0, 1.0, 0.0, "Info on using this suite:"); y += fh; 
		DrawStringS(x+5, y, 1.0, 1.0, 1.0, "http://junkerhq.net/240p/"); y += fh; 

		DrawStringS(x, y, 0.0, .75, .75, "This program is free Software");  y += fh;
                DrawStringS(x, y, 0.0, .75, .75, "Source code is available under GPL.");  y += fh;

		y = 58;
		DrawStringS(220, y, 1.0, 1.0, 1.0, VERSION_NUMBER); y += fh; 
		DrawStringS(220, y, 1.0, 1.0, 1.0, VERSION_DATE); y += fh; 

		EndScene();
		counter ++;

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done = 1;

			if (pressed & CONT_START)
				ShowMenu(GENERALHELP);

			if (pressed & CONT_RTRIGGER)
				DrawIntro();
		}
	}
}

void DrawIntro()
{
	uint32			counter, frames = 60;
	float			delta;
	ImagePtr		black;

	black = LoadKMG("/rd/black.kmg.gz", 1);
	if(!black)
		return;

	black->alpha = 1.0f;
	delta = 1.0f / frames;
	black->w = dW;
	black->h = dH;
	black->layer = 5.0f;
	for(counter = 0; counter < frames*2; counter ++)
	{
		black->alpha -= delta;
		if(black->alpha < 0.0f)
			black->alpha = 0.0f;

		if(counter == frames)
			delta *= -1;

		StartScene();
		DrawStringS(120, 115, 1.0, 1.0, 1.0, "KORDAMP PRESENTS");
		DrawImage(black);

		EndScene();
	}
	FreeImage(&black);
}

