/* 
 * 240p Test Suite
 * Copyright (C)2011-2022 Artemio Urbina
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
#include <sys/cdefs.h>
#include <dc/sound/sound.h>
#include <dc/sound/sfxmgr.h>

#include "image.h"
#include "font.h"
#include "vmodes.h"
#include "vmu.h"
#include "vmufs.h"

#include "controller.h"
#include "tests.h"
#include "patterns.h"
#include "sound.h"
#include "hardware.h"

#include "help.h"
#include "menu.h"

/* romdisk */
extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);
KOS_INIT_FLAGS(INIT_DEFAULT);

void TestPatternsMenu(ImagePtr title, ImagePtr sd);
void VideoTestsMenu(ImagePtr title, ImagePtr sd);
void AudioTestsMenu(ImagePtr title, ImagePtr sd);
void HardwareTestsMenu(ImagePtr title, ImagePtr sd);
void DrawIntro();
int DrawFooter(float x, float y, int sel, int c, int showcredits);

int main(void)
{
	int 		done = 0, sel = 1, joycnt = 0, broadcast = FLASHROM_BROADCAST_UNKNOWN;
	uint16		pressed;
	ImagePtr	title, sd;
	controller	*st;
	char		error[256];

	if(cdrom_init() != 0)
		dbglog(DBG_ERROR, "Could not initialize GD-ROM\n");
	if(cdrom_spin_down() != ERR_OK)
		dbglog(DBG_ERROR,"Could not stop GD-ROM from spinning\n");

	vcable = vid_check_cable();

	InitImages();

	LoadVMUSave(error);
	
	// Define if PAL modes are enabled
	broadcast = flashrom_get_region_broadcast();
	if(broadcast != FLASHROM_BROADCAST_NTSC && 
		broadcast != FLASHROM_BROADCAST_UNKNOWN)
		
	{
		IsPALDC = 1;
		settings.EnablePAL = 1;
	}
	else
		settings.EnablePAL = 0;
	
	// Boot in 640x480 is VGA, or 288 in PAL for safety,
	// Some monitors take PAL60 as NTSC 4.43 and decode colors incorrectly
	if(vcable != CT_VGA)
	{
		if(IsPALDC && vcable == CT_COMPOSITE)
			ChangeResolution(VIDEO_288P);
		else
			ChangeResolution(VIDEO_240P);
	}
	else
		ChangeResolution(VIDEO_480P_SL);

	snd_stream_init();	
	LoadFont();
	LoadScanlines();
	
	title = LoadKMG("/rd/back.kmg.gz", 0);
	sd = LoadKMG("/rd/SD.kmg.gz", 0);
	if(sd)
	{
		sd->x = 195;
		sd->y = 85;
	}
	
	DrawIntro();
	refreshVMU = 1;
	while(!done && !EndProgram) 
	{
		float 	r = 1.0f;
		float 	g = 1.0f;
		float 	b = 1.0f;
		int   	c = 1;
		float 	x = 70.0f;
		float 	y = 90.0f;

		StartScene();
		DrawImage(title);
		DrawImage(sd);

		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Test Patterns"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Video Tests"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Audio Tests"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Hardware Tests"); y += fh; c++;

		y += fh;
		c = DrawFooter(x, y, sel, c, 1);
		
		EndScene();
		VMURefresh("240p Test", "");
		
		st = ReadController(0, &pressed);
	
		if (pressed & CONT_START)
			ShowMenu(GENERALHELP);

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
		
		JoystickMenuMove(st, &sel, c, &joycnt);
		
		if (pressed & CONT_A)
		{
			refreshVMU = 1;
			switch(sel)
			{
				case 1:
					TestPatternsMenu(title, sd);
					break;
				case 2:
					VideoTestsMenu(title, sd);
					break;
				case 3:
					AudioTestsMenu(title, sd);
					break;
				case 4:
					HardwareTestsMenu(title, sd);
					break;
				case 5:
					ShowMenu(GENERALHELP);
					break;
				case 6:
					DrawCreditsOnFB();
					break;
				case 7:
					HelpWindow(GENERALHELP, title);
					break;
#ifdef DCLOAD
				case 8:
					TestVideoMode(vmode);
					break;
#endif
			}
			refreshVMU = 1;
		}
	}

	updateVMU(" Goodbye ", " m(_ _)m ", 1);

	FreeImage(&title);		
	FreeImage(&sd);		
	ReleaseScanlines();
	ReleaseFont();
	CleanImages();
	snd_stream_shutdown();
	cdrom_shutdown();
#ifndef DCLOAD
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
		int		c = 1;
		float 	x = 40.0f;
		float 	y = 55.0f;

		StartScene();
		DrawImage(title);
		DrawImage(sd);

		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Pluge"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Color Bars"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "EBU Color Bars"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "SMPTE Color Bars"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Color Bars with Gray Scale"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Color Bleed Check"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Grid"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Monoscope"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Gray Ramp"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "White & RGB screens"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "100 IRE"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Sharpness"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Overscan"); y += fh; c++;
		DrawStringS(x, y, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); y += fh; c++;

		c = DrawFooter(x, y, sel, c, 0);
		
		EndScene();
		VMURefresh("Patterns", "");
		st = ReadController(0, &pressed);
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
	
		JoystickMenuMove(st, &sel, c, &joycnt);
	
		if (pressed & CONT_B)
			done = 1;
	
		if (pressed & CONT_A)
		{
			refreshVMU = 1;
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
					DrawGrid();
					break;
				case 8:
					DrawMonoscope();
					break;					
				case 9:
					DrawGrayRamp();
					break;
				case 10:
					DrawWhiteScreen();
					break;
				case 11:
					Draw100IRE();
					break;
				case 12:
					DrawSharpness();
					break;
				case 13:
					DrawOverscan();
					break;
				case 14:
					done = 1;
					break;
				case 15:
					ShowMenu(GENERALHELP);
					break;
				case 16:
					HelpWindow(GENERALHELP, title);
					break;
#ifdef DCLOAD
				case 17:
					TestVideoMode(vmode);
					break;
#endif
			} 			
			refreshVMU = 1;
		}			
	}

	return;
}


void VideoTestsMenu(ImagePtr title, ImagePtr sd)
{
	int 			done = 0, sel = 1, joycnt = 0;
	uint16			pressed;		
	controller		*st;

	while(!done && !EndProgram) 
	{		
		float 	r = 1.0f;
		float 	g = 1.0f;
		float 	b = 1.0f;
		int		c = 1;
		float 	x = 40.0f;
		float 	y = 55.0f;
#ifndef NO_FFTW
		maple_device_t *sip = NULL;
#endif

		StartScene();
		DrawImage(title);
		DrawImage(sd);

#ifndef NO_FFTW
		sip = maple_enum_type(0, MAPLE_FUNC_MICROPHONE);
#endif
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Drop Shadow Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Striped Sprite Test"); y += fh; c++;    
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Lag Test"); y += fh; c++;
#ifndef NO_FFTW
		if(sip)
		{
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Microphone Lag Test"); y += fh; c++;
		}
		else
		{
#endif
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Timing & Reflex Test"); y += fh; c++;
#ifndef NO_FFTW
		}
#endif
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Scroll Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Grid Scroll Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Horizonta/Vertical Stripes"); y += fh; c++;    
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Checkerboard"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Backlit Zone Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Diagonal Test"); y += fh; c++;
		if(vcable == CT_VGA)
		{
			DrawStringS(x, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f,
				"Alternating 240p/480i Test"); y += fh; c++;
		}
		else
		{
			if(!IsPAL)
			{
				DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b,
					"Alternating 240p/480i Test"); y += fh; c++;
			}
			else
			{
				DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b,
					"Alternating 288p/576i Test"); y += fh; c++;
			}
		}
		DrawStringS(x, y + fh, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); y += fh;  c++;

		y += fh;
		c = DrawFooter(x, y, sel, c, 0);
		
		EndScene();
		VMURefresh("Video", "");
		st = ReadController(0, &pressed);
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
	
		JoystickMenuMove(st, &sel, c, &joycnt);
	
		if (pressed & CONT_B)
			done = 1;
	
		if (pressed & CONT_A)
		{
			refreshVMU = 1;
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
#ifndef NO_FFTW
					if(sip)
						SIPLagTest();
					else
#endif
						ReflexNTimming();
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
					LEDZoneTest();
					break;
				case 10:
					DiagonalPatternTest();
					break;
				case 11:
					if(vcable != CT_VGA)
						Alternate240p480i();
					break;
				case 12:
					done = 1;
					break;
				case 13:
					ShowMenu(GENERALHELP);
					break;
				case 14:
					HelpWindow(GENERALHELP, title);
					break;
#ifdef DCLOAD
				case 15:
					TestVideoMode(vmode);
					break;
#endif
			} 			
			refreshVMU = 1;
		}			
	}

	return;
}

void AudioTestsMenu(ImagePtr title, ImagePtr sd)
{
	int 			done = 0, sel = 1, joycnt = 0;
	uint16			pressed;		
	controller		*st;

	while(!done && !EndProgram) 
	{		
		float 	r = 1.0f;
		float 	g = 1.0f;
		float 	b = 1.0f;
		int		c = 1;
		float 	x = 70.0f;
		float 	y = 90.0f;

		StartScene();
		DrawImage(title);
		DrawImage(sd);

		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Sound Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Audio Sync Test"); y += fh; c++;    
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "MDFourier"); y += fh; c++;

		DrawStringS(x, y + fh, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); y += fh; c++;

		y += fh;
		c = DrawFooter(x, y, sel, c, 0);
		
		EndScene();
		VMURefresh("Audio", "");
		st = ReadController(0, &pressed);
		if (pressed & CONT_START)
			ShowMenu(GENERALHELP);

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
	
		JoystickMenuMove(st, &sel, c, &joycnt);
	
		if (pressed & CONT_B)
			done = 1;
	
		if (pressed & CONT_A)
		{
			refreshVMU = 1;
			switch(sel)
			{
				case 1:
					SoundTest();
					break;
				case 2:
					AudioSyncTest();
					break;
				case 3:
					MDFourier();
					break;
				case 4:
					done = 1;
					break;
				case 5:
					ShowMenu(GENERALHELP);
					break;
				case 6:
					HelpWindow(GENERALHELP, title);
					break;
#ifdef DCLOAD
				case 8:
					TestVideoMode(vmode);
					break;
#endif
			} 			
			refreshVMU = 1;
		}			
	}

	return;
}

void HardwareTestsMenu(ImagePtr title, ImagePtr sd)
{
	int 			done = 0, sel = 1, joycnt = 0;
	uint16			pressed;		
	controller		*st;

	while(!done && !EndProgram) 
	{		
		float 	r = 1.0f;
		float 	g = 1.0f;
		float 	b = 1.0f;
		int		c = 1;
		float 	x = 70.0f;
		float 	y = 90.0f;

		StartScene();
		DrawImage(title);
		DrawImage(sd);

		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Controller Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Memory Viewer"); y += fh; c++;    

		DrawStringS(x, y + fh, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); y += fh; c++;

		y += fh;
		c = DrawFooter(x, y, sel, c, 0);
		
		EndScene();
		VMURefresh("Hardware", "");
		st = ReadController(0, &pressed);
		if (pressed & CONT_START)
			ShowMenu(GENERALHELP);

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
	
		JoystickMenuMove(st, &sel, c, &joycnt);
	
		if (pressed & CONT_B)
			done = 1;
	
		if (pressed & CONT_A)
		{
			refreshVMU = 1;
			switch(sel)
			{
				case 1:
					ControllerTest();
					break;
				case 2:
					MemoryViewer(0);
					break;
				case 3:
					done = 1;
					break;
				case 4:
					ShowMenu(GENERALHELP);
					break;
				case 5:
					HelpWindow(GENERALHELP, title);
					break;
#ifdef DCLOAD
				case 7:
					TestVideoMode(vmode);
					break;
#endif
			} 			
			refreshVMU = 1;
		}			
	}

	return;
}

int DrawFooter(float x, float y, int sel, int c, int showcredits)
{
	float 	r = 1.0f;
	float 	g = 1.0f;
	float 	b = 1.0f;
	char	vdata[40], msg[80];
	char	*region, *broadcast;
	
	DrawStringS(x, y, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, "Configuration"); y += fh; c++;
	if(showcredits)
	{
		DrawStringS(x, y, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, "Credits"); y += fh; c++;
	}
	DrawStringS(x, y, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, "Help"); 

#ifdef DCLOAD
	//if((vmode == VIDEO_480P || vmode == VIDEO_480P_SL) && vcable == CT_VGA)
	if(showcredits)
	{
		c++;
		DrawStringS(x, y +fh, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Video Settings"); 
	}
#endif
		
	// Check cable is done at EndScene globally
	
	x = 220.0f;
	y = 212.0f;
	
	r = 0.8f;
	g = 0.8f;
	b = 0.8f;
	
	switch(flashrom_get_region_country())
	{
		case FLASHROM_REGION_JAPAN:
			region = "Japan";
			break;
		case FLASHROM_REGION_US:
			region = "USA";
			break;
		case FLASHROM_REGION_EUROPE:
			region = "Europe";
			break;
		case FLASHROM_REGION_UNKNOWN:
		default:
			region = "????";
			break;
	}
	
	switch(flashrom_get_region_broadcast())
	{
		case FLASHROM_BROADCAST_NTSC:
			broadcast = "NTSC";
			break;
		case FLASHROM_BROADCAST_PAL:
			broadcast = "PAL";
			break;
		case FLASHROM_BROADCAST_PALM:
			broadcast = "PAL-M";
			break;
		case FLASHROM_BROADCAST_PALN:
			broadcast = "PAL-N";
			break;
		case FLASHROM_BROADCAST_UNKNOWN:
		default:
			broadcast = "????";
			break;
	}
	
	sprintf(msg, "%s %s", region, broadcast);
	DrawStringS(x, y, r, g, b, msg);
	
	y += fh - 2;
	GetVideoModeStr(vdata, 1);
	switch(vcable)
	{
		case CT_RGB:
			sprintf(msg, "RGB %s", vdata);
			DrawStringS(x, y, r, g, b, msg);
			break;
		case CT_VGA:
			sprintf(msg, "VGA %s", vdata);
			DrawStringS(x, y, r, g, b, msg);
			break;
		case CT_COMPOSITE:
			sprintf(msg, "CVBS %s", vdata);
			DrawStringS(x, y, r, g, b, msg);
			break;
		case CT_NONE:
			sprintf(msg, "NONE %s", vdata);
			DrawStringS(x, y, r, g, b, msg);
			break;
		case CT_ANY:
			sprintf(msg, "ANY %s", vdata);
			DrawStringS(x, y, r, g, b, msg);
			break;
		default:
			sprintf(msg, "???? %s", vdata);
			DrawStringS(x, y, r, g, b, msg);
			break;
	}
	
	return c;
}