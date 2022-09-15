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

BYTE p1,p2,ps,p1e,p2e;

void tp_pluge()
{
	int done = 0, draw = 1, IsNTSC = 0, text = 0;
	picture image1;
	picture image2;

	static const ushort plugergb_pal[]= {
		0x8000, 0x8000, 0x7000, 0x7fff, 0x7bbb, 0x8888, 0x8444, 0xf000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,};
	static const ushort plugentsc_pal[]= {
		0x8000, 0x8000, 0x8222, 0x7fff, 0x7bbb, 0x8888, 0x8444, 0xf111, 0x7000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,};

	clearFixLayer();
	clearSprites(1, 22);

	while (!done)
	{
		SCClose();
		waitVBlank();

		if (draw)
		{
			if (!IsNTSC)
			{	pictureInit(&image1, &plugergb, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,1,plugergb_pal);
				SCClose();
			}
			else {
				pictureInit(&image2, &plugentsc, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,1,plugentsc_pal);
				SCClose();
			}
			draw = 0;
		}

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (text)
		{
			text--;
			if (!text)
			{
				clearFixLayer();
			}
		}

		if (p1e & JOY_A)
		{
			IsNTSC = !IsNTSC;

			if (!IsNTSC)
			{
				pictureInit(&image1, &plugergb, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,1,plugergb_pal);
				SCClose();
				fixPrint(24, 3, 1, 3, "RGB FULL RANGE");
			}
			else {
				pictureInit(&image2, &plugentsc, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,1,plugentsc_pal);
				SCClose();
				fixPrint(24, 3, 1, 3, "NTSC 7.5 IRE  ");
			}
			text = 60;
		}

		if (p1e & JOY_B)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		if (ps & P1_START)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_PLUGE);
			draw = 1;
		}

	}

}

void tp_colorchart()
{
	int done = 0, draw = 1;
	picture image;

	clearFixLayer();
	clearSprites(1, 22);

	while (!done)
	{
		SCClose();
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (draw)
		{
			pictureInit(&image, &colorchart, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,colorchart.palInfo->count,colorchart.palInfo->data);
			draw = 0;
		}

		if (p1e & JOY_B)
		{
			done = 1;
			return;
		}

		if (ps & P1_START)
		{
			done = 1;
			return;
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_COLORS);
			draw = 1;
		}
	}
}

void tp_colorbars()
{
	int done = 0, draw = 1, Is75 = 0, text = 0;
	picture image1;
	picture image2;

	clearFixLayer();
	clearSprites(1, 22);

	while (!done)
	{
		SCClose();
		waitVBlank();

		if (draw)
		{
			if (!Is75)
			{
				pictureInit(&image1, &colorebu, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,colorebu.palInfo->count,colorebu.palInfo->data);
				SCClose();
			}
			else {
				pictureInit(&image2, &colorebu75, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,colorebu75.palInfo->count,colorebu75.palInfo->data);
				SCClose();
			}
			draw = 0;
		}

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (text)
		{
			text--;
			if (!text)
			{
				clearFixLayer();
			}
		}

		if (p1e & JOY_A)
		{
			Is75 = !Is75;

			if (!Is75)
			{
				pictureInit(&image1, &colorebu, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,colorebu.palInfo->count,colorebu.palInfo->data);
				SCClose();
				fixPrint(32, 3, 0, 3, "100%");
			}
			else {
				pictureInit(&image2, &colorebu75, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,colorebu75.palInfo->count,colorebu75.palInfo->data);
				SCClose();
				fixPrint(32, 3, 0, 3, " 75%");
			}
			text = 60;
		}

		if (p1e & JOY_B)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		if (ps & P1_START)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_601CB);
			draw = 1;
		}
	}
}

void tp_smpte_color_bars()
{
	int done = 0, draw = 1, Is75 = 0, text = 0;
	picture image1;
	picture image2;

	backgroundColor(0xfc1f);
	clearFixLayer();
	clearSprites(1, 22);

	while (!done)
	{
		if (draw)
		{
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

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (text)
		{
			text--;
			if (!text)
			{
				clearFixLayer();
			}
		}

		if (p1e & JOY_A)
		{
			Is75 = !Is75;

			if (!Is75)
			{
				pictureInit(&image1, &colorbarssmpte, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,colorbarssmpte.palInfo->count,colorbarssmpte.palInfo->data);
				fixPrint(32, 3, 0, 3, "100%");
			}
			else {
				pictureInit(&image2, &colorbarssmpte75, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,colorbarssmpte75.palInfo->count,colorbarssmpte75.palInfo->data);
				fixPrint(32, 3, 0, 3, " 75%");
			}
			text = 60;
		}

		if (p1e & JOY_B)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		if (ps & P1_START)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_SMPTE);
			draw = 1;
		}
	}
}

void tp_ref_color_bars()
{
	int done = 0, draw = 1;
	picture image;

	clearFixLayer();
	clearSprites(1, 22);

	while (!done)
	{
		SCClose();
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (draw)
		{
			pictureInit(&image, &refcolor, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,refcolor.palInfo->count,refcolor.palInfo->data);
			draw = 0;
		}

		if (p1e & JOY_B)
		{
			done = 1;
			return;
		}

		if (ps & P1_START)
		{
			done = 1;
			return;
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_601CB);
			draw = 1;
		}
	}
}

void tp_color_bleed_check()
{
	int done = 0, draw = 1, Ischeck = 0;
	picture image1;
	picture image2;

	clearFixLayer();
	clearSprites(1, 22);

	while (!done)
	{
		SCClose();
		waitVBlank();

		if (draw)
		{
			if (!Ischeck)
			{
				pictureInit(&image1, &colorbleedbars, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,colorbleedbars.palInfo->count,colorbleedbars.palInfo->data);
				SCClose();
			}
			else {
				pictureInit(&image2, &colorbleedcheck, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,colorbleedcheck.palInfo->count,colorbleedcheck.palInfo->data);
				SCClose();
			}
			draw = 0;
		}

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_A)
		{
			Ischeck = !Ischeck;

			if (!Ischeck)
			{
				pictureInit(&image1, &colorbleedbars, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,colorbleedbars.palInfo->count,colorbleedbars.palInfo->data);
				SCClose();
			}
			else {
				pictureInit(&image2, &colorbleedcheck, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,colorbleedcheck.palInfo->count,colorbleedcheck.palInfo->data);
				SCClose();
			}
		}

		if (p1e & JOY_B)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		if (ps & P1_START)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_BLEED);
			draw = 1;
		}
	}
}

void tp_grid()
{
	int done = 0, draw = 1, Isgray = 0;
	picture image1;
	picture image2;

	clearFixLayer();
	clearSprites(1, 22);

	while (!done)
	{
		SCClose();
		waitVBlank();

		if (draw)
		{
			if (!Isgray)
			{
				pictureInit(&image1, &grid, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,grid.palInfo->count,grid.palInfo->data);
				SCClose();
			}
			else {
				pictureInit(&image2, &grid, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,grid.palInfo->count,grid.palInfo->data);
				SCClose();
			}
			draw = 0;
		}

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_A)
		{
			Isgray = !Isgray;

			if (!Isgray)
			{
				pictureInit(&image1, &grid, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,grid.palInfo->count,grid.palInfo->data);
				SCClose();
			}
			else {
				pictureInit(&image2, &grid, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,grid.palInfo->count,grid.palInfo->data);
				SCClose();
			}
		}

		if (p1e & JOY_B)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		if (ps & P1_START)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_GRID);
			draw = 1;
		}
	}
}

void tp_monoscope()
{
	int done = 0, draw = 1;
	picture image;

	clearFixLayer();
	clearSprites(1, 22);

	while (!done)
	{
		SCClose();
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (draw)
		{
			pictureInit(&image, &monoscope, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,monoscope.palInfo->count,monoscope.palInfo->data);
			draw = 0;
		}

		if (p1e & JOY_B)
		{
			done = 1;
			return;
		}

		if (ps & P1_START)
		{
			done = 1;
			return;
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_MONOSCOPE);
			draw = 1;
		}
	}
}

void tp_gray_ramp()
{
	int done = 0, draw = 1;
	picture image;

	clearFixLayer();
	clearSprites(1, 22);

	while (!done)
	{
		SCClose();
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (draw)
		{
			pictureInit(&image, &grayramp, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,grayramp.palInfo->count,grayramp.palInfo->data);
			draw = 0;
		}

		if (p1e & JOY_B)
		{
			done = 1;
			return;
		}

		if (ps & P1_START)
		{
			done = 1;
			return;
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_GRAY);
			draw = 1;
		}
	}
}

void tp_white_rgb()
{
	int done = 0, color = 1;

	clearFixLayer();
	clearSprites(1, 1);
	clearSprites(1, 22);

	while (!done)
	{
		SCClose();
		waitVBlank();

		p1  = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		switch (color)
		{
			case 1:
				backgroundColor(0x7FFF);
			break;

			case 2:
				backgroundColor(0x0000);
			break;

			case 3:
				backgroundColor(0x0F00);
			break;

			case 4:
				backgroundColor(0x00F0);
			break;

			case 5:
				backgroundColor(0x000F);
			break;
		}

		if (p1e & JOY_A)
		{
			color++;
		}

		if (p1e & JOY_B)
		{
			color--;
		}

		if (ps & P1_START)
		{
			done = 1;
			return;
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_WHITE);
		}

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


	clearFixLayer();
	clearSprites(1, 22);

	while (!done)
	{
		SCClose();
		waitVBlank();

		if (draw)
		{
			pictureInit(&image, &ire, 1, 16, 0, 0,FLIP_NONE);
			palJobPut(16,ire.palInfo->count,ire.palInfo->data);
			draw = 0;
		}

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		switch (irenum)
		{
			case 0:
				palJobPut(16,1,ire_13);     // 13
			break;

			case 1:
				palJobPut(16,1,ire_25);     // 25
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

		if (p1e & JOY_A)
		{
			if (irenum != 0)
				irenum--;
			fixPrintf1(32, 25, 0, 3, "IRE:%2d", irevals[irenum]);
			text = 60;
		}

		if (p1e & JOY_B)
		{
			if (irenum != 6)
				irenum++;
			fixPrintf1(32, 25, 0, 3, "IRE:%2d", irevals[irenum]);
			text = 60;
		}

		if (ps & P1_START)
		{
			done = 1;
			return;
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_IRE);
		}

		if (text)
		{
			text--;
			if (!text)
			{
				clearFixLayer();
			}
		}

	}

}

void tp_sharpness()
{
	int done = 0, draw = 1, Isbrick = 0;
	picture image1;
	picture image2;

	clearFixLayer();
	clearSprites(1, 22);

	while (!done)
	{
		SCClose();
		waitVBlank();

		if (draw)
		{
			if (!Isbrick)
			{
				pictureInit(&image1, &sharp, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,sharp.palInfo->count,sharp.palInfo->data);
				SCClose();
			}
			else {
				pictureInit(&image2, &sharpbrick, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,sharpbrick.palInfo->count,sharpbrick.palInfo->data);
				SCClose();
			}
			draw = 0;
		}

		p1  = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (p1e & JOY_A)
		{
			Isbrick = !Isbrick;

			if (!Isbrick)
			{
				pictureInit(&image1, &sharp, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,sharp.palInfo->count,sharp.palInfo->data);
				SCClose();
			}
			else {
				pictureInit(&image2, &sharpbrick, 1, 16, 0, 0,FLIP_NONE);
				palJobPut(16,sharpbrick.palInfo->count,sharpbrick.palInfo->data);
				SCClose();
			}
		}

		if (p1e & JOY_B)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		if (ps & P1_START)
		{
			done = 1;
			clearFixLayer();
			return;
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_SHARPNESS);
			draw = 1;
		}
	}
}

void tp_overscan()
{
	int done = 0;
	picture image;

	clearFixLayer();
	clearSprites(1, 22);

	pictureInit(&image, &colorchart, 1, 16, 0, 0,FLIP_NONE);
	palJobPut(16,colorchart.palInfo->count,colorchart.palInfo->data);

	while (!done)
	{
		SCClose();
		waitVBlank();

		p1 = volMEMBYTE(P1_CURRENT);
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
	int done = 0, pattern = 1;
	picture image;

	clearFixLayer();
	clearSprites(1, 22);

	while (!done)
	{
		SCClose();
		waitVBlank();

		p1  = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		ps  = volMEMBYTE(PS_CURRENT);

		if (ps & P1_START)
		{
			done = 1;
			return;
		}

		if (p1e & JOY_A)
		{
			pattern++;
			if (pattern > 3)
			{
				pattern = 1;
			}
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
		}

		if (ps & P1_SELECT)
		{
			DrawHelp(HELP_CONVERGENCE);
			//draw = 1;
		}

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

	}

}