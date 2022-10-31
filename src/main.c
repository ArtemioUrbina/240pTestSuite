/* 
 * 240p Test Suite for the Neo Geo
 * by Artemio Urbina and Dustin Dembrosky (Dasutin)
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdlib.h>
#include <stdio.h>
#include <DATlib.h>
#include <input.h>
#include "ng.h"
#include "tools.h"
#include "externs.h"
#include "patterns.h"
#include "tests.h"
#include "help.h"
#include "sound.h"
#include "string_ng.h"

bkp_ram_info bkp_data;

BYTE p1,p2,ps,pse,p1e,p2e,p1b,p2b;
BYTE isMVS, is4S, is6S, isMulti, hwChange, allowIRE107;
BYTE vmode_snk, isPAL, usePAL256, isPALinMVS, enable_shadow;

#define NUM_FONT_COLORS 9
static const ushort fixPalettes[]= {
	_BLACK, WH_100, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK,	// Used for BIOS Mask
	_BLACK, WH_100, 0x0333, 0x5fa7, 0xde85, 0x2c74, 0x2a52, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, // fontColorWhite
	_BLACK, RED100, 0x0300, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, // fontColorRed
	_BLACK, GRN100, 0x0030, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, // fontColorGreen
	_BLACK, BLU100, 0x0003, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, // fontColorBlue
	_BLACK, 0xf999, 0xf444, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, // fontColorGrayLight
	_BLACK, 0xf555, 0xf222, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, // fontColorGrayDark
	_BLACK, _BLACK, WH_100, 0xee51, 0x4f81, 0x4fa1, 0x4fc1, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, // fontColorBlack
	_BLACK, _BLACK, WH_100, 0x8333, 0xe7b0, 0xc580, 0xe250, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, // fontColorSolid
	//_BLACK, 0xefb8, 0x0111, 0xde96, 0x3c75, 0x2950, 0x4720, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK,  Unknown
	//_BLACK, 0x8444, 0x0111, 0xf555, 0xf666, 0x7777, 0x8888, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK,  Unknown
	//_BLACK, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  Unknown
};

void menu_tp()
{
	int done = 0, curse = 1, cursemax = 16, redraw = 1;
	blinker blinkdata;

	while (!done)
	{
		if (redraw)
		{
			gfxClear();
			draw_background_w_gil(&blinkdata);
			redraw = 0;
		}

		SCClose();
		waitVBlank();
		SD_blink_cycle(&blinkdata);

		readController();

		if (PRESSED_UP)		curse=curse>1?curse-1:cursemax;
		if (PRESSED_DOWN)	curse=curse<cursemax?curse+1:1;

		fixPrint(5, 8, curse == 1 ? fontColorRed : fontColorWhite, 3, "Pluge");
		fixPrint(5, 9, curse == 2 ? fontColorRed : fontColorWhite, 3, "Color Bars");
		fixPrint(5, 10, curse == 3 ? fontColorRed : fontColorWhite, 3, "EBU Color Bars");
		fixPrint(5, 11, curse == 4 ? fontColorRed : fontColorWhite, 3, "SMPTE Color Bars");
		fixPrint(5, 12, curse == 5 ? fontColorRed : fontColorWhite, 3, "Referenced Color Bars");
		fixPrint(5, 13, curse == 6 ? fontColorRed : fontColorWhite, 3, "Color Bleed Check");
		fixPrint(5, 14, curse == 7 ? fontColorRed : fontColorWhite, 3, "Monoscope");
		fixPrint(5, 15, curse == 8 ? fontColorRed : fontColorWhite, 3, "Grid");
		fixPrint(5, 16, curse == 9 ? fontColorRed : fontColorWhite, 3, "Gray Ramp");
		fixPrint(5, 17, curse == 10 ? fontColorRed : fontColorWhite, 3, "White & RGB Screens");
		fixPrint(5, 18, curse == 11 ? fontColorRed : fontColorWhite, 3, "100 IRE");
		fixPrint(5, 19, curse == 12 ? fontColorRed : fontColorWhite, 3, "Sharpness");
		fixPrint(5, 20, curse == 13 ? fontColorRed : fontColorWhite, 3, "Overscan");
		fixPrint(5, 21, curse == 14 ? fontColorRed : fontColorWhite, 3, "Convergence");

		fixPrint(5, 23, curse == 15 ? fontColorRed : fontColorWhite, 3, "Help");
		fixPrint(5, 24, curse == 16 ? fontColorRed : fontColorWhite, 3, "Back to Main Menu");

		menu_footer();

		if (BTTN_EXIT)
			done = 1;

		if (checkHelp(HELP_GENERAL))
			redraw = 1;

		if (BTTN_MAIN)
		{
			gfxClear();
			switch (curse)
			{
				case 1:
					tp_pluge();
				break;
					
				case 2:
					tp_colorchart();
				break;

				case 3:
					tp_colorbars();
				break;

				case 4:
					tp_smpte_color_bars();
				break;

				case 5:
					tp_ref_color_bars();
				break;

				case 6:
					tp_color_bleed_check();
				break;

				case 7:
					tp_monoscope();
				break;

				case 8:
					tp_grid();
				break;

				case 9:
					tp_gray_ramp();
				break;

				case 10:
					tp_white_rgb();
				break;

				case 11:
					tp_100_ire();
				break;

				case 12:
					tp_sharpness();
				break;

				case 13:
					tp_overscan();
				break;

				case 14:
					tp_convergence();
				break;

				case 15:
					DrawHelp(HELP_GENERAL);
				break;

				case 16:
					done = 1;
				break;
			}
			redraw = 1;
		}
	}
	return;
}

void menu_vt()
{
	int done = 0, curse = 1, cursemax = 12, redraw = 1;
	blinker blinkdata;

	while (!done)
	{
		if (redraw)
		{
			gfxClear();
			draw_background_w_gil(&blinkdata);
			redraw = 0;
		}

		SCClose();
		waitVBlank();
		SD_blink_cycle(&blinkdata);

		readController();

		if (PRESSED_UP)		curse=curse>1?curse-1:cursemax;
		if (PRESSED_DOWN)	curse=curse<cursemax?curse+1:1;

		fixPrint(5, 10, curse == 1 ? fontColorRed : fontColorWhite, 3, "Drop Shadow Test");
		fixPrint(5, 11, curse == 2 ? fontColorRed : fontColorWhite, 3, "Striped Sprite Test");
		fixPrint(5, 12, curse == 3 ? fontColorRed : fontColorWhite, 3, "Lag Test");
		fixPrint(5, 13, curse == 4 ? fontColorRed : fontColorWhite, 3, "Timing & Reflex Test");
		fixPrint(5, 14, curse == 5 ? fontColorRed : fontColorWhite, 3, "Scroll Test");
		fixPrint(5, 15, curse == 6 ? fontColorRed : fontColorWhite, 3, "Grid Scroll Test");
		fixPrint(5, 16, curse == 7 ? fontColorRed : fontColorWhite, 3, "Horizontal Stripes");
		fixPrint(5, 17, curse == 8 ? fontColorRed : fontColorWhite, 3, "Vertical Stripes");
		fixPrint(5, 18, curse == 9 ? fontColorRed : fontColorWhite, 3, "Checkerboard");
		fixPrint(5, 19, curse == 10 ? fontColorRed : fontColorWhite, 3, "Backlit Zone Test");

		fixPrint(5, 21, curse == 11 ? fontColorRed : fontColorWhite, 3, "Help");
		fixPrint(5, 22, curse == 12 ? fontColorRed : fontColorWhite, 3, "Back to Main Menu");

		menu_footer();

		if (BTTN_EXIT)
			done = 1;

		if (checkHelp(HELP_GENERAL))
			redraw = 1;

		if (BTTN_MAIN)
		{
			gfxClear();
			switch (curse)
			{
				case 1:
					vt_drop_shadow_test();
				break;
					
				case 2:
					vt_striped_sprite_test();
				break;

				case 3:
					vt_lag_test();
				break;

				case 4:
					vt_reflex_test();
				break;

				case 5:
					vt_scroll_test();
				break;

				case 6:
					vt_gridscroll_test();
				break;

				case 7:
					vt_horizontal_stripes();
				break;

				case 8:
					vt_vertical_stripes();
				break;

				case 9:
					vt_checkerboard();
				break;

				case 10:
					vt_backlitzone_test();
				break;

				case 11:
					DrawHelp(HELP_GENERAL);
				break;

				case 12:
					done = 1;
				break;
			}
			redraw = 1;
		}
	}
	return;
}

void menu_at()
{
	int done = 0, curse = 1, cursemax = 4, redraw = 1;
	blinker blinkdata;

	while (!done)
	{
		if (redraw)
		{
			gfxClear();
			draw_background_w_gil(&blinkdata);
			redraw = 0;
		}

		SCClose();
		waitVBlank();
		SD_blink_cycle(&blinkdata);

		readController();

		if (PRESSED_UP)		curse=curse>1?curse-1:cursemax;
		if (PRESSED_DOWN)	curse=curse<cursemax?curse+1:1;

		fixPrint(5, 14, curse == 1 ? fontColorRed : fontColorWhite, 3, "Sound Test");
		fixPrint(5, 15, curse == 2 ? fontColorRed : fontColorWhite, 3, "Audio Sync Test");
		fixPrint(5, 16, curse == 3 ? fontColorRed : fontColorWhite, 3, "MDFourier");

		fixPrint(5, 18, curse == 4 ? fontColorRed : fontColorWhite, 3, "Help");
		fixPrint(5, 19, curse == 5 ? fontColorRed : fontColorWhite, 3, "Back to Main Menu");

		menu_footer();

		if (BTTN_EXIT)
			done = 1;

		if (checkHelp(HELP_GENERAL))
			redraw = 1;

		if (BTTN_MAIN)
		{
			gfxClear();
			switch (curse)
			{
				case 1:
					at_sound_test();
				break;
								
				case 2:
					at_audiosync_test();
				break;

				case 3:
					at_sound_mdfourier();
				break;

				case 4:
					DrawHelp(HELP_GENERAL);
				break;

				case 5:
					done = 1;
				break;
			}
			redraw = 1;
		}
	}
	return;
}

void menu_ht()
{
	int done = 0, curse = 1, cursemax = 7, redraw = 1;
	blinker blinkdata;

	while (!done)
	{
		if (redraw)
		{
			gfxClear();
			draw_background_w_gil(&blinkdata);
			redraw = 0;
		}

		SCClose();
		waitVBlank();
		SD_blink_cycle(&blinkdata);

		readController();

		if (PRESSED_UP)		curse=curse>1?curse-1:cursemax;
		if (PRESSED_DOWN)	curse=curse<cursemax?curse+1:1;

		fixPrint(5, 13, curse == 1 ? fontColorRed : fontColorWhite, 3, "Controller Test");
		fixPrint(5, 14, curse == 2 ? fontColorRed : fontColorWhite, 3, "SDRAM Check");
		fixPrint(5, 15, curse == 3 ? fontColorRed : fontColorWhite, 3, "Memory Viewer");
		fixPrint(5, 16, curse == 4 ? fontColorRed : fontColorWhite, 3, "BIOS Info");
		fixPrint(5, 17, curse == 5 ? fontColorRed : fontColorWhite, 3, "Register View");

		fixPrint(5, 19, curse == 6 ? fontColorRed : fontColorWhite, 3, "Help");
		fixPrint(5, 20, curse == 7 ? fontColorRed : fontColorWhite, 3, "Back to Main Menu");

		menu_footer();

		if (BTTN_EXIT)
			done = 1;

		if (checkHelp(HELP_GENERAL))
			redraw = 1;

		if (BTTN_MAIN)
		{
			gfxClear();
			switch (curse)
			{
				case 1:
					ht_controller_test();
				break;
					
				case 2:
					
				break;

				case 3:
					ht_memory_viewer(0);
				break;

				case 4:
					ht_check_ng_bios_crc(BIOS_ADDRESS);
				break;

				case 5:
					ht_displayregs();
				break;

				case 6:
					DrawHelp(HELP_GENERAL);
				break;

				case 7:
					done = 1;
				break;
			}
			redraw = 1;
		}
	}
	return;
}

void credits()
{
	int done = 0, draw = 1;
	picture qr;
	short half_pal[16];

	gfxClear();
	draw_background();

	pictureInit(&qr, &barcode, 26, 17, 260, 80, FLIP_NONE);
	palJobPut(17,barcode.palInfo->count,barcode.palInfo->data);
	memcpy(half_pal, back.palInfo->data, sizeof(ushort)*16);
	darken_palette(half_pal, 4);
	palJobPut(16, 1, half_pal);

	while (!done)
	{
		if(draw)
		{
			int x = 4, y = 6;

			fixPrint(14, y-2, fontColorGreen, 3, "== Credits ==");
			fixPrint(x+24, y, fontColorGreen, 3, "Ver. 0.5");
			fixPrint(x+24, y+1, fontColorWhite, 3, "10/28/2022");
			fixPrint(x, y++, fontColorGreen, 3, "Code by:");
			fixPrint(x+1, y++, fontColorWhite, 3, "Dustin Dembrosky");
			fixPrint(x+1, y++, fontColorWhite, 3, "Artemio Urbina");
			fixPrint(x, y++, fontColorGreen, 3, "Monoscope:");
			fixPrint(x+1, y++, fontColorWhite, 3, "Keith Raney");
			fixPrint(x, y++, fontColorGreen, 3, "Donna:");
			fixPrint(x+1, y++, fontColorWhite, 3, "Jose Salot");
			fixPrint(x, y++, fontColorGreen, 3, "Menu Pixel Art:");
			fixPrint(x+1, y++, fontColorWhite, 3, "Asher");
			y++;
			fixPrint(x, y++, fontColorGreen, 3, "Neo Geo SDK");
			fixPrint(x+1, y++, fontColorWhite, 3, "NeoDev (Jeff Kurtz)");
			fixPrint(x, y++, fontColorGreen, 3, "Z80 Sound Driver");
			fixPrint(x+1, y++, fontColorWhite, 3, "Based on freem ADPCM example");
			y++;
			fixPrint(x, y++, fontColorGreen, 3, "MVS flashcart provided by:");
			fixPrint(x+1, y++, fontColorWhite, 3, "MobiusStripTech & Jose Cruz");
			fixPrint(x, y++, fontColorGreen, 3, "AES flashcart borrowed from:");
			fixPrint(x+1, y++, fontColorWhite, 3, "El Diablo (Jorge Velazquez)");
			y++;
			fixPrint(5, y++, fontColorGreen, 3, "Info on using this test suite:");
			fixPrint(6, y, fontColorWhite, 3, "http://junkerhq.net/240p");
			draw = 0;
		}
		
		SCClose();
		waitVBlank();

		readController();
		
		if (BTTN_EXIT)
			done = 1;
	}
	return;
}

void draw_debug_data()
{
	if (bkp_data.debug_dip1 & DP_DEBUG1)
	{
		displayRegByte(4, 8, "BIOS_USER_REQS", BIOS_USER_REQS);
		displayRegByte(4, 9, "BIOS_USER_MODE", BIOS_USER_MODE);
		displayRegByte(4, 10, "BIOS_START_FLAG", BIOS_START_FLAG);
		displayRegByte(4, 11, "BIOS_DEV_MODE", BIOS_DEV_MODE);
	}
}

// This function is called by the BIOS when the start button is
// pressed (AES) and when enough credits are available (MVS)
void _240p_mvs_player_start(void)
{
	if (volMEMBYTE(BIOS_USER_MODE) != BIOS_UM_INGAME)
	{
		// Tell the BIOS the game has started
		volMEMBYTE(BIOS_USER_MODE) = BIOS_UM_INGAME;
		// Set player 1's status to running
		if(volMEMBYTE(BIOS_PLAYER_MOD1) != BIOS_PM_PLAYING)
			volMEMBYTE(BIOS_PLAYER_MOD1) = BIOS_PM_PLAYING;
	}
	else
		volMEMBYTE(BIOS_START_FLAG) = 0x00; // don't decrease credits
}

void game_over()
{
	// Set Game Over state
	volMEMBYTE(BIOS_PLAYER_MOD1) = BIOS_PM_GAMEOVER;
}

void _240p_mvs_game_change(void)
{
}

void _240p_mvs_coin_sound(void)
{
	playSound(SOUNDCMD_PlayCoinA);
}

void menu_main()
{
	int curse = 1, cursemax = 7, redraw = 1, done = 0, showexit = 0;
	blinker blinkdata;

	palJobPut(0,8,fixPalettes);

	// Show Exit in neo Geo CD and MVS with credits and demo mode
#ifndef __cd__
	if (getSoftDipvalue(SOFT_DIP_1))
	{
#endif
		showexit = 1;
		cursemax++;
#ifndef __cd__
	}
#endif

	if(isPALinMVS)
		draw_warning("MVS HW is supposed to be NTSC.\nIt is supported by the Suite,\nbut PAL options will be disabled\nto honor SNK definition.", 1, 16, 1);

	while (!done)
	{
		if (redraw)
		{
			gfxClear();
			draw_background_w_gil(&blinkdata);
			redraw = 0;
		}

		SCClose();
		waitVBlank();
		SD_blink_cycle(&blinkdata);

		readController();

		if (PRESSED_UP)		curse=curse>1?curse-1:cursemax;
		if (PRESSED_DOWN)	curse=curse<cursemax?curse+1:1;

		fixPrint(6, 12, curse == 1 ? fontColorRed : fontColorWhite, 3, "Test Patterns");
		fixPrint(6, 13, curse == 2 ? fontColorRed : fontColorWhite, 3, "Video Tests");
		fixPrint(6, 14, curse == 3 ? fontColorRed : fontColorWhite, 3, "Audio Tests");
		fixPrint(6, 15, curse == 4 ? fontColorRed : fontColorWhite, 3, "Hardware Tools");

		fixPrint(6, 19, curse == 5 ? fontColorRed : fontColorWhite, 3, "Help");
		fixPrint(6, 20, curse == 6 ? fontColorRed : fontColorWhite, 3, "Credits");
		fixPrint(6, 21, curse == 7 ? fontColorRed : fontColorWhite, 3, "Options");
		if (showexit)
			fixPrint(6, 22, curse == 8 ? fontColorRed : fontColorWhite, 3, "Exit");

		draw_debug_data();
		menu_footer();

		if (checkHelp(HELP_GENERAL))
			redraw = 1;

		if (BTTN_MAIN)
		{
			gfxClear();
			switch (curse)
			{
				case 1:
					menu_tp();
				break;
				
				case 2:
					menu_vt();
				break;

				case 3:
					menu_at();
				break;

				case 4:
					menu_ht();
				break;

				case 5:
					DrawHelp(HELP_GENERAL);
				break;

				case 6:
					credits();
				break;

				case 7:
					menu_options();
				break;

				case 8:
					return;
				break;
			}
			redraw = 1;
		}
	}
	game_over();
}

#define DEMO_BG	4
#define DEMO_LEN 1800
void draw_mvs_demo()
{
	int toggle = 0, demo_frames = DEMO_LEN, demo_change = 0, freeplay = 0, redraw = 0;
	int currdemo = 0, index = 40, palindex = 25;
	picture background, foreground, titledsp;
	scroller grid;
	blinker blinkdata;

	gfxClear();
	pictureInit(&foreground, &gillian, index, palindex, 132, 50, FLIP_NONE);
	palJobPut(palindex,gillian.palInfo->count,gillian.palInfo->data);
	index += getPicSprites(foreground.info);
	palindex += gillian.palInfo->count;

	pictureInit(&titledsp, &title, index, palindex, 56, 28, FLIP_NONE);
	palJobPut(palindex,title.palInfo->count,title.palInfo->data);
	index += getPicSprites(titledsp.info);
	palindex += title.palInfo->count;

	load_blinkdata(&blinkdata, &index, &palindex, 132, 50);

	fixPrint(10, 26, fontColorSolid, 4, "2022 Dasutin/Artemio");

	demo_change = 1;
	while (demo_frames)
	{
		int credits = 0;

		SCClose();
		waitVBlank();

		SD_blink_cycle(&blinkdata);
		
		if (redraw)
		{
			currdemo ++;
			switch(currdemo)
			{
				case 1:
					pictureInit(&background, &monoscope, 1, 16, 0, 0,FLIP_NONE);
					palJobPut(16,monoscope.palInfo->count,monoscope.palInfo->data);
				break;
				case 2:
					if(vmode_snk)
					{
						pictureInit(&background, &colorbarssmpte75_304, 1, 16, 0, 0,FLIP_NONE);
						palJobPut(16,colorbarssmpte75_304.palInfo->count,colorbarssmpte75_304.palInfo->data);
					}
					else
					{
						pictureInit(&background, &colorbarssmpte75, 1, 16, 0, 0,FLIP_NONE);
						palJobPut(16,colorbarssmpte75.palInfo->count,colorbarssmpte75.palInfo->data);
					}
				break;
				case 3:
					scrollerInit(&grid, &grids, 1, 16, getHorScroll(), PATTERN_SCROLL);
					palJobPut(16, grids.palInfo->count, grids.palInfo->data);
				break;
				case 4:
					scrollerSetPos(&grid, -320, -224);
					pictureInit(&background, &conver_rgb_b, 1, 16, 0, 0, FLIP_NONE);
					palJobPut(16,conver_rgb_b.palInfo->count,conver_rgb_b.palInfo->data);
				break;
			}
			redraw = 0;
		}

		if (isMVS)
		{
			freeplay = getHardDipValue(DP_FREE);
			credits = getCreditCount();
		}
		readController();

		if (toggle == 30)
			fixPrint(14, 23, fontColorWhite, 3, "            ");
		if (toggle == 0)
			fixPrint(14, 23, fontColorRed, 3, freeplay || credits ? "PRESS  START" : "INSERT COIN");

		if (isMVS)
			fixPrintf(28, 28, fontColorSolid, 4, "CREDIT%c %02d", credits <= 1 ? ' ' : 'S', credits);  // credit counter

		draw_debug_data();
		
		toggle ++;
		if (toggle > 60)
			toggle = 0;
		demo_frames--;
		demo_change--;

		if (demo_change == 0)
		{
			demo_change = DEMO_LEN/DEMO_BG;
			redraw = 1;
		}

		if (volMEMBYTE(BIOS_USER_MODE) == BIOS_UM_INGAME)
		{
			menu_main();
			return;
		}
	}
}

void draw_mvs_title()
{
	int toggle = 0, bios_timer = 0, freeplay = 0, index = 1, palindex = 16;
	picture foreground;
	picture background;
	blinker blinkdata;

	//When set to 1, stops BIOS from calling command 3 twice after Game Over if credits are in the system.
	volMEMBYTE(BIOS_TITLE_MODE) = 1;

	gfxClear();

	pictureInit(&background, &back, index, palindex, 0, 0,FLIP_NONE);
	palJobPut(palindex,back.palInfo->count,back.palInfo->data);
	index += getPicSprites(background.info);
	palindex += back.palInfo->count;

	pictureInit(&foreground, &gillian, index, palindex, 132, 50, FLIP_NONE);
	palJobPut(17,gillian.palInfo->count,gillian.palInfo->data);
	index += getPicSprites(foreground.info);
	palindex += gillian.palInfo->count;

	load_blinkdata(&blinkdata, &index, &palindex, 132, 50);

	fixPrint(10, 26, fontColorWhite, 3, "2022 Dasutin/Artemio");

	while (1)
	{
		int credits = 0;

		SCClose();
		waitVBlank();

		SD_blink_cycle(&blinkdata);

		readController();

		freeplay = getHardDipValue(DP_FREE);

		if (toggle == 30)
			fixPrint(14, 23, fontColorRed, 3, "            ");
		if (toggle == 0)
			fixPrint(14, 23, fontColorRed, 3, "PRESS  START");

		if (!freeplay)
		{
			bios_timer = bcdToDec(volMEMBYTE(BIOS_COMP_TIME));
			fixPrintf(16, 28, fontColorWhite, 3, "TIME:%02d", bios_timer); // BIOS-COMPULSION-TIMER - timer for forced game start
		}
		
		credits = getCreditCount();
		fixPrintf(28, 28, fontColorWhite, 3, "CREDIT%c %02d", credits <= 1 ? ' ' : 'S', credits);  // credit counter
		
		toggle ++;
		if (toggle > 60)
			toggle = 0;

		if (volMEMBYTE(BIOS_USER_MODE) == BIOS_UM_INGAME)
		{
			menu_main();
			return;
		}

		draw_debug_data();
	}
}

void check_bios_init()
{
	// BIOS_USER_REQS == 0 is called by BIOS
	// in order to initialize memory
	if (volMEMBYTE(BIOS_USER_REQS) == BIOS_UR_INIT) 
	{
		waitVBlank();
		memset(bkp_data.data, 0x00, sizeof(BYTE)*(BKP_SIZE-2));
		RETURN_TO_BIOS;
	}

	// Eye catcher, we have none...
	if (volMEMBYTE(BIOS_USER_REQS) == BIOS_UR_EYE)
	{
		gfxClear();
		RETURN_TO_BIOS;
	}

	// If some other game in a multi system enabled dark mode, disable it
	volMEMBYTE(REG_NOSHADOW) = 1;

	first_grid = 1;
	first_overscan = 1;
	first_colorramp = 1;
}

void mvs_state()
{
	// If DEMO MODE draw the insert coin screen
	if (volMEMBYTE(BIOS_USER_REQS) == BIOS_UR_DEMO)
	{
		// Enter demo mode in MVS following Soft Dip Switches
		if (getSoftDipvalue(SOFT_DIP_1))
		{
			draw_mvs_demo();
		}
		else
		{
			// Force game mode in MVS following Soft Dip Switches
			volMEMBYTE(BIOS_USER_MODE) = BIOS_UM_INGAME;
			volMEMBYTE(BIOS_PLAYER_MOD1) = BIOS_PM_PLAYING;
			menu_main();
		}
		RETURN_TO_BIOS;
	}

	// If TITLE MODE grab the control
	if (volMEMBYTE(BIOS_USER_REQS) == BIOS_UR_TITLE)
	{
		// Enter title mode in MVS following Soft Dip Switches
		if (getSoftDipvalue(SOFT_DIP_1))
		{
			draw_mvs_title();
		}
		else
		{
			// Force game mode in MVS following Soft Dip Switches
			volMEMBYTE(BIOS_USER_MODE) = BIOS_UM_INGAME;
			volMEMBYTE(BIOS_PLAYER_MOD1) = BIOS_PM_PLAYING;
			menu_main();
		}
		RETURN_TO_BIOS;
	}

	RETURN_TO_BIOS;
}

int main(void)
{
	check_bios_init();
	check_systype();

	initGfx();
	gfxClear();
	palJobPut(0,NUM_FONT_COLORS,fixPalettes);
	SCClose();
	waitVBlank();

	if (isMVS)
		mvs_state();
	else
		menu_main();
	
	return 0;
}
