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

void TestPatterns();
void DrawCredits();

int main(void) 
{
	u16 redraw = 1, change = 0;
	u16 pad0, oldpad = 0xffff, pressed;
	int sel = 0;
	    
	setBrightness(0);
	
	consoleInit();		
	
	// Main loop
	while(1) 
	{
		if(redraw)
		{
			u16 size = 0;
					
			setBrightness(0);	
			
			setPaletteColor(0x00, RGB5(0, 0, 0));
			consoleInitText(0, 7, &font);
			size = (&back_tiles_end - &back_tiles);
			bgInitTileSet(1, &back_tiles, &back_pal, 1, size, 16*2, BG_16COLORS, 0x6000);			
			
			size = (&back_map_end - &back_map);	
			bgInitMapSet(1, &back_map, size, SC_32x32, 0x2000);
			
			size = (&gillian_tiles_end-&gillian_tiles);
			DrawTilesWithSprites(170, 80, 64, 112, &gillian_tiles, size, &gillian_pal);				
						
			setMode(BG_MODE1,0); 	
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);						
									
			change = 1;
		}			
		
		if(change)
		{
			u16 pos = 7;
			
			setPaletteColor(0x71, RGB5(31, 31, 31));
			setPaletteColor(0x61, RGB5(31, 0, 0));
			
			drawText(3, pos, sel == 0 ? 6 : 7, "Test Patterns"); pos ++;
			drawText(3, pos, sel == 1 ? 6 : 7, "Drop Shadow Test"); pos ++;
			drawText(3, pos, sel == 2 ? 6 : 7, "Lag Test"); pos ++;
			drawText(3, pos, sel == 3 ? 6 : 7, "Manual Lag Test"); pos ++;
			drawText(3, pos, sel == 4 ? 6 : 7, "Scroll Test"); pos ++;
			drawText(3, pos, sel == 5 ? 6 : 7, "Grid Scroll Test"); pos ++;
			drawText(3, pos, sel == 6 ? 6 : 7, "Horizontal Stripes"); pos ++;
			drawText(3, pos, sel == 7 ? 6 : 7, "Checkerboard"); pos ++;
			drawText(3, pos, sel == 8 ? 6 : 7, "Backlit Zone Test"); pos ++;
			drawText(3, pos, sel == 9 ? 6 : 7, "Sound Test"); pos ++;
			drawText(3, pos, sel == 10 ? 6 : 7, "Help"); pos += 2;	
			drawText(3, pos, sel == 11 ? 6 : 7, "Credits"); 
			
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

		if(sel < 0)
			sel = 11;
			
		if(sel > 11)
			sel = 0;
			
		if(pressed == KEY_A)
		{							
			setFadeEffect(FADE_OUT);		
			oamClear(0, 0);
			
			switch(sel)
			{
				case 0:
					TestPatterns();
					break;
				case 1:
					DropShadowTest();
					break;
				case 2:
					PassiveLagTest();
					break;
				case 6:
					DrawStripes();
					break;
				case 7:
					DrawCheck();
					break;
				case 11:
					DrawCredits();
					break;
			}
			redraw = 1;
		}
		
		WaitForVBlank();
	}
	return 0;
}

void TestPatterns(void) 
{
	u16 redraw = 1, change = 0;
	u16 pad0, oldpad = 0xffff, pressed;
	int sel = 0, exit = 0;	    	
		
	while(!exit) 
	{
		if(redraw)
		{
			u16 size = 0;
					
			setBrightness(0);	
			
			setPaletteColor(0x00, RGB5(0, 0, 0));
			consoleInitText(0, 7, &font);				
			size = (&back_tiles_end - &back_tiles);
			bgInitTileSet(1, &back_tiles, &back_pal, 1, size, 16*2, BG_16COLORS, 0x6000);			
			
			size = (&back_map_end - &back_map);	
			bgInitMapSet(1, &back_map, size, SC_32x32, 0x2000);
			
			size = (&gillian_tiles_end-&gillian_tiles);
			DrawTilesWithSprites(170, 80, 64, 112, &gillian_tiles, size, &gillian_pal);				
						
			setMode(BG_MODE1,0); 	
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);						
									
			change = 1;
		}			
		
		if(change)
		{
			u16 pos = 6;
			
			setPaletteColor(0x71, RGB5(31, 31, 31));
			setPaletteColor(0x61, RGB5(31, 0, 0));
			
			drawText(3, pos, sel == 0 ? 6 : 7, "Pluge"); pos ++;
			drawText(3, pos, sel == 1 ? 6 : 7, "Color Bars"); pos ++;
			drawText(3, pos, sel == 2 ? 6 : 7, "SMPTE Color Bars"); pos ++;
			drawText(3, pos, sel == 3 ? 6 : 7, "Color Bars w/ Gray Ref"); pos ++;
			drawText(3, pos, sel == 4 ? 6 : 7, "Color Bleed Check"); pos ++;
			drawText(3, pos, sel == 5 ? 6 : 7, "Grid 256x224"); pos ++;
			drawText(3, pos, sel == 6 ? 6 : 7, "Grid 256x239"); pos ++;
			drawText(3, pos, sel == 7 ? 6 : 7, "Linearity"); pos ++;
			drawText(3, pos, sel == 8 ? 6 : 7, "Gray Ramp"); pos ++;
			drawText(3, pos, sel == 9 ? 6 : 7, "White & RGB Screen"); pos ++;
			drawText(3, pos, sel == 10 ? 6 : 7, "100 IRE"); pos ++;	
			drawText(3, pos, sel == 11 ? 6 : 7, "Sharpness"); pos++;
			drawText(3, pos, sel == 12 ? 6 : 7, "Overscan"); pos+=2;
			drawText(3, pos, sel == 13 ? 6 : 7, "Back to Main Menu"); 
			
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

		if(sel < 0)
			sel = 13;
			
		if(sel > 13)
			sel = 0;
			
		if(pressed == KEY_A)
		{							
			setFadeEffect(FADE_OUT);		
			oamClear(0, 0);
			
			switch(sel)
			{
				case 0:
					DrawPluge();
					break;
				case 1:
					DrawColorBars();
					break;
				case 2:
					DrawSMPTE();
					break;
				case 3:
					Draw601CB();
					break;
				case 4:
					DrawColorBleed();
					break;
				case 5:
					DrawGrid(0);
					break;
				case 6:
					DrawGrid(1);
					break;
				case 7:
					Drawcircles();
					break;
				case 8:
					DrawGrayRamp();
					break;
				case 9:
					DrawWhite();
					break;
				case 10:
					Draw100IRE();
					break;
				case 11:
					DrawSharpness();
					break;
				case 13:
					exit = 1;
					break;
			}
			redraw = 1;
		}
		
		if(pressed == KEY_B)
		{
			exit = 1;
			setFadeEffect(FADE_OUT);		
			oamClear(0, 0);
		}
		
		WaitForVBlank();
	}
	return 0;
}

void DrawCredits(void) 
{
	u16 redraw = 1, counter = 1;
	u16 pad0, oldpad = 0xffff, pressed;
	int exit = 0;
	    	
	while(!exit) 
	{
		if(redraw)
		{
			u16 size = 0;
			u16 pos = 8;
					
			setBrightness(0);	
			
			consoleInitText(0, 7, &font);				
			size = (&back_tiles_end - &back_tiles);
			bgInitTileSet(1, &back_tiles, &back_pal, 1, size, 16*2, BG_16COLORS, 0x6000);			
			
			size = (&back_map_end - &back_map);	
			bgInitMapSet(1, &back_map, size, SC_32x32, 0x2000);						
						
			setMode(BG_MODE1,0); 	
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);						
			
			setPaletteColor(0x71, RGB5(31, 31, 31));
			setPaletteColor(0x61, RGB5(0, 31, 0));
			setPaletteColor(0x51, RGB5(0, 0, 31));
			
			drawText(3, pos, 6, "Code and Patterns:"); pos ++;
			pos++;
			drawText(3, pos, 6, "Menu Pixel Art:"); pos ++;
			drawText(4, pos, 7, "Asher"); pos ++;
			drawText(3, pos, 6, "SDK:"); pos ++;
			drawText(4, pos, 7, "PVSnesLib"); pos ++;
			drawText(3, pos, 6, "SDK Consultor:"); pos ++;
			drawText(4, pos, 7, "Alekmaul"); pos ++;
			drawText(3, pos, 6, "Advisor:"); pos ++;
			drawText(4, pos, 7, "Fudoh"); pos ++;
			drawText(3, pos, 6, "Collaboration:"); pos ++;
			drawText(4, pos, 7, "Konsolkongen & shmups"); pos ++;
			drawText(3, pos, 6, "Info on using this suite:"); pos ++;
			drawText(4, pos, 7, "http://junkerhq.net/240p"); pos ++;
			
			drawText(19, 6, 5, "Ver. 0.02");
			drawText(19, 7, 7, "12/07/2014");
			 			
			if(redraw)
				redraw = 0;						
		}
				
		if(counter == 1)
			drawText(4, 9, 7, "Artemio Urbina      ");
		if(counter == 60*4)
			drawText(4, 9, 7, "@Artemio (twitter)  ");  
		if(counter == 60*8)
			drawText(4, 9, 7, "aurbina@junkerhq.net");
		if(counter == 60*16)
			counter = 0;

		setBrightness(0xF);		
		WaitForVBlank();
		counter ++;
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;					
		
		if(pressed == KEY_B)
		{
			exit = 1;
			setFadeEffect(FADE_OUT);				
		}
	}
	return 0;
}
