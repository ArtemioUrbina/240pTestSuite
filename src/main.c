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
#include "externs.h"
#include "patterns.h"
#include "tests.h"
#include "help.h"
#include "string_ng.h"

typedef struct bkp_ram_info {
	WORD debug_dips;
	BYTE stuff[14];
	//16 bytes
} bkp_ram_info;

bkp_ram_info bkp_data;

BYTE p1,p2,ps,p1e,p2e;
BYTE isMVS, is4S, is6S, isMulti;

// We can't detect between 1 and 2 slot yet
void check_systype()
{
	isMVS = is4S = is6S = isMulti = 0;

	if(MEMBYTE(BIOS_MVS_FLAG)==SYSTEM_MVS)
	{
		BYTE reg = 0;

		isMVS = 1;
		reg = volMEMBYTE(REG_SYSTYPE);
		if(reg & MVS_MULTI)
		{
			isMulti = 1;
			reg = volMEMBYTE(REG_STATUS_A);
			if(reg & MVS_4_OR_6)
				is6S = 1;
			else
				is4S = 1;
		}
	}
}

static const ushort fixPalettes[]= {
	0x8000, 0x7fff, 0x0333, 0x5fa7, 0xde85, 0x2c74, 0x2a52, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	0x8000, 0x0f00, 0x0300, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	0x8000, 0x00f0, 0x0030, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	0x8000, 0x000f, 0x0003, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	0x8000, 0x8000, 0x7fff, 0xee51, 0x4f81, 0x4fa1, 0x4fc1, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	0x8000, 0x8000, 0x7fff, 0x8333, 0xe7b0, 0xc580, 0xe250, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	0x8000, 0xefb8, 0x0111, 0xde96, 0x3c75, 0x2950, 0x4720, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	0x8000, 0x8444, 0x0111, 0xf555, 0xf666, 0x7777, 0x8888, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	0x8000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

// placed here temporarily
inline void gfxClear()
{
	clearSprites(1, MAX_SPRITES);
	clearFixLayer();
}

void draw_background_w_gil()
{
	picture background;
	picture foreground;

	pictureInit(&background, &back,1, 16, 0, 0,FLIP_NONE);
	palJobPut(16,back.palInfo->count,back.palInfo->data);

	pictureInit(&foreground, &gillian,22, 17, 216, 70,FLIP_NONE);
	palJobPut(17,gillian.palInfo->count,gillian.palInfo->data);
}

void draw_background()
{
	picture background;

	pictureInit(&background, &back,1, 16, 0, 0,FLIP_NONE);
	palJobPut(16,back.palInfo->count,back.palInfo->data);
}

void menu_footer()
{
	fixPrint(23, 26, 0, 3, "NTSC 320x224p");
	fixPrint(17, 28, 0, 3, isMVS?"Neo Geo MVS":"Neo Geo AES");
	if(isMVS && (is4S||is6S))
	{
		fixPrint(28, 28, 0, 3, is4S ? "4S" : "6S");
	}
	if((MEMBYTE(BIOS_COUNTRY_CODE)==SYSTEM_JAPAN))
	{
		fixPrint(32, 28, 0, 3, "Japan");
	}
	else if ((MEMBYTE(BIOS_COUNTRY_CODE)==SYSTEM_USA))
	{
		fixPrint(34, 28, 0, 3, "USA");
	}
	else if ((MEMBYTE(BIOS_COUNTRY_CODE)==SYSTEM_EUROPE))
	{	
		fixPrint(31, 28, 0, 3, "Europe");
	}

	if(isMVS && volMEMBYTE(SOFT_DIP_2))
		fixPrintf(3, 26, 0, 3, "CREDITS %02d", hexToDec(volMEMBYTE(BIOS_NM_CREDIT)));  // credit counter
}

void menu_tp()
{
	int done = 0, curse = 1, cursemax = 16, redraw = 1;

	while (!done)
	{
		if(redraw)
		{
			gfxClear();
			draw_background_w_gil();
			redraw = 0;
		}
		SCClose();
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);

		if(p1e & JOY_UP)	curse=curse>1?curse-1:cursemax;
		if(p1e & JOY_DOWN)	curse=curse<cursemax?curse+1:1;

		fixPrint(5, 8, curse == 1 ? 1 : 0, 3, "Pluge");
		fixPrint(5, 9, curse == 2 ? 1 : 0, 3, "Color Bars");
		fixPrint(5, 10, curse == 3 ? 1 : 0, 3, "EBU Color Bars");
		fixPrint(5, 11, curse == 4 ? 1 : 0, 3, "SMPTE Color Bars");
		fixPrint(5, 12, curse == 5 ? 1 : 0, 3, "Referenced Color Bars");
		fixPrint(5, 13, curse == 6 ? 1 : 0, 3, "Color Bleed Check");
		fixPrint(5, 14, curse == 7 ? 1 : 0, 3, "Monoscope");
		fixPrint(5, 15, curse == 8 ? 1 : 0, 3, "Grid");
		fixPrint(5, 16, curse == 9 ? 1 : 0, 3, "Gray Ramp");
		fixPrint(5, 17, curse == 10 ? 1 : 0, 3, "White & RGB Screens");
		fixPrint(5, 18, curse == 11 ? 1 : 0, 3, "100 IRE");
		fixPrint(5, 19, curse == 12 ? 1 : 0, 3, "Sharpness");
		fixPrint(5, 20, curse == 13 ? 1 : 0, 3, "Overscan");
		fixPrint(5, 21, curse == 14 ? 1 : 0, 3, "Convergence");

		fixPrint(5, 23, curse == 15 ? 1 : 0, 3, "Help");
		fixPrint(5, 24, curse == 16 ? 1 : 0, 3, "Back to Main Menu");

		menu_footer();

		if (p1e & JOY_B)
		{
			done = 1;
		}

		if(checkHelp(HELP_GENERAL))
			redraw = 1;

		if (p1e & JOY_A)
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

	while (!done)
	{
		if(redraw)
		{
			gfxClear();
			draw_background_w_gil();
			redraw = 0;
		}
		SCClose();
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);

		if(p1e & JOY_UP)	curse=curse>1?curse-1:cursemax;
		if(p1e & JOY_DOWN)	curse=curse<cursemax?curse+1:1;

		fixPrint(5, 10, curse == 1 ? 1 : 0, 3, "Drop Shadow Test");
		fixPrint(5, 11, curse == 2 ? 1 : 0, 3, "Striped Sprite Test");
		fixPrint(5, 12, curse == 3 ? 1 : 0, 3, "Lag Test");
		fixPrint(5, 13, curse == 4 ? 1 : 0, 3, "Timing & Reflex Test");
		fixPrint(5, 14, curse == 5 ? 1 : 0, 3, "Scroll Test");
		fixPrint(5, 15, curse == 6 ? 1 : 0, 3, "Grid Scroll Test");
		fixPrint(5, 16, curse == 7 ? 1 : 0, 3, "Horizontal Stripes");
		fixPrint(5, 17, curse == 8 ? 1 : 0, 3, "Vertical Stripes");
		fixPrint(5, 18, curse == 9 ? 1 : 0, 3, "Checkerboard");
		fixPrint(5, 19, curse == 10 ? 1 : 0, 3, "Backlit Zone Test");

		fixPrint(5, 21, curse == 11 ? 1 : 0, 3, "Help");
		fixPrint(5, 22, curse == 12 ? 1 : 0, 3, "Back to Main Menu");

		menu_footer();

		if (p1e & JOY_B)
		{
			done = 1;
		}

		if(checkHelp(HELP_GENERAL))
			redraw = 1;

		if (p1e & JOY_A)
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

	while (!done)
	{
		if(redraw)
		{
			gfxClear();
			draw_background_w_gil();
			redraw = 0;
		}
		SCClose();
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);

		if(p1e & JOY_UP)	curse=curse>1?curse-1:cursemax;
		if(p1e & JOY_DOWN)	curse=curse<cursemax?curse+1:1;

		fixPrint(5, 14, curse == 1 ? 1 : 0, 3, "Sound Test");
		fixPrint(5, 15, curse == 2 ? 1 : 0, 3, "Audio Sync Test");

		fixPrint(5, 17, curse == 3 ? 1 : 0, 3, "Help");
		fixPrint(5, 18, curse == 4 ? 1 : 0, 3, "Back to Main Menu");

		menu_footer();

		if (p1e & JOY_B)
		{
			done = 1;
		}

		if(checkHelp(HELP_GENERAL))
			redraw = 1;

		if (p1e & JOY_A)
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
					DrawHelp(HELP_GENERAL);
				break;

				case 4:
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
	int done = 0, curse = 1, cursemax = 6, redraw = 1;

	while (!done)
	{
		if(redraw)
		{
			gfxClear();
			draw_background_w_gil();
			redraw = 0;
		}
		SCClose();
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);

		if(p1e & JOY_UP)	curse=curse>1?curse-1:cursemax;
		if(p1e & JOY_DOWN)	curse=curse<cursemax?curse+1:1;

		fixPrint(5, 13, curse == 1 ? 1 : 0, 3, "Controller Test");
		fixPrint(5, 14, curse == 2 ? 1 : 0, 3, "SDRAM Check");
		fixPrint(5, 15, curse == 3 ? 1 : 0, 3, "Memory Viewer");
		fixPrint(5, 16, curse == 4 ? 1 : 0, 3, "BIOS Info");

		fixPrint(5, 18, curse == 5 ? 1 : 0, 3, "Help");
		fixPrint(5, 19, curse == 6 ? 1 : 0, 3, "Back to Main Menu");

		menu_footer();

		if (p1e & JOY_B)
		{
			done = 1;
		}

		if(checkHelp(HELP_GENERAL))
			redraw = 1;

		if (p1e & JOY_A)
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
					ht_memory_viewer(1);
				break;

				case 4:
					ht_check_ng_bios_crc(BIOS_ADDRESS);
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

void credits()
{
	int done = 0;
	picture qr;

	gfxClear();
	draw_background();

	pictureInit(&qr, &barcode, 26, 17, 260, 110, FLIP_NONE);
	palJobPut(17,barcode.palInfo->count,barcode.palInfo->data);

	while (!done)
	{
		int y = 8;
		
		SCClose();
		waitVBlank();

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		fixPrint(16, 6, 2, 3, "Credits");
		fixPrint(28, 8, 2, 3, "Ver. 0.1");
		fixPrint(28, 9, 0, 3, "9/4/2022");
		fixPrint(5, y++, 2, 3, "Code by:");
		fixPrint(6, y++, 0, 3, "Dustin Dembrosky");
		fixPrint(6, y++, 0, 3, "Artemio Urbina");
		fixPrint(5, y++, 2, 3, "Monoscope:");
		fixPrint(6, y++, 0, 3, "Keith Raney");
		fixPrint(5, y++, 2, 3, "Donna:");
		fixPrint(6, y++, 0, 3, "Jose Salot");
		fixPrint(5, y++, 2, 3, "Menu Pixel Art:");
		fixPrint(6, y++, 0, 3, "Asher");
		fixPrint(5, y++, 2, 3, "Neo Geo SDK");
		fixPrint(6, y++, 0, 3, "NeoDev (Jeff Kurtz)");
		fixPrint(5, y++, 2, 3, "Graphics Library");
		fixPrint(6, y++, 0, 3, "DATlib (HPMAN)");
		fixPrint(5, y++, 2, 3, "Flashcart provided by:");
		fixPrint(6, y++, 0, 3, "MobiusStripTech & Jose Cruz");
		fixPrint(5, y++, 2, 3, "Info on using this test suite:");
		fixPrint(6, y, 0, 3, "http://junkerhq.net/240p");

		if (p1e & JOY_B || ps & P1_START)
		{
			done = 1;
		}
	}
	return;
}

void menu_main()
{
	int curse = 1, cursemax = 6, redraw = 1, done = 0, showexit = 0;

	palJobPut(0,8,fixPalettes);
	if(isMVS && volMEMBYTE(SOFT_DIP_1))
	{
		showexit = 1;
		cursemax++;
	}

	while(!done)
	{
		if(redraw)
		{
			gfxClear();
			draw_background_w_gil();
			redraw = 0;
		}
		SCClose();
		waitVBlank();

		p1  = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);

		if(p1e & JOY_UP)	curse=curse>1?curse-1:cursemax;
		if(p1e & JOY_DOWN)	curse=curse<cursemax?curse+1:1;
		fixPrint(6, 12, curse == 1 ? 1 : 0, 3, "Test Patterns");
		fixPrint(6, 13, curse == 2 ? 1 : 0, 3, "Video Tests");
		fixPrint(6, 14, curse == 3 ? 1 : 0, 3, "Audio Tests");
		fixPrint(6, 15, curse == 4 ? 1 : 0, 3, "Hardware Tools");

		fixPrint(6, 20, curse == 5 ? 1 : 0, 3, "Help");
		fixPrint(6, 21, curse == 6 ? 1 : 0, 3, "Credits");
		if(showexit)
			fixPrint(6, 22, curse == 7 ? 1 : 0, 3, "Exit");

		menu_footer();

		if(checkHelp(HELP_GENERAL))
			redraw = 1;

		if (p1e & JOY_A)
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
					return;
				break;
			}
			redraw = 1;
		}
	}
}

// This function is called by the BIOS when the start button is
// pressed (AES) and when enough credits are available (MVS)
void player_start(void)
{
    // Tell the BIOS the game has started
    volMEMBYTE(BIOS_USER_MODE) = 0x02;
    // Set player 1's status to running
    volMEMBYTE(BIOS_PLAYER_MOD1) = 0x01;
}

#define RETURN_TO_BIOS	__asm__ ("jmp 0xc00444 \n")

void draw_mvs_demo()
{
	int toggle = 0, demo_frames = 1800, freeplay = 0, redraw = 0;
	picture foreground;
	picture background;

	backgroundColor(0x0000);
	gfxClear();
	pictureInit(&foreground, &gillian, 22, 17, 132, 50, FLIP_NONE);
	palJobPut(17,gillian.palInfo->count,gillian.palInfo->data);
	fixPrint(12, 6, 2, 3, "240p Test Suite");
	fixPrint(10, 26, 2, 3, "2022 Dasutin/Artemio");

	while(demo_frames)
	{
		SCClose();
		waitVBlank();
		
		if(redraw)
		{
			switch(redraw)
			{
				case 1:
					backgroundColor(0x0000);
					pictureInit(&background, &monoscope, 1, 16, 0, 0,FLIP_NONE);
					palJobPut(16,monoscope.palInfo->count,monoscope.palInfo->data);
				break;
				case 2:
					backgroundColor(0xfc1f);
					pictureInit(&background, &colorbarssmpte, 1, 16, 0, 0,FLIP_NONE);
					palJobPut(16,colorbarssmpte.palInfo->count,colorbarssmpte.palInfo->data);
				break;
				case 3:
					backgroundColor(0x0000);
					pictureInit(&background, &grid, 1, 16, 0, 0,FLIP_NONE);
					palJobPut(16,grid.palInfo->count,grid.palInfo->data);
				break;
				default:
					backgroundColor(0x0000);
				break;
			}
			redraw = 0;
		}

		freeplay = !(volMEMBYTE(REG_DIPSW) & DP_FREE);
		p1 = volMEMBYTE(PS_CURRENT);

		if(toggle == 30)
			fixPrint(14, 23, 1, 3, "            ");
		if(toggle == 0)
			fixPrint(14, 23, 1, 3, freeplay ? "PRESS  START" : "INSERT COIN");

		if(volMEMBYTE(SOFT_DIP_2))
			fixPrintf(28, 28, 0, 3, "CREDITS %02d", hexToDec(volMEMBYTE(BIOS_NM_CREDIT)));  // credit counter
		
		toggle ++;
		if(toggle > 60)
			toggle = 0;
		demo_frames--;

		if(demo_frames == 1600)
			redraw = 1;

		if(demo_frames == 1000)
			redraw = 2;

		if(demo_frames == 400)
			redraw = 3;

		if(freeplay && (p1 & P1_START))
		{
			//volMEMBYTE(BIOS_USER_MODE) = 0x02;
			menu_main();
			volMEMBYTE(BIOS_USER_MODE) = 0x01;
			return;
		}
	}
}

void draw_mvs_title()
{
	int toggle = 0, bios_timer = 0, freeplay = 0;
	picture foreground;
	picture background;

	backgroundColor(0x7666);
	gfxClear();

	pictureInit(&foreground, &gillian, 22, 17, 132, 50, FLIP_NONE);
	palJobPut(17,gillian.palInfo->count,gillian.palInfo->data);
	pictureInit(&background, &back,1, 16, 0, 0,FLIP_NONE);
	palJobPut(16,back.palInfo->count,back.palInfo->data);

	fixPrint(10, 26, 0, 3, "2022 Dasutin/Artemio");

	while(1)
	{
		SCClose();
		waitVBlank();
		p1 = volMEMBYTE(PS_CURRENT);
		freeplay = !(volMEMBYTE(REG_DIPSW) & DP_FREE);

		if(toggle == 30)
			fixPrint(14, 23, 1, 3, "            ");
		if(toggle == 0)
			fixPrint(14, 23, 1, 3, "PRESS  START");

		if(!freeplay)
		{
			bios_timer = hexToDec(volMEMBYTE(BIOS_COMP_TIME));
			fixPrintf(16, 28, 0, 3, "TIME:%02d", bios_timer); // BIOS-COMPULSION-TIMER - timer for forced game start
		}

		if(isMVS && volMEMBYTE(SOFT_DIP_2))
			fixPrintf(28, 28, 0, 3, "CREDITS %02d", hexToDec(volMEMBYTE(BIOS_NM_CREDIT)));  // credit counter
		
		toggle ++;
		if(toggle > 60)
			toggle = 0;

		if (p1 & P1_START || (!freeplay && bios_timer <= 0))
		{
			//volMEMBYTE(BIOS_USER_MODE) = 0x02;
			menu_main();
			volMEMBYTE(BIOS_USER_MODE) = 0x01;
			return;
		}
	}
}

void mvs_state()
{
	// BIOS_USER_REQS == 0 is called by BIOS
	// in order to initialize memory
	if(volMEMBYTE(BIOS_USER_REQS) == 0) 
	{
		waitVBlank();
		memset(&bkp_data, 0x00, sizeof(bkp_ram_info));
		// return to bios in order for memory to be saved
		RETURN_TO_BIOS;
	}

	// If DEMO MODE draw the insert coin screen
	if(volMEMBYTE(BIOS_USER_REQS) == 2 && volMEMBYTE(BIOS_USER_MODE) == 1)
	{
		// Enter game mode in MVS following Soft Dip Switches
		if(volMEMBYTE(SOFT_DIP_1))
		{
			draw_mvs_demo();
		}
		else
		{
			// Enter game mode in MVS following Soft Dip Switches
			volMEMBYTE(BIOS_USER_MODE) = 0x02;
			menu_main();
		}
		RETURN_TO_BIOS;
	}

	// If TITLE MODE grab the control
	if(volMEMBYTE(BIOS_USER_REQS) == 3 && volMEMBYTE(BIOS_USER_MODE) == 1)
	{
		// Enter game mode in MVS following Soft Dip Switches
		if(volMEMBYTE(SOFT_DIP_1))
		{
			draw_mvs_title();
		}
		else
		{
			// Enter game mode in MVS following Soft Dip Switches
			volMEMBYTE(BIOS_USER_MODE) = 0x02;
			menu_main();
		}
		RETURN_TO_BIOS;
	}
}

int	main(void)
{
	check_systype();

	clearFixLayer();
	backgroundColor(0x7bbb);
	initGfx();
	palJobPut(0,8,fixPalettes);
	//jobMeterSetup(true);
	SCClose();
	
	if(isMVS)
		mvs_state();
	else
		menu_main();
	
	return 0;
}
