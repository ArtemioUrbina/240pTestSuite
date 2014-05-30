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
	int 		done = 0, sel = 1, joycnt = 0;
	uint16		pressed;
	ImagePtr	title, sd;
	controller	*st;

	/* init kos	*/
	vcable = vid_check_cable();
	if(vcable != CT_VGA)
		ChangeResolution(VIDEO_240P);
	else
		ChangeResolution(VIDEO_480P_SL);

	region = flashrom_get_region();
	if(region == FLASHROM_REGION_EUROPE)
		settings.EnablePAL = 1;

	InitImages();
	LoadFont();
	LoadScanlines();
	title = LoadKMG("/rd/back.kmg.gz", 0);
	sd = LoadKMG("/rd/SD.kmg.gz", 0);
	if(sd)
	{
		sd->x = 221;
		sd->y = 94;
	}
	
	DrawIntro();
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
				
		// Check cable again in case it was changed on the fly
		vcable = vid_check_cable();

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

		res[0] = '\0';
		switch(vmode)
		{
			case VIDEO_240P:
				if(vcable != CT_VGA)
					sprintf(res, "Video: 240p");
				break;
			case VIDEO_480I_A240:
				if(vcable != CT_VGA)
					sprintf(res, "Video: 480i (scaled 240p)");
				break;
			case VIDEO_480I:
				if(vcable != CT_VGA)
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
				if(vcable == CT_VGA)
					sprintf(res, "Video: 480p (Scaling disabled)");
				break;
			case VIDEO_480P_SL:
				if(vcable == CT_VGA)
					sprintf(res, "Video: 480p (scaled 240p)");
				break;
		}
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, res); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Options"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Help"); 

//#ifdef VGA_SETTINGS
		//if((vmode == VIDEO_480P || vmode == VIDEO_480P_SL) && vcable == CT_VGA)
		{
			c++;
			DrawStringS(x, y +fh, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Video Settings"); 
		}    
//#endif

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
				DrawStringS(265.0f, 225.0f, r, g, b, "Composite");
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
				settings.drawpvrbg = !settings.drawpvrbg;

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
							Alternate240p480i();
						break;
					case 12:
						SoundTest();
						break;
					case 13:
						SelectVideoMode(title);
						break;
					case 14:
						ChangeOptions(title);
						break;
					case 15:
						HelpWindow(GENERALHELP, title);
						break;
					case 16:
						TestVideoMode(&custom_288);
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
				DrawStringS(265.0f, 225.0f, r, g, b, "RGB");
				break;
			case CT_VGA:
				DrawStringS(265.0f, 225.0f, r, g, b, "VGA");
				break;
			case CT_COMPOSITE:
				DrawStringS(265.0f, 225.0f, r, g, b, "Composite");
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
