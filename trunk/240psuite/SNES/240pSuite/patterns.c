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
 
#include <stdio.h>  
#include <stdlib.h>  
#include "patterns.h"
#include "font.h"
#include "help.h"
#include "control.h"

void DrawGrid(u8 type) 
{	
	u16 pressed, end = 0;	 
	u16 redraw = 1;
		
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			bgInitTileSet(1, &grid_tiles, &grid_pal, 0, (&grid_tiles_end - &grid_tiles), 16*2, BG_16COLORS, 0x4000);	
			
			if(type)
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
		WaitForVBlank();
		
		pressed = PadPressed(0);
		
		if(pressed == KEY_START)
		{
			if(type)
			{
				Set224pMode();
				DrawHelp(HELP_GRID_239);				
			}
			else
				DrawHelp(HELP_GRID);
			redraw = 1;
		}
		
		if(pressed == KEY_B)
			end = 1;		
	}	
	Transition();
	Set224pMode();
	
	return;
}


void DrawSMPTE() 
{	
	u16 pressed, end = 0, type = 0;
	u16 redraw = 1, text = 0;
				
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			consoleInitText(0, 7, &font);				
			
			setPaletteColor(0x71, RGB5(31, 31, 31));			
			bgInitTileSet(1, &SMPTECB75_tiles, &SMPTECB75_pal, 0, (&SMPTECB75_tiles_end - &SMPTECB75_tiles), 16*2, BG_16COLORS, 0x4000);	
			
			bgInitMapSet(1, &SMPTECB75_map, (&SMPTECB75_map_end - &SMPTECB75_map), SC_32x32, 0x2000);
			
			setMode(BG_MODE1,0); 			
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);
			setBrightness(0xF);
			redraw = 0;
		}
		WaitForVBlank();
		
		if(text)
		{
			text --;
			if(!text)
				CleanFontMap();
		}
		
		pressed = PadPressed(0);
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_SMPTE);
			redraw = 1;
		}
		
		if(pressed == KEY_B)
			end = 1;		
		
		if(pressed == KEY_A)
		{
			if(type)
			{
				drawText(26, 1, 7, " 75%");
				WaitForVBlank();
				dmaCopyCGram(&SMPTECB75_pal, 0, 16*2);
			}
			else
			{
				drawText(26, 1, 7, "100%");
				WaitForVBlank();
				dmaCopyCGram(&SMPTECB100_pal, 0, 16*2);
			}
			text = 30;
			type = !type;
		}		
	}	
	Transition();
	
	return;
}

void DrawColorBars(void) 
{	
	u16 pressed, end = 0;	 
	u16 redraw = 1, size = 0, type = 0;	
	
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			if(!type)
			{
				size = (&color_tiles_end - &color_tiles);
				bgInitTileSet(0, &color_tiles, &color_pal, 0, size, 128*2, BG_256COLORS, 0x0000);		
	
				bgInitMapSet(0, &color_map, (&color_map_end - &color_map), SC_32x32, 0x7000);
			}
			else
			{
				size = (&color_grid_tiles_end - &color_grid_tiles);
				bgInitTileSet(0, &color_grid_tiles, &color_pal, 0, size, 128*2, BG_256COLORS, 0x0000);		
	
				bgInitMapSet(0, &color_grid_map, (&color_grid_map_end - &color_grid_map), SC_32x32, 0x7000);
			}
						
			setMode(BG_MODE3,0); 					
			bgSetDisable(1);
			
			bgSetScroll(0, 0, -1);
			setBrightness(0xF);
			redraw = 0;
		}
		WaitForVBlank();
		
		pressed = PadPressed(0);
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_COLORS);
			redraw = 1;
		}
		
		if(pressed == KEY_A)
		{
			type = !type;
			redraw = 1;
		}
		
		if(pressed == KEY_B)
			end = 1;		
	}	
	Transition();
	
	return;
}

void Drawcircles() 
{	
	u16 pressed, end = 0, grid = 0;
	u16 redraw = 1, grid = 0;
		
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			if(grid)
			{
				bgInitTileSet(0, &circlesgrid_tiles, &grid_pal, 0, (&circlesgrid_tiles_end - &circlesgrid_tiles), 16*2, BG_16COLORS, 0x6000);
				bgInitMapSet(0, &fullscreen_map, (&fullscreen_map_end - &fullscreen_map), SC_32x32, 0x7000);
			}
			
			bgInitTileSet(1, &circles_tiles, &circles_pal, 0, (&circles_tiles_end - &circles_tiles), 16*2, BG_16COLORS, 0x4000);	
			bgInitMapSet(1, &circles_map, (&circles_map_end - &circles_map), SC_32x32, 0x1000);
			
			setMode(BG_MODE1,0); 
			if(!grid)
				bgSetDisable(0);
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);
			setBrightness(0xF);
			redraw = 0;
		}
		WaitForVBlank();
		
		pressed = PadPressed(0);
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_LINEARITY);
			redraw = 1;
		}
		
		if(pressed == KEY_B)
			end = 1;		
		
		if(pressed == KEY_A)
		{
			grid = !grid;
			redraw = 1;
		}
	}	
	Transition();
	
	return;
}

void DrawPluge() 
{	
	u16 pressed, end = 0;
	u16 redraw = 1;
		
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			bgInitTileSet(1, &pluge_tiles, &pluge_pal, 0, (&pluge_tiles_end - &pluge_tiles), 16*2, BG_16COLORS, 0x4000);	
			bgInitMapSet(1, &pluge_map, (&pluge_map_end - &pluge_map), SC_32x32, 0x1000);
			
			setMode(BG_MODE1,0); 
			bgSetDisable(0);		
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);
			setBrightness(0xF);
			redraw = 0;
		}
		WaitForVBlank();
		
		pressed = PadPressed(0);
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_PLUGE);
			redraw = 1;
		}
		
		if(pressed == KEY_B)
			end = 1;
	}	
	Transition();
	
	return;
}

void DrawGrayRamp(void) 
{	
	u16 pressed, end = 0;	 
	u16 redraw = 1, size = 0;	
	
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			size = (&grayramp_tiles_end - &grayramp_tiles);
			bgInitTileSet(0, &grayramp_tiles, &grayramp_pal, 0, size, 128*2, BG_256COLORS, 0x0000);		
			
			bgInitMapSet(0, &grayramp_map, (&grayramp_map_end - &grayramp_map), SC_32x32, 0x7000);
						
			setMode(BG_MODE3,0); 					
			bgSetDisable(1);
			
			bgSetScroll(0, 0, -1);
			setBrightness(0xF);
			redraw = 0;
		}
		WaitForVBlank();
		
		pressed = PadPressed(0);
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_GRAY);
			redraw = 1;
		}
		
		if(pressed == KEY_B)
			end = 1;		
	}	
	Transition();
	
	return;
}

void DrawColorBleed() 
{	
	u16 pressed, end = 0, toggle = 0;
	u16 redraw = 1;
		
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			if(toggle)
				bgInitTileSet(1, &colorbleedchk_tiles, &colorbleed_pal, 0, (&colorbleedchk_tiles_end - &colorbleedchk_tiles), 16*2, BG_16COLORS, 0x4000);	
			else
				bgInitTileSet(1, &colorbleed_tiles, &colorbleed_pal, 0, (&colorbleed_tiles_end - &colorbleed_tiles), 16*2, BG_16COLORS, 0x4000);				
				
			bgInitMapSet(1, &colorbleed_map, (&colorbleed_map_end - &colorbleed_map), SC_32x32, 0x1000);
			
			setMode(BG_MODE1,0); 
			bgSetDisable(0);		
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);
			setBrightness(0xF);
			redraw = 0;
		}
		WaitForVBlank();
		
		pressed = PadPressed(0);
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_BLEED);
			redraw = 1;
		}
		
		if(pressed == KEY_B)
			end = 1;		

		if(pressed == KEY_A)
		{
			toggle = !toggle;			
			redraw = 1;
		}		
	}	
	Transition();
	
	return;
}

void Draw601CB() 
{	
	u16 pressed, end = 0;
	u16 redraw = 1;
		
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			bgInitTileSet(1, &cb601701_tiles, &cb601701_pal, 0, (&cb601701_tiles_end - &cb601701_tiles), 16*2, BG_16COLORS, 0x4000);	
			bgInitMapSet(1, &cb601701_map, (&cb601701_map_end - &cb601701_map), SC_32x32, 0x1000);
			
			setMode(BG_MODE1,0); 
			bgSetDisable(0);		
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);
			setBrightness(0xF);
			redraw = 0;
		}
		WaitForVBlank();
		
		pressed = PadPressed(0);
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_601CB);
			redraw = 1;
		}
		
		if(pressed == KEY_B)
			end = 1;
	}	
	Transition();
	
	return;
}

void DrawSharpness() 
{	
	u16 pressed, end = 0;
	u16 redraw = 1;
		
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			bgInitTileSet(1, &sharpness_tiles, &sharpness_pal, 0, (&sharpness_tiles_end - &sharpness_tiles), 16*2, BG_16COLORS, 0x4000);	
			bgInitMapSet(1, &sharpness_map, (&sharpness_map_end - &sharpness_map), SC_32x32, 0x1000);
			
			setMode(BG_MODE1,0); 
			bgSetDisable(0);		
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);
			setBrightness(0xF);
			redraw = 0;
		}
		WaitForVBlank();
		
		pressed = PadPressed(0);
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_SHARPNESS);
			redraw = 1;
		}
		
		if(pressed == KEY_B)
			end = 1;		
	}	
	Transition();
	
	return;
}

void DrawWhite() 
{	
	u16 pressed, held, end = 0;
	u16 redraw = 1, color, change = 1;
	u8	custom = 0, fast = 0;
	s16 sel = 0, mod = 0, r, g, b;
			
	r = g = b = 255;
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			InitTextColor(1, 7, RGB5(31, 31, 31), RGB5(0, 0, 0));
			AddTextColor(6, RGB5(0, 31, 0), RGB5(0, 0, 0));
			
			ClearScreen256(0);
						
			setMode(BG_MODE3,0); 			
			bgSetDisable(2);
			
			bgSetScroll(0, 0, -1);
			setBrightness(0xF);
			redraw = 0;
		}
		
		if(change)
		{
			switch(sel)
			{
				case 0:
					color = RGB8(r, g, b);
					break;
				case 1:
					color = RGB8(0, 0, 0);
					break;
				case 2:
					color = RGB8(255, 0, 0);
					break;
				case 3:
					color = RGB8(0, 255, 0);
					break;
				case 4:
					color = RGB8(0, 0, 255);
					break;
			}

			if(custom && sel == 0)
			{				
				drawText(12, 1, mod == 0 ? 6 : 7, "R:%0.3d", r);			
				drawText(18, 1, mod == 1 ? 6 : 7, "G:%0.3d", g);				
				drawText(24, 1, mod == 2 ? 6 : 7, "B:%0.3d", b);
			}
			else			
				CleanFontMap();
			
			setPaletteColor(0x00, color);			
			change = 0;
		}
		
		WaitForVBlank();
		
		pressed = PadPressed(0);
		held = PadHeld(0);
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_WHITE);
			redraw = 1;
			change = 1;
		}
		
		if(pressed == KEY_A)
		{
			custom = !custom;	
			change = 1;
		}
			
		if(pressed == KEY_B)
			end = 1;	

		if(pressed == KEY_R)
		{
			sel ++;
			change = 1;
		}
			
		if(pressed == KEY_L)
		{
			sel --;
			change = 1;
		}
			
		if(sel > 4)
			sel = 0;
		if(sel < 0)
			sel = 4;
			
		if(custom)
		{
			if(pressed == KEY_SELECT)
				fast = !fast;
			
			if(pressed == KEY_RIGHT)
			{
				mod ++;
				change = 1;
			}
				
			if(pressed == KEY_LEFT)
			{
				mod --;
				change = 1;
			}
				
			if(mod > 2)
				mod = 0;
			if(mod < 0)
				mod = 2;
				
			if(pressed == KEY_UP || fast && held == KEY_UP)
			{
				s16 *edit = NULL;
	  
				switch(mod)
				{
					case 0:
						edit = &r;
					break;
					case 1:
						edit = &g;
						break;
					case 2:
						edit = &b;
						break;
				}
				  
				if(edit)
				{  		
					if(*edit != 0xff)
						(*edit) += 1;
									
					change = 1;
				}  
			}
				
			if(pressed == KEY_DOWN || fast && held == KEY_DOWN)
			{
				s16 *edit = NULL;
	  
				switch(mod)
				{
					case 0:
						edit = &r;
					break;
					case 1:
						edit = &g;
						break;
					case 2:
						edit = &b;
						break;
				}
				  
				if(edit)
				{  		
					if(*edit != 0x00)
						(*edit) -= 1;
									
					change = 1;
				}	 
			}
			
			if(pressed == KEY_Y)
			{
				s16 *edit = NULL;
	  
				switch(mod)
				{
					case 0:
						edit = &r;
					break;
					case 1:
						edit = &g;
						break;
					case 2:
						edit = &b;
						break;
				}
				  
				if(edit)
				{  		
					*edit = 0;
					
					change = 1;
				}  
			}
				
			if(pressed == KEY_X)
			{
				s16 *edit = NULL;
	  
				switch(mod)
				{
					case 0:
						edit = &r;
					break;
					case 1:
						edit = &g;
						break;
					case 2:
						edit = &b;
						break;
				}
				  
				if(edit)
				{  		
					*edit = 0xff;
									
					change = 1;
				}	 
			}
		}
	}	
	Transition();
	
	return;
}

void Draw100IRE() 
{	
	u16 pressed, end = 0;
	u16 redraw = 1;
	int	irecount = 10, iremax = 10, changedire = 0; 
	u8	*irevalues = NULL, invert = 0, text = 0;
	u8	irevalues100[11] = { 0, 26, 51, 77, 102, 128, 153, 179, 204, 230, 255};
	u8	irevalues140[5] = { 0, 63, 127, 190, 255 };	
		
	irevalues = irevalues100;
	while(!end) 
	{		
		if(redraw)
		{			
			setBrightness(0);
				
			WaitForVBlank();		
			bgInitTileSet(0, &IRE100_tiles, &grid_pal, 0, (&IRE100_tiles_end - &IRE100_tiles), 16*2, BG_256COLORS, 0x4000);	
			bgInitMapSet(0, &IRE100_map, (&IRE100_map_end - &IRE100_map), SC_32x32, 0x2000);
			
			setPaletteColor(0x00, RGB8(0, 0, 0));
			setPaletteColor(0x01, RGB8(0xff, 0xff, 0xff));
			
			InitTextColor(1, 7, RGB5(31, 31, 31), RGB5(0, 0, 0));
			
			setMode(BG_MODE3,0);			
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);
			setBrightness(0xF);
			redraw = 0;
		}
		
		if(changedire)
		{
			setPaletteColor(!invert, RGB8(irevalues[irecount], irevalues[irecount], irevalues[irecount]));
			changedire = 0;			
		}
		
		if(text)
		{
			if(!invert)
			{
				if(text > 30)
					drawText(14, 26, 7, "RANGE 0-100 IRE   ");
				else
					drawText(14, 26, 7, "         %0.3d IRE", 
							(s16)((irevalues[irecount]*100.0)/255.0));					
			  	text --;
			}
			else
			{
				if(text > 30)
					drawText(14, 26, 7, "RANGE 100-140 IRE   ");
				else
					drawText(14, 26, 7, "         %0.3d IRE  ", 
							(s16)(100.0f + ((irevalues[irecount] * 40.5)/255.0)));
			  	text --;
			}
			
			if(!text)
				CleanFontMap();
		}
		WaitForVBlank();
		
		pressed = PadPressed(0);
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_IRE);
			redraw = 1;
		}
		
		if(pressed == KEY_B)
			end = 1;		
			
		if(pressed == KEY_A)
		{
			invert = !invert;
			if(invert)
			{
				irecount = 0;
				iremax = 4;
				irevalues = irevalues140;
			}
			else
			{
				irecount = 10;
				iremax = 10;
				irevalues = irevalues100;
			}
			
			WaitForVBlank();
			setPaletteColor(0x00, RGB8(0, 0, 0));
			setPaletteColor(0x01, RGB8(0xff, 0xff, 0xff));
			
			text = 60;
			changedire = 1;
		}
			
		if(pressed == KEY_L)
		{			
			if(irecount > 0)
			{
				irecount--;	
				changedire = 1;
			}
			text = 30;
		}
			
		if(pressed == KEY_R)
		{
			irecount ++;
				
			if(irecount > iremax)
				irecount = iremax;	
			changedire = 1;
			text = 30;
		}		
	}	
	Transition();
	
	return;
}

u8 *empty_over, *tile_l, *tile_r, *tile_t, *tile_b;
u8 *full_over, *tile_lb, *tile_lt, *tile_rt, *tile_rb;
u8 *tiles_over = NULL;
u8 *map_over = NULL;

inline void CleanTile(u8 *array)
{
	memset(array, 0x00, 0x20);
}

inline void FillTile(u8 *array)
{
	s16 i = 0;
	
	// fill all bitplanes
	for(i = 0; i < 8; i++)
		array[i<<1] = 0xFF;
}

inline void place_tile_in_map(u16 x, u16 y, u8 *map, u8 pal, u16 tileNum) 
{
	u16 x1; 
    
	x1 = y*0x40 + x*2;

	// Write tile to screen
	map[x1] = tileNum;
	x1++;
	map[x1] = (pal<<2) | (1<<5);
}

inline void FillTiles(s16 left, s16 right, s16 top, s16 bottom, u8 mode)
{
	u8 mask;
	s16 i, j, maxw, maxh;
	s16 lp, rp, tp, bp;
	s16 l, r, t, b, index = 0;
  
	maxw = 31;
	maxh = 27;
	
	if(mode)
		maxh = 29;
	
	lp = left >> 3;
	rp = right >> 3;
	tp = top >> 3;
	bp = bottom >> 3;
	
	l = left % 8;
	r = right % 8;
	t = top % 8;
	b = bottom % 8;
	
	CleanTile(tile_l);
	CleanTile(tile_r);
	CleanTile(tile_t);
	CleanTile(tile_b);
	CleanTile(tile_lt);
	CleanTile(tile_lb);
	CleanTile(tile_rt);
	CleanTile(tile_rb);
	
	/*-------- Tiles -------*/
	
	// Top	
	for(i = 0; i < t; i++)
	{
		index = i << 1;
		tile_t[index] = 0xFF;
		tile_rt[index] = 0xFF;
		tile_lt[index] = 0xFF;
	}
	
	// Bottom  	
	for(i = 0; i < b; i++)
	{
		index = 14 - (i << 1);
		tile_b[index] = 0xFF;
		tile_rb[index] = 0xFF;
		tile_lb[index] = 0xFF;
	}
	
	// left
	mask = 0x00;
	for(i = 0; i < l; i++)
		mask |= 1 << (7 - i);
		
	for(i = 0; i < 8; i++)
	{  	
		index = i << 1;
		tile_l[index] = mask;  
		tile_lt[index] |= mask;
		tile_lb[index] |= mask;	
	}
	
	// right
	mask = 0x00;
	for(i = 0; i < r; i++)
		mask |= 1 << i;
		
	for(i = 0; i < 8; i++)
	{  	
		index = i << 1;
		tile_r[index] = mask;  
		tile_rt[index] |= mask;
		tile_rb[index] |= mask;	
	}
	
	/*-------- Maps -------*/
	
	// Clean map
	memset(map_over, 0, 0x800);
	
	// left map
	for(i = tp; i < maxh-bp; i++)
		place_tile_in_map(lp, i, map_over, 1, 1);
		
	// right map
	for(i = tp; i < maxh-bp; i++)
		place_tile_in_map(maxw-rp, i, map_over, 1, 2);
		
	// top map
	for(i = lp+1; i < maxw-rp; i++)
		place_tile_in_map(i, tp, map_over, 1, 3);
	
	// bottom map
	for(i = lp+1; i < maxw-rp; i++)
		place_tile_in_map(i, maxh-bp, map_over, 1, 4);
		
	// corners	
	place_tile_in_map(lp, maxh-bp, map_over, 1, 5);
	place_tile_in_map(lp, tp, map_over, 1, 6);	
	place_tile_in_map(maxw-rp, tp, map_over, 1, 7);
	place_tile_in_map(maxw-rp, maxh-bp, map_over, 1, 8);
	
	// Whites
	maxh++;
	maxw++;
	
	// left
	for(i = 0; i < lp; i++)
		for(j = 0; j < maxh; j++)
			place_tile_in_map(i, j, map_over, 1, 9);
		
	// right map
	for(i = maxw-rp; i < maxw; i++)
		for(j = 0; j < maxh; j++)
			place_tile_in_map(i, j, map_over, 1, 9);
			
	// top map
	for(j = 0; j < tp; j++)		
		for(i = 0; i < maxw; i++)
			place_tile_in_map(i, j, map_over, 1, 9);
	
	// bottom map
	for(j = maxh-bp; j < maxh; j++)
		for(i = 0; i < maxw; i++)
			place_tile_in_map(i, j, map_over, 1, 9);	
	
	WaitForVBlank();
	dmaCopyVram(tiles_over, 0x4000, 0x140<<1);  
	
	if(mode) // doesn't end the copy if in 240p mode...
	{
		setBrightness(0x0);
		WaitForVBlank();
	}
	
	dmaCopyVram(map_over, 0x2000, 0x800);
	if(mode) // doesn't end the copy if in 240p mode...
		setBrightness(0xf);		
}

void DrawOverscan(u8 mode)
{
	u16 pressed, end = 0, held = 0, changed = 0;
	u16 redraw = 1, changedval = 1, i = 0;
	s16 top, bottom, left, right, sel = 0;	
	
	tiles_over = (u8*)malloc(sizeof(u8)*0x140);
	if(!tiles_over)
		return;
		
	map_over = (u8*)malloc(sizeof(u8)*0x800);
	if(!map_over)
	{
		if(tiles_over)
		{
			free(tiles_over);
			tiles_over = NULL;
		}
		return;
	}
		
	empty_over = &tiles_over[0x00];
	tile_l = &tiles_over[0x20];
	tile_r = &tiles_over[0x40];
	tile_t = &tiles_over[0x60];
	tile_b = &tiles_over[0x80];
	tile_lb = &tiles_over[0xA0];
	tile_lt = &tiles_over[0xC0];
	tile_rt = &tiles_over[0xE0];
	tile_rb = &tiles_over[0x100];
	full_over = &tiles_over[0x120];
	
	top = bottom = left = right = 0;
	
	if(mode)
		bottom = 1;  // deal with 239
	
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			if(mode)
				Set240pMode();	
					
			CleanTile(empty_over);
			CleanTile(full_over);
			FillTile(full_over);
			
			bgSetGfxPtr(1, 0x4000);
			bgSetMapPtr(1, 0x2000, SC_32x32);
			
			consoleInitText(0, 7, &font);				
			setPaletteColor(0x61, RGB5(0, 12, 27));
			setPaletteColor(0x51, RGB5(0, 27, 27));
			
			setPaletteColor(0x11, RGB5(31, 31, 31));
		
			ClearScreen(2);
			setPaletteColor(0x00, RGB5(17, 17, 17));
			changed = 1;
			changedval = 1;
		}
		
		if(changed)
		{
			if(changedval)
			{
				FillTiles(left, right, top, bottom, mode);
				changedval = 0;
			}
			
			drawText(7, 12+mode, sel == 0 ? 6 : 7, "Top:    %0.2d pixels", top);
			drawText(7, 13+mode, sel == 1 ? 6 : 7, "Bottom: %0.2d pixels", mode ? bottom - 1 : bottom);
			drawText(7, 14+mode, sel == 2 ? 6 : 7, "Left:   %0.2d pixels", left);
			drawText(7, 15+mode, sel == 3 ? 6 : 7, "Right:  %0.2d pixels", right);
			if(mode)
			{
				drawText(2, 20, 5, "Screen flashing is normal in");
				drawText(2, 21, 5, "239p due to forced vblanking");
				drawText(2, 22, 5, "in order to DMA into VRAM.");
			}
			
			changed = 0;
			if(redraw)
			{
				setMode(BG_MODE1,0); 			
				bgSetScroll(0, 0, -1);
				bgSetScroll(1, 0, -1);
				bgSetScroll(2, 0, -1);
				setBrightness(0xF);
				redraw = 0;
			}
		}
		
		pressed = PadPressed(0);
		held = PadHeld(0);
		
		if(pressed == KEY_UP)
		{
			sel --;
			changed = 1;
		}
		
		if(pressed == KEY_DOWN)
		{
			sel ++;
			changed = 1;
		}
		
		if(changed)
		{
			if(sel > 3)
				sel = 0;
			if(sel < 0)
				sel = 3;
		}
		
		if (pressed == KEY_LEFT || held == KEY_L)    
		{ 
			s16 *data = NULL;

			switch(sel)
			{
				case 0:
					data = &top;
					break;
				case 1:
					data = &bottom;
					break;
				case 2:
					data = &left;
					break;
				case 3:
					data = &right;
					break;
			} 

			if(data)
			{
				(*data) --;
				if(*data < 0)
					*data = 0;
			}
			
			if(sel == 1 && mode && bottom == 0)
				bottom = 1;
				
			changed = 1;
			changedval = 1;
		}

		if (pressed == KEY_RIGHT || held == KEY_R)     
		{
			s16 *data = NULL;

			switch(sel)
			{
				case 0:
					data = &top;
					break;
				case 1:
					data = &bottom;
					break;
				case 2:
					data = &left;
					break;
				case 3:
					data = &right;
					break;
			} 

			if(data)
			{
				(*data) ++;
				if(*data > 99)
					*data = 99;
			} 
			changed = 1;
			changedval = 1;
		}
		
		if(pressed == KEY_START)
		{
			if(mode)
				Set224pMode();
			DrawHelp(HELP_OVERSCAN);
			if(mode)
				Set240pMode();
			redraw = 1;
		}
		
		if(pressed == KEY_A)
		{
			top = bottom = left = right = 0;
			if(mode)
				bottom = 1;
			changed = 1;
			changedval = 1;
		}
		
		if(pressed == KEY_B)
			end = 1;	
			
		WaitForVBlank();
	}
	
	if(tiles_over)
	{
		free(tiles_over);
		tiles_over = NULL;
	}
	if(map_over)
	{
		free(map_over);
		map_over = NULL;
	}
	if(mode)
		Set224pMode();
}

void DrawMode7()
{
	u16 held, end = 0, pressed = 0;
	u8 angle=0, redraw = 1;;
		
	while(!end) 
	{	
		if(redraw)
		{
			bgInitMapTileSet7(&mode7_tiles,  &mode7_map, &grid_pal, (&mode7_tiles_end-&mode7_tiles), 0x0000);	
			setMode7(0); 
			setMode7Rot(angle);
			redraw = 0;
		}
		
		scanPads();
		held = PadHeld(0);
		pressed = PadPressed(0);
		
		if(held == KEY_L || pressed == KEY_LEFT)
		{
			angle--; 
			setMode7Rot(angle);
		}
		
		if(held == KEY_R || pressed == KEY_RIGHT)
		{
			angle++; 
			setMode7Rot(angle);
		}
			
		if(pressed == KEY_A)
		{
			angle = 0;
			setMode7Rot(angle);
		}
		
		if(pressed == KEY_START)
		{
			consoleInit();
			DrawHelp(HELP_MODE7);
			redraw = 1;
		}
		
		if(pressed == KEY_B)
			end = 1;
		
		WaitForVBlank();
	}
	consoleInit();
}