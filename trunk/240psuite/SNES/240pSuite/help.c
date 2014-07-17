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

void DrawHelp(u16 option)
{
	u16 redraw = 1, change = 0, end = 0;
	u16 pad0, oldpad = 0xffff, pressed;	
	int page = 1, totalpages = 1;
	
	switch(option)
	{
		case HELP_PLUGE:
		case HELP_STRIPES:
		case HELP_SHADOW:
		case HELP_LED:
		case HELP_LAG:
			totalpages = 2;
			break;
		case HELP_MANUALLAG:
		case HELP_GENERAL:
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
			
			CleanFontMap();
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
							drawText(3, y++, 7, "224 and 240 active video");
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
			}
			drawText(7, 24, 6, "Press B to exit help");
			
			if(redraw)
			{
				redraw = 0;
				
				setBrightness(0xF);
				WaitForVBlank();			
			}
						
			change = 0;			
		}
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
		
		if(pressed == KEY_B)
			end = 1;
			
		if(pressed == KEY_LEFT)
		{
			if(page - 1 > 0)
			{
				page --;
				change = 1;
			}
		}
			
		if(pressed == KEY_RIGHT)
		{
			if(page + 1 <= totalpages)
			{
				page ++;
				change = 1;
			}
		}
			
		WaitForVBlank();
	}
	setFadeEffect(FADE_OUT);	
}