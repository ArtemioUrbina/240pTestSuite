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
#include "tests.h"
#include "patterns.h"
#include "help.h"
#include "control.h"
#include "hardware.h"

u16 CheckController(u16 joypad, u16 oldButtons, u16 y)
{
	u16	buttons, x = 3;
	
	buttons = PadPressed(joypad);
	
	drawText(x, y-2, buttons & KEY_L ? 5 : 2, "L-Trigger");
	drawText(x+17, y-2, buttons & KEY_R ? 5 : 2, "R-Trigger");
	
	drawText(x+4, y, buttons & KEY_UP ? 5 : 7, "Up");
	drawText(x, y+1, buttons & KEY_LEFT ? 5 : 7, "Left");
	drawText(x+6, y+1, buttons & KEY_RIGHT ? 5 : 7, "Right");
	drawText(x+3, y+2, buttons & KEY_DOWN ? 5 : 7, "Down");
	
	x+=17;
	drawText(x+3, y, buttons & KEY_X ? 5 : 3, "X");
	drawText(x, y+1, buttons & KEY_Y ? 5 : 3, "Y");
	drawText(x+6, y+1, buttons & KEY_A ? 5 : 4, "A");
	drawText(x+3, y+2, buttons & KEY_B ? 5 : 4, "B");
	
	x=15; 
	drawText(x, y, buttons & KEY_SELECT ? 4 : 7, "SEL");
	drawText(x-1, y+2, buttons & KEY_START ? 4 : 7, "START");
}

void ControllerTest()
{	
	int	change = 1;
	u16 pressed, end = 0, pad1 = 0xffff, pad2 = 0xffff;	 
	u16 redraw = 1, size = 0;	
	
	
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
			AddTextColor(4, RGB5(9,8,21), RGB5(0, 0, 0));	
			AddTextColor(3, RGB5(22,22,27), RGB5(0, 0, 0));	
			AddTextColor(2, RGB5(25,24,24), RGB5(0, 0, 0));	
			
			size = (&back_tiles_end - &back_tiles);
			bgInitTileSetMine(1, &back_tiles, &back_pal, 1, size, 16*2, BG_16COLORS, 0x6000);			
			
			size = (&back_map_end - &back_map);	
			bgInitMapSetMine(1, &back_map, size, SC_32x32, 0x2000);
			
			drawText(8, 4, 6, "Controller Test");
			drawText(6, 24, 6, "Use START+LEFT P1 to exit");
			
			setMode(BG_MODE1,0); 	
			bgSetDisable(2);

			EndDMA();			
			
			change = 1;
			redraw = 0;
		}
		
		pad1 = CheckController(0, pad1, 9);
		pad2 = CheckController(1, pad2, 17);
		
		WaitForVBlank();
		
		pressed = PadHeld(0);
		
		if(pressed & KEY_START &&
			pressed & KEY_LEFT)
			end = 1;

	}	
	Transition();
	
	return;
}

#define XSIZE	1
void MemoryViewer()
{	
	u16 address = 0x8000;
	u16 pressed, end = 0;	 
	u16 redraw = 1;	
	
	while(!end) 
	{		
		if(redraw)
		{
			u8 *mem = NULL, i = 0, j = 0;
			
			StartDMA();
			setPaletteColor(0x00, RGB5(0, 0, 0));
			consoleInitTextMine(0, 7, &font);
			
			AddTextColor(7, RGB5(31, 31, 31), RGB5(0, 0, 0));
			AddTextColor(6, RGB5(0, 31, 0), RGB5(0, 0, 0));			
			AddTextColor(5, RGB5(31, 0, 0), RGB5(0, 0, 0));	
			AddTextColor(4, RGB5(28, 28, 0), RGB5(0, 0, 0));	
			
			setMode(BG_MODE1,0); 	
			bgSetDisable(1);
			bgSetDisable(2);	

			drawText(28, 0, 5, "%0.4X", address);	
			drawText(28, 27, 5, "%0.4X", address+28*XSIZE);	
			mem = (u8*)address;
			for(i = 0; i < 28; i++)
			{
				for(j = 0; j <XSIZE; j++)
				{
					drawText(j*2, i, 7, "%0.4X:%0.2X", i*XSIZE+j, (u8)mem[i*XSIZE+j]);	
				}
			}	
			EndDMA();	
			
			redraw = 0;
		}
		
		WaitForVBlank();
		
		pressed = PadHeld(0);
		
		if(pressed & KEY_START &&
			pressed & KEY_LEFT)
			end = 1;

	}	
	Transition();
	
	return;
}