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

BYTE p1,p2,ps,p1e,p2e;

static const ushort plugergb_pal[]= {
		0x8000, 0x8000, 0x7000, 0x7fff, 0x7bbb, 0x8888, 0x8444, 0xf000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,};
static const ushort plugentsc_pal[]= {
		0x8000, 0x8000, 0x8222, 0x7fff, 0x7bbb, 0x8888, 0x8444, 0xf111, 0x7000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,};

void tp_pluge()
{
	int done = 0, draw = 1, IsNTSC = 0, text = 0;
	picture plugergb_back;
	picture plugentsc_back;

	while (!done)
	{
		int palindex = 16, palcount = 1, sprindex = 1;

		if (draw)
		{
			gfxClear();

			pictureInit(&plugergb_back, &plugergb, sprindex, palindex, 0, 0,FLIP_NONE);
			palJobPut(palindex,palcount,plugergb_pal);

			pictureInit(&plugentsc_back, &plugentsc, sprindex, palindex, 0, 0,FLIP_NONE);
			palJobPut(palindex,palcount,plugentsc_pal);

			if (!IsNTSC){
				pictureHide(&plugentsc_back);
				pictureShow(&plugergb_back);
				palJobPut(palindex,palcount,plugergb_pal);
			} else {
				pictureHide(&plugergb_back);
				pictureShow(&plugentsc_back);
				palJobPut(palindex,palcount,plugentsc_pal);
			}

			draw = 0;
		}

		SCClose();
		waitVBlank();

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (text)
		{
			text--;
			if (!text)
				suiteClearFixLayer();
		}

		if (p1e & JOY_A)
		{
			IsNTSC = !IsNTSC;
			if (!IsNTSC){
				pictureHide(&plugentsc_back);
				pictureShow(&plugergb_back);
				palJobPut(palindex,palcount,plugergb_pal);
				fixPrint(24, 3, fontColorRed, 3, "RGB FULL RANGE");
			} else {
				pictureHide(&plugergb_back);
				pictureShow(&plugentsc_back);
				palJobPut(palindex,palcount,plugentsc_pal);
				fixPrint(24, 3, fontColorRed, 3, "NTSC 7.5 IRE  ");
			}
			text = 60;
			SCClose();
		}

		if (p1e & JOY_B || ps & P1_START)
		{
			done = 1;
		}

		if(checkHelp(HELP_PLUGE))
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

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_B || ps & P1_START)
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
	picture image1;
	picture image2;
	
	while (!done)
	{
		if (draw)
		{
			backgroundColor(0xfc1f);
			gfxClear();
			if (!Is75)
			{
				pictureInit(&image1, &colorebu, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,colorebu.palInfo->count,colorebu.palInfo->data);
			}
			else {
				pictureInit(&image2, &colorebu75, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,colorebu75.palInfo->count,colorebu75.palInfo->data);
			}
			draw = 0;
		}
		SCClose();
		waitVBlank();

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (text)
		{
			text--;
			if (!text)
				suiteClearFixLayer();
		}

		if (p1e & JOY_A)
		{
			Is75 = !Is75;
			if (!Is75)
			{
				pictureInit(&image1, &colorebu, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,colorebu.palInfo->count,colorebu.palInfo->data);
				fixPrint(32, 3, fontColorWhite, 3, "100%");
			}
			else {
				pictureInit(&image2, &colorebu75, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,colorebu75.palInfo->count,colorebu75.palInfo->data);
				fixPrint(32, 3, fontColorWhite, 3, " 75%");
			}
			text = 60;
			SCClose();
		}

		if (p1e & JOY_B || ps & P1_START)
		{
			done = 1;
		}

		if(checkHelp(HELP_601CB))
			draw = 1;
	}
}

void tp_smpte_color_bars()
{
	int done = 0, draw = 1, Is75 = 0, text = 0;
	picture image1;
	picture image2;

	while (!done)
	{
		if (draw)
		{
			backgroundColor(0xfc1f);
			gfxClear();

			if (!Is75)
			{
				pictureInit(&image1, &colorbarssmpte, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,colorbarssmpte.palInfo->count,colorbarssmpte.palInfo->data);
			}
			else {
				pictureInit(&image2, &colorbarssmpte75, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,colorbarssmpte75.palInfo->count,colorbarssmpte75.palInfo->data);
			}
			draw = 0;
		}
		SCClose();
		waitVBlank();

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (text)
		{
			text--;
			if (!text)
				suiteClearFixLayer();
		}

		if (p1e & JOY_A)
		{
			Is75 = !Is75;

			if (!Is75)
			{
				pictureInit(&image1, &colorbarssmpte, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,colorbarssmpte.palInfo->count,colorbarssmpte.palInfo->data);
				fixPrint(32, 3, fontColorWhite, 3, "100%");
			}
			else {
				pictureInit(&image2, &colorbarssmpte75, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,colorbarssmpte75.palInfo->count,colorbarssmpte75.palInfo->data);
				fixPrint(32, 3, fontColorWhite, 3, " 75%");
			}
			text = 60;
			SCClose();
		}

		if (p1e & JOY_B || ps & P1_START)
		{
			done = 1;
		}

		if(checkHelp(HELP_SMPTE))
			draw = 1;
	}
}

void tp_ref_color_bars()
{
	int done = 0, draw = 1;
	picture image;

	while (!done)
	{
		if (draw)
		{
			gfxClear();

			pictureInit(&image, &refcolor, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,refcolor.palInfo->count,refcolor.palInfo->data);
			draw = 0;
		}
		SCClose();
		waitVBlank();

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_B || ps & P1_START)
		{
			done = 1;
		}

		if(checkHelp(HELP_601CB))
			draw = 1;
	}
}

void tp_color_bleed_check()
{
	int done = 0, draw = 1, Ischeck = 0;
	picture image1;
	picture image2;

	while (!done)
	{
		if (draw)
		{
			gfxClear();

			if (!Ischeck)
			{
				pictureInit(&image1, &colorbleedbars, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,colorbleedbars.palInfo->count,colorbleedbars.palInfo->data);
			}
			else {
				pictureInit(&image2, &colorbleedcheck, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,colorbleedcheck.palInfo->count,colorbleedcheck.palInfo->data);
			}
			draw = 0;
		}
		SCClose();
		waitVBlank();

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_A)
		{
			Ischeck = !Ischeck;
			if (!Ischeck)
			{
				pictureInit(&image1, &colorbleedbars, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,colorbleedbars.palInfo->count,colorbleedbars.palInfo->data);
			}
			else {
				pictureInit(&image2, &colorbleedcheck, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,colorbleedcheck.palInfo->count,colorbleedcheck.palInfo->data);
			}
			SCClose();
		}

		if (p1e & JOY_B || ps & P1_START)
		{
			done = 1;
		}

		if(checkHelp(HELP_BLEED))
			draw = 1;
	}
}

void tp_grid()
{
	int done = 0, draw = 1, gray = 0;
	picture image;

	while (!done)
	{
		if (draw)
		{
			gfxClear();

			pictureInit(&image, &grid, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,grid.palInfo->count,grid.palInfo->data);
			draw = 0;
		}

		if (!gray)
		{
			volMEMWORD(0x402204) = 0x8000;
		} else {
			volMEMWORD(0x402204) = 0x7777;
		}

		SCClose();
		waitVBlank();

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_A)
		{
			gray = !gray;
		}

		if (p1e & JOY_B || ps & P1_START)
		{
			done = 1;
			return;
		}

		if(checkHelp(HELP_GRID))
			draw = 1;
	}
}

void tp_monoscope()
{
	int done = 0, draw = 1, pattern = 1, gray = 0;
	picture image;

	while (!done)
	{
		if (draw)
		{
			gfxClear();

			pictureInit(&image, &monoscope, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,monoscope.palInfo->count,monoscope.palInfo->data);
			draw = 0;
		}

		if (!gray)
		{
			volMEMWORD(0x402206) = 0x8000;
		}else{
			volMEMWORD(0x402206) = 0x7777;
		}

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

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_A)
		{
			pattern++;
			if (pattern > 7)
				pattern = 1;
		}

		if (p1e & JOY_B)
		{
			gray = !gray;
		}

		if (ps & P1_START)
		{
			done = 1;
		}

		if(checkHelp(HELP_MONOSCOPE))
			draw = 1;
	}
}

void tp_gray_ramp()
{
	int done = 0, draw = 1;
	picture image;

	while (!done)
	{
		if (draw)
		{
			gfxClear();

			pictureInit(&image, &grayramp, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,grayramp.palInfo->count,grayramp.palInfo->data);
			draw = 0;
		}
		SCClose();
		waitVBlank();

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_B || ps & P1_START)
		{
			done = 1;
		}

		if(checkHelp(HELP_GRAY))
			draw = 1;
	}
}

void tp_white_rgb()
{
	int done = 0, color = 1, draw = 1;

	while (!done)
	{
		if (draw)
		{
			gfxClear();

			switch (color)
			{
				case 1:
					backgroundColor(0xFFFF);
				break;

				case 2:
					backgroundColor(0x8000);
				break;

				case 3:
					backgroundColor(0x8F00);
				break;

				case 4:
					backgroundColor(0x80F0);
				break;

				case 5:
					backgroundColor(0x800F);
				break;
			}
			draw = 0;
		}
		SCClose();
		waitVBlank();

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_A)
		{
			color++;
			draw = 1;
		}

		if (p1e & JOY_B)
		{
			color--;
			draw = 1;
		}

		if (ps & P1_START)
		{
			done = 1;
		}

		if(checkHelp(HELP_WHITE))
			draw = 1;

		if (color > 5)
			color = 1;

		if (color < 1)
			color = 5;
	}
}

void tp_100_ire()
{
	int done = 0, text = 0, draw = 1;
	ushort irevals[] = {13,25,41,53,66,82,94};
	int irenum = 6;
	picture image;

	static const ushort ire_94[]= {
		0x8000, 0x8000, 0x2fef, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,};
	static const ushort ire_82[]= {
		0x8000, 0x8000, 0x2ddd, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,};
	static const ushort ire_66[]= {
		0x8000, 0x8000, 0x2bbb, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,};
	static const ushort ire_53[]= {
		0x8000, 0x8000, 0x2999, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,};
	static const ushort ire_41[]= {
		0x8000, 0x8000, 0x2777, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,};
	static const ushort ire_25[]= {
		0x8000, 0x8000, 0x2555, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,};
	static const ushort ire_13[]= {
		0x8000, 0x8000, 0x2222, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,};


	while (!done)
	{
		if (draw)
		{
			gfxClear();

			pictureInit(&image, &ire, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,ire.palInfo->count,ire.palInfo->data);
			draw = 0;
		}

		switch (irenum)
		{
			case 0:
				palJobPut(16,1,ire_13);  // 13
			break;

			case 1:
				palJobPut(16,1,ire_25);  // 25
			break;

			case 2:
				palJobPut(16,1,ire_41);  // 41
			break;

			case 3:
				palJobPut(16,1,ire_53);  // 53
			break;

			case 4:
				palJobPut(16,1,ire_66);  // 66
			break;

			case 5:
				palJobPut(16,1,ire_82);  // 82
			break;

			case 6:
				palJobPut(16,1,ire_94);  // 94
			break;
		}
		SCClose();
		waitVBlank();

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_A)
		{
			if (irenum != 0)
				irenum--;
			fixPrintf1(32, 25, fontColorWhite, 3, "IRE:%u", irevals[irenum]);
			text = 60;
		}

		if (p1e & JOY_B)
		{
			if (irenum != 6)
				irenum++;
			fixPrintf1(32, 25, fontColorWhite, 3, "IRE:%u", irevals[irenum]);
			text = 60;
		}

		if (ps & P1_START)
		{
			done = 1;
		}

		if(checkHelp(HELP_IRE))
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
	picture image1;
	picture image2;

	while (!done)
	{
		if (draw)
		{
			gfxClear();

			if (!Isbrick)
			{
				pictureInit(&image1, &sharp, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,sharp.palInfo->count,sharp.palInfo->data);
			}
			else {
				pictureInit(&image2, &sharpbrick, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,sharpbrick.palInfo->count,sharpbrick.palInfo->data);
			}
			draw = 0;
		}
		SCClose();
		waitVBlank();

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_A)
		{
			Isbrick = !Isbrick;

			if (!Isbrick)
			{
				pictureInit(&image1, &sharp, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,sharp.palInfo->count,sharp.palInfo->data);
			}
			else {
				pictureInit(&image2, &sharpbrick, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,sharpbrick.palInfo->count,sharpbrick.palInfo->data);
			}
			SCClose();
		}

		if (p1e & JOY_B || ps & P1_START)
		{
			done = 1;
		}

		if(checkHelp(HELP_SHARPNESS))
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

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_B)
		{
			done = 1;
			return;
		}
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

		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_A)
		{
			pattern++;
			if (pattern > 3)
			{
				pattern = 1;
			}
			draw = 1;
		}

		if (p1e & JOY_B)
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

		if (ps & P1_START)
		{
			done = 1;
		}

		if(checkHelp(HELP_CONVERGENCE))
			draw = 1;
	}
}