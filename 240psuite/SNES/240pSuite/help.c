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
	s16 page = 1, totalpages = 1;
		
	switch(option)
	{
		case HELP_COLORS:	
		case HELP_SHADOW:	
		case HELP_LED:
		case HELP_LAG:
			totalpages = 2;
			break;		
		case HELP_GENERAL:
		case HELP_STRIPES:	
		case HELP_MONOSCOPE:
			totalpages = 3;
			break;
		case HELP_PLUGE:
		case HELP_MANUALLAG:
			totalpages = 4;
			break;
	}  
  
	oamClear(0, 0);
	while(!end) 
	{
		if(redraw)
		{
			u16 size = 0;
						
			StartDMA();
			
			setPaletteColor(0x00, RGB5(0, 0, 0));
			consoleInitTextMine(0, 7, &font);
			
			AddTextColor(7, RGB5(31, 31, 31), RGB5(0, 0, 0));
			AddTextColor(6, RGB5(0, 31, 0), RGB5(0, 0, 0));			
			AddTextColor(5, RGB5(0, 27, 27), RGB5(0, 0, 0));	
			AddTextColor(4, RGB5(28, 28, 0), RGB5(0, 0, 0));	
			
			size = (&back_tiles_end - &back_tiles);
			bgInitTileSetMine(1, &back_tiles, &back_pal, 1, size, 16*2, BG_16COLORS, 0x6000);			
			
			size = (&back_map_end - &back_map);	
			bgInitMapSetMine(1, &back_map, size, SC_32x32, 0x2000);
						
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
				StartDMA();
		
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
							drawText(20, 22, 4, "(cont...)");							
							break;
						case 2:
							drawText(3, 4  , 6, "        HELP (2/3)"); 
							drawText(3, y++, 7, "The SNES/SFC can output"); 
							drawText(3, y++, 7, "224 and 239 active video");
							drawText(3, y++, 7, "lines.");
							y++;
							drawText(3, y++, 7, "An interlaced mode is"); 
							drawText(3, y++, 7, "also available.");
							y++;
							drawText(3, y++, 7, "Help can be shown by"); 
							drawText(3, y++, 7, "pressing 'START' in any");
							drawText(3, y++, 7, "test or pattern.");
							y++;
												
							drawText(20, 22, 4, "(cont...)");
							break;
						case 3:
							DrawTilesWithSpritesBarcode(190, 140, 32, 32, &barcode_tiles, &barcode_tiles_end-&barcode_tiles, &barcode_pal);				
							
							drawText(3, 4  , 6, "        HELP (3/3)"); 
							drawText(3, y++, 7, "The 240p suite is also"); 
							drawText(3, y++, 7, "available in GameCube,");
							drawText(3, y++, 7, "NES, Wii, Genesis/MD,");
							drawText(3, y++, 7, "Sega CD, PC Engine/");
							drawText(3, y++, 7, "TG-16, Neo Geo, Dreamcast,");
							drawText(3, y++, 7, "and more.");
							y++;
							drawText(3, y++, 7, "Please visit:"); 
							drawText(3, y++, 5, "http://junkerhq.net/240p");
							drawText(3, y++, 7, "for more information.");							
							break;
					}
					break;
				case HELP_PLUGE:
					switch(page)
					{
						case 1:
							drawText(3, 4  , 6, "        PLUGE (1/4)"); 
							drawText(3, y++, 7, "The PLUGE pattern is used"); 
							drawText(3, y++, 7, "to help adjust the black");
							drawText(3, y++, 7, "level to a correct value.");
							y++;
							drawText(3, y++, 7, "You can toggle between");
							drawText(3, y++, 7, "full range RGB bars and");
							drawText(3, y++, 7, "7.5 IRE NTSC bars with A.");
							y++;
							drawText(3, y++, 7, "The inner bars are the"); 
							drawText(3, y++, 7, "lowest values the system");
							drawText(3, y++, 7, "can show: blue at 1 and 2");
							drawText(3, y++, 7, "IRE, and gray at 3 IRE in");
							drawText(3, y++, 7, "full RGB. When in NTSC");
							drawText(3, y++, 7, "values are: 3.5, 7.5 and");
							drawText(3, y++, 7, "11.5 IRE.");
							drawText(20, 22, 4, "(cont...)");							
							break;
						case 2:
							drawText(3, 4  , 6, "        PLUGE (2/4)"); 
							drawText(3, y++, 7, "You should use the full");
							drawText(3, y++, 7, "range RGB mode to ensure");	
							drawText(3, y++, 7, "all detail is shown.");	
							y++;
							drawText(3, y++, 7, "If these bars are not"); 
							drawText(3, y++, 7, "visible, adjust the");
							drawText(3, y++, 7, "\"brightness\" control until");
							drawText(3, y++, 7, "they are.");					
							y++;					
							drawText(3, y++, 7, "You should lower it until"); 
							drawText(3, y++, 7, "the inner ones are not");
							drawText(3, y++, 7, "visible, and raise it");
							drawText(3, y++, 7, "until they show.");
							y++;
							drawText(20, 22, 4, "(cont...)");													
							break;
						case 3:
							drawText(3, 4  , 6, "        PLUGE (3/4)"); 
							drawText(3, y++, 7, "NTSC levels require black"); 
							drawText(3, y++, 7, "at 7.5 IRE, but for games");
							drawText(3, y++, 7, "you should always use full");
							drawText(3, y++, 7, "range RGB.");
							y++;
							drawText(3, y++, 7, "Although this system can");
							drawText(3, y++, 7, "only output 3.5 IRE in");
							drawText(3, y++, 7, "gray, it can output lower");					
							drawText(3, y++, 7, "luminance levels in each"); 
							drawText(3, y++, 7, "pure color.");
							y++;
							drawText(3, y++, 7, "You can switch between"); 
							drawText(3, y++, 7, "these values for the inner");
							drawText(3, y++, 7, "bars with the X button.");
							y++;
							drawText(20, 22, 4, "(cont...)");
							break;
						case 4:
							drawText(3, 4  , 6, "        PLUGE (4/4)"); 
							drawText(3, y++, 7, "You can press L or R"); 
							drawText(3, y++, 7, "for a tiled brightness");
							drawText(3, y++, 7, "and contrast sub test.");
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
							drawText(20, 22, 4, "(cont...)");							
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
							drawText(3, y++, 7, "Sharpness should be set in"); 
							drawText(3, y++, 7, "your CRT to a value that ");
							drawText(3, y++, 7, "shows clean black and gray");			
							drawText(3, y++, 7, "transitions with no white");
							drawText(3, y++, 7, "ghosting in between.");							
							y++;
							drawText(3, y++, 7, "On most modern displays,"); 
							drawText(3, y++, 7, "sharpness is an edge-");
							drawText(3, y++, 7, "enhancement control, and");
							drawText(3, y++, 7, "should probably be set to");
							drawText(3, y++, 7, "zero, or at the middle.");				
							y++;
							drawText(3, y++, 7, "You can toggle to a brick"); 
							drawText(3, y++, 7, "tileset to verify this");
							drawText(3, y++, 7, "setting.");
					break;
				case HELP_SMPTE:
							if(!snes_50hz)
								drawText(3, 4  , 6, "     SMPTE COLOR BARS"); 
							else
								drawText(3, 4  , 6, "      EBU COLOR BARS"); 
							drawText(3, y++, 7, "This pattern can be used"); 
							if(snes_50hz)
								drawText(3, y++, 7, "to calibrate for PAL");
							else
								drawText(3, y++, 7, "to calibrate for NTSC");
							drawText(3, y++, 7, "levels regarding contrast,");							
							drawText(3, y++, 7, "brightness and colors.");
							y++;
							drawText(3, y++, 7, "You can toggle between 75%");
							if(snes_50hz)
								drawText(3, y++, 7, "and 100% EBU color bars");
							else
								drawText(3, y++, 7, "and 100% SMPTE color bars");
							drawText(3, y++, 7, "with A.");
							y++;
							drawText(3, y++, 7, "You can use color filters"); 
							drawText(3, y++, 7, "or the blue only option in");
							drawText(3, y++, 7, "your display-if available-");
							drawText(3, y++, 7, "to confirm color balance.");								
					break;
				case HELP_MONOSCOPE:
					switch(page)
					{
						case 1:
							drawText(3, 4  , 6, "      MONOSCOPE (1/3)"); 
							drawText(3, y++, 7, "This pattern contains"); 
							drawText(3, y++, 7, "elements to calibrate"); 
							drawText(3, y++, 7, "multiple aspects of a CRT."); 
							drawText(3, y++, 7, "Read your monitor's"); 
							drawText(3, y++, 7, "service manual to learn"); 
							drawText(3, y++, 7, "how. Use L&R to change."); 
							y++;
							drawText(3, y++, 5, "Brightness adjustment:"); 
							drawText(3, y++, 7, "Adjust convergence at low"); 
							drawText(3, y++, 7, "brightness (e.g. 30 IRE)."); 
							drawText(3, y++, 7, "An overly bright pattern"); 
							drawText(3, y++, 7, "can mask bloom issues."); 
							drawText(20, 22, 4, "(cont...)");	
							break;
						case 2:
							drawText(3, 4  , 6, "      MONOSCOPE (2/3)"); 
							drawText(3, y++, 5, "Convergence:"); 
							drawText(3, y++, 7, "Use the center crosshair"); 
							drawText(3, y++, 7, "to check static (center"); 
							drawText(3, y++, 7, "of screen) convergence."); 
							drawText(3, y++, 7, "Use the patterns at the"); 
							drawText(3, y++, 7, "sides to check dynamic"); 
							drawText(3, y++, 7, "(edge) convergence."); 
							y++;
							drawText(3, y++, 5, "Corners:"); 
							drawText(3, y++, 7, "After setting center and"); 
							drawText(3, y++, 7, "edge convergence, use"); 
							drawText(3, y++, 7, "magnets to adjust corner"); 
							drawText(3, y++, 7, "purity and geometry."); 
							drawText(20, 22, 4, "(cont...)");	
							break;
						case 3:
							drawText(3, 4  , 6, "      MONOSCOPE (3/3)"); 
							drawText(3, y++, 5, "Size and aspect ratio:"); 
							drawText(3, y++, 7, "If vertical and horizontal"); 
							drawText(3, y++, 7, "size are correct, the red"); 
							drawText(3, y++, 7, "squares in the pattern"); 
							drawText(3, y++, 7, "will be perfect squares."); 
							drawText(3, y++, 7, "After setting H size, use"); 
							drawText(3, y++, 7, "a tape measure to adjust"); 
							drawText(3, y++, 7, "V size to match it."); 
							y++;
							drawText(3, y++, 5, "Linearity:"); 
							drawText(3, y++, 7, "The squares in each corner"); 
							drawText(3, y++, 7, "should get you started."); 
							drawText(3, y++, 7, "Confirm your adjustment"); 
							drawText(3, y++, 7, "using the scroll tests."); 
							y++;
							drawText(3, y++, 4, "Designed by Keith Raney"); 
							break;
					}
					break;
				case HELP_GRAY:
							drawText(3, 4  , 6, "        GRAY RAMP"); 
							drawText(3, y++, 7, "This gray ramp pattern can"); 
							drawText(3, y++, 7, "be used to check color");
							drawText(3, y++, 7, "balance.");							
							y++;
							drawText(3, y++, 7, "You should make sure the");
							drawText(3, y++, 7, "bars are gray, with no red");
							drawText(3, y++, 7, "or blue bias.");
							if(!snes_50hz)
							{
								y++;
								drawText(3, y++, 7, "This can be adjusted with ");
								drawText(3, y++, 7, "the color control in NTSC");
								drawText(3, y++, 7, "displays.");
							}
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
							drawText(3, y++, 7, "available with A, SELECT");
							drawText(3, y++, 7, "enables fast color mode.");
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
							drawText(3, y++, 7, "with the X button. A frame");
							drawText(3, y++, 7, "counter is also available");							
							drawText(3, y++, 7, "with button A.");
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
							drawText(3, y++, 7, "with the X button. A frame");
							drawText(3, y++, 7, "counter is also available");
							drawText(3, y++, 7, "with A.");				
							y++;								
							drawText(3, y++, 7, "When auto-toggle is set,");
							drawText(3, y++, 7, "you should see the lines");				
							drawText(20, 22, 4, "(cont...)");							
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
							drawText(20, 22, 4, "(cont...)");							
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
							drawText(20, 22, 4, "(cont...)");							
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
							y++;
							drawText(3, y++, 7, "Donna by Jose Salot");
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
							drawText(3, y++, 7, "This test shows a 2 layer"); 
							drawText(3, y++, 7, "background or a single");
							drawText(3, y++, 7, "vertical one. Speed can");
							drawText(3, y++, 7, "be varied with the d-pad");
							drawText(3, y++, 7, "and direction can be");
							drawText(3, y++, 7, "changed with X. The A");
							drawText(3, y++, 7, "button stops the scroll");
							drawText(3, y++, 7, "and Y switches between");
							drawText(3, y++, 7, "vertical and horizontal.");
							drawText(3, y++, 7, "This can be used to");
							drawText(3, y++, 7, "notice any drops in");
							drawText(3, y++, 7, "framerate.");
							y++;						
							drawText(3, y++, 7, "Sonic is a trademark of");
							drawText(3, y++, 7, "Sega Ltd and Kiki Kaikai");
							drawText(3, y++, 7, "of Taito.");
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
				case HELP_MDFOURIER:
							drawText(3, 4  , 6, "        MDFOURIER"); 
							drawText(3, y++, 7, "Button A plays the test"); 
							drawText(3, y++, 7, "tones. Please record it"); 
							drawText(3, y++, 7, "with your audio interface"); 
							drawText(3, y++, 7, "and use the software at:"); 
							y++;
							drawText(3, y++, 7, "http://junkerhq.net/MDF"); 
							y++;
							drawText(3, y++, 7, "Keep in mind that this"); 
							drawText(3, y++, 7, "platform uses a ceramic"); 
							drawText(3, y++, 7, "resonator which varies"); 
							drawText(3, y++, 7, "with temperature and"); 
							drawText(3, y++, 7, "between consoles. Use the"); 
							drawText(3, y++, 7, "special options to"); 
							drawText(3, y++, 7, "compensate for it."); 
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
							drawText(20, 22, 4, "(cont...)");							
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
							drawText(20, 22, 4, "(cont...)");							
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
				case HELP_VIDEO:
							drawText(3, 4  , 6, "       VIDEO OPTIONS"); 
							drawText(3, y++, 7, "Here you can enable or"); 
							drawText(3, y++, 7, "disable the System's");
							drawText(3, y++, 7, "interlaced video mode.");
							y++;
							drawText(3, y++, 7, "When enabled, the system");
							drawText(3, y++, 7, "outputs each field twice.");
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
				case HELP_ALTERNATE:
							drawText(3, 4  , 6, "  ALTERNATING 240p/480i"); 
							drawText(3, y++, 7, "Some devices have a delay"); 
							drawText(3, y++, 7, "when the source changes");
							drawText(3, y++, 7, "between resolutions, which ");							
							drawText(3, y++, 7, "happens in some games.");
							y++;
							drawText(3, y++, 7, "This test allows to time");
							drawText(3, y++, 7, "that delay manually.");
							y++;
							drawText(3, y++, 7, "Press A to switch modes");
							drawText(3, y++, 7, "press it again when you");
							drawText(3, y++, 7, "are able to see the screen");
							drawText(3, y++, 7, "back in your display.");
					break;
				case HELP_AUDIOSYNC:
							drawText(3, 4  , 6, "      AUDIO SYNC TEST"); 
							drawText(3, y++, 7, "This test flashes the"); 
							drawText(3, y++, 7, "whole screen white for two");
							drawText(3, y++, 7, "frames, along with a 1khz");							
							drawText(3, y++, 7, "test tone.");
							y++;
							drawText(3, y++, 7, "You can verify the sync");
							drawText(3, y++, 7, "between audio and video");
							drawText(3, y++, 7, "with recording equipment");
							drawText(3, y++, 7, "or specialized hardware.");
							y++;
							drawText(3, y++, 7, "Press A to start/stop");
							drawText(3, y++, 7, "the test.");
					break;
				case HELP_MANUALLAG:
					switch(page)
					{
						case 1:
							drawText(3, 4  , 6, "Timing & Reflex Test (1/4)"); 
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
							drawText(20, 22, 4, "(cont...)");							
							break;
						case 2:
							drawText(3, 4  , 6, "Timing & Reflex Test (2/4)"); 
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
							drawText(20, 22, 4, "(cont...)");
							break;
						case 3:
							drawText(3, 4  , 6, "Timing & Reflex Test (3/4)"); 
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
							drawText(20, 22, 4, "(cont...)");
							break;
						case 4:
							drawText(3, 4  , 6, "Timing & Reflex Test (4/4)");  
							drawText(3, y++, 7, "experienced player the"); 
							drawText(3, y++, 7, "hang of the system when");
							drawText(3, y++, 7, "testing via different");
							drawText(3, y++, 7, "paths.");
							break;
					}
					break;
					case HELP_CONTRAST:
							drawText(3, 4  , 6, "     PLUGE CONTRAST"); 
							drawText(3, y++, 7, "This sub test fills the"); 
							drawText(3, y++, 7, "screen with a pattern. By ");
							drawText(3, y++, 7, "default it is in color.");							
							y++;
							drawText(3, y++, 7, "Pressing A changes the"); 
							drawText(3, y++, 7, "palette to display a dark");
							drawText(3, y++, 7, "or white pattern in order");
							drawText(3, y++, 7, "to check contrast and");
							drawText(3, y++, 7, "brightness for clipping.");
							y++;
							drawText(3, y++, 7, "L or R return to PLUGE"); 
					break;
					case HELP_CONVERGENCE:
							drawText(3, 4  , 6, "    CONVERGENCE TESTS"); 
							drawText(3, y++, 7, "These are used to adjust"); 
							drawText(3, y++, 7, "color convergence in CRTs.");
							y++;
							drawText(3, y++, 7, "The 'L/R' button change"); 
							drawText(3, y++, 7, "the cross hatch pattern");
							drawText(3, y++, 7, "between lines, dots and");
							drawText(3, y++, 7, "crosses. Then to a color");
							drawText(3, y++, 7, "pattern for transition");
							drawText(3, y++, 7, "boundary check with and"); 
							drawText(3, y++, 7, "without a black border."); 
					break;
					case HELP_DISAPPEAR:
							drawText(3, 4  , 6, "    DISAPPEARING LOGO"); 
							drawText(3, y++, 7, "This test allows you to"); 
							drawText(3, y++, 7, "use a slow motion camera");
							drawText(3, y++, 7, "to measure how long it ");
							drawText(3, y++, 7, "takes from a button");
							drawText(3, y++, 7, "press to when the logo");
							drawText(3, y++, 7, "disappears.");							
							y++;
							drawText(3, y++, 7, "You can also use a"); 
							drawText(3, y++, 7, "photodiode and press X,");
							drawText(3, y++, 7, "which turns the whole");
							drawText(3, y++, 7, "screen to white for");
							drawText(3, y++, 7, "2 frames.");
					break;
					case HELP_PHASE:
							drawText(3, 4  , 6, "        PHASE CHECK"); 
							drawText(3, y++, 7, "This test allows you to"); 
							drawText(3, y++, 7, "check phase alignment in");
							drawText(3, y++, 7, "upscalers.");
							y++;
							drawText(3, y++, 7, "Use left or right to");
							drawText(3, y++, 7, "move, X for checker bg,");
							drawText(3, y++, 7, "and A to reset position.");
							y++;
							drawText(3, y++, 7, "Designed by FirebrandX");
					break;
			}
			drawText(5, 24, 5, "Press B to exit help");
			
			if(redraw)	
			{
				EndDMA();
				redraw = 0;
			}
						
			change = 0;			
		}
		WaitForVBlank();
		
		pressed = PadPressed(0);
		
		if(pressed & KEY_START)
			end = 1;
			
		if(pressed & KEY_B)
			end = 1;
			
		if(pressed & KEY_LEFT || pressed & KEY_L)
		{
			if(page - 1 > 0)
			{
				page --;
				change = 1;
			}
		}
			
		if(pressed & KEY_RIGHT || pressed & KEY_R)
		{
			if(page + 1 <= totalpages)
			{
				page ++;
				change = 1;
			}
		}
		if(change)
			oamInit();
	}
	Transition();
}