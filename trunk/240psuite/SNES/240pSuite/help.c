/* 
 * 240p Test Suite
 * Copyright (C)2014 Artemio Urbina (SNES)
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

#include "help.h"
#include "res.h"
#include "font.h"
#include "video.h"
#include "control.h"

void DrawHelp(u16 option)
{
	u16 redraw = 1, change = 0, end = 0;
	u16 pressed;	
	int page = 1, totalpages = 1;
		
	switch(option)
	{
		case HELP_PLUGE:
		case HELP_COLORS:	
		case HELP_SHADOW:	
		case HELP_LED:
		case HELP_LAG:
			totalpages = 2;
			break;		
		case HELP_GENERAL:
		case HELP_MANUALLAG:
		case HELP_STRIPES:	
			totalpages = 3;
			break;
	}  
  
	oamClear(0, 0);
	while(!end) 
	{
		if(redraw)
		{
			u16 size = 0;
						
			setBrightness(0);	
			
			setPaletteColor(0x00, RGB5(0, 0, 0));
			consoleInitText(0, 7, &font);
			
			AddTextColor(7, RGB5(31, 31, 31), RGB5(0, 0, 0));
			AddTextColor(6, RGB5(0, 31, 0), RGB5(0, 0, 0));			
			AddTextColor(5, RGB5(0, 27, 27), RGB5(0, 0, 0));	
			
			size = (&back_tiles_end - &back_tiles);
			bgInitTileSet(1, &back_tiles, &back_pal, 1, size, 16*2, BG_16COLORS, 0x6000);			
			
			size = (&back_map_end - &back_map);	
			bgInitMapSet(1, &back_map, size, SC_32x32, 0x2000);
						
			setMode(BG_MODE1,0); 	
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);						
			bgSetScroll(0, 0, -2);	
			
			change = 1;
		}			
		
		if(change)
		{
			u16 y = 7;			
			
			if(!redraw)
				CleanFontMap();
			else
				setBrightness(0);	
		
			switch(option)
			{
				case HELP_GENERAL:
					switch(page)
					{
						case 1:
							drawText(3, 4  , 6, "        HELP (1/3)"); 
							drawText(3, y++, 7, "The 240p Test Suite was"); 
							drawText(3, y++, 7, "designed with two goals in");
							drawText(3, y++, 7, "mind:");
							y++;
							drawText(3, y++, 7, "1) Evaluate 240p signals"); 
							drawText(3, y++, 7, "on TV sets and video");
							drawText(3, y++, 7, "procesors; and");
							y++;
							drawText(3, y++, 7, "2) Provide calibration"); 
							drawText(3, y++, 7, "patterns from a game");
							drawText(3, y++, 7, "console to help in");
							drawText(3, y++, 7, "properly calibrating the");
							drawText(3, y++, 7, "display black, white and");
							drawText(3, y++, 7, "color levels.");
							drawText(20, 22, 6, "(cont...)");							
							break;
						case 2:
							drawText(3, 4  , 6, "        HELP (2/3)"); 
							drawText(3, y++, 7, "The SNES/SFC can output"); 
							drawText(3, y++, 7, "224 and 239 active video");
							drawText(3, y++, 7, "lines.");
							y++;
							drawText(3, y++, 7, "Help can be shown by"); 
							drawText(3, y++, 7, "pressing 'START' in any");
							drawText(3, y++, 7, "test or pattern.");
							y++;
							drawText(3, y++, 7, "The 240p suite is also"); 
							drawText(3, y++, 7, "available in GameCube,");
							drawText(3, y++, 7, "Wii, Genesis, Sega CD");
							drawText(3, y++, 7, "and Dreamcast.");							
							drawText(20, 22, 6, "(cont...)");
							break;
						case 3:
							drawText(3, 4  , 6, "        HELP (3/3)"); 
							drawText(3, y++, 7, "Please visit:"); 
							drawText(3, y++, 7, "http://junkerhq.net/240p");
							drawText(3, y++, 7, "for more information.");							
							break;
					}
					break;
				case HELP_PLUGE:
					switch(page)
					{
						case 1:
							drawText(3, 4  , 6, "        PLUGE (1/2)"); 
							drawText(3, y++, 7, "The PLUGE pattern is used"); 
							drawText(3, y++, 7, "to help adjust the black");
							drawText(3, y++, 7, "level to a correct value.");
							y++;
							drawText(3, y++, 7, "The inner bars on the"); 
							drawText(3, y++, 7, "sides are black @ 3.5 IRE,");
							drawText(3, y++, 7, "the middle ones @ 7.5 IRE,");
							drawText(3, y++, 7, "and the outer @ 11.5 IRE.");
							y++;
							drawText(3, y++, 7, "If these bars are not"); 
							drawText(3, y++, 7, "visible, adjust the");
							drawText(3, y++, 7, "\"brightness\" control until");
							drawText(3, y++, 7, "they are.");							
							drawText(20, 22, 6, "(cont...)");							
							break;
						case 2:
							drawText(3, 4  , 6, "        PLUGE (2/2)"); 
							drawText(3, y++, 7, "You should lower it until"); 
							drawText(3, y++, 7, "they are not visible, and");
							drawText(3, y++, 7, "raise it until they show.");
							y++;
							drawText(3, y++, 7, "NTSC levels require black"); 
							drawText(3, y++, 7, "@ 7.5 IRE for video.");							
							y++;
							drawText(3, y++, 7, "In PAL - and console"); 
							drawText(3, y++, 7, "gaming in general - it is");
							drawText(3, y++, 7, "adviced to use the 3.5");
							drawText(3, y++, 7, "IRE value as black."		);														
							break;
					}
					break;
				case HELP_COLORS:
					switch(page)
					{
						case 1:
							drawText(3, 4  , 6, "     COLORBARS (1/2)"); 
							drawText(3, y++, 7, "This pattern allows you to"); 
							drawText(3, y++, 7, "calibrate each color: Red,");
							drawText(3, y++, 7, "Green and Blue; as well as");
							drawText(3, y++, 7, "white.");
							y++;
							drawText(3, y++, 7, "Adjust the white level"); 
							drawText(3, y++, 7, "first, using the");
							drawText(3, y++, 7, "\"Contrast\" control on your");
							drawText(3, y++, 7, "TV set.");
							y++;
							drawText(3, y++, 7, "Raise it until you cannot");							
							drawText(3, y++, 7, "distinguish between the"); 
							drawText(3, y++, 7, "blocks under \"C\" and \"E\",");
							drawText(3, y++, 7, "and lower it slowly until");														
							drawText(20, 22, 6, "(cont...)");							
							break;
						case 2:
							drawText(3, 4  , 6, "     COLORBARS (2/2)"); 
							drawText(3, y++, 7, "you can clearly tell them");
							drawText(3, y++, 7, "appart."); 
							y++;
							drawText(3, y++, 7, "Do the same for each");
							drawText(3, y++, 7, "color.");
							y++;
							drawText(3, y++, 7, "A grid can be displayed");
							drawText(3, y++, 7, "with the A button.");
							break;
					}
					break;
				case HELP_GRID:
				case HELP_GRID_239:
							drawText(3, 4  , 6, "           GRID"); 
							drawText(3, y++, 7, "This grid uses the full"); 
							if(option == HELP_GRID_239)
								drawText(3, y++, 7, "256x239 resolution.");					
							else
								drawText(3, y++, 7, "256x224 resolution.");
							y++;
							drawText(3, y++, 7, "You can use it to verify"); 
							drawText(3, y++, 7, "that all the visible area");
							drawText(3, y++, 7, "is being displayed, and");
							drawText(3, y++, 7, "that there is no");
							drawText(3, y++, 7, "distortion present.");
							if(option == HELP_GRID_239)
							{
								y++;
								drawText(3, y++, 7, "The 239 resolution is");
								drawText(3, y++, 7, "rarely used."); 								
							}
					break;					
				case HELP_BLEED:
							drawText(3, 4  , 6, "        COLOR BLEED"); 
							drawText(3, y++, 7, "This pattern helps"); 
							drawText(3, y++, 7, "diagnose color bleed");
							drawText(3, y++, 7, "caused by unneeded color");
							drawText(3, y++, 7, "upsampling.");
							y++;
							drawText(3, y++, 7, "You can toggle between"); 
							drawText(3, y++, 7, "vertical bars and");
							drawText(3, y++, 7, "checkerboard with 'A'." );							
					break;
				case HELP_IRE:
							drawText(3, 4  , 6, "          100 IRE"); 
							drawText(3, y++, 7, "You can vary IRE intensity"); 
							drawText(3, y++, 7, "with L and R by 10 IRE");
							drawText(3, y++, 7, "steps.");
							y++;
							drawText(3, y++, 7, "You can toggle between"); 
							drawText(3, y++, 7, "0-100 and 100-140 ranges");
							drawText(3, y++, 7, "with the 'A' button." );							
					break;
				case HELP_601CB:
							drawText(3, 4  , 6, "      601 COLORBARS"); 
							drawText(3, y++, 7, "You can use color filters"); 
							drawText(3, y++, 7, "or the blue only option in");
							drawText(3, y++, 7, "your display-if available-");							
							drawText(3, y++, 7, "to confirm color balance.");
					break;
				case HELP_SHARPNESS:
							drawText(3, 4  , 6, "         SHARPNESS"); 
							drawText(3, y++, 7, "You should set the"); 
							drawText(3, y++, 7, "sharpness of your CRT to a");
							drawText(3, y++, 7, "value that shows clean");							
							drawText(3, y++, 7, "black and gray"); 
							drawText(3, y++, 7, "transitions with no white");
							drawText(3, y++, 7, "ghosting between.");							
							y++;
							drawText(3, y++, 7, "On most modern displays,"); 
							drawText(3, y++, 7, "the sharpness control is");
							drawText(3, y++, 7, "an edge-enhancement");
							drawText(3, y++, 7, "control, and most probably");
							drawText(3, y++, 7, "should be set to zero, or");
							drawText(3, y++, 7, "in the middle.");							
					break;
				case HELP_SMPTE:
							drawText(3, 4  , 6, "     SMPTE COLOR BARS"); 
							drawText(3, y++, 7, "This pattern can be used"); 
							drawText(3, y++, 7, "to calibrate for NTSC");
							drawText(3, y++, 7, "levels regarding contrast,");							
							drawText(3, y++, 7, "brightness and colors.");
							y++;
							drawText(3, y++, 7, "You can toggle between 75%");
							drawText(3, y++, 7, "and 100% SMPTE color bars");
							drawText(3, y++, 7, "with A.");
							y++;
							drawText(3, y++, 7, "You can use color filters"); 
							drawText(3, y++, 7, "or the blue only option in");
							drawText(3, y++, 7, "your display-if available-");
							drawText(3, y++, 7, "to confirm color balance.");								
					break;
				case HELP_LINEARITY:
							drawText(3, 4  , 6, "         LINEARITY"); 
							drawText(3, y++, 7, "This pattern shows 5"); 
							drawText(3, y++, 7, "circles, that are matched");
							drawText(3, y++, 7, "to the 10:11 NTSC aspect");							
							drawText(3, y++, 7, "ratio.");
							y++;
							drawText(3, y++, 7, "The linearity of the");
							drawText(3, y++, 7, "display or upscaler can be");
							drawText(3, y++, 7, "verified by measuring the");
							drawText(3, y++, 7, "diameter of the circles.");
							y++;
							drawText(3, y++, 7, "Of course the linearity"); 
							drawText(3, y++, 7, "should be kept in all");
							drawText(3, y++, 7, "directions.");
					break;
				case HELP_GRAY:
							drawText(3, 4  , 6, "        GRAY RAMP"); 
							drawText(3, y++, 7, "This gray ramp pattern can"); 
							drawText(3, y++, 7, "be used to check color");
							drawText(3, y++, 7, "balance.");							
							y++;
							drawText(3, y++, 7, "You should make sure the");
							drawText(3, y++, 7, "bars are gray, with no red");
							drawText(3, y++, 7, "or blue bias. This can be");
							drawText(3, y++, 7, "adjusted with the color");
							drawText(3, y++, 7, "control in NTSC displays.");
					break;
				case HELP_WHITE:
							drawText(3, 4  , 6, "       WHITE SCREEN"); 
							drawText(3, y++, 7, "This screen can be"); 
							drawText(3, y++, 7, "changed between white,");
							drawText(3, y++, 7, "black, red, green and blue");
							drawText(3, y++, 7, "screens with the L and R");
							drawText(3, y++, 7, "buttons.");
							y++;
							drawText(3, y++, 7, "Some displays and scalers");
							drawText(3, y++, 7, "have issues when changing");
							drawText(3, y++, 7, "between a black 0 IRE and");
							drawText(3, y++, 7, "a white screen.");
							y++;
							drawText(3, y++, 7, "A custom color mode is");
							drawText(3, y++, 7, "available by pressing A.");
					break;
				case HELP_CHECK:
							drawText(3, 4  , 6, "       CHECKERBOARD"); 
							drawText(3, y++, 7, "This pattern shows all the"); 
							drawText(3, y++, 7, "visible pixels in an");
							drawText(3, y++, 7, "alternating white and");
							drawText(3, y++, 7, "black grid array.");
							y++;
							drawText(3, y++, 7, "You can toggle the pattern");
							drawText(3, y++, 7, "with button R, or turn on");
							drawText(3, y++, 7, "auto-toggle each frame");
							drawText(3, y++, 7, "with the A button. A frame");
							drawText(3, y++, 7, "counter is also available");							
							drawText(3, y++, 7, "with X.");
					break;
				case HELP_STRIPES:
				switch(page)
					{
						case 1:
							drawText(3, 4  , 6, " HORIZONTAL STRIPES (1/3)"); 
							drawText(3, y++, 7, "You should see a pattern"); 
							drawText(3, y++, 7, "of lines, each one pixel");
							drawText(3, y++, 7, "in height.");
							y++;
							drawText(3, y++, 7, "You can toggle the pattern"); 
							drawText(3, y++, 7, "with button R, or turn on");
							drawText(3, y++, 7, "auto-toggle each frame");
							drawText(3, y++, 7, "with the A button. A frame");
							drawText(3, y++, 7, "counter is also available");
							drawText(3, y++, 7, "with X.");				
							y++;								
							drawText(3, y++, 7, "When auto-toggle is set,");
							drawText(3, y++, 7, "you should see the lines");				
							drawText(20, 22, 6, "(cont...)");							
							break;
						case 2:
							drawText(3, 4  , 6, " HORIZONTAL STRIPES (2/3)"); 
							drawText(3, y++, 7, "alternating rapidly. On"); 
							drawText(3, y++, 7, "some setups, the pattern");
							drawText(3, y++, 7, "doesn't change at all.");
							drawText(3, y++, 7, "This means that the signal");
							drawText(3, y++, 7, "is being treated as"); 
							drawText(3, y++, 7, "interlaced and very other");
							drawText(3, y++, 7, "frame is being discarded.");
							y++;
							drawText(3, y++, 7, "You can also display");							
							drawText(3, y++, 7, "vertical bars by pressing"); 
							drawText(3, y++, 7, "Y. That pattern will help");
							drawText(3, y++, 7, "you evaluate if the signal");												
							drawText(20, 22, 6, "(cont...)");							
							break;
						case 3:
							drawText(3, 4  , 6, " HORIZONTAL STRIPES (3/3)"); 
							drawText(3, y++, 7, "is not distorted"); 
							drawText(3, y++, 7, "horizontaly, since all");
							drawText(3, y++, 7, "lines should be one pixel");
							drawText(3, y++, 7, "wide.");
							break;
					}
					break;
			case HELP_SHADOW:
				switch(page)
					{
						case 1:
							drawText(3, 4  , 6, "   DROP SHADOW TEST (1/2)"); 
							drawText(3, y++, 7, "This is a crucial test for"); 
							drawText(3, y++, 7, "240p upscan converters. It");
							drawText(3, y++, 7, "displays a simple sprite");
							drawText(3, y++, 7, "shadow against an image,"); 
							drawText(3, y++, 7, "but the shadow is shown");
							drawText(3, y++, 7, "only on every other frame.");
							y++;
							drawText(3, y++, 7, "On a CRT this achieves a");
							drawText(3, y++, 7, "transparency effect, since");
							drawText(3, y++, 7, "you are watching a 30/25hz");				
							drawText(3, y++, 7, "shadow on a 60/50hz");
							drawText(3, y++, 7, "signal. No background");				
							drawText(3, y++, 7, "detail should be lost and");				
							drawText(3, y++, 7, "the shadow must be shown.");				
							drawText(20, 22, 6, "(cont...)");							
							break;
						case 2:
							drawText(3, 4  , 6, "   DROP SHADOW TEST (2/2)"); 
							drawText(3, y++, 7, "The user can toggle the"); 
							drawText(3, y++, 7, "frame used to draw the");
							drawText(3, y++, 7, "shadow with button A.");
							y++;
							drawText(3, y++, 7, "Backgrounds can be");
							drawText(3, y++, 7, "switched with the R and"); 
							drawText(3, y++, 7, "L buttons, and X toggles");
							drawText(3, y++, 7, "sprites.");
							break;						
					}
					break;
				case HELP_STRIPED:
							drawText(3, 4  , 6, "    STRIPED SPRITE TEST"); 
							drawText(3, y++, 7, "There are deinterlacers"); 
							drawText(3, y++, 7, "out there that can display");
							drawText(3, y++, 7, "the drop shadows correctly");
							drawText(3, y++, 7, "and still interpret 240p");
							drawText(3, y++, 7, "as 480i.");
							y++;
							drawText(3, y++, 7, "With a striped sprite it");
							drawText(3, y++, 7, "should be easy to tell if");
							drawText(3, y++, 7, "a processor tries to");
							drawText(3, y++, 7, "deinterlace and/or");
							drawText(3, y++, 7, "interpolate the image.");
							y++;						
							drawText(3, y++, 7, "You can change backgrounds");
							drawText(3, y++, 7, "with R & L.");
					break;
				case HELP_HSCROLL:
							drawText(3, 4  , 6, "       SCROLL TEST"); 
							drawText(3, y++, 7, "This test shows a two"); 
							drawText(3, y++, 7, "layer background from");
							drawText(3, y++, 7, "Sonic the Hedgehog. The");
							drawText(3, y++, 7, "speed can be varied with");
							drawText(3, y++, 7, "the d-pad, and direction");
							drawText(3, y++, 7, "can be changed with X.");
							drawText(3, y++, 7, "The A button stops the");
							drawText(3, y++, 7, "scroll. This can be used");
							drawText(3, y++, 7, "to notice any drops in");
							drawText(3, y++, 7, "framerate.");
							y++;						
							drawText(3, y++, 7, "Sonic The Hedgehog is a");
							drawText(3, y++, 7, "trademark of Sega");
							drawText(3, y++, 7, "Enterprises Ltd.");
					break;
				case HELP_VSCROLL:
							drawText(3, 4  , 6, "      GRID SCROLL TEST"); 
							drawText(3, y++, 7, "A grid is scrolled"); 
							drawText(3, y++, 7, "vertically or horizontally");
							drawText(3, y++, 7, "which can be used to test");
							drawText(3, y++, 7, "linearity and how well the");
							drawText(3, y++, 7, "display or video processor");
							drawText(3, y++, 7, "copes with scrolling and");
							drawText(3, y++, 7, "framerate.");
							y++;
							drawText(3, y++, 7, "Button X can be used to");
							drawText(3, y++, 7, "toggle between horizontal");
							drawText(3, y++, 7, "and vertical, while the");						
							drawText(3, y++, 7, "d-pad regulates speed.");
							drawText(3, y++, 7, "Button A stops the scroll");
							drawText(3, y++, 7, "and button Y changes");
							drawText(3, y++, 7, "direction.");
					break;
				case HELP_SOUND:
							drawText(3, 4  , 6, "        SOUND TEST"); 
							drawText(3, y++, 7, "This simple test plays a"); 
							drawText(3, y++, 7, "sound in either channel,");
							drawText(3, y++, 7, "or both simultaneously.");
							y++;
							drawText(3, y++, 7, "It can be used to check if");
							drawText(3, y++, 7, "the audio chain is working");
							drawText(3, y++, 7, "properly.");
					break;
				case HELP_LED:
					switch(page)
					{
						case 1:
							drawText(3, 4  , 6, "    BACKLIT TEST (1/2)"); 
							drawText(3, y++, 7, "This test allows you to"); 
							drawText(3, y++, 7, "check how the display's");
							drawText(3, y++, 7, "backlit works when only a");
							drawText(3, y++, 7, "small array of pixels is"); 
							drawText(3, y++, 7, "shown. This can be very");
							drawText(3, y++, 7, "revealing and can be used");
							drawText(3, y++, 7, "to detect the zones in LED");
							drawText(3, y++, 7, "backlit displays, or to");
							drawText(3, y++, 7, "evaluate global and local");
							drawText(3, y++, 7, "dimming.");
							y++;
							drawText(3, y++, 7, "The user can move around"); 
							drawText(3, y++, 7, "the white pixel arrays");
							drawText(3, y++, 7, "with the d-pad, and change");
							drawText(20, 22, 6, "(cont...)");							
							break;
						case 2:
							drawText(3, 4  , 6, "    BACKLIT TEST (2/2)"); 
							drawText(3, y++, 7, "the size of the sprite"); 
							drawText(3, y++, 7, "with L & R.");
							y++;
							drawText(3, y++, 7, "The A button allows the"); 
							drawText(3, y++, 7, "user to hide the sprite in");
							drawText(3, y++, 7, "order to alternate a fully"); 
							drawText(3, y++, 7, "black screen.");
							break;
					}
					break;
				case HELP_LAG:
					switch(page)
					{
						case 1:
							drawText(3, 4  , 6, "      LAG TEST (1/2)"); 
							drawText(3, y++, 7, "This test is designed to"); 
							drawText(3, y++, 7, "be used with two displays");
							drawText(3, y++, 7, "conected at the same time.");
							drawText(3, y++, 7, "One being a CRT, or a"); 
							drawText(3, y++, 7, "display with a known lag");
							drawText(3, y++, 7, "as reference, and the");
							drawText(3, y++, 7, "other the display to test.");
							y++;
							drawText(3, y++, 7, "Using a camera, a picture");
							drawText(3, y++, 7, "should be taken of both");
							drawText(3, y++, 7, "screens at the same time.");
							drawText(3, y++, 7, "The picture will show the"); 
							drawText(3, y++, 7, "frame discrepancy between");
							drawText(3, y++, 7, "them.");
							drawText(20, 22, 6, "(cont...)");							
							break;
						case 2:
							drawText(3, 4  , 6, "      LAG TEST (2/2)"); 
							drawText(3, y++, 7, "The circles in the bottom"); 
							drawText(3, y++, 7, "help determine the frame");
							drawText(3, y++, 7, "even when the numbers are"); 
							drawText(3, y++, 7, "blurry.");
							break;
					}
					break;
				case HELP_MODE7:
							drawText(3, 4  , 6, "       MODE 7 GRID"); 
							drawText(3, y++, 7, "This serves no particular"); 
							drawText(3, y++, 7, "purpose, but using Mode 7.");
							y++;
							drawText(3, y++, 7, "The grid can be rotated");
							drawText(3, y++, 7, "with L & R, reset with A.");
							y++;
							drawText(3, y++, 7, "Rotation can be done in");
							drawText(3, y++, 7, "steps with the d-pad.");
						break;
				case HELP_OVERSCAN:
							drawText(3, 4  , 6, "         OVERSCAN"); 
							drawText(3, y++, 7, "With this pattern you can"); 
							drawText(3, y++, 7, "interactively find out the");
							drawText(3, y++, 7, "overscan in pixels of each");							
							drawText(3, y++, 7, "edge in a display.");
							y++;
							drawText(3, y++, 7, "Use left and right to");
							drawText(3, y++, 7, "increment the overscan");
							drawText(3, y++, 7, "until you see the white");
							drawText(3, y++, 7, "border, then go back one");
							drawText(3, y++, 7, "pixel. The resulting");
							drawText(3, y++, 7, "number is the amount of");
							drawText(3, y++, 7, "overscan in pixels in each");
							drawText(3, y++, 7, "direction.");
					break;
				case HELP_MANUALLAG:
					switch(page)
					{
						case 1:
							drawText(3, 4  , 6, "  MANUAL LAG TEST (1/4)"); 
							drawText(3, y++, 7, "The main intention is to"); 
							drawText(3, y++, 7, "show a changing pattern on");
							drawText(3, y++, 7, "the screen, which can be");
							drawText(3, y++, 7, "complemented with audio."); 
							drawText(3, y++, 7, "This should show to some");
							drawText(3, y++, 7, "degree any lag when");
							drawText(3, y++, 7, "processing the signal.");
							y++;
							drawText(3, y++, 7, "As an added feature, the"); 
							drawText(3, y++, 7, "user can click the A");
							drawText(3, y++, 7, "button when the sprite is");
							drawText(3, y++, 7, "aligned with the one on");
							drawText(3, y++, 7, "the background, and the");
							drawText(3, y++, 7, "offset in frames from the");
							drawText(20, 22, 6, "(cont...)");							
							break;
						case 2:
							drawText(3, 4  , 6, "  MANUAL LAG TEST (2/4)"); 
							drawText(3, y++, 7, "actual intersection will"); 
							drawText(3, y++, 7, "be shown on screen. This");
							drawText(3, y++, 7, "can be repeated ten times");
							drawText(3, y++, 7, "and the software will"); 
							drawText(3, y++, 7, "calculate the average.");
							y++;
							drawText(3, y++, 7, "Whenever the button was");
							drawText(3, y++, 7, "pressed before the actual"); 
							drawText(3, y++, 7, "intersection frame, the");
							drawText(3, y++, 7, "result will be ignored,");
							drawText(3, y++, 7, "but still shown onscreen.");
							y++;
							drawText(3, y++, 7, "Button X can be used to");
							drawText(3, y++, 7, "change the direction of");
							drawText(20, 22, 6, "(cont...)");
							break;
						case 3:
							drawText(3, 4  , 6, "  MANUAL LAG TEST (3/4)"); 
							drawText(3, y++, 7, "the sprite from vertical"); 
							drawText(3, y++, 7, "to horizontal, or both");
							drawText(3, y++, 7, "at the same time.");
							y++;				
							drawText(3, y++, 7, "Of course the evaluation"); 
							drawText(3, y++, 7, "is dependent on reflexes");
							drawText(3, y++, 7, "and/or rhythm more than");
							drawText(3, y++, 7, "anything. The visual and"); 
							drawText(3, y++, 7, "audio cues are the more");
							drawText(3, y++, 7, "revealing aspects which");
							drawText(3, y++, 7, "the user should consider,"); 
							drawText(3, y++, 7, "of course the interactive");
							drawText(3, y++, 7, "factor can give an");
							drawText(20, 22, 6, "(cont...)");
							break;
						case 4:
							drawText(3, 4  , 6, "  MANUAL LAG TEST (4/4)"); 
							drawText(3, y++, 7, "experienced player the"); 
							drawText(3, y++, 7, "hang of the system when");
							drawText(3, y++, 7, "testing via different");
							drawText(3, y++, 7, "paths.");
							break;
					}
					break;
			}
			drawText(5, 24, 5, "Press B to exit help");
			
			if(redraw)	
			{
				setBrightness(0xF);
				redraw = 0;
			}
						
			change = 0;			
		}
		WaitForVBlank();
		
		pressed = PadPressed(0);
		
		if(pressed == KEY_START)
			end = 1;
			
		if(pressed == KEY_B)
			end = 1;
			
		if(pressed == KEY_LEFT || pressed == KEY_L)
		{
			if(page - 1 > 0)
			{
				page --;
				change = 1;
			}
		}
			
		if(pressed == KEY_RIGHT || pressed == KEY_R)
		{
			if(page + 1 <= totalpages)
			{
				page ++;
				change = 1;
			}
		}
	}
	Transition();
}