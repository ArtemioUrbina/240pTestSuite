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
#include "help.h"
#include "string_ng.h"
#include "tools.h"

BYTE first_grid;
BYTE first_overscan;

// MAME says 0x7000 and 0xF000 are the same
// We confirmed with the scope that they are the lowest, and the same
// 0x7111 is slightly higher than 0xF111
// lowest colors are 0x8222, 0xF111, 0x7111, (0x7000 & 0xF000)


//		Experimenting with values, until w eget the AES flashcart we can't measure IRE. Measured with MVS and JAMMA
//      0 transp, 1 fondo,  2 external, 3 top middle, 4 second middle, 5 third middle, 6 fourth middle, 7 external middle, 8 external center
//		2 external, 7 middle, 8 center
static const ushort plugergb_pal[]= {
	//                  XXXXXX                                  XXXXXX  XXXXXX
	//	0x0000, 0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666, 0x7777, 0x8888, 0x9999, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,};
		0x8000, 0x8000, 0x8222, 0x7fff, 0x7bbb, 0x8888, 0x8444, 0xF111, 0x7000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,};
static const ushort plugentsc_pal[]= {
		0x8000, 0x8000, 0x8222, 0x7fff, 0x7bbb, 0x8888, 0x8444, 0xf111, 0x7000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,};

void tp_pluge()
{
	int done = 0, draw = 1, IsNTSC = 0, text = 0;
	picture plugergb_back;
	picture plugentsc_back;

	while (!done)
	{
		int palindex = 16, sprindex = 1;

		if (draw)
		{
			gfxClear();

			pictureInit(&plugergb_back, &plugergb, sprindex, palindex, 0, 0,FLIP_NONE);
			palJobPut(palindex,1,plugergb_pal);
			sprindex += getPicSprites(plugergb_back.info);
			palindex += plugergb.palInfo->count;

			pictureInit(&plugentsc_back, &plugentsc, sprindex, palindex, 0, 0,FLIP_NONE);
			palJobPut(palindex,1,plugentsc_pal);

			if (!IsNTSC) {
				pictureHide(&plugentsc_back);
				pictureShow(&plugergb_back);
			} else {
				pictureHide(&plugergb_back);
				pictureShow(&plugentsc_back);
			}

			draw = 0;
		}

		SCClose();
		waitVBlank();

		readController();

		if (text)
		{
			text--;
			if (!text)
				suiteClearFixLayer();
		}

		if (PRESSED_A)
		{
			IsNTSC = !IsNTSC;
			if (!IsNTSC){
				pictureHide(&plugentsc_back);
				pictureShow(&plugergb_back);
				fixPrint(24, 3, fontColorRed, 3, "RGB FULL RANGE");
			} else {
				pictureHide(&plugergb_back);
				pictureShow(&plugentsc_back);
				fixPrint(24, 3, fontColorRed, 3, "NTSC 7.5 IRE  ");
			}
			text = 60;
			SCClose();
		}

		if (PRESSED_B || PRESSED_START)
			done = 1;

		if (checkHelp(HELP_PLUGE))
			draw = 1;
	}
}

void tp_colorchart()
{
	int done = 0, draw = 1, i = 0, pal = 0, color = 0, mode = 0, scrl_index = 1;
	scroller background; 
	picture red, green, blue, white;
	picture d_red, d_green, d_blue, d_white;
	ushort white_chart_pal[64], red_chart_pal[64], green_chart_pal[64], blue_chart_pal[64];
	ushort d_white_chart_pal[64], d_red_chart_pal[64], d_green_chart_pal[64], d_blue_chart_pal[64];


	memset(white_chart_pal, 0, sizeof(ushort)*64);
	memset(red_chart_pal, 0, sizeof(ushort)*64);
	memset(green_chart_pal, 0, sizeof(ushort)*64);
	memset(blue_chart_pal, 0, sizeof(ushort)*64);

	memset(d_white_chart_pal, 0, sizeof(ushort)*64);
	memset(d_red_chart_pal, 0, sizeof(ushort)*64);
	memset(d_green_chart_pal, 0, sizeof(ushort)*64);
	memset(d_blue_chart_pal, 0, sizeof(ushort)*64);

	color = 0;
	for(pal = 0; pal < 4; pal++)
	{
		for(i = 1; i < 9; i++)
		{
			white_chart_pal[i+pal*16] = PackColor(color, color, color, 0);
			red_chart_pal[i+pal*16] = PackColor(color, 0, 0, 0);
			green_chart_pal[i+pal*16] = PackColor(0, color, 0, 0);
			blue_chart_pal[i+pal*16] = PackColor(0, 0, color, 0);

			color++;
		}
	}

	color = 0;
	for(pal = 0; pal < 4; pal++)
	{
		for(i = 1; i < 9; i++)
		{
			d_white_chart_pal[i+pal*16] = PackColor(color, color, color, 1);
			d_red_chart_pal[i+pal*16] = PackColor(color, 0, 0, 1);
			d_green_chart_pal[i+pal*16] = PackColor(0, color, 0, 1);
			d_blue_chart_pal[i+pal*16] = PackColor(0, 0, color, 1);

			color++;
		}
	}
	

	while (!done)
	{
		if (draw)
		{
			int index = 1, palindex = 16, x = 0, y = 0, back_x = 0;

			gfxClear();

			if(!mode) {
				x = 56;
				y = 32;
			}
			else {
				x = 32;
				y = 36;
				if(vmode_snk)
					back_x = 8;
			}

			if(!mode)
				scrollerInit(&background, &colorchart, scrl_index, 16, 0, PATTERN_SCROLL);
			else
				scrollerInit(&background, &colorchart, scrl_index, 16, 320+back_x, PATTERN_SCROLL);

			palJobPut(palindex, colorchart.palInfo->count, colorchart.palInfo->data);
			index += SCROLLER_SIZE;
			palindex += colorchart.palInfo->count;

			// ===================================
			// Draw Colors No Dark Bit Set
			// ===================================

			pictureInit(&red, &colorchartsingle, index, palindex, x, y, FLIP_NONE);
			palJobPut(palindex,colorchartsingle.palInfo->count,red_chart_pal);
			index += getPicSprites(red.info);
			palindex += colorchartsingle.palInfo->count;
			y += 40;

			// TODO: Figure out getPicSprites
			pictureInit(&green, &colorchartsingle, index, palindex, x, y, FLIP_NONE);
			palJobPut(palindex,colorchartsingle.palInfo->count,green_chart_pal);
			index += getPicSprites(green.info);
			palindex += colorchartsingle.palInfo->count;
			y += 40;

			pictureInit(&blue, &colorchartsingle, index, palindex, x, y, FLIP_NONE);
			palJobPut(palindex,colorchartsingle.palInfo->count,blue_chart_pal);
			index += getPicSprites(blue.info);
			palindex += colorchartsingle.palInfo->count;
			y += 40;

			pictureInit(&white, &colorchartsingle, index, palindex, x, y, FLIP_NONE);
			palJobPut(palindex,colorchartsingle.palInfo->count,white_chart_pal);
			index += getPicSprites(blue.info);
			palindex += colorchartsingle.palInfo->count;

			// ===================================
			// Draw Colors With Dark Bit Set
			// ===================================
			if(!mode)
				y = 48;
			else
				y = 52;

			pictureInit(&d_red, &colorchartsingle, index, palindex, x, y, FLIP_NONE);
			palJobPut(palindex,colorchartsingle.palInfo->count,d_red_chart_pal);
			index += getPicSprites(red.info);
			palindex += colorchartsingle.palInfo->count;
			y += 40;

			// TODO: Figure out getPicSprites
			pictureInit(&d_green, &colorchartsingle, index, palindex, x, y, FLIP_NONE);
			palJobPut(palindex,colorchartsingle.palInfo->count,d_green_chart_pal);
			index += getPicSprites(green.info);
			palindex += colorchartsingle.palInfo->count;
			y += 40;

			pictureInit(&d_blue, &colorchartsingle, index, palindex, x, y, FLIP_NONE);
			palJobPut(palindex,colorchartsingle.palInfo->count,d_blue_chart_pal);
			index += getPicSprites(blue.info);
			palindex += colorchartsingle.palInfo->count;
			y += 40;

			pictureInit(&d_white, &colorchartsingle, index, palindex, x, y, FLIP_NONE);
			palJobPut(palindex,colorchartsingle.palInfo->count,d_white_chart_pal);
			index += getPicSprites(blue.info);
			palindex += colorchartsingle.palInfo->count;

			draw = 0;
		}

		SCClose();
		waitVBlank();

		readController();

		if (PRESSED_A)
		{
			mode = !mode;
			draw = 1;
		}

		if (PRESSED_B || PRESSED_START)
		{
			done = 1;
			return;
		}

		if(checkHelp(HELP_COLORS))
			draw = 1;
	}
}

void tp_colorbars()
{
	int done = 0, draw = 1, Is75 = 1, text = 0, palindex = 16, swap_pal = 0;
	scroller colorebu;
	ushort ebu100_320_pal[] = { 0x8000, 0x7fff, 0x6ff0, 0x30ff, 0x20f0, 0x5f0f, 0x4f00, 0x100f, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000 };
	ushort ebu100_304_pal[] = { 0x8000, 0x7fff, 0x6ff0, 0x30ff, 0x20f0, 0x4f00, 0x100f, 0x8000, 0x5f0f, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000 };
	ushort *ebu_100_pal = NULL, *ebu_075_pal = NULL;
	
	while (!done)
	{
		if (draw)
		{
			gfxClear();

			if(vmode_snk)
			{
				ebu_100_pal = ebu100_304_pal;
				ebu_075_pal = colorebu75_304.palInfo->data;

				scrollerInit(&colorebu, &colorebu75_304, 1, palindex, 0, PATTERN_SCROLL);
			}
			else
			{
				ebu_100_pal = ebu100_320_pal;
				ebu_075_pal = colorebu75.palInfo->data;

				scrollerInit(&colorebu, &colorebu75, 1, palindex, 0, PATTERN_SCROLL);
			}

			swap_pal = 1;
			draw = 0;
		}

		if(swap_pal)
		{
			if (!Is75) {
				palJobPut(palindex, 1, ebu_100_pal);
				if(text)
					fixPrint(32, 3, fontColorWhite, 3, "100%");
			} else {
				palJobPut(palindex, 1, ebu_075_pal);
				if(text)
					fixPrint(32, 3, fontColorWhite, 3, " 75%");
			}
			swap_pal = 0;
		}

		SCClose();
		waitVBlank();

		readController();

		if (text)
		{
			text--;
			if (!text)
				suiteClearFixLayer();
		}

		if (PRESSED_A)
		{
			Is75 = !Is75;

			text = 60;
			swap_pal = 1;
		}

		if (PRESSED_B || PRESSED_START)
			done = 1;

		if (checkHelp(HELP_601CB))
			draw = 1;
	}
}

void tp_smpte_color_bars()
{
	int done = 0, draw = 1, Is75 = 1, text = 0, palindex = 16, swap_pal = 0;
	picture colorbarssmpte_back;
	ushort smpte100_320_pal[] = { 0x8000, 0x7fff, 0x6ff0, 0x100f, 0xf111, 0x30ff, 0x20f0, 0x5f0f, 0x8025, 0x7fff, 0x4f00, 0xc307, 0x7000, 0x8222, 0x8000, 0x8000 };
	ushort smpte100_304_pal[] = { 0x8000, 0x7fff, 0x6ff0, 0x100f, 0xf111, 0x30ff, 0x20f0, 0x5f0f, 0x4f00, 0x8025, 0x7fff, 0xc307, 0x7000, 0x8222, 0x8000, 0x8000 };
	ushort *smpte_100_pal = NULL, *smpte_075_pal = NULL;

	while (!done)
	{
		if (draw)
		{
			gfxClear();

			if(vmode_snk)
			{
				smpte_100_pal = smpte100_304_pal;
				smpte_075_pal = colorbarssmpte75_304.palInfo->data;

				pictureInit(&colorbarssmpte_back, &colorbarssmpte75_304, 1, palindex, 0, 0,FLIP_NONE);
			}
			else
			{
				smpte_100_pal = smpte100_320_pal;
				smpte_075_pal = colorbarssmpte75.palInfo->data;

				pictureInit(&colorbarssmpte_back, &colorbarssmpte75, 1, palindex, 0, 0,FLIP_NONE);
			}

			swap_pal = 1;
			draw = 0;
		}

		if(swap_pal)
		{
			if (!Is75) {
				palJobPut(palindex, 1, smpte_100_pal);
				if(text)
					fixPrint(32, 3, fontColorWhite, 3, "100%");
			} else {
				palJobPut(palindex, 1, smpte_075_pal);
				if(text)
					fixPrint(32, 3, fontColorWhite, 3, " 75%");
			}
			swap_pal = 0;
		}

		SCClose();
		waitVBlank();

		readController();

		if (text)
		{
			text--;
			if (!text)
				suiteClearFixLayer();
		}

		if (PRESSED_A)
		{
			Is75 = !Is75;

			text = 60;
			swap_pal = 1;
		}

		if (PRESSED_B || PRESSED_START)
			done = 1;

		if (checkHelp(HELP_SMPTE))
			draw = 1;
	}
}

/*
23	74.19354839 closest to 75%
Red			0x4B00
Green		0x20B0
Blue		0x100B
Gray		0x7BBB
Cyan		0x30BB
Magenta		0x5B0B
Yellow		0x6BB0
ushort pal_75pct[] = { 0x8000, 0x7BBB, 0x6BB0, 0x30BB, 0x20B0, 0x5B0B, 0x4B00, 0x100B, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000 };
*/

void tp_ref_color_bars()
{
	int done = 0, draw = 1;
	picture refcolor_back;

	while (!done)
	{
		if (draw)
		{
			gfxClear();

			pictureInit(&refcolor_back, &refcolor, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,refcolor.palInfo->count,refcolor.palInfo->data);
			draw = 0;
		}

		SCClose();
		waitVBlank();

		readController();

		if (PRESSED_B || PRESSED_START)
			done = 1;

		if (checkHelp(HELP_601CB))
			draw = 1;
	}
}

void tp_color_bleed_check()
{
	int done = 0, draw = 1, Ischeck = 0;
	picture colorbleedbars_back;
	picture colorbleedcheck_back;

	while (!done)
	{
		int palindex = 16, sprindex = 1;

		if (draw)
		{
			gfxClear();

			pictureInit(&colorbleedbars_back, &colorbleedbars, sprindex, palindex, 0, 0,FLIP_NONE);
			palJobPut(palindex,colorbleedbars.palInfo->count,colorbleedbars.palInfo->data);
			sprindex += getPicSprites(colorbleedbars_back.info);
			palindex += colorbleedbars.palInfo->count;

			pictureInit(&colorbleedcheck_back, &colorbleedcheck, sprindex, palindex, 0, 0,FLIP_NONE);
			palJobPut(palindex,colorbleedcheck.palInfo->count,colorbleedcheck.palInfo->data);

			if (!Ischeck) {
				pictureHide(&colorbleedcheck_back);
				pictureShow(&colorbleedbars_back);
			} else {
				pictureHide(&colorbleedbars_back);
				pictureShow(&colorbleedcheck_back);
			}
			draw = 0;
		}
		SCClose();
		waitVBlank();

		readController();

		if (PRESSED_A)
		{
			Ischeck = !Ischeck;
			if (!Ischeck) {
				pictureHide(&colorbleedcheck_back);
				pictureShow(&colorbleedbars_back);
			} else {
				pictureHide(&colorbleedbars_back);
				pictureShow(&colorbleedcheck_back);
			}
			SCClose();
		}

		if (PRESSED_B || PRESSED_START)
			done = 1;

		if (checkHelp(HELP_BLEED))
			draw = 1;
	}
}

void tp_grid()
{
	int done = 0, draw = 1, updatepalette = 0, gray = 0;
	WORD color = 0x8000;
	scroller grid;

	color = 0x8000;
	while (!done)
	{
		if (draw)
		{
			int index = 1, palindex = 16;
			gfxClear();
			backgroundColor(color);

			scrollerInit(&grid, &grids, index, palindex, getHorScroll(), PATTERN_SCROLL);
			palJobPut(palindex, grids.palInfo->count, grids.palInfo->data);
			index += SCROLLER_SIZE;
			palindex += grids.palInfo->count;

			draw = 0;
			updatepalette = 1; 
			if(first_grid)
			{
				if(isMVS && !AES_AS_MVS && !vmode_snk && !isMulti)
					draw_warning("Some later MVS systems can't\ndisplay the last few pixels\nwhen in full 320 mode.", index, palindex, 0);
				first_grid = 0;
				draw = 1;
			}
		}

		SCClose();
		waitVBlank();

		// these is non buffered, execute after in hardware
		if (updatepalette)
		{
			if (!gray)
				color = 0x8000;
			else
				color = PackColor(7, 7, 7, 0);
			VRAM_PAL(16, 3) = color;
			VRAM_PAL(0, 2)  = color;	// SNK BIOS Border
			backgroundColor(color);
			updatepalette = 0;
		}

		readController();

		if (PRESSED_A)
		{
			gray = !gray;
			updatepalette = 1;
		}

		if (PRESSED_B || PRESSED_START)
			done = 1;

		if(checkHelp(HELP_GRID))
			draw = 1;
	}
	VRAM_PAL(0, 2) = 0x8000;	// restore SNK BIOS Border Color
}

void tp_monoscope()
{
	int done = 0, draw = 1, pattern = 0, gray = 0, updatepalette = 1, changepattern = 1;
	int bright[7] = { 31, 25, 20, 15, 10, 6, 2 }, color = 0;
	picture monoscope_back;

	color = 0x8000;
	while (!done)
	{
		if (draw)
		{
			gfxClear();

			pictureInit(&monoscope_back, &monoscope, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,monoscope.palInfo->count,monoscope.palInfo->data);
			
			updatepalette = 1;
			changepattern = 1;
			draw = 0;
		}
		
		SCClose();
		waitVBlank();

		// these are non buffered, execute after in hardware
		if (updatepalette)
		{
			if (!gray)
				color = 0x8000;
			else
				color = PackColor(7, 7, 7, 0);
			VRAM_PAL(16, 3) = color;
			VRAM_PAL(0, 2)  = color;	// SNK BIOS Border
			backgroundColor(color);
			updatepalette = 0;
		}

		if(changepattern)
		{
			VRAM_PAL(16, 1) = PackColor(bright[pattern], bright[pattern], bright[pattern], 0);
			changepattern = 0;
		}

		readController();

		if (PRESSED_A)
		{
			pattern++;
			if (pattern > 6)
				pattern = 0;
			changepattern = 1;
		}

		if (PRESSED_B)
		{
			gray = !gray;
			updatepalette = 1;
		}

		if (PRESSED_START)
			done = 1;

		if (checkHelp(HELP_MONOSCOPE))
			draw = 1;
	}
	VRAM_PAL(0, 2) = 0x8000;	// SNK BIOS Border
}

void tp_gray_ramp()
{
	int done = 0, draw = 1;
	picture grayramp_back;

	while (!done)
	{
		if (draw)
		{
			gfxClear();

			pictureInit(&grayramp_back, &grayramp, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,grayramp.palInfo->count,grayramp.palInfo->data);
			draw = 0;
		}

		SCClose();
		waitVBlank();

		readController();

		if (PRESSED_B || PRESSED_START)
			done = 1;

		if (checkHelp(HELP_GRAY))
			draw = 1;
	}
}

void tp_white_rgb()
{
	int done = 0, color = 1, draw = 1, editmode = 0, sel = 0;
	short r = 31, g = 31, b = 31, dark = 0, text = 0;
	WORD edit_color = 0x7FFF;

	while (!done)
	{
		if (draw)
		{
			gfxClear();
			switch (color)
			{
				case 1:		// White/Edit
					backgroundColor(edit_color);
				break;

				case 2:		// Black
					backgroundColor(0x0000);
				break;

				case 3:		// Red
					backgroundColor(PackColor(31, 0, 0, 0));
				break;

				case 4:		// Green
					backgroundColor(PackColor(0, 31, 0, 0));
				break;

				case 5:		// Blue
					backgroundColor(PackColor(0, 0, 31, 0));
				break;
			}
			draw = 0;
		}

		if (text)
		{
			text--;
			if (!text)
				suiteClearFixLayer();
		}

		if (editmode)
		{
			fixPrintf(15, 5, fontColorSolid, 4, "%cR:%02d %cG:%02d %cB:%02d %cD: %d", 
								sel == 0 ? '>' : ' ', r, 
								sel == 1 ? '>' : ' ', g,
								sel == 2 ? '>' : ' ', b,
								sel == 3 ? '>' : ' ', dark);
		}
		
		SCClose();
		waitVBlank();

		readController();

		if (!editmode)
		{
			if (PRESSED_A)
			{
				color++;
				draw = 1;
			}

			if (PRESSED_B)
			{
				color--;
				draw = 1;
			}

			if (color > 5)
				color = 1;

			if (color < 1)
				color = 5;

			if (draw)
			{
				char colorname[20];

				switch (color)
				{
					case 1:
						sprintf(colorname, "%s", edit_color != 0x7FFF ? "Edit " : "White");
					break;

					case 2:
						sprintf(colorname, "Black");
					break;

					case 3:
						sprintf(colorname, "Red  ");
					break;

					case 4:
						sprintf(colorname, "Green");
					break;

					case 5:
						sprintf(colorname, "Blue ");
					break;
				}
				fixPrintf(28, 25, color == 1 ? fontColorGreen : fontColorWhite, 3, colorname);
				text = 60;
			}

			if (checkHelp(HELP_WHITE))
				draw = 1;
		}

		if (PRESSED_C && color == 1)
		{
			editmode = !editmode;
			if (!editmode)
				suiteClearFixLayer();
		}

		if (PRESSED_START)
			done = 1;

		if (editmode)
		{
			short *edit = NULL, hasedit = 0;

			if (PRESSED_LEFT)
				sel --;

			if (PRESSED_RIGHT)
				sel ++;

			if (sel < 0)
				sel = 3;
			if (sel > 3)
				sel = 0;

			switch (sel)
			{
				case 0:
					edit = &r;
				break;

				case 1:
					edit = &g;
				break;

				case 2:
					edit = &b;
				break;

				case 3:
					edit = &dark;
				break;
			}

			if (PRESSED_UP)
			{
				(*edit)++;
				hasedit = 1;
			}

			if (PRESSED_DOWN)
			{
				(*edit)--;
				hasedit = 1;
			}

			if (PRESSED_A)
			{
				*edit = 0;
				hasedit = 1;
			}

			if (PRESSED_B)
			{
				*edit = 31;
				hasedit = 1;
			}

			if (PRESSED_D)
			{
				dark = !dark;
				hasedit = 1;
			}

			if (hasedit)
			{
				if (sel != 3)
				{
					if(*edit < 0)
						*edit = 0;
					if(*edit > 31)
						*edit = 31;
				}

				if (sel == 3)
				{
					if(*edit < 0)
						*edit = 0;
					if(*edit > 1)
						*edit = 1;
				}

				edit_color = PackColor(r, g, b, dark);
				backgroundColor(edit_color);
			}

			if (bkp_data.debug_dip1 & DP_DEBUG1)
			{
				char buffer[7];

				sprintf(buffer, "0x");
				intToHex(edit_color, buffer+2, 4);
				fixPrint(23, 7, fontColorSolid, 4, buffer);
			}
		}
	}
}

void tp_100_ire()
{
	int done = 0, text = 0, draw = 1;
	ushort irevals[] = {13,25,41,53,66,82,94};
	int irenum = 6;
	picture ire_back;

	while (!done)
	{
		if (draw)
		{
			gfxClear();

			pictureInit(&ire_back, &ire, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,ire.palInfo->count,ire.palInfo->data);
			draw = 0;
		}

		switch (irenum)
		{
			case 0:
				volMEMWORD(0x402204) = 0x2222;
			break;

			case 1:
				volMEMWORD(0x402204) = 0x2555;
			break;

			case 2:
				volMEMWORD(0x402204) = 0x2777;
			break;

			case 3:
				volMEMWORD(0x402204) = 0x2999;
			break;

			case 4:
				volMEMWORD(0x402204) = 0x2bbb;
			break;

			case 5:
				volMEMWORD(0x402204) = 0x2ddd;
			break;

			case 6:
				volMEMWORD(0x402204) = 0x2fef;
			break;
		}

		SCClose();
		waitVBlank();

		readController();

		if (PRESSED_A)
		{
			if (irenum != 0)
				irenum--;
			fixPrintf1(32, 25, fontColorWhite, 3, "IRE:%u", irevals[irenum]);
			text = 60;
		}

		if (PRESSED_B)
		{
			if (irenum != 6)
				irenum++;
			fixPrintf1(32, 25, fontColorWhite, 3, "IRE:%u", irevals[irenum]);
			text = 60;
		}

		if (PRESSED_START)
			done = 1;

		if (checkHelp(HELP_IRE))
			draw = 1;

		if (text)
		{
			text--;
			if (!text)
				suiteClearFixLayer();
		}
	}
}

void tp_sharpness()
{
	int done = 0, draw = 1, Isbrick = 0, changed = 0;
	picture sharp_back;
	picture sharpbrick_back;

	while (!done)
	{
		int palindex = 16, sprindex = 1;

		if (draw)
		{
			gfxClear();

			pictureInit(&sharp_back, &sharp, sprindex, palindex, 0, 0,FLIP_NONE);
			palJobPut(palindex,sharp.palInfo->count,sharp.palInfo->data);
			sprindex += getPicSprites(sharp_back.info);
			palindex += sharp.palInfo->count;

			pictureInit(&sharpbrick_back, &sharpbrick, sprindex, palindex, 0, 0,FLIP_NONE);
			palJobPut(palindex,sharpbrick.palInfo->count,sharpbrick.palInfo->data);

			draw = 0;
			changed = 1;
		}

		if(changed)
		{
			if (!Isbrick) {
				pictureHide(&sharpbrick_back);
				pictureShow(&sharp_back);
			} else {
				pictureHide(&sharp_back);
				pictureShow(&sharpbrick_back);
			}
			changed = 0;
		}

		SCClose();
		waitVBlank();

		readController();

		if (PRESSED_A)
		{
			Isbrick = !Isbrick;
			changed = 1;
			SCClose();
		}

		if (PRESSED_B || PRESSED_START)
			done = 1;

		if (checkHelp(HELP_SHARPNESS))
			draw = 1;
	}
}

#define BAR_WIDTH	64

void tp_overscan()
{
	int done = 0, redraw = 1, sel = 1, scroll = 1, fast = 0;
	int top_y, bottom_y, left_x, right_x, side_y;
	int t_max, t_min, l_max, l_min, b_max, b_min, r_max, r_min;
	scroller top, bottom, left, right, top_cover, bottom_cover;
	
	// Setup ratios depending on NTSC/PAL and 304/320
	if(isPAL && usePAL256)
		side_y = 16;
	else
		side_y = 0;

	if(isPAL && usePAL256)
		t_min = 16;
	else
		t_min = 0;
	t_max = t_min + BAR_WIDTH;
	top_y = t_max;

	if(isPAL && usePAL256)
		b_min = -240;
	else
		b_min = -224;
	b_max = b_min + BAR_WIDTH;
	bottom_y = b_min;
	
	l_max = 400;
	l_min = l_max - BAR_WIDTH;
	left_x = l_max;

	r_min = 0;
	r_max = r_min + BAR_WIDTH;
	right_x = r_min;

	if(vmode_snk)
	{
		r_min += 8;
		r_max += 8;
		right_x += 8;

		l_max -= 8;
		l_min -= 8;
		left_x -= 8;
	}

	while (!done)
	{
		if (redraw)
		{
			int sprindex = 1, palindex = 16;

			gfxClear();
			backgroundColor(PackColor(10, 10, 10, 0));

			palJobPut(palindex, overscan_vert.palInfo->count, overscan_vert.palInfo->data);

			scrollerInit(&top, &overscan_vert, sprindex, palindex, 0, top_y);
			sprindex += SCROLLER_SIZE;

			scrollerInit(&bottom, &overscan_vert, sprindex, palindex, 0, bottom_y);
			sprindex += SCROLLER_SIZE;

			scrollerInit(&left, &overscan_horz, sprindex, palindex, left_x, side_y);
			sprindex += SCROLLER_SIZE;

			scrollerInit(&right, &overscan_horz, sprindex, palindex, right_x, side_y);
			sprindex += SCROLLER_SIZE;

			if(isPAL && !usePAL256)  // yeah... I got myself into this
			{
				scrollerInit(&top_cover, &overscan_vert, sprindex, fontColorBlack, 0, t_max);
				sprindex += SCROLLER_SIZE;

				scrollerInit(&bottom_cover, &overscan_vert, sprindex, fontColorBlack, 0, b_min);
				sprindex += SCROLLER_SIZE;
			}

			redraw = 0;
			scroll = 1;

			if(vmode_snk && first_overscan)
			{
				draw_warning("You can draw 16 more pixels\nhorizontally by enabling\n320 mode in options.", sprindex, palindex, 0);
				redraw = 1;
				first_overscan = 0;
			}
		}

		if (scroll)
		{
			scrollerSetPos(&top, 0, top_y);
			scrollerSetPos(&bottom, 0, bottom_y);
			scrollerSetPos(&left, left_x, side_y);
			scrollerSetPos(&right, right_x, side_y);
			if(isPAL&&!usePAL256)
			{
				scrollerSetPos(&top_cover, 0, t_max);
				scrollerSetPos(&bottom_cover, 0, b_min);
			}

			fixPrintf(9, 14, fontColorWhite, 3, " Top overscan :   %3d", t_max - top_y);
			fixPrintf(9, 15, fontColorWhite, 3, " Bottom overscan: %3d", bottom_y - b_min);
			fixPrintf(9, 16, fontColorWhite, 3, " Left overscan:   %3d", l_max - left_x);
			fixPrintf(9, 17, fontColorWhite, 3, " Right overscan:  %3d", right_x - r_min);
			fixPrintf(9, 13+sel, fast ? fontColorRed : fontColorGreen, 3, ">");

			if (bkp_data.debug_dip1 & DP_DEBUG1)
			{
				fixPrintf(8, 18, fontColorGreen, 3, "Top Y:    %4d(%04d/%04d)", top_y, t_min, t_max);
				fixPrintf(8, 19, fontColorGreen, 3, "Bottom Y: %4d(%04d/%04d)", bottom_y, b_min, b_max);
				fixPrintf(8, 20, fontColorGreen, 3, "Left X:   %4d(%04d/%04d)", left_x, l_min, l_max);
				fixPrintf(8, 21, fontColorGreen, 3, "Right X:  %4d(%04d/%04d)", right_x, r_min, r_max);
			}

			scroll = 0;
		}

		SCClose();
		waitVBlank();

		readController();

		if (PRESSED_UP)
		{
			sel -= 1;
			scroll = 1;
		}

		if (PRESSED_DOWN)
		{
			sel += 1;
			scroll = 1;
		}

		if (scroll)
		{
			if(sel < 1)
				sel = 4;
			if(sel > 4)
				sel = 1;
		}

		if (PRESSED_RIGHT || (fast && HELD_RIGHT))
		{
			switch(sel)
			{
			case 1:
				top_y -= 1;
				if(top_y < t_min)
					top_y = t_min;
				break;
			case 2:
				bottom_y += 1;
				if(bottom_y > b_max)
					bottom_y = b_max;
				break;
			case 3:
				left_x -= 1;
				if(left_x < l_min)
					left_x = l_min;
				break;
			case 4:
				right_x += 1;
				if(right_x > r_max)
					right_x = r_max;
				break;
			}
			scroll = 1;
		}

		if (PRESSED_LEFT || (fast && HELD_LEFT))
		{
			switch(sel)
			{
			case 1:
				top_y += 1;
				if(top_y > t_max)
					top_y = t_max;
				break;
			case 2:
				bottom_y -= 1;
				if(bottom_y < b_min)
					bottom_y = b_min;
				break;
			case 3:
				left_x += 1;
				if(left_x > l_max)
					left_x = l_max;
				break;
			case 4:
				right_x -= 1;
				if(right_x < r_min)
					right_x = r_min;
				break;
			}
			scroll = 1;
		}

		if (PRESSED_A)
		{
			top_y = t_max;
			bottom_y = b_min;
			left_x = l_max;
			right_x = r_min;

			scroll = 1;
		}
		
		if (PRESSED_C)
		{
			fast = !fast;
			scroll = 1;
		}

		if (PRESSED_START)
			done = 1;

		if (checkHelp(HELP_OVERSCAN))
			redraw = 1;
	}
}

void tp_convergence()
{
	int done = 0, pattern = 1, draw = 1;
	picture image;

	while (!done)
	{
		if (draw)
		{
			gfxClear();

			switch (pattern)
			{
				case 1:
					pictureInit(&image, &conver_grid, 1, 16, 0, 0,FLIP_NONE);
					palJobPut(16,conver_grid.palInfo->count,conver_grid.palInfo->data);
				break;

				case 2:
					pictureInit(&image, &conver_dot, 1, 16, 0, 0,FLIP_NONE);
					palJobPut(16,conver_dot.palInfo->count,conver_dot.palInfo->data);
				break;

				case 3:
					pictureInit(&image, &conver_star, 1, 16, 0, 0,FLIP_NONE);
					palJobPut(16,conver_star.palInfo->count,conver_star.palInfo->data);
				break;

				case 4:
					pictureInit(&image, &conver_rgb, 1, 16, 0, 0,FLIP_NONE);
					palJobPut(16,conver_rgb.palInfo->count,conver_rgb.palInfo->data);
				break;

				case 5:
					pictureInit(&image, &conver_rgb_b, 1, 16, 0, 0,FLIP_NONE);
					palJobPut(16,conver_rgb_b.palInfo->count,conver_rgb_b.palInfo->data);
				break;
			}
			draw = 0;
		}

		SCClose();
		waitVBlank();

		readController();

		if (PRESSED_A)
		{
			pattern++;
			if (pattern > 3)
			{
				pattern = 1;
			}
			draw = 1;
		}

		if (PRESSED_B)
		{
			if (pattern < 3)
			{
				pattern = 4;
			}
			pattern++;
			if (pattern > 5)
			{
				pattern = 4;
			}
			draw = 1;
		}

		if (PRESSED_START)
			done = 1;

		if (checkHelp(HELP_CONVERGENCE))
			draw = 1;
	}
}