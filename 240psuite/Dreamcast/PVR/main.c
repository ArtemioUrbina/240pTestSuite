/* 
 * 240p Test Suite
 * Copyright (C)2011-2023 Artemio Urbina
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

KOS_INIT_FLAGS(INIT_DEFAULT | INIT_FS_ROMDISK);

void TestPatternsMenu(ImagePtr title, ImagePtr sd);
void TestPatternsColorMenu(ImagePtr title, ImagePtr sd);
void TestPatternsGeometryMenu(ImagePtr title, ImagePtr sd);
void VideoTestsMenu(ImagePtr title, ImagePtr sd);
void AudioTestsMenu(ImagePtr title, ImagePtr sd);
void HardwareTestsMenu(ImagePtr title, ImagePtr sd);
void DrawIntro();
int DrawFooter(float x, float y, int sel, int c, int showcredits);

ImagePtr SD_b1 = NULL, SD_b2 = NULL;

int main(void)
{
	int 		done = 0, sel = 1, joycnt = 0;
	int			broadcast = FLASHROM_BROADCAST_UNKNOWN, loadedvmu = VMU_NOSAVE;
	uint16		pressed;
	ImagePtr	title = NULL, sd = NULL;
	controller	*st = NULL;
	char		error[256];

	cdrom_init();
	if(cdrom_spin_down() != ERR_OK)
		dbglog(DBG_ERROR,"Could not stop GD-ROM from spinning\n");

	vcable = vid_check_cable();

	InitImages();
	InitController();
	
	// Define if PAL modes are enabled
	broadcast = flashrom_get_broadcast(0);
	if(broadcast != FLASHROM_BROADCAST_NTSC && 
		broadcast != FLASHROM_BROADCAST_UNKNOWN)
	{
		IsPALDC = 1;
		settings.EnablePAL = 1;
	}
	else
		settings.EnablePAL = 0;
	
	if(isMemCardPresent() && 
		MemcardSaveExists(VMU_NAME, NULL, NULL, NULL) == VMU_SAVE_EXISTS)
			loadedvmu = LoadMemCardSave(error, 0);
	
	// Boot in 640x480 is VGA, or 288 in PAL for safety,
	// Some monitors take PAL60 as NTSC 4.43 and decode colors incorrectly
	// if options specifically disables PAL modes, we honor that
	if(vcable != CT_VGA)
	{
		if(settings.EnablePAL && IsPALDC && vcable == CT_COMPOSITE)
			ChangeResolution(VIDEO_288P);
		else
			ChangeResolution(VIDEO_240P);
	}
	else
		ChangeResolution(VIDEO_480P_SL);

	snd_stream_init();	
	LoadFont();
	LoadScanlines();
	LoadSysSettings();
	
	title = LoadIMG("/rd/back.kmg.gz", 0);
	sd = LoadIMG("/rd/SD.kmg.gz", 0);
	if(sd)
	{
		sd->x = 195;
		sd->y = 85;
		
		SD_b1 = LoadIMG("/rd/SD_b1.kmg.gz", 0);
		SD_b2 = LoadIMG("/rd/SD_b2.kmg.gz", 0);
	}
	
	DrawIntro();
	
	if(loadedvmu == VMU_ERROR)
		DrawMessage(error);
	
	if(loadedvmu == VMU_SAVE_UPDATE)
		DrawMessage("Your #YVMU save#Y has been updated\nThank you for using the #C240p Test Suite#C!");
	
	srand((int)(time(0) ^ getpid()));
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
		SD_blink_cycle(sd);

		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Test Patterns"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Video Tests"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Audio Tests"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Hardware Tests"); y += fh; c++;

		y += fh;
		c = DrawFooter(x, y, sel, c, 1);
		
		EndScene();

		st = ReadController(0, &pressed);
		if(refreshVMU)
		{
			updateVMU_SD();
			refreshVMU = 0;
		}
		else
			SD_VMU_blink_cycle();
	
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
#ifdef TEST_VIDEO
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
	FreeImage(&SD_b1);
	FreeImage(&SD_b2);
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
		float 	x = 70.0f;
		float 	y = 90.0f;

		StartScene();
		DrawImage(title);
		DrawImage(sd);
		SD_blink_cycle(sd);

		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Color & Black Levels"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Geometry"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "HCFR Patterns"); y += fh; c++;
		DrawStringS(x, y + fh, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); y += fh; c++;

		y += fh;
		c = DrawFooter(x, y, sel, c, 0);
		
		EndScene();
		st = ReadController(0, &pressed);
		if(refreshVMU)
		{
			updateVMU_SD();
			refreshVMU = 0;
		}
		else
			SD_VMU_blink_cycle();
	
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
					TestPatternsColorMenu(title, sd);
					break;
				case 2:
					TestPatternsGeometryMenu(title, sd);
					break;
				case 3:
					DrawHCFR();
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
#ifdef TEST_VIDEO
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

void TestPatternsColorMenu(ImagePtr title, ImagePtr sd)
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
		float 	y = 60.0f;

		StartScene();
		DrawImage(title);
		DrawImage(sd);
		SD_blink_cycle(sd);

		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "PLUGE"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Color Bars"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "EBU Color Bars"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "SMPTE Color Bars"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Color Bars with Gray Scale"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Color Bleed Check"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Gray Ramp"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "White & RGB screens"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "100 IRE"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Sharpness"); y += fh; c++;
		DrawStringS(x, y + fh, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Patterns Menu"); y += fh; c++;

		y += fh;
		c = DrawFooter(x, y, sel, c, 0);
		
		EndScene();
		VMURefresh("Patterns", "Color&B/W");
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
					DrawGrayRamp();
					break;
				case 8:
					DrawWhiteScreen();
					break;
				case 9:
					Draw100IRE();
					break;
				case 10:
					DrawSharpness();
					break;
				case 11:
					done = 1;
					break;
				case 12:
					ShowMenu(GENERALHELP);
					break;
				case 13:
					HelpWindow(GENERALHELP, title);
					break;
#ifdef TEST_VIDEO
				case 14:
					TestVideoMode(vmode);
					break;
#endif
			} 			
			refreshVMU = 1;
		}			
	}

	return;
}

void TestPatternsGeometryMenu(ImagePtr title, ImagePtr sd)
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
		SD_blink_cycle(sd);

		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Monoscope"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Grid"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Overscan"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Convergence"); y += fh; c++;
		DrawStringS(x, y + fh, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Patterns Menu"); y += fh; c++;

		y += fh;
		c = DrawFooter(x, y, sel, c, 0);
		
		EndScene();
		VMURefresh("Patterns", "Geometry");
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
					DrawMonoscope();
					break;		
				case 2:
					DrawGrid();
					break;
				case 3:
					DrawOverscan();
					break;
				case 4:
					DrawConvergence();
					break;
				case 5:
					done = 1;
					break;
				case 6:
					ShowMenu(GENERALHELP);
					break;
				case 7:
					HelpWindow(GENERALHELP, title);
					break;
#ifdef TEST_VIDEO
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


void VideoTestsMenu(ImagePtr title, ImagePtr sd)
{
	int 			done = 0, sel = 1, joycnt = 0;
	uint16			pressed;		
	controller		*st;

	refreshVMU = 1;
	while(!done && !EndProgram) 
	{		
		float 	r = 1.0f;
		float 	g = 1.0f;
		float 	b = 1.0f;
		int		c = 1;
		float 	x = 50.0f;
		float 	y = 60.0f;

		StartScene();
		DrawImage(title);
		DrawImage(sd);
		SD_blink_cycle(sd);

		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Drop Shadow Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Striped Sprite Test"); y += fh; c++;    
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Lag Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Timing & Reflex Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Scroll Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Grid Scroll Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Horizonta/Vertical Stripes"); y += fh; c++;    
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Checkerboard"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Phase & Sample Rate"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Disappearing Logo"); y += fh; c++;
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
		y += fh;
		DrawStringS(x, y, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); y += fh;  c++;
		
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
			disableSleep();
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
					DrawPhase();
					break;
				case 10:
					DrawDisappear();
					break;
				case 11:
					LEDZoneTest();
					break;
				case 12:
					DiagonalPatternTest();
					break;
				case 13:
					if(vcable != CT_VGA)
						Alternate240p480i();
					break;
				case 14:
					done = 1;
					break;
#ifdef TEST_VIDEO
				case 15:
					TestVideoMode(vmode);
					break;
#endif
			}
			enableSleep();
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

	refreshVMU = 1;
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
		SD_blink_cycle(sd);

		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Sound Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Audio Sync Test"); y += fh; c++;    
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "MDFourier"); y += fh; c++;
#ifndef NO_FFTW
		if(isSIPPresent())
		{
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Microphone Lag Test"); y += fh; c++;
		}
		else
		{
			DrawStringS(x, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, "Microphone Lag Test"); y += fh; c++;
		}
#endif
		DrawStringS(x, y + fh, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); y += fh; c++;

#ifndef NO_FFTW
		if(sel == 4 && !isSIPPresent())
		{
			DrawStringS(x-15, y + 6*fh, 0.8f, 0.8f, 0.8f,
				"You need a microphone to use this feature");
		}
#endif
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
			disableSleep();
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
#ifndef NO_FFTW
				case 4:
					if(isSIPPresent())
						SIPLagTest();
					break;
				case 5:
					done = 1;
					break;
				case 6:
					ShowMenu(GENERALHELP);
					break;
				case 7:
					HelpWindow(GENERALHELP, title);
					break;
#ifdef TEST_VIDEO
				case 8:
					TestVideoMode(vmode);
					break;
#endif
#else
				case 4:
					done = 1;
					break;
				case 5:
					ShowMenu(GENERALHELP);
					break;
				case 6:
					HelpWindow(GENERALHELP, title);
					break;
#ifdef TEST_VIDEO
				case 7:
					TestVideoMode(vmode);
					break;
#endif
#endif
			} 			
			enableSleep();
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

	refreshVMU = 1;
	while(!done && !EndProgram) 
	{		
		float 	r = 1.0f;
		float 	g = 1.0f;
		float 	b = 1.0f;
		int		c = 1;
		float 	x = 70.0f;
		float 	y = 70.0f;

		StartScene();
		DrawImage(title);
		DrawImage(sd);
		SD_blink_cycle(sd);

		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Maple Device List"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Controller Test"); y += fh; c++;
		if(isVMUPresent())
		{
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "VMU Control & MDFourier"); y += fh; c++;    
		}
		else
		{
			DrawStringS(x, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, "VMU Control & MDFourier"); y += fh; c++;    
		}
		if(isLightGunPresent())
		{
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Light Gun Test"); y += fh; c++;    
		}
		else
		{
			DrawStringS(x, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, "Light Gun Test"); y += fh; c++;    
		}
		if(isSIPPresent())
		{
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Microphone Test"); y += fh; c++;    
		}
		else
		{
			DrawStringS(x, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, "Microphone Test"); y += fh; c++;    
		}
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "BIOS and Flashrom data"); y += fh; c++; 
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Show ISP Data"); y += fh; c++; 
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Memory Viewer"); y += fh; c++; 
		
		DrawStringS(x, y + fh, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); y += fh; c++;

		y += fh;
		c = DrawFooter(x, y, sel, c, 0);
		
		if(sel == 3 && !isVMUPresent())
		{
			if(isMemCardPresent())
				DrawStringS(x-15, y + 3*fh, 0.8f, 0.8f, 0.8f,
					"You need a VMU with LCD to use this test");
			else
				DrawStringS(x-15, y + 3*fh, 0.8f, 0.8f, 0.8f,
					"You need a VMU to use this test");
		}
		
		if(sel == 4 && !isLightGunPresent())
		{
			DrawStringS(x-15, y + 3*fh, 0.8f, 0.8f, 0.8f,
				"You need a Light Gun to use this test");
		}
		
		if(sel == 5 && !isSIPPresent())
		{
			DrawStringS(x-15, y + 3*fh, 0.8f, 0.8f, 0.8f,
				"You need a Microphone to use this test");
		}
		
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
					ListMapleDevices();
					break;
				case 2:
					ControllerTest();
					break;
				case 3:
					VMUControllerTest();
					break;
				case 4:
					LightGunTest();
					break;
				case 5:
					if(isSIPPresent())
						MicrophoneTest();
					break;
				case 6:
					ShowBIOSandFlash();
					break;
				case 7:
					Show_ISP_Data();
					break;
				case 8:
					MemoryViewer(0);
					break;
				case 9:
					done = 1;
					break;
				case 10:
					ShowMenu(GENERALHELP);
					break;
				case 11:
					HelpWindow(GENERALHELP, title);
					break;
#ifdef TEST_VIDEO
				case 12:
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
	char	vdata[40], msg[100];
	char	region[40], broadcast[40];
	
	DrawStringS(x, y, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, "Configuration"); y += fh; c++;
	if(showcredits)
	{
		DrawStringS(x, y, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, "Credits"); y += fh; c++;
	}
	DrawStringS(x, y, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, "Help"); 

#ifdef TEST_VIDEO
	//if((vmode == VIDEO_480P || vmode == VIDEO_480P_SL) && vcable == CT_VGA)
	if(showcredits)
	{
		c++;
		DrawStringS(x, y +fh, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Video Settings"); 
	}
#endif
	
	x = 220.0f;
	y = 212.0f;
	
	r = 0.8f;
	g = 0.8f;
	b = 0.8f;
	
	if(!is_flashrom_region_data_changed())
		sprintf(region, "%s", get_flash_region_str(0));
	else
	{
		sprintf(region, "%s(%s)", 
			get_flash_region_str(0), 
			get_flash_region_str(1));
		x -= 4*fw;
	}
		
	if(!is_flashrom_broadcast_changed())
		sprintf(broadcast, "%s", get_flash_broadcast_str(0));
	else
	{
		sprintf(broadcast, "%s(%s)", 
			get_flash_broadcast_str(0), 
			get_flash_broadcast_str(1));
		x -= 4*fw;
	}

	sprintf(msg, "%s %s", region, broadcast);
	DrawStringS(x, y, r, g, b, msg);
	
	y += fh - 2;
	// Check cable is done at EndScene globally
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

void SD_blink_cycle(ImagePtr sd)
{
	static int blink_counter = 0;
	static int is_blinking = 0;
	
	if(sd)
	{
		if(SD_b1)
		{
			SD_b1->x = sd->x+16;
			SD_b1->y = sd->y+32;
		}
		
		if(SD_b2)
		{
			SD_b2->x = sd->x+16;
			SD_b2->y = sd->y+32;
		}
	}
	
	blink_counter++;	
	if(SD_b1 && SD_b2 && blink_counter > 230)
	{
		if(!is_blinking)
		{
			if(rand() % 100 < 98)	// 2% chance every frame after 240
			{
				is_blinking = 1;
				blink_counter = 230;
				DrawImage(SD_b1);
			}
		}
		else
		{
			if(blink_counter >= 232 && blink_counter < 234)
				DrawImage(SD_b2);
				
			if(blink_counter >= 234 && blink_counter < 238)
				DrawImage(SD_b1);
	
			if(blink_counter >= 238)
			{	
				blink_counter = 0;
				is_blinking = 0;
			}
		}
	}
}