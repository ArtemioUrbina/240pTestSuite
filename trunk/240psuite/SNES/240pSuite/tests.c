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
 
#include "tests.h"

void DropShadowTest(void) 
{	
	u16 pad0, oldpad = 0xffff, pressed, end = 0;	 
	u16 redraw = 1, size = 0;
	
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			size = (&motoko_tiles1_end - &motoko_tiles1);
			bgInitTileSet(0, &motoko_tiles1, &motoko_pal, 0, size, 256*2, BG_256COLORS, 0x0000);	
			dmaCopyVram(&motoko_tiles2, 0x3000, (&motoko_tiles2_end-&motoko_tiles2));
			
			bgInitMapSet(0, &motoko_map, (&motoko_map_end - &motoko_map), SC_32x32, 0x7000);
						
			setMode(BG_MODE3,0); 					
			bgSetDisable(1);
			
			bgSetScroll(0, 0, -1);
			setBrightness(0xF);
			redraw = 0;
		}
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
		
		if(pressed == KEY_A)
			end = 1;		
		
		WaitForVBlank();
	}	
	setFadeEffect(FADE_OUT);	
	
	return;
}

void DrawStripes(void)
{	
	u16 pad0, oldpad = 0xffff, pressed, end = 0;	 
	u16 redraw = 1, alternate = 0, pos = 0;
	
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			bgInitTileSet(0, &hstripes_tiles, &grid_pal, 0, (&hstripes_tiles_end - &hstripes_tiles), 16*2, BG_16COLORS, 0x6000);	
			bgInitMapSet(0, &fullscreen_map, (&fullscreen_map_end - &fullscreen_map), SC_32x32, 0x7000);
			
			setPaletteColor(0x00, RGB8(0, 0, 0));
			setPaletteColor(0x01, RGB8(0xff, 0xff, 0xff));
			setMode(BG_MODE1,0); 					
			bgSetDisable(1);
			bgSetDisable(2);
			
			bgSetScroll(0, 0, -1);
			setBrightness(0xF);
			redraw = 0;
		}
		
		if(alternate)
		{
			if(pos)
			{
				setPaletteColor(0x00, RGB8(0, 0, 0));
				setPaletteColor(0x01, RGB8(0xff, 0xff, 0xff));
			}
			else
			{
				setPaletteColor(0x01, RGB8(0, 0, 0));
				setPaletteColor(0x00, RGB8(0xff, 0xff, 0xff));
			}
			pos = !pos;
		}
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
		
		if(pressed == KEY_B)
			alternate = !alternate;
			
		if(pressed == KEY_A)
			end = 1;		
		
		WaitForVBlank();
	}	
	setFadeEffect(FADE_OUT);	
	
	return;
}