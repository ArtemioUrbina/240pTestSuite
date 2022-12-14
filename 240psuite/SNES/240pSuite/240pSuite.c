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
 
 // Doesn't link porperly with pvsneslib-2.3.2
 // Due to error: 
 // COMPUTE_PENDING_CALCULATIONS: a computation is out of 8bit range.
 // Please use pvsneslib-2.2.0 (26/07/2014) for the time being

#include "font.h"
#include "tests.h"
#include "patterns.h"
#include "help.h"
#include "control.h"
#include "hardware.h"

void TestPatterns();
void VideoTests();
void AudioTests();
void HardwareTools();
void DrawCredits();
void DrawIntro();
void LoadAudio();

int main(void) 
{
	u16 redraw = 1, change = 0;
	u16 pressed;
	s16 sel = 0, start = 1;
	    
	setBrightness(0);
	consoleInit();
	
	LoadAudio();
	
	// Main loop
	DrawIntro();
	while(1) 
	{
		if(redraw)
		{
			u16 size = 0;
					
			StartDMA();	
			
			setPaletteColor(0x00, RGB5(0, 0, 0));
			consoleInitTextMine(0, 7, &font);
			size = (&back_tiles_end - &back_tiles);
			bgInitTileSetMine(1, &back_tiles, &back_pal, 1, size, 16*2, BG_16COLORS, 0x6000);			
			
			size = (&back_map_end - &back_map);	
			bgInitMapSetMine(1, &back_map, size, SC_32x32, 0x2000);
			
			size = (&gillian_tiles_end-&gillian_tiles);
			DrawTilesWithSprites(176, 72, 64, 112, &gillian_tiles, size, &gillian_pal);				
						
			setMode(BG_MODE1,0); 	
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);						

			change = 1;
		}			
		
		if(change)
		{
			u16 pos = 10;
			
			AddTextColor(7, RGB5(31, 31, 31), RGB5(0, 0, 0));
			AddTextColor(6, RGB5(31, 0, 0), RGB5(0, 0, 0));			
			AddTextColor(5, RGB5(0, 28, 28), RGB5(0, 0, 0));			
			
			drawText(5, pos, sel == 0 ? 6 : 7, "Test Patterns"); pos ++;
			drawText(5, pos, sel == 1 ? 6 : 7, "Video Tests"); pos ++;
			drawText(5, pos, sel == 2 ? 6 : 7, "Audio Tests"); pos ++;
			//drawText(5, pos, sel == 3 ? 6 : 7, "Hardware tools"); pos ++;
			drawText(5, pos, sel == 3 ? 6 : 7, "Controller Test"); pos ++;
			pos = 18;
			drawText(5, pos, sel == 4 ? 6 : 7, "Help"); pos ++;	
			drawText(5, pos, sel == 5 ? 6 : 7, "Video: %s", interlaced ? "256x480i" : "256x224p"); pos ++;	
			drawText(5, pos, sel == 6 ? 6 : 5, "Credits"); pos++;
			
			drawText(25, 26, 7, snes_50hz ? "PAL" : "NTSC"); 			
				
			if(redraw)
			{
				redraw = 0;
				EndDMA();
				if(start)
				{
#ifdef ENABLE_MOSAIC
					setMosaicEffect(MOSAIC_OUT, MOSAIC_BG1);
					WaitForVBlank();
#endif
					start = 0;
				}
			}
						
			change = 0;			
		}
		WaitForVBlank();
		check_blink();
		
		pressed = PadPressed(0);
					
		if(pressed & KEY_DOWN)
		{
			sel+=1;
			change = 1;
		}
		if(pressed & KEY_UP)
		{			
			sel-=1;
			change = 1;		
		}	

		if(sel < 0)
			sel = 6;
			
		if(sel > 6)
			sel = 0;
			
		if(pressed & KEY_START)
		{
			Transition();
			DrawHelp(HELP_GENERAL);
			redraw = 1;
		}
		
		if(pressed & KEY_A && sel == 5)
		{
			if(interlaced)
				ClearInterlaced();
			else
				SetInterlaced();
			redraw = 1;
		}
			
		if(pressed & KEY_A && sel != 5)
		{							
			Transition();
			oamClear(0, 0);
			
			switch(sel)
			{
				case 0:
					TestPatterns();
					break;
				case 1:
					VideoTests();
					break;
				case 2:
					AudioTests();
					break;
				case 3:
					//HardwareTools();
					ControllerTest();
					break;
				case 4:
					DrawHelp(HELP_GENERAL);
					break;
				case 6:
					DrawCredits();
					break;
				
			}
			redraw = 1;
		}
	}
	return 0;
}

void TestPatterns(void) 
{
	u16 redraw = 1, change = 0;
	u16 pressed;
	s16 sel = 0, exit = 0;	    	
		
	while(!exit) 
	{
		if(redraw)
		{
			u16 size = 0;
					
			StartDMA();	
			
			setPaletteColor(0x00, RGB5(0, 0, 0));
			consoleInitTextMine(0, 7, &font);				
			size = (&back_tiles_end - &back_tiles);
			bgInitTileSetMine(1, &back_tiles, &back_pal, 1, size, 16*2, BG_16COLORS, 0x6000);			
			
			size = (&back_map_end - &back_map);	
			bgInitMapSetMine(1, &back_map, size, SC_32x32, 0x2000);
			
			size = (&gillian_tiles_end-&gillian_tiles);
			DrawTilesWithSprites(180, 86, 64, 112, &gillian_tiles, size, &gillian_pal);				
						
			setMode(BG_MODE1,0); 	
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);						
									
			change = 1;
		}			
		
		if(change)
		{
			u16 pos = 6;
			
			AddTextColor(7, RGB5(31, 31, 31), RGB5(0, 0, 0));
			AddTextColor(6, RGB5(31, 0, 0), RGB5(0, 0, 0));	
			AddTextColor(5, RGB5(0, 28, 28), RGB5(0, 0, 0));
			
			drawText(3, pos, sel == 0 ? 6 : 7, "Pluge"); pos ++;
			drawText(3, pos, sel == 1 ? 6 : 7, "Color Bars"); pos ++;
			drawText(3, pos, sel == 2 ? 6 : 7, "EBU Color Bars"); pos ++;
			drawText(3, pos, sel == 3 ? 6 : 7, "SMPTE Color Bars"); pos ++;
			drawText(3, pos, sel == 4 ? 6 : 7, "Color Bars w/ Gray Ref"); pos ++;
			drawText(3, pos, sel == 5 ? 6 : 7, "Color Bleed Check"); pos ++;
			drawText(3, pos, sel == 6 ? 6 : 7, "Monoscope"); pos ++;
			drawText(3, pos, sel == 7 ? 6 : 7, "Grid"); pos ++;
			drawText(3, pos, sel == 8 ? 6 : 7, "Gray Ramp"); pos ++;
			drawText(3, pos, sel == 9 ? 6 : 7, "White & RGB Screen"); pos ++;
			drawText(3, pos, sel == 10 ? 6 : 7, "100 IRE"); pos ++;	
			drawText(3, pos, sel == 11 ? 6 : 7, "Sharpness"); pos++;
			drawText(3, pos, sel == 12 ? 6 : 7, "Overscan"); pos++;
			drawText(3, pos, sel == 13 ? 6 : 7, "Convergence"); pos++;
			drawText(3, pos, sel == 14 ? 6 : 7, "Mode 7"); pos+=2;
			drawText(3, pos, sel == 15 ? 6 : 5, "Back to Main Menu"); 
			
			drawText(25, 26, 7, snes_50hz ? "PAL" : "NTSC"); 
			
			if(redraw)
			{
				redraw = 0;
				EndDMA();
			}
						
			change = 0;			
		}
		WaitForVBlank();
		check_blink();
		
		pressed = PadPressed(0);
					
		if(pressed & KEY_DOWN)
		{
			sel+=1;
			change = 1;
		}
		if(pressed & KEY_UP)
		{			
			sel-=1;
			change = 1;		
		}	

		if(sel < 0)
			sel = 15;
			
		if(sel > 15)
			sel = 0;
			
		if(pressed & KEY_START)
		{
			Transition();
			DrawHelp(HELP_GENERAL);
			redraw = 1;
		}
			
		if(pressed & KEY_A)
		{							
			Transition();
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
					DrawEBU();
					break;
				case 3:
					DrawSMPTE();
					break;
				case 4:
					Draw601CB();
					break;
				case 5:
					DrawColorBleed();
					break;
				case 6:
					DrawMonoscope();
					break;
				case 7:
					DrawGrid();
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
				case 12:
					DrawOverscan();
					break;
				case 13:
					DrawConvergence();
					break;
				case 14:
					DrawMode7();
					break;
				case 15:
					exit = 1;
					break;
			}
			redraw = 1;
		}
		
		if(pressed & KEY_B)
		{
			exit = 1;
			Transition();
			oamClear(0, 0);
		}
	}
	return 0;
}

#define VT_START 3
void VideoTests(void) 
{
	u16 redraw = 1, change = 0;
	u16 pressed;
	s16 sel = 0, exit = 0;	    	
		
	while(!exit) 
	{
		if(redraw)
		{
			u16 size = 0;
					
			StartDMA();	
			
			setPaletteColor(0x00, RGB5(0, 0, 0));
			consoleInitTextMine(0, 7, &font);				
			size = (&back_tiles_end - &back_tiles);
			bgInitTileSetMine(1, &back_tiles, &back_pal, 1, size, 16*2, BG_16COLORS, 0x6000);			
			
			size = (&back_map_end - &back_map);	
			bgInitMapSetMine(1, &back_map, size, SC_32x32, 0x2000);
			
			size = (&gillian_tiles_end-&gillian_tiles);
			DrawTilesWithSprites(176, 80, 64, 112, &gillian_tiles, size, &gillian_pal);				
						
			setMode(BG_MODE1,0); 	
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);						
									
			change = 1;
		}			
		
		if(change)
		{
			u16 pos = 7;
			
			AddTextColor(7, RGB5(31, 31, 31), RGB5(0, 0, 0));
			AddTextColor(6, RGB5(31, 0, 0), RGB5(0, 0, 0));	
			AddTextColor(5, RGB5(0, 28, 28), RGB5(0, 0, 0));
			
			drawText(VT_START, pos, sel == 0 ? 6 : 7, "Drop Shadow Test"); pos ++;
			drawText(VT_START, pos, sel == 1 ? 6 : 7, "Stripped Sprite Test"); pos ++;
			drawText(VT_START, pos, sel == 2 ? 6 : 7, "Lag Test"); pos ++;
			drawText(VT_START, pos, sel == 3 ? 6 : 7, "Timing & Reflex Test"); pos ++;
			drawText(VT_START, pos, sel == 4 ? 6 : 7, "Scroll Test"); pos ++;
			drawText(VT_START, pos, sel == 5 ? 6 : 7, "Grid Scroll Test"); pos ++;
			drawText(VT_START, pos, sel == 6 ? 6 : 7, "Horiz/Vert Stripes"); pos ++;
			drawText(VT_START, pos, sel == 7 ? 6 : 7, "Checkerboard"); pos ++;
			drawText(VT_START, pos, sel == 8 ? 6 : 7, "Backlit Zone Test"); pos ++;
			drawText(VT_START, pos, sel == 9 ? 6 : 7, "Alternate 240p/480i"); pos ++;
			drawText(VT_START, pos, sel == 10 ? 6 : 7, "Disappearing Logo"); pos ++;
			pos = 19;
			drawText(VT_START, pos, sel == 11 ? 6 : 7, "Help"); pos ++;	
			drawText(VT_START, pos, sel == 12 ? 6 : 7, "Video: %s", interlaced ? "256x480i" : "256x224p"); pos ++;	
			drawText(VT_START, pos, sel == 13 ? 6 : 5, "Back to Main Menu"); pos++;
			
			drawText(25, 26, 7, snes_50hz ? "PAL" : "NTSC"); 
			
			if(redraw)
			{
				redraw = 0;
				EndDMA();
			}
						
			change = 0;			
		}
		WaitForVBlank();
		check_blink();
		
		pressed = PadPressed(0);
					
		if(pressed & KEY_DOWN)
		{
			sel+=1;
			change = 1;
		}
		if(pressed & KEY_UP)
		{			
			sel-=1;
			change = 1;		
		}	

		if(sel < 0)
			sel = 13;
			
		if(sel > 13)
			sel = 0;
			
		if(pressed & KEY_START)
		{
			Transition();
			DrawHelp(HELP_GENERAL);
			redraw = 1;
		}
			
		if(pressed & KEY_A)
		{							
			Transition();
			oamClear(0, 0);
			
			switch(sel)
			{
				case 0:
					DropShadowTest();
					break;
				case 1:
					StripedSpriteTest();
					break;
				case 2:
					PassiveLagTest();
					break;
				case 3:
					ManualLagTest();
					break;
				case 4:
					HScrollTest();
					break;
				case 5:
					VScrollTest();
					break;
				case 6:
					DrawStripes();
					break;
				case 7:
					DrawCheck();
					break;
				case 8:
					LEDZoneTest();
					break;
				case 9:
					Alternate240p480i();
					break;
				case 10:
					DisappearingLogo();
					break;
				case 11:
					DrawHelp(HELP_GENERAL);
					break;
				case 12:
					if(interlaced)
						ClearInterlaced();
					else
						SetInterlaced();
					break;
				case 13:
					exit = 1;
					Transition();
					oamClear(0, 0);
					break;
			}
			redraw = 1;
		}
		
		if(pressed & KEY_B)
		{
			exit = 1;
			Transition();
			oamClear(0, 0);
		}
	}
	return 0;
}

void AudioTests(void) 
{
	u16 redraw = 1, change = 0;
	u16 pressed;
	s16 sel = 0, exit = 0;	    	
		
	while(!exit) 
	{
		if(redraw)
		{
			u16 size = 0;
					
			StartDMA();	
			
			setPaletteColor(0x00, RGB5(0, 0, 0));
			consoleInitTextMine(0, 7, &font);				
			size = (&back_tiles_end - &back_tiles);
			bgInitTileSetMine(1, &back_tiles, &back_pal, 1, size, 16*2, BG_16COLORS, 0x6000);			
			
			size = (&back_map_end - &back_map);	
			bgInitMapSetMine(1, &back_map, size, SC_32x32, 0x2000);
			
			size = (&gillian_tiles_end-&gillian_tiles);
			DrawTilesWithSprites(176, 72, 64, 112, &gillian_tiles, size, &gillian_pal);				
						
			setMode(BG_MODE1,0); 	
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);						
									
			change = 1;
		}			
		
		if(change)
		{
			u16 pos = 10;
			
			AddTextColor(7, RGB5(31, 31, 31), RGB5(0, 0, 0));
			AddTextColor(6, RGB5(31, 0, 0), RGB5(0, 0, 0));	
			AddTextColor(5, RGB5(0, 28, 28), RGB5(0, 0, 0));
			
			drawText(4, pos, sel == 0 ? 6 : 7, "Sound Test"); pos ++;
			drawText(4, pos, sel == 1 ? 6 : 7, "Audio Sync Test"); pos ++;
			drawText(4, pos, sel == 2 ? 6 : 7, "MDFourier"); pos ++;
			pos = 18;
			drawText(4, pos, sel == 3 ? 6 : 7, "Help"); pos ++;	
			drawText(4, pos, sel == 4 ? 6 : 7, "Video: %s", interlaced ? "256x480i" : "256x224p"); pos ++;	
			drawText(4, pos, sel == 5 ? 6 : 5, "Back to Main Menu"); pos++;
			
			drawText(25, 26, 7, snes_50hz ? "PAL" : "NTSC"); 
			
			if(redraw)
			{
				redraw = 0;
				EndDMA();
			}
						
			change = 0;			
		}
		WaitForVBlank();
		check_blink();
		
		pressed = PadPressed(0);
					
		if(pressed & KEY_DOWN)
		{
			sel+=1;
			change = 1;
		}
		if(pressed & KEY_UP)
		{			
			sel-=1;
			change = 1;		
		}	

		if(sel < 0)
			sel = 4;
			
		if(sel > 4)
			sel = 0;
			
		if(pressed & KEY_START)
		{
			Transition();
			DrawHelp(HELP_GENERAL);
			redraw = 1;
		}
			
		if(pressed & KEY_A)
		{							
			Transition();
			oamClear(0, 0);
			
			switch(sel)
			{
				case 0:
					SoundTest();
					break;
				case 1:
					AudioSyncTest();
					break;
				case 2:
					MDFourier(0);
					break;
				case 3:
					DrawHelp(HELP_GENERAL);
					break;
				case 4:
					if(interlaced)
						ClearInterlaced();
					else
						SetInterlaced();
					break;
				case 5:
					exit = 1;
					Transition();
					oamClear(0, 0);
					break;
			}
			redraw = 1;
		}
		
		if(pressed & KEY_B)
		{
			exit = 1;
			Transition();
			oamClear(0, 0);
		}
	}
	return 0;
}

void HardwareTools(void) 
{
	u16 redraw = 1, change = 0;
	u16 pressed;
	s16 sel = 0, exit = 0;	    	
		
	while(!exit) 
	{
		if(redraw)
		{
			u16 size = 0;
					
			StartDMA();	
			
			setPaletteColor(0x00, RGB5(0, 0, 0));
			consoleInitTextMine(0, 7, &font);				
			size = (&back_tiles_end - &back_tiles);
			bgInitTileSetMine(1, &back_tiles, &back_pal, 1, size, 16*2, BG_16COLORS, 0x6000);			
			
			size = (&back_map_end - &back_map);	
			bgInitMapSetMine(1, &back_map, size, SC_32x32, 0x2000);
			
			size = (&gillian_tiles_end-&gillian_tiles);
			DrawTilesWithSprites(176, 72, 64, 112, &gillian_tiles, size, &gillian_pal);				
						
			setMode(BG_MODE1,0); 	
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);						
									
			change = 1;
		}			
		
		if(change)
		{
			u16 pos = 10;
			
			AddTextColor(7, RGB5(31, 31, 31), RGB5(0, 0, 0));
			AddTextColor(6, RGB5(31, 0, 0), RGB5(0, 0, 0));	
			AddTextColor(5, RGB5(0, 28, 28), RGB5(0, 0, 0));
			
			drawText(4, pos, sel == 0 ? 6 : 7, "Controller Test"); pos ++;
			drawText(4, pos, sel == 1 ? 6 : 7, "Memory Viewer"); pos ++;
			pos = 18;
			drawText(4, pos, sel == 2 ? 6 : 7, "Help"); pos ++;	
			drawText(4, pos, sel == 3 ? 6 : 7, "Video: %s", interlaced ? "256x480i" : "256x224p"); pos ++;	
			drawText(4, pos, sel == 4 ? 6 : 5, "Back to Main Menu"); pos++;
			
			drawText(25, 26, 7, snes_50hz ? "PAL" : "NTSC"); 
			
			if(redraw)
			{
				redraw = 0;
				EndDMA();
			}
						
			change = 0;			
		}
		WaitForVBlank();
		check_blink();
		
		pressed = PadPressed(0);
					
		if(pressed & KEY_DOWN)
		{
			sel+=1;
			change = 1;
		}
		if(pressed & KEY_UP)
		{			
			sel-=1;
			change = 1;		
		}	

		if(sel < 0)
			sel = 4;
			
		if(sel > 4)
			sel = 0;
			
		if(pressed & KEY_START)
		{
			Transition();
			DrawHelp(HELP_GENERAL);
			redraw = 1;
		}
			
		if(pressed & KEY_A)
		{							
			Transition();
			oamClear(0, 0);
			
			switch(sel)
			{
				case 0:
					ControllerTest();
					break;
				case 1:
					MemoryViewer();
					break;
				case 2:
					DrawHelp(HELP_GENERAL);
					break;
				case 3:
					if(interlaced)
						ClearInterlaced();
					else
						SetInterlaced();
					break;
				case 4:
					exit = 1;
					Transition();
					oamClear(0, 0);
					break;
			}
			redraw = 1;
		}
		
		if(pressed & KEY_B)
		{
			exit = 1;
			Transition();
			oamClear(0, 0);
		}
	}
	return 0;
}


void DrawCat(void) 
{	
	u16 held, pressed, end = 0;	 
	u16 redraw = 1, size = 0, count = 0;	
	
#ifdef ENABLE_MOSAIC
	setMosaicEffect(MOSAIC_IN, MOSAIC_BG1);
#endif
	while(!end) 
	{		
		if(redraw)
		{
			StartDMA();
			
			consoleInitTextMine(1, 4, &font);			
			size = (&nishka_tiles_end - &nishka_tiles);
			bgInitTileSetMine(0, &nishka_tiles, &nishka_pal, 0, size, 256*2, BG_256COLORS, 0x2000);		
	
			bgInitMapSetMine(0, &nishka_map, (&nishka_map_end - &nishka_map), SC_32x32, 0x7000);
						
			setMode(BG_MODE3,0); 								
			
			drawText(5, 25, 4, "The cat was unimpressed");
						
			EndDMA();
			redraw = 0;
#ifdef ENABLE_MOSAIC
			setMosaicEffect(MOSAIC_OUT, MOSAIC_BG1);
#endif
		}
		WaitForVBlank();
		
		pressed = PadPressed(0);
		held = PadHeld(0);
		
		if(pressed & KEY_B)
			end = 1;
		
		if(held & KEY_L)
			count = 0;

		count++;
		if(count == 20)
			end = 1;
	}	
	
	return;
}

void DrawIntro(void) 
{	
	u8 i = 0;
	
	StartDMA();	
	ClearScreen(0);
	EndDMA();
	
	StartDMA();
	consoleInitTextMine(0, 7, &font);
	AddTextColor(7, RGB5(31, 31, 31), RGB5(0, 0, 0));
	
	drawText(8, 12, 7, "KORDAMP PRESENTS");
	setMode(BG_MODE1,0); 	
	bgSetDisable(1);
	bgSetDisable(2);
	bgSetScroll(1, 0, -1);
	EndDMA();
	
	while(i++ < 20)
		WaitForVBlank();

#ifdef ENABLE_MOSAIC		
	setMosaicEffect(MOSAIC_IN, MOSAIC_BG1);
	WaitForVBlank();
#endif
}

void DrawCredits(void) 
{
	u16 redraw = 1, counter = 1;
	u16 pressed;
	s16 exit = 0;
	 
	while(!exit) 
	{
		if(redraw)
		{
			u16 size = 0;
			u16 pos = 7;
			
			StartDMA();
			
			consoleInitTextMine(0, 7, &font);
			size = (&back_tiles_end - &back_tiles);
			bgInitTileSetMine(1, &back_tiles, &back_pal, 1, size, 16*2, BG_16COLORS, 0x6000);			
			
			size = (&back_map_end - &back_map);	
			bgInitMapSetMine(1, &back_map, size, SC_32x32, 0x2000);

			size = (&barcode_tiles_end-&barcode_tiles);
			DrawTilesWithSpritesBarcode(205, 72, 32, 32, &barcode_tiles, size, &barcode_pal);									
						
			setMode(BG_MODE1,0); 	
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);						
			
			AddTextColor(7, RGB5(31, 31, 31), RGB5(0, 0, 0));
			AddTextColor(6, RGB5(0, 31, 0), RGB5(0, 0, 0));	
			AddTextColor(5, RGB5(0, 25, 25), RGB5(0, 0, 0));	
			
			drawText(3, pos, 6, "Code, patterns & contact:"); pos ++;
			pos++;
			drawText(3, pos, 6, "Menu Pixel Art:"); pos ++;
			drawText(4, pos, 7, "Asher"); pos ++;
			drawText(3, pos, 6, "SDK:"); pos ++;
			drawText(4, pos, 7, "PVSnesLib by Alekmaul"); pos ++;
			drawText(3, pos, 6, "Monoscope Pattern:"); pos ++;
			drawText(4, pos, 7, "Keith Raney"); pos ++;
			drawText(3, pos, 6, "Donna Art:"); pos ++;
			drawText(4, pos, 7, "Jose Salot (@pepe_salot)"); pos ++;
			drawText(3, pos, 6, "Advisor:"); pos ++;
			drawText(4, pos, 7, "Fudoh"); pos ++;
			drawText(3, pos, 6, "Collaboration:"); pos ++;
			drawText(4, pos, 7, "shmups regulars"); pos ++;
			drawText(3, pos, 6, "Info on using this suite:"); pos ++;
			drawText(4, pos, 7, "http://junkerhq.net/240p"); pos ++;
			
			drawText(4, 4, 5, "Ver. 1.07");
			drawText(19, 4, 7, "13/12/2022");
			drawText(10, 24, 5, "Dedicated to Elisa");
			
			EndDMA();	
			if(redraw)
				redraw = 0;
		}
				
		if(counter == 1)
			drawText(4, 8, 7, "Artemio Urbina      ");
		if(counter == 60*4)
			drawText(4, 8, 7, "@Artemio (twitter)  ");  
		if(counter == 60*8)
			counter = 0;
	
		WaitForVBlank();
		counter ++;
		
		pressed = PadPressed(0);
		
		if(pressed & KEY_B)
			exit = 1;
		
		if(pressed & KEY_L)
		{			
			Transition();
			DrawCat();
			redraw = 1;
			counter = 1;
		}
	}
	oamInit();
	return 0;
}

// https://forums.nesdev.com/viewtopic.php?f=12&t=7975
// can only do 32k blocks

// We only need a few bytes, not the 64kb
// https://en.wikibooks.org/wiki/Super_NES_Programming/Loading_SPC700_programs

void LoadAudio()
{
	int i = 0;

	spcBoot();	
	spcSetBank(&__SOUNDBANK__);
	
	
	// allocate around 10K of sound ram (40 256-byte blocks)
	spcAllocateSoundRegion(40);
	spcLoad(0);
	for(i = 0; i < 12; i++)
		spcLoadEffect(i);
}
