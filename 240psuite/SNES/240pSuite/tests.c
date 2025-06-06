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
#include "tests.h"
#include "font.h"
#include "help.h"
#include "control.h"
#include "msu.h"

#define PAN_LEFT	0
#define PAN_CENTER	8
#define PAN_RIGHT	15

// volpan	volume(0..15) AND panning(0..15) (volume*16+pan) 
#define playSample(pitch, sfxIndex, volume, pan) spcEffect(pitch, sfxIndex, volume*16+pan)

inline void waitSNDVBlank()
{
	spcProcess();
	//spcFlush();
	WaitForVBlank();
}

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
			StartDMA();
			
			// We add the font, it will be overwritten by sprites they 
			// need to be in 8k vram steps. Lowercase will be available
			consoleInitTextMine(1, 7, &font);			
			
			if(back == 0)
			{
				size = (&donna_tiles1_end - &donna_tiles1);
				bgInitTileSetMine(0, &donna_tiles1, &donna_pal, 0, size, 256*2, BG_256COLORS, 0x2000);	
				CopyExtraTiles(&donna_tiles2, 0x5000, (&donna_tiles2_end-&donna_tiles2));
				
				bgInitMapSetMine(0, &donna_map, (&donna_map_end - &donna_map), SC_32x32, 0x7C00);
				mode = BG_MODE3;
			}
			
			if(back == 1)
			{
				size = (&sonicback_tiles_end - &sonicback_tiles);
				bgInitTileSetMine(0, &sonicback_tiles, &sonicback_pal, 0, size, 32*2, BG_256COLORS, 0x3000);	
				bgInitMapSetMine(0, &sonicback_map, (&sonicback_map_end - &sonicback_map), SC_32x32, 0x1000);
			
				size = (&sonicfloor_tiles_end - &sonicfloor_tiles);
				bgInitTileSetMine(1, &sonicfloor_tiles, &sonicfloor_pal, 2, size, 16*2, BG_16COLORS, 0x6000);	
				bgInitMapSetMine(1, &sonicfloor_map, (&sonicfloor_map_end - &sonicfloor_map), SC_32x32, 0x5400);	
				mode = BG_MODE3;				
			}
			
			if(back == 2)
			{
				size = (&hstripes_tiles_end - &hstripes_tiles);
				bgInitTileSetMine(1, &hstripes_tiles, &grid_pal, 0, size, 16*2, BG_16COLORS, 0x6000);	
				bgInitMapSetMine(1, &fullscreen_map, (&fullscreen_map_end - &fullscreen_map), SC_32x32, 0x7000);
				
				setPaletteColor(0x00, RGB5(0, 0, 0));
				setPaletteColor(0x01, RGB5(0xff, 0xff, 0xff));
				mode = BG_MODE1;
			}
			
			if(back == 3)
			{
				size = (&check_tiles_end - &check_tiles);
				bgInitTileSetMine(1, &check_tiles, &grid_pal, 0, size, 16*2, BG_16COLORS, 0x6000);						
				bgInitMapSetMine(1, &fullscreen_map, (&fullscreen_map_end - &fullscreen_map), SC_32x32, 0x7000);
				setPaletteColor(0x00, RGB5(0, 0, 0));
				setPaletteColor(0x01, RGB5(0xff, 0xff, 0xff));
				mode = BG_MODE1;
			}
			
			oamInitGfxSetMine(&sprites_tiles, (&sprites_tiles_end - &sprites_tiles), &sprites_pal, 16*2, 7, 0, OBJ_SIZE32);			
			
			setMode(mode,0);			
			
			if(back == 0)
				bgSetScroll(0, 0, -1);
			if(back == 1)
			{
				bgSetScroll(0, x*2, -1);
				bgSetScroll(1, x*4, -97);
			}
			if(back > 1)
			{
				bgSetScroll(1, 0, -1);
				bgSetDisable(0);
				bgSetDisable(2);
			}				
			
			EndDMA();
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
				drawText(21, 1, 0, "odd");
				drawText(25, 1, 0, "frames");
			}
			else
			{
				drawText(21, 1, 0, "even");
				drawText(26, 1, 0, "frames");
			}
			text --;
			if(!text)						
				CleanFontMap();			
		}
				
		WaitForVBlank();
		
		drawShadow = !drawShadow;
		
		pressed = PadPressed(0);
		held = PadHeld(0);
		
		if(pressed & KEY_START)
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
			
		if(pressed & KEY_B)
			end = 1;		
			
		if(pressed & KEY_X)
		{
			sprite = !sprite;
			changesprite = 1;
		}
		
		if(pressed & KEY_A)
		{		
			CleanFontMap();	
			odd = !odd;
			drawShadow = !drawShadow;
			text = 30;
		}
		
		if(pressed & KEY_R)
		{		
			back ++;
			redraw = 1;
			
			if(back > 3)
				back = 0;
		}
		
		if(pressed & KEY_L)
		{		
			back --;
			redraw = 1;
			
			if(back < 0)
				back = 3;
		}
	}	
	transitionAndClear();
	
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
			StartDMA();
			
			// We add the font, it will be overwritten by sprites they 
			// need to be in 8k vram steps. Lowercase will be available
			consoleInitTextMine(1, 7, &font);	
			
			if(back == 0)
			{
				size = (&donna_tiles1_end - &donna_tiles1);
				bgInitTileSetMine(0, &donna_tiles1, &donna_pal, 0, size, 256*2, BG_256COLORS, 0x2000);	
				CopyExtraTiles(&donna_tiles2, 0x5000, (&donna_tiles2_end-&donna_tiles2));
				
				bgInitMapSetMine(0, &donna_map, (&donna_map_end - &donna_map), SC_32x32, 0x7C00);
				mode = BG_MODE3;
			}
			
			if(back == 1)
			{
				size = (&sonicback_tiles_end - &sonicback_tiles);
				bgInitTileSetMine(0, &sonicback_tiles, &sonicback_pal, 0, size, 32*2, BG_256COLORS, 0x3000);	
				bgInitMapSetMine(0, &sonicback_map, (&sonicback_map_end - &sonicback_map), SC_32x32, 0x1000);
			
				size = (&sonicfloor_tiles_end - &sonicfloor_tiles);
				bgInitTileSetMine(1, &sonicfloor_tiles, &sonicfloor_pal, 2, size, 16*2, BG_16COLORS, 0x6000);	
				bgInitMapSetMine(1, &sonicfloor_map, (&sonicfloor_map_end - &sonicfloor_map), SC_32x32, 0x5400);	
				mode = BG_MODE3;				
			}
			
			if(back == 2)
			{
				size = (&hstripes_tiles_end - &hstripes_tiles);
				bgInitTileSetMine(1, &hstripes_tiles, &grid_pal, 0, size, 16*2, BG_16COLORS, 0x6000);	
				bgInitMapSetMine(1, &fullscreen_map, (&fullscreen_map_end - &fullscreen_map), SC_32x32, 0x7000);
				
				setPaletteColor(0x00, RGB5(0, 0, 0));
				setPaletteColor(0x01, RGB5(0xff, 0xff, 0xff));
				mode = BG_MODE1;
			}
			
			if(back == 3)
			{
				size = (&check_tiles_end - &check_tiles);
				bgInitTileSetMine(1, &check_tiles, &grid_pal, 0, size, 16*2, BG_16COLORS, 0x6000);						
				bgInitMapSetMine(1, &fullscreen_map, (&fullscreen_map_end - &fullscreen_map), SC_32x32, 0x7000);
				setPaletteColor(0x00, RGB5(0, 0, 0));
				setPaletteColor(0x01, RGB5(0xff, 0xff, 0xff));
				mode = BG_MODE1;
			}
			
			oamInitGfxSetMine(&striped_tiles, (&striped_tiles_end - &striped_tiles), &striped_pal, 16*2, 7, 0, OBJ_SIZE32);			
			
			setMode(mode, 0);			
			
			if(back == 0)
				bgSetScroll(0, 0, -1);
			if(back == 1)
			{
				bgSetScroll(0, x*2, -1);
				bgSetScroll(1, x*4, -97);
			}
			if(back > 1)
			{
				bgSetScroll(1, 0, -1);
				bgSetDisable(0);
				bgSetDisable(2);
			}				
			
			oamSet(0, x, y, 2, 0, 0, 0, 7); 
			oamSetEx(0, OBJ_SMALL, OBJ_SHOW);
			oamSetVisible(0, OBJ_SHOW);
				
			EndDMA();
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
		
		if(pressed & KEY_START)
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
			
		if(pressed & KEY_B)
			end = 1;		
		
		if(pressed & KEY_R)
		{		
			back ++;
			redraw = 1;
			
			if(back > 3)
				back = 0;
		}
		
		if(pressed & KEY_L)
		{		
			back --;
			redraw = 1;
			
			if(back < 0)
				back = 3;
		}
	}	
	transitionAndClear();
	
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
			StartDMA();
			
			InitTextColor(0, 7, RGB5(31, 31, 31), RGB5(0, 0, 0));			
			
			if(!vert)
				bgInitTileSetMine(1, &hstripes_tiles, &grid_pal, 0, (&hstripes_tiles_end - &hstripes_tiles), 16*2, BG_16COLORS, 0x6000);	
			else
				bgInitTileSetMine(1, &vstripes_tiles, &grid_pal, 0, (&vstripes_tiles_end - &vstripes_tiles), 16*2, BG_16COLORS, 0x6000);	
				
			bgInitMapSetMine(1, &fullscreen_map, (&fullscreen_map_end - &fullscreen_map), SC_32x32, 0x7000);
			
			setPaletteColor(0x00, RGB5(0, 0, 0));
			setPaletteColor(0x01, RGB5(0xff, 0xff, 0xff));
			
			setMode(BG_MODE1,0); 								
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);
			EndDMA();
			redraw = 0;
		}
		
		if(alternate || pressed & KEY_R)
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
			if(snes_50hz)
			{
				if(frame == 50)
					frame = 0;	
			}
			else
			{
				if(frame == 60)
					frame = 0;			
			}
		}
		
		WaitForVBlank();
		
		pressed = PadPressed(0);
		
		if(pressed & KEY_START)
		{
			DrawHelp(HELP_STRIPES);
			redraw = 1;
		}
		
		if(pressed & KEY_X)
			alternate = !alternate;
			
		if(pressed & KEY_A)
		{
			drawframe = !drawframe;
			if(!drawframe)
				CleanFontMap();
		}
			
		if(pressed & KEY_Y)
		{
			vert = !vert;
			redraw = 1;
		}		
			
		if(pressed & KEY_B)
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
			StartDMA();
			
			InitTextColor(0, 7, RGB5(31, 31, 31), RGB5(0, 0, 0));		
						
			bgInitTileSetMine(1, &check_tiles, &grid_pal, 0, (&check_tiles_end - &check_tiles), 16*2, BG_16COLORS, 0x6000);	
				
			bgInitMapSetMine(1, &fullscreen_map, (&fullscreen_map_end - &fullscreen_map), SC_32x32, 0x7000);
			
			setPaletteColor(0x00, RGB5(0, 0, 0));
			setPaletteColor(0x01, RGB5(0xff, 0xff, 0xff));
			
			setMode(BG_MODE1,0); 								
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);
			EndDMA();
			redraw = 0;
		}
		
		if(alternate || pressed & KEY_R)
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
			if(snes_50hz)
			{
				if(frame == 50)
					frame = 0;
			}
			else
			{
				if(frame == 60)
					frame = 0;
			}
		}
		
		WaitForVBlank();
		
		pressed = PadPressed(0);
		
		if(pressed & KEY_START)
		{
			DrawHelp(HELP_CHECK);
			redraw = 1;
		}
		
		if(pressed & KEY_A)
			alternate = !alternate;
			
		if(pressed & KEY_X)
		{
			drawframe = !drawframe;
			if(!drawframe)
				CleanFontMap();
		}
			
		if(pressed & KEY_B)
			end = 1;
	}	
	Transition();
	
	return;
}

void loadStopWatch(u16 *xpos, u16 *spriteIndex, u16 *numberIndex, u16 y, u16 drawCircles)
{
	StartDMA();
	
	consoleInitTextMine(0, 7, &font);	
	
	if(drawCircles)
	{
		bgInitTileSetMine(1, &lagtest_tiles, &lagtest_pal, 0, (&lagtest_tiles_end - &lagtest_tiles), 16*2, BG_16COLORS, 0x6000);	
		bgInitMapSetMine(1, &lagtest_map, (&lagtest_map_end - &lagtest_map), SC_32x32, 0x1000);
		setPaletteColor(0x01, RGB5(31, 31, 31));
	}
	else
	{
		bgInitTileSetMine(1, &dissapear_tiles, &dissapear_pal, 0, (&dissapear_tiles_end - &dissapear_tiles), 16*2, BG_16COLORS, 0x6000);	
		bgInitMapSetMine(1, &dissapear_map, (&dissapear_map_end - &dissapear_map), SC_32x32, 0x1000);
	}
	
	oamInitGfxSetMine(&numbers_tiles, &numbers_tiles_end - &numbers_tiles,	&numbers_pal, 16*2, 0, 0x2000, OBJ_SIZE32);
	
	/*****Numbers*****/
	
	// Hours
	DrawNumber(xpos[0], y, spriteIndex[0], numberIndex[0], 0);	
	DrawNumber(xpos[1], y, spriteIndex[1], numberIndex[0], 0);	
	
	// Minutes
	DrawNumber(xpos[2], y, spriteIndex[2], numberIndex[0], 0);	
	DrawNumber(xpos[3], y, spriteIndex[3], numberIndex[0], 0);	
	
	// Seconds
	DrawNumber(xpos[4], y, spriteIndex[4], numberIndex[0], 0);
	DrawNumber(xpos[5], y, spriteIndex[5], numberIndex[0], 0);
	
	// Frames
	DrawNumber(xpos[6], y, spriteIndex[6], numberIndex[0], 0);
	DrawNumber(xpos[7], y, spriteIndex[7], numberIndex[0], 0);
	
	if(drawCircles)
	{
		/*****Circles*****/			
		u16 numberTopIndex[8] = { 64, 68, 72, 76, 80, 84, 88, 92 };	
		
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

		/**** Set palettes ****/
		setPaletteColor(0x91, RGB5(31, 31, 31));
		setPaletteColor(0xA3, RGB5(31, 0, 0));
		setPaletteColor(0xA1, RGB5(31, 0, 0));
		setPaletteColor(0xB1, RGB5(0, 0, 31));
		
		AddTextColor(7, RGB5(0, 0, 0), RGB5(31, 31, 31));
	}
	else
	{
		setPaletteColor(0x81, RGB5(0, 31, 0));
		setPaletteColor(0x91, RGB5(0, 31, 0));
		
		setPaletteColor(1, RGB5(0, 0, 0));
		setPaletteColor(2, RGB5(0, 31, 0));
		
		AddTextColor(7, RGB5(31, 31, 31), RGB5(20, 20, 20));
	}
	
	drawText(1, 1, 7, "hours   minutes seconds frames");
	
	setMode(BG_MODE1,0);
	bgSetScroll(1, 0, -1);	
	bgSetDisable(2);
	
	EndDMA();
}

void DrawStopWatch(timecode *tc, u16 *xpos, u16 *spriteIndex, u16 *numberIndex, u16 y, u16 bgcol)
{
	u16 msd, lsd;
	
	if(snes_50hz)	
	{
		if(tc->frames > 49)
		{
			tc->frames = 0;
			tc->seconds ++;	
		}
	}
	else
	{
		if(tc->frames > 59)
		{
			tc->frames = 0;
			tc->seconds ++;	
		}
	}

	if(tc->seconds > 59)
	{
		tc->seconds = 0;
		tc->minutes ++;
	}

	if(tc->minutes > 59)
	{
		tc->minutes = 0;
		tc->hours ++;
	}

	if(tc->hours > 99)
		tc->hours = 0;
	  
	// Hours			
	lsd = tc->hours % 10;
	msd = tc->hours / 10;			
	ChangeNumber(xpos[0], y, spriteIndex[0], numberIndex[msd], 0);	
	ChangeNumber(xpos[1], y, spriteIndex[1], numberIndex[lsd], 0);	
	
	// Minutes			
	lsd = tc->minutes % 10;
	msd = tc->minutes / 10;			
	ChangeNumber(xpos[2], y, spriteIndex[2], numberIndex[msd], 0);	
	ChangeNumber(xpos[3], y, spriteIndex[3], numberIndex[lsd], 0);	
	
	// Seconds		
	lsd = tc->seconds % 10;
	msd = tc->seconds / 10;			
	ChangeNumber(xpos[4], y, spriteIndex[4], numberIndex[msd], 0);	
	ChangeNumber(xpos[5], y, spriteIndex[5], numberIndex[lsd], 0);	
	
	// frames		
	lsd = tc->frames % 10;
	msd = tc->frames / 10;			
	ChangeNumber(xpos[6], y, spriteIndex[6], numberIndex[msd], bgcol);	
	ChangeNumber(xpos[7], y, spriteIndex[7], numberIndex[lsd], bgcol);	
}

void PassiveLagTest()
{
	timecode tc;
	u16 pressed, end = 0;
	u16 framecnt = 0;
	u16 numberIndex[12] = { 0, 8, 64, 72, 128, 136, 192, 200, 256, 264, 320, 328};
	u16 spriteIndex[8] = { 0, 4, 8, 12, 16, 20, 24, 28 };
	u16 xpos[8] = { 5, 30, 70, 95, 135, 160, 200, 225 };
	u16 y = 20, running = 1, redraw = 1, color = 1, bgcol = 0xa, toggle = 0;	

	memset(&tc, 0, sizeof(timecode));		
	while(!end) 
	{	
		u16 count = 0, mul = 1;
	
		if(redraw)
		{
			loadStopWatch(xpos, spriteIndex, numberIndex, y, 1);
			redraw = 0;
		}
		
		if(framecnt > 7)
			framecnt = 0;
			
		if(running)
		{
			if(toggle)
			{
				if(color)
					bgcol = 0xb;
				setPaletteColor(0x01, RGB5(0, 0, 0));
			}
			else 
			{
				if(color)
					bgcol = 0xa;
				setPaletteColor(0x01, RGB5(31, 31, 31));
			}
			toggle = !toggle;
		}
	
		DrawStopWatch(&tc, xpos, spriteIndex, numberIndex, y, bgcol);
		
		count = framecnt;
		if(count > 3)
		{
			count -= 4;
			mul = 2;
		}
			
		ChangeCircle(count*64, 70*mul, 192, numberIndex[10], color ? 2 : 1);
	
		WaitForVBlank();
		
		if(running)
		{	
			tc.frames ++;
			framecnt ++;
		}
		pressed = PadPressed(0);
		
		if(pressed & KEY_START)
		{
			DrawHelp(HELP_LAG);
			redraw = 1;
		}
		
		if(pressed & KEY_A)
			running = !running;
				
		if(pressed & KEY_B)
			end = 1;
			
		if(pressed & KEY_Y)
		{
			color = !color;
			if(!color)
			{
				bgcol = 0;
				setPaletteColor(3, RGB5(15, 15, 15));
			}
			else
				setPaletteColor(3, RGB5(0, 0, 31));
		}
			
		if (pressed & KEY_X && !running)
		{
			memset(&tc, 0, sizeof(timecode));
			framecnt = 0;
		}		
	}
	transitionAndClear();
	return;
}

void HScrollTest() 
{	
	u16 pressed, end = 0, vertical = 0;
	u16 redraw = 1, x = 0, y = 0, pause = 0, frame = 0;
	s16 speed = 1, acc = 1;
		
	while(!end) 
	{		
		if(redraw)
		{
			StartDMA();
						
			if(!vertical)
			{
				bgInitTileSetMine(0, &sonicback_tiles, &sonicback_pal, 0, (&sonicback_tiles_end - &sonicback_tiles), 32*2, BG_256COLORS, 0x3000);	
				bgInitMapSetMine(0, &sonicback_map, (&sonicback_map_end - &sonicback_map), SC_32x32, 0x1000);
				
				bgInitTileSetMine(1, &sonicfloor_tiles, &sonicfloor_pal, 2, (&sonicfloor_tiles_end - &sonicfloor_tiles), 16*2, BG_16COLORS, 0x6000);	
				bgInitMapSetMine(1, &sonicfloor_map, (&sonicfloor_map_end - &sonicfloor_map), SC_32x32, 0x5400);			
				
				setMode(BG_MODE3,0); 			
				
				bgSetScroll(0, 0, -1);
				bgSetScroll(1, 0, -97);
			}
			else
			{			
				bgInitTileSetMine(0, &kiki_tiles, &kiki_pal, 0, (&kiki_tiles_end - &kiki_tiles), 16*2, BG_16COLORS, 0x6000);	
				bgInitMapSetMine(0, &kiki_map, (&kiki_map_end - &kiki_map), SC_32x64, 0x1000);
				
				setMode(BG_MODE1,0); 
				bgSetDisable(1);
				bgSetDisable(2);
				
				bgSetScroll(0, 0, -1);
			}
			
			EndDMA();
			redraw = 0;
		}
		
		if(!vertical)
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
		}

		frame ++;
		if(frame > 90)
			frame = 1;
		
		if(!vertical)
		{
			bgSetScroll(0, x/2, -1);
			bgSetScroll(1, x, -97);
		}
		else
		{
			bgSetScroll(0, 0, y/2);
		}
		
		WaitForVBlank();
		
		pressed = PadPressed(0);
		
		if(pressed & KEY_START)
		{
			DrawHelp(HELP_HSCROLL);
			redraw = 1;
		}
		
		if(pressed & KEY_B)
			end = 1;	
			
		if(pressed & KEY_UP)
			speed++;	
			
		if(pressed & KEY_DOWN)
			speed--;
			
		if(speed > 20)        
			speed = 20;          

		if(speed < 0)        
			speed = 0;  
			
		if (pressed & KEY_A)
			pause = !pause;

		if (pressed & KEY_X)
			acc *= -1;
		
		if (pressed & KEY_Y)
		{
			vertical = !vertical;
			redraw = 1;
		}

		if(!pause)
		{
			if(!vertical)
				x += acc*speed;
			else
				y -= acc*speed;
		}
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
			StartDMA();
			
			bgInitTileSetMine(0, &smallgrid_tiles, &grid_pal, 0, (&smallgrid_tiles_end - &smallgrid_tiles), 16*2, BG_16COLORS, 0x6000);					
			bgInitMapSetMine(0, &fullscreen_map, (&fullscreen_map_end - &fullscreen_map), SC_32x32, 0x7000);
			
			setPaletteColor(0x00, RGB5(0, 0, 0));
			setPaletteColor(0x01, RGB5(0xff, 0xff, 0xff));
			
			setMode(BG_MODE1,0); 								
			bgSetDisable(1);
			bgSetDisable(2);
			
			bgSetScroll(0, posx, posy);
			EndDMA();
			redraw = 0;
		}
				
		WaitForVBlank();
		
		pressed = PadPressed(0);
		
		if(pressed & KEY_START)
		{
			DrawHelp(HELP_VSCROLL);
			redraw = 1;
		}
		
		if(pressed & KEY_B)
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
			StartDMA();
			
			ClearScreen(0);
			
			oamInitGfxSetMine(&LEDsprites_tiles, (&LEDsprites_tiles_end - &LEDsprites_tiles), &LEDsprites_pal, 16*2, 7, 0, OBJ_SIZE8);
			
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
			EndDMA();
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
		
		if(pressed & KEY_START)
		{
			DrawHelp(HELP_LED);
			redraw = 1;
		}
		
		if(pressed & KEY_B)
			end = 1;		
		
		if(pressed & KEY_A)
		{
			shown = !shown;
			changed = 1;
		}
			
		if(pressed & KEY_L)
		{
			sprite --;
			changed = 1;
		}
			
		if(pressed & KEY_R)
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
	transitionAndClear();
	return;
}

enum sounds{ 	jump, beep, beep2fr,
				hz250, khz1, khz4,
				khz8, khz9, khz11,
				khz13, khz15, khz8fr 	};

void ExecutePulseTrain()
{
	s16 f = 0, w = 0;
	//Sync
	
	// Each playback is 4 frames
	for(f = 0; f < 5; f++)
	{		
		playSample(8, khz8fr, 15, PAN_CENTER);
		for(w = 0; w < 4; w++)
			waitSNDVBlank();
	}
}

void ExecuteSilence()
{
	s16 frame = 0;
	
	//Silence
	for(frame = 0; frame < 20; frame++)
		waitSNDVBlank();
}

void PlayScale(enum sounds note, s16 pan)
{
	int i, j, volume = 15;
	
	// 15 times 10 frames each -> Only 5 full frames usable for windowing
	for(i = 1; i < 16; i++)
	{
		playSample(i, note, 15, pan);
		for(j = 0; j < 6; j++)
			waitSNDVBlank();
		for(j = 0; j < 4; j++)
		{
			spcStop();
			waitSNDVBlank();
		}
	}
}

/*
			Let desired DSP pitch be P.
			
			https://wiki.superfamicom.org/spc700-reference

Pitch->Hz
               P
HZ = 32000 * ------
              2^12

(HZ = P * 7.8125)

Hz->Pitch
             HZ
P = 2^12 * -------
            32000

(P = HZ / 7.8125)

The highest pitch will reproduce the sound at approx. 2 octaves higher (~128KHz sample rate). The lowest pitch is basically not limited (but you will lose accuracy at lower pitches).

A few pitch->interval relations...

    400h     800h   1000h     2000h   3FFFh
-----|--------|-------|---------|-------|-----
   -2oct   -1oct   original   +1oct   +2oct
                    sound
                (best quality!)

*/
// 1 to 15, 8 is 32khz correct

//spcFadeModuleVolume(0, 255); 	

void ExecuteMDFourier()
{
	int i = 0;
	
	waitSNDVBlank();
	ExecutePulseTrain();
	ExecuteSilence();
	
	for(i = 0; i < 1; i++)
	{
		PlayScale(hz250, PAN_CENTER);
		PlayScale(khz1, PAN_CENTER);
		PlayScale(khz4, PAN_CENTER);
		PlayScale(khz8, PAN_CENTER);
		PlayScale(khz9, PAN_CENTER);
		PlayScale(khz11, PAN_CENTER);
		PlayScale(khz13, PAN_CENTER);
		PlayScale(khz15, PAN_CENTER);
	}
	
	ExecuteSilence();
	ExecutePulseTrain();
}

void MDFourier(u8 boot)
{
	u16 redraw = 1, change = 0, end = 0;
	u16 pressed;	

	if(boot)
		ExecutePulseTrain();
	else
		DrawHelp(HELP_MDFOURIER);
		
	while(!end) 
	{
		if(redraw)
		{
			u16 size = 0;
					
			StartDMA();	
			
			CleanFontMap();
			consoleInitTextMine(0, 7, &font);
			AddTextColor(7, RGB5(31, 31, 31), RGB5(0, 0, 0));
			AddTextColor(6, RGB5(0, 31, 0), RGB5(0, 0, 0));	
			AddTextColor(5, RGB5(31, 0, 0), RGB5(0, 0, 0));
			
			size = (&back_tiles_end - &back_tiles);
			bgInitTileSetMine(1, &back_tiles, &back_pal, 1, size, 16*2, BG_16COLORS, 0x6000);			
			
			size = (&back_map_end - &back_map);	
			bgInitMapSetMine(1, &back_map, size, SC_32x32, 0x2000);
						
			setMode(BG_MODE1,0); 	
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);			
			
			EndDMA();		
			
			redraw = 0;
			change = 1;
		}			
		
		if(change)
		{
			u16 y = 7;						
			
			drawText(10, 7, 6, "MDFourier"); 	
			if(!msu1available())
				drawText(11, 16, 7, "Press A");
			else
			{
				drawText(11, 16, 7, "Press A for MDF");
				drawText(11, 17, 7, "Press X for MSU"); 
			}
			
			change = 0;
		}	

		waitSNDVBlank();	
		
		pressed = PadPressed(0);
		
		if(pressed & KEY_START)
		{
			DrawHelp(HELP_MDFOURIER);
			redraw = 1;
		}
		
		if(pressed & KEY_X)
		{
			if(msu1available())
			{
				int i = 0;
				
				drawText(6, 17, 5, "Please record playback");
				ExecuteMDFourier();
				for(i = 0; i < 4; i++)	
					waitSNDVBlank();
				msu1play(1);
				redraw = 1;
			}
		}

		if(pressed & KEY_A)
		{
			drawText(6, 16, 5, "Please record playback");
			ExecuteMDFourier();
			redraw = 1;
		}

		if(pressed & KEY_B)
			end = 1;			
	}
	spcStop();
	waitSNDVBlank();
	Transition();
	return;
}


void SoundTest()
{
	u16 redraw = 1, change = 0, end = 0;
	u16 pressed;	
	s16 sound = 1, effect = 0, pan[3] = { PAN_LEFT, PAN_CENTER, PAN_RIGHT };

	while(!end) 
	{
		if(redraw)
		{
			u16 size = 0;
					
			StartDMA();	
			
			CleanFontMap();
			consoleInitTextMine(0, 7, &font);
			AddTextColor(7, RGB5(31, 31, 31), RGB5(0, 0, 0));
			AddTextColor(6, RGB5(0, 31, 0), RGB5(0, 0, 0));	
			AddTextColor(5, RGB5(31, 0, 0), RGB5(0, 0, 0));
			
			size = (&back_tiles_end - &back_tiles);
			bgInitTileSetMine(1, &back_tiles, &back_pal, 1, size, 16*2, BG_16COLORS, 0x6000);			
			
			size = (&back_map_end - &back_map);	
			bgInitMapSetMine(1, &back_map, size, SC_32x32, 0x2000);
						
			setMode(BG_MODE1,0); 	
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);			
			
			EndDMA();		
			
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
		
		if(pressed & KEY_START)
		{
			DrawHelp(HELP_SOUND);
			redraw = 1;
		}
		
		if(pressed & KEY_A)
			playSample(2, 0, 15, pan[sound]);
			
		if(pressed & KEY_B)
			end = 1;	
		
		if(pressed & KEY_LEFT)
		{
			sound --;
			change = 1;
		}
			
		if(pressed & KEY_RIGHT)
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
	u16 pressed, end = 0, change = 1, draw = 0, audio = 1, sound = 0;
	u16 redraw = 1, changed = 0, variation = 1, pos = 0, drawoffset = 0;
	s16 x = 112, y = 96, x2 = 112, y2 = 96, speed = 1, vary = 1;
	s16 clicks[10], pal = 7, sprpal = 7, view = 0, trigger = 0;
		
	while(!end) 
	{		
		if(redraw)
		{
			StartDMA();
			
			ClearScreen(1);
			
			consoleInitTextMine(0, 7, &font);
			AddTextColor(7, RGB5(31, 31, 31), RGB5(0, 0, 0));
			AddTextColor(6, RGB5(0, 31, 0), RGB5(0, 0, 0));	
			AddTextColor(5, RGB5(31, 0, 0), RGB5(0, 0, 0));
			AddTextColor(4, RGB5(0, 25, 25), RGB5(0, 0, 0));
			
			AddTextColor(15, RGB5(31, 31, 31), RGB5(0, 0, 0));
			AddTextColor(14, RGB5(0, 31, 0), RGB5(0, 0, 0));	
			AddTextColor(13, RGB5(31, 0, 0), RGB5(0, 0, 0));
			AddTextColor(12, RGB5(0, 25, 25), RGB5(0, 0, 0));
			
			oamInitGfxSetMine(&lagspr_tiles, (&lagspr_tiles_end - &lagspr_tiles), &lagspr_pal, 16*2, 7, 0x2000, OBJ_SIZE32);
			
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
			EndDMA();
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
				playSample(4, beep, 15, sound);
				setPaletteColor(0, RGB5(0xff, 0xff, 0xff));
				if(sound == PAN_LEFT)
					sound = PAN_RIGHT;
				else
					sound = PAN_LEFT;
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
			setPaletteColor(0, RGB5(0, 0, 0));
		}
		if(y == 98 || y == 94) //Back to white two pixels before or after
		{
			sprpal = 7;
			changed = 1;
			setPaletteColor(0, RGB5(0, 0, 0));
		}
		
		if(view == 0 || view == 2)
			oamSetXY(0, x, y);

		if(view == 1 || view == 2)
			oamSetXY(8, x2, y2);
		
		pressed = PadPressed(0);
		
		if(pressed & KEY_START)
		{
			DrawHelp(HELP_MANUALLAG);
			redraw = 1;
		}
		
		if(pressed & KEY_B)
			end = 1;		
		
		if(pressed & KEY_UP)
			trigger = 1;
		if(pressed & KEY_DOWN)
			trigger = 1;
		if(pressed & KEY_LEFT)
			trigger = 1;
		if(pressed & KEY_RIGHT)
			trigger = 1;
		if(pressed & KEY_A)
			trigger = 1;
		if(trigger)
		{
			if(change)
			{                
				clicks[pos] = (y - 96) * speed;
				drawoffset = 1;
				if(clicks[pos] >= 0)
					change = 0;
				if(audio && clicks[pos] != 0)
					playSample(2, beep, 15, sound); // play 500hz tone
			}      
		}
		trigger = 0;
		
		if(pressed & KEY_X)
		{
			view ++;
			if(view > 2)
				view = 0;
			changed = 1;
		}
		
		if(pressed & KEY_Y)
		{
			variation = !variation;
			if(!variation)
				vary = 0;
			draw = 1;
		}
		
		if(pressed & KEY_R)
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
		s16 frames = 0;  // float crashes the newest pvsneslib with COMPUTE_PENDING_CALCULATIONS: Result (451/$1c3) of a computation is out of 8bit range.
		
		end = 0;
		oamClear(0, 0);

		StartDMA();	
		
		setPaletteColor(0x00, RGB5(0, 0, 0));
		consoleInitTextMine(0, 7, &font);
		
		AddTextColor(7, RGB5(31, 31, 31), RGB5(0, 0, 0));
		AddTextColor(6, RGB5(0, 31, 0), RGB5(0, 0, 0));			
		AddTextColor(5, RGB5(0, 27, 27), RGB5(0, 0, 0));	
		
		size = (&back_tiles_end - &back_tiles);
		bgInitTileSetMine(1, &back_tiles, &back_pal, 1, size, 16*2, BG_16COLORS, 0x6000);			
		
		size = (&back_map_end - &back_map);	
		bgInitMapSetMine(1, &back_map, size, SC_32x32, 0x2000);
		
		EndDMA();

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
			s16 fint;	
			s16 frame, totalms = 0;
			
			if(snes_50hz)	
				frame = 20; //20.0f;
			else
				frame = 16; //16.6392f;
		
			
			frames = total/count;
			fint = frames;			
			
			drawText(h - 2, v++, 5, "----");
			drawText(h, v++, 7, "%d/%d=%d frames", total, count, fint);
			
			totalms = frame * frames;
			fint = totalms;			
						
			drawText(h, v++, 7, "%d milliseconds", fint);
			
			
			drawText(3, v++, 6, "These are your reflexes,");
			drawText(3, v++, 6, "not a lag test. A frame is");
			if(snes_50hz)	
				drawText(3, v, 6, "20 ms in PAL.");
			else
				drawText(3, v, 6, "around 16.63 ms.");
			
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
			
			if(pressed & KEY_START)
			{
				DrawHelp(HELP_MANUALLAG);
				redraw = 1;
			}
			
			if(pressed & KEY_B)
				end = 1;
				
			WaitForVBlank();
		}
	}
	transitionAndClear();
	return;
}

u16 ConvertToFrames(timecode *time)
{
	u16	frames = 0;

	if(!time)
		return frames;

	frames = time->frames;
	if(snes_50hz)
		frames += time->seconds*50;
	else
		frames += time->seconds*60;
	frames += time->minutes*3600;
	frames += time->hours*216000;
	return frames;
}

void ConvertFromFrames(timecode *value, u16 Frames)
{
	if(!value)
		return;
	value->hours = Frames / 216000;
	Frames = Frames % 216000;
	value->minutes = Frames / 3600;
	Frames = Frames % 3600;
	if(snes_50hz)
	{
		value->seconds = Frames / 50;
		value->frames = Frames % 50;
	}
	else
	{
		value->seconds = Frames / 60;
		value->frames = Frames % 60;
	}
}

void Alternate240p480i()
{
	u16 redraw = 1, end = 0;
	u16 pressed, frames = 0;
	u16 frames = 0, seconds = 0, minutes = 0, hours = 0;
	u8 res = 0, current = 0, status = 0, oldinterlaced, changed = 1;
	timecode *times = NULL;

	oldinterlaced = interlaced;
	times = (timecode*)malloc(sizeof(timecode)*20);
	if(!times)
		return;
	
	if(interlaced)
		ClearInterlaced();
		
	while(!end) 
	{
		if(redraw)
		{
			u16 size = 0;
					
			StartDMA();	
			
			consoleInitTextMine(0, 7, &font);
			AddTextColor(7, RGB5(31, 31, 31), RGB5(0, 0, 0));
			AddTextColor(6, RGB5(0, 31, 0), RGB5(0, 0, 0));	
			AddTextColor(5, RGB5(31, 0, 0), RGB5(0, 0, 0));
			AddTextColor(4, RGB5(0, 26, 26), RGB5(0, 0, 0));
			
			ClearScreen(1);
						
			setMode(BG_MODE1,0); 	
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);					
			
			EndDMA();
			
			changed = 1;
		}			
		
		drawText(1, 3, 7, "Elapsed Timer: %02d:%02d:%02d:%02d", hours, minutes, seconds, frames);
		
		if(changed)
		{
			u8 i = 0;
				
			if(redraw)
			{
				i = 0;
				redraw = 0;
			}
			else 
				i = current - 1;
				
			drawText(1, 2, 6, "Current Resolution: %s", res == 0 ? "240p" : "480i"); 			
			if(current)
			{
				for(i = 0; i < current; i++)
				{
					u8 len = 0;
					
					if(times[i].type == 0)
					{
						drawText(2, i+5, 4, "Switched %s:", times[i].res == 0 ? "240p" : "480i");
						len = 14;
					}
					else
					{
						drawText(2, i+5, 4, "Viewed:");
						len = 8;
					}

					drawText(3+len, i+5, 7, "%02d:%02d:%02d",
							times[i].minutes, times[i].seconds, times[i].frames);
					
					if(times[i].type != 0 && i >= 1 && i <= 19)
					{
						u16 		framesA = 0, framesB = 0, res = 0;
						timecode 	len;

						framesB = ConvertToFrames(&times[i]);
						framesA = ConvertToFrames(&times[i - 1]);
						res = framesB - framesA;
						ConvertFromFrames(&len, res);
						drawText(20, i+5, 5, "%02d:%02d:%02d",
							len.minutes, len.seconds, len.frames);
					}
				}
			}
			changed = 0;
		}


		WaitForVBlank();	
		
		frames ++;
		if(snes_50hz)	
		{
			if(frames > 49)
			{
				frames = 0;
				seconds ++;	
			}
		}
		else
		{
			if(frames > 59)
			{
				frames = 0;
				seconds ++;	
			}
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
		
		pressed = PadPressed(0);
		
		if(pressed & KEY_START)
		{
			DrawHelp(HELP_ALTERNATE);
			redraw = 1;
		}
		
		if(pressed & KEY_A)
		{
			if(current <= 19)
				current ++;
			else
			{
				current = 1;
				CleanFontMap();
			}

			times[current - 1].frames = frames;
			times[current - 1].minutes = minutes;
			times[current - 1].seconds = seconds;
			times[current - 1].hours = hours;

			status ++;
			if(status == 1)
			{
				times[current - 1].type = 0;
				res = !res;
				times[current - 1].res = res;	
				if(!res)
					ClearInterlaced();
				else
					SetInterlaced();
			}
			if(status == 2)
			{
				times[current - 1].type = 1;
				times[current - 1].res = res;
				status = 0;
			}
			changed = 1;
		}
			
		if(pressed & KEY_B)
			end = 1;	

	}
	if(times)
	{
		free(times);
		times = NULL;
	}
	
	Transition();
	
	if(oldinterlaced)
		SetInterlaced();
	else
		ClearInterlaced();

	return;
}

void RedrawAudioSync()
{	
	u16 i = 0;
	
	StartDMA();
			
	bgInitTileSetMine(0, &audiosync_tiles, &LEDsprites_pal, 0, &audiosync_tiles_end - &audiosync_tiles, 16*2, BG_16COLORS, 0x3000);		
	
	bgInitMapSetMine(0, &audiosync_map, (&audiosync_map_end - &audiosync_map), SC_32x32, 0x7000);
				
	setMode(BG_MODE1,0); 					
	bgSetDisable(1);
	bgSetDisable(2);
	
	// back
	setPaletteColor(0, RGB5(0, 0, 0));
	
	// tiles in background
	for(i = 3; i < 8; i++)
	{
		setPaletteColor(i, RGB5(0, 0, 0));
	}
	
	bgSetScroll(0, 0, -1);
	
	EndDMA();
}

void AudioSyncTest(void) 
{	
	u16 pressed, end = 0;
	u16 redraw = 1, refresh = 1;
	s16 status = -1, acc = -1, x = 128, y = 160;
	
	while(!end) 
	{		
		if(redraw)
		{
			RedrawAudioSync();
			
			StartDMA();
			oamInitGfxSetMine(&LEDsprites_tiles, (&LEDsprites_tiles_end - &LEDsprites_tiles), &LEDsprites_pal, 16*2, 7, 0, OBJ_SIZE8);
			oamSet(0, x, y, 2, 0, 0, 1, 7);
			oamSetEx(0, OBJ_SMALL, OBJ_SHOW);
			oamSetVisible(0, OBJ_SHOW);
			EndDMA();
			redraw = 0;
		}
		
		spcProcess();
		WaitForVBlank();
				
		if(refresh && status == -1)
		{
			status = 0;
			acc = -1;
		}
		
		if(status > -1)
		{
			status++;
			if(status <= 120)
			{
				y += acc;
			}
		}

		if(status >= 20 && status <= 120)
		{
			switch (status)
			{
			case 20:
				break;
			case 40:
				setPaletteColor(3, RGB5(0xff, 0xff, 0xff));
				break;
			case 60:
				acc = 1;
				setPaletteColor(4, RGB5(0xff, 0xff, 0xff));
				break;
			case 80:
				setPaletteColor(5, RGB5(0xff, 0xff, 0xff));
				break;
			case 100:
				setPaletteColor(6, RGB5(0xff, 0xff, 0xff));
				break;
			case 120:
				setPaletteColor(7, RGB5(0xff, 0xff, 0xff));
				break;
			}
		}
		
		if(status == 120)
		{
			playSample(4, beep2fr, 15, PAN_CENTER);
			setPaletteColor(0, RGB5(0xff, 0xff, 0xff));
		}

		if(status == 122)
		{
			u16 pal = 0;
			
			//spcStop(); No need, sample is 2 frames long
			setPaletteColor(0, RGB5(0, 0, 0));
			
			for(pal = 3; pal < 8; pal++)
			{
				setPaletteColor(pal, RGB5(0, 0, 0));
			}

			status = -1;
		}
		
		pressed = PadPressed(0);
		
		if(pressed & KEY_START)
		{
			oamClear(0, 0);
			DrawHelp(HELP_AUDIOSYNC);
			
			redraw = 1;
			refresh = 0;
			status = 121;
			y = 160;
		}
		
		if(pressed & KEY_B)
			end = 1;		
			
		if(pressed & KEY_A)
		{
			refresh = !refresh;
			if(!refresh)
				status = 121;
			else
				y = 160;
		}
		
		oamSetXY(0, x, y);
	}	
	transitionAndClear();
	
	return;
}

void fillGillian(u16 color)
{
	int i = 0;
	
	for(i = 3; i < 15; i++) {
		setPaletteColor(i, color);
	}
	setPaletteColor(0, color);
}

void DisappearingLogo()
{
	timecode tc;
	u16 pressed, end = 0;
	u16 numberIndex[12] = { 0, 8, 64, 72, 128, 136, 192, 200, 256, 264, 320, 328};
	u16 spriteIndex[8] = { 0, 4, 8, 12, 16, 20, 24, 28 };
	u16 xpos[8] = { 5, 30, 70, 95, 135, 160, 200, 225 };
	u16 black = 0, white = 0;
	u16 y = 20, reload = 1, redraw = 0, draw = 1, frames = 0;	

	black = RGB5(0, 0, 0);
	white = RGB5(31, 31, 31);
	memset(&tc, 0, sizeof(timecode));
	
	while(!end)
	{
		if(reload)
		{
			loadStopWatch(xpos, spriteIndex, numberIndex, y, 0);
			reload = 0;
			if(!draw)
				redraw = 1;
		}
		
		DrawStopWatch(&tc, xpos, spriteIndex, numberIndex, y, 0x9);
		
		if(redraw)
		{	
			StartDMA();
			if(!draw)
				fillGillian(black);
			else
			{
				setPalette(&dissapear_pal, 0, 16*2);
				setPaletteColor(2, RGB5(0, 31, 0));
			}
			EndDMA();
			redraw = 0;
		}
		
		// read immediately so response is accurate
		WaitForVBlank();
		tc.frames++;
		
		pressed = PadPressed(0);
		
		if(pressed & KEY_START)
		{
			DrawHelp(HELP_DISAPPEAR);
			reload = 1;
		}
		
		if(pressed & KEY_B)
			end = 1;

		if(pressed & KEY_A)
		{
			draw = !draw;
			redraw = 1;
		}		
		
		if(frames)
		{
			frames--;
			if(!frames)
			{
				StartDMA();
				setPaletteColor(1, black);
				if(!draw)
					fillGillian(black);
				EndDMA();
			}
		}
		
		if(pressed & KEY_X)
		{
			if(!frames)
			{
				StartDMA();
				setPaletteColor(1, white);
				if(!draw)
					fillGillian(white);
				EndDMA();
				frames = 2;
			}
		}
	}
	transitionAndClear();
}

void DrawPhase() 
{	
	u16 pressed, end = 0;
	u16 redraw = 1, usecheck = 0;
	s16 x = 0;
		
	while(!end) 
	{		
		if(redraw)
		{
			u16 size = 0;
			
			StartDMA();
			
			if(usecheck)
			{
				bgInitTileSetMine(1, &check_tiles, &grid_pal, 0, (&check_tiles_end - &check_tiles), 16*2, BG_16COLORS, 0x6000);	
					
				bgInitMapSetMine(1, &fullscreen_map, (&fullscreen_map_end - &fullscreen_map), SC_32x32, 0x7000);
				
				setPaletteColor(0x00, RGB5(0, 0, 0));
				setPaletteColor(0x01, RGB5(0xff, 0xff, 0xff));
			}
			else
			{			
				bgInitTileSetMine(1, &phase_tiles, &phase_pal, 0, (&phase_tiles_end - &phase_tiles), 16*2, BG_16COLORS, 0x6000);	
				bgInitMapSetMine(1, &phase_map, (&phase_map_end - &phase_map), SC_32x32, 0x2000);
			}
			
			size = (&gillian_tiles_end-&gillian_tiles);
			oamInitGfxSet(&gillian_tiles, size, &gillian_pal, 16*2, 0, 0x4000, OBJ_SIZE16);

			setMode(BG_MODE1,0); 
			bgSetDisable(0);		
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);
			EndDMA();
			redraw = 0;
		}
		
		DrawTilesWithSpritesLoaded(18+x, 63, 64, 112, 0);
		DrawTilesWithSpritesLoaded(74+x, 63, 64, 112, 112);
		DrawTilesWithSpritesLoaded(130+x, 63, 64, 112, 224);
		DrawTilesWithSpritesLoaded(186+x, 63, 64, 112, 336);
		
		WaitForVBlank();
		check_blink();
		
		pressed = PadPressed(0);
		
		if(pressed & KEY_START)
		{
			DrawHelp(HELP_PHASE);
			redraw = 1;
		}
		
		if(pressed & KEY_A)
			x = 0;

		if(pressed & KEY_B)
			end = 1;		
		
		if(pressed & KEY_X)
		{
			usecheck = !usecheck;
			redraw = 1;
		}
			
		if(pressed & KEY_LEFT || pressed & KEY_L)
		{
			x -= 1;
			if(x < -18)
				x = -18;
		}
		
		if(pressed & KEY_RIGHT || pressed & KEY_R)
		{
			x += 1;
			if(x > 18)
				x = 18;
		}
	}
	transitionAndClear();
	return;
}