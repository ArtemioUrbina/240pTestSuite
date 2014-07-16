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
	u8	drawShadow = 1, odd = 0;
	int sprite = 0, x = 100, y = 100, text = 0;
	
	sprite = rand() % 2;
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			// We add the font, it will be overwritten by sprites they 
			// need to be in 8k vram steps. Lowercase will be available
			consoleInitText(1, 7, &font);	
			
			size = (&motoko_tiles1_end - &motoko_tiles1);
			bgInitTileSet(0, &motoko_tiles1, &motoko_pal, 0, size, 256*2, BG_256COLORS, 0x2000);	
			dmaCopyVram(&motoko_tiles2, 0x5000, (&motoko_tiles2_end-&motoko_tiles2));
			
			bgInitMapSet(0, &motoko_map, (&motoko_map_end - &motoko_map), SC_32x32, 0x7C00);
			
			oamInitGfxSet(&sprites_tiles, (&sprites_tiles_end - &sprites_tiles), &sprites_pal, 16*2, 7, 0, OBJ_SIZE32);			
			
			setMode(BG_MODE3,0);			
			
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
		
		if(text)
		{			
			if(odd)
			{
				drawText(20, 1, 7, "odd");
				drawText(24, 1, 7, "frames");
			}
			else
			{
				drawText(20, 1, 7, "even");
				drawText(25, 1, 7, "frames");
			}
			text --;
			if(!text)						
				CleanFontMap();			
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
			
		if(pressed == KEY_B)
			end = 1;		
			
		if(pressed == KEY_X)
		{
			sprite = !sprite;
			changesprite = 1;
		}
		
		if(pressed == KEY_Y)
		{		
			odd = !odd;
			drawShadow = !drawShadow;
			text = 30;
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
		
		if(pressed == KEY_A)
			alternate = !alternate;
			
		if(pressed == KEY_Y)
		{
			drawframe = !drawframe;
			if(!drawframe)
				CleanFontMap();
		}
			
		if(pressed == KEY_X)
		{
			vert = !vert;
			redraw = 1;
		}		
			
		if(pressed == KEY_B)
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
		
		if(pressed == KEY_A)
			alternate = !alternate;
			
		if(pressed == KEY_Y)
		{
			drawframe = !drawframe;
			if(!drawframe)
				CleanFontMap();
		}
			
		if(pressed == KEY_B)
			end = 1;
	}	
	setFadeEffect(FADE_OUT);	
	
	return;
}

void PassiveLagTest()
{
	u16 pad0, oldpad = 0xffff, pressed, end = 0;
	u16 frames = 0, seconds = 0, minutes = 0, hours = 0, msd, lsd, framecnt = 0;
	u16 numberIndex[12] = { 0, 8, 64, 72, 128, 136, 192, 200, 256, 264, 320, 328};
	u16 spriteIndex[12] = { 0, 4, 8, 12, 16, 20, 24, 28, 32 };
	u16 numberTopIndex[8] = { 64, 68, 72, 76, 80, 84, 88, 92 };	
	u16 xpos[8] = { 5, 30, 70, 95, 135, 160, 200, 225 };
	int index = 0, y = 20, running = 0, redraw = 1;	
	
	while(!end) 
	{	
		u16 count = 0, mul = 1;
	
		if(redraw)
		{
			consoleInitText(0, 7, &font);	
			
			setPaletteColor(0x91, RGB5(31, 31, 31));
			setPaletteColor(0xA3, RGB5(31, 0, 0));
			
			AddTextColor(7, RGB5(0, 0, 0), RGB5(31, 31, 31));
			
			oamInitGfxSet(&numbers_tiles, &numbers_tiles_end - &numbers_tiles,	&numbers_pal, 16*2, 0, 0x2000, OBJ_SIZE32);
			
			bgInitTileSet(1, &lagtest_tiles, &lagtest_pal, 0, (&lagtest_tiles_end - &lagtest_tiles), 16*2, BG_16COLORS, 0x6000);	
			bgInitMapSet(1, &lagtest_map, (&lagtest_map_end - &lagtest_map), SC_32x32, 0x1000);
			
			drawText(1, 1, 7, "hours   minutes seconds frames");
			
			/*****Numbers*****/
			
			// Hours			
			DrawNumber(xpos[0], y, spriteIndex[0], numberIndex[0], 0);	
			DrawNumber(xpos[1], y, spriteIndex[1], numberIndex[0], 0);	
			
			// Minutes
			DrawNumber(xpos[2], y, spriteIndex[2], numberIndex[0], 0);	
			DrawNumber(xpos[3], y, spriteIndex[3], numberIndex[0], 0);	
			
			//Seconds
			DrawNumber(xpos[4], y, spriteIndex[4], numberIndex[0], 0);	
			DrawNumber(xpos[5], y, spriteIndex[5], numberIndex[0], 0);	
			
			// Frames
			DrawNumber(xpos[6], y, spriteIndex[6], numberIndex[0], 0);	
			DrawNumber(xpos[7], y, spriteIndex[7], numberIndex[0], 0);			
					
			/*****Circles*****/			
			
			DrawCircle(0, 70, 192, numberIndex[11], 2);
		
			/*****Numbers on Circles*****/
			DrawNumber(20, 80, numberTopIndex[0], numberIndex[1], 1);	
			DrawNumber(84, 80, numberTopIndex[1], numberIndex[2], 1);
			DrawNumber(148, 80, numberTopIndex[2], numberIndex[3], 1);	
			DrawNumber(212, 80, numberTopIndex[3], numberIndex[4], 1);	
			
			DrawNumber(20, 150, numberTopIndex[4], numberIndex[5], 1);	
			DrawNumber(84, 150, numberTopIndex[5], numberIndex[6], 1);	
			DrawNumber(148, 150, numberTopIndex[6], numberIndex[7], 1);	
			DrawNumber(212, 150, numberTopIndex[7], numberIndex[8], 1);				
			
			setMode(BG_MODE1,0);
			bgSetScroll(1, 0, -1);			
			bgSetDisable(2);
			redraw = 0;
		}
		
		if(framecnt > 7)
			framecnt = 0;
		
		if(frames > 59)
		{
			frames = 0;
			seconds ++;	
		}
	
		if(seconds > 59)
		{
		  seconds = 0;
		  minutes ++;
		}

		if(minutes > 59)
		{
		  minutes = 0;
		  hours ++;
		}

		if(hours > 99)
		  hours = 0;
		  
		// Hours			
		lsd = hours % 10;
		msd = hours / 10;			
		ChangeNumber(xpos[0], y, spriteIndex[0], numberIndex[msd], 0);	
		ChangeNumber(xpos[1], y, spriteIndex[1], numberIndex[lsd], 0);	
		
		// Minutes			
		lsd = minutes % 10;
		msd = minutes / 10;			
		ChangeNumber(xpos[2], y, spriteIndex[2], numberIndex[msd], 0);	
		ChangeNumber(xpos[3], y, spriteIndex[3], numberIndex[lsd], 0);	
		
		// Seconds		
		lsd = seconds % 10;
		msd = seconds / 10;			
		ChangeNumber(xpos[4], y, spriteIndex[4], numberIndex[msd], 0);	
		ChangeNumber(xpos[5], y, spriteIndex[5], numberIndex[lsd], 0);	
		
		// frames		
		lsd = frames % 10;
		msd = frames / 10;			
		ChangeNumber(xpos[6], y, spriteIndex[6], numberIndex[msd], 0);	
		ChangeNumber(xpos[7], y, spriteIndex[7], numberIndex[lsd], 0);	
		
		count = framecnt;
		if(count > 3)
		{
			count -= 3;
			mul = 2;
		}
			
		ChangeCircle(count*64, 70*mul, 192, numberIndex[11], 2);
		
		if(running)
		{	
			frames ++;
			framecnt ++;
		}
		
		WaitForVBlank();
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
		
		if(pressed == KEY_A)
			running = !running;
				
		if(pressed == KEY_B)
			end = 1;
			
		if (pressed == KEY_X && !running)
		{
			frames = hours = minutes = seconds = 0;
			framecnt = 0;
		}		
	}
	setFadeEffect(FADE_OUT);		
	oamClear(0, 0);
}