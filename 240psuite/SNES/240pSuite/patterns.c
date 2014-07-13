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
		
		if(pressed == KEY_A)
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
			
			bgInitMapSet(1, &SMPTECB75_map, (&SMPTECB75_map_end - &SMPTECB75_map), SC_32x32, 0x1000);
			
			setMode(BG_MODE1,0); 			
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);
			setBrightness(0xF);
			redraw = 0;
		}
		
		if(text)
		{
			text --;
			if(!text)
				drawText(26, 1, 7, "    ");
		}
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
		
		if(pressed == KEY_A)
			end = 1;		
		
		if(pressed == KEY_B)
		{
			if(type)
			{
				drawText(26, 1, 7, " 75%");
				dmaCopyCGram(&SMPTECB75_pal, 0, 16*2);				
			}
			else
			{
				drawText(26, 1, 7, "100%");
				dmaCopyCGram(&SMPTECB100_pal, 0, 16*2);				
			}
			text = 30;
			type = !type;
		}
		
		WaitForVBlank();
	}	
	setFadeEffect(FADE_OUT);	
	
	return;
}

void DrawColorBars(void) 
{	
	u16 pad0, oldpad = 0xffff, pressed, end = 0;	 
	u16 redraw = 1, size = 0;	
	
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			size = (&color_tiles_end - &color_tiles);
			bgInitTileSet(0, &color_tiles, &color_pal, 0, size, 128*2, BG_256COLORS, 0x0000);		
	
			bgInitMapSet(0, &color_map, (&color_map_end - &color_map), SC_32x32, 0x7000);
						
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

void Drawcircles() 
{	
	u16 pad0, oldpad = 0xffff, pressed, end = 0, grid = 0;
	u16 redraw = 1;
		
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			bgInitTileSet(1, &circles_tiles, &circles_pal, 0, (&circles_tiles_end - &circles_tiles), 16*2, BG_16COLORS, 0x4000);	
			bgInitMapSet(1, &circles_map, (&circles_map_end - &circles_map), SC_32x32, 0x1000);
			
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
		
		if(pressed == KEY_A)
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
		
		if(pressed == KEY_A)
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
		
		if(pressed == KEY_A)
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
		
		if(pressed == KEY_A)
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
			
			consoleInitText(1, 7, &fontback);
			
			setPaletteColor(0x60, RGB5(0, 0, 0));
			setPaletteColor(0x61, RGB5(0, 0xff, 0));
			setPaletteColor(0x70, RGB5(0, 0, 0));
			setPaletteColor(0x71, RGB5(0xff, 0xff, 0xff));
			
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
			{
				drawText(12, 1, 7, "     ");
				drawText(18, 1, 7, "     ");
				drawText(24, 1, 7, "     ");
			}
			
			setPaletteColor(0x00, color);			
			change = 0;
		}
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
		
		if(pressed == KEY_B)
		{
			custom = !custom;	
			change = 1;
		}
			
		if(pressed == KEY_A)
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