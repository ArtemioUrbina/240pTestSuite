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
 
#include "patterns.h"

void DrawGrid(void) 
{	
	u16 pad0, oldpad = 0xffff, pressed, end = 0;	 
	u16 toggle = 0, redraw = 1;
	
	bgInitTileSet(1, &grid_tiles, &grid_pal, 0, (&grid_tiles_end - &grid_tiles), 16*2, BG_16COLORS, 0x4000);	
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			if(toggle)
			{					
				bgInitMapSet(1, &grid240_map, (&grid240_map_end - &grid240_map), SC_32x32, 0x1000);
			
				Set240pMode();
			}
			else
			{				
				bgInitMapSet(1, &grid_map, (&grid_map_end - &grid_map), SC_32x32, 0x1000);
			
				Set224pMode();
			}
			setMode(BG_MODE1,0); 
			bgSetDisable(0);		
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);
			setBrightness(0xF);
			redraw = 0;
		}
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
		
		if(pressed == KEY_A)
			end = 1;		
			
		if(pressed == KEY_B)
		{
			toggle = !toggle;
			redraw = 1;
		}
		
		WaitForVBlank();
	}	
	setFadeEffect(FADE_OUT);
	Set224pMode();
	
	return;
}