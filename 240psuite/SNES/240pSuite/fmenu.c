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

#include "font.h"
#include "res.h"
#include "video.h"
#include "fmenu.h"

u16 DrawFloatMenu()
{	
	int	sel = 0, change = 1;
	u16 pressed, end = 0, x = 0, y = 0, ix = 0, iy = 0;	 
	u16 redraw = 1, size = 0;	
	fmenudata resmenudata[] = { {FLOAT_CANCEL, "Display Mode"}, {RES_224, "256x224"}, {RES_239, "256x239"} };
	
	sel = 1;
	
	while(!end) 
	{		
		if(redraw)
		{
			StartDMA();
			
			setPaletteColor(0x00, RGB5(0, 0, 0));
			consoleInitTextMine(0, 7, &font);
			
			AddTextColor(7, RGB5(31, 31, 31), RGB5(0, 0, 0));
			AddTextColor(6, RGB5(0, 31, 0), RGB5(0, 0, 0));			
			AddTextColor(5, RGB5(31, 0, 0), RGB5(0, 0, 0));	
			AddTextColor(4, RGB5(28, 28, 0), RGB5(0, 0, 0));	
			
			size = (&float_tiles_end - &float_tiles);
			bgInitTileSetMine(1, &float_tiles, &float_pal, 0, size, 16*2, BG_16COLORS, 0x4000);		
			
			bgInitMapSetMine(1, &float_map, (&float_map_end - &float_map), SC_32x32, 0x1000);
			
			setMode(BG_MODE1,0); 	
			bgSetDisable(2);
			
			bgSetScroll(0, 0, -2);
			EndDMA();
			
			change = 1;
			redraw = 0;
		}
		
		if(change)
		{
			x = 10;
			y = 9;
			drawText(x, y, 6, resmenudata[0].name); 
			y+= 4;
			drawText(x+2, y++, sel == 1 ? 5 : 7, resmenudata[1].name); 
			drawText(x+2, y++, sel == 2 ? 5 : 7, resmenudata[2].name); 
			
		}
		WaitForVBlank();
		
		pressed = PadPressed(0);
		
		if(pressed & KEY_START ||
			pressed & KEY_B)
			return FLOAT_CANCEL;

		if(pressed & KEY_DOWN)
		{
			sel++;
			change = 1;
		}
		if(pressed & KEY_UP)
		{			
			sel--;
			change = 1;		
		}	

		if(sel < 1)
			sel = 2;
			
		if(sel > 2)
			sel = 1;		
		
		if(pressed & KEY_A)
			end = 1;
	}	
	Transition();
	
	return resmenudata[sel].id;
}