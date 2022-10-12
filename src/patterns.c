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

// MAME says 0x7000 and 0xF000 are the same
// We confirmed with the scope that they are the lowest, and the same
// 0x7111 is slightly higher than 0xF111
// lowest colors are 0x8222, 0xF111, 0x7111, (0x7000 & 0xF000)


/*
A color word is composed in the following manner:
Bit 	15 	14 	13 	12 	11 	10 	9 	8 	7 	6 	5 	4 	3 	2 	1 	0
Def 	Dk  R0	G0	B0	R4	R3	R2	R1	G4	G3	G2	G1	B4	B3	B2	B1
*/

WORD PackColor(short r, short g, short b, BYTE dark)
{
	WORD color = 0;
	BYTE r_lsb, r_msb, g_lsb, g_msb, b_lsb, b_msb;

	r_lsb = r & (short)0x01;
	r_msb = (r & (short)0x1E) >> 1;

	g_lsb = g & (short)0x01;
	g_msb = (g & (short)0x1E) >> 1;

	b_lsb = b & (short)0x01;
	b_msb = (b & (short)0x1E) >> 1;

	color = (dark & 0x01) << 15 | (r_lsb << 14) | (g_lsb << 13) | (b_lsb << 12) | r_msb << 8 | g_msb << 4 | b_msb;
	return color;
}

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
			backgroundColor(0x8000);
			gfxClear();

			pictureInit(&plugergb_back, &plugergb, sprindex, palindex, 0, 0,FLIP_NONE);
			palJobPut(palindex,1,plugergb_pal);
			sprindex += plugergb_back.info->stripSize*2;
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
	int done = 0, draw = 1;
	picture image, red, green, blue, white;
	// These are still experimental, red is still incomplete
	static const ushort white_chart_pal[] = {
		0x8000, 0x0000, 0x8111, 0xf111, 0x8222, 0xf222, 0x8333, 0xf333, 0x8444, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
		0x8000, 0xf444, 0x8555, 0xf555, 0x8666, 0xf666, 0x8777, 0xf777, 0x8888, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
		0x8000, 0xf888, 0x8999, 0xf999, 0x8aaa, 0xfaaa, 0x8bbb, 0xfbbb, 0x8ccc, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
		0x8000, 0xfccc, 0x8ddd, 0xfddd, 0x8eee, 0xfeee, 0x8fff, 0xffff, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000 };
	static const ushort red_chart_pal[] = {
		0x8000, 0x0000, 0x4000, 0x0100, 0x4100, 0x8200, 0xc200, 0x8300, 0xc400, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
		0x8000, 0x4400, 0xc500, 0x4500, 0x8600, 0xc600, 0xc700, 0x4700, 0xc800, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
		0x8000, 0x4800, 0xc900, 0x4900, 0xca00, 0x4a00, 0xcb00, 0x4b00, 0xcc00, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
		0x8000, 0x4c00, 0xcd00, 0x4d00, 0xce00, 0x4e00, 0xcf00, 0x4f00, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000 };
	static const ushort green_chart_pal[] = {
		0x8000, 0x0000, 0x2000, 0x8010, 0x2010, 0x8020, 0x2020, 0x8030, 0x2030, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
		0x8000, 0x8040, 0x2040, 0x8050, 0x2050, 0x8060, 0x2060, 0x8070, 0x2070, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
		0x8000, 0x8080, 0x2080, 0x8090, 0x2090, 0x80a0, 0x20a0, 0x80b0, 0x20b0, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
		0x8000, 0x80c0, 0x20c0, 0x80d0, 0x20d0, 0x80e0, 0x20e0, 0x80f0, 0x20f0, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000 };
	
	while (!done)
	{
		if (draw)
		{
			int index = 1, palindex = 16;

			gfxClear();

			pictureInit(&image, &colorchart, 1, palindex, 0, 0,FLIP_NONE);
			palJobPut(palindex,colorchart.palInfo->count,colorchart.palInfo->data);
			index += image.info->stripSize*2;
			palindex += colorchart.palInfo->count;

			pictureInit(&red, &colorchartsingle, index, palindex, 56, 32, FLIP_NONE);
			palJobPut(palindex,colorchartsingle.palInfo->count,red_chart_pal);
			index += red.info->stripSize*2;
			palindex += colorchartsingle.palInfo->count;

			pictureInit(&green, &colorchartsingle, index, palindex, 56, 72, FLIP_NONE);
			palJobPut(palindex,colorchartsingle.palInfo->count,green_chart_pal);
			index += green.info->stripSize*2;
			palindex += colorchartsingle.palInfo->count;

			pictureInit(&blue, &colorchartsingle, index, palindex, 56, 112, FLIP_NONE);
			palJobPut(palindex,colorchartsingle.palInfo->count,colorchartsingle.palInfo->data);
			index += blue.info->stripSize*2;
			palindex += colorchartsingle.palInfo->count;

			pictureInit(&white, &colorchartsingle, index, palindex, 56, 152, FLIP_NONE);
			palJobPut(palindex,colorchartsingle.palInfo->count,white_chart_pal);

			draw = 0;
		}
		
		SCClose();
		waitVBlank();

		readController();

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
	int done = 0, draw = 1, Is75 = 0, text = 0;
	picture colorebu_back;
	picture colorebu75_back;
	
	while (!done)
	{
		int palindex = 16, sprindex = 1;

		if (draw)
		{
			backgroundColor(0xfc1f);
			gfxClear();

			pictureInit(&colorebu_back, &colorebu, sprindex, palindex, 0, 0,FLIP_NONE);
			palJobPut(palindex,colorebu.palInfo->count,colorebu.palInfo->data);
			sprindex += colorebu_back.info->stripSize*2;
			palindex += colorebu.palInfo->count;

			pictureInit(&colorebu75_back, &colorebu75, sprindex, palindex, 0, 0,FLIP_NONE);
			palJobPut(palindex,colorebu75.palInfo->count,colorebu75.palInfo->data);

			if (!Is75) {
				pictureHide(&colorebu75_back);
				pictureShow(&colorebu_back);
			} else {
				pictureHide(&colorebu_back);
				pictureShow(&colorebu75_back);
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
			Is75 = !Is75;
			if (!Is75) {
				pictureHide(&colorebu75_back);
				pictureShow(&colorebu_back);
				fixPrint(32, 3, fontColorWhite, 3, "100%");
			} else {
				pictureHide(&colorebu_back);
				pictureShow(&colorebu75_back);
				fixPrint(32, 3, fontColorWhite, 3, " 75%");
			}
			text = 60;
			SCClose();
		}

		if (PRESSED_B || PRESSED_START)
			done = 1;

		if (checkHelp(HELP_601CB))
			draw = 1;
	}
}

void tp_smpte_color_bars()
{
	int done = 0, draw = 1, Is75 = 0, text = 0;
	picture colorbarssmpte_back;
	picture colorbarssmpte75_back;

	while (!done)
	{
		int palindex = 16, sprindex = 1;

		if (draw)
		{
			backgroundColor(0xfc1f);
			gfxClear();

			pictureInit(&colorbarssmpte_back, &colorbarssmpte, sprindex, palindex, 0, 0,FLIP_NONE);
			palJobPut(palindex,colorbarssmpte.palInfo->count,colorbarssmpte.palInfo->data);
			sprindex += colorbarssmpte_back.info->stripSize*2;
			palindex += colorbarssmpte.palInfo->count;

			pictureInit(&colorbarssmpte75_back, &colorbarssmpte75, sprindex, palindex, 0, 0,FLIP_NONE);
			palJobPut(palindex,colorbarssmpte75.palInfo->count,colorbarssmpte75.palInfo->data);

			if (!Is75) {
				pictureHide(&colorbarssmpte75_back);
				pictureShow(&colorbarssmpte_back);
			} else {
				pictureHide(&colorbarssmpte_back);
				pictureShow(&colorbarssmpte75_back);
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
			Is75 = !Is75;

			if (!Is75) {
				pictureHide(&colorbarssmpte75_back);
				pictureShow(&colorbarssmpte_back);
				fixPrint(32, 3, fontColorWhite, 3, "100%");
			} else {
				pictureHide(&colorbarssmpte_back);
				pictureShow(&colorbarssmpte75_back);
				fixPrint(32, 3, fontColorWhite, 3, " 75%");
			}
			text = 60;
			SCClose();
		}

		if (PRESSED_B || PRESSED_START)
			done = 1;

		if (checkHelp(HELP_SMPTE))
			draw = 1;
	}
}

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
			sprindex += colorbleedbars_back.info->stripSize*2;
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
	scroller grid;

	while (!done)
	{
		if (draw)
		{
			backgroundColor(0x8000);
			gfxClear();

			scrollerInit(&grid, &grids, 1, 16, getHorScroll(), PATTERN_SCROLL);
			palJobPut(16, grids.palInfo->count, grids.palInfo->data);
			draw = 0;
			updatepalette = 1; 
		}

		if (updatepalette)
		{
			if (!gray)
				volMEMWORD(0x400206) = 0x8000;
			else
				volMEMWORD(0x400206) = 0x7777;
			updatepalette = 0;
		}

		SCClose();
		waitVBlank();

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
}

void tp_monoscope()
{
	int done = 0, draw = 1, pattern = 1, gray = 0;
	picture monoscope_back;

	while (!done)
	{
		if (draw)
		{
			gfxClear();

			pictureInit(&monoscope_back, &monoscope, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,monoscope.palInfo->count,monoscope.palInfo->data);
			draw = 0;
		}

		if (!gray)
			volMEMWORD(0x402206) = 0x8000;
		else
			volMEMWORD(0x402206) = 0x7777;

		switch (pattern)
		{
			case 1:
				volMEMWORD(0x402202) = 0x2fef;
			break;

			case 2:
				volMEMWORD(0x402202) = 0x2ddd;
			break;

			case 3:
				volMEMWORD(0x402202) = 0x2bbb;
			break;

			case 4:
				volMEMWORD(0x402202) = 0x2999;
			break;

			case 5:
				volMEMWORD(0x402202) = 0x2777;
			break;

			case 6:
				volMEMWORD(0x402202) = 0x2555;
			break;

			case 7:
				volMEMWORD(0x402202) = 0x2222;
			break;
		}

		SCClose();
		waitVBlank();

		readController();

		if (PRESSED_A)
		{
			pattern++;
			if (pattern > 7)
				pattern = 1;
		}

		if (PRESSED_B)
			gray = !gray;

		if (PRESSED_START)
			done = 1;

		if (checkHelp(HELP_MONOSCOPE))
			draw = 1;
	}
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

	gfxClear();
	while (!done)
	{
		if (draw)
		{
			switch (color)
			{
				case 1:		// White/Edit
					backgroundColor(edit_color);
				break;

				case 2:		// Black
					backgroundColor(0x0000);
				break;

				case 3:		// Red
					backgroundColor(0x4F00);
				break;

				case 4:		// Green
					backgroundColor(0x20F0);
				break;

				case 5:		// Blue
					backgroundColor(0x100F);
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
	int done = 0, draw = 1, Isbrick = 0;
	picture sharp_back;
	picture sharpbrick_back;

	while (!done)
	{
		int palindex = 16, sprindex = 1;

		if (draw)
		{
			gfxClear();

			pictureInit(&sharp_back, &sharp, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,sharp.palInfo->count,sharp.palInfo->data);
			sprindex += sharp_back.info->stripSize*2;
			palindex += sharp.palInfo->count;

			pictureInit(&sharpbrick_back, &sharpbrick, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,sharpbrick.palInfo->count,sharpbrick.palInfo->data);

			if (!Isbrick) {
				pictureHide(&sharpbrick_back);
				pictureShow(&sharp_back);
			} else {
				pictureHide(&sharp_back);
				pictureShow(&sharpbrick_back);
			}
			draw = 0;
		}

		SCClose();
		waitVBlank();

		readController();

		if (PRESSED_A)
		{
			Isbrick = !Isbrick;

			if (!Isbrick) {
				pictureHide(&sharpbrick_back);
				pictureShow(&sharp_back);
			} else {
				pictureHide(&sharp_back);
				pictureShow(&sharpbrick_back);
			}
			SCClose();
		}

		if (PRESSED_B || PRESSED_START)
			done = 1;

		if (checkHelp(HELP_SHARPNESS))
			draw = 1;
	}
}

void tp_overscan()
{
	int done = 0;
	picture image;

	gfxClear();

	pictureInit(&image, &colorchart, 1, 16, 0, 0,FLIP_NONE);
	palJobPut(16,colorchart.palInfo->count,colorchart.palInfo->data);

	while (!done)
	{
		SCClose();
		waitVBlank();

		readController();

		if (PRESSED_START)
			done = 1;
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