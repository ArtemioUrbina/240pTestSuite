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
#include "externs.h"
#include "help.h"

BYTE p1,p2,ps,p1e,p2e;

#define fontColorWhite 0
#define fontColorRed 1
#define fontColorGreen 2
#define fontColorBlue 3

void DrawHelp(int option)
{
	int exit = 0, page = 1, totalpages = 1;

	clearFixLayer();
	draw_background();

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
		totalpages = 2;
		break;
	case HELP_PLUGE:
	case HELP_MONOSCOPE:
		totalpages = 3;
		break;
	}

	while (!exit) 
	{
		SCClose();
		waitVBlank();

		switch (option)
			{
			case HELP_GENERAL:
				switch (page)
				{
				case 1:
					fixPrint(16, 6, fontColorGreen, 3, "HELP (1/2)");

					fixPrint(4, 8, fontColorWhite, 3, "The 240p Test Suite was designed");
					fixPrint(4, 9, fontColorWhite, 3, "with two goals in mind:");

					fixPrint(4, 11, fontColorWhite, 3, "1) Evaluate 240p signals on TV");
					fixPrint(4, 12, fontColorWhite, 3, "sets and video processors; and");

					fixPrint(4, 14, fontColorWhite, 3, "2) Provide calibration patterns");
					fixPrint(4, 15, fontColorWhite, 3, "from a game console to help in");
					fixPrint(4, 16, fontColorWhite, 3, "properly calibrating the display");
					fixPrint(4, 17, fontColorWhite, 3, "black, white, and color levels.");

					fixPrint(4, 19, fontColorGreen, 3, "Help can be shown with 'SELECT'");
					fixPrint(4, 20, fontColorGreen, 3, "in any test.");

					fixPrint(26, 22, fontColorWhite, 3, "(cont...)");

					fixPrint(7, 26, fontColorWhite, 3, "Press SELECT to exit help");

					break;
				case 2:
					fixPrint(16, 6, fontColorGreen, 3, "HELP (2/2)");

					fixPrint(4, 8, fontColorWhite, 3, "The Neo Geo can output 224");
					fixPrint(4, 9, fontColorWhite, 3, "active video lines. In PAL");
					fixPrint(4, 10, fontColorWhite, 3, "consoles, it can display either");
					fixPrint(4, 11,  fontColorWhite, 3, "224 or 240 lines.");

					fixPrint(4, 13, fontColorWhite, 3, "The 240p suite is also available");
					fixPrint(4, 14, fontColorWhite, 3, "on NES/FC, SNES/SFC, GameCube,");
					fixPrint(4, 15, fontColorWhite, 3, "Wii, GBA, Neo Geo MVS/AES and CD,");
					fixPrint(4, 16, fontColorWhite, 3, "Dreamcast, Genesis/Mega Drive,");
					fixPrint(4, 17, fontColorWhite, 3, "Sega/Mega CD, SMS, and");
					fixPrint(4, 18, fontColorWhite, 3, "PCE/TG-16/PCE-Duo/SCD/SCD2");

					fixPrint(4, 20, fontColorWhite, 3, "Visit:");
					fixPrint(4, 21, fontColorGreen, 3, "http://junkerhq.net/240p");
					fixPrint(4, 22, fontColorWhite, 3, "for more information");

					fixPrint(7, 26, fontColorWhite, 3, "Press SELECT to exit help");

					break;
				}
				break;
			case HELP_PLUGE:
				switch (page)
				{
				case 1:
					fixPrint(15, 6, fontColorGreen, 3, "PLUGE (1/3)");

					fixPrint(4, 9, fontColorWhite, 3, "NTSC levels require black to be");
					fixPrint(4, 10, fontColorWhite, 3, "at 7.5 IRE for video. This HW");
					fixPrint(4, 11, fontColorWhite, 3, "lowest is 6 IRE (6%), so using");
					fixPrint(4, 12, fontColorWhite, 3, "this value for general 240p use");
					fixPrint(4, 13, fontColorWhite, 3, "is not recommended.");

					fixPrint(4, 15, fontColorWhite, 3, "Of course using it as reference");
					fixPrint(4, 16, fontColorWhite, 3, "will work perfectly for games");
					fixPrint(4, 17, fontColorWhite, 3, "in this platform.");

					fixPrint(4, 19, fontColorWhite, 3, "In PAL - and console gaming in");
					fixPrint(4, 20, fontColorWhite, 3, "general - it is adviced to use");
					fixPrint(4, 21, fontColorWhite, 3, "a value of 2 IRE as black.");

					fixPrint(26, 24, fontColorWhite, 3, "(cont...)");
					break;
				case 2:
					fixPrint(12, 6, fontColorGreen, 3, "PLUGE (2/3)");

					fixPrint(4, 9, fontColorWhite, 3, "The PLUGE pattern is used to");
					fixPrint(4, 10, fontColorWhite, 3, "help adjust the black level to");
					fixPrint(4, 11, fontColorWhite, 3, "a correct value.");

					fixPrint(4, 13, fontColorWhite, 3, "The inner bars on the sides are");
					fixPrint(4, 14, fontColorWhite, 3, "black at 6%, the outer at 12%.");
					fixPrint(4, 15, fontColorWhite, 3, "If these bars are not visible,");
					fixPrint(4, 16, fontColorWhite, 3, "adjust the \"brightness\" control");
					fixPrint(4, 17, fontColorWhite, 3, "until they are.");

					fixPrint(4, 19, fontColorWhite, 3, "You should lower it until they");
					fixPrint(4, 20, fontColorWhite, 3, "are not visible, and raise it");
					fixPrint(4, 21, fontColorWhite, 3, "until they show.");

					fixPrint(26, 24, fontColorWhite, 3, "(cont...)");
					break;
				case 3:
					fixPrint(12, 6, fontColorGreen, 3, "PLUGE (3/3)");

					fixPrint(4, 9, fontColorWhite, 3, "Within it A button changes");
					fixPrint(4, 10, fontColorWhite, 3, "palettes between the original,");
					fixPrint(4, 11, fontColorWhite, 3, "highest and lowest values the");
					fixPrint(4, 12, fontColorWhite, 3, "hardware can display.");
					break;
				}
				break;
			case HELP_COLORS:
				fixPrint(15, 6, fontColorGreen, 3, "COLORBARS");

				fixPrint(4, 9, fontColorWhite, 3, "This pattern allows you to");
				fixPrint(4, 10, fontColorWhite, 3, "calibrate each color: Red, Green");
				fixPrint(4, 11, fontColorWhite, 3, "and Blue; as well as white.");

				fixPrint(4, 13, fontColorWhite, 3, "Adjust the white level first,");
				fixPrint(4, 14, fontColorWhite, 3, "using the \"Contrast\" control");
				fixPrint(4, 15, fontColorWhite, 3, "on your TV set. Raise it until");
				fixPrint(4, 16, fontColorWhite, 3, "you cannot distinguish between");
				fixPrint(4, 17, fontColorWhite, 3, "the blocks under \"E\" and \"F\",");
				fixPrint(4, 18, fontColorWhite, 3, "and lower it slowly until you");
				fixPrint(4, 19, fontColorWhite, 3, "can clearly tell them appart.");

				fixPrint(4, 21, fontColorWhite, 3, "Do the same for each color.");
				break;
			case HELP_GRID:
				fixPrint(17, 6, fontColorGreen, 3, "GRID");

				fixPrint(4, 9, fontColorWhite, 3, "This grid uses the full 40x224");
				fixPrint(4, 10, fontColorWhite, 3, "resolution.");

				fixPrint(4, 12, fontColorWhite, 3, "You can use it to verify that");
				fixPrint(4, 13, fontColorWhite, 3, "all the visible area is being");
				fixPrint(4, 14, fontColorWhite, 3, "displayed, and that there is no");
				fixPrint(4, 15, fontColorWhite, 3, "distortion present.");

				fixPrint(4, 17, fontColorWhite, 3, "The full active video signal can");
				fixPrint(4, 18, fontColorWhite, 3, "be filled with gray by pressing");
				fixPrint(4, 19, fontColorWhite, 3, "the 'A' button.");
				break;
			case HELP_MONOSCOPE:
				switch (page)
				{
					case 1:
						fixPrint(9, 6, fontColorGreen, 3, "MONOSCOPE (1/3)");

						fixPrint(4, 9, fontColorWhite, 3, "This pattern contains elements");
						fixPrint(4, 10, fontColorWhite, 3, "to calibrate multiple aspects");
						fixPrint(4, 11, fontColorWhite, 3, "of a CRT.");

						fixPrint(4, 13, fontColorWhite, 3, "Read your monitor's service");
						fixPrint(4, 14, fontColorWhite, 3, "manual to learn how, and use");
						fixPrint(4, 15, fontColorWhite, 3, "'A' button to change IRE.");

						fixPrint(4, 17, fontColorWhite, 3, "Brightness adjustment: Adjust");
						fixPrint(4, 18, fontColorWhite, 3, "convergence at low brightness");
						fixPrint(4, 19, fontColorWhite, 3, "(13/25 IRE). An overly bright");
						fixPrint(4, 20, fontColorWhite, 3, "pattern can mask convergence.");
						fixPrint(4, 21, fontColorWhite, 3, "issues.");

						fixPrint(26, 24, fontColorWhite, 3, "(cont...)");
					break;
					case 2:
						fixPrint(9, 6, fontColorGreen, 3, "MONOSCOPE (2/3)");

						fixPrint(4, 9, fontColorWhite, 3, "Convergence: Use the center");
						fixPrint(4, 10, fontColorWhite, 3, "crosshair to check static");
						fixPrint(4, 11, fontColorWhite, 3, "(center of screen) convergence.");
						fixPrint(4, 12, fontColorWhite, 3, "Use the patterns at the sides");
						fixPrint(4, 13, fontColorWhite, 3, "to check dynamic (edge)");
						fixPrint(4, 14, fontColorWhite, 3, "convergence.");

						fixPrint(4, 16, fontColorWhite, 3, "Corners: After setting center");
						fixPrint(4, 17, fontColorWhite, 3, "and edge convergence, use");
						fixPrint(4, 18, fontColorWhite, 3, "magnets to adjust corner purity");
						fixPrint(4, 19, fontColorWhite, 3, "and geometry.");

						fixPrint(26, 24, fontColorWhite, 3, "(cont...)");
					break;
					case 3:
						fixPrint(9, 6, fontColorGreen, 3, "MONOSCOPE (3/3)");

						fixPrint(4, 9, fontColorWhite, 3, "Size and aspect ratio: If");
						fixPrint(4, 10, fontColorWhite, 3, "vertical and horizontal size are");
						fixPrint(4, 11, fontColorWhite, 3, "correct, the red squares in the");
						fixPrint(4, 12, fontColorWhite, 3, "pattern will be perfect squares.");
						fixPrint(4, 13, fontColorWhite, 3, "After setting H size, use a tape");
						fixPrint(4, 14, fontColorWhite, 3, "measure to adjust V size to");
						fixPrint(4, 15, fontColorWhite, 3, "match it.");

						fixPrint(4, 17, fontColorWhite, 3, "Linearity: The squares in each");
						fixPrint(4, 18, fontColorWhite, 3, "corner should get you started.");
						fixPrint(4, 19, fontColorWhite, 3, "Confirm your adjustment using");
						fixPrint(4, 20, fontColorWhite, 3, "the scroll tests.");

						fixPrint(4, 21, fontColorWhite, 3, "Designed by Keith Raney.");
					break;
				}
				break;
			case HELP_BLEED:
				fixPrint(12, 6, fontColorGreen, 3, "COLOR BLEED");

				fixPrint(4, 9, fontColorWhite, 3, "This pattern helps diagnose");
				fixPrint(4, 10, fontColorWhite, 3, "color bleed caused by");
				fixPrint(4, 11, fontColorWhite, 3, "unneeded color upsampling.");

				fixPrint(4, 13, fontColorWhite, 3, "You can toggle between");
				fixPrint(4, 14, fontColorWhite, 3, "vertical bars and");
				fixPrint(4, 15, fontColorWhite, 3, "checkerboard with 'A'.");
				break;
			case HELP_IRE:
				fixPrint(16, 6, fontColorGreen, 3, "100 IRE");

				fixPrint(4, 9, fontColorWhite, 3, "You can vary IRE intensity");
				fixPrint(4, 10, fontColorWhite, 3, "with 'A' and 'B'. Values are:");
				fixPrint(4, 11, fontColorWhite, 3, "13, 25, 41, 53, 66, 82, 94");
				break;
			case HELP_601CB:
				fixPrint(14, 6, fontColorGreen, 3, "EBU COLORBARS");

				fixPrint(4, 9, fontColorWhite, 3, "You can use color filters or the");
				fixPrint(4, 10, fontColorWhite, 3, "blue only option in your display");
				fixPrint(4, 11, fontColorWhite, 3, "to confirm color balance.");
				break;
			case HELP_SHARPNESS:
				fixPrint(14, 6, fontColorGreen, 3, "SHARPNESS");

				fixPrint(4, 9, fontColorWhite, 3, "You should set the sharpness of");
				fixPrint(4, 10, fontColorWhite, 3, "your CRT to a value that shows");
				fixPrint(4, 11, fontColorWhite, 3, "clean black and gray transitions");
				fixPrint(4, 12, fontColorWhite, 3, "with no white ghosting between.");

				fixPrint(4, 13, fontColorWhite, 3, "Pressing 'A' toggles a diagonal");
				fixPrint(4, 14, fontColorWhite, 3, "brick tileset.");
				break;
			case HELP_OVERSCAN:
				fixPrint(15, 6, fontColorGreen, 3, "OVERSCAN");

				fixPrint(4, 9, fontColorWhite, 3, "With this pattern you can");
				fixPrint(4, 10, fontColorWhite, 3, "interactively find out the");
				fixPrint(4, 11, fontColorWhite, 3, "overscan in pixels of each edge");
				fixPrint(4, 12, fontColorWhite, 3, "in a display.");

				fixPrint(4, 14, fontColorWhite, 3, "Use left and right to increment");
				fixPrint(4, 15, fontColorWhite, 3, "the overscan until you see the");
				fixPrint(4, 16, fontColorWhite, 3, "white border, then go back one");
				fixPrint(4, 17, fontColorWhite, 3, "pixel. The resulting number is");
				fixPrint(4, 18, fontColorWhite, 3, "the amount of overscan in pixels");
				fixPrint(4, 19, fontColorWhite, 3, "in each direction.");
				break;
			case HELP_SMPTE:
				fixPrint(12, 6, fontColorGreen, 3, "SMPTE COLOR BARS");

				fixPrint(4, 9, fontColorWhite, 3, "This pattern can be used to");
				fixPrint(4, 10, fontColorWhite, 3, "approximate for NTSC levels");
				fixPrint(4, 11, fontColorWhite, 3, "regarding contrast,");
				fixPrint(4, 12, fontColorWhite, 3, "brightness and colors.");

				fixPrint(4, 14, fontColorWhite, 3, "You can toggle between 75% and");
				fixPrint(4, 15, fontColorWhite, 3, "100% SMPTE color bars with A.");
				fixPrint(4, 16, fontColorWhite, 3, "Of course the percentages are");
				fixPrint(4, 17, fontColorWhite, 3, "relative to the console output.");

				fixPrint(4, 19, fontColorWhite, 3, "You can use color filters or the");
				fixPrint(4, 20, fontColorWhite, 3, "blue only option in your display");
				fixPrint(4, 21, fontColorWhite, 3, "to confirm color balance");
				break;
			case HELP_GRAY:
				fixPrint(14, 6, fontColorGreen, 3, "GRAY RAMP");

				fixPrint(4, 9, fontColorWhite, 3, "This gray ramp pattern can be");
				fixPrint(4, 10, fontColorWhite, 3, "used to check color balance.");

				fixPrint(4, 12, fontColorWhite, 3, "You should make sure the bars");
				fixPrint(4, 13, fontColorWhite, 3, "are gray, with no color bias.");
				break;
			case HELP_WHITE:
				fixPrint(11, 6, fontColorGreen, 3, "WHITE SCREEN");

				fixPrint(4, 9, fontColorWhite, 3, "This pattern can be changed");
				fixPrint(4, 10, fontColorWhite, 3, "between white, black, red,");
				fixPrint(4, 11, fontColorWhite, 3, "green and blue screens with the");
				fixPrint(4, 12, fontColorWhite, 3, "'A' and 'B' buttons.");

				fixPrint(4, 14, fontColorWhite, 3, "Some displays and scalers have");
				fixPrint(4, 15, fontColorWhite, 3, "issues when changing between a");
				fixPrint(4, 16, fontColorWhite, 3, "black 0 IRE and a white screen.");

				fixPrint(4, 18, fontColorWhite, 3, "A custom color mode is");
				fixPrint(4, 19, fontColorWhite, 3, "available by pressing 'C'.");
				break;
			case HELP_CONVERGENCE:
				fixPrint(7, 6, fontColorGreen, 3, "CONVERGENCE TESTS");

				fixPrint(4, 9, fontColorWhite, 3, "These are used to adjust color");
				fixPrint(4, 10, fontColorWhite, 3, "convergence in CRT displays");

				fixPrint(4, 12, fontColorWhite, 3, "The 'A' button changes the cross");
				fixPrint(4, 13, fontColorWhite, 3, "hatch pattern between lines,");
				fixPrint(4, 14, fontColorWhite, 3, "dots and crosses");

				fixPrint(4, 16, fontColorWhite, 3, "The 'B' button changes to a");
				fixPrint(4, 17, fontColorWhite, 3, "color pattern for transition");
				fixPrint(4, 18, fontColorWhite, 3, "boundary check.");
				break;
			case HELP_CHECK:
				fixPrint(11, 6, fontColorGreen, 3, "CHECKERBOARD");

				fixPrint(4, 9, fontColorWhite, 3, "This pattern shows all the");
				fixPrint(4, 10, fontColorWhite, 3, "visible pixels in an");
				fixPrint(4, 11, fontColorWhite, 3, "alternating white and black");
				fixPrint(4, 12, fontColorWhite, 3, "grid array.");

				fixPrint(4, 14, fontColorWhite, 3, "You can toggle the pattern with");
				fixPrint(4, 15, fontColorWhite, 3, "button 'Up', or turn on");
				fixPrint(4, 16, fontColorWhite, 3, "auto-toggle each frame with the");
				fixPrint(4, 17, fontColorWhite, 3, "'A' button. A frame counter is");
				fixPrint(4, 18, fontColorWhite, 3, "also available with 'B'.");
				break;
			case HELP_STRIPES:
				switch (page)
				{
				case 1:
					/* fixPrint("HOR/VER STRIPES (1/2)", 6, 6, fontColorGreen);

					fixPrint("You should see a pattern of", 4, 57, fontColorWhite);
					fixPrint("lines, each one pixel in height", 4, 65, fontColorWhite);
					fixPrint("starting with a white one.", 4, 73, fontColorWhite);

					fixPrint("You can toggle the pattern with", 4, 90, fontColorWhite);
					fixPrint("button 'UP', or turn on", 4, 98, fontColorWhite);
					fixPrint("auto-toggle each frame with the", 4, 106, fontColorWhite);
					fixPrint("'A' button. A frame counter is", 4, 114, fontColorWhite);
					fixPrint("also available with 'B'.", 4, 122, fontColorWhite);

					fixPrint("When auto-toggle is set, you", 4, 139, fontColorWhite);
					fixPrint("should see the lines", 4, 147, fontColorWhite);
					fixPrint("alternating rapidly.", 4, 155, fontColorWhite);
					fixPrint("(cont...)", 207, 178, fontColorWhite); */
					break;
				case 2:
					/* fixPrint("HOR/VER STRIPES (2/2)", 6, 6, fontColorGreen);

					fixPrint("You can also display vertical", 4, 57, fontColorWhite);
					fixPrint("bars by pressing 'LEFT'. That", 4, 65, fontColorWhite);
					fixPrint("pattern will help you", 4, 73, fontColorWhite);
					fixPrint("evaluate if the signal is not", 4, 81, fontColorWhite);
					fixPrint("distorted horizontaly, since", 4, 89, fontColorWhite);
					fixPrint("all lines should be one pixel", 4, 97, fontColorWhite);
					fixPrint("wide.", 4, 105, fontColorWhite); */
					break;
				}
				break;
			case HELP_SHADOW:
				fixPrint(7, 6, fontColorGreen, 3, "DROP SHADOW TEST");

				fixPrint(4, 9, fontColorWhite, 3, "This is a crucial test for 240p");
				fixPrint(4, 10, fontColorWhite, 3, "It displays a simple sprite");
				fixPrint(4, 11, fontColorWhite, 3, "shadow against a background,");
				fixPrint(4, 12, fontColorWhite, 3, "but the shadow is shown only");
				fixPrint(4, 13, fontColorWhite, 3, "on each other frame. Achieving");
				fixPrint(4, 14, fontColorWhite, 3, "a transparency effect.");

				fixPrint(4, 16, fontColorWhite, 3, "The user can toggle the frame");
				fixPrint(4, 17, fontColorWhite, 3, "used to draw the shadow with");
				fixPrint(4, 18, fontColorWhite, 3, "button 'A'. Backgrounds can be");
				fixPrint(4, 19, fontColorWhite, 3, "switched with the 'B' button,");
				fixPrint(4, 20, fontColorWhite, 3, "and button 'C' toggles sprites.");
				break;
			case HELP_STRIPED:
				/* fixPrint("STRIPED SPRITE TEST", 7, 6, fontColorGreen);

				fixPrint("There are deinterlacers out", 4, 57, fontColorWhite);
				fixPrint("there that can display the drop", 4, 65, fontColorWhite);
				fixPrint("shadows correctly and still", 4, 73, fontColorWhite);
				fixPrint("interpret 240p as 480i. With a", 4, 81, fontColorWhite);
				fixPrint("striped sprite it should be easy", 4, 89, fontColorWhite);
				fixPrint("to tell if a processor tries to", 4, 97, fontColorWhite);
				fixPrint("deinterlace (plus interpolate).", 4, 105, fontColorWhite);

				fixPrint("You can change backgrounds with", 4, 122, fontColorWhite);
				fixPrint("'A'.", 4, 130, fontColorWhite); */
				break;
			case HELP_MANUALLAG:
				switch (page)
				{
				case 1:
					/* fixPrint("TIMING & REFLEX (1/2)", 6, 6, fontColorGreen);

					fixPrint("The main intention is to show a", 4, 56, fontColorWhite);
					fixPrint("changing pattern on the screen,", 4, 64, fontColorWhite);
					fixPrint("which can be complemented with", 4, 72, fontColorWhite);
					fixPrint("audio. This should show to some", 4, 80, fontColorWhite);
					fixPrint("degree any lag when processing", 4, 88, fontColorWhite);
					fixPrint("the signal.", 4, 96, fontColorWhite);

					fixPrint("As an added feature, the user", 4, 112, fontColorWhite);
					fixPrint("can click the 'A' button when", 4, 120, fontColorWhite);
					fixPrint("the sprite is aligned with the", 4, 128, fontColorWhite);
					fixPrint("one on the background, and the", 4, 136, fontColorWhite);
					fixPrint("offset in frames from the actual", 4, 144, fontColorWhite);
					fixPrint("intersection will be shown on", 4, 152, fontColorWhite);
					fixPrint("screen. A 1khz tone will be", 4, 160, fontColorWhite);
					fixPrint("played for 1 frame when pressed.", 4, 168, fontColorWhite);
					fixPrint("(cont...)", 207, 178, fontColorWhite); */
					break;
				case 2:
					/* fixPrint("TIMING & REFLEX (2/2)", 6, 6, fontColorGreen);

					fixPrint("Button 'B' can be used to", 4, 56, fontColorWhite);
					fixPrint("change the direction of the", 4, 64, fontColorWhite);
					fixPrint("sprite.", 4, 72, fontColorWhite);

					fixPrint("Of course the evaluation is", 4, 88, fontColorWhite);
					fixPrint("dependent on reflexes and/or", 4, 96, fontColorWhite);
					fixPrint("rhythm more than anything. The", 4, 104, fontColorWhite);
					fixPrint("visual and audio cues are the", 4, 112, fontColorWhite);
					fixPrint("more revealing aspects which", 4, 120, fontColorWhite);
					fixPrint("the user should consider, of", 4, 128, fontColorWhite);
					fixPrint("course the interactive factor", 4, 136, fontColorWhite);
					fixPrint("can give an experienced player", 4, 144, fontColorWhite);
					fixPrint("the hang of the system when", 4, 152, fontColorWhite);
					fixPrint("testing via different", 4, 160, fontColorWhite);
					fixPrint("connections.", 4, 168, fontColorWhite); */
					break;
				}
				break;
			case HELP_HSCROLL:
				fixPrint(11, 6, fontColorGreen, 3, "SCROLL TEST");

				fixPrint(4, 9, fontColorWhite, 3, "This test shows either an");
				fixPrint(4, 10, fontColorWhite, 3, "horizontal 40x224 back from");
				fixPrint(4, 11, fontColorWhite, 3, "Sonic or a vertical 256x224");
				fixPrint(4, 12, fontColorWhite, 3, "back from Kiki Kaikai.");

				fixPrint(4, 14, fontColorWhite, 3, "Speed can be varied with Up &");
				fixPrint(4, 15, fontColorWhite, 3, "Down and scroll direction with");
				fixPrint(4, 16, fontColorWhite, 3, "Left. The 'A' button stops the");
				fixPrint(4, 17, fontColorWhite, 3, "scroll and 'B' toggles between");
				fixPrint(4, 18, fontColorWhite, 3, "vertical and horizontal.");

				fixPrint(4, 20, fontColorWhite, 3, "This can be used to notice any");
				fixPrint(4, 21, fontColorWhite, 3, "drops in framerate, or pixel");
				fixPrint(4, 22, fontColorWhite, 3, "width inconsistencies.");
				break;
			case HELP_VSCROLL:
				fixPrint(10, 6, fontColorGreen, 3, "GRID SCROLL TEST");

				fixPrint(4, 9, fontColorWhite, 3, "A grid is scrolled vertically or");
				fixPrint(4, 10, fontColorWhite, 3, "horizontally, which can be used");
				fixPrint(4, 11, fontColorWhite, 3, "to test linearity of the signal");
				fixPrint(4, 12, fontColorWhite, 3, "and how well the display or");
				fixPrint(4, 13, fontColorWhite, 3, "video processor copes with");
				fixPrint(4, 14, fontColorWhite, 3, "scrolling and framerate.");

				fixPrint(4, 16, fontColorWhite, 3, "Button 'B' can be used to toggle");
				fixPrint(4, 17, fontColorWhite, 3, "between horizontal and vertical,");
				fixPrint(4, 18, fontColorWhite, 3, "while Up/Down regulate speed.");

				fixPrint(4, 20, fontColorWhite, 3, "Button 'A' stops the scroll and");
				fixPrint(4, 21, fontColorWhite, 3, "'Left' changes direction.");
				break;
			case HELP_SOUND:
				/* fixPrint("SOUND TEST", 12, 6, fontColorGreen);

				fixPrint("You can test the sound from the", 4, 56, fontColorWhite);
				fixPrint("4X 2-channel PWM and", 4, 64, fontColorWhite);
				fixPrint("Genesis/Mega Drive PSG here.", 4, 72, fontColorWhite);

				fixPrint("Panning can be changed when", 4, 88, fontColorWhite);
				fixPrint("possible. This can help you", 4, 96, fontColorWhite);
				fixPrint("identify stereo cabling issues.", 4, 104, fontColorWhite);

				fixPrint("PSG has 200hz, 2khz and 4khz", 4, 120, fontColorWhite);
				fixPrint("tones on its channels and white", 4, 128, fontColorWhite);
				fixPrint("noise at 500hz.", 4, 136, fontColorWhite); */
				break;
		case HELP_LED:
				switch (page)
				{
				case 1:
					/* fixPrint("BACKLIT TEST", 12, 6, fontColorGreen);

					fixPrint("This test allows you to check", 4, 57, fontColorWhite);
					fixPrint("how the display's backlit works", 4, 65, fontColorWhite);
					fixPrint("when only a small array of", 4, 73, fontColorWhite);
					fixPrint("pixels is shown.", 4, 81, fontColorWhite);

					fixPrint("The user can move around the", 4, 97, fontColorWhite);
					fixPrint("white pixel arrays with the", 4, 105, fontColorWhite);
					fixPrint("d-pad, and change the size of", 4, 113, fontColorWhite);
					fixPrint("the pixel array with 'A'.", 4, 121, fontColorWhite);
					fixPrint("The 'B' button allows the user", 4, 129, fontColorWhite);
					fixPrint("to hide the pixel array in", 4, 137, fontColorWhite);
					fixPrint("order to alternate a fully black", 4, 145, fontColorWhite);
					fixPrint("screen.", 4, 153, fontColorWhite); */
					break;
				}
				break;
			case HELP_LAG:
				switch (page)
				{
				case 1:
					/* fixPrint("LAG TEST (1/2)", 11, 6, fontColorGreen);

					fixPrint("This test is designed to be used", 4, 57, fontColorWhite);
					fixPrint("with two displays conected at", 4, 65, fontColorWhite);
					fixPrint("the same time. One being a CRT,", 4, 73, fontColorWhite);
					fixPrint("or a display with a known lag as", 4, 81, fontColorWhite);
					fixPrint("reference, and the other the", 4, 89, fontColorWhite);
					fixPrint("display to test.", 4, 97, fontColorWhite);

					fixPrint("Using a camera, a picture should", 4, 114, fontColorWhite);
					fixPrint("be taken of both screens at the", 4, 122, fontColorWhite);
					fixPrint("same time. The picture will show", 4, 130, fontColorWhite);
					fixPrint("the frame discrepancy between", 4, 138, fontColorWhite);
					fixPrint("them.", 4, 146, fontColorWhite);

					fixPrint("The circles in the bottom help", 4, 163, fontColorWhite);
					fixPrint("determine the frame even when", 4, 171, fontColorWhite);
					fixPrint("(cont...)", 207, 178, fontColorWhite); */
					break;
				case 2:
					/* fixPrint("LAG TEST (2/2)", 11, 6, fontColorGreen);

					fixPrint("the numbers are blurry.", 4, 57, fontColorWhite);

					fixPrint("This version of the suite can be", 4, 74, fontColorWhite);
					fixPrint("used with a Sega Nomad as the", 4, 82, fontColorWhite);
					fixPrint("reference display.", 4, 90, fontColorWhite);

					fixPrint("You can also split the video", 4, 107, fontColorWhite);
					fixPrint("signal and feed both displays.", 4, 115, fontColorWhite);

					fixPrint("The vertical bars on the sides", 4, 14, fontColorWhite);
					fixPrint("change color each frame to help", 4, 140, fontColorWhite);
					fixPrint("when using LCD photos.", 4, 148, fontColorWhite);

					fixPrint("Press A to start/stop, B to", 4, 165, fontColorWhite);
					fixPrint("reset and C for B&W test.", 4, 173, fontColorWhite); */
					break;
				}
				break;
			case HELP_ALTERNATE:
				/* fixPrint("ALTERNATE 240p/480i", 7, 6, fontColorGreen);

				fixPrint("Some devices have a delay when", 4, 57, fontColorWhite);
				fixPrint("the source changes between", 4, 65, fontColorWhite);
				fixPrint("resolutions, which happens in", 4, 73, fontColorWhite);
				fixPrint("some games. This test allows to", 4, 81, fontColorWhite);
				fixPrint("time that delay manually.", 4, 89, fontColorWhite);

				fixPrint("Press A to switch the", 4, 106, fontColorWhite);
				fixPrint("resolution; press it again when", 4, 114, fontColorWhite);
				fixPrint("you are able to see the screen", 4, 122, fontColorWhite);
				fixPrint("back in your display.", 4, 130, fontColorWhite); */
				break;
			case HELP_VIDEO:
				/* fixPrint("VIDEO OPTIONS", 10, 6, fontColorGreen);

				fixPrint("Here you can enable interlaced", 4, 57, fontColorWhite);
				fixPrint("(480i) mode for the whole suite", 4, 65, fontColorWhite);
				fixPrint("and disable horizontal 256 in", 4, 73, fontColorWhite);
				fixPrint("compatible screens.", 4, 81, fontColorWhite);

				fixPrint("For PAL consoles, you can also", 4, 98, fontColorWhite);
				fixPrint("enable full 240p, instead of", 4, 106, fontColorWhite);
				fixPrint("224p.", 4, 114, fontColorWhite); */
				break;
			case HELP_AUDIOSYNC:
				/* fixPrint("AUDIO SYNC", 14, 6, fontColorGreen);

				fixPrint("This test flashes the whole", 4, 57, fontColorWhite);
				fixPrint("screen white for 2 frames,", 4, 65, fontColorWhite);
				fixPrint("along with a 1khz test tone.", 4, 73, fontColorWhite);

				fixPrint("You can verify the sync between", 4, 90, fontColorWhite);
				fixPrint("audio and video with recording", 4, 98, fontColorWhite);
				fixPrint("equipment or specialized", 4, 106, fontColorWhite);
				fixPrint("hardware.", 4, 114, fontColorWhite);

				fixPrint("Press A to start/stop the test.", 4, 131, fontColorWhite); */
				break;
			case HELP_CONTRAST:
				/* fixPrint("PLUGE CONTRAST", 11, 6, fontColorGreen);

				fixPrint("This sub test fills the screen", 4, 57, fontColorWhite);
				fixPrint("with a pattern. By default it", 4, 65, fontColorWhite);
				fixPrint("is drawn in color.", 4, 73, fontColorWhite);

				fixPrint("You can change palettes with", 4, 90, fontColorWhite);
				fixPrint("A to display a dark or white", 4, 98, fontColorWhite);
				fixPrint("pattern in order to check", 4, 106, fontColorWhite);
				fixPrint("contrast and brightness.", 4, 114, fontColorWhite);

				fixPrint("The C button returns to PLUGE.", 4, 131, fontColorWhite); */
				break;
			case HELP_MEMVIEW:
				fixPrint(11, 6, fontColorGreen, 3, "MEMORY VIEWER");

				fixPrint(4, 9, fontColorWhite, 3, "This shows selected regions of");
				fixPrint(4, 10, fontColorWhite, 3, "the main CPU memory map.");

				fixPrint(4, 12, fontColorWhite, 3, "The current address range is");
				fixPrint(4, 13, fontColorWhite, 3, "shown in red at the right from");
				fixPrint(4, 14, fontColorWhite, 3, "top to bottom.");

				fixPrint(4, 16, fontColorWhite, 3, "The left part of the screen");
				fixPrint(4, 17, fontColorWhite, 3, "shows 0x1C0 bytes in hex.");

				fixPrint(4, 19, fontColorWhite, 3, "- Button A enabled CRC of the");
				fixPrint(4, 20, fontColorWhite, 3, "current screen.");
				fixPrint(4, 21, fontColorWhite, 3, "- Button B jumps to relevant");
				fixPrint(4, 22, fontColorWhite, 3, "memory locations");
				break;
		default:
			exit = 1;
			break;
	}

		p1 = volMEMBYTE(P1_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);

		if (p1e & JOY_RIGHT)
		{
			if(page + 1 <= totalpages)
			{
				clearFixLayer();
				page++;
			}
		}

		if (p1e & JOY_LEFT)
		{
			if(page - 1 > 0)
			{
				clearFixLayer();
				page--;
			}
		}

		if (p1e & P1_SELECT)
		{
			exit = 1;
			clearFixLayer();
		}

		if (p1e & JOY_B)
		{
			exit = 1;
			clearFixLayer();
		}

	}
}