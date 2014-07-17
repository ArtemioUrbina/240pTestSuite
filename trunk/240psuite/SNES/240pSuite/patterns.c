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
#include "font.h"
#include "help.h"
#include <stdio.h>  

void DrawGrid(u8 type) 
{	
	u16 pad0, oldpad = 0xffff, pressed, end = 0;	 
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
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
		
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
		
		WaitForVBlank();
	}	
	setFadeEffect(FADE_OUT);
	Set224pMode();
	
	return;
}


void DrawSMPTE() 
{	
	u16 pad0, oldpad = 0xffff, pressed, end = 0, type = 0;
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
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
		
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
	setFadeEffect(FADE_OUT);	
	
	return;
}

void DrawColorBars(void) 
{	
	u16 pad0, oldpad = 0xffff, pressed, end = 0;	 
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
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
		
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
		
		WaitForVBlank();
	}	
	setFadeEffect(FADE_OUT);	
	
	return;
}

void Drawcircles() 
{	
	u16 pad0, oldpad = 0xffff, pressed, end = 0, grid = 0;
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
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
		
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
		
		WaitForVBlank();
	}	
	setFadeEffect(FADE_OUT);	
	
	return;
}

void DrawPluge() 
{	
	u16 pad0, oldpad = 0xffff, pressed, end = 0;
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
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_PLUGE);
			redraw = 1;
		}
		
		if(pressed == KEY_B)
			end = 1;		

		WaitForVBlank();
	}	
	setFadeEffect(FADE_OUT);	
	
	return;
}

void DrawGrayRamp(void) 
{	
	u16 pad0, oldpad = 0xffff, pressed, end = 0;	 
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
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_GRAY);
			redraw = 1;
		}
		
		if(pressed == KEY_B)
			end = 1;		
		
		WaitForVBlank();
	}	
	setFadeEffect(FADE_OUT);	
	
	return;
}

void DrawColorBleed() 
{	
	u16 pad0, oldpad = 0xffff, pressed, end = 0, toggle = 0;
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
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
		
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
		
		WaitForVBlank();
	}	
	setFadeEffect(FADE_OUT);	
	
	return;
}

void Draw601CB() 
{	
	u16 pad0, oldpad = 0xffff, pressed, end = 0;
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
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_601CB);
			redraw = 1;
		}
		
		if(pressed == KEY_B)
			end = 1;		

		WaitForVBlank();
	}	
	setFadeEffect(FADE_OUT);	
	
	return;
}

void DrawSharpness() 
{	
	u16 pad0, oldpad = 0xffff, pressed, end = 0;
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
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_SHARPNESS);
			redraw = 1;
		}
		
		if(pressed == KEY_B)
			end = 1;		

		WaitForVBlank();
	}	
	setFadeEffect(FADE_OUT);	
	
	return;
}

void DrawWhite() 
{	
	u16 pad0, oldpad = 0xffff, pressed, end = 0;
	u16 redraw = 1, color, change = 1;
	u8	custom = 0, fast = 0;
	int sel = 0, mod = 0, r, g, b;
			
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
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
		
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
				
			if(pressed == KEY_UP || fast && pad0 == KEY_UP)
			{
				int *edit = NULL;
	  
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
				
			if(pressed == KEY_DOWN || fast && pad0 == KEY_DOWN)
			{
				int *edit = NULL;
	  
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
				int *edit = NULL;
	  
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
				int *edit = NULL;
	  
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

		WaitForVBlank();
	}	
	setFadeEffect(FADE_OUT);	
	
	return;
}

void Draw100IRE() 
{	
	u16 pad0, oldpad = 0xffff, pressed, end = 0;
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
							(int)((irevalues[irecount]*100.0)/255.0));					
			  	text --;
			}
			else
			{
				if(text > 30)
					drawText(14, 26, 7, "RANGE 100-140 IRE   ");
				else
					drawText(14, 26, 7, "         %0.3d IRE  ", 
							(int)(100.0f + ((irevalues[irecount] * 40.5)/255.0)));
			  	text --;
			}
			
			if(!text)
				CleanFontMap();
		}
		WaitForVBlank();
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
		
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
	setFadeEffect(FADE_OUT);	
	
	return;
}