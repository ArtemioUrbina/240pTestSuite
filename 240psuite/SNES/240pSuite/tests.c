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
#include "font.h"

void DropShadowTest(void) 
{	
	u16 pad0, oldpad = 0xffff, pressed, end = 0;	 
	u16 redraw = 1, size = 0, changesprite = 1;
	u8	drawShadow = 1;
	int sprite = 0, x = 100, y = 100;
	
	sprite = rand() % 2;
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			size = (&motoko_tiles1_end - &motoko_tiles1);
			bgInitTileSet(0, &motoko_tiles1, &motoko_pal, 0, size, 256*2, BG_256COLORS, 0x2000);	
			dmaCopyVram(&motoko_tiles2, 0x5000, (&motoko_tiles2_end-&motoko_tiles2));
			
			bgInitMapSet(0, &motoko_map, (&motoko_map_end - &motoko_map), SC_32x32, 0x7400);
			
			oamInitGfxSet(&sprites_tiles, (&sprites_tiles_end - &sprites_tiles), &sprites_pal, 16*2, 7, 0x0, OBJ_SIZE32);
			
			//InitTextColor(1, 7, RGB5(0, 31, 0), RGB5(0, 0, 0));
			
			setMode(BG_MODE3,0); 					
			bgSetDisable(1);
			
			bgSetScroll(0, 0, -1);
			setBrightness(0xF);
			redraw = 0;
		}
		
		if(changesprite)
		{
			oamClear(0, 0);
			
			if(sprite)
			{
				oamSet(0, x, y, 3, 0, 0, 8, 7); 
				oamSetEx(0, OBJ_SMALL, OBJ_SHOW);
				oamSetVisible(0, OBJ_SHOW);
			}
			else
			{
				oamSet(0, x, y, 3, 0, 0, 0, 7); 
				oamSetEx(0, OBJ_SMALL, OBJ_SHOW);
				oamSetVisible(0, OBJ_SHOW);
				
				oamSet(4, x+10, y+10, 2, 0, 0, 4, 7); 
				oamSetEx(4, OBJ_SMALL, OBJ_SHOW);
				oamSetVisible(2, OBJ_SHOW);				
			}
			changesprite = 0;
		}
		
		oamSetXY(0, x, y);
		if(!sprite)
			oamSetXY(4, x+10, y+10);
		
		if(drawShadow)
		{
			if(sprite)
				oamSetVisible(0, OBJ_SHOW);
			else
				oamSetVisible(4, OBJ_SHOW);
		}
		else
		{			
			if(sprite)
				oamSetVisible(0, OBJ_HIDE);
			else
				oamSetVisible(4, OBJ_HIDE);
		}
		
		drawShadow = !drawShadow;
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
		
		if(pad0 & KEY_UP)
			y--;
		if(pad0 & KEY_DOWN)
			y++;
		if(pad0 & KEY_LEFT)
		{
			if(sprite)
			{
				oamSet(0, x, y, 3, 0, 0, 8, 7);
			}
			else
			{
				oamSet(0, x, y, 3, 0, 0, 0, 7); 				
				oamSet(4, x+10, y+10, 2, 0, 0, 4, 7); 
			}
			x--;
		}
		
		if(pad0 & KEY_RIGHT)
		{
			if(sprite)
			{
				oamSet(0, x, y, 3, 1, 0, 8, 7);
			}
			else
			{
				oamSet(0, x, y, 3, 1, 0, 0, 7); 				
				oamSet(4, x+10, y+10, 2, 1, 0, 4, 7); 
			}
			x++;
		}
			
		if(x > 216)
			x = 216;
		if(x < 0)
			x = 0;
		if(y > 192)
			y = 192;
		if(y < 0)
			y = 0;
			
		if(pressed == KEY_A)
			end = 1;		
			
		if(pressed == KEY_X)
		{
			sprite = !sprite;
			changesprite = 1;
		}
		
		WaitForVBlank();
	}	
	setFadeEffect(FADE_OUT);	
	
	return;
}

void DrawStripes(void)
{	
	u16 pad0, oldpad = 0xffff, pressed, end = 0, drawframe = 0;	 
	u16 redraw = 1, alternate = 0, pos = 0, vert = 0, frame = 0;
	
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			InitTextColor(0, 7, RGB5(31, 31, 31), RGB5(0, 0, 0));			
			
			if(!vert)
				bgInitTileSet(1, &hstripes_tiles, &grid_pal, 0, (&hstripes_tiles_end - &hstripes_tiles), 16*2, BG_16COLORS, 0x6000);	
			else
				bgInitTileSet(1, &vstripes_tiles, &grid_pal, 0, (&vstripes_tiles_end - &vstripes_tiles), 16*2, BG_16COLORS, 0x6000);	
				
			bgInitMapSet(1, &fullscreen_map, (&fullscreen_map_end - &fullscreen_map), SC_32x32, 0x7000);
			
			setPaletteColor(0x00, RGB5(0, 0, 0));
			setPaletteColor(0x01, RGB5(0xff, 0xff, 0xff));
			
			setMode(BG_MODE1,0); 								
			bgSetDisable(2);
			
			bgSetScroll(0, 0, -1);
			setBrightness(0xF);
			redraw = 0;
		}
		
		if(alternate || pressed == KEY_R)
		{
			if(pos)
			{
				setPaletteColor(0x00, RGB5(0, 0, 0));
				setPaletteColor(0x01, RGB5(0xff, 0xff, 0xff));
			}
			else
			{
				setPaletteColor(0x01, RGB5(0, 0, 0));
				setPaletteColor(0x00, RGB5(0xff, 0xff, 0xff));
			}
			pos = !pos;
		}
		
		if(drawframe)
		{
			drawText(2, 25, 7, "Frame:%0.2d", frame);
			frame ++;
			if(frame == 60)
				frame = 0;			
		}
		
		WaitForVBlank();
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
		
		if(pressed == KEY_B)
			alternate = !alternate;
			
		if(pressed == KEY_Y)
		{
			drawframe = !drawframe;
			if(!drawframe)
				drawText(2, 25, 7, "           ");
		}
			
		if(pressed == KEY_X)
		{
			vert = !vert;
			redraw = 1;
		}		
			
		if(pressed == KEY_A)
			end = 1;				
	}	
	setFadeEffect(FADE_OUT);	
	
	return;
}

void DrawCheck(void)
{	
	u16 pad0, oldpad = 0xffff, pressed, end = 0, drawframe = 0;	 
	u16 redraw = 1, alternate = 0, pos = 0, frame = 0;
	
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			InitTextColor(0, 7, RGB5(31, 31, 31), RGB5(0, 0, 0));		
						
			bgInitTileSet(1, &check_tiles, &grid_pal, 0, (&check_tiles_end - &check_tiles), 16*2, BG_16COLORS, 0x6000);	
				
			bgInitMapSet(1, &fullscreen_map, (&fullscreen_map_end - &fullscreen_map), SC_32x32, 0x7000);
			
			setPaletteColor(0x00, RGB5(0, 0, 0));
			setPaletteColor(0x01, RGB5(0xff, 0xff, 0xff));
			
			setMode(BG_MODE1,0); 								
			bgSetDisable(2);
			
			bgSetScroll(0, 0, -1);
			setBrightness(0xF);
			redraw = 0;
		}
		
		if(alternate || pressed == KEY_R)
		{
			if(pos)
			{
				setPaletteColor(0x00, RGB5(0, 0, 0));
				setPaletteColor(0x01, RGB5(0xff, 0xff, 0xff));
			}
			else
			{
				setPaletteColor(0x01, RGB5(0, 0, 0));
				setPaletteColor(0x00, RGB5(0xff, 0xff, 0xff));
			}
			pos = !pos;
		}
		
		if(drawframe)
		{
			drawText(2, 25, 7, "Frame:%0.2d", frame);
			frame ++;
			if(frame == 60)
				frame = 0;			
		}
		
		WaitForVBlank();
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
		
		if(pressed == KEY_B)
			alternate = !alternate;
			
		if(pressed == KEY_Y)
		{
			drawframe = !drawframe;
			if(!drawframe)
				drawText(2, 25, 7, "           ");
		}
			
		if(pressed == KEY_A)
			end = 1;				
	}	
	setFadeEffect(FADE_OUT);	
	
	return;
}