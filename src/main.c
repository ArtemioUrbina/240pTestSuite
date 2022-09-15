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

typedef struct bkp_ram_info {
	WORD debug_dips;
	BYTE stuff[254];
	//256 bytes
} bkp_ram_info;

bkp_ram_info bkp_data;

BYTE p1,p2,ps,p1e,p2e;
BYTE isMVS;

void check_systype()
{
	if(MEMBYTE(BIOS_MVS_FLAG)==SYSTEM_MVS)
		isMVS = 1;
	else
		isMVS = 0;
}

static const ushort fixPalettes[]= {
	0x8000, 0x7fff, 0x0333, 0x5fa7, 0xde85, 0x2c74, 0x2a52, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	0x8000, 0x0f00, 0x0000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	0x8000, 0x00f0, 0x0000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	0x8000, 0x000f, 0x0000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	0x8000, 0xed31, 0xc311, 0xee51, 0x4f81, 0x4fa1, 0x4fc1, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	0x8000, 0xbad3, 0x0111, 0x09c0, 0xe7b0, 0xc580, 0xe250, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	0x8000, 0xefb8, 0x0111, 0xde96, 0x3c75, 0x2950, 0x4720, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	0x8000, 0x8444, 0x0111, 0xf555, 0xf666, 0x7777, 0x8888, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
};

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
	if((MEMBYTE(BIOS_COUNTRY_CODE)==SYSTEM_JAPAN))
	{
		fixPrint(20, 28, 0, 3, isMVS?"Neo Geo MVS":"Neo Geo AES");
		fixPrint(32, 28, 0, 3, "Japan");
	}
	else if ((MEMBYTE(BIOS_COUNTRY_CODE)==SYSTEM_USA))
	{
		fixPrint(22, 28, 0, 3, isMVS?"Neo Geo MVS":"Neo Geo AES");
		fixPrint(34, 28, 0, 3, "USA");
	}
	else if ((MEMBYTE(BIOS_COUNTRY_CODE)==SYSTEM_EUROPE))
	{
		fixPrint(19, 28, 0, 3, isMVS?"Neo Geo MVS":"Neo Geo AES");
		fixPrint(31, 28, 0, 3, "Europe");
	}
}

void menu_tp()
{
	int done = 0, curse = 1, cursemax = 16, redraw = 1;

	while (!done)
	{
		if(redraw)
		{
			clearSprites(1, 1);
			clearFixLayer();
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

		if (p1e & JOY_D)
		{
			clearSprites(1, 22);
			DrawHelp(HELP_GENERAL);
		}

		if (p1e & JOY_A)
		{
			clearSprites(1, 22);
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
			clearSprites(1, 1);
			clearFixLayer();
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

		if (p1e & JOY_D)
		{
			clearSprites(1, 22);
			DrawHelp(HELP_GENERAL);
			draw_background_w_gil();
		}

		if (p1e & JOY_A)
		{
			clearSprites(1, 22);
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
			clearSprites(1, 1);
			clearFixLayer();
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

		if (p1e & JOY_D)
		{
			clearSprites(1, 22);
			DrawHelp(HELP_GENERAL);
			draw_background_w_gil();
		}

		if (p1e & JOY_A)
		{
			clearSprites(1, 22);
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
			clearSprites(1, 1);
			clearFixLayer();
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

		if (p1e & JOY_D)
		{
			clearSprites(1, 22);
			DrawHelp(HELP_GENERAL);
			draw_background_w_gil();
		}

		if (p1e & JOY_A)
		{
			clearSprites(1, 22);
			switch (curse) 
			{
				case 1:
					ht_controller_test();
				break;
					
				case 2:
					
				break;

				case 3:
					//ht_memory_viewer(1);
				break;

				case 4:
					
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

	clearFixLayer();
	clearSprites(1, 22);
	draw_background();

	pictureInit(&qr, &barcode, 26, 17, 260, 110, FLIP_NONE);
	palJobPut(17,barcode.palInfo->count,barcode.palInfo->data);

	while (!done)
	{
		SCClose();
		waitVBlank();
		
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		fixPrint(16, 6, 2, 3, "Credits");
		fixPrint(28, 8, 2, 3, "Ver. 0.1");
		fixPrint(28, 9, 0, 3, "9/4/2022");
		fixPrint(5, 9, 2, 3, "Code by:");
		fixPrint(6, 10, 0, 3, "Dustin Dembrosky");
		fixPrint(6, 11, 0, 3, "Artemio Urbina");
		fixPrint(5, 12, 2, 3, "Patterns:");
		fixPrint(6, 13, 0, 3, "Artemio Urbina");
		fixPrint(5, 14, 2, 3, "Menu Pixel Art:");
		fixPrint(6, 15, 0, 3, "Asher");
		fixPrint(5, 16, 2, 3, "Donna:");
		fixPrint(6, 17, 0, 3, "Jose Salot");
		fixPrint(5, 18, 2, 3, "Neo Geo SDK");
		fixPrint(6, 19, 0, 3, "NeoDev (Jeff Kurtz)");
		fixPrint(5, 20, 2, 3, "Graphics Library");
		fixPrint(6, 21, 0, 3, "DATlib (HPMAN)");
		fixPrint(5, 22, 2, 3, "Info on using this test suite:");
		fixPrint(6, 23, 0, 3, "http://junkerhq.net/240p");

		if (p1e & JOY_B || ps & P1_START)
		{
			done = 1;
		}
	}
	return;
}

int	main(void)
{
	int curse = 1, cursemax = 6, redraw = 1;

	check_systype();

	clearFixLayer();
	backgroundColor(0x7bbb);
	initGfx();
	palJobPut(0,8,fixPalettes);
	//jobMeterSetup(true);

	draw_background_w_gil();

	while(1)
	{
		if(redraw)
		{
			clearSprites(1, 26);
			clearFixLayer();
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

		menu_footer();

		if (p1e & JOY_A)
		{
			clearSprites(1, 22);
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
			}
			redraw = 1;
		}
	}
	return 0;
}
