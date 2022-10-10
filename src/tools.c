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

//#include <stddef.h>
#include <stdarg.h>
#include <DATlib.h>
#include <input.h>
#include "types.h"
#include "ng.h"
#include "string_ng.h"
#include "externs.h"
#include "tools.h"
#include "help.h"

u16 randbase;

void setRandomSeed(u16 seed)
{
	// xor it with a random value to avoid 0 value
	randbase = seed ^ 0xD94B;
}

u16 random()
{
	unsigned int fc = DAT_frameCounter;
	randbase ^= (randbase >> 1) ^ fc;
	randbase ^= (randbase << 1);

	return randbase;
}

int hexToDec(int hex)
{
	return hex-(hex/16)*6;
}

/*
NTSC AES

    24.167829MHz main clock / 4 = 6.041957MHz pixel clock
    6.041957MHz / 384 pixels per line = 15.734kHz horizontal rate
    15.734kHz / 264 lines = 59.599 frames/second

PAL AES

    24.167829MHz main clock / 4 = 6.041957MHz pixel clock
    6.041957MHz / 384 pixels per line = 15.734kHz horizontal rate
    15.734kHz / 312 lines = 50.429 frames/second

MVS

    24.000000MHz main clock / 4 = 6.000000MHz pixel clock
    6MHz / 384 pixels per line = 15.625kHz horizontal rate
    15.625kHz / 264 lines = 59.1856 frames/second

	Source: https://wiki.neogeodev.org/index.php?title=Framerate
*/

// We can't detect between 2 and 4 slot systems
void check_systype()
{
	BYTE reg = 0;

	isMVS = is4S = is6S = isMulti = hwChange = 0, vmode_snk = isPAL = 0;

	if (MEMBYTE(BIOS_MVS_FLAG) == SYSTEM_MVS)
	{
		isMVS = 1;
		reg = volMEMBYTE(REG_SYSTYPE);
		if (reg & MVS_MULTI)
		{
			isMulti = 1;
			reg = volMEMBYTE(REG_STATUS_A);
			if (reg & MVS_4_OR_6)
				is6S = 1;
			else
				is4S = 1;
		}
	}

	// Detect if MVS/AES IDs are in conflict
	reg = volMEMBYTE(REG_STATUS_B);
	if (reg & MVS_OR_AES && !isMVS)
		hwChange = 1;

	// Check is 304 mode is enabled, and follow BIOS resolution and rules
	// Also available under options, default is 304 mode to comply
	if (isMVS)
	{
		reg = volMEMBYTE(SOFT_DIP_4);
		if (reg)
			vmode_snk = 0;
		else
			vmode_snk = 1;
	} else {
		// read from memory card (TODO)
		vmode_snk = 1;
	}
	
	// NTSC at zero, but PAL apparently only works with the newer LPSC2 in AES
	// we need to test what happens on an AES with LPSC-A0 & LSPC2-A2 with a PAL or modded system
	// LSPC-A0 by NEC is the VDC part of the first generation chipset, see LSPC2-A2 for more details. (AES & MVS)
	// LSPC2-A2 by Fujitsu is the second generation Line SPrite Controller, it is only found in cartridge systems. (AES & MVS)
	// We also need to know what we'll do with PAL systems... since they are 240p and have different PAR (TODO)
	if (!isMVS)
	{
		reg = volMEMWORD(REG_LSPCMODE);
		if (reg & LPSC2_NTSC_PAL)
			isPAL = 1;
	}
}

// placed here temporarily
inline void suiteClearFixLayer()
{
	if (vmode_snk)
		BIOS_FIX_CLEAR;		// Call the SNK BIOS clear that puts the black Mask and reduces it to 304 visible pixels
	else
		clearFixLayer();
}

inline void gfxClear()
{
	clearSprites(1, MAX_SPRITES);
	suiteClearFixLayer();
}


void menu_options()
{
	int done = 0, curse = 1, cursemax = 2, redraw = 1, text = 0;

	while (!done)
	{
		int toggle = 0;

		if (redraw)
		{
			gfxClear();
			draw_background();
			redraw = 0;
		}

		SCClose();
		waitVBlank();

		readController();

		if (PRESSED_UP)		curse=curse>1?curse-1:cursemax;
		if (PRESSED_DOWN)	curse=curse<cursemax?curse+1:1;

		fixPrintf(14, 6, fontColorGreen, 3, "%s Options", isMVS ? "MVS" : "AES");
		fixPrintf(5, 14, curse == 1 ? fontColorRed : fontColorWhite, 3, "Horizontal Width:    %s", vmode_snk ? "BIOS 304" : "FULL 320");
		fixPrintf(5, 18, curse == 2 ? fontColorRed : fontColorWhite, 3, "Back to Main Menu");

		menu_footer();

		// Extra description
		if (curse == 1)
		{
			fixPrintf(4, 20, fontColorGreen, 3, "The 304 mode uses the BIOS mask");
			fixPrintf(4, 21, fontColorGreen, 3, "to hide 8 pixels on each side, ");
			fixPrintf(4, 22, fontColorGreen, 3, "as required by SNK in games.");
			if (!vmode_snk)
				fixPrintf(4, 23, fontColorRed, 3, "Some systems trim pixel col. 320");
			text = 1;
		} else {
			if (text)
			{
				int i = 0;

				for (i = 20; i < 25; i++)
					fixPrintf(4, i, fontColorGreen, 3, "                               ");
				text = 0;
			}
		}

		if (curse == 1 && (PRESSED_LEFT || PRESSED_RIGHT))
			toggle = 1;

		if (PRESSED_A || toggle)
		{
			gfxClear();
			switch (curse)
			{
				case 1:
					vmode_snk = !vmode_snk;
					suiteClearFixLayer();
				break;

				case 2:
					done = 1;
				break;
			}
			redraw = 1;
		}

		if (PRESSED_B)
			done = 1;

		if (checkHelp(HELP_GENERAL))
			redraw = 1;
	}
	return;
}

void draw_background_w_gil()
{
	int index = 1, palindex = 16;
	picture background;
	picture foreground;

	pictureInit(&background, &back, index, palindex, 0, 0,FLIP_NONE);
	palJobPut(palindex, back.palInfo->count,back.palInfo->data);
	index += background.info->stripSize*2;
	palindex += back.palInfo->count;

	pictureInit(&foreground, &gillian, index, palindex, 216, 70,FLIP_NONE);
	palJobPut(palindex, gillian.palInfo->count, gillian.palInfo->data);
}

void draw_background()
{
	picture background;

	pictureInit(&background, &back,1, 16, 0, 0,FLIP_NONE);
	palJobPut(16,back.palInfo->count,back.palInfo->data);
}

void menu_footer()
{
	fixPrintf(23, 26, fontColorWhite, 3, "%s %03dx224p", isPAL ? "PAL " : "NTSC", vmode_snk ? 304 : 320);
	if (isMVS)
	{
		fixPrint(23, 28, fontColorWhite, 3, isMVS ? "MVS" : "AES");
		if (is4S || is6S)
			fixPrint(26, 28, fontColorWhite, 3, is4S ? "2/4S" : "6S");
		else
			fixPrint(26, 28, fontColorWhite, 3, "1S");
		if (hwChange)
			fixPrint(19, 28, fontColorWhite, 3, "AES>");
	} else {
		fixPrint(27, 28, fontColorWhite, 3, "AES");
		if (hwChange)
			fixPrint(23, 28, fontColorWhite, 3, "MVS>");
	}

	if ((MEMBYTE(BIOS_COUNTRY_CODE) == SYSTEM_JAPAN))
	{
		fixPrint(32, 28, fontColorWhite, 3, "Japan");
	}
	else if ((MEMBYTE(BIOS_COUNTRY_CODE) == SYSTEM_USA))
	{
		fixPrint(34, 28, fontColorWhite, 3, "USA");
	}
	else if ((MEMBYTE(BIOS_COUNTRY_CODE) == SYSTEM_EUROPE))
	{
		fixPrint(31, 28, fontColorWhite, 3, "Europe");
	}

	if (isMVS && volMEMBYTE(SOFT_DIP_2))
	{
		int credits;
		
		credits = hexToDec(volMEMBYTE(BIOS_NM_CREDIT));
		fixPrintf(4, 26, fontColorWhite, 3, "CREDIT%c %02d", credits <= 1 ? ' ' : 'S', credits);  // credit counter
	}
}

inline void readController()
{
	p1  = volMEMBYTE(P1_CURRENT);
	p1e = volMEMBYTE(P1_EDGE);
	ps  = volMEMBYTE(PS_CURRENT);
	pse = volMEMBYTE(PS_EDGE);
}

inline void clearController()
{
	p1 = p2 = ps = pse = p1e = p2e = p1b = p2b = 0;
	waitVBlank();
}