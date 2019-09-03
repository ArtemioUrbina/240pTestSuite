/* 
 * 240p Test Suite
 * Copyright (C)2011-2014 Artemio Urbina
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

u8 joytype = JOY_TYPE_UNKNOWN;
u8 IsPALVDP = 0;
u8 VDPChanged = 0;

void VBlankIntCallback();
void DrawResolution();
void VideoOptions();

int main()
{
	u16 cursel = 1, pos, reload = 1;
	u16 buttons, oldButtons = 0xffff, pressedButtons;
	u16 ind = 0, size = 0; // hasSCD = 0;
	char md_ver[30];

	VDP_init();
	JOY_init();

	SYS_setVIntCallback(VBlankIntCallback);
	enable_256 = 0;
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

	/*
	if(DetectSCDBIOS())
	{
		//InitSCD_C();
		hasSCD = 1;
	}
	*/
	
	VDP_loadFontData(font_tiles, FONT_LEN, USE_DMA);
	DrawIntro();
	while(1)
	{
		if(reload)
		{
			VDP_setPalette(PAL2, back_pal);
			VDP_setPalette(PAL3, gillian_pal);

			ind = TILE_USERINDEX;
			size = sizeof(back_tiles) / 32;
			VDP_loadTileData(back_tiles, ind, size, USE_DMA);
			ind += size;
			size = sizeof(gillian_tiles) / 32;
			VDP_loadTileData(gillian_tiles, ind, size, USE_DMA);

			VDP_setMyTileMapRect(BPLAN, back_map, TILE_USERINDEX, 0, 0, 320 / 8, 224 / 8);
			VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL3, 0, 0, 0) + ind, 216 / 8, 80 / 8, 56 / 8, 104 / 8);
			reload = 0;
		}

		if(VDPChanged)
		{
			if(cursel == 13)
				cursel = 14;
		}

		pos = 6;
		VDP_drawTextBG(APLAN, "Test Patterns", TILE_ATTR(cursel == 1 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Drop Shadow Test", TILE_ATTR(cursel == 2 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Striped Sprite Test", TILE_ATTR(cursel == 3 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Lag Test", TILE_ATTR(cursel == 4 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Manual Lag Test", TILE_ATTR(cursel == 5 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Scroll Test", TILE_ATTR(cursel == 6 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Grid Scroll Test", TILE_ATTR(cursel == 7 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Horizontal Stripes", TILE_ATTR(cursel == 8 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Checkerboard", TILE_ATTR(cursel == 9 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Backlit Zone Test", TILE_ATTR(cursel == 10 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Alternate 240p/480i", TILE_ATTR(cursel == 11 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Sound Test", TILE_ATTR(cursel == 12 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Audio Sync Test", TILE_ATTR(cursel == 13 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "MDFourier", TILE_ATTR(cursel == 14 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Help", TILE_ATTR(cursel == 15 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Options", TILE_ATTR(cursel == 16 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Credits", TILE_ATTR(cursel == 17 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		//VDP_drawTextBG(APLAN, "SCD RAM Check", TILE_ATTR(cursel == 18 ? PAL1 : PAL0, 0, 0, 0), 5, ++pos);
			
		DrawResolution();

		Detect_MD(md_ver);
		VDP_drawTextBG(APLAN, md_ver, TILE_ATTR(PAL0, 0, 0, 0), 20, 26);

		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_GENERAL))
			reload = 1;

		if(pressedButtons & BUTTON_DOWN)
		{
			cursel++;
			if(cursel > pos - 6)
				cursel = 1;
		}

		if(pressedButtons & BUTTON_UP)
		{
			cursel--;
			if(cursel < 1)
				cursel = pos - 6;
		}
		
		if(pressedButtons & BUTTON_RIGHT)
		{
			TestPatternMenu();
			FadeAndCleanUp();
			reload = 1;
		}

		if(pressedButtons & BUTTON_A)
		{
			FadeAndCleanUp();
			switch (cursel)
			{
			case 1:
				TestPatternMenu();
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
				HScrollTest();
				break;
			case 7:
				VScrollTest();
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
				Alternate240p480i();
				break;
			case 12:
				SoundTest();
				break;
			case 13:
				AudioSyncTest();
				break;
			case 14:
				MDFourier();
				break;
			case 15:
				DrawHelp(HELP_GENERAL);
				break;
			case 16:
				VideoOptions();
				break;
			case 17:
				DrawCredits();
				break;
			/*
			case 18:
				SegaCDRAMCheck(hasSCD);
				break;
			*/
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
	u16 ind = 0, size = 0, done = 0;
	char md_ver[30];

	FadeAndCleanUp();
	while(!done)
	{
		if(reload)
		{
			VDP_setPalette(PAL2, back_pal);
			VDP_setPalette(PAL3, gillian_pal);

			ind = TILE_USERINDEX;
			size = sizeof(back_tiles) / 32;
			VDP_loadTileData(back_tiles, ind, size, USE_DMA);
			ind += size;
			size = sizeof(gillian_tiles) / 32;
			VDP_loadTileData(gillian_tiles, ind, size, USE_DMA);

			VDP_setMyTileMapRect(BPLAN, back_map, TILE_USERINDEX, 0, 0, 320 / 8, 224 / 8);
			VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL3, 0, 0, 0) + ind, 216 / 8, 80 / 8, 56 / 8, 104 / 8);

			reload = 0;
		}

		pos = 6;
		VDP_drawTextBG(APLAN, "Pluge", TILE_ATTR(cursel == 1 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Color Bars", TILE_ATTR(cursel == 2 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		if(!IsPALVDP)
			VDP_drawTextBG(APLAN, "SMPTE Color Bars", TILE_ATTR(cursel == 3 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		else
			VDP_drawTextBG(APLAN, "EBU Color Bars", TILE_ATTR(cursel == 3 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Color Bars with Gray Reference", TILE_ATTR(cursel == 4 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Color Bleed Check", TILE_ATTR(cursel == 5 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, pal_240 ? "Grid 320x240" : "Grid 320x224", TILE_ATTR(cursel == 6 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, pal_240 ? "Grid 256x240" : "Grid 256x224", TILE_ATTR(cursel == 7 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Linearity 320x224", TILE_ATTR(cursel == 8 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Linearity 256x224", TILE_ATTR(cursel == 9 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Gray Ramp", TILE_ATTR(cursel == 10 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "White & RGB Screens", TILE_ATTR(cursel == 11 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "100 IRE", TILE_ATTR(cursel == 12 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Sharpness", TILE_ATTR(cursel == 13 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Overscan", TILE_ATTR(cursel == 14 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Video Options", TILE_ATTR(cursel == 15 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Back to Main Menu", TILE_ATTR(cursel == 16 ? PAL1 : PAL0, 0, 0, 0), 5, ++pos);
		DrawResolution();
		Detect_MD(md_ver);
		VDP_drawTextBG(APLAN, md_ver, TILE_ATTR(PAL0, 0, 0, 0), 20, 26);
		
		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_GENERAL))
			reload = 1;

		if(pressedButtons & BUTTON_DOWN)
		{
			cursel++;
			if(cursel > 16)
				cursel = 1;
		}

		if(pressedButtons & BUTTON_UP)
		{
			cursel--;
			if(cursel < 1)
				cursel = 16;
		}

		if(pressedButtons & BUTTON_B || 
			pressedButtons & BUTTON_START || 
			pressedButtons & BUTTON_LEFT)
		{
			done = 1;
		}

		if(pressedButtons & BUTTON_A)
		{
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
				DrawSMPTE();
				break;
			case 4:
				Draw601ColorBars();
				break;
			case 5:
				DrawColorBleed();
				break;
			case 6:
				DrawGrid(GRID_320);
				break;
			case 7:
				DrawGrid(GRID_256);
				break;
			case 8:
				DrawLinearity();
				break;
			case 9:
				DrawLinearity256();
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
				VideoOptions();
				break;
			case 16:
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

void DrawIntro()
{
	VDP_setPalette(PAL0, palette_black);
	VDP_drawTextBG(APLAN, "KORDAMP PRESENTS", TILE_ATTR(PAL0, 0, 0, 0), 12, 12);
	VDP_fadeIn(0, 15, palette_grey, 60, 1);
	VDP_waitFadeCompletion();
	VDP_fadeOutAll(40, 0);
	VDP_resetScreen();
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

//#define DBELEC

void DrawN()
{
	u16 size;
	u16 exit = 0;
	u16 buttons;

	
	FadeAndCleanUp();
	size = sizeof(n_tiles) / 32;
	VDP_setPalette(PAL0, n_pal);
	VDP_loadTileData(n_tiles, TILE_USERINDEX, size, USE_DMA);
	VDP_setMyTileMapRect(BPLAN, n_map, TILE_USERINDEX, 120 / 8, 72 / 8, 80 / 8, 80 / 8);
	
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
	u16 ind, size, exit = 0, pos = 0, counter = 0;
	u16 buttons, oldButtons = 0xffff, pressedButtons, loadvram = 1;

	FadeAndCleanUp();
	
	while(!exit)
	{
		if(loadvram)
		{
			ind = 0;
			size = 0;
			pos = 8;
			counter = 1;
			
#ifdef SEGACD
			pos = 6;
#endif

#ifdef DBELEC
			pos = 7;
#endif
			VDP_setPalette(PAL0, palette_grey);
			VDP_setPalette(PAL1, palette_green);
			VDP_setPalette(PAL2, back_pal);
			VDP_setPalette(PAL3, bw_pal);

			ind = TILE_USERINDEX;
			size = sizeof(back_tiles) / 32;
			VDP_loadTileData(back_tiles, ind, size, USE_DMA);

			VDP_setMyTileMapRect(BPLAN, back_map, TILE_USERINDEX, 0, 0, 320 / 8, 224 / 8);

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
	
#ifdef DBELEC
			VDP_drawTextBG(APLAN, "Cartridge and distribution by:", TILE_ATTR(PAL1, 0, 0, 0), 4, pos++);
			VDP_drawTextBG(APLAN, "http://db-electronics.ca/", TILE_ATTR(PAL0, 0, 0, 0), 5, pos++);
#endif

			VDP_drawTextBG(APLAN, "Ver. 1.18", TILE_ATTR(PAL1, 0, 0, 0), 26, 6);
			VDP_drawTextBG(APLAN, "03/09/2019", TILE_ATTR(PAL0, 0, 0, 0), 26, 7);
			
			VDP_drawTextBG(BPLAN, "Dedicated to Elisa", TILE_ATTR(PAL0, 0, 0, 0), 18, 24);

#ifdef SEGACD
			pos = 7;
#else
#ifdef DBELEC
			pos = 8;
#else
			pos = 9;
#endif
#endif
			loadvram = 0;
		}

		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		if(counter == 1)
			VDP_drawTextBG(APLAN, "Artemio Urbina      ", TILE_ATTR(PAL0, 0, 0, 0), 5, pos);
		if(counter == 60 * 4)
			VDP_drawTextBG(APLAN, "@Artemio (twitter)  ", TILE_ATTR(PAL0, 0, 0, 0), 5, pos);
		if(counter == 60 * 8)
			VDP_drawTextBG(APLAN, "aurbina@junkerhq.net", TILE_ATTR(PAL0, 0, 0, 0), 5, pos);
		if(counter == 60 * 16)
			counter = 0;

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
	VDP_resetSprites();
	VDP_updateSprites();
	VDP_setHorizontalScroll(PLAN_B, 0);
	VDP_setHorizontalScroll(PLAN_A, 0);
	VDP_setVerticalScroll(PLAN_A, 0);
	VDP_setHilightShadow(0);
	VDP_clearTileMapRect(BPLAN, 0, 0, 320 / 8, 224 / 8);
	VDP_clearTileMapRect(APLAN, 0, 0, 320 / 8, 224 / 8);
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
	u16 ind = 0, size = 0, exit = 0;
	u16 buttons, oldButtons = 0xffff, pressedButtons;

	FadeAndCleanUp();
	
	while(!exit)
	{
		if(loadvram)
		{
			VDP_setScreenWidth320();
			
			VDP_setPalette(PAL0, palette_grey);
			VDP_setPalette(PAL1, palette_green);
			VDP_setPalette(PAL2, back_pal);
			VDP_setPalette(PAL3, palette_red);

			ind = TILE_USERINDEX;
			size = sizeof(back_tiles) / 32;
			VDP_loadTileData(back_tiles, ind, size, USE_DMA);

			VDP_setMyTileMapRect(BPLAN, back_map, TILE_USERINDEX, 0, 0, 320 / 8, 224 / 8);
			loadvram = 0;
		}

		VDP_drawTextBG(APLAN, "Video Options", TILE_ATTR(PAL1, 0, 0, 0), 14, 8);

		DrawResolution();

		VDP_drawTextBG(APLAN, "Enable Horizontal 256:", TILE_ATTR(sel == 0 ? PAL3 : PAL0, 0, 0, 0), 5, 11);
		VDP_drawTextBG(APLAN, enable_256 ? "ON " : "OFF", TILE_ATTR(sel == 0 ? PAL3 : PAL0, 0, 0, 0), 28, 11);
		VDP_drawTextBG(APLAN, "Enable 480i mode:", TILE_ATTR(sel == 1 ? PAL3 : PAL0, 0, 0, 0), 5, 12);
		VDP_drawTextBG(APLAN, VDP_Detect_Interlace()? "ON " : "OFF", TILE_ATTR(sel == 1 ? PAL3 : PAL0, 0, 0, 0), 28, 12);
		VDP_drawTextBG(APLAN, "Enable 240 in PAL:", TILE_ATTR(sel == 2 ? PAL3 : PAL0, 0, 0, 0), 5, 13);
		VDP_drawTextBG(APLAN, pal_240 ? "ON " : "OFF", TILE_ATTR(sel == 2 ? PAL3 : PAL0, 0, 0, 0), 28, 13);

		VDP_drawTextBG(APLAN, "Back", TILE_ATTR(sel == 3 ? PAL3 : PAL0, 0, 0, 0), 5, 16);

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
				if(VDP_Detect_Interlace())
					VDP_setScanMode(INTERLACED_NONE);
				else
					VDP_setScanMode(INTERLACED_MODE1);
			}

			if(sel == 2 && IsPALVDP)
			{
				if(IsPALVDP)
				{
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
	
	VDP_fadeOutAll(FADE_TIME, 0);
	VDP_clearTileMapRect(APLAN, 0, 0, 320 / 8, 224 / 8);
	VDP_clearTileMapRect(BPLAN, 0, 0, 320 / 8, 224 / 8);
	VDP_resetScreen();
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
			VideoOptions();
			return 1;
		}
		if(*pressedButtons & BUTTON_Z)
		{
			DrawHelp(option);
			return 1;
		}
	}
	return 0;
}

void DrawHelpText()
{
	if(joytype != JOY_TYPE_PAD6)
		VDP_drawTextBG(APLAN, "Press B+DOWN for help", TILE_ATTR(PAL0, 0, 0, 0), 9, 24);
	else
		VDP_drawTextBG(APLAN, "Press Z for help", TILE_ATTR(PAL0, 0, 0, 0), 11, 24);
}

void StopPSG()
{
	PSG_setEnvelope(0, PSG_ENVELOPE_MIN);
	PSG_setEnvelope(1, PSG_ENVELOPE_MIN);
	PSG_setEnvelope(2, PSG_ENVELOPE_MIN);
	PSG_setEnvelope(3, PSG_ENVELOPE_MIN);
}


/**********************************/

int memcmp(const void *s1, const void *s2, int n)
{
    unsigned char u1, u2;

    for ( ; n-- ; s1++, s2++) {
	u1 = * (unsigned char *) s1;
	u2 = * (unsigned char *) s2;
	if ( u1 != u2) {
	    return (u1-u2);
	}
    }
    return 0;
}

// from ChillyWilly Example of Mode1 playback
/*
 * Check for CD BIOS
 * When a cart is inserted in the MD, the CD hardware is mapped to
 * 0x400000 instead of 0x000000. So the BIOS ROM is at 0x400000, the
 * Program RAM bank is at 0x420000, and the Word RAM is at 0x600000.
 */
 
int DetectSCDBIOS()
{
    uint8_t *bios;
	
    bios = (uint8_t *)0x415800;
    if (memcmp(bios + 0x6D, "SEGA", 4))
    {
        bios = (uint8_t *)0x416000;
        if (memcmp(bios + 0x6D, "SEGA", 4))
        {
            // check for WonderMega/X'Eye
            if (memcmp(bios + 0x6D, "WONDER", 6))
            {
                bios = (uint8_t *)0x41AD00; // might also be 0x40D500
                // check for LaserActive
                if (memcmp(bios + 0x6D, "SEGA", 4))
					return 0;
            }
        }
    }
    return 1;
}

int ShowBIOSData()
{
    int possible[] = {0x415800, 0x416000, 0x41AD00, 0x40D500}, i;
	
	for(i = 0; i < 4; i++)
	{
		char buffer[10];
		uint8_t *bios = NULL;
		
		bios = (uint8_t*)possible[i];
	
		VDP_drawTextBG(APLAN, "BIOS 0x", TILE_ATTR(PAL0, 0, 0, 0), 4, 8+i*3);
		intToHex((int)bios, buffer, 6);
		VDP_drawTextBG(APLAN, buffer, TILE_ATTR(PAL0, 0, 0, 0), 11, 8+i*3);
		
		VDP_drawTextBG(APLAN, "ASCII:", TILE_ATTR(PAL1, 0, 0, 0), 19, 9+i*3);
		buffer[0] = bios[0x6D];
		buffer[1] = bios[0x6E];
		buffer[2] = bios[0x6F];
		buffer[3] = bios[0x70];
		buffer[4] = '\0';
		VDP_drawTextBG(APLAN, buffer, TILE_ATTR(PAL0, 0, 0, 0), 26, 9+i*3);
		
		VDP_drawTextBG(APLAN, "Hex:", TILE_ATTR(PAL1, 0, 0, 0), 21, 10+i*3);
		intToHex(bios[0x6D], buffer+0, 2);
		intToHex(bios[0x6E], buffer+2, 2);
		intToHex(bios[0x6F], buffer+4, 2);
		intToHex(bios[0x70], buffer+6, 2);
		VDP_drawTextBG(APLAN, buffer, TILE_ATTR(PAL0, 0, 0, 0), 26, 10+i*3);
	}
    return 1;
}

#define MEMORY_OK 0xfffff

int CheckSCDRegion(char value)
{
	int 			address = 0;
	unsigned char	*word_ram = (void*)0x600000;
	
	for(address = 0; address < 0x1ffff; address++)
		word_ram[address] = value;
	
	for(address = 0; address < 0x1ffff; address++)
	{
		if(word_ram[address] != 0)
			return address;
	}
	
	return MEMORY_OK;
}

int CheckSCDRAMWithValue(char * message, char value, int pos)
{
	int memoryFail = 0;
	
	
	VDP_drawTextBG(APLAN, message, TILE_ATTR(PAL1, 0, 0, 0), 14, pos);
	memoryFail = CheckSCDRegion(value);
	
	if(memoryFail != MEMORY_OK)
	{
		char buffer[10];
		
		VDP_drawTextBG(APLAN, "FAILED 0x", TILE_ATTR(PAL2, 0, 0, 0), 12, pos+1);
		intToHex(memoryFail, buffer, 6);
		VDP_drawTextBG(APLAN, buffer, TILE_ATTR(PAL2, 0, 0, 0), 21, pos+1);
		return 0;
	}
	
	VDP_drawTextBG(APLAN, "ALL OK", TILE_ATTR(PAL2, 0, 0, 0), 18, pos+1);
	return 1;
}

void write_byte(void *dst, unsigned char val)
{
    asm("movea.l 4(%sp),%a0");
    asm("move.l  8(%sp),%d0");
    asm("move.b  %d0,(%a0)");
}

void write_word(void *dst, unsigned short val)
{
    asm("movea.l 4(%sp),%a0");
    asm("move.l  8(%sp),%d0");
    asm("move.w  %d0,(%a0)");
}

unsigned char read_byte(void *src)
{
	return(*((unsigned char*)src));
}

// from ChillyWilly Example of Mode1 playback
void InitSCD()
{
	
	/*
	 * Reset the Gate Array - this specific sequence of writes is recognized by
	 * the gate array as a reset sequence, clearing the entire internal state -
	 * this is needed for the LaserActive
	 */
	write_word((void*)0xA12002, 0xFF00);
	write_byte((void*)0xA12001, 0x03);
	write_byte((void*)0xA12001, 0x02);
	write_byte((void*)0xA12001, 0x00);

    /*
     * Reset the Sub-CPU, request the bus
     */
    //write_byte((void*)0xA12001, 0x02);
    //while (!(read_byte((void*)0xA12001) & 2)) write_byte((void*)0xA12001, 0x02); // wait on bus acknowledge
	
    write_word((void*)0xA12002, 0x00FA); // no write-protection, bank 0, 2M mode, Word RAM assigned to Sub-CPU
	
	write_byte((void*)0xA12003, 0x00); // assign word RAM to main CPU
}

void InitSCD_C()
{
	unsigned char	*scd_reg1 = (void*)0xA12001;
	unsigned int	*scd_reg2 = (void*)0xA12002;
	unsigned char	*scd_reg3 = (void*)0xA12003;
	
	/*
	 * Reset the Gate Array - this specific sequence of writes is recognized by
	 * the gate array as a reset sequence, clearing the entire internal state -
	 * this is needed for the LaserActive
	 */
	*scd_reg2 = 0xFF00;
	*scd_reg1 = 0x03;
	*scd_reg1 = 0x02;
	*scd_reg1 = 0x00;

    /*
     * Reset the Sub-CPU, request the bus
     */
    //*scd_reg1 = 0x02;
    //while (!((*scd_reg1) & 2)) *scd_reg1 = 0x2; // wait on bus acknowledge
	
	*scd_reg2 = 0x00FA; // no write-protection, bank 0, 2M mode, Word RAM assigned to Sub-CPU
    *scd_reg3 = 0x00; // assign word RAM to main CPU
}

void SegaCDRAMCheck(int cdpresent)
{
	int loadvram = 1, ind = 0, size = 0;
	u16 exit = 0, redraw = 0;
	u16 buttons, oldButtons = 0xffff, pressedButtons;
	
	while(!exit)
	{
		if(loadvram)
		{
			VDP_setPalette(PAL0, palette_grey);
			VDP_setPalette(PAL1, palette_green);
			VDP_setPalette(PAL2, back_pal);
			VDP_setPalette(PAL3, palette_red);

			ind = TILE_USERINDEX;
			size = sizeof(back_tiles) / 32;
			VDP_loadTileData(back_tiles, ind, size, USE_DMA);

			VDP_setMyTileMapRect(BPLAN, back_map, TILE_USERINDEX, 0, 0, 320 / 8, 224 / 8);
			loadvram = 0;
			redraw = 1;
		}
		
		if(redraw)
		{
			if(cdpresent)
				VDP_drawTextBG(APLAN, "Sega CD RAM", TILE_ATTR(PAL1, 0, 0, 0), 14, 6);
			else
				ShowBIOSData();
				
			redraw = 0;
		}

		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		//if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_SOUND))
			//loadvram = 1;

		if(pressedButtons & BUTTON_START)
			exit = 1;

		if(pressedButtons & BUTTON_A && cdpresent)
		{
			int 			close = 0;
			
			CheckSCDRAMWithValue("Setting to Zero", 0x00, 8);
			CheckSCDRAMWithValue("Setting to 0xAA", 0xAA, 10);
			CheckSCDRAMWithValue("Setting to 0x55", 0x55, 12);
			CheckSCDRAMWithValue("Setting to 0xFF", 0xFF, 14);

			VDP_drawTextBG(APLAN, "PRESS A", TILE_ATTR(PAL1, 0, 0, 0), 14, 18);
			do
			{
				buttons = JOY_readJoypad(JOY_1);
				pressedButtons = buttons & ~oldButtons;
				oldButtons = buttons;
				if(pressedButtons & BUTTON_A)
					close = 1;
				VDP_waitVSync();
			}while(!close);
			
			VDP_clearTileMapRect(APLAN, 0, 0, 320 / 8, 224 / 8);
			redraw = 1;
		}
		
		if(pressedButtons & BUTTON_B && cdpresent)
		{
			unsigned char *request = (void*)0xA12003;
			unsigned int *mode = (void*)0xA12002;
			
			// assign word RAM to sub CPU
			*request = 0x00;
			*mode = 0x00FA;
		}
			
		if(pressedButtons & BUTTON_C && cdpresent)
		{
			unsigned char *request = (void*)0xA12003;
			unsigned int *mode = (void*)0xA12002;
			
			// assign word RAM to sub CPU
			*request = 0x01;
			*mode = 0x00FA;
		}
		
		if(pressedButtons & BUTTON_X && cdpresent)
			ShowBIOSData();
			
		if(pressedButtons & BUTTON_Y && cdpresent)
			InitSCD();
			
		if(pressedButtons & BUTTON_Z && cdpresent)
			InitSCD_C();
		
		
		VDP_waitVSync();
	}
}
