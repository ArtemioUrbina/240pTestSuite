/* 
 * 240p Test Suite for the Neo Geo
 * by Artemio Urbina and Dustin Dembrosky (Dasutin)
 * Copyright (C)2011-2023
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
#include "hardware.h"
#include "audio.h"
#include "string_ng.h"

bkp_ram_info bkp_data;

BYTE p1,p2,ps,pse,p1e,p2e,p1b,p2b;
BYTE isMVS, is2S, is4S, is6S, isMulti, hwChange, allowIRE107;
BYTE vmode_snk, isPAL, usePAL256, isPALinMVS;
BYTE enable_shadow, fill_color_bg;
WORD max_z80_timout;
WORD min_z80_timout;
BYTE disable_z80_check;
int z80Response, z80Expected;
#ifdef __cd__
BYTE isCDFront, isCDZ, ngcd_region;
#endif
WORD fbase;

#define USE_SERIAL

#ifdef USE_SERIAL
//static const char serialnumber[] = "#SERIALSTART#37154#00047#SERIALEND#";
static const char serialnumber[] = "#SERIALSTART#00000#00000#SERIALEND#";
#endif

#define NUM_FONT_COLORS 10
static const ushort fixPalettes[]= {
	_BLACK, WH_100, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK,	// Used for BIOS Mask
	_BLACK, WH_100, 0x0333, 0x5fa7, 0xde85, 0x2c74, 0x2a52, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, // fontColorWhite
	_BLACK, RED100, 0x0300, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, // fontColorRed
	_BLACK, GRN100, 0x0030, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, // fontColorGreen
	_BLACK, BLU100, 0x0003, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, // fontColorBlue
	_BLACK, YEL100, 0x0003, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, // fontColorYellow
	_BLACK, IRE_60, IRE_20, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, // fontColorGrayLight
	_BLACK, IRE_30, IRE_10, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, // fontColorGrayDark
	_BLACK, _BLACK, WH_100, 0xee51, 0x4f81, 0x4fa1, 0x4fc1, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, // fontColorBlack
	_BLACK, _BLACK, WH_100, 0x8333, 0xe7b0, 0xc580, 0xe250, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, // fontColorSolid
};

void menu_tp_colormenu()
{
	int done = 0, curse = 1, cursemax = 12, redraw = 1;
	blinker blinkdata;

	while (!done)
	{
		int y = 9;

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

		fixPrint(5, y++, curse == 1 ? fontColorRed : fontColorWhite, fbase, "Pluge");
		fixPrint(5, y++, curse == 2 ? fontColorRed : fontColorWhite, fbase, "Color Bars");
		fixPrint(5, y++, curse == 3 ? fontColorRed : fontColorWhite, fbase, "EBU Color Bars");
		fixPrint(5, y++, curse == 4 ? fontColorRed : fontColorWhite, fbase, "SMPTE Color Bars");
		fixPrint(5, y++, curse == 5 ? fontColorRed : fontColorWhite, fbase, "Referenced Color Bars");
		fixPrint(5, y++, curse == 6 ? fontColorRed : fontColorWhite, fbase, "Color Bleed Check");
		fixPrint(5, y++, curse == 7 ? fontColorRed : fontColorWhite, fbase, "Gray Ramp");
		fixPrint(5, y++, curse == 8 ? fontColorRed : fontColorWhite, fbase, "White & RGB Screens");
		fixPrint(5, y++, curse == 9 ? fontColorRed : fontColorWhite, fbase, "100 IRE");
		fixPrint(5, y++, curse == 10 ? fontColorRed : fontColorWhite, fbase, "Sharpness");
		y += 2;
		fixPrint(5, y++, curse == 11 ? fontColorRed : fontColorWhite, fbase, "Help");
		fixPrint(5, y++, curse == 12 ? fontColorRed : fontColorWhite, fbase, "Back to Main Menu");

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
					tp_gray_ramp();
				break;

				case 8:
					tp_white_rgb();
				break;

				case 9:
					tp_100_ire();
				break;

				case 10:
					tp_sharpness();
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

void menu_tp_geometrymenu()
{
	int done = 0, curse = 1, cursemax = 6, redraw = 1;
	blinker blinkdata;

	while (!done)
	{
		int x = 6, y = 14;

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

		fixPrint(x, y++, curse == 1 ? fontColorRed : fontColorWhite, fbase, "Monoscope");
		fixPrint(x, y++, curse == 2 ? fontColorRed : fontColorWhite, fbase, "Grid");;
		fixPrint(x, y++, curse == 3 ? fontColorRed : fontColorWhite, fbase, "Overscan");
		fixPrint(x, y++, curse == 4 ? fontColorRed : fontColorWhite, fbase, "Convergence");
		y+= 2;
		fixPrint(x, y++, curse == 5 ? fontColorRed : fontColorWhite, fbase, "Help");
		fixPrint(x, y++, curse == 6 ? fontColorRed : fontColorWhite, fbase, "Back to Main Menu");

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
					tp_monoscope();
				break;

				case 2:
					tp_grid();
				break;

				case 3:
					tp_overscan();
				break;

				case 4:
					tp_convergence();
				break;

				case 5:
					DrawHelp(HELP_GENERAL);
				break;

				case 6:
					done = 1;
				break;
			}
			redraw = 1;
		}
	}
	return;
}

void menu_tp()
{
	int done = 0, curse = 1, cursemax = 5, redraw = 1;
	blinker blinkdata;

	while (!done)
	{
		int y = 14;

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

		fixPrint(5, y++, curse == 1 ? fontColorRed : fontColorWhite, fbase, "Color & Black Levels");
		fixPrint(5, y++, curse == 2 ? fontColorRed : fontColorWhite, fbase, "Geometry");
		fixPrint(5, y++, curse == 3 ? fontColorRed : fontColorWhite, fbase, "HCFR Patterns");
		y+=2;
		fixPrint(5, y++, curse == 4 ? fontColorRed : fontColorWhite, fbase, "Help");
		fixPrint(5, y++, curse == 5 ? fontColorRed : fontColorWhite, fbase, "Back to Main Menu");

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
					menu_tp_colormenu();
				break;
					
				case 2:
					menu_tp_geometrymenu();
				break;

				case 3:
					tp_hcfr();
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

void menu_vt()
{
	int done = 0, curse = 1, cursemax = 14, redraw = 1;
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

		fixPrint(5, 9,  curse == 1 ? fontColorRed : fontColorWhite, fbase, "Drop Shadow Test");
		fixPrint(5, 10, curse == 2 ? fontColorRed : fontColorWhite, fbase, "Striped Sprite Test");
		fixPrint(5, 11, curse == 3 ? fontColorRed : fontColorWhite, fbase, "Lag Test");
		fixPrint(5, 12, curse == 4 ? fontColorRed : fontColorWhite, fbase, "Timing & Reflex Test");
		fixPrint(5, 13, curse == 5 ? fontColorRed : fontColorWhite, fbase, "Scroll Test");
		fixPrint(5, 14, curse == 6 ? fontColorRed : fontColorWhite, fbase, "Grid Scroll Test");
		fixPrint(5, 15, curse == 7 ? fontColorRed : fontColorWhite, fbase, "Horizontal Stripes");
		fixPrint(5, 16, curse == 8 ? fontColorRed : fontColorWhite, fbase, "Vertical Stripes");
		fixPrint(5, 17, curse == 9 ? fontColorRed : fontColorWhite, fbase, "Checkerboard");
		fixPrint(5, 18, curse == 10 ? fontColorRed : fontColorWhite, fbase, "Phase & Sample Rate");
		fixPrint(5, 19, curse == 11 ? fontColorRed : fontColorWhite, fbase, "Backlit Zone Test");
		fixPrint(5, 20, curse == 12 ? fontColorRed : fontColorWhite, fbase, "Disappearing Logo");

		fixPrint(5, 22, curse == 13 ? fontColorRed : fontColorWhite, fbase, "Help");
		fixPrint(5, 23, curse == 14 ? fontColorRed : fontColorWhite, fbase, "Back to Main Menu");

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
					vt_phase_check();
				break;

				case 11:
					vt_backlitzone_test();
				break;

				case 12:
					vt_disappear_logo();
				break;

				case 13:
					DrawHelp(HELP_GENERAL);
				break;

				case 14:
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
	int done = 0, curse = 1, cursemax = 5, redraw = 1;
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

		fixPrint(5, 14, curse == 1 ? fontColorRed : fontColorWhite, fbase, "Sound Test");
		fixPrint(5, 15, curse == 2 ? fontColorRed : fontColorWhite, fbase, "Audio Sync Test");
		fixPrint(5, 16, curse == 3 ? fontColorRed : fontColorWhite, fbase, "MDFourier");

		fixPrint(5, 18, curse == 4 ? fontColorRed : fontColorWhite, fbase, "Help");
		fixPrint(5, 19, curse == 5 ? fontColorRed : fontColorWhite, fbase, "Back to Main Menu");

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
	int debug = 0, olddebug = 2;
	blinker blinkdata;

	while (!done)
	{
		int y = 12 - debug;

		if (redraw)
		{
			gfxClear();
			draw_background_w_gil(&blinkdata);
			redraw = 0;
		}

		debug = DEBUG_ENABLED;
		if(olddebug != debug)
		{
			if(debug) {
				y = 11;
				debug = 1;
			} else {
				y = 12;
				debug = 0;
			}
			cursemax = 7 + debug;
			suiteClearFixLayer();
			olddebug = debug;
		}

		SCClose();
		waitVBlank();
		SD_blink_cycle(&blinkdata);

		readController();

		if (PRESSED_UP)		curse=curse>1?curse-1:cursemax;
		if (PRESSED_DOWN)	curse=curse<cursemax?curse+1:1;

		fixPrint(5, y++, curse == 1 ? fontColorRed : fontColorWhite, fbase, "Controller Test");
		fixPrint(5, y++, curse == 2 ? fontColorRed : fontColorWhite, fbase, "BIOS Info");
		fixPrint(5, y++, curse == 3 ? fontColorRed : fontColorWhite, fbase, "Z80 RAM test");
		fixPrint(5, y++, curse == 4 ? fontColorRed : fontColorWhite, fbase, "Memory Viewer");
		fixPrint(5, y++, curse == 5 ? fontColorRed : fontColorWhite, fbase, "Register View");
		if (debug)
			fixPrint(5, y++, curse == 6 ? fontColorRed : fontColorWhite, fbase, "Internal Vars");
		y++;
		fixPrint(5, y++, curse == 6 + debug ? fontColorRed : fontColorWhite, fbase, "Help");
		fixPrint(5, y++, curse == 7 + debug ? fontColorRed : fontColorWhite, fbase, "Back to Main Menu");

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
					ht_check_ng_bios_crc(BIOS_ADDRESS);
				break;

				case 3:
					ht_z80RAMtest();
				break;
				
				case 4:
					ht_memory_viewer(0);
				break;

				case 5:
					ht_displayregs();
				break;

				default:
					if(debug && curse == 6)
						ht_showInternalVars();
					if(curse == 6 + debug)
						DrawHelp(HELP_GENERAL);
					else if(curse == 6 + debug)
						DrawHelp(HELP_GENERAL);
					else if(curse == 7 + debug)
						done = 1;
				break;
			}
			redraw = 1;
		}
	}
	return;
}

#ifdef USE_SERIAL
int checkserialset()
{
	int sum = 0, i = 0;

	for(i = 0; i < 5; i++)
		sum += (int)serialnumber[13+i] - (int)'0';
	
	if(sum == 20)
		return 1;
	return sum;
}
#endif

void credits()
{
	int done = 0, draw = 1;
	picture qr;

	while (!done)
	{
		if(draw)
		{
			int x = 4, y = 3;
			int index = 1, palindex = 16;
			picture background;

			gfxClear();
			pictureInit(&background, &donnaline, index, palindex, 0, 0,FLIP_NONE);
			palJobPut(palindex,donnaline.palInfo->count,donnaline.palInfo->data);

			pictureInit(&qr, &barcode, 26, 17, 260, 150, FLIP_NONE);
			palJobPut(17,barcode.palInfo->count,barcode.palInfo->data);

			fixPrint(x+24, y, fontColorGreen, fbase, "Ver. 1.02");
			fixPrint(x+24, y+1, fontColorWhite, fbase, "17/10/2023");
#ifdef USE_SERIAL
			if(checkserialset())
			{
				char *serial = (char*)serialnumber + 20;
				fixPrintf(x+24, y+2, fontColorYellow, fbase, "CART #%c%c%c%c",
					serial[0], serial[1], serial[2], serial[3]);
			}
#endif
			fixPrint(x, y++, fontColorGreen, fbase, "Code by:");
			fixPrint(x+1, y++, fontColorWhite, fbase, "Dustin Dembrosky");
			fixPrint(x+1, y++, fontColorWhite, fbase, "Artemio Urbina");
			fixPrint(x, y++, fontColorGreen, fbase, "Monoscope by:");
			fixPrint(x+1, y++, fontColorWhite, fbase, "Keith Raney");
			fixPrint(x, y++, fontColorGreen, fbase, "Donna by:");
			fixPrint(x+1, y++, fontColorWhite, fbase, "Jose Salot");
			fixPrint(x, y++, fontColorGreen, fbase, "Neo Geo SDK & Graphics Library:");
			fixPrint(x+1, y++, fontColorWhite, fbase, "NeoDev/Jeff Kurtz & DATlib/HPMAN");
			fixPrint(x, y++, fontColorGreen, fbase, "Z80 Sound Driver:");
			fixPrint(x+1, y++, fontColorWhite, fbase, "Based on freem ADPCM example");
			fixPrint(x, y++, fontColorGreen, fbase, "Cartridge Hardware by:");
			fixPrint(x+1, y++, fontColorWhite, fbase, "herzmx");
			fixPrint(x, y++, fontColorGreen, fbase, "Cartridge funding:");
			fixPrint(x+1, y++, fontColorWhite, fbase, "Rolando Cedillo");
			fixPrint(x, y, fontColorGreen, fbase, "Menu Pixel Art:");
			fixPrint(x+18, y++, fontColorGreen, fbase, "Jingle by:");
			fixPrint(x+1, y, fontColorWhite, fbase, "Asher");
			fixPrint(x+18, y++, fontColorWhite, fbase, "Neko Milkshake");
#ifndef __cd__
			fixPrint(x, y++, fontColorGreen, fbase, "MVS flashcart provided by:");
			fixPrint(x+1, y++, fontColorWhite, fbase, "MobiusStripTech & Jose Cruz");
			fixPrint(x, y++, fontColorGreen, fbase, "AES flashcart borrowed from:");
			fixPrint(x+1, y++, fontColorWhite, fbase, "El Diablo (Jorge Velazquez)");
#else
			fixPrint(x, y++, fontColorGreen, fbase, "Neo Geo CDZ borrowed from:");
			fixPrint(x+1, y++, fontColorWhite, fbase, "Rolando Cedillo");
#endif
			fixPrint(x, y++, fontColorYellow, fbase, "http://junkerhq.net/240p");
			y++;
			fixPrint(18, y, fontColorWhite, fbase, "Dedicated to Elisa");
			draw = 0;
		}
		
		SCClose();
		waitVBlank();

		readController();
		if (PRESSED_C)	{
			picture memory;

			gfxClear();
			pictureInit(&memory, &beto, 26, 16, 16, 8, FLIP_NONE);
			palJobPut(16,beto.palInfo->count,beto.palInfo->data);
			fixPrint(9, 3, fontColorGrayLight, fbase, "Thank you Beto Garcia");
			SCClose();
			do {
				waitVBlank();
				readController();
			} while (HELD_C);

			draw = 1;
		}
		if (PRESSED_D)	{
			picture n_t;

			gfxClear();
			pictureInit(&n_t, &n_m, 26, 16, 96, 48, FLIP_NONE);
			palJobPut(16,n_m.palInfo->count,n_m.palInfo->data);
			SCClose();
			do {
				waitVBlank();
				readController();
			} while (HELD_D);

			draw = 1;
		}
		if (BTTN_EXIT)
			done = 1;
	}
	return;
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
	// in case z80 is not working, don't expect a response
	sendZ80commandnoWait(SOUNDCMD_PlayCoinA);
}

void CheckWarnings()
{
	if(isPALinMVS)
		draw_warning("MVS HW is supposed to be NTSC.\nIt is supported by the Suite,\nbut PAL options will be disabled\nto honor SNK definition.", 1, 16, 1);

	checkZ80Version();
}

void menu_main()
{
	int curse = 1, cursemax = 7, redraw = 1, done = 0, showexit = 0;
	blinker blinkdata;

	palJobPut(0,8,fixPalettes);

	// Show Exit in neo Geo CD and MVS with credits and demo mode
#ifndef __cd__
	if (getSoftDipvalue(SD_MVS_DEMO)) {
#endif
		showexit = 1;
		cursemax++;
#ifndef __cd__
	}
#endif

	CheckWarnings();

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

		fixPrint(6, 12, curse == 1 ? fontColorRed : fontColorWhite, fbase, "Test Patterns");
		fixPrint(6, 13, curse == 2 ? fontColorRed : fontColorWhite, fbase, "Video Tests");
		fixPrint(6, 14, curse == 3 ? fontColorRed : fontColorWhite, fbase, "Audio Tests");
		fixPrint(6, 15, curse == 4 ? fontColorRed : fontColorWhite, fbase, "Hardware Tools");

		fixPrint(6, 19, curse == 5 ? fontColorRed : fontColorWhite, fbase, "Help");
		fixPrint(6, 20, curse == 6 ? fontColorRed : fontColorWhite, fbase, "Credits");
		fixPrint(6, 21, curse == 7 ? fontColorRed : fontColorWhite, fbase, "Options");
		if (showexit)
			fixPrint(6, 22, curse == 8 ? fontColorRed : fontColorWhite, fbase, "Exit");

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
void draw_demo()
{
	int toggle = 0, demo_frames = DEMO_LEN, demo_change = 0, redraw = 0;
	int currdemo = 0, index = 40, palindex = 25;
	picture background, foreground, titledsp;
	scroller grid, monoscope;
	blinker blinkdata;

	gfxClear();
	pictureInit(&foreground, &gillian, index, palindex, 133, 50, FLIP_NONE);
	palJobPut(palindex,gillian.palInfo->count,gillian.palInfo->data);
	index += getPicSprites(foreground.info);
	palindex += gillian.palInfo->count;

	pictureInit(&titledsp, &title, index, palindex, 56, 28, FLIP_NONE);
	palJobPut(palindex,title.palInfo->count,title.palInfo->data);
	index += getPicSprites(titledsp.info);
	palindex += title.palInfo->count;

	load_blinkdata(&blinkdata, &index, &palindex, 133, 50);

	fixPrint(10, 26, fontColorSolid, fbase+1, "2023 Dasutin/Artemio");

	demo_change = 1;
	while (demo_frames)
	{
		int credits = 0, pressStart = 0;

		SCClose();
		waitVBlank();

		SD_blink_cycle(&blinkdata);
		
		if (redraw)
		{
			currdemo ++;
			switch(currdemo)
			{
				case 1:
					scrollerInit(&monoscope, &monoscopes, 1, 16, getHorScrollMonoscope(), PATTERN_SCROLL);
					palJobPut(16, monoscopes.palInfo->count, monoscopes.palInfo->data);
				break;
				case 2:
					scrollerSetPos(&monoscope, -320, -224);
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
			int freeplay = 0;

			freeplay = getHardDipValue(DP_FREE);
			credits = getCreditCount();
			if(freeplay || credits)
				pressStart = 1;
		}
		else
			pressStart = 1;

		readController();

		if (toggle == 30)
			fixPrint(14, 23, fontColorWhite, fbase, "            ");
		if (toggle == 0)
			fixPrint(14, 23, fontColorRed, fbase, pressStart ? "PRESS  START" : "INSERT COIN");

		if (isMVS)
			fixPrintf(28, 28, fontColorSolid, fbase+1, "CREDIT%c %02d", credits <= 1 ? ' ' : 'S', credits);  // credit counter
		
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

#ifndef __cd__
		if (volMEMBYTE(BIOS_USER_MODE) == BIOS_UM_INGAME)
		{
			menu_main();
			return;
		}
#else
		if (PRESSED_START)
		{
			menu_main();
			RETURN_TO_CDPLAYER;
		}
#endif
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

	fixPrint(10, 26, fontColorWhite, fbase, "2023 Dasutin/Artemio");

	while (1)
	{
		int credits = 0;

		SCClose();
		waitVBlank();

		SD_blink_cycle(&blinkdata);

		readController();

		freeplay = getHardDipValue(DP_FREE);

		if (toggle == 30)
			fixPrint(14, 23, fontColorRed, fbase, "            ");
		if (toggle == 0)
			fixPrint(14, 23, fontColorRed, fbase, "PRESS  START");

		if (!freeplay)
		{
			bios_timer = bcdToDec(volMEMBYTE(BIOS_COMP_TIME));
			fixPrintf(16, 28, fontColorWhite, fbase, "TIME:%02d", bios_timer); // BIOS-COMPULSION-TIMER - timer for forced game start
		}
		
		credits = getCreditCount();
		fixPrintf(28, 28, fontColorWhite, fbase, "CREDIT%c %02d", credits <= 1 ? ' ' : 'S', credits);  // credit counter
		
		toggle ++;
		if (toggle > 60)
			toggle = 0;

		if (volMEMBYTE(BIOS_USER_MODE) == BIOS_UM_INGAME)
		{
			menu_main();
			return;
		}
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

	// default font
	if (getSoftDipvalue(SD_DEFAULT_FONT))
		fbase = 5;
	else
		fbase = 3;
	
	// Set default global values
	first_grid = 1;
	first_overscan = 1;
	first_colorramp = 1;
	max_z80_timout = 0;
	min_z80_timout = 65535;
	disable_z80_check = 0;
	fill_color_bg = 0;
}

void parse_bios_status()
{
	// If DEMO MODE draw the insert coin screen
	// or Title Screen in AES mode
	if (volMEMBYTE(BIOS_USER_REQS) == BIOS_UR_DEMO)
	{
		// Enter demo mode in MVS following Soft Dip Switches or AES directly
		if ((isMVS && getSoftDipvalue(SD_MVS_DEMO)) || !isMVS)
		{
			draw_demo();
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
		if (getSoftDipvalue(SD_MVS_DEMO))
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
	check_systype();
	check_bios_init();

	initGfx();
	gfxClear();
	palJobPut(0,NUM_FONT_COLORS,fixPalettes);
	SCClose();
	waitVBlank();

	parse_bios_status();
	
	return 0;
}
