/* 
 * 240p Test Suite
 * Copyright (C)2011-2019 Artemio Urbina
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

#include "main.h"
#include "mdfourier.h"
#include "segacdtests.h"

u8 joytype = JOY_TYPE_UNKNOWN;
u8 joytype2 = JOY_TYPE_UNKNOWN;
u8 IsPALVDP = 0;
u8 VDPChanged = 0;
u8 intCancel = 0;

int main()
{
	u16 cursel = 1, pos, reload = 1;
	u16 buttons, oldButtons = 0xffff, pressedButtons;

	char md_ver[30];

	VDP_init();

	enable_256 = 1;
	VDP_Start();
	VDP_setScreenWidth320();
	if(Detect_VDP_PAL())
	{
		VDP_setScreenHeight240();
		pal_240 = 1;
	}
	else
	{
		VDP_setScreenHeight224();
		pal_240 = 0;
	}

#ifdef SEGACD	
	SendSCDCommand(Op_InitCD);
#endif

	VDP_loadFontData(font_tiles, FONT_LEN, USE_DMA);
	VDP_End();
	
	DrawIntro();
	
	SYS_setVIntCallback(VBlankIntCallback);
	
	while(1)
	{
		if(reload)
		{
			DrawMainBGwithGillian(1, 216, 72);
			reload = 0;
		}

		pos = 10;
		VDP_Start();
		VDP_drawTextBG(APLAN, "Test Patterns", TILE_ATTR(cursel == 1 ? PAL1 : PAL0, 0, 0, 0), 6, pos++);
		VDP_drawTextBG(APLAN, "Video tests", TILE_ATTR(cursel == 2 ? PAL1 : PAL0, 0, 0, 0), 6, pos++);
		VDP_drawTextBG(APLAN, "Audio tests", TILE_ATTR(cursel == 3 ? PAL1 : PAL0, 0, 0, 0), 6, pos++);
		VDP_drawTextBG(APLAN, "Hardware tools", TILE_ATTR(cursel == 4 ? PAL1 : PAL0, 0, 0, 0), 6, pos++);
		pos+=4;
		VDP_drawTextBG(APLAN, "Help", TILE_ATTR(cursel == 5 ? PAL1 : PAL0, 0, 0, 0), 6, pos++);
		VDP_drawTextBG(APLAN, "Options", TILE_ATTR(cursel == 6 ? PAL1 : PAL0, 0, 0, 0), 6, pos++);
		VDP_drawTextBG(APLAN, "Credits", TILE_ATTR(cursel == 7 ? PAL1 : PAL0, 0, 0, 0), 6, pos);
			
		DrawResolution();
		Detect_MD(md_ver);
		
		VDP_drawTextBG(APLAN, md_ver, TILE_ATTR(PAL0, 0, 0, 0), 20, 26);
		VDP_End();

		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_GENERAL))
			reload = 1;

		if(pressedButtons & BUTTON_DOWN)
		{
			cursel++;
			if(cursel > 7)
				cursel = 1;
		}

		if(pressedButtons & BUTTON_UP)
		{
			cursel--;
			if(cursel < 1)
				cursel = 7;
		}

		if(pressedButtons & BUTTON_A)
		{
			if(cursel < 5)
				FadeAndCleanUp();
			switch (cursel)
			{
			case 1:
				TestPatternMenu();
				break;
			case 2:
				VideoTestsMenu();
				break;
			case 3:
				AudioTestsMenu();
				break;
			case 4:
				HardwareMenu();
				break;
			case 5:
				DrawHelp(HELP_GENERAL);
				break;
			case 6:
				VideoOptions();
				break;
			case 7:
				DrawCredits();
				break;
			}

			FadeAndCleanUp();
			reload = 1;
		}
		VDP_waitVSync();
	}

	return 0;
}

void TestPatternMenu()
{
	u16 cursel = 1, pos, reload = 1;
	u16 buttons, oldButtons = 0xffff, pressedButtons;
	u16 done = 0;
	char md_ver[30];

	FadeAndCleanUp();
	while(!done)
	{
		if(reload)
		{
			DrawMainBGwithGillian(1, 216, 72);
			
			reload = 0;
		}

		pos = 6;
		VDP_Start();
		VDP_drawTextBG(APLAN, "Pluge", TILE_ATTR(cursel == 1 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Color Bars", TILE_ATTR(cursel == 2 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "EBU Color Bars", TILE_ATTR(cursel == 3 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "SMPTE Color Bars", TILE_ATTR(cursel == 4 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Referenced Color Bars", TILE_ATTR(cursel == 5 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Color Bleed Check", TILE_ATTR(cursel == 6 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Monoscope", TILE_ATTR(cursel == 7 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Grid", TILE_ATTR(cursel == 8 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Gray Ramp", TILE_ATTR(cursel == 9 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "White & RGB Screens", TILE_ATTR(cursel == 10 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "100 IRE", TILE_ATTR(cursel == 11 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Sharpness", TILE_ATTR(cursel == 12 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Overscan", TILE_ATTR(cursel == 13 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Convergence", TILE_ATTR(cursel == 14 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		pos++;
		VDP_drawTextBG(APLAN, "Back to Main Menu", TILE_ATTR(cursel == 15 ? PAL1 : PAL0, 0, 0, 0), 5, pos);
		
		DrawResolution();
		Detect_MD(md_ver);
		
		VDP_drawTextBG(APLAN, md_ver, TILE_ATTR(PAL0, 0, 0, 0), 20, 26);
		VDP_End();
		
		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_GENERAL))
			reload = 1;

		if(pressedButtons & BUTTON_DOWN)
		{
			cursel++;
			if(cursel > 15)
				cursel = 1;
		}

		if(pressedButtons & BUTTON_UP)
		{
			cursel--;
			if(cursel < 1)
				cursel = 15;
		}

		if(pressedButtons & BUTTON_B || 
			pressedButtons & BUTTON_START || 
			pressedButtons & BUTTON_LEFT)
		{
			done = 1;
		}

		if(pressedButtons & BUTTON_A)
		{
			if(cursel < 14)
				FadeAndCleanUp();
			switch (cursel)
			{
			case 1:
				DrawPluge();
				break;
			case 2:
				DrawColorBars();
				break;
			case 3:
				DrawSMPTEorEBU(1);
				break;
			case 4:
				DrawSMPTEorEBU(0);
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
				DrawConvergence();
				break;
			case 15:
				done = 1;
				break;
			}

			FadeAndCleanUp();
			buttons = JOY_readJoypad(JOY_1);
			pressedButtons = buttons & ~oldButtons;
			oldButtons = buttons;

			reload = 1;
		}

		VDP_waitVSync();
	}

	return;
}

void VideoTestsMenu()
{
	u16 cursel = 1, pos, reload = 1;
	u16 buttons, oldButtons = 0xffff, pressedButtons;
	u16 done = 0;
	char md_ver[30];

	FadeAndCleanUp();
	while(!done)
	{
		if(reload)
		{
			DrawMainBGwithGillian(1, 216, 72);

			reload = 0;
		}

		pos = 7;
		VDP_Start();
		VDP_drawTextBG(APLAN, "Drop Shadow Test", TILE_ATTR(cursel == 1 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Striped Sprite Test", TILE_ATTR(cursel == 2 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Lag Test", TILE_ATTR(cursel == 3 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Timing & Reflex Test", TILE_ATTR(cursel == 4 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Scroll Test", TILE_ATTR(cursel == 5 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Grid Scroll Test", TILE_ATTR(cursel == 6 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Horiz/Vert Stripes", TILE_ATTR(cursel == 7 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Checkerboard", TILE_ATTR(cursel == 8 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Backlit Zone Test", TILE_ATTR(cursel == 9 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Alternate 240p/480i", TILE_ATTR(cursel == 10 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		pos++;
		VDP_drawTextBG(APLAN, "Help", TILE_ATTR(cursel == 11 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Options", TILE_ATTR(cursel == 12 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Back to Main Menu", TILE_ATTR(cursel == 13 ? PAL1 : PAL0, 0, 0, 0), 5, pos);
		
		DrawResolution();
		Detect_MD(md_ver);
		
		VDP_drawTextBG(APLAN, md_ver, TILE_ATTR(PAL0, 0, 0, 0), 20, 26);
		VDP_End();
		
		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_GENERAL))
			reload = 1;

		if(pressedButtons & BUTTON_DOWN)
		{
			cursel++;
			if(cursel > 13)
				cursel = 1;
		}

		if(pressedButtons & BUTTON_UP)
		{
			cursel--;
			if(cursel < 1)
				cursel = 13;
		}

		if(pressedButtons & BUTTON_B || 
			pressedButtons & BUTTON_START || 
			pressedButtons & BUTTON_LEFT)
		{
			done = 1;
		}

		if(pressedButtons & BUTTON_A)
		{
			if(cursel < 11)
				FadeAndCleanUp();
			switch (cursel)
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
				ReflexNTiming();
				break;
			case 5:
				HScrollTest();
				break;
			case 6:
				VScrollTest();
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
				Alternate240p480i();
				break;
			case 11:
				DrawHelp(HELP_GENERAL);
				break;
			case 12:
				VideoOptions();
				break;
			case 13:
				done = 1;
				break;
			}

			FadeAndCleanUp();
			buttons = JOY_readJoypad(JOY_1);
			pressedButtons = buttons & ~oldButtons;
			oldButtons = buttons;

			reload = 1;
		}

		VDP_waitVSync();
	}

	return;
}

void AudioTestsMenu()
{
	u16 cursel = 1, pos, reload = 1;
	u16 buttons, oldButtons = 0xffff, pressedButtons;
	u16 done = 0;
	char md_ver[30];

	FadeAndCleanUp();
	while(!done)
	{
		if(reload)
		{
			DrawMainBGwithGillian(1, 216, 72);

			reload = 0;
		}

		pos = 11;
		VDP_Start();
		VDP_drawTextBG(APLAN, "Sound Test", TILE_ATTR(cursel == 1 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Audio Sync Test", TILE_ATTR(cursel == 2 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "MDFourier", TILE_ATTR(cursel == 3 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		pos++;
		VDP_drawTextBG(APLAN, "Help", TILE_ATTR(cursel == 4 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Options", TILE_ATTR(cursel == 5 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Back to Main Menu", TILE_ATTR(cursel == 6 ? PAL1 : PAL0, 0, 0, 0), 5, pos);
		
		DrawResolution();
		Detect_MD(md_ver);
		
		VDP_drawTextBG(APLAN, md_ver, TILE_ATTR(PAL0, 0, 0, 0), 20, 26);
		VDP_End();
		
		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_GENERAL))
			reload = 1;

		if(pressedButtons & BUTTON_DOWN)
		{
			cursel++;
			if(cursel > 6)
				cursel = 1;
		}

		if(pressedButtons & BUTTON_UP)
		{
			cursel--;
			if(cursel < 1)
				cursel = 6;
		}

		if(pressedButtons & BUTTON_B || 
			pressedButtons & BUTTON_START || 
			pressedButtons & BUTTON_LEFT)
		{
			done = 1;
		}

		if(pressedButtons & BUTTON_A)
		{
			if(cursel < 4)
				FadeAndCleanUp();
			switch (cursel)
			{
			case 1:
				SoundTest();
				break;
			case 2:
				AudioSyncTest();
				break;
			case 3:
				MDFourier(0);
				break;
			case 4:
				DrawHelp(HELP_GENERAL);
				break;
			case 5:
				VideoOptions();
				break;
			case 6:
				done = 1;
				break;
			}

			FadeAndCleanUp();
			buttons = JOY_readJoypad(JOY_1);
			pressedButtons = buttons & ~oldButtons;
			oldButtons = buttons;

			reload = 1;
		}

		VDP_waitVSync();
	}

	return;
}

void HardwareMenu()
{
	u16 cursel = 1, pos, reload = 1;
	u16 buttons, oldButtons = 0xffff, pressedButtons;
	u16 done = 0;
	char md_ver[30];

	FadeAndCleanUp();
	while(!done)
	{
		if(reload)
		{
			DrawMainBGwithGillian(1, 216, 72);

			reload = 0;
		}

		pos = 10;
		VDP_Start();
		VDP_drawTextBG(APLAN, "Controller Test", TILE_ATTR(cursel == 1 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Z80 RAM Test", TILE_ATTR(cursel == 2 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Memory Viewer", TILE_ATTR(cursel == 3 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Sega CD Tests", TILE_ATTR(cursel == 4 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		pos++;
		VDP_drawTextBG(APLAN, "Help", TILE_ATTR(cursel == 5 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Options", TILE_ATTR(cursel == 6 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Back to Main Menu", TILE_ATTR(cursel == 7 ? PAL1 : PAL0, 0, 0, 0), 5, pos);
		
		DrawResolution();
		Detect_MD(md_ver);
		
		VDP_drawTextBG(APLAN, md_ver, TILE_ATTR(PAL0, 0, 0, 0), 20, 26);
		VDP_End();
		
		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_GENERAL))
			reload = 1;

		if(pressedButtons & BUTTON_DOWN)
		{
			cursel++;
			if(cursel > 7)
				cursel = 1;
		}

		if(pressedButtons & BUTTON_UP)
		{
			cursel--;
			if(cursel < 1)
				cursel = 7;
		}

		if(pressedButtons & BUTTON_B || 
			pressedButtons & BUTTON_START || 
			pressedButtons & BUTTON_LEFT)
		{
			done = 1;
		}

		if(pressedButtons & BUTTON_A)
		{
			if(cursel < 5)
				FadeAndCleanUp();
				
			switch (cursel)
			{
			case 1:
				ControllerTest();
				break;
			case 2:
				Z80RamTest();
				break;
			case 3:
				MemViewer(0);
				break;
			case 4:
				SegaCDMenu();
				break;
			case 5:
				DrawHelp(HELP_GENERAL);
				break;
			case 6:
				VideoOptions();
				break;
			case 7:
				done = 1;
				break;
			}

			FadeAndCleanUp();
			buttons = JOY_readJoypad(JOY_1);
			pressedButtons = buttons & ~oldButtons;
			oldButtons = buttons;

			reload = 1;
		}

		VDP_waitVSync();
	}

	return;
}


#define IntCallCheck()	if(intCancel){intCancel=0;SYS_setVIntCallback(VBlankIntCallback);FadeAndCleanUp();MDFourier(1);return;}
void DrawIntro()
{	
	SYS_setVIntCallback(VBlankIntCallbackCancel);	
	
	IntCallCheck();
	VDP_setPalette(PAL0, palette_black);
	VDP_drawTextBG(APLAN, "KORDAMP PRESENTS", TILE_ATTR(PAL0, 0, 0, 0), 12, 12);
	VDP_fadeIn(0, 15, palette_grey, 30, 1);
	VDP_waitFadeCompletion();
	IntCallCheck();
	VDP_fadeOutAll(15, 0);
	VDP_resetScreen();
	IntCallCheck();
}

void Detect_MD(char *str)
{
	char *pointer = NULL;
	u16 IsPAL = 0, data = 0;
	u16 TMSS = 0, IsAsia = 0;

	pointer = (char *)0xA10001;

	data = *pointer;

	IsPAL = (data & 0x40);
	TMSS = (data & 0x0F);
	IsAsia = (data & 0xFF00) == 0x0;

	if(TMSS)
		strcpy(str, "TMSS ");
	else
		strcpy(str, "     ");

	if(!IsAsia)
	{
		if(IsPAL)
			strcat(str, "MegaDrive EU");
		else
			strcat(str, "Sega Genesis");
	}
	else
		strcat(str, "MegaDrive JP");
}

void DrawN()
{
	u16 size;
	u16 exit = 0;
	u16 buttons;

	FadeAndCleanUp();
	size = sizeof(n_tiles) / 32;
	VDP_Start();
	VDP_setPalette(PAL0, n_pal);
	VDP_loadTileData(n_tiles, TILE_USERINDEX, size, USE_DMA);
	VDP_setMyTileMapRect(BPLAN, n_map, TILE_USERINDEX, 120 / 8, 72 / 8, 80 / 8, 80 / 8);
	VDP_End();
	
	while(!exit)
	{
		buttons = JOY_readJoypad(JOY_1);

		if(!(buttons & BUTTON_C))
			exit = 1;

		VDP_waitVSync();
	}
}

void DrawCredits()
{
	u16 exit = 0, pos = 0, counter = 0;
	u16 buttons, oldButtons = 0xffff, pressedButtons, loadvram = 1;

	FadeAndCleanUp();
	
	while(!exit)
	{
		if(loadvram)
		{
			pos = 8;
			counter = 1;
			
#ifdef SEGACD
			pos = 6;
#endif
			DrawMainBG();

			VDP_Start();
			VDP_setVerticalScroll(PLAN_A, 4);
			VDP_setHorizontalScroll(PLAN_A, -4);

			VDP_drawTextBG(APLAN, "Code and Patterns:", TILE_ATTR(PAL1, 0, 0, 0), 4, pos++);
			pos++;
			VDP_drawTextBG(APLAN, "Menu Pixel Art:", TILE_ATTR(PAL1, 0, 0, 0), 4, pos++);
			VDP_drawTextBG(APLAN, "Asher", TILE_ATTR(PAL0, 0, 0, 0), 5, pos++);
			VDP_drawTextBG(APLAN, "SDK:", TILE_ATTR(PAL1, 0, 0, 0), 4, pos++);
			VDP_drawTextBG(APLAN, "http://github.com/Stephane-D/SGDK", TILE_ATTR(PAL0, 0, 0, 0), 4, pos++);
			VDP_drawTextBG(APLAN, "SDK Consultor:", TILE_ATTR(PAL1, 0, 0, 0), 4, pos++);
			VDP_drawTextBG(APLAN, "Stef", TILE_ATTR(PAL0, 0, 0, 0), 5, pos++);
#ifdef SEGACD
			VDP_drawTextBG(APLAN, "SEGA CD Loader by:", TILE_ATTR(PAL1, 0, 0, 0), 4, pos++);
			VDP_drawTextBG(APLAN, "Luke Usher/SoullessSentinel", TILE_ATTR(PAL0, 0, 0, 0), 5, pos++);
			VDP_drawTextBG(APLAN, "SEGA CD Consultors:", TILE_ATTR(PAL1, 0, 0, 0), 4, pos++);
			VDP_drawTextBG(APLAN, "Chilly Willy & TascoDLX", TILE_ATTR(PAL0, 0, 0, 0), 5, pos++);
#endif
			VDP_drawTextBG(APLAN, "Advisor:", TILE_ATTR(PAL1, 0, 0, 0), 4, pos++);
			VDP_drawTextBG(APLAN, "Fudoh", TILE_ATTR(PAL0, 0, 0, 0), 5, pos++);
			VDP_drawTextBG(APLAN, "Collaboration:", TILE_ATTR(PAL1, 0, 0, 0), 4, pos++);
			VDP_drawTextBG(APLAN, "Shmups regulars", TILE_ATTR(PAL0, 0, 0, 0), 5, pos++);
			VDP_drawTextBG(APLAN, "Info on using this test suite:", TILE_ATTR(PAL1, 0, 0, 0), 4, pos++);
			VDP_drawTextBG(APLAN, "http://junkerhq.net/240p", TILE_ATTR(PAL0, 0, 0, 0), 5, pos++);

			VDP_drawTextBG(APLAN, "Ver. 1.23d", TILE_ATTR(PAL1, 0, 0, 0), 26, 6);
			VDP_drawTextBG(APLAN, "12/08/2021", TILE_ATTR(PAL0, 0, 0, 0), 26, 7);
			
			VDP_drawTextBG(BPLAN, "Dedicated to Elisa", TILE_ATTR(PAL0, 0, 0, 0), 18, 24);
			VDP_End();

#ifdef SEGACD
			pos = 7;
#else
			pos = 9;
#endif
			loadvram = 0;
		}

		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		VDP_Start();
		if(counter == 1)
			VDP_drawTextBG(APLAN, "Artemio Urbina      ", TILE_ATTR(PAL0, 0, 0, 0), 5, pos);
		if(counter == 60 * 4)
			VDP_drawTextBG(APLAN, "@Artemio (twitter)  ", TILE_ATTR(PAL0, 0, 0, 0), 5, pos);
		if(counter == 60 * 8)
			VDP_drawTextBG(APLAN, "aurbina@junkerhq.net", TILE_ATTR(PAL0, 0, 0, 0), 5, pos);
		if(counter == 60 * 16)
			counter = 0;
		VDP_End();

		if(pressedButtons & BUTTON_START)
			exit = 1;
		
		if(pressedButtons & BUTTON_C)
		{
			DrawN();
			loadvram = 1;
		}

		VDP_waitVSync();
		counter++;
	}

	VDP_fadeOutAll(FADE_TIME, 0);
	VDP_setVerticalScroll(PLAN_A, 0);
}

void CleanUp()
{
	StopPSG();
	VDP_Start();
	VDP_resetSprites();
	VDP_updateSprites();
	VDP_setHorizontalScroll(PLAN_B, 0);
	VDP_setHorizontalScroll(PLAN_A, 0);
	VDP_setVerticalScroll(PLAN_B, 0);
	VDP_setVerticalScroll(PLAN_A, 0);
	VDP_setHilightShadow(0);
	VDP_clearTileMapRect(BPLAN, 0, 0, 320 / 8, 224 / 8);
	VDP_clearTileMapRect(APLAN, 0, 0, 320 / 8, 224 / 8);
	VDP_End();
}

void FadeAndCleanUp()
{
	VDP_fadeOutAll(FADE_TIME, 0);
	CleanUp();
	VDP_setScreenWidth320();
	VDP_resetScreen();
}

void VBlankIntCallback()
{
	u8 PalCheck = 0;

	joytype = JOY_getJoypadType(JOY_1);
	joytype2 = JOY_getJoypadType(JOY_2);
	
	PalCheck = Detect_VDP_PAL();
	if(PalCheck != IsPALVDP)
	{
		IsPALVDP = PalCheck;
		if(!IsPALVDP && pal_240)
		{
			VDP_setScreenHeight224();
			pal_240 = 0;
		}
		VDPChanged = 1;
	}
	else
		VDPChanged = 0;
}
	
void VBlankIntCallbackCancel()
{
	if(JOY_readJoypad(JOY_1) & BUTTON_START)
		intCancel = 1;
}

void DrawResolution()
{
	if(IsPALVDP)
	{
		if(VDP_Detect_Interlace())
			VDP_drawTextBG(APLAN, pal_240 ? "PAL VDP 320x480i " : " PAL VDP 320x448i", TILE_ATTR(PAL0, 0, 0, 0), 19, 24);
		else
			VDP_drawTextBG(APLAN, pal_240 ? "PAL VDP 320x240p " : " PAL VDP 320x224p", TILE_ATTR(PAL0, 0, 0, 0), 19, 24);
	}
	else
	{
		if(VDP_Detect_Interlace())
			VDP_drawTextBG(APLAN, "NTSC VDP 320x448i", TILE_ATTR(PAL0, 0, 0, 0), 19, 24);
		else
			VDP_drawTextBG(APLAN, "NTSC VDP 320x224p", TILE_ATTR(PAL0, 0, 0, 0), 19, 24);
	}
}

void VideoOptions()
{
	int sel = 0, loadvram = 1;
	u16 exit = 0;
	u16 buttons, oldButtons = 0xffff, pressedButtons;

	FadeAndCleanUp();
	
	while(!exit)
	{
		if(loadvram)
		{
			DrawMainBG();
			VDP_Start();
			VDP_setScreenWidth320();
			VDP_drawTextBG(APLAN, "Options", TILE_ATTR(PAL1, 0, 0, 0), 16, 8);
			VDP_End();
			loadvram = 0;
		}

		DrawResolution();

		VDP_Start();
		VDP_drawTextBG(APLAN, "Enable Horizontal 256:", TILE_ATTR(sel == 0 ? PAL3 : PAL0, 0, 0, 0), 5, 11);
		VDP_drawTextBG(APLAN, enable_256 ? "ON " : "OFF", TILE_ATTR(sel == 0 ? PAL3 : PAL0, 0, 0, 0), 28, 11);
		VDP_drawTextBG(APLAN, "Enable 480i mode:", TILE_ATTR(sel == 1 ? PAL3 : PAL0, 0, 0, 0), 5, 12);
		VDP_drawTextBG(APLAN, VDP_Detect_Interlace()? "ON " : "OFF", TILE_ATTR(sel == 1 ? PAL3 : PAL0, 0, 0, 0), 28, 12);
		VDP_drawTextBG(APLAN, "Enable 240 in PAL:", TILE_ATTR(sel == 2 ? PAL3 : PAL0, 0, 0, 0), 5, 13);
		VDP_drawTextBG(APLAN, pal_240 ? "ON " : "OFF", TILE_ATTR(sel == 2 ? PAL3 : PAL0, 0, 0, 0), 28, 13);

		VDP_drawTextBG(APLAN, "Back", TILE_ATTR(sel == 3 ? PAL3 : PAL0, 0, 0, 0), 5, 18);

		VDP_End();
		
		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_VIDEO))
			loadvram = 1;

		if(pressedButtons & BUTTON_START)
			exit = 1;

		if(pressedButtons & BUTTON_UP)
		{
			sel--;
			if(sel < 0)
				sel = 3;
		}

		if(pressedButtons & BUTTON_DOWN)
		{
			sel++;
			if(sel > 3)
				sel = 0;
		}
		
		if(pressedButtons & BUTTON_LEFT || 
			pressedButtons & BUTTON_RIGHT ||
			pressedButtons & BUTTON_A)
		{
			if(sel == 0)
			{
				enable_256 = !enable_256;
			}
			
			if(sel == 1)
			{
				VDP_Start();
				if(VDP_Detect_Interlace())
					VDP_setScanMode(INTERLACED_NONE);
				else
					VDP_setScanMode(INTERLACED_MODE1);
				VDP_End();
			}

			if(sel == 2 && IsPALVDP)
			{
				if(IsPALVDP)
				{
					VDP_Start();
					if(!pal_240)
					{
						VDP_setScreenHeight240();
						pal_240 = 1;
					}
					else
					{
						VDP_setScreenHeight224();
						pal_240 = 0;
					}
					VDP_End();
				}
			}
		}
			
		if((pressedButtons & BUTTON_A && sel == 3) ||
			pressedButtons & BUTTON_Y || pressedButtons & BUTTON_START)
		{
			exit = 1;
		}

		VDP_waitVSync();
	}
	
	FadeAndCleanUp();
}

u8 CheckHelpAndVO(u16 *buttons, u16 *pressedButtons, int option)
{
	if(joytype != JOY_TYPE_PAD6)
	{
		if(option != HELP_VIDEO && *buttons & BUTTON_UP && *buttons & BUTTON_START)
		{
			*buttons = 0;
			*pressedButtons = 0;
			VideoOptions();
			return 1;
		}

		if(*buttons & BUTTON_DOWN && *buttons & BUTTON_START)
		{
			*buttons = 0;
			*pressedButtons = 0;
			DrawHelp(option);
			return 1;
		}
	}
	else
	{
		if(option != HELP_VIDEO && *pressedButtons & BUTTON_Y)
		{
			*buttons = 0;
			*pressedButtons = 0;
			VideoOptions();
			return 1;
		}
		if(*pressedButtons & BUTTON_Z)
		{
			*buttons = 0;
			*pressedButtons = 0;
			DrawHelp(option);
			return 1;
		}
	}
	
	return 0;
}

void DrawHelpText()
{
	VDP_Start();
	if(joytype != JOY_TYPE_PAD6)
		VDP_drawTextBG(APLAN, "Press DOWN+START for help", TILE_ATTR(PAL0, 0, 0, 0), 7, 24);
	else
		VDP_drawTextBG(APLAN, "Press Z for help", TILE_ATTR(PAL0, 0, 0, 0), 11, 24);
	VDP_End();
}

void StopPSG()
{
	PSG_setEnvelope(0, PSG_ENVELOPE_MIN);
	PSG_setEnvelope(1, PSG_ENVELOPE_MIN);
	PSG_setEnvelope(2, PSG_ENVELOPE_MIN);
	PSG_setEnvelope(3, PSG_ENVELOPE_MIN);
}

void DrawMainBG()
{
	DrawMainBGwithGillian(0, 0, 0);
	VDP_Start();
	VDP_setPalette(PAL1, palette_green);
	VDP_setPalette(PAL3, palette_red);
	VDP_End();
}

void DrawMainBGwithGillian(u8 DrawGillian, u8 GillianX, u8 GillianY)
{
	u16 ind = 0, size = 0;
	
	VDP_Start();
	VDP_setPalette(PAL0, palette_grey);
	VDP_setPalette(PAL1, palette_red);
	VDP_setPalette(PAL2, back_pal);
	if(DrawGillian)
		VDP_setPalette(PAL3, gillian_pal);

	ind = TILE_USERINDEX;
	size = sizeof(back_tiles) / 32;
	VDP_loadTileData(back_tiles, ind, size, USE_DMA);
	if(DrawGillian)
	{
		ind += size;
		size = sizeof(gillian_tiles) / 32;
		VDP_loadTileData(gillian_tiles, ind, size, USE_DMA);
	}

	VDP_setMyTileMapRect(BPLAN, back_map, TILE_USERINDEX, 0, 0, 320 / 8, 224 / 8);
	if(DrawGillian)
		VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL3, 0, 0, 0) + ind, GillianX / 8, GillianY / 8, 56 / 8, 104 / 8);
	VDP_End();
}

u16 DrawFloatMenuRes(u16 def)
{
	if(!enable_256)
		return RES_320;
		
	return(DrawFloatMenuResExtra(def, NULL));
}

u16 DrawFloatMenuResExtra(u16 def, char *option)
{
	fmenudata resmenudata[] = { {FLOAT_CANCEL, "Display Mode"}, {RES_320, "320x224"}, {RES_256, "256x224"}, {FLOAT_OPTION, NULL} };
	
	if(!enable_256 && option)
		return FLOAT_OPTION;
		
	if(option)
		resmenudata[FLOAT_OPTION].name = option;
	
	return(DrawFloatMenu(def, resmenudata, option != NULL ? 4 : 3));
}

u16 DrawFloatMenu(u16 def, fmenudata *data, u16 size)
{
	int	i = 0, sel = 1;
	u16 len = 0, ix, iy;
	u16 redraw = 1, loadvram = 1, exit = 0;
	u16 buttons, pressedButtons, oldButtons = 0xffff;
	u8 x = 100, y = 72; 
	
	sel = def;
	CleanUp();
	
	x = (VDP_getScreenWidth() - 128)/ 2;
	y = (VDP_getScreenHeight() - 80)/ 2;
	
	while(!exit)
	{
		if(loadvram)
		{
			VDP_Start();
			VDP_setPalette(PAL0, palette_grey);
			VDP_setPalette(PAL1, palette_red);
			VDP_setPalette(PAL2, palette_green);

			VDP_loadTileData(float_tiles, TILE_USERINDEX, sizeof(float_tiles) / 32, USE_DMA);
			VDP_setMyTileMapRect(BPLAN, float_map, TILE_USERINDEX, x/8, y/8, 128/8, 80/8);

			// Title at index 0
			len = strlen(data[0].name);	
			ix = (128/8 - len)/2 + x/8;
			iy = y/8;

			VDP_drawTextBG(APLAN, data[0].name, TILE_ATTR(PAL2, 0, 0, 0), ix, iy);
			VDP_End();
			
			redraw = 1;
		}
			
		if(redraw)
		{
			VDP_Start();
			
			for(i = 1; i < size; i++)
			{	
				len = strlen(data[i].name);
				
				ix = (128/8 - len)/2 + x/8;
				iy = y/8+2+i+(6-size)/2;
				
				VDP_drawTextBG(APLAN, data[i].name, TILE_ATTR(sel == i ? PAL1 : PAL0, 0, 0, 0), ix, iy);
			}
			
			VDP_End();
			redraw = 0;
		}
		
		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;
		
		if(pressedButtons & BUTTON_UP)
		{
			sel --;
			if(sel < 1)
				sel = size - 1;
			redraw = 1;
		}

		if(pressedButtons & BUTTON_DOWN)
		{
			sel ++;
			if(sel > size - 1)
				sel = 1;
			redraw = 1;
		}
		
		if(pressedButtons & BUTTON_A)
			exit = 1;
			
		if(pressedButtons & BUTTON_START)
		{
			CleanUp();
			return FLOAT_CANCEL;
		}
			
		VDP_waitVSync();
	}
	
	CleanUp();
	return data[sel].id;
}

void resetController(u16 *oldButtons)
{
	if(!oldButtons)
		return;
	
	*oldButtons |= BUTTON_A;
	*oldButtons |= BUTTON_START;
}

u16 CheckController(u16 joypad, u16 oldButtons, u16 y)
{
	u16	buttons, pos = 0, x = 6, type = JOY_TYPE_UNKNOWN;
	
	if(joypad == JOY_1)
		type = joytype;
	else
		type = joytype2;
		
	if(type != JOY_TYPE_PAD3 && type != JOY_TYPE_PAD6)
		return 0;
		
	buttons = JOY_readJoypad(joypad);
	
	VDP_Start();
	VDP_drawTextBG(APLAN, "Up", TILE_ATTR(buttons & BUTTON_UP ? PAL3 : PAL0, 0, 0, 0), x+4, y);
	VDP_drawTextBG(APLAN, "Left", TILE_ATTR(buttons & BUTTON_LEFT ? PAL3 : PAL0, 0, 0, 0), x, y+1);
	VDP_drawTextBG(APLAN, "Right", TILE_ATTR(buttons & BUTTON_RIGHT ? PAL3 : PAL0, 0, 0, 0), x+6, y+1);
	VDP_drawTextBG(APLAN, "Down", TILE_ATTR(buttons & BUTTON_DOWN ? PAL3 : PAL0, 0, 0, 0), x+3, y+2);
	
	VDP_drawTextBG(APLAN, "Start", TILE_ATTR(buttons & BUTTON_START ? PAL3 : PAL0, 0, 0, 0), x+13, y+1);
		
	if(type == JOY_TYPE_PAD6)
	{
		VDP_drawTextBG(APLAN, "X", TILE_ATTR(buttons & BUTTON_X ? PAL3 : PAL0, 0, 0, 0), x+21, y);
		VDP_drawTextBG(APLAN, "Y", TILE_ATTR(buttons & BUTTON_Y ? PAL3 : PAL0, 0, 0, 0), x+23, y);
		VDP_drawTextBG(APLAN, "Z", TILE_ATTR(buttons & BUTTON_Z ? PAL3 : PAL0, 0, 0, 0), x+25, y);
		VDP_drawTextBG(APLAN, "M", TILE_ATTR(buttons & BUTTON_MODE ? PAL3 : PAL0, 0, 0, 0), x+27, y-1);
		
		pos = y+2;
	}
	else
		pos = y+1;
	

	VDP_drawTextBG(APLAN, "A", TILE_ATTR(buttons & BUTTON_A ? PAL3 : PAL0, 0, 0, 0), x+21, pos);
	VDP_drawTextBG(APLAN, "B", TILE_ATTR(buttons & BUTTON_B ? PAL3 : PAL0, 0, 0, 0), x+23, pos);
	VDP_drawTextBG(APLAN, "C", TILE_ATTR(buttons & BUTTON_C ? PAL3 : PAL0, 0, 0, 0), x+25, pos);
	
	VDP_updateSprites();
	VDP_End();
	
	return buttons;
}

void ControllerTest()
{
	u8 loadvram = 1, redraw = 1, refresh = 0;
	u8 oldJoyType1 = JOY_TYPE_UNKNOWN, oldJoyType2 = JOY_TYPE_UNKNOWN;
	u16 exit = 0;
	u16 buttons1 = 0xffff, buttons2 = 0xffff;
	
	JOY_init();
	
	while(!exit)
	{	
		oldJoyType1 = joytype;
		oldJoyType2 = joytype2;
		
		if(loadvram)
		{
			DrawMainBG();

			loadvram = 0;
			redraw = 1;
			
		}
		if(redraw)
		{
			VDP_Start();
			VDP_drawTextBG(APLAN, "Controller Test", TILE_ATTR(PAL1, 0, 0, 0), 12, 7);
			VDP_drawTextBG(APLAN, "Use START+RIGHT to reset ports", TILE_ATTR(PAL0, 0, 0, 0), 5, 22);
			VDP_drawTextBG(APLAN, "Use START+LEFT to exit", TILE_ATTR(PAL1, 0, 0, 0), 9, 24);
			VDP_End();
		}
		buttons1 = CheckController(JOY_1, buttons1, 10);
		buttons2 = CheckController(JOY_2, buttons2, 16);

		// self explanatory, no help?
		//if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_CONTROLLER))
			//loadvram = 1;

		if((buttons1 & BUTTON_LEFT && buttons1 & BUTTON_START) ||
			(buttons2 & BUTTON_LEFT && buttons2 & BUTTON_START))
			exit = 1;
			
		if((buttons1 & BUTTON_RIGHT && buttons1 & BUTTON_START) || 
			(buttons2 & BUTTON_RIGHT && buttons2 & BUTTON_START))
		{
			joytype = joytype2 = JOY_TYPE_UNKNOWN;
			JOY_init();
		}
		
		if(oldJoyType1 != joytype || oldJoyType2 != joytype2)
			refresh = 1;
			
		if(refresh)
		{
			VDP_Start();
			VDP_clearTileMapRect(APLAN, 0, 0, 320 / 8, 224 / 8);
			VDP_End();
			redraw = 1;
			refresh = 0;
		}

		VDP_waitVSync();
	}
}

