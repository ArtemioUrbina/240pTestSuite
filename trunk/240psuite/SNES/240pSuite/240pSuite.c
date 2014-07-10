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

int main(void) 
{
	u16 sel = 0, redraw = 1;
	u16 pad0, oldpad = 0xffff, pressed;
	    
	setBrightness(0);
	
	consoleInit();	
	consoleInitText(0, 0, &font);	

	// Main loop
	while(1) 
	{
		u16 change = 0;				
		
		if(redraw)
		{
			u16 size = 0;
					
			setBrightness(0);	
						
			size = (&back_tiles_end - &back_tiles);
			bgInitTileSet(1, &back_tiles, &back_pal, 1, size, 16*2, BG_16COLORS, 0x6000);			
			
			size = (&back_map_end - &back_map);	
			bgInitMapSet(1, &back_map, size, SC_32x32, 0x2000);
			
			size = (&gillian_tiles_end-&gillian_tiles);
			DrawTilesWithSprites(160, 80, 64, 112, &gillian_tiles, size, &gillian_pal);
			
			consoleDrawText(4, 6, "Grid");
			//consoleDrawText(4, 7, "Horizontal stripes");	
						
			setMode(BG_MODE1,0); 	
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);						
			
			setBrightness(0xF);
			WaitForVBlank();						
			redraw = 0;
		}			
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
					
		if(pressed == KEY_DOWN)
		{
			sel+=1;
			change = 1;
		}
		if(pressed == KEY_UP)
		{			
			sel-=1;
			change = 1;		
		}	

		if(sel != 0)
			sel = 0;
			
		if(pressed == KEY_B)
		{							
			setFadeEffect(FADE_OUT);		
			oamClear(0, 0);
			
			DrawGrid();
			redraw = 1;
		}
		
		WaitForVBlank();
	}
	return 0;
}
