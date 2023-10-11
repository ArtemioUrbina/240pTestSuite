/* 
 * 240p Test Suite
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

#include "genesis.h"
#include "res.h"
#include "help.h"
#include "myvdp.h"
#include "main.h"

void ChangeResolution(u16 *y)
{
	(*y)++;
	VDP_drawTextBG(APLAN, "Resolution can be changed by", TILE_ATTR(PAL0, 0, 0, 0), 4, (*y)++);
	VDP_drawTextBG(APLAN, "pressing 'C'.", TILE_ATTR(PAL0, 0, 0, 0), 4, (*y)++);
}

void DrawHelp(int option)
{
	u16 exit = 0, redraw = 1, totalpages = 1, page = 1, ind = 0;
	u16 buttons, oldButtons = 0xffff, pressedButtons;

	FadeAndCleanUp();
	
	ind = DrawMainBG();
	
	VDP_setPalette(PAL3, gillian_pal);
	switch (option)
	{
	case HELP_GENERAL:
	case HELP_STRIPES:
	case HELP_LED:
	case HELP_LAG:
	case HELP_HSCROLL:
	case HELP_MEMVIEW:
	case HELP_MANUALLAG:
		totalpages = 2;
		break;
	case HELP_PLUGE:
	case HELP_MONOSCOPE:
		totalpages = 3;
		break;
	}
	
	while(!exit)
	{
		if(redraw)
		{
			u16 y = 7;

			VDP_Start();
			VDP_clearTileMapRect(APLAN, 0, 0, 320 / 8, 224 / 8);

			switch (option)
			{
			case HELP_GENERAL:
				switch (page)
				{
				case 1:
					VDP_drawTextBG(APLAN, "HELP (1/2)", TILE_ATTR(PAL1, 0, 0, 0), 14, 4);
					VDP_drawTextBG(APLAN, "The 240p Test Suite was designed", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "with two goals in mind:", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					y++;
					VDP_drawTextBG(APLAN, "1) Evaluate 240p signals on TV", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "sets and video procesors; and", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					y++;
					VDP_drawTextBG(APLAN, "2) Provide calibration patterns", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "from a game console to help in", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "properly calibrating the display", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "black, white and color levels.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					y++;
					if(joytype != JOY_TYPE_PAD6)
					{
						VDP_drawTextBG(APLAN, "Help is available everywhere", TILE_ATTR(PAL1, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "with DOWN+START. Video Options", TILE_ATTR(PAL1, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "can be changed with UP+START.", TILE_ATTR(PAL1, 0, 0, 0), 4, y);
					}
					else
					{
						VDP_drawTextBG(APLAN, "Help can be shown with 'Z' in", TILE_ATTR(PAL1, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "any test. Video Options can be", TILE_ATTR(PAL1, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "changed with 'Y'.", TILE_ATTR(PAL1, 0, 0, 0), 4, y);
					}
					VDP_drawTextBG(APLAN, "(cont...)", TILE_ATTR(PAL0, 0, 0, 0), 26, 22);
					break;
				case 2:
					{
						VDP_drawTextBG(APLAN, "HELP (2/2)", TILE_ATTR(PAL1, 0, 0, 0), 14, 4);
						VDP_drawTextBG(APLAN, "The Genesis/MegaDrive can output", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "224 active video lines.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "In PAL consoles, it can display", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "either 224 or 240 lines.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);					
						y++;
						VDP_drawTextBG(APLAN, "Visit:", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "http://junkerhq.net/240p", TILE_ATTR(PAL1, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "for more information.", TILE_ATTR(PAL0, 0, 0, 0), 4, y);
						
						VDP_loadTileData(barcode_tiles, ind, sizeof(barcode_tiles) / 32, USE_DMA);
						VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL3, 0, 0, 0) + ind, 256/8, 130/8, 32/8, 32 / 8);
					}
					break;
				}
				break;
			case HELP_PLUGE:
				switch (page)
				{
				case 1:
					VDP_drawTextBG(APLAN, "PLUGE (1/3)", TILE_ATTR(PAL1, 0, 0, 0), 13, 4);
					VDP_drawTextBG(APLAN, "NTSC levels require black to be", TILE_ATTR(PAL1, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "at 7.5 IRE for video. This HW", TILE_ATTR(PAL1, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "lowest is 6 IRE (6%), so using", TILE_ATTR(PAL1, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "this value for general 240p use", TILE_ATTR(PAL1, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "is not recommended.", TILE_ATTR(PAL1, 0, 0, 0), 4, y++);
					y++;
					VDP_drawTextBG(APLAN, "Of course using it as reference", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "will work perfectly for games", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "in this platform.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					y++;
					VDP_drawTextBG(APLAN, "In PAL - and console gaming in", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "general - it is adviced to use", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "a value of 2 IRE as black.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "(cont...)", TILE_ATTR(PAL0, 0, 0, 0), 26, 22);
					break;
				case 2:
					VDP_drawTextBG(APLAN, "PLUGE (2/3)", TILE_ATTR(PAL1, 0, 0, 0), 13, 4);
					VDP_drawTextBG(APLAN, "The PLUGE pattern is used to", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "help adjust the black level to", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "a correct value.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					y++;
					VDP_drawTextBG(APLAN, "The inner bars on the sides are", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "black at 6%, the outer at 12%.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "If these bars are not visible,", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "adjust the \"brightness\" control", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "until they are.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					y++;
					VDP_drawTextBG(APLAN, "You should lower it until they", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "are not visible, and raise it", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "until they show.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "(cont...)", TILE_ATTR(PAL0, 0, 0, 0), 26, 22);
					break;
				case 3:
					VDP_drawTextBG(APLAN, "PLUGE (3/3)", TILE_ATTR(PAL1, 0, 0, 0), 13, 4);
					VDP_drawTextBG(APLAN, "You can change to a contrast", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "test with C.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					y++;
					VDP_drawTextBG(APLAN, "Within it A button changes", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "palettes between the original,", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "highest and lowest values the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "hardware can display.", TILE_ATTR(PAL0, 0, 0, 0), 4, y);
					break;
				}
				break;
			case HELP_COLORS:
				VDP_drawTextBG(APLAN, "COLOR BARS", TILE_ATTR(PAL1, 0, 0, 0), 15, 4);
				VDP_drawTextBG(APLAN, "This pattern allows you to", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "calibrate each color: Red, Green", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "and Blue; as well as white.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "Adjust the white level first,", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "using the \"Contrast\" control", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "on your TV set. Raise it until", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "you cannot distinguish between", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "the blocks under \"C\" and \"E\",", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "and lower it slowly until you", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "can clearly tell them appart.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "Do the same for each color.", TILE_ATTR(PAL0, 0, 0, 0), 4, y);
				break;
			case HELP_GRID:
				VDP_drawTextBG(APLAN, "GRID", TILE_ATTR(PAL1, 0, 0, 0), 18, 4);
				if(enable_PAL240)
					VDP_drawTextBG(APLAN, "This grid uses the full 320x240", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				else
					VDP_drawTextBG(APLAN, "This grid uses the full 320x224", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				if(IsPALVDP)
					VDP_drawTextBG(APLAN, "PAL resolution.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				else
					VDP_drawTextBG(APLAN, "resolution.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "You can use it to verify that", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "all the visible area is being", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "displayed, and that there is no", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "distortion present.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "The full active video signal can", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "be filled with gray by pressing", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "the 'A' button.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				ChangeResolution(&y);
				break;
			case HELP_MONOSCOPE:
				switch (page)
				{
					case 1:
						VDP_drawTextBG(APLAN, "MONOSCOPE (1/3)", TILE_ATTR(PAL1, 0, 0, 0), 12, 4);
						VDP_drawTextBG(APLAN, "This pattern contains elements", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "to calibrate multiple aspects", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "of a CRT.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						y++;
						VDP_drawTextBG(APLAN, "Read your monitor's service", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "manual to learn how, and use", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "'A' button to change IRE.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						y++;
						VDP_drawTextBG(APLAN, "Brightness adjustment: Adjust", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "convergence at low brightness", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "(13/25 IRE). An overly bright", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "pattern can mask convergence", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "issues.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						y++;
						VDP_drawTextBG(APLAN, "(cont...)", TILE_ATTR(PAL0, 0, 0, 0), 26, 22);
					break;
					case 2:
						VDP_drawTextBG(APLAN, "MONOSCOPE (2/3)", TILE_ATTR(PAL1, 0, 0, 0), 12, 4);
						VDP_drawTextBG(APLAN, "Convergence: Use the center", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "crosshair to check static", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "(center of screen) convergence.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "Use the patterns at the sides", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "to check dynamic (edge)", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "convergence.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						y++;
						VDP_drawTextBG(APLAN, "Corners: After setting center", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "and edge convergence, use", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "magnets to adjust corner purity", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "and geometry.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						y+=2;
						VDP_drawTextBG(APLAN, "(cont...)", TILE_ATTR(PAL0, 0, 0, 0), 26, 22);
					break;
					case 3:
						VDP_drawTextBG(APLAN, "MONOSCOPE (3/3)", TILE_ATTR(PAL1, 0, 0, 0), 12, 4);
						VDP_drawTextBG(APLAN, "Size and aspect ratio: If", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "vertical and horizontal size are", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "correct, the red squares in the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "pattern will be perfect squares.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "After setting H size, use a tape", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "measure to adjust V size to match", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "it.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						y++;
						VDP_drawTextBG(APLAN, "Linearity: The squares in each", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "corner should get you started.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "Confirm your adjustment using", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						VDP_drawTextBG(APLAN, "the scroll tests.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
						y++;
						VDP_drawTextBG(APLAN, "Designed by Keith Raney.", TILE_ATTR(PAL0, 0, 0, 0), 4, y);
					break;
				}
				break;
			case HELP_BLEED:
				VDP_drawTextBG(APLAN, "COLOR BLEED", TILE_ATTR(PAL1, 0, 0, 0), 14, 4);
				VDP_drawTextBG(APLAN, "This pattern helps diagnose", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "color bleed caused by", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "unneeded color upsampling.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "You can toggle between", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "vertical bars and", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "checkerboard with 'A'.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				
				ChangeResolution(&y);
				break;
			case HELP_IRE:
				VDP_drawTextBG(APLAN, "100 IRE", TILE_ATTR(PAL1, 0, 0, 0), 14, 4);
				VDP_drawTextBG(APLAN, "You can vary IRE intensity", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "with A and B. Values are:", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "13, 25, 41, 53, 66, 82, 94", TILE_ATTR(PAL0, 0, 0, 0), 4, y);
				break;
			case HELP_601CB:
				VDP_drawTextBG(APLAN, "601 COLORBARS", TILE_ATTR(PAL1, 0, 0, 0), 14, 4);
				VDP_drawTextBG(APLAN, "You can use color filters or the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "blue only option in your display", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "to confirm color balance.", TILE_ATTR(PAL0, 0, 0, 0), 4, y);
				break;
			case HELP_SHARPNESS:
				VDP_drawTextBG(APLAN, "SHARPNESS", TILE_ATTR(PAL1, 0, 0, 0), 16, 4);
				VDP_drawTextBG(APLAN, "You should set the sharpness of", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "your CRT to a value that shows", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "clean black and gray transitions", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "with no white ghosting between.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "Pressing B toggles a diagonal", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "brick tileset.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				ChangeResolution(&y);
				break;
			case HELP_OVERSCAN:
				VDP_drawTextBG(APLAN, "OVERSCAN", TILE_ATTR(PAL1, 0, 0, 0), 16, 4);
				VDP_drawTextBG(APLAN, "With this pattern you can", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "interactively find out the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "overscan in pixels of each edge", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "in a display.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "Use left and right to increment", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "the overscan until you see the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "white border, then go back one", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "pixel. The resulting number is", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "the amount of overscan in pixels", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "in each direction.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				ChangeResolution(&y);
				break;
			case HELP_SMPTE:
				VDP_drawTextBG(APLAN, "SMPTE COLOR BARS", TILE_ATTR(PAL1, 0, 0, 0), 13, 4);
				VDP_drawTextBG(APLAN, "This pattern can be used to", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "approximate for NTSC levels", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "regarding contrast,", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "brightness and colors.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "You can toggle between 75% and", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "100% SMPTE color bars with A.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "Of course the percentages are", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "relative to the console output.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "You can use color filters or the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "blue only option in your display", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "to confirm color balance.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "This HW lowest black is 6%.", TILE_ATTR(PAL3, 0, 0, 0), 4, y);
				break;
			case HELP_GRAY:
				VDP_drawTextBG(APLAN, "GRAY RAMP", TILE_ATTR(PAL1, 0, 0, 0), 14, 4);
				VDP_drawTextBG(APLAN, "This gray ramp pattern can be", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "used to check color balance.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "You should make sure the bars", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "are gray, with no color bias.", TILE_ATTR(PAL0, 0, 0, 0), 4, y);
				break;
			case HELP_WHITE:
				VDP_drawTextBG(APLAN, "WHITE SCREEN", TILE_ATTR(PAL1, 0, 0, 0), 14, 4);
				VDP_drawTextBG(APLAN, "This pattern can be changed", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "between white, black, red,", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "green and blue screens with the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "'A' and 'B' buttons.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "A custom color mode is", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "available by pressing 'C'.", TILE_ATTR(PAL0, 0, 0, 0), 4, y);
				break;
			case HELP_CHECK:
				VDP_drawTextBG(APLAN, "CHECKERBOARD", TILE_ATTR(PAL1, 0, 0, 0), 14, 4);
				VDP_drawTextBG(APLAN, "This pattern shows all the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "visible pixels in an", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "alternating white and black", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "grid array.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "You can toggle the pattern with", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "button 'Up', or turn on", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "auto-toggle each frame with the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "'A' button. A frame counter is", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "also available with 'B'.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				ChangeResolution(&y);
				break;
			case HELP_STRIPES:
				switch (page)
				{
				case 1:
					VDP_drawTextBG(APLAN, "HOR/VER STRIPES (1/2)", TILE_ATTR(PAL1, 0, 0, 0), 8, 4);
					VDP_drawTextBG(APLAN, "You should see a pattern of", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "lines, each one pixel in height", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "starting with a white one.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					y++;
					VDP_drawTextBG(APLAN, "You can toggle the pattern with", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "button 'UP', or turn on", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "auto-toggle each frame with the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "'A' button. A frame counter is", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "also available with 'B'.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					y++;
					VDP_drawTextBG(APLAN, "When auto-toggle is set, you", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "should see the lines", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "alternating rapidly.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "(cont...)", TILE_ATTR(PAL0, 0, 0, 0), 26, 22);
					break;
				case 2:
					VDP_drawTextBG(APLAN, "HOR/VER STRIPES (2/2)", TILE_ATTR(PAL1, 0, 0, 0), 8, 4);
					VDP_drawTextBG(APLAN, "You can also display vertical", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "bars by pressing 'LEFT'. That", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "pattern will help you", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "evaluate if the signal is not", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "distorted horizontaly, since", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "all lines should be one pixel", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "wide.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					
					ChangeResolution(&y);
					break;
				}
				break;
			case HELP_SHADOW:
				VDP_drawTextBG(APLAN, "DROP SHADOW TEST", TILE_ATTR(PAL1, 0, 0, 0), 12, 4);
				VDP_drawTextBG(APLAN, "This is a crucial test for 240p.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "It displays a", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "simple sprite shadow against a", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "background, but the shadow is", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "shown only on each other frame", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "achievieng a transparency effect", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "The user can toggle the frame", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "used to draw the shadow with", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "button 'A'. Backgrounds can be", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "switched with the 'B' button and", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "button 'C' toggles sprites.", TILE_ATTR(PAL0, 0, 0, 0), 4, y);
				break;
			case HELP_STRIPED:
				VDP_drawTextBG(APLAN, "STRIPED SPRITE TEST", TILE_ATTR(PAL1, 0, 0, 0), 10, 4);
				VDP_drawTextBG(APLAN, "There are deinterlacers out", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "there that can display the drop", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "shadows correctly and still", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "interpret 240p as 480i. With a", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "striped sprite it should be easy", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "to tell if a processor tries to", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "deinterlace (plus interpolate).", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "You can change backgrounds with", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "'A'.", TILE_ATTR(PAL0, 0, 0, 0), 4, y);
				break;
			case HELP_MANUALLAG:
				switch (page)
				{
				case 1:
					VDP_drawTextBG(APLAN, "TIMING & REFLEX (1/2)", TILE_ATTR(PAL1, 0, 0, 0), 9, 4);
					VDP_drawTextBG(APLAN, "The main intention is to show a", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "changing pattern on the screen,", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "which can be complemented with", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "audio. This should show to some", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "degree any lag when processing", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "the signal.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					y++;
					VDP_drawTextBG(APLAN, "As an added feature, the user", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "can click the 'A' button when", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "the sprite is aligned with the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "one on the background, and the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "offset in frames from the actual", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "intersection will be shown on", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "screen. A 1khz tone will be", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "played for 1 frame when pressed.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "(cont...)", TILE_ATTR(PAL0, 0, 0, 0), 26, 22);
					break;
				case 2:
					VDP_drawTextBG(APLAN, "TIMING & REFLEX (2/2)", TILE_ATTR(PAL1, 0, 0, 0), 9, 4);
					VDP_drawTextBG(APLAN, "Button 'B' can be used to", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "change the direction of the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "sprite.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					y++;
					VDP_drawTextBG(APLAN, "Of course the evaluation is", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "dependent on reflexes and/or", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "rhythm more than anything. The", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "visual and audio cues are the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "more revealing aspects which", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "the user should consider, of", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "course the interactive factor", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "can give an experienced player", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "the hang of the system when", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "testing via different", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "connections.", TILE_ATTR(PAL0, 0, 0, 0), 4, y);
					break;
				}
				break;
			case HELP_HSCROLL:
				switch (page)
				{
				case 1:
					VDP_drawTextBG(APLAN, "SCROLL TEST (1/2)", TILE_ATTR(PAL1, 0, 0, 0), 11, 4);
					VDP_drawTextBG(APLAN, "This test shows either an", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "horizontal 320x224 back from", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "Sonic or a vertical 256x224", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "back from Kiki Kaikai.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					y++;
					VDP_drawTextBG(APLAN, "Speed can be varied with Up &", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "Down and scroll direction with", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "Left. The 'A' button stops the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "scroll and 'B' toggles between", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "vertical and horizontal.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					y++;
					VDP_drawTextBG(APLAN, "The vertical 256 scroll is also", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "used while in 320 mode but it", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "can be positioned with 'Right'.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					
					VDP_drawTextBG(APLAN, "(cont...)", TILE_ATTR(PAL0, 0, 0, 0), 26, 22);
					break;					
				case 2:
					VDP_drawTextBG(APLAN, "SCROLL TEST (2/2)", TILE_ATTR(PAL1, 0, 0, 0), 11, 4);
					VDP_drawTextBG(APLAN, "This can be used to notice any", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "drops in framerate, or pixel", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "width inconsistencies.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					ChangeResolution(&y);
					y++;
					VDP_drawTextBG(APLAN, "Sonic is a trademark of Sega", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "Enterprises Ltd. Kiki Kaikai", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "is a trademark of Taito.", TILE_ATTR(PAL0, 0, 0, 0), 4, y);
					break;
				}
				break;
			case HELP_VSCROLL:
				VDP_drawTextBG(APLAN, "GRID SCROLL TEST", TILE_ATTR(PAL1, 0, 0, 0), 11, 4);
				VDP_drawTextBG(APLAN, "A grid is scrolled vertically or", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "horizontally, which can be used", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "to test linearity of the signal", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "and how well the display or", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "video processor copes with", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "scrolling and framerate.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "Button 'B' can be used to toggle", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "between horizontal and vertical,", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "while Up/Down regulate speed.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "Button 'A' stops the scroll and", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "'Left' changes direction.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				ChangeResolution(&y);
				break;
			case HELP_SOUND:
				VDP_drawTextBG(APLAN, "SOUND TEST", TILE_ATTR(PAL1, 0, 0, 0), 14, 4);
				VDP_drawTextBG(APLAN, "You can test the Sound from the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
#ifndef SEGACD
				VDP_drawTextBG(APLAN, "Yamaha 2612 and PSG here.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
#else				
				VDP_drawTextBG(APLAN, "YM2612, PSG, CD-DA and PCM here.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
#endif
				y++;
				VDP_drawTextBG(APLAN, "The octave and panning can be", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "changed when possible. This can", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "help you identify stereo cabling", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "issues.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "PSG has 200hz, 2khz and 4khz", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "tones on its channels and white", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "noise at 500hz.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
#ifdef SEGACD
				y++;
				VDP_drawTextBG(APLAN, "With PCM, C changes sample used.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
#endif
				y++;
				VDP_drawTextBG(APLAN, "For balance and frequency checks", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "please use MDFourier.", TILE_ATTR(PAL0, 0, 0, 0), 4, y);
				break;
			case HELP_LED:
				VDP_drawTextBG(APLAN, "BACKLIT TEST", TILE_ATTR(PAL1, 0, 0, 0), 10, 4);
				VDP_drawTextBG(APLAN, "This test allows you to check", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "how the display's backlit works", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "when only a small array of", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "pixels is shown.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "The user can move around the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "white pixel arrays with the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "d-pad, and change the size of", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "the pixel array with 'A'.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "The 'B' button allows the user", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "to hide the pixel array in", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "order to alternate a fully black", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "screen.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				ChangeResolution(&y);
				break;
			case HELP_PHASE:
				VDP_drawTextBG(APLAN, "PHASE CHECK", TILE_ATTR(PAL1, 0, 0, 0), 14, 4);
				VDP_drawTextBG(APLAN, "This test allows you to check", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "phase alignment in upscalers.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "Designed by FirebrandX", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				break;
			case HELP_LAG:
				switch (page)
				{
				case 1:
					VDP_drawTextBG(APLAN, "LAG TEST (1/2)", TILE_ATTR(PAL1, 0, 0, 0), 14, 4);
					VDP_drawTextBG(APLAN, "This test is designed to be used", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "with two displays conected at", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "the same time. One being a CRT,", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "or a display with a known lag as", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "reference, and the other the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "display to test.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					y++;
					VDP_drawTextBG(APLAN, "Using a camera, a picture should", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "be taken of both screens at the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "same time. The picture will show", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "the frame discrepancy between", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "them.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					y++;
					VDP_drawTextBG(APLAN, "The circles in the bottom help", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "determine the frame even when", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "(cont...)", TILE_ATTR(PAL0, 0, 0, 0), 26, 22);
					break;
				case 2:
					VDP_drawTextBG(APLAN, "LAG TEST (2/2)", TILE_ATTR(PAL1, 0, 0, 0), 14, 4);
					VDP_drawTextBG(APLAN, "the numbers are blurry.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					y++;
					VDP_drawTextBG(APLAN, "This version of the suite can be", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "used with a Sega Nomad as the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "reference display.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					y++;
					VDP_drawTextBG(APLAN, "You can also split the video", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "signal and feed both displays.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					y++;
					VDP_drawTextBG(APLAN, "The vertical bars on the sides", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "change color each frame to help", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "when using LCD photos.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					y++;
					VDP_drawTextBG(APLAN, "Press A to start/stop, B to", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "reset and C for B&W test.", TILE_ATTR(PAL0, 0, 0, 0), 4, y);
					break;
				}
				break;
			case HELP_ALTERNATE:
				VDP_drawTextBG(APLAN, "ALTERNATE 240p/480i", TILE_ATTR(PAL1, 0, 0, 0), 11, 4);
				VDP_drawTextBG(APLAN, "Some devices have a delay when", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "the source changes between", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "resolutions, which happens in", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "some games. This test allows to", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "time that delay manually.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "Press A to switch the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "resolution; press it again when", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "you are able to see the screen", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "back in your display.", TILE_ATTR(PAL0, 0, 0, 0), 4, y);
				break;
			case HELP_VIDEO:
				VDP_drawTextBG(APLAN, "VIDEO OPTIONS", TILE_ATTR(PAL1, 0, 0, 0), 13, 4);
				VDP_drawTextBG(APLAN, "Here you can enable interlaced", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "(480i) mode for the whole suite", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "and disable horizontal 256 in", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "compatible screens.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "For PAL consoles, you can also", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "enable full 240p, instead of", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "224p.", TILE_ATTR(PAL0, 0, 0, 0), 4, y);
				break;
			case HELP_AUDIOSYNC:
				VDP_drawTextBG(APLAN, "AUDIO SYNC", TILE_ATTR(PAL1, 0, 0, 0), 15, 4);
				VDP_drawTextBG(APLAN, "This test flashes the whole", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "screen white for 2 frames,", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "along with a 1khz test tone.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "You can verify the sync between", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "audio and video with recording", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "equipment or specialized", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "hardware.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "Press A to start/stop the test.", TILE_ATTR(PAL0, 0, 0, 0), 4, y);
				break;
			case HELP_CONTRAST:
				VDP_drawTextBG(APLAN, "PLUGE CONTRAST", TILE_ATTR(PAL1, 0, 0, 0), 15, 4);
				VDP_drawTextBG(APLAN, "This sub test fills the screen", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "with a pattern. By default it", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "is drawn in color.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "You can change palettes with", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "A to display a dark or white", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "pattern in order to check", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "contrast and brightness.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "The C button returns to PLUGE.", TILE_ATTR(PAL0, 0, 0, 0), 4, y);
				break;
			case HELP_MDFOURIER:
				VDP_drawTextBG(APLAN, "MDFourier", TILE_ATTR(PAL1, 0, 0, 0), 16, 4);
				VDP_drawTextBG(APLAN, "Button A plays the test tones.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "Please record it with your audio", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "card and use the software at:", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "http://junkerhq.net/MDFourier", TILE_ATTR(PAL1, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "It will generate plots with the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "audio signature from this system", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "that can be compared to others.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "Can be booted straight here by", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "holding START", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "X button opens the debug menu", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
#ifdef SEGACD
				VDP_drawTextBG(APLAN, "for various PCM tests.", TILE_ATTR(PAL0, 0, 0, 0), 4, y);
#else
				VDP_drawTextBG(APLAN, "for Z80 bus requests.", TILE_ATTR(PAL0, 0, 0, 0), 4, y);
#endif
				break;
			case HELP_SEGACD:
				VDP_drawTextBG(APLAN, "Sega CD", TILE_ATTR(PAL1, 0, 0, 0), 16, 4);
#ifdef SEGACD
				VDP_drawTextBG(APLAN, "More options are available", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "when runing as a cart: RAM tests", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "and register tests.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
#else
				VDP_drawTextBG(APLAN, "You can identify some issues", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "on non booting systems with", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "these.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
#endif
				y++;
				VDP_drawTextBG(APLAN, "Special thanks to Leo Oliveira.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				break;
			case HELP_MEMVIEW:
				switch (page)
				{
				case 1:
					VDP_drawTextBG(APLAN, "MEMORY VIEWER", TILE_ATTR(PAL1, 0, 0, 0), 12, 4);
					VDP_drawTextBG(APLAN, "This shows selected regions of", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "the main CPU memory map.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					y++;
					VDP_drawTextBG(APLAN, "The current address range is", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "shown in red at the right from", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "top to bottom.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					y++;
					VDP_drawTextBG(APLAN, "The left part of the screen", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "shows 0x1C0 bytes in hex.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					y++;
					VDP_drawTextBG(APLAN, "- Button A enabled CRC of", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "current screen.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					y++;
					VDP_drawTextBG(APLAN, "- Button B jumps to relevant", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "memory locations", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "(cont...)", TILE_ATTR(PAL0, 0, 0, 0), 26, 22);
					break;
				case 2:
					
					VDP_drawTextBG(APLAN, "- Button C switches banks in", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "SCD Program RAM, or switches", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "to ASCII mode.", TILE_ATTR(PAL0, 0, 0, 0), 4, y);
#ifndef SEGACD	
					y++;
					VDP_drawTextBG(APLAN, "With a working SCD system", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "Program RAM banks should show", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "0xAA for bank 1 (0xBB for bank", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					VDP_drawTextBG(APLAN, "2 etc.) in all four corners.", TILE_ATTR(PAL0, 0, 0, 0), 4, y);
#endif
					break;
				}
				break;
			case HELP_CONVERGENCE:
				VDP_drawTextBG(APLAN, "CONVERGENCE TESTS", TILE_ATTR(PAL1, 0, 0, 0), 11, 4);
				VDP_drawTextBG(APLAN, "These are used to adjust color", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "convergence in CRT displays.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "A and B buttons change the cross", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "hatch pattern between lines,", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "dots and crosses. Then to a", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "color pattern for transition", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "boundary check with and without", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "a black border.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				ChangeResolution(&y);
				break;
			case HELP_DISAPPEAR:
				VDP_drawTextBG(APLAN, "DISAPPEARING LOGO", TILE_ATTR(PAL1, 0, 0, 0), 11, 4);
				VDP_drawTextBG(APLAN, "This test allows you to use a", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "slow motion camera to measure", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "how long it takes from a button", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "press to when the logo", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "disappears.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				y++;
				VDP_drawTextBG(APLAN, "You can also use a photodiode", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "and press C, which turns", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "the whole back to white for", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				VDP_drawTextBG(APLAN, "2 frames.", TILE_ATTR(PAL0, 0, 0, 0), 4, y);
				break;
			default:
				exit = 1;
				break;
			}

			VDP_End();
			redraw = 0;
		}

		buttons = JOY_readJoypad(JOY_ALL);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		if(joytype != JOY_TYPE_PAD6)
		{
			VDP_Start();
			VDP_drawTextBG(APLAN, "Press START to exit help", TILE_ATTR(PAL0, 0, 0, 0), 9, 24);
			VDP_End();

			if(pressedButtons & BUTTON_START)
				exit = 1;
			if(pressedButtons & BUTTON_B)
				exit = 1;
		}
		else
		{
			VDP_Start();
			VDP_drawTextBG(APLAN, "Press Z to exit help", TILE_ATTR(PAL0, 0, 0, 0), 11, 24);
			VDP_End();

			if(pressedButtons & BUTTON_Z)
				exit = 1;
			if(pressedButtons & BUTTON_B)
				exit = 1;
			if(pressedButtons & BUTTON_START)
				exit = 1;
		}

		if(pressedButtons & BUTTON_LEFT)
		{
			if(page - 1 > 0)
			{
				page--;
				redraw = 1;
			}
		}

		if(pressedButtons & BUTTON_RIGHT)
		{
			if(page + 1 <= totalpages)
			{
				page++;
				redraw = 1;
			}
		}

		VDP_waitVSync();
	}
	VDP_fadeOutAll(FADE_TIME, 0);
	VDP_clearTileMapRect(APLAN, 0, 0, 320 / 8, 224 / 8);
	VDP_clearTileMapRect(BPLAN, 0, 0, 320 / 8, 224 / 8);
	VDP_resetScreen();
	VDP_resetSprites();
	VDP_updateSprites();
}
