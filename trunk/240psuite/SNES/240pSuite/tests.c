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
#include "help.h"
#include "control.h"

void ShiftPalette(u16 pal, u16 pos)
{	
	u16 *palette = (unsigned int*)&sonicback_pal;
	
	if(pos == 1)
	{
		setPaletteColor(pal*0x10+2, palette[16+2]);
		setPaletteColor(pal*0x10+3, palette[16+3]);
		setPaletteColor(pal*0x10+4, palette[16+4]);
		setPaletteColor(pal*0x10+5, palette[16+5]);
	}
	
	if(pos == 2)
	{
		setPaletteColor(pal*0x10+2, palette[16+5]);
		setPaletteColor(pal*0x10+3, palette[16+2]);
		setPaletteColor(pal*0x10+4, palette[16+3]);
		setPaletteColor(pal*0x10+5, palette[16+4]);
	}
	
	if(pos == 3)
	{
		setPaletteColor(pal*0x10+2, palette[16+4]);
		setPaletteColor(pal*0x10+3, palette[16+5]);
		setPaletteColor(pal*0x10+4, palette[16+2]);
		setPaletteColor(pal*0x10+5, palette[16+3]);
	}
}

void DropShadowTest(void) 
{	
	u16 pressed, held, end = 0;	 
	u16 redraw = 1, size = 0, changesprite = 1;
	u8	drawShadow = 1, odd = 0, mode = BG_MODE3, frame = 0;
	s16 sprite = 0, x = 100, y = 100, text = 0, back = 0;
	
	sprite = rand() % 2;
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			// We add the font, it will be overwritten by sprites they 
			// need to be in 8k vram steps. Lowercase will be available
			consoleInitText(1, 7, &font);	
			
			if(back == 0)
			{
				size = (&motoko_tiles1_end - &motoko_tiles1);
				bgInitTileSet(0, &motoko_tiles1, &motoko_pal, 0, size, 256*2, BG_256COLORS, 0x2000);	
				dmaCopyVram(&motoko_tiles2, 0x5000, (&motoko_tiles2_end-&motoko_tiles2));
				
				bgInitMapSet(0, &motoko_map, (&motoko_map_end - &motoko_map), SC_32x32, 0x7C00);
				mode = BG_MODE3;
			}
			
			if(back == 1)
			{
				size = (&sonicback_tiles_end - &sonicback_tiles);
				bgInitTileSet(0, &sonicback_tiles, &sonicback_pal, 0, size, 32*2, BG_256COLORS, 0x3000);	
				bgInitMapSet(0, &sonicback_map, (&sonicback_map_end - &sonicback_map), SC_32x32, 0x1000);
			
				size = (&sonicfloor_tiles_end - &sonicfloor_tiles);
				bgInitTileSet(1, &sonicfloor_tiles, &sonicfloor_pal, 2, size, 16*2, BG_16COLORS, 0x6000);	
				bgInitMapSet(1, &sonicfloor_map, (&sonicfloor_map_end - &sonicfloor_map), SC_32x32, 0x5400);	
				mode = BG_MODE3;				
			}
			
			if(back == 2)
			{
				size = (&hstripes_tiles_end - &hstripes_tiles);
				bgInitTileSet(1, &hstripes_tiles, &grid_pal, 0, size, 16*2, BG_16COLORS, 0x6000);	
				bgInitMapSet(1, &fullscreen_map, (&fullscreen_map_end - &fullscreen_map), SC_32x32, 0x7000);
				
				setPaletteColor(0x00, RGB5(0, 0, 0));
				setPaletteColor(0x01, RGB5(0xff, 0xff, 0xff));
				mode = BG_MODE1;
			}
			
			if(back == 3)
			{
				size = (&check_tiles_end - &check_tiles);
				bgInitTileSet(1, &check_tiles, &grid_pal, 0, size, 16*2, BG_16COLORS, 0x6000);						
				bgInitMapSet(1, &fullscreen_map, (&fullscreen_map_end - &fullscreen_map), SC_32x32, 0x7000);
				setPaletteColor(0x00, RGB5(0, 0, 0));
				setPaletteColor(0x01, RGB5(0xff, 0xff, 0xff));
				mode = BG_MODE1;
			}
			
			oamInitGfxSet(&sprites_tiles, (&sprites_tiles_end - &sprites_tiles), &sprites_pal, 16*2, 7, 0, OBJ_SIZE32);			
			
			setMode(mode,0);			
			
			bgSetScroll(0, 0, -1);
			if(back == 1)
				bgSetScroll(1, 0, -97);
			if(back > 1)
			{
				bgSetScroll(1, 0, -1);
				bgSetDisable(0);
				bgSetDisable(2);
			}				
			setBrightness(0xF);
			redraw = 0;
		}
		
		if(back == 1)
		{
			switch(frame)
			{
				case 30:
					ShiftPalette(1, 3);
					break;
				case 60:
					ShiftPalette(1, 2);
					break;
				case 90:
					ShiftPalette(1, 1);
					break;
			}

			frame ++;
			if(frame > 90)
				frame = 1;
				
			bgSetScroll(0, x*2, -1);
			bgSetScroll(1, x*4, -97);  
		}
		
		if(changesprite)
		{
			oamClear(0, 0);
			
			if(sprite)
			{
				oamSet(0, x, y, 2, 0, 0, 8, 7); 
				oamSetEx(0, OBJ_SMALL, OBJ_SHOW);
				oamSetVisible(0, OBJ_SHOW);
			}
			else
			{
				oamSet(0, x, y, 2, 0, 0, 0, 7); 
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
				
		WaitForVBlank();
		
		drawShadow = !drawShadow;
		
		pressed = PadPressed(0);
		held = PadHeld(0);
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_SHADOW);
			redraw = 1;
		}
			
		if(held & KEY_UP)
			y--;
		if(held & KEY_DOWN)
			y++;
		if(held & KEY_LEFT)
		{
			if(sprite)
			{
				oamSet(0, x, y, 2, 0, 0, 8, 7);
			}
			else
			{
				oamSet(0, x, y, 2, 0, 0, 0, 7); 				
				oamSet(4, x+10, y+10, 2, 0, 0, 4, 7); 
			}
			x--;
		}
		
		if(held & KEY_RIGHT)
		{
			if(sprite)
			{
				oamSet(0, x, y, 2, 1, 0, 8, 7);
			}
			else
			{
				oamSet(0, x, y, 2, 1, 0, 0, 7); 				
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
		
		if(pressed == KEY_A)
		{		
			CleanFontMap();	
			odd = !odd;
			drawShadow = !drawShadow;
			text = 30;
		}
		
		if(pressed == KEY_R)
		{		
			back ++;
			redraw = 1;
			
			if(back > 3)
				back = 0;
		}
		
		if(pressed == KEY_L)
		{		
			back --;
			redraw = 1;
			
			if(back < 0)
				back = 3;
		}
	}	
	Transition();
	oamClear(0, 0);	
	
	return;
}

void StripedSpriteTest(void) 
{	
	u16 pressed, held, end = 0;	 
	u16 redraw = 1, size = 0;
	u8	drawShadow = 1, odd = 0, mode = BG_MODE3, frame = 0;
	s16 x = 100, y = 100, back = 0;
		
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			// We add the font, it will be overwritten by sprites they 
			// need to be in 8k vram steps. Lowercase will be available
			consoleInitText(1, 7, &font);	
			
			if(back == 0)
			{
				size = (&motoko_tiles1_end - &motoko_tiles1);
				bgInitTileSet(0, &motoko_tiles1, &motoko_pal, 0, size, 256*2, BG_256COLORS, 0x2000);	
				dmaCopyVram(&motoko_tiles2, 0x5000, (&motoko_tiles2_end-&motoko_tiles2));
				
				bgInitMapSet(0, &motoko_map, (&motoko_map_end - &motoko_map), SC_32x32, 0x7C00);
				mode = BG_MODE3;
			}
			
			if(back == 1)
			{
				size = (&sonicback_tiles_end - &sonicback_tiles);
				bgInitTileSet(0, &sonicback_tiles, &sonicback_pal, 0, size, 32*2, BG_256COLORS, 0x3000);	
				bgInitMapSet(0, &sonicback_map, (&sonicback_map_end - &sonicback_map), SC_32x32, 0x1000);
			
				size = (&sonicfloor_tiles_end - &sonicfloor_tiles);
				bgInitTileSet(1, &sonicfloor_tiles, &sonicfloor_pal, 2, size, 16*2, BG_16COLORS, 0x6000);	
				bgInitMapSet(1, &sonicfloor_map, (&sonicfloor_map_end - &sonicfloor_map), SC_32x32, 0x5400);	
				mode = BG_MODE3;				
			}
			
			if(back == 2)
			{
				size = (&hstripes_tiles_end - &hstripes_tiles);
				bgInitTileSet(1, &hstripes_tiles, &grid_pal, 0, size, 16*2, BG_16COLORS, 0x6000);	
				bgInitMapSet(1, &fullscreen_map, (&fullscreen_map_end - &fullscreen_map), SC_32x32, 0x7000);
				
				setPaletteColor(0x00, RGB5(0, 0, 0));
				setPaletteColor(0x01, RGB5(0xff, 0xff, 0xff));
				mode = BG_MODE1;
			}
			
			if(back == 3)
			{
				size = (&check_tiles_end - &check_tiles);
				bgInitTileSet(1, &check_tiles, &grid_pal, 0, size, 16*2, BG_16COLORS, 0x6000);						
				bgInitMapSet(1, &fullscreen_map, (&fullscreen_map_end - &fullscreen_map), SC_32x32, 0x7000);
				setPaletteColor(0x00, RGB5(0, 0, 0));
				setPaletteColor(0x01, RGB5(0xff, 0xff, 0xff));
				mode = BG_MODE1;
			}
			
			oamInitGfxSet(&striped_tiles, (&striped_tiles_end - &striped_tiles), &striped_pal, 16*2, 7, 0, OBJ_SIZE32);			
			
			setMode(mode, 0);			
			
			bgSetScroll(0, 0, -1);
			if(back == 1)
				bgSetScroll(1, 0, -97);
			if(back > 1)
			{
				bgSetScroll(1, 0, -1);
				bgSetDisable(0);
				bgSetDisable(2);
			}				
			
			oamSet(0, x, y, 2, 0, 0, 0, 7); 
			oamSetEx(0, OBJ_SMALL, OBJ_SHOW);
			oamSetVisible(0, OBJ_SHOW);
				
			setBrightness(0xF);
			redraw = 0;
		}
		
		if(back == 1)
		{
			switch(frame)
			{
				case 30:
					ShiftPalette(1, 3);
					break;
				case 60:
					ShiftPalette(1, 2);
					break;
				case 90:
					ShiftPalette(1, 1);
					break;
			}

			frame ++;
			if(frame > 90)
				frame = 1;
				
			bgSetScroll(0, x*2, -1);
			bgSetScroll(1, x*4, -97);  
		}
				
		oamSetXY(0, x, y);	
		
		WaitForVBlank();			
		
		pressed = PadPressed(0);
		held = PadHeld(0);
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_STRIPED);
			redraw = 1;
		}
			
		if(held & KEY_UP)
			y--;
		if(held & KEY_DOWN)
			y++;
		if(held & KEY_LEFT)
			x--;		
		if(held & KEY_RIGHT)
			x++;		
			
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
		
		if(pressed == KEY_R)
		{		
			back ++;
			redraw = 1;
			
			if(back > 3)
				back = 0;
		}
		
		if(pressed == KEY_L)
		{		
			back --;
			redraw = 1;
			
			if(back < 0)
				back = 3;
		}
	}	
	Transition();
	oamClear(0, 0);	
	
	return;
}

void DrawStripes(void)
{	
	u16 pressed, end = 0, drawframe = 0;	 
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
			
			bgSetScroll(1, 0, -1);
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
		
		pressed = PadPressed(0);
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_STRIPES);
			redraw = 1;
		}
		
		if(pressed == KEY_A)
			alternate = !alternate;
			
		if(pressed == KEY_X)
		{
			drawframe = !drawframe;
			if(!drawframe)
				CleanFontMap();
		}
			
		if(pressed == KEY_Y)
		{
			vert = !vert;
			redraw = 1;
		}		
			
		if(pressed == KEY_B)
			end = 1;				
	}	
	Transition();
	
	return;
}

void DrawCheck(void)
{	
	u16 pressed, end = 0, drawframe = 0;	 
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
			
			bgSetScroll(1, 0, -1);
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
		
		pressed = PadPressed(0);
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_CHECK);
			redraw = 1;
		}
		
		if(pressed == KEY_A)
			alternate = !alternate;
			
		if(pressed == KEY_X)
		{
			drawframe = !drawframe;
			if(!drawframe)
				CleanFontMap();
		}
			
		if(pressed == KEY_B)
			end = 1;
	}	
	Transition();
	
	return;
}

void PassiveLagTest()
{
	u16 pressed, end = 0;
	u16 frames = 0, seconds = 0, minutes = 0, hours = 0, msd, lsd, framecnt = 0;
	u16 numberIndex[12] = { 0, 8, 64, 72, 128, 136, 192, 200, 256, 264, 320, 328};
	u16 spriteIndex[8] = { 0, 4, 8, 12, 16, 20, 24, 28 };
	u16 numberTopIndex[8] = { 64, 68, 72, 76, 80, 84, 88, 92 };	
	u16 xpos[8] = { 5, 30, 70, 95, 135, 160, 200, 225 };
	u16 y = 20, running = 1, redraw = 1, color = 1, bgcol = 0xa;	
	
	while(!end) 
	{	
		u16 count = 0, mul = 1;
	
		if(redraw)
		{
			setBrightness(0);
			
			consoleInitText(0, 7, &font);	
			
			setPaletteColor(0x91, RGB5(31, 31, 31));
			setPaletteColor(0xA3, RGB5(31, 0, 0));
			setPaletteColor(0xA1, RGB5(31, 0, 0));
			setPaletteColor(0xB1, RGB5(0, 0, 31));
			
			AddTextColor(7, RGB5(0, 0, 0), RGB5(31, 31, 31));
						
			bgInitTileSet(1, &lagtest_tiles, &lagtest_pal, 0, (&lagtest_tiles_end - &lagtest_tiles), 16*2, BG_16COLORS, 0x6000);	
			bgInitMapSet(1, &lagtest_map, (&lagtest_map_end - &lagtest_map), SC_32x32, 0x1000);
			
			drawText(1, 1, 7, "hours   minutes seconds frames");
			
			oamInitGfxSet(&numbers_tiles, &numbers_tiles_end - &numbers_tiles,	&numbers_pal, 16*2, 0, 0x2000, OBJ_SIZE32);
			
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
			
			DrawCircle(0, 70, 192, numberIndex[10], 2);
		
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
			
			setBrightness(0xf);
			
			redraw = 0;
		}
		WaitForVBlank();
		
		if(framecnt > 7)
			framecnt = 0;
			
		if(running && color)
		{
			if(bgcol == 0xa)
				bgcol = 0xb;
			else
				bgcol = 0xa;
		}
		
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
		ChangeNumber(xpos[6], y, spriteIndex[6], numberIndex[msd], bgcol);	
		ChangeNumber(xpos[7], y, spriteIndex[7], numberIndex[lsd], bgcol);	
		
		count = framecnt;
		if(count > 3)
		{
			count -= 3;
			mul = 2;
		}
			
		ChangeCircle(count*64, 70*mul, 192, numberIndex[10], 2);
		
		if(running)
		{	
			frames ++;
			framecnt ++;
		}
			
		pressed = PadPressed(0);
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_LAG);
			redraw = 1;
		}
		
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
	Transition();
	oamClear(0, 0);
	return;
}

void HScrollTest() 
{	
	u16 pressed, end = 0;
	u16 redraw = 1, x = 0, pause = 0, frame = 0;
	s16 speed = 1, acc = 1;
		
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
						
			bgInitTileSet(0, &sonicback_tiles, &sonicback_pal, 0, (&sonicback_tiles_end - &sonicback_tiles), 32*2, BG_256COLORS, 0x3000);	
			bgInitMapSet(0, &sonicback_map, (&sonicback_map_end - &sonicback_map), SC_32x32, 0x1000);
			
			bgInitTileSet(1, &sonicfloor_tiles, &sonicfloor_pal, 2, (&sonicfloor_tiles_end - &sonicfloor_tiles), 16*2, BG_16COLORS, 0x6000);	
			bgInitMapSet(1, &sonicfloor_map, (&sonicfloor_map_end - &sonicfloor_map), SC_32x32, 0x5400);			
			
			setMode(BG_MODE3,0); 			
			
			bgSetScroll(0, 0, -1);
			bgSetScroll(1, 0, -97);
			setBrightness(0xF);
			redraw = 0;
		}
		
		switch(frame)
		{
			case 30:
				ShiftPalette(1, 3);
				break;
			case 60:
				ShiftPalette(1, 2);
				break;
			case 90:
				ShiftPalette(1, 1);
				break;
		}

		frame ++;
		if(frame > 90)
			frame = 1;
		
		bgSetScroll(0, x/2, -1);
		bgSetScroll(1, x, -97);	
		
		WaitForVBlank();
		
		pressed = PadPressed(0);
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_HSCROLL);
			redraw = 1;
		}
		
		if(pressed == KEY_B)
			end = 1;	
			
		if(pressed == KEY_UP)
			speed++;	
			
		if(pressed == KEY_DOWN)
			speed--;
			
		if(speed > 20)        
			speed = 20;          

		if(speed < 0)        
			speed = 0;  
			
		if (pressed & KEY_A)
			pause = !pause;

		if (pressed & KEY_X)
			acc *= -1;

		if(!pause)
			x += acc*speed;
	}	
	Transition();
	
	return;
}

void VScrollTest(void)
{	
	u16 pressed, end = 0;	 
	u16 redraw = 1;
	s16 posx = 0, posy = -1, speed = 1, acc = -1, pause = 0, *pos = NULL;
	
	pos = &posx;
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			bgInitTileSet(0, &circlesgrid_tiles, &grid_pal, 0, (&circlesgrid_tiles_end - &circlesgrid_tiles), 16*2, BG_16COLORS, 0x6000);					
			bgInitMapSet(0, &fullscreen_map, (&fullscreen_map_end - &fullscreen_map), SC_32x32, 0x7000);
			
			setPaletteColor(0x00, RGB5(0, 0, 0));
			setPaletteColor(0x01, RGB5(0xff, 0xff, 0xff));
			
			setMode(BG_MODE1,0); 								
			bgSetDisable(1);
			bgSetDisable(2);
			
			bgSetScroll(0, posx, posy);
			setBrightness(0xF);
			redraw = 0;
		}
				
		WaitForVBlank();
		
		pressed = PadPressed(0);
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_VSCROLL);
			redraw = 1;
		}
		
		if(pressed == KEY_B)
			end = 1;
		
		if(pressed & KEY_UP)
			speed++;

		if(pressed & KEY_DOWN)
			speed--;

		if(speed > 5)        
			speed = 5;          

		if(speed < 0)        
			speed = 0;          

		if(pressed & KEY_A)
			pause = !pause;

		if(pressed & KEY_Y)
			acc *= -1;

		if(pressed & KEY_X)
		{
			if(pos == &posx)
				pos = &posy;
			else
				pos = &posx;
		}

		if(!pause)
			*pos += acc*speed;
				
		bgSetScroll(0, posx, posy);
	}	
	Transition();
	
	return;
}

void LEDZoneTest() 
{	
	u16 pressed, held, end = 0;
	u16 redraw = 1, changed = 0, shown = 1;		
	s16 x = 128, y = 112, sprite = 0;
		
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			ClearScreen(0);
			
			oamInitGfxSet(&LEDsprites_tiles, (&LEDsprites_tiles_end - &LEDsprites_tiles), &LEDsprites_pal, 16*2, 7, 0, OBJ_SIZE8);
			
			oamSet(0, x, y, 2, 0, 0, sprite, 7); 
			if(shown)
			{
				oamSetEx(0, OBJ_SMALL, OBJ_SHOW);
				oamSetVisible(0, OBJ_SHOW);
			}
			else
			{
				oamSetEx(0, OBJ_SMALL, OBJ_HIDE);
				oamSetVisible(0, OBJ_HIDE);
			}
			
			setMode(BG_MODE1,0); 
			bgSetDisable(1);		
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);
			setBrightness(0xF);
			redraw = 0;
		}
		
		if(changed)
		{
			oamSet(0, x, y, 2, 0, 0, sprite, 7); 
			if(shown)
			{
				oamSetEx(0, OBJ_SMALL, OBJ_SHOW);
				oamSetVisible(0, OBJ_SHOW);
			}
			else
			{
				oamSetEx(0, OBJ_SMALL, OBJ_HIDE);
				oamSetVisible(0, OBJ_HIDE);
			}
		}
		
		oamSetXY(0, x, y);

		WaitForVBlank();
		
		pressed = PadPressed(0);
		held = PadHeld(0);
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_LED);
			redraw = 1;
		}
		
		if(pressed == KEY_B)
			end = 1;		
		
		if(pressed == KEY_A)
		{
			shown = !shown;
			changed = 1;
		}
			
		if(pressed == KEY_L)
		{
			sprite --;
			changed = 1;
		}
			
		if(pressed == KEY_R)
		{
			sprite ++;
			changed = 1;
		}
		
		if(sprite > 3)
			sprite = 0;
		if(sprite < 0)
			sprite = 3;
		
		if(held & KEY_UP)
			y--;
		if(held & KEY_DOWN)
			y++;
		if(held & KEY_LEFT)
			x--;		
		if(held & KEY_RIGHT)
			x++;
			
		if(x > 255)
			x = 255;
		if(x < 0)
			x = 0;
		if(y > 223)
			y = 223;
		if(y < 0)
			y = 0;
	}	
	Transition();
	oamClear(0, 0);
	return;
}

void SoundTest()
{
	u16 redraw = 1, change = 0, end = 0;
	u16 pressed;	
	s16 sound = 1, effect = 0, pan[3] = { 0, 8, 15 };

	while(!end) 
	{
		if(redraw)
		{
			u16 size = 0;
					
			setBrightness(0);	
			
			CleanFontMap();
			consoleInitText(0, 7, &font);
			AddTextColor(7, RGB5(31, 31, 31), RGB5(0, 0, 0));
			AddTextColor(6, RGB5(0, 31, 0), RGB5(0, 0, 0));	
			AddTextColor(5, RGB5(31, 0, 0), RGB5(0, 0, 0));
			
			size = (&back_tiles_end - &back_tiles);
			bgInitTileSet(1, &back_tiles, &back_pal, 1, size, 16*2, BG_16COLORS, 0x6000);			
			
			size = (&back_map_end - &back_map);	
			bgInitMapSet(1, &back_map, size, SC_32x32, 0x2000);
						
			setMode(BG_MODE1,0); 	
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);					
			
			redraw = 0;
			change = 1;
		}			
		
		if(change)
		{
			u16 y = 7;						
			
			drawText(11, 7, 6, "Sound Test"); 			
			drawText(6, 14, sound == 0 ? 5 : 7, "Left"); 
			drawText(13, 16, sound == 1 ? 5 : 7, "Center"); 
			drawText(22, 14, sound == 2 ? 5 : 7, "Right"); 
			change = 0;
		}	

		spcProcess();	
		WaitForVBlank();	
		
		pressed = PadPressed(0);
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_SOUND);
			redraw = 1;
		}
		
		if(pressed == KEY_A)
			spcEffect(2, 0, 15*16+pan[sound]);
			
		if(pressed == KEY_B)
			end = 1;	
		
		if(pressed == KEY_LEFT)
		{
			sound --;
			change = 1;
		}
			
		if(pressed == KEY_RIGHT)
		{
			sound ++;
			change = 1;
		}

		if(sound < 0)
			sound = 2;
		if(sound > 2)
			sound = 0;		
		
	}
	spcStop();
	Transition();
	return;
}

void ManualLagTest() 
{	
	u16 pressed, end = 0, change = 1, draw = 0, audio = 0, sound = 0;
	u16 redraw = 1, changed = 0, variation = 1, pos = 0, drawoffset = 0;
	s16 x = 112, y = 96, x2 = 112, y2 = 96, speed = 1, vary = 1;
	s16 clicks[10], pal = 7, sprpal = 7, view = 0;
		
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			ClearScreen(1);
			
			consoleInitText(0, 7, &font);
			AddTextColor(7, RGB5(31, 31, 31), RGB5(0, 0, 0));
			AddTextColor(6, RGB5(0, 31, 0), RGB5(0, 0, 0));	
			AddTextColor(5, RGB5(31, 0, 0), RGB5(0, 0, 0));
			AddTextColor(4, RGB5(0, 25, 25), RGB5(0, 0, 0));
			
			AddTextColor(15, RGB5(31, 31, 31), RGB5(0, 0, 0));
			AddTextColor(14, RGB5(0, 31, 0), RGB5(0, 0, 0));	
			AddTextColor(13, RGB5(31, 0, 0), RGB5(0, 0, 0));
			AddTextColor(12, RGB5(0, 25, 25), RGB5(0, 0, 0));
			
			oamInitGfxSet(&lagspr_tiles, (&lagspr_tiles_end - &lagspr_tiles), &lagspr_pal, 16*2, 7, 0x2000, OBJ_SIZE32);
			
			oamSet(0, x, y, 3, 0, 0, 0, 7); 
			oamSetEx(0, OBJ_SMALL, OBJ_SHOW);
			oamSetVisible(0, OBJ_SHOW);
			
			oamSet(4, 112, 96, 2, 0, 0, 0, 7); 
			oamSetEx(4, OBJ_SMALL, OBJ_SHOW);
			oamSetVisible(4, OBJ_SHOW);
			
			oamSet(8, x2, y2, 3, 0, 0, 0, 7); 
			oamSetEx(8, OBJ_SMALL, OBJ_HIDE);
			oamSetVisible(8, OBJ_HIDE);		
			
			drawText(2, 21, 6, "Press the A button when the");
			drawText(2, 22, 6, "sprite is aligned. A negative");
			drawText(2, 23, 6, "value means you pressed A");
			drawText(2, 24, 6, "before they intersect.");
			drawText(2, 25, 6, "X button toggles horz/vert");
			drawText(2, 26, 6, "R button toggles audio");
			drawText(2, 27, 6, "Y toggles random/rhythmic");
					
			if(pos)		  
			{
				s16 i = 0;
				u16 ppos = 0;

				for(i = 0; i < pos; i++)	
				{
					pal = 7;

					if(clicks[i] == 0)
						pal = 6;
					if(clicks[i] < 0)
						pal = 5;

					ppos = i + 1;                    					
					drawText(0, i, 4, "%d:", ppos);
					if(clicks[i] == 1)
						drawText(3, i, pal, "%d frame   ", clicks[i]);
					else
						drawText(3, i, pal, "%d frames  ", clicks[i]);
				}  
			}
			
			setMode(BG_MODE1,0); 
			bgSetDisable(2);
			
			bgSetScroll(0, 0, -1);
			bgSetScroll(1, 0, -1);
			setBrightness(0xF);
			redraw = 0;
			draw = 1;
		}
		
		if(draw)
		{      
			drawText(16, 0, 4, "Audio: %s", audio ? "on " : "off");
			drawText(15, 1, 4, "Timing: %s", variation ? "random  " : "rhythmic");
			draw = 0;
		}
		
		if(drawoffset)
		{
			u16 ppos = 0;

			pal = 7;

			if(clicks[pos] == 0)
				pal = 6;
			if(clicks[pos] < 0)
				pal = 5;

			ppos = pos + 1;
			drawText(0, pos, 4, "%d:", ppos);
			if(clicks[pos] == 1)
				drawText(3, pos, pal, "%d frame   ", clicks[pos]);
			else
				drawText(3, pos, pal, "%d frames  ", clicks[pos]);

			if(clicks[pos] >= 0)
				pos++;

			if(pos > 9)
				end = 1;
			drawoffset = 0;
		} 
		
		if(changed)
		{
			u8 show = OBJ_HIDE;
			
			oamSet(0, x, y, 3, 0, 0, 0, sprpal); 
			if(view == 0 || view == 2)
				show = OBJ_SHOW;
			oamSetEx(0, OBJ_SMALL, show);
			oamSetVisible(0, show);	

			show = OBJ_HIDE;
			
			oamSet(8, x2, y2, 3, 0, 0, 0, sprpal); 
			if(view == 1 || view == 2)
				show = OBJ_SHOW;
			oamSetEx(8, OBJ_SMALL, show);
			oamSetVisible(8, show);		
			changed = 0;
		}
		
		if(audio)
		{
			if(y == 96)
			{
				spcEffect(2, 1, 15*16+sound);
				if(sound == 0)
					sound = 15;
				else
					sound = 0;
			}
			spcProcess();	
		}

		WaitForVBlank();
		
		if(y > 132 + vary)
		{
			speed = -1;
			change = 1;
			if(variation)
			{
				if(rand() % 2)
					vary = rand() % 7;
				else
					vary = -1 * rand() % 7;
			}
		}
		
		if(y < 60 + vary)
		{
			speed = 1;
			change = 1;
			if(variation)
			{
				if(rand() % 2)
					vary = rand() % 7;
				else
					vary = -1 * rand() % 7;
			}
		}
	
		y += speed;
		x2 += speed;
		
		if(y == 96) // Red on the spot
		{
			sprpal = 5;
			changed = 1;
		}
		if(y == 95 || y == 97) //Green one pixel before or after
		{
			sprpal = 6;
			changed = 1;
		}
		if(y == 98 || y == 94) //Back to white two pixels before or after
		{
			sprpal = 7;
			changed = 1;
		}
		
		if(view == 0 || view == 2)
			oamSetXY(0, x, y);

		if(view == 1 || view == 2)
			oamSetXY(8, x2, y2);
		
		pressed = PadPressed(0);		
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_MANUALLAG);
			redraw = 1;
		}
		
		if(pressed == KEY_B)
			end = 1;		
		
		if(pressed == KEY_A)
		{
			if(change)
			{                
				clicks[pos] = (y - 96) * speed;
				drawoffset = 1;
				if(clicks[pos] >= 0)
					change = 0;
			}      
		}
		
		if(pressed == KEY_X)
		{
			view ++;
			if(view > 2)
				view = 0;
			changed = 1;
		}
		
		if(pressed == KEY_Y)
		{
			variation = !variation;
			if(!variation)
				vary = 0;
			draw = 1;
		}
		
		if(pressed == KEY_R)
		{
			audio = !audio;
			draw = 1;
		}
	}	
	
	if(pos > 9)
	{
		s16 i = 0;
		u16 size = 0;			
		u16 ppos = 0, count = 0;
		s16 total = 0;
		float frames = 0;
		
		end = 0;
		oamClear(0, 0);

		setBrightness(0);	
		
		setPaletteColor(0x00, RGB5(0, 0, 0));
		consoleInitText(0, 7, &font);
		
		AddTextColor(7, RGB5(31, 31, 31), RGB5(0, 0, 0));
		AddTextColor(6, RGB5(0, 31, 0), RGB5(0, 0, 0));			
		AddTextColor(5, RGB5(0, 27, 27), RGB5(0, 0, 0));	
		
		size = (&back_tiles_end - &back_tiles);
		bgInitTileSet(1, &back_tiles, &back_pal, 1, size, 16*2, BG_16COLORS, 0x6000);			
		
		size = (&back_map_end - &back_map);	
		bgInitMapSet(1, &back_map, size, SC_32x32, 0x2000);

		for(i = 0; i < pos; i++)
		{
			drawText(8, 7+i, 7, "%0.2d", clicks[i]);
			if(clicks[i] >= 0)
			{
				total += clicks[i];
				count ++;
			}
		}
		drawText(6, 11, 5, "+");
		
		if(count > 0)
		{
		    u16 h = 8;
			u16 v = 17; 
			s16 fint, fdec;	
			float frame = 1000f/59.97f, totalms = 0;
			
			frames = (float)total/(float)count;
			fint = (s16)frames;			
			fdec = (s16)((frames - (float)fint)*100.0f);
			fdec ++;
			
			drawText(h - 2, v++, 5, "----");
			drawText(h, v++, 7, "%d/%d=%d.%0.2d frames", total, count, fint, fdec);
			
			totalms = frame * frames;
			fint = (s16)totalms;			
			fdec = (s16)((totalms - (float)fint)*100.0f);
			fdec ++;
						
			drawText(h, v++, 7, "%d.%0.2d milliseconds", fint, fdec);
			
			drawText(3, v++, 6, "Keep in mind that a frame");
			drawText(3, v, 6, "is around 16.68 ms.");
			
			if(total < 5)
				drawText(10, 11, 6, "EXCELLENT REFLEXES!");
			if(total == 0)
				drawText(10, 11, 6, "INCREDIBLE REFLEXES!");
		}

		setMode(BG_MODE1,0); 	
		bgSetDisable(2);
		
		bgSetScroll(1, 0, -1);						
		bgSetScroll(0, 0, -2);	
		while(!end) 
		{
			pressed = PadPressed(0);		
			
			if(pressed == KEY_START)
			{
				DrawHelp(HELP_MANUALLAG);
				redraw = 1;
			}
			
			if(pressed == KEY_B)
				end = 1;
				
			WaitForVBlank();
		}
	}
	Transition();
	oamClear(0, 0);
	return;
}