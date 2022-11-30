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
BYTE first_colorramp;

void tp_pluge()
{
	int done = 0, draw = 1, IsNTSC = 0, text = 0, swappal = 0;
	picture pluge_back;
	ushort plugergb_pal[]= {
		_BLACK, _BLACK, IRE7_5, WH_100, IRE_75, IRE_50, IRE_24, IRE4_5, IRE2_5, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK,};
	ushort plugentsc_pal[]= {
		_BLACK, _BLACK, IRE_10, WH_100, IRE_75, IRE_50, IRE_24, IRE7_5, IRE4_5, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK,};

	while (!done)
	{
		if (draw)
		{
			gfxClear();

			pictureInit(&pluge_back, &pluge, 1, 16, 0, 0,FLIP_NONE);
			
			swappal = 1;
			draw = 0;
		}

		if(swappal)
		{
			if(IsNTSC)	{
				palJobPut(16, 1, plugentsc_pal);
			}
			else {
				palJobPut(16, 1, plugergb_pal);
			}
			swappal = 0;
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

		if (BTTN_MAIN)
		{
			IsNTSC = !IsNTSC;
			if (!IsNTSC){
				fixPrint(24, 3, fontColorRed, 3, "RGB FULL RANGE");
			} else {;
				fixPrint(24, 3, fontColorRed, 3, "NTSC 7.5 IRE  ");
			}
			swappal = 1;
			text = 60;
		}

		if (BTTN_EXIT)
			done = 1;

		if (checkHelp(HELP_PLUGE))
			draw = 1;
	}
}

void tp_colorchart()
{
	int done = 0, draw = 1, i = 0, pal = 0, color = 0, mode = 0, scrl_index = 1;
	int dark_mode = 0, change_pallettes = 0;
	int pal_red_top = 0, pal_green_top = 0, pal_blue_top = 0, pal_white_top = 0;
	int pal_red_bottom = 0, pal_green_bottom = 0, pal_blue_bottom = 0, pal_white_bottom = 0;
	int index = 1, palindex = 16;
	ushort white_chart_pal[64], red_chart_pal[64], green_chart_pal[64], blue_chart_pal[64];
	ushort d_white_chart_pal[64], d_red_chart_pal[64], d_green_chart_pal[64], d_blue_chart_pal[64];
	picture red, green, blue, white;
	picture d_red, d_green, d_blue, d_white;
	picture text_light, text_dark;
	scroller background; 

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

	if(!allowIRE107)
	{
		red_chart_pal[8+3*16] = _BLACK;
		green_chart_pal[8+3*16] = _BLACK;
		blue_chart_pal[8+3*16] = _BLACK;
		white_chart_pal[8+3*16] = _BLACK;
	}
	
	while (!done)
	{
		if (draw)
		{
			int x = 0, y = 0, back_x = 0;

			index = 1;
			palindex = 16;

			gfxClear();

			if(!mode) {
				x = 48;
				y = 32;
			}
			else {
				x = 32;
				y = 36;
				if(vmode_snk)
					back_x = 8;
			}

			if(!mode)
				scrollerInit(&background, &colorchart, scrl_index, palindex, 0, PATTERN_SCROLL);
			else
				scrollerInit(&background, &colorchart, scrl_index, palindex, 320+back_x, PATTERN_SCROLL);

			palJobPut(palindex, colorchart.palInfo->count, colorchart.palInfo->data);
			index += SCROLLER_SIZE;
			palindex += colorchart.palInfo->count;

			// ===================================
			// Draw Colors No Dark Bit Set
			// ===================================

			pal_red_top = palindex;
			pictureInit(&red, &colorchartsingle, index, palindex, x, y, FLIP_NONE);
			index += getPicSprites(red.info);
			palindex += colorchartsingle.palInfo->count;
			y += 40;

			pal_green_top = palindex;
			pictureInit(&green, &colorchartsingle, index, palindex, x, y, FLIP_NONE);
			index += getPicSprites(green.info);
			palindex += colorchartsingle.palInfo->count;
			y += 40;

			pal_blue_top = palindex;
			pictureInit(&blue, &colorchartsingle, index, palindex, x, y, FLIP_NONE);
			index += getPicSprites(blue.info);
			palindex += colorchartsingle.palInfo->count;
			y += 40;

			pal_white_top = palindex;
			pictureInit(&white, &colorchartsingle, index, palindex, x, y, FLIP_NONE);
			index += getPicSprites(blue.info);
			palindex += colorchartsingle.palInfo->count;

			// ===================================
			// Draw Colors With Dark Bit Set
			// ===================================
			if(!mode)
				y = 48;
			else
				y = 52;

			pal_red_bottom = palindex;
			pictureInit(&d_red, &colorchartsingle, index, palindex, x, y, FLIP_NONE);
			index += getPicSprites(red.info);
			palindex += colorchartsingle.palInfo->count;
			y += 40;

			pal_green_bottom = palindex;
			pictureInit(&d_green, &colorchartsingle, index, palindex, x, y, FLIP_NONE);
			index += getPicSprites(green.info);
			palindex += colorchartsingle.palInfo->count;
			y += 40;

			pal_blue_bottom = palindex;
			pictureInit(&d_blue, &colorchartsingle, index, palindex, x, y, FLIP_NONE);
			index += getPicSprites(blue.info);
			palindex += colorchartsingle.palInfo->count;
			y += 40;

			pal_white_bottom = palindex;
			pictureInit(&d_white, &colorchartsingle, index, palindex, x, y, FLIP_NONE);
			index += getPicSprites(blue.info);
			palindex += colorchartsingle.palInfo->count;

			// ===================================
			// Draw Color Text
			// ===================================
			if(!mode)
			{
				pictureInit(&text_light, &colorchart_light, index, palindex, 16, 32, FLIP_NONE);
				palJobPut(palindex,colorchart_light.palInfo->count,colorchart_light.palInfo->data);
				index += getPicSprites(text_light.info);
				palindex += colorchart_light.palInfo->count;
				
				pictureInit(&text_dark, &colorchart_dark, index, palindex, 0, 48, FLIP_NONE);
				palJobPut(palindex,colorchart_dark.palInfo->count,colorchart_dark.palInfo->data);
				index += getPicSprites(text_dark.info);
				palindex += colorchart_dark.palInfo->count;
			}

			draw = 0;
			change_pallettes = 1;
		}

		if(change_pallettes)
		{
			int dark = 0, light = 0, y = 32;

			if(dark_mode == 0 || dark_mode == 1)
				light = 1;

			if(!mode && light)
			{
				pictureSetPos(&text_light, 16, y);
				pictureShow(&text_light);
				y += 16;
			}
			else
				pictureHide(&text_light);

			palJobPut(pal_red_top,colorchartsingle.palInfo->count, light ? red_chart_pal : d_red_chart_pal);
			palJobPut(pal_green_top,colorchartsingle.palInfo->count, light ? green_chart_pal : d_green_chart_pal);
			palJobPut(pal_blue_top,colorchartsingle.palInfo->count, light ? blue_chart_pal : d_blue_chart_pal);
			palJobPut(pal_white_top,colorchartsingle.palInfo->count, light ? white_chart_pal : d_white_chart_pal);

			if(dark_mode == 0 || dark_mode == 2)
				dark = 1;

			if(!mode && dark)
			{
				if (y == 32) y = 24;
				pictureSetPos(&text_dark, 0, y);
				pictureShow(&text_dark);
			}
			else
				pictureHide(&text_dark);

			palJobPut(pal_red_bottom,colorchartsingle.palInfo->count, dark ? d_red_chart_pal : red_chart_pal);
			palJobPut(pal_green_bottom,colorchartsingle.palInfo->count, dark ? d_green_chart_pal : green_chart_pal);
			palJobPut(pal_blue_bottom,colorchartsingle.palInfo->count, dark ? d_blue_chart_pal : blue_chart_pal);
			palJobPut(pal_white_bottom,colorchartsingle.palInfo->count, dark ? d_white_chart_pal : white_chart_pal);

			change_pallettes = 0;
		}

		if(first_colorramp && !allowIRE107)
		{
			draw_warning("Limiting IRE to 100.\nAs a result the top bar on the\nramps will be blacked out.", index, palindex, 0);
			first_colorramp = 0;
			draw = 1;
		}

		SCClose();
		waitVBlank();

		readController();

		if (BTTN_MAIN)
		{
			dark_mode++;
			if(dark_mode > 2)
				dark_mode = 0;
			change_pallettes = 1;
		}

		if (BTTN_EXIT)
		{
			done = 1;
			return;
		}

		if (BTTN_OPTION_1)
		{
			int x = 0, y = 0, back_x = 0;

			mode = !mode;
			if(!mode) {
				x = 48;
				y = 32;
				scrollerSetPos(&background, 0, PATTERN_SCROLL);
			}
			else {
				x = 32;
				y = 36;
				if(vmode_snk)
					back_x = 8;
				scrollerSetPos(&background, 320+back_x, PATTERN_SCROLL);
			}

			pictureSetPos(&red, x, y);
			y += 40;
			pictureSetPos(&green, x, y);
			y += 40;
			pictureSetPos(&blue, x, y);
			y += 40;
			pictureSetPos(&white, x, y);
			y += 40;

			if(!mode)
				y = 48;
			else
				y = 52;

			pictureSetPos(&d_red, x, y);
			y += 40;
			pictureSetPos(&d_green, x, y);
			y += 40;
			pictureSetPos(&d_blue, x, y);
			y += 40;
			pictureSetPos(&d_white, x, y);
			y += 40;

			change_pallettes = 1;
		}

		if(checkHelp(HELP_COLORS))
			draw = 1;
	}
}


void tp_colorbars()
{
	int done = 0, draw = 1, Is75 = 1, text = 0, palindex = 16, swap_pal = 0;
	scroller colorebu;
	ushort ebu107_320_pal[] = { _BLACK, WH_107, YEL107, CYN107, GRN107, MAG107, RED107, BLU107, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK };
	ushort ebu107_304_pal[] = { _BLACK, WH_107, YEL107, CYN107, GRN107, RED107, BLU107, _BLACK, MAG107, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK };
	ushort ebu100_320_pal[] = { _BLACK, WH_100, YEL100, CYN100, GRN100, MAG100, RED100, BLU100, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK };
	ushort ebu100_304_pal[] = { _BLACK, WH_100, YEL100, CYN100, GRN100, RED100, BLU100, _BLACK, MAG100, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK };
	ushort *ebu_100_pal = NULL, *ebu_075_pal = NULL;
	
	while (!done)
	{
		if (draw)
		{
			gfxClear();

			if(vmode_snk)
			{
				ebu_100_pal = allowIRE107 ? ebu107_304_pal : ebu100_304_pal;
				ebu_075_pal = colorebu75_304.palInfo->data;

				scrollerInit(&colorebu, &colorebu75_304, 1, palindex, 0, PATTERN_SCROLL);
			}
			else
			{
				ebu_100_pal = allowIRE107 ? ebu107_320_pal : ebu100_320_pal;
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
					fixPrint(32, 3, fontColorWhite, 3, allowIRE107 ? "107%" : "100%");
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

		if (BTTN_MAIN)
		{
			Is75 = !Is75;

			text = 60;
			swap_pal = 1;
		}

		if (BTTN_EXIT)
			done = 1;

		if (checkHelp(HELP_EBU))
			draw = 1;
	}
}


void tp_smpte_color_bars()
{
	int done = 0, draw = 1, Is75 = 1, text = 0, palindex = 16, swap_pal = 0;
	picture colorbarssmpte_back;
	ushort smpte107_320_pal[] = { _BLACK, WH_107, YEL107, BLU107, IRE7_5, CYN107, GRN107, MAG107, NEG__I, WH_107, RED107, POS__Q, IRE2_5, IRE_10, _BLACK, _BLACK };
	ushort smpte107_304_pal[] = { _BLACK, WH_107, YEL107, BLU107, IRE7_5, CYN107, GRN107, MAG107, RED107, NEG__I, WH_107, POS__Q, IRE2_5, IRE_10, _BLACK, _BLACK };
	ushort smpte100_320_pal[] = { _BLACK, WH_100, YEL100, BLU100, IRE7_5, CYN100, GRN100, MAG100, NEG__I, WH_100, RED100, POS__Q, IRE2_5, IRE_10, _BLACK, _BLACK };
	ushort smpte100_304_pal[] = { _BLACK, WH_100, YEL100, BLU100, IRE7_5, CYN100, GRN100, MAG100, RED100, NEG__I, WH_100, POS__Q, IRE2_5, IRE_10, _BLACK, _BLACK };
	ushort smpte075_320_pal[] = { _BLACK, IRE_75, YEL075, BLU075, IRE7_5, CYN075, GRN075, MAG075, NEG__I, WH_100, RED075, POS__Q, IRE2_5, IRE_10, _BLACK, _BLACK };
	ushort smpte075_304_pal[] = { _BLACK, IRE_75, YEL075, BLU075, IRE7_5, CYN075, GRN075, MAG075, RED075, NEG__I, WH_100, POS__Q, IRE2_5, IRE_10, _BLACK, _BLACK };
	ushort *smpte_100_pal = NULL, *smpte_075_pal = NULL;

	while (!done)
	{
		if (draw)
		{
			gfxClear();

			if(vmode_snk)
			{
				smpte_100_pal = allowIRE107 ? smpte107_304_pal : smpte100_304_pal;
				smpte_075_pal = smpte075_304_pal;

				pictureInit(&colorbarssmpte_back, &colorbarssmpte75_304, 1, palindex, 0, 0,FLIP_NONE);
			}
			else
			{
				smpte_100_pal = allowIRE107 ? smpte107_320_pal : smpte100_320_pal;
				smpte_075_pal = smpte075_320_pal;

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
					fixPrint(32, 3, fontColorWhite, 3, allowIRE107 ? "107%" : "100%");
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

		if (BTTN_MAIN)
		{
			Is75 = !Is75;

			text = 60;
			swap_pal = 1;
		}

		if (BTTN_EXIT)
			done = 1;

		if (checkHelp(HELP_SMPTE))
			draw = 1;
	}
}

void tp_ref_color_bars()
{
	int done = 0, draw = 1;
	picture refcolor_back;
	ushort pal_75pct[] = { _BLACK, IRE_75, YEL075, CYN075, GRN075, MAG075, RED075, BLU075, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK };

	while (!done)
	{
		if (draw)
		{
			gfxClear();

			if(vmode_snk)
			{
				pictureInit(&refcolor_back, &refcolor_304, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,1,pal_75pct);
			}
			else
			{
				pictureInit(&refcolor_back, &refcolor, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,1,pal_75pct);
			}
			draw = 0;
		}

		SCClose();
		waitVBlank();

		readController();

		if (BTTN_EXIT)
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

		if (BTTN_MAIN)
		{
			Ischeck = !Ischeck;
			if (!Ischeck) {
				pictureHide(&colorbleedcheck_back);
				pictureShow(&colorbleedbars_back);
			} else {
				pictureHide(&colorbleedbars_back);
				pictureShow(&colorbleedcheck_back);
			}
		}

		if (BTTN_EXIT)
			done = 1;

		if (checkHelp(HELP_BLEED))
			draw = 1;
	}
}

void tp_grid()
{
	int done = 0, draw = 1, updatepalette = 0, gray = 0;
	WORD color = _BLACK;
	scroller grid;

	color = _BLACK;
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
#ifndef __cd__
			if((isMVS && !AES_AS_MVS && !vmode_snk && !isMulti && first_grid))
			{
				draw_warning("Some later MVS systems can't\ndisplay the last column of\npixels when in full 320 mode.", index, palindex, 0);
				first_grid = 0;
				draw = 1;
			}
#else
			if(isCDZ && !vmode_snk && first_grid)	// Have only checked this variant, maybe all have this behaviour?
			{
				draw_warning("CDZ systems can't display\nthe last column of\npixels when in full 320 mode.", index, palindex, 0);
				first_grid = 0;
				draw = 1;
			}
#endif
		}

		SCClose();
		waitVBlank();

		// these is non buffered, execute after in hardware
		if (updatepalette)
		{
			if (!gray)
				color = _BLACK;
			else
				color = IRE_30;
			VRAM_PAL(16, 3) = color;
			VRAM_PAL(0, 2)  = color;	// SNK BIOS Border
			backgroundColor(color);
			updatepalette = 0;
		}

		readController();

		if (BTTN_MAIN)
		{
			gray = !gray;
			updatepalette = 1;
		}

		if (BTTN_EXIT)
			done = 1;

		if(checkHelp(HELP_GRID))
			draw = 1;
	}
	VRAM_PAL(0, 2) = _BLACK;	// restore SNK BIOS Border Color
}

void tp_monoscope()
{
	int done = 0, draw = 1, pattern = 0, gray = 0, updatepalette = 0, changepattern = 0;
	int bright[7] = { 31, 25, 20, 15, 10, 6, 2 }, color = _BLACK;
	scroller monoscope;

	while (!done)
	{
		if (draw)
		{
			int index = 1, palindex = 16;

			gfxClear();

			scrollerInit(&monoscope, &monoscopes, index, palindex, getHorScrollAspect(), PATTERN_SCROLL);
			palJobPut(palindex, monoscopes.palInfo->count, monoscopes.palInfo->data);
			index += SCROLLER_SIZE;
			palindex += monoscopes.palInfo->count;

			updatepalette = 1;
			changepattern = 1;
			draw = 0;

#ifndef __cd__
			if((isMVS && !AES_AS_MVS && !vmode_snk && !isMulti && first_grid))
			{
				draw_warning("Some later MVS systems can't\ndisplay the last column of\npixels when in full 320 mode.", index, palindex, 0);
				first_grid = 0;
				draw = 1;
			}
#else
			if(isCDZ && !vmode_snk && first_grid)	// Have only checked this variant, maybe all have this behaviour?
			{
				draw_warning("CDZ systems can't display\nthe last column of\npixels when in full 320 mode.", index, palindex, 0);
				first_grid = 0;
				draw = 1;
			}
#endif
		}
		
		SCClose();
		waitVBlank();

		// these are non buffered, execute after in hardware
		if (updatepalette)
		{
			// black _BLACK
			if (!gray)
				color = _BLACK;
			else
				color = PackColor(7, 7, 7, 0);
			VRAM_PAL(16, 3) = color;
			VRAM_PAL(0, 3)  = color;	// SNK BIOS Border
			backgroundColor(color);
			updatepalette = 0;
		}

		if(changepattern)
		{
			// white  0x7fff
			VRAM_PAL(16, 1) = PackColor(bright[pattern], bright[pattern], bright[pattern], 0);
			changepattern = 0;
		}

		readController();

		if (BTTN_MAIN)
		{
			pattern++;
			if (pattern > 6)
				pattern = 0;
			changepattern = 1;
		}

		if (BTTN_OPTION_1)
		{
			gray = !gray;
			updatepalette = 1;
		}

		if (BTTN_EXIT)
			done = 1;

		if (checkHelp(HELP_MONOSCOPE))
			draw = 1;
	}
	VRAM_PAL(0, 3) = _BLACK;	// SNK BIOS Border
}

void display_ramp_pal(ushort *palette, int x)
{
	int y = 2, pal = 0, i = 0;
	char buffer[5];

	for(pal = 0; pal < 4; pal++)
	{
		for(i = 1; i < 9; i++)
		{
			intToHex(palette[i+pal*16], buffer, 4);
			fixPrint(x, y++, fontColorRed, 3, buffer);
			if(y > 30)
			{
				x += 4;
				y = 3;
			}
		}
	}
}

void tp_gray_ramp()
{
	int done = 0, draw = 1, color = 0, pal = 0, i = 0;
	int version = 0, setpalettes = 0, text = 0, local_shadow = 0;
	int pal_top_light = 0, pal_top_dark = 0;
	int pal_bottom_light = 0, pal_bottom_dark = 0;
	picture ramp_top_light_back;
	picture ramp_top_dark_back;
	picture ramp_bottom_light_back;
	picture ramp_bottom_dark_back;
	ushort ramp_light_pal[64], ramp_dark_pal[64];
	ushort ramp_light_rev_pal[64], ramp_dark_rev_pal[64];

	// We only use the tiles and generate the palettes on the fly
	memset(ramp_light_pal, 0, sizeof(ushort)*64);
	memset(ramp_dark_pal, 0, sizeof(ushort)*64);
	memset(ramp_light_rev_pal, 0, sizeof(ushort)*64);
	memset(ramp_dark_rev_pal, 0, sizeof(ushort)*64);

	local_shadow = enable_shadow;

	for(pal = 0; pal < 4; pal++)
	{
		for(i = 1; i < 9; i++)
		{
			ramp_light_pal[i+pal*16] = PackColor(color, color, color, 0);
			ramp_dark_pal[i+pal*16] = PackColor(color, color, color, 1);
			ramp_light_rev_pal[i+pal*16] = PackColor(31-color, 31-color, 31-color, 0);
			ramp_dark_rev_pal[i+pal*16] = PackColor(31-color, 31-color, 31-color, 1);

			color++;
		}
	}

	if(!allowIRE107)
	{
		ramp_light_pal[8+3*16] = PackColor(0, 0, 0, 1);
		ramp_light_rev_pal[1] = PackColor(0, 0, 0, 1);
	}

	while (!done)
	{
		if (draw)
		{
			int sprindex = 1, palindex = 16;

			gfxClear();

			pal_top_dark = palindex;
			pictureInit(&ramp_top_dark_back, &grayramp, sprindex, palindex, 32, 0, FLIP_NONE);
			sprindex += getPicSprites(ramp_top_dark_back.info);
			palindex += grayramp.palInfo->count;

			pal_top_light = palindex;
			pictureInit(&ramp_top_light_back, &grayramp, sprindex, palindex, 36, 0, FLIP_NONE);
			sprindex += getPicSprites(ramp_top_light_back.info);
			palindex += grayramp.palInfo->count;
			
			pal_bottom_light = palindex;
			pictureInit(&ramp_bottom_light_back, &grayramp, sprindex, palindex, 32, 112, FLIP_NONE);
			sprindex += getPicSprites(ramp_top_dark_back.info);
			palindex += grayramp.palInfo->count;

			pal_bottom_dark = palindex;
			pictureInit(&ramp_bottom_dark_back, &grayramp, sprindex, palindex, 36, 112, FLIP_NONE);
			
			draw = 0;
			setpalettes = 1;
		}

		if(setpalettes)
		{
			switch(version)
			{
				case 0:
					palJobPut(pal_top_dark, grayramp.palInfo->count, ramp_dark_pal);
					palJobPut(pal_top_light, grayramp.palInfo->count, ramp_light_pal);
					palJobPut(pal_bottom_dark, grayramp.palInfo->count, ramp_dark_rev_pal);
					palJobPut(pal_bottom_light, grayramp.palInfo->count, ramp_light_rev_pal);
				break;

				case 1:
					palJobPut(pal_top_dark, grayramp.palInfo->count, ramp_dark_pal);
					palJobPut(pal_top_light, grayramp.palInfo->count, ramp_dark_pal);
					palJobPut(pal_bottom_dark, grayramp.palInfo->count, ramp_dark_rev_pal);
					palJobPut(pal_bottom_light, grayramp.palInfo->count, ramp_dark_rev_pal);
				break;

				case 2:
					palJobPut(pal_top_dark, grayramp.palInfo->count, ramp_light_pal);
					palJobPut(pal_top_light, grayramp.palInfo->count, ramp_light_pal);
					palJobPut(pal_bottom_dark, grayramp.palInfo->count, ramp_light_rev_pal);
					palJobPut(pal_bottom_light, grayramp.palInfo->count, ramp_light_rev_pal);
				break;
			}

			if (bkp_data.debug_dip1 & DP_DEBUG1)
			{
				display_ramp_pal(ramp_light_pal, 4);
				display_ramp_pal(ramp_dark_pal, 12);
				display_ramp_pal(ramp_light_rev_pal, 20);
				display_ramp_pal(ramp_dark_rev_pal, 28);
			}
			
			setpalettes = 0;
		}

		if(text)
		{
			text--;
			if(!text)
				suiteClearFixLayer();
		}

		SCClose();
		waitVBlank();

		readController();

		if (BTTN_MAIN)
		{
			version++;
			if(version > 2)
				version = 0;
			setpalettes = 1;

			switch(version)
			{
				case 0:
					fixPrint(25, 3, fontColorRed, 3, "Full ");
				break;

				case 1:
					fixPrint(25, 3, fontColorRed, 3, "Dark ");
				break;

				case 2:
					fixPrint(25, 3, fontColorRed, 3, "Light");
				break;
			}
			text = 60;
		}

		if (BTTN_EXIT)
			done = 1;

		if (BTTN_OPTION_1)
		{
			local_shadow = !local_shadow;
			if(local_shadow)
			{
				volMEMBYTE(REG_SHADOW) = 1;
				fixPrint(25, 4, fontColorRed, 3, "Darken");
			}
			else
			{
				volMEMBYTE(REG_NOSHADOW) = 1;
				fixPrint(25, 4, fontColorRed, 3, "Normal");
			}
			text = 60;
		}

		if (checkHelp(HELP_GRAY))
			draw = 1;
	}

	// Leave it as globally set
	if(enable_shadow)
		volMEMBYTE(REG_SHADOW) = 1;
	else
		volMEMBYTE(REG_NOSHADOW) = 1;
}

void tp_white_rgb()
{
	int done = 0, color = 1, draw = 1, editmode = 0, sel = 0;
	short r = 31, g = 31, b = 31, dark = 0, text = 0;
	WORD edit_color = WH_107;

	if(allowIRE107)
		edit_color = WH_107;
	else
	{
		edit_color = WH_100;
		dark = 1;
	}

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
					backgroundColor(_BLACK);
				break;

				case 3:		// Red
					backgroundColor(allowIRE107 ? RED107 : RED100);
				break;

				case 4:		// Green
					backgroundColor(allowIRE107 ? GRN107 : GRN100);
				break;

				case 5:		// Blue
					backgroundColor(allowIRE107 ? BLU107 : BLU100);
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
			if (PRESSED_RIGHT || BTTN_OPTION_1)
			{
				color++;
				draw = 1;
			}

			if (PRESSED_LEFT || BTTN_OPTION_2)
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
						sprintf(colorname, "%s", edit_color != WH_100 ? "Edit " : "White");
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

		if (BTTN_MAIN && color == 1 && !editmode)
			editmode = 1;

		if (BTTN_EXIT)
		{
			if(color == 1)
			{
				if (!editmode)
					done = 1;
				else
				{
					editmode = 0;
					suiteClearFixLayer();
				}
			}
			else
				done = 1;
		}

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

			if (BTTN_OPTION_1)
			{
				*edit = 0;
				hasedit = 1;
			}

			if (BTTN_OPTION_2)
			{
				*edit = 31;
				hasedit = 1;
			}

			if (BTTN_MAIN)
			{
				dark = !dark;
				hasedit = 1;
			}

			if (hasedit)
			{
				int warn = 0;

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
				if(!allowIRE107 && edit_color == WH_107)
				{
					edit_color = WH_100;
					warn = 1;
				}
				if(!allowIRE107 && edit_color == RED107)
				{
					edit_color = RED100;
					warn = 1;
				}
				if(!allowIRE107 && edit_color == GRN107)
				{
					edit_color = GRN100;
					warn = 1;
				}
				if(!allowIRE107 && edit_color == BLU107)
				{
					edit_color = BLU100;
					warn = 1;
				}

				if(warn)
				{
					dark = 1;
					fixPrint(5, 8, fontColorSolid, 4, "IRE limited to 100 in options");
					text = 60;
				}

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
	int done = 0, text = 0, draw = 1, changed = 0, irenum = 9;
	ushort irevals[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 107};
	ushort irecolor[] = {IRE_10, IRE_20, IRE_30, IRE_40, IRE_50, IRE_60, IRE_70, IRE_80, IRE_90, WH_100, WH_107};
	ushort pal_ire[] = { _BLACK, _BLACK, WH_100, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK, _BLACK };
	picture ire_back;

	while (!done)
	{
		if (draw)
		{
			gfxClear();

			pictureInit(&ire_back, &ire, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,1,pal_ire);
			draw = 0;
			changed = 1;
		}

		if (text)
		{
			text--;
			if (!text)
				suiteClearFixLayer();
		}

		if(changed)
		{
			VRAM_PAL(16, 2) = irecolor[irenum];
			fixPrintf1(30, 25, fontColorWhite, 3, "IRE %u ", irevals[irenum]);
			text = 60;
			changed = 0;
		}
		
		SCClose();
		waitVBlank();

		readController();

		if (BTTN_MAIN)
		{
			if (irenum != 0)
			{
				irenum--;
				changed = 1;
			}
		}

		if (BTTN_OPTION_1)
		{
			if (irenum != (allowIRE107 ? 10 : 9))
			{
				irenum++;
				changed = 1;
			}
		}

		if (checkHelp(HELP_IRE))
			draw = 1;

		if (BTTN_EXIT)
			done = 1;
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

		if (BTTN_MAIN)
		{
			Isbrick = !Isbrick;
			changed = 1;
		}

		if (BTTN_EXIT)
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

		if (BTTN_MAIN)
		{
			fast = !fast;
			scroll = 1;
		}

		if (BTTN_OPTION_1)
		{
			top_y = t_max;
			bottom_y = b_min;
			left_x = l_max;
			right_x = r_min;

			scroll = 1;
		}

		if (BTTN_EXIT)
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

		if (BTTN_MAIN)
		{
			pattern++;
			if (pattern > 3)
			{
				pattern = 1;
			}
			draw = 1;
		}

		if (BTTN_OPTION_1)
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

		if (BTTN_EXIT)
			done = 1;

		if (checkHelp(HELP_CONVERGENCE))
			draw = 1;
	}
}