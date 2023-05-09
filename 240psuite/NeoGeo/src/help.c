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
#include "help.h"
#include "string_ng.h"
#include "tools.h"

int checkHelp(int helpID)
{
	if (BTTN_HELP)
	{
		DrawHelp(helpID);
		return 1;
	}

	return 0;
}

void DrawHelp(int option)
{
	int exit = 0, page = 1, totalpages = 1, redraw = 1, index = 0;

	switch (option)
	{
		case HELP_GENERAL:
		case HELP_STRIPES:
		case HELP_SHADOW:
		case HELP_LED:
		case HELP_LAG:
		case HELP_MANUALLAG:
		case HELP_HSCROLL:
		case HELP_MEMVIEW:
		case HELP_WHITE:
		case HELP_PLUGE:
		case HELP_COLORS:
		totalpages = 2;
		break;

		case HELP_MONOSCOPE:
		totalpages = 3;
		break;

		case HELP_HCFR:
		totalpages = 8;
		break;
	}

	while (!exit)
	{
		if (redraw)
		{
			int y = 9;

			gfxClear();
			index = draw_background();
			redraw = 0;

			fixPrintC(26, fontColorWhite, fbase, "Press START or B to exit help");
			switch (option)
			{
				case HELP_GENERAL:
					switch (page)
					{
						case 1:
							fixPrintC(6, fontColorGreen, fbase, "HELP (1/2)");

							fixPrint(4, 8, fontColorWhite, fbase, "The 240p Test Suite was designed");
							fixPrint(4, 9, fontColorWhite, fbase, "with two goals in mind:");

							fixPrint(4, 11, fontColorWhite, fbase, "1) Evaluate 240p signals on TV");
							fixPrint(4, 12, fontColorWhite, fbase, "sets and video processors; and");

							fixPrint(4, 14, fontColorWhite, fbase, "2) Provide calibration patterns");
							fixPrint(4, 15, fontColorWhite, fbase, "from a game console to help in");
							fixPrint(4, 16, fontColorWhite, fbase, "properly calibrating the display");
							fixPrint(4, 17, fontColorWhite, fbase, "black, white, and color levels.");
							fixPrint(4, 19, fontColorGreen, fbase, "Help can be shown with 'START'");
							fixPrint(4, 20, fontColorGreen, fbase, "in any test.");

							fixPrint(26, 24, fontColorYellow, fbase, "(cont...)");
						break;

						case 2:
						{
							picture qr;

							pictureInit(&qr, &barcode, index, 17, 246, 144, FLIP_NONE);
							palJobPut(17,barcode.palInfo->count,barcode.palInfo->data);

							fixPrintC(6, fontColorGreen, fbase, "HELP (2/2)");

							if(isPAL)
							{
								fixPrint(4, 8, fontColorWhite, fbase, "PAL systems can output either ");
								fixPrint(4, 9,  fontColorWhite, fbase, "224 or 256 lines.");
							}
							else
							{
								fixPrint(4, 8, fontColorWhite, fbase, "The Neo Geo can output 224");
								fixPrint(4, 9, fontColorWhite, fbase, "active video lines.");
							}

							fixPrint(4, 11, fontColorWhite, fbase, "For horizontal resolution it can");
							fixPrint(4, 12, fontColorWhite, fbase, "mask the sides to SNK's default");
							fixPrint(4, 13, fontColorWhite, fbase, "304 or the full 320 pixels.");

							fixPrint(4, 15, fontColorWhite, fbase, "The 240p suite is also available");
							fixPrint(4, 16, fontColorWhite, fbase, "on NES/FC, SNES/SFC, GameCube,");
							fixPrint(4, 17, fontColorWhite, fbase, "Wii, GBA, Dreamcast,");
							fixPrint(4, 18, fontColorWhite, fbase, "Genesis/Mega Drive, Sega 32X");
							fixPrint(4, 19, fontColorWhite, fbase, "Sega/Mega CD, SMS, and");
							fixPrint(4, 20, fontColorWhite, fbase, "PCE/TG-16/PCE-Duo/SCD/SCD2");

							fixPrint(4, 22, fontColorWhite, fbase, "Visit:");
							fixPrint(4, 23, fontColorGreen, fbase, "http://junkerhq.net/240p");
							fixPrint(4, 24, fontColorWhite, fbase, "for more information");
						}
						break;
					}
				break;

				case HELP_PLUGE:
					switch (page)
					{
						case 1:
							fixPrintC(6, fontColorGreen, fbase, "PLUGE (1/2)");

							fixPrint(4, y++, fontColorWhite, fbase, "The PLUGE pattern is used to");
							fixPrint(4, y++, fontColorWhite, fbase, "help adjust the black level to");
							fixPrint(4, y++, fontColorWhite, fbase, "a correct value.");
							y++;
							fixPrint(4, y++, fontColorWhite, fbase, "In PAL - and console gaming in");
							fixPrint(4, y++, fontColorWhite, fbase, "general - it is adviced to use");
							fixPrint(4, y++, fontColorWhite, fbase, "a value of 2 IRE as black.");
							y++;
							fixPrint(4, y++, fontColorWhite, fbase, "This system can output 2.5 IRE");
							fixPrint(4, y++, fontColorWhite, fbase, "and up to 107 IRE.");
							y++;
							fixPrint(4, y++, fontColorWhite, fbase, "The inner bars are at 2.5%, then");
							fixPrint(4, y++, fontColorWhite, fbase, "the middle at 4.5% and the outer");
							fixPrint(4, y++, fontColorWhite, fbase, "at 7.5%.");

							fixPrint(26, 24, fontColorYellow, fbase, "(cont...)");
						break;

						case 2:
							fixPrintC(6, fontColorGreen, fbase, "PLUGE (2/2)");

							fixPrint(4, y++, fontColorWhite, fbase, "If these bars are not visible,");
							fixPrint(4, y++, fontColorWhite, fbase, "adjust the \"brightness\" control");
							fixPrint(4, y++, fontColorWhite, fbase, "until they are.");
							y++;
							fixPrint(4, y++, fontColorWhite, fbase, "You should lower it until they");
							fixPrint(4, y++, fontColorWhite, fbase, "are not visible, and raise it");
							fixPrint(4, y++, fontColorWhite, fbase, "until they show.");
							y++;
							fixPrint(4, y++, fontColorWhite, fbase, "The A button changes between the");
							fixPrint(4, y++, fontColorWhite, fbase, "suggested RGB and NTSC values.");
							y++;
							fixPrint(4, y++, fontColorWhite, fbase, "NTSC levels require black to be");
							fixPrint(4, y++, fontColorWhite, fbase, "at 7.5 IRE for video. So the bar");
							fixPrint(4, y++, fontColorWhite, fbase, "values are 4.5, 7.5 and 10 IRE.");
						break;
					}
				break;

				case HELP_COLORS:
					switch(page)
					{
					case 1:
						fixPrintC(6, fontColorGreen, fbase, "COLORBARS (1/2)");

						fixPrint(4, y++, fontColorWhite, fbase, "This pattern allows you to");
						fixPrint(4, y++, fontColorWhite, fbase, "calibrate each color: Red, Green");
						fixPrint(4, y++, fontColorWhite, fbase, "and Blue; as well as white.");
						y++;
						fixPrint(4, y++, fontColorWhite, fbase, "Adjust the white level first,");
						fixPrint(4, y++, fontColorWhite, fbase, "using the \"Contrast\" control");
						fixPrint(4, y++, fontColorWhite, fbase, "on your TV set. Raise it until");
						fixPrint(4, y++, fontColorWhite, fbase, "you cannot distinguish between");
						fixPrint(4, y++, fontColorWhite, fbase, "the blocks under \"1E\" and the");
						fixPrint(4, y++, fontColorWhite, fbase, "last bar, then lower it slowly");
						fixPrint(4, y++, fontColorWhite, fbase, "until you can clearly tell them");
						fixPrint(4, y++, fontColorWhite, fbase, "appart.");
						y++;
						fixPrint(4, y++, fontColorWhite, fbase, "Do the same for each color.");
						fixPrint(26, 24, fontColorYellow, fbase, "(cont...)");
					break;
					case 2:
						fixPrintC(6, fontColorGreen, fbase, "COLORBARS (2/2)");

						fixPrint(4, y++, fontColorWhite, fbase, "The Neo Geo displays 64 shades");
						fixPrint(4, y++, fontColorWhite, fbase, "of red, green, blue and white. ");
						fixPrint(4, y++, fontColorWhite, fbase, "Each one in a light or dark ");
						fixPrint(4, y++, fontColorWhite, fbase, "variant set by the \"dark bit\".");
						y++;
						fixPrint(4, y++, fontColorWhite, fbase, "You can toggle the dark bit with");
						fixPrint(4, y++, fontColorWhite, fbase, "the A button. If the IRE 100");
						fixPrint(4, y++, fontColorWhite, fbase, "option is enabled the brightest");
						fixPrint(4, y++, fontColorWhite, fbase, "colors on each ramp will be");
						fixPrint(4, y++, fontColorWhite, fbase, "hidden, since they are at");
						fixPrint(4, y++, fontColorWhite, fbase, "107 IRE.");
						y++;
						fixPrint(4, y++, fontColorWhite, fbase, "The B button toggles a grid");
						fixPrint(4, y++, fontColorWhite, fbase, "background");
					break;
					}
				
				break;

				case HELP_GRID:
					// TODO Mention 302 and 320 info here
					fixPrintC(6, fontColorGreen, fbase, "GRID");

					fixPrint(4, 9, fontColorWhite, fbase, "This grid uses the full 320x224");
					fixPrint(4, 10, fontColorWhite, fbase, "resolution.");

					fixPrint(4, 12, fontColorWhite, fbase, "You can use it to verify that");
					fixPrint(4, 13, fontColorWhite, fbase, "all the visible area is being");
					fixPrint(4, 14, fontColorWhite, fbase, "displayed, and that there is no");
					fixPrint(4, 15, fontColorWhite, fbase, "distortion present.");

					fixPrint(4, 17, fontColorWhite, fbase, "The full active video signal can");
					fixPrint(4, 18, fontColorWhite, fbase, "be filled with gray by pressing");
					fixPrint(4, 19, fontColorWhite, fbase, "the 'A' button.");
				break;

				case HELP_MONOSCOPE:
					switch (page)
					{
						case 1:
							fixPrintC(6, fontColorGreen, fbase, "MONOSCOPE (1/3)");

							fixPrint(4, 9, fontColorWhite, fbase, "This pattern contains elements");
							fixPrint(4, 10, fontColorWhite, fbase, "to calibrate multiple aspects");
							fixPrint(4, 11, fontColorWhite, fbase, "of a CRT.");

							fixPrint(4, 13, fontColorWhite, fbase, "Read your monitor's service");
							fixPrint(4, 14, fontColorWhite, fbase, "manual to learn how, and use");
							fixPrint(4, 15, fontColorWhite, fbase, "'A' button to change IRE.");

							fixPrint(4, 17, fontColorWhite, fbase, "Brightness adjustment: Adjust");
							fixPrint(4, 18, fontColorWhite, fbase, "convergence at low brightness");
							fixPrint(4, 19, fontColorWhite, fbase, "(13/25 IRE). An overly bright");
							fixPrint(4, 20, fontColorWhite, fbase, "pattern can mask convergence.");
							fixPrint(4, 21, fontColorWhite, fbase, "issues.");

							fixPrint(26, 24, fontColorYellow, fbase, "(cont...)");
						break;

						case 2:
							fixPrintC(6, fontColorGreen, fbase, "MONOSCOPE (2/3)");

							fixPrint(4, 9, fontColorWhite, fbase, "Convergence: Use the center");
							fixPrint(4, 10, fontColorWhite, fbase, "crosshair to check static");
							fixPrint(4, 11, fontColorWhite, fbase, "(center of screen) convergence.");
							fixPrint(4, 12, fontColorWhite, fbase, "Use the patterns at the sides");
							fixPrint(4, 13, fontColorWhite, fbase, "to check dynamic (edge)");
							fixPrint(4, 14, fontColorWhite, fbase, "convergence.");

							fixPrint(4, 16, fontColorWhite, fbase, "Corners: After setting center");
							fixPrint(4, 17, fontColorWhite, fbase, "and edge convergence, use");
							fixPrint(4, 18, fontColorWhite, fbase, "magnets to adjust corner purity");
							fixPrint(4, 19, fontColorWhite, fbase, "and geometry.");

							fixPrint(26, 24, fontColorYellow, fbase, "(cont...)");
						break;

						case 3:
							fixPrintC(6, fontColorGreen, fbase, "MONOSCOPE (3/3)");

							fixPrint(4, 9, fontColorWhite, fbase, "Size and aspect ratio: If");
							fixPrint(4, 10, fontColorWhite, fbase, "vertical and horizontal size are");
							fixPrint(4, 11, fontColorWhite, fbase, "correct, the red squares in the");
							fixPrint(4, 12, fontColorWhite, fbase, "pattern will be perfect squares.");
							fixPrint(4, 13, fontColorWhite, fbase, "After setting H size, use a tape");
							fixPrint(4, 14, fontColorWhite, fbase, "measure to adjust V size to");
							fixPrint(4, 15, fontColorWhite, fbase, "match it.");

							fixPrint(4, 17, fontColorWhite, fbase, "Linearity: The squares in each");
							fixPrint(4, 18, fontColorWhite, fbase, "corner should get you started.");
							fixPrint(4, 19, fontColorWhite, fbase, "Confirm your adjustment using");
							fixPrint(4, 20, fontColorWhite, fbase, "the scroll tests.");

							fixPrint(4, 22, fontColorGreen, fbase, "Designed by Keith Raney.");
						break;
					}
				break;

				case HELP_BLEED:
					fixPrintC(6, fontColorGreen, fbase, "COLOR BLEED");

					fixPrint(4, 9, fontColorWhite, fbase, "This pattern helps diagnose");
					fixPrint(4, 10, fontColorWhite, fbase, "color bleed caused by unneeded");
					fixPrint(4, 11, fontColorWhite, fbase, "color upsampling.");

					fixPrint(4, 13, fontColorWhite, fbase, "You can toggle between vertical");
					fixPrint(4, 14, fontColorWhite, fbase, "bars and checkerboard with 'A'.");
				break;

				case HELP_IRE:
					fixPrintC(6, fontColorGreen, fbase, "100 IRE");

					fixPrint(4, y++, fontColorWhite, fbase, "You can vary IRE intensity with");
					fixPrint(4, y++, fontColorWhite, fbase, "'A' and 'C'.");
					y++;
					fixPrint(4, y++, fontColorWhite, fbase, "Values are:");
					fixPrint(4, y++, fontColorWhite, fbase, "10, 20, 30, 40, 50, 60, 70, 80,");
					fixPrint(4, y++, fontColorWhite, fbase, "90 and 100.");
					y++;
					fixPrint(4, y++, fontColorWhite, fbase, "When IRE 100 limit is not");
					fixPrint(4, y++, fontColorWhite, fbase, "enabled 107 IRE is also");
					fixPrint(4, y++, fontColorWhite, fbase, "available.");
				break;

				case HELP_601CB:
					fixPrintC(6, fontColorGreen, fbase, "REFERENCED COLORBARS");

					fixPrint(4, y++, fontColorWhite, fbase, "You can use color filters or the");
					fixPrint(4, y++, fontColorWhite, fbase, "blue only option in your display");
					fixPrint(4, y++, fontColorWhite, fbase, "to confirm color balance.");
					y++;
					fixPrint(4, y++, fontColorWhite, fbase, "The background and colors are");
					fixPrint(4, y++, fontColorWhite, fbase, "at 75 IRE, in order to check");
					fixPrint(4, y++, fontColorWhite, fbase, "color balance. This can be");
					fixPrint(4, y++, fontColorWhite, fbase, "set by color and tint controls");
					fixPrint(4, y++, fontColorWhite, fbase, "when using composite video.");
				break;

				case HELP_EBU:
					fixPrintC(6, fontColorGreen, fbase, "EBU COLORBARS");

					fixPrint(4, y++, fontColorWhite, fbase, "You can toggle 75% and 100% bars");
					fixPrint(4, y++, fontColorWhite, fbase, "with the A button.");
					y++;
					fixPrint(4, y++, fontColorWhite, fbase, "This pattern follows the \"limit");
					fixPrint(4, y++, fontColorWhite, fbase, "to 100 IRE\" option.");
				break;

				case HELP_SHARPNESS:
					fixPrintC(6, fontColorGreen, fbase, "SHARPNESS");

					fixPrint(4, 9, fontColorWhite, fbase, "You should set the sharpness of");
					fixPrint(4, 10, fontColorWhite, fbase, "your CRT to a value that shows");
					fixPrint(4, 11, fontColorWhite, fbase, "clean black and gray transitions");
					fixPrint(4, 12, fontColorWhite, fbase, "with no white ghosting between.");

					fixPrint(4, 13, fontColorWhite, fbase, "Pressing 'A' toggles a diagonal");
					fixPrint(4, 14, fontColorWhite, fbase, "brick tileset.");
				break;

				case HELP_OVERSCAN:
					fixPrintC(6, fontColorGreen, fbase, "OVERSCAN");

					fixPrint(4, 9, fontColorWhite, fbase, "With this pattern you can");
					fixPrint(4, 10, fontColorWhite, fbase, "interactively find out the");
					fixPrint(4, 11, fontColorWhite, fbase, "overscan in pixels of each edge");
					fixPrint(4, 12, fontColorWhite, fbase, "in a display.");

					fixPrint(4, 14, fontColorWhite, fbase, "Use left and right to change");
					fixPrint(4, 15, fontColorWhite, fbase, "the overscan until you see the");
					fixPrint(4, 16, fontColorWhite, fbase, "white border, then go back one");
					fixPrint(4, 17, fontColorWhite, fbase, "pixel. The resulting number is");
					fixPrint(4, 18, fontColorWhite, fbase, "the overscan in each direction.");

					fixPrint(4, 20, fontColorWhite, fbase, "A toggles fast/per pixel change");
					fixPrint(4, 21, fontColorWhite, fbase, "and C resets to defaults.");
				break;

				case HELP_SMPTE:
					fixPrintC(6, fontColorGreen, fbase, "SMPTE COLOR BARS");

					fixPrint(4, y++, fontColorWhite, fbase, "This pattern can be used to");
					fixPrint(4, y++, fontColorWhite, fbase, "calibrate for NTSC levels");
					fixPrint(4, y++, fontColorWhite, fbase, "regarding contrast, brightness");
					fixPrint(4, y++, fontColorWhite, fbase, "and colors.");
					y++;
					fixPrint(4, y++, fontColorWhite, fbase, "You can toggle between 75% and");
					fixPrint(4, y++, fontColorWhite, fbase, "100% SMPTE color bars with A.");
					y++;
					fixPrint(4, y++, fontColorWhite, fbase, "This test follows the 100 IRE");
					fixPrint(4, y++, fontColorWhite, fbase, "limit option. So if you want to");
					fixPrint(4, y++, fontColorWhite, fbase, "measure the hardware, use it at");
					fixPrint(4, y++, fontColorWhite, fbase, "107 IRE.");
					y++;
					fixPrint(4, y++, fontColorWhite, fbase, "But if you want to calibrate");
					fixPrint(4, y++, fontColorWhite, fbase, "for NTSC, use it limited to 100.");
				break;

				case HELP_GRAY:
					// TODO: add dark/light description here
					fixPrintC(6, fontColorGreen, fbase, "GRAY RAMP");

					fixPrint(4, 9, fontColorWhite, fbase, "This gray ramp pattern can be");
					fixPrint(4, 10, fontColorWhite, fbase, "used to check color balance.");

					fixPrint(4, 12, fontColorWhite, fbase, "You should make sure the bars");
					fixPrint(4, 13, fontColorWhite, fbase, "are gray, with no color bias.");
				break;

				case HELP_WHITE:
					switch (page)
					{
						case 1:
							fixPrintC(6, fontColorGreen, fbase, "WHITE SCREEN (1/2)");

							fixPrint(4, 9, fontColorWhite, fbase, "This pattern can be changed");
							fixPrint(4, 10, fontColorWhite, fbase, "between white, black, red,");
							fixPrint(4, 11, fontColorWhite, fbase, "green and blue screens with the");
							fixPrint(4, 12, fontColorWhite, fbase, "'C' and 'D' buttons.");

							fixPrint(4, 14, fontColorWhite, fbase, "Some displays and scalers have");
							fixPrint(4, 15, fontColorWhite, fbase, "issues when changing between a");
							fixPrint(4, 16, fontColorWhite, fbase, "black 0 IRE and a white screen.");

							fixPrint(4, 18, fontColorWhite, fbase, "A custom color mode is");
							fixPrint(4, 19, fontColorWhite, fbase, "available by pressing 'A'");
							fixPrint(4, 20, fontColorWhite, fbase, "while in the white screen.");
							fixPrint(26, 24, fontColorYellow, fbase, "(cont...)");
						break;

						case 2:
							fixPrintC(6, fontColorGreen, fbase, "WHITE SCREEN (2/2)");

							fixPrint(4, 9, fontColorWhite, fbase, "While in edit mode, left and");
							fixPrint(4, 10, fontColorWhite, fbase, "right change between colors,");
							fixPrint(4, 11, fontColorWhite, fbase, "and up & down change values.");

							fixPrint(4, 14, fontColorWhite, fbase, "'C' and 'D' buttons go to");
							fixPrint(4, 15, fontColorWhite, fbase, "zero and 31, while 'A' toggles");
							fixPrint(4, 16, fontColorWhite, fbase, "the dark bit.");
						break;
					}
				break;

				case HELP_CONVERGENCE:
					fixPrintC(6, fontColorGreen, fbase, "CONVERGENCE TESTS");

					fixPrint(4, 9, fontColorWhite, fbase, "These are used to adjust color");
					fixPrint(4, 10, fontColorWhite, fbase, "convergence in CRT displays");

					fixPrint(4, 12, fontColorWhite, fbase, "The 'A' button changes the cross");
					fixPrint(4, 13, fontColorWhite, fbase, "hatch pattern between lines,");
					fixPrint(4, 14, fontColorWhite, fbase, "dots and crosses");

					fixPrint(4, 16, fontColorWhite, fbase, "The 'C' button changes to a");
					fixPrint(4, 17, fontColorWhite, fbase, "color pattern for transition");
					fixPrint(4, 18, fontColorWhite, fbase, "boundary check.");
				break;

				case HELP_CHECK:
					fixPrintC(6, fontColorGreen, fbase, "CHECKERBOARD");

					fixPrint(4, 9, fontColorWhite, fbase, "This pattern shows all the");
					fixPrint(4, 10, fontColorWhite, fbase, "visible pixels in an");
					fixPrint(4, 11, fontColorWhite, fbase, "alternating white and black");
					fixPrint(4, 12, fontColorWhite, fbase, "grid array.");

					fixPrint(4, 14, fontColorWhite, fbase, "You can toggle the pattern with");
					fixPrint(4, 15, fontColorWhite, fbase, "button 'Up', or turn on");
					fixPrint(4, 16, fontColorWhite, fbase, "auto-toggle each frame with the");
					fixPrint(4, 17, fontColorWhite, fbase, "'A' button. A frame counter is");
					fixPrint(4, 18, fontColorWhite, fbase, "also available with 'C'.");
				break;

				case HELP_STRIPES:
					switch (page)
					{
						case 1:
							fixPrintC(6, fontColorGreen, fbase, "HOR/VER STRIPES (1/2)");

							fixPrint(4, 9, fontColorWhite, fbase, "You should see a pattern of");
							fixPrint(4, 10, fontColorWhite, fbase, "lines, each one pixel in height");
							fixPrint(4, 11, fontColorWhite, fbase, "starting with a white one.");

							fixPrint(4, 13, fontColorWhite, fbase, "You can toggle the pattern with");
							fixPrint(4, 14, fontColorWhite, fbase, "button 'UP', or turn on");
							fixPrint(4, 15, fontColorWhite, fbase, "auto-toggle each frame with the");
							fixPrint(4, 16, fontColorWhite, fbase, "'A' button. A frame counter is");
							fixPrint(4, 17, fontColorWhite, fbase, "also available with 'B'.");

							fixPrint(4, 18, fontColorWhite, fbase, "When auto-toggle is set, you");
							fixPrint(4, 19, fontColorWhite, fbase, "should see the lines");
							fixPrint(4, 20, fontColorWhite, fbase, "alternating rapidly.");
							fixPrint(26, 24, fontColorYellow, fbase, "(cont...)");
						break;

						case 2:
							fixPrintC(6, fontColorGreen, fbase, "HOR/VER STRIPES (2/2)");

							fixPrint(4, 9, fontColorWhite, fbase, "You can also display vertical");
							fixPrint(4, 10, fontColorWhite, fbase, "bars by pressing 'LEFT'. That");
							fixPrint(4, 11, fontColorWhite, fbase, "pattern will help you");
							fixPrint(4, 12, fontColorWhite, fbase, "evaluate if the signal is not");
							fixPrint(4, 13, fontColorWhite, fbase, "distorted horizontaly, since");
							fixPrint(4, 14, fontColorWhite, fbase, "all lines should be one pixel");
							fixPrint(4, 15, fontColorWhite, fbase, "wide.");
						break;
					}
				break;

				case HELP_SHADOW:
					fixPrintC(6, fontColorGreen, fbase, "DROP SHADOW TEST");

					fixPrint(4, 9, fontColorWhite, fbase, "This is a crucial test for 240p");
					fixPrint(4, 10, fontColorWhite, fbase, "It displays a simple sprite");
					fixPrint(4, 11, fontColorWhite, fbase, "shadow against a background,");
					fixPrint(4, 12, fontColorWhite, fbase, "but the shadow is shown only");
					fixPrint(4, 13, fontColorWhite, fbase, "on each other frame. Achieving");
					fixPrint(4, 14, fontColorWhite, fbase, "a transparency effect.");

					fixPrint(4, 16, fontColorWhite, fbase, "The user can toggle the frame");
					fixPrint(4, 17, fontColorWhite, fbase, "used to draw the shadow with");
					fixPrint(4, 18, fontColorWhite, fbase, "button 'C'. Backgrounds can be");
					fixPrint(4, 19, fontColorWhite, fbase, "switched with the 'A' button,");
					fixPrint(4, 20, fontColorWhite, fbase, "and button 'D' toggles sprites.");

					fixPrint(4, 22, fontColorGreen, fbase, "Donna by Pepe Salot");
				break;

				case HELP_STRIPED:
					fixPrintC(6, fontColorGreen, fbase, "STRIPED SPRITE TEST");

					fixPrint(4, 9, fontColorWhite, fbase, "There are deinterlacers out");
					fixPrint(4, 10, fontColorWhite, fbase, "there that can display the drop");
					fixPrint(4, 11, fontColorWhite, fbase, "shadows correctly and still");
					fixPrint(4, 12, fontColorWhite, fbase, "interpret 240p as 480i. With a");
					fixPrint(4, 13, fontColorWhite, fbase, "striped sprite it should be easy");
					fixPrint(4, 14, fontColorWhite, fbase, "to tell if a processor tries to");
					fixPrint(4, 15, fontColorWhite, fbase, "deinterlace (plus interpolate).");

					fixPrint(4, 17, fontColorWhite, fbase, "You can change backgrounds with");
					fixPrint(4, 18, fontColorWhite, fbase, "'A'.");
				break;

				case HELP_MANUALLAG:
					switch (page)
					{
						case 1:
							fixPrintC(6, fontColorGreen, fbase, "TIMING & REFLEX (1/2)");

							fixPrint(4, 8, fontColorWhite, fbase, "The main intention is to show a");
							fixPrint(4, 9, fontColorWhite, fbase, "changing pattern on the screen,");
							fixPrint(4, 10, fontColorWhite, fbase, "which can be complemented with");
							fixPrint(4, 11, fontColorWhite, fbase, "audio. This should show to some");
							fixPrint(4, 12, fontColorWhite, fbase, "degree any lag when processing");
							fixPrint(4, 13, fontColorWhite, fbase, "the signal.");

							fixPrint(4, 15, fontColorWhite, fbase, "As an added feature, the user");
							fixPrint(4, 16, fontColorWhite, fbase, "can click the 'A' button when");
							fixPrint(4, 17, fontColorWhite, fbase, "the sprite is aligned with the");
							fixPrint(4, 18, fontColorWhite, fbase, "one on the background, and the");
							fixPrint(4, 19, fontColorWhite, fbase, "offset in frames from the actual");
							fixPrint(4, 20, fontColorWhite, fbase, "intersection will be shown on");
							fixPrint(4, 21, fontColorWhite, fbase, "screen. A 1khz tone will be");
							fixPrint(4, 22, fontColorWhite, fbase, "played for 1 frame when pressed.");
							fixPrint(26, 24, fontColorYellow, fbase, "(cont...)");
						break;

						case 2:
							fixPrintC(6, fontColorGreen, fbase, "TIMING & REFLEX (2/2)");

							fixPrint(4, 8, fontColorWhite, fbase, "Button 'B' can be used to");
							fixPrint(4, 9, fontColorWhite, fbase, "change the direction of the");
							fixPrint(4, 10, fontColorWhite, fbase, "sprite.");

							fixPrint(4, 12, fontColorWhite, fbase, "Of course the evaluation is");
							fixPrint(4, 13, fontColorWhite, fbase, "dependent on reflexes and/or");
							fixPrint(4, 14, fontColorWhite, fbase, "rhythm more than anything. The");
							fixPrint(4, 15, fontColorWhite, fbase, "visual and audio cues are the");
							fixPrint(4, 16, fontColorWhite, fbase, "more revealing aspects which");
							fixPrint(4, 17, fontColorWhite, fbase, "the user should consider, of");
							fixPrint(4, 18, fontColorWhite, fbase, "course the interactive factor");
							fixPrint(4, 19, fontColorWhite, fbase, "can give an experienced player");
							fixPrint(4, 20, fontColorWhite, fbase, "the hang of the system when");
							fixPrint(4, 21, fontColorWhite, fbase, "testing via different");
							fixPrint(4, 22, fontColorWhite, fbase, "connections.");
						break;
					}
				break;

				case HELP_HSCROLL:
					switch (page)
					{
						case 1:
							fixPrintC(6, fontColorGreen, fbase, "SCROLL TEST (1/2)");

							fixPrint(4, 9, fontColorWhite, fbase, "This test shows either an");
							fixPrint(4, 10, fontColorWhite, fbase, "horizontal 320x224 back from");
							fixPrint(4, 11, fontColorWhite, fbase, "Sonic or a vertical 256x224");
							fixPrint(4, 12, fontColorWhite, fbase, "back from Kiki Kaikai.");

							fixPrint(4, 14, fontColorWhite, fbase, "Speed can be varied with Up &");
							fixPrint(4, 15, fontColorWhite, fbase, "Down and scroll direction with");
							fixPrint(4, 16, fontColorWhite, fbase, "'C'. The 'A' button stops the");
							fixPrint(4, 17, fontColorWhite, fbase, "scroll and 'D' toggles between");
							fixPrint(4, 18, fontColorWhite, fbase, "vertical and horizontal.");

							fixPrint(4, 20, fontColorWhite, fbase, "You can use Left & Right while");
							fixPrint(4, 21, fontColorWhite, fbase, "using the vertical test to move");
							fixPrint(4, 22, fontColorWhite, fbase, "the scroll to each edge.");
							fixPrint(26, 24, fontColorYellow, fbase, "(cont...)");
						break;

						case 2:
							fixPrintC(6, fontColorGreen, fbase, "SCROLL TEST (2/2)");

							fixPrint(4, 9, fontColorWhite, fbase, "This can be used to notice any");
							fixPrint(4, 10, fontColorWhite, fbase, "drops in framerate, or pixel");
							fixPrint(4, 11, fontColorWhite, fbase, "width inconsistencies.");

							fixPrint(4, 13, fontColorWhite, fbase, "Sonic is a trademark of Sega");
							fixPrint(4, 14, fontColorWhite, fbase, "Enterprises Ltd. Kiki Kaikai");
							fixPrint(4, 15, fontColorWhite, fbase, "is a trademark of Taito.");
						break;
					}
				break;

					case HELP_GRIDSCROLL:
						fixPrintC(6, fontColorGreen, fbase, "GRID SCROLL TEST");

						fixPrint(4, 9, fontColorWhite, fbase, "A grid is scrolled vertically or");
						fixPrint(4, 10, fontColorWhite, fbase, "horizontally, which can be used");
						fixPrint(4, 11, fontColorWhite, fbase, "to test linearity of the signal");
						fixPrint(4, 12, fontColorWhite, fbase, "and how well the display or");
						fixPrint(4, 13, fontColorWhite, fbase, "video processor copes with");
						fixPrint(4, 14, fontColorWhite, fbase, "scrolling and framerate.");

						fixPrint(4, 16, fontColorWhite, fbase, "Button 'C' can be used to toggle");
						fixPrint(4, 17, fontColorWhite, fbase, "between horizontal and vertical,");
						fixPrint(4, 18, fontColorWhite, fbase, "while Up/Down regulate speed.");

						fixPrint(4, 20, fontColorWhite, fbase, "Button 'A' stops the scroll and");
						fixPrint(4, 21, fontColorWhite, fbase, "'Left' changes direction.");
					break;

					case HELP_SOUND:
						fixPrintC(6, fontColorGreen, fbase, "SOUND TEST");

						fixPrint(4, y++, fontColorWhite, fbase, "You can test the sound from the");
	#ifndef __cd__
						fixPrint(4, y++, fontColorWhite, fbase, "YM2610 FM, SSG, ADPCM-A and");
						fixPrint(4, y++, fontColorWhite, fbase, "ADPCM-B.");
	#else
						fixPrint(4, y++, fontColorWhite, fbase, "YM2610 FM, SSG, ADPCM-A and CDDA.");
	#endif // __cd__
						y++;
						fixPrint(4, y++, fontColorWhite, fbase, "Panning can be changed when");
						fixPrint(4, y++, fontColorWhite, fbase, "possible. This can help you");
						fixPrint(4, y++, fontColorWhite, fbase, "identify stereo cabling issues.");
						y++;
						fixPrint(4, y++, fontColorWhite, fbase, "SSG has 260hz and 1khz tones on");
						fixPrint(4, y++, fontColorWhite, fbase, "channels 0 and 1, and noise");
						fixPrint(4, y++, fontColorWhite, fbase, "at arounf 5khz in channel 2.");
						y++;
						if(IS_HARDWARE_MVS)
						{
							fixPrint(4, y++, fontColorWhite, fbase, "MV1C system have ADPCM-B stereo");
							fixPrint(4, y++, fontColorWhite, fbase, "reversed. You can fix this via");
							fixPrint(4, y++, fontColorWhite, fbase, "BIOS softdips.");
						}
					break;

				case HELP_LED:
					switch (page)
					{
						case 1:
							fixPrintC(6, fontColorGreen, fbase, "BACKLIT TEST");

							fixPrint(4, 9, fontColorWhite, fbase, "This test allows you to check");
							fixPrint(4, 10, fontColorWhite, fbase, "how the display's backlit works");
							fixPrint(4, 11, fontColorWhite, fbase, "when only a small array of");
							fixPrint(4, 12, fontColorWhite, fbase, "pixels is shown.");

							fixPrint(4, 14, fontColorWhite, fbase, "The user can move around the");
							fixPrint(4, 15, fontColorWhite, fbase, "white pixel arrays with the");
							fixPrint(4, 16, fontColorWhite, fbase, "d-pad, and change the size of");
							fixPrint(4, 17, fontColorWhite, fbase, "the pixel array with 'A'.");
							fixPrint(4, 18, fontColorWhite, fbase, "The 'D' button allows the user");
							fixPrint(4, 19, fontColorWhite, fbase, "to hide the pixel array in");
							fixPrint(4, 20, fontColorWhite, fbase, "order to alternate a fully black");
							fixPrint(4, 21, fontColorWhite, fbase, "screen. 'C' button toggles the");
							fixPrint(4, 22, fontColorWhite, fbase, "fast mode, where the sprite is");
							fixPrint(4, 23, fontColorWhite, fbase, "moved by its width per frame.");
						break;
					}
				break;

				case HELP_LAG:
					switch (page)
					{
						case 1:
							fixPrintC(6, fontColorGreen, fbase, "LAG TEST (1/2)");

							fixPrint(4, 9, fontColorWhite, fbase, "This test is designed to be used");
							fixPrint(4, 10, fontColorWhite, fbase, "with two displays conected at");
							fixPrint(4, 11, fontColorWhite, fbase, "the same time. One being a CRT,");
							fixPrint(4, 12, fontColorWhite, fbase, "or a display with a known lag as");
							fixPrint(4, 13, fontColorWhite, fbase, "reference, and the other the");
							fixPrint(4, 14, fontColorWhite, fbase, "display to test.");

							fixPrint(4, 16, fontColorWhite, fbase, "Using a camera, a picture should");
							fixPrint(4, 17, fontColorWhite, fbase, "be taken of both screens at the");
							fixPrint(4, 18, fontColorWhite, fbase, "same time. The picture will show");
							fixPrint(4, 19, fontColorWhite, fbase, "the frame discrepancy between");
							fixPrint(4, 20, fontColorWhite, fbase, "them.");

							fixPrint(4, 22, fontColorWhite, fbase, "The circles in the bottom help");
							fixPrint(4, 23, fontColorWhite, fbase, "determine the frame even when");
							fixPrint(26, 24, fontColorYellow, fbase, "(cont...)");
						break;

						case 2:
							fixPrintC(6, fontColorGreen, fbase, "LAG TEST (2/2)");

							fixPrint(4, 9, fontColorWhite, fbase, "the numbers are blurry.");

							fixPrint(4, 11, fontColorWhite, fbase, "This version of the suite can be");
							fixPrint(4, 12, fontColorWhite, fbase, "used with a Sega Nomad as the");
							fixPrint(4, 13, fontColorWhite, fbase, "reference display.");

							fixPrint(4, 15, fontColorWhite, fbase, "You can also split the video");
							fixPrint(4, 16, fontColorWhite, fbase, "signal and feed both displays.");

							fixPrint(4, 18, fontColorWhite, fbase, "The vertical bars on the sides");
							fixPrint(4, 19, fontColorWhite, fbase, "change color each frame to help");
							fixPrint(4, 20, fontColorWhite, fbase, "when using LCD photos.");

							fixPrint(4, 22, fontColorWhite, fbase, "Press A to start/stop, C to");
							fixPrint(4, 23, fontColorWhite, fbase, "reset when stopped.");
						break;
						}
				break;

				case HELP_AUDIOSYNC:
					fixPrintC(6, fontColorGreen, fbase, "AUDIO SYNC TEST");

					fixPrint(4, 9, fontColorWhite, fbase, "This test flashes the whole");
					fixPrint(4, 10, fontColorWhite, fbase, "screen white for 1 frames,");
					fixPrint(4, 11, fontColorWhite, fbase, "along with a 1khz test tone.");

					fixPrint(4, 13, fontColorWhite, fbase, "You can verify the sync between");
					fixPrint(4, 14, fontColorWhite, fbase, "audio and video with recording");
					fixPrint(4, 15, fontColorWhite, fbase, "equipment or specialized");
					fixPrint(4, 16, fontColorWhite, fbase, "hardware.");

					fixPrint(4, 18, fontColorWhite, fbase, "Press A to start/stop the test.");
				break;

				case HELP_CONTRAST:
					fixPrintC(6, fontColorGreen, fbase, "PLUGE CONTRAST");

					fixPrint(4, 9, fontColorWhite, fbase, "This sub test fills the screen");
					fixPrint(4, 10, fontColorWhite, fbase, "with a pattern. By default it");
					fixPrint(4, 11, fontColorWhite, fbase, "is drawn in color.");

					fixPrint(4, 12, fontColorWhite, fbase, "You can change palettes with");
					fixPrint(4, 13, fontColorWhite, fbase, "A to display a dark or white");
					fixPrint(4, 14, fontColorWhite, fbase, "pattern in order to check");
					fixPrint(4, 15, fontColorWhite, fbase, "contrast and brightness.");
				break;

				case HELP_MEMVIEW:
					fixPrintC(6, fontColorGreen, fbase, "MEMORY VIEWER");

					fixPrint(4, 9, fontColorWhite, fbase, "This shows selected regions of");
					fixPrint(4, 10, fontColorWhite, fbase, "the main CPU memory map.");

					fixPrint(4, 12, fontColorWhite, fbase, "The current address range is");
					fixPrint(4, 13, fontColorWhite, fbase, "shown in red at the right from");
					fixPrint(4, 14, fontColorWhite, fbase, "top to bottom.");

					fixPrint(4, 16, fontColorWhite, fbase, "The left part of the screen");
					fixPrint(4, 17, fontColorWhite, fbase, "shows 0x1C0 bytes in hex.");

					fixPrint(4, 19, fontColorWhite, fbase, "- Button A enabled CRC of the");
					fixPrint(4, 20, fontColorWhite, fbase, "current screen.");
					fixPrint(4, 21, fontColorWhite, fbase, "- Button C jumps to relevant");
					fixPrint(4, 22, fontColorWhite, fbase, "memory locations");
					fixPrint(4, 23, fontColorWhite, fbase, "- Button D toggles ASCII");
				break;
				case HELP_MDFOURIER:
				{
					picture qr;

					pictureInit(&qr, &qrmdf, index, 17, 248, 140, FLIP_NONE);
					palJobPut(17,qrmdf.palInfo->count,qrmdf.palInfo->data);

					fixPrintC(6, fontColorGreen, fbase, "MDFourier");

					fixPrint(4, y++, fontColorWhite, fbase, "Button A plays the test tones.");
					y++;
					fixPrint(4, y++, fontColorWhite, fbase, "Please record it with your audio");
					fixPrint(4, y++, fontColorWhite, fbase, "card and use the software at:");
					fixPrint(4, y++, fontColorYellow, fbase, "http://junkerhq.net/MDFourier");
					y++;
					fixPrint(4, y++, fontColorWhite, fbase, "It will generate plots with the");
					fixPrint(4, y++, fontColorWhite, fbase, "audio signature from this system");
					fixPrint(4, y++, fontColorWhite, fbase, "that can be compared to others.");
#ifndef __cd__					
					y++;
					fixPrint(4, y++, fontColorWhite, fbase, "NGCD compatible mode skips");
					fixPrint(4, y++, fontColorWhite, fbase, "sample playback.");
#endif
				}
				break;
				case HELP_HCFR:
					switch(page)
					{
					case 1:
					{
						picture qr;

						// URL https://sourceforge.net/p/hcfr/wiki/Home/
						pictureInit(&qr, &qrhcfr, index, 17, 250, 142, FLIP_NONE);
						palJobPut(17,qrhcfr.palInfo->count,qrhcfr.palInfo->data);

						fixPrintC(6, fontColorGreen, fbase, "HCFR PATTERNS (1/8)");

						fixPrint(4, y++, fontColorWhite, fbase, "The HCFR Colormeter software");
						fixPrint(4, y++, fontColorWhite, fbase, "allows for profiling and");
						fixPrint(4, y++, fontColorWhite, fbase, "calibration of displays using");
						fixPrint(4, y++, fontColorWhite, fbase, "the combination of a pattern");
						fixPrint(4, y++, fontColorWhite, fbase, "and a tristimulous colorimeter");
						fixPrint(4, y++, fontColorWhite, fbase, "to read and measure the outputs.");
						y++;
						fixPrint(4, y++, fontColorWhite, fbase, "Then you will have to change");
						fixPrint(4, y++, fontColorWhite, fbase, "settings on your display in");
						fixPrint(4, y++, fontColorWhite, fbase, "order to refine the results and");
						fixPrint(4, y++, fontColorWhite, fbase, "attempt to match the chosen");
						fixPrint(4, y++, fontColorWhite, fbase, "standard of color.");

						fixPrint(26, 24, fontColorYellow, fbase, "(cont...)");
					}
					break;
					case 2:
						fixPrintC(6, fontColorGreen, fbase, "HCFR PATTERNS (2/8)");

						fixPrint(4, y++, fontColorWhite, fbase, "The HCFR Colormeter software is ");
						fixPrint(4, y++, fontColorWhite, fbase, "at the heart of the system. It");
						fixPrint(4, y++, fontColorWhite, fbase, "interprets the results from the");
						fixPrint(4, y++, fontColorWhite, fbase, "colorimeter and provides CCIE");
						fixPrint(4, y++, fontColorWhite, fbase, "color differences (Delta E");
						fixPrint(4, y++, fontColorWhite, fbase, "values) chromaticity diagrams,");
						fixPrint(4, y++, fontColorWhite, fbase, "to read and measure the outputs.");
						fixPrint(4, y++, fontColorWhite, fbase, "curves and other values to");
						fixPrint(4, y++, fontColorWhite, fbase, "assist you in profiling and");
						fixPrint(4, y++, fontColorWhite, fbase, "calibrating your display.");
						y++;
						fixPrint(4, y++, fontColorWhite, fbase, "To set up HCFR Colormeter,");
						fixPrint(4, y++, fontColorWhite, fbase, "attach your colorimeter to your");

						fixPrint(26, 24, fontColorYellow, fbase, "(cont...)");
					break;

					case 3:
						fixPrintC(6, fontColorGreen, fbase, "HCFR PATTERNS (3/8)");

						fixPrint(4, y++, fontColorWhite, fbase, "computer, open the program,");
						fixPrint(4, y++, fontColorWhite, fbase, "click File -> New, and select");
						fixPrint(4, y++, fontColorWhite, fbase, "DVD Manual as your generator.");
						fixPrint(4, y++, fontColorWhite, fbase, "Select your colorimeter from");
						fixPrint(4, y++, fontColorWhite, fbase, "the list, and click OK.");
						y++;
						fixPrint(4, y++, fontColorWhite, fbase, "Once in HCFR Colormeter, click");
						fixPrint(4, y++, fontColorWhite, fbase, "Advance -> Preferences.");
						y++;
						fixPrint(4, y++, fontColorWhite, fbase, "Under the General tab, ensure");
						fixPrint(4, y++, fontColorWhite, fbase, "that the section Colors to add");
						fixPrint(4, y++, fontColorWhite, fbase, "when measuring primaries and");
						fixPrint(4, y++, fontColorWhite, fbase, "secondaries is set to White.");

						fixPrint(26, 24, fontColorYellow, fbase, "(cont...)");
					break;

					case 4:
						fixPrintC(6, fontColorGreen, fbase, "HCFR PATTERNS (4/8)");

						fixPrint(4, y++, fontColorWhite, fbase, "Under the References tab, choos");
						fixPrint(4, y++, fontColorWhite, fbase, "your color space. PAL/SECAM is");
						fixPrint(4, y++, fontColorWhite, fbase, "recommended for PAL CRT users.");
						fixPrint(4, y++, fontColorWhite, fbase, "SDTV - REC601 (NTSC) is");
						fixPrint(4, y++, fontColorWhite, fbase, "recommended for NTSC CRT users.");
						fixPrint(4, y++, fontColorWhite, fbase, "HD TV REC 709 is recommended for");
						fixPrint(4, y++, fontColorWhite, fbase, "HD and UHD/4K in SDR flat panel");
						fixPrint(4, y++, fontColorWhite, fbase, "TV users. sRGB is recommended");
						fixPrint(4, y++, fontColorWhite, fbase, "for PC VGA and up CRT monitors.");
						y++;
						fixPrint(4, y++, fontColorWhite, fbase, "White point is normally D65 for");
						fixPrint(4, y++, fontColorWhite, fbase, "western regions of all color");
						fixPrint(4, y++, fontColorWhite, fbase, "space types.");

						fixPrint(26, 24, fontColorYellow, fbase, "(cont...)");
					break;

					case 5:
						fixPrintC(6, fontColorGreen, fbase, "HCFR PATTERNS (5/8)");

						fixPrint(4, y++, fontColorWhite, fbase, "Japanese SDTV often used D93 as");
						fixPrint(4, y++, fontColorWhite, fbase, "a white point (which will appear");
						fixPrint(4, y++, fontColorWhite, fbase, "cooler, or bluer, than D65).");
						y++;
						fixPrint(4, y++, fontColorWhite, fbase, "Most gamma calculations normally");
						fixPrint(4, y++, fontColorWhite, fbase, "default to ITU-R BT.1886, which");
						fixPrint(4, y++, fontColorWhite, fbase, "uses Display Gamma Power Law 2.4");
						fixPrint(4, y++, fontColorWhite, fbase, "Your device may have a different");
						fixPrint(4, y++, fontColorWhite, fbase, "native gamma response, so choose");
						fixPrint(4, y++, fontColorWhite, fbase, "a different Power Law value if");
						fixPrint(4, y++, fontColorWhite, fbase, "required. sRGB has a specific");
						fixPrint(4, y++, fontColorWhite, fbase, "non-linear gamma that cannot be");
						fixPrint(4, y++, fontColorWhite, fbase, "customized.");

						fixPrint(26, 24, fontColorYellow, fbase, "(cont...)");
					break;

					case 6:
						fixPrintC(6, fontColorGreen, fbase, "HCFR PATTERNS (6/8)");

						fixPrint(4, y++, fontColorWhite, fbase, "Ensure your options chosen here");
						fixPrint(4, y++, fontColorWhite, fbase, "match the test pattern list you");
						fixPrint(4, y++, fontColorWhite, fbase, "choose in the 240p Test Suite.");
						y++;
						fixPrint(4, y++, fontColorWhite, fbase, "When you are ready to start");
						fixPrint(4, y++, fontColorWhite, fbase, "measuring, click the Measure");
						fixPrint(4, y++, fontColorWhite, fbase, "menu, and select your");
						fixPrint(4, y++, fontColorWhite, fbase, "measurement. If any large scale");
						fixPrint(4, y++, fontColorWhite, fbase, "changes are made to your display");
						fixPrint(4, y++, fontColorWhite, fbase, "it is recommended to re-run a");
						fixPrint(4, y++, fontColorWhite, fbase, "Gray-scale sweep often, so that");
						fixPrint(4, y++, fontColorWhite, fbase, "baseline calculations can be");
						fixPrint(4, y++, fontColorWhite, fbase, "accurately made for Delta E");
						fixPrint(4, y++, fontColorWhite, fbase, "measurements.");

						fixPrint(26, 24, fontColorYellow, fbase, "(cont...)");
					break;

					case 7:
						fixPrintC(6, fontColorGreen, fbase, "HCFR PATTERNS (7/8)");

						fixPrint(4, y++, fontColorWhite, fbase, "A menu will pop up asking you to");
						fixPrint(4, y++, fontColorWhite, fbase, "select a given DVD chapter and ");
						fixPrint(4, y++, fontColorWhite, fbase, "color name. Use the L and R");
						fixPrint(4, y++, fontColorWhite, fbase, "buttons to scroll through the");
						fixPrint(4, y++, fontColorWhite, fbase, "color list and select the color");
						fixPrint(4, y++, fontColorWhite, fbase, "desired. Once ready to measure,");
						fixPrint(4, y++, fontColorWhite, fbase, "click OK in HCFR Colormeter to");
						fixPrint(4, y++, fontColorWhite, fbase, "measure the test pattern.");
						y++;
						fixPrint(4, y++, fontColorWhite, fbase, "Use the Continuous measure mode");
						fixPrint(4, y++, fontColorWhite, fbase, "to be able to adjust your");

						fixPrint(26, 24, fontColorYellow, fbase, "(cont...)");
					break;

					case 8:
						fixPrintC(6, fontColorGreen, fbase, "HCFR PATTERNS (8/8)");

						fixPrint(4, y++, fontColorWhite, fbase, "display's settings to match the");
						fixPrint(4, y++, fontColorWhite, fbase, "values required for your chosen");
						fixPrint(4, y++, fontColorWhite, fbase, "color space, white point, gamma,");
						fixPrint(4, y++, fontColorWhite, fbase, "etc.");
						y++;
						fixPrint(4, y++, fontColorWhite, fbase, "Original data and testing");
						fixPrint(4, y++, fontColorWhite, fbase, "by Dan Mons and Keith Raney.");

						fixPrint(26, 24, fontColorYellow, fbase, "(cont...)");
					break;
					}
				break;
				case HELP_DISAPPEAR:
					fixPrintC(6, fontColorGreen, fbase, "DISAPPEARING LOGO");

					fixPrint(4, y++, fontColorWhite, fbase, "This test allows you to use a");
					fixPrint(4, y++, fontColorWhite, fbase, "slow motion camera to measure");
					fixPrint(4, y++, fontColorWhite, fbase, "how long it takes from a button");
					fixPrint(4, y++, fontColorWhite, fbase, "press to when the logo");
					fixPrint(4, y++, fontColorWhite, fbase, "disappears");
					y++;
					fixPrint(4, y++, fontColorWhite, fbase, "You can also use a photodiode");
					fixPrint(4, y++, fontColorWhite, fbase, "and press C, which turns");
					fixPrint(4, y++, fontColorWhite, fbase, "the whole back to white for");
					fixPrint(4, y++, fontColorWhite, fbase, "2 frames.");
				break;
				case HELP_Z80RAM:
					fixPrintC(6, fontColorGreen, fbase, "Z80 RAM TEST");

					fixPrint(4, y++, fontColorWhite, fbase, "This test instructs the Z80 CPU");
					fixPrint(4, y++, fontColorWhite, fbase, "to check its RAM. Of course if");
					fixPrint(4, y++, fontColorWhite, fbase, "the Z80 is non functional this");
					fixPrint(4, y++, fontColorWhite, fbase, "will fail.");
					y++;
					fixPrint(4, y++, fontColorWhite, fbase, "The test itself also uses some");
					fixPrint(4, y++, fontColorWhite, fbase, "RAM as heap.");
				break;
			default:
				exit = 1;
				break;
		}
		}

		SCClose();
		waitVBlank();

		readController();

		if (PRESSED_RIGHT)
		{
			if (page + 1 <= totalpages)
			{
				suiteClearFixLayer();
				page++;
				redraw = 1;
			}
		}

		if (PRESSED_LEFT)
		{
			if (page - 1 > 0)
			{
				suiteClearFixLayer();
				page--;
				redraw = 1;
			}
		}

		if (BTTN_EXIT || PRESSED_START)
		{
			exit = 1;
			gfxClear();
		}
	}
	clearController();
}