/* 
 * 240p Test Suite for Nintendo 64
 * Copyright (C)2016 Artemio Urbina
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	02111-1307	USA
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>
#include "image.h"
#include "utils.h"
#include "tests.h"
#include "patterns.h"

void DrawPatternsMenu();

int main(void)
{
	int sel = 1, reload = 0;
	sprite_t *back = NULL, *sd = NULL;
	struct controller_data keys;
	char str[20];

	init_n64();
	
	ClearScreen();
	
	back = LoadImage("/back.bin");
	sd = LoadImage("/sd.bin");
    while(1) 
    {
		int x = 38, y = 62, r = 0xff, g = 0xff, b = 0xff, c = 1;
		
		if(reload)
		{
			if(!back)
				back = LoadImage("/back.bin");
			if(!sd)
				sd = LoadImage("/sd.bin");
				
			reload = 0;
		}
		
		GetDisplay();
		
		drawImageDMA(0, 0, back);
		SoftDrawImage(221, 86, sd);		
		
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Test Patterns >"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Drop Shadow Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Striped Sprite Test"); y += fh; c++;    
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Lag Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Manual Lag Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Scroll Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Grid Scroll Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Horizontal Stripes"); y += fh; c++;    
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Checkerboard"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Backlit Zone Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Diagonal Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Alternating 240p/480i Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Sound Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Options"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Help"); 
		
		if(isNTSC())
			DrawStringS(254, 216, 0xfa, 0xfa, 0xfa, "N64 NTSC");
		else
			DrawStringS(254, 216, 0xfa, 0xfa, 0xfa, "N64 PAL"); 
		
		sprintf(str, "RAM %d MB", DetectRamSize());
		DrawStringS(254, 224, 0xfa, 0xfa, 0xfa, str);

        WaitVsync();

        controller_scan();
        keys = get_keys_down();

		if(keys.c[0].up)
			sel--;

		if(keys.c[0].down)
			sel++;
		
		if(sel > c)
			sel = 1;
		if(sel < 1)
			sel = c;
			
		if(keys.c[0].A)
		{	
			FreeImage(&back);
			FreeImage(&sd);
			
			ClearScreen();
			
			switch(sel)
			{
				case 1:
					DrawPatternsMenu();
					break;
				case 2:
					DropShadowTest();
					break;
				case 9:
					DrawCheckerboard();
					break;
				case 14:
					current_resolution++;
					if(current_resolution > RESOLUTION_512x480)
						current_resolution = RESOLUTION_320x240;
					set_video();
					break;
				default:
					break;
			}
			
			ClearScreen();
			reload = 1;
		}
	}
	
	FreeImage(&back);
	FreeImage(&sd);
}

void DrawPatternsMenu()
{
	int sel = 1, reload = 0, end = 0;
	sprite_t *back = NULL, *sd = NULL;
	struct controller_data keys;
	char str[20];

	back = LoadImage("/back.bin");
	sd = LoadImage("/sd.bin");
    while(!end) 
    {
		int x = 38, y = 62, r = 0xff, g = 0xff, b = 0xff, c = 1;
		
		if(reload)
		{
			if(!back)
				back = LoadImage("/back.bin");
			if(!sd)
				sd = LoadImage("/sd.bin");
				
			reload = 0;
		}
		
		GetDisplay();
		drawImageDMA(0, 0, back);
		SoftDrawImage(221, 86, sd);
		
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Pluge"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Color Bars"); y += fh; c++;
		if(!isNTSC())
		{
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "EBU Color Bars"); y += fh; c++;
		}
		else
		{
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "SMPTE Color Bars"); y += fh; c++;
		}
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Color Bars with Gray Scale"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Color Bleed Check"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Grid"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Grid 224p"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Linearity"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Linearity 224p"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Gray Ramp"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "White & RGB screens"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "100 IRE"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Sharpness"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Overscan"); y += fh; c++;
		DrawStringS(x, y + fh, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); y += fh; 
		
		if(isNTSC())
			DrawStringS(254, 216, 0xfa, 0xfa, 0xfa, "N64 NTSC");
		else
			DrawStringS(254, 216, 0xfa, 0xfa, 0xfa, "N64 PAL"); 
		
		sprintf(str, "RAM %d MB", DetectRamSize());
		DrawStringS(254, 224, 0xfa, 0xfa, 0xfa, str);

        WaitVsync();

        controller_scan();
        keys = get_keys_down();

		if(keys.c[0].up)
			sel--;

		if(keys.c[0].down)
			sel++;
		
		if(sel > c)
			sel = 1;
		if(sel < 1)
			sel = c;
			
		if(keys.c[0].A)
		{	
			FreeImage(&back);
			FreeImage(&sd);
			
			ClearScreen();
			
			switch(sel)
			{
				case 1:
					DrawPLUGE();
					break;
				case 2:
					DrawColorBars();
					break;
				case 6:
					DrawGrid();
					break;
				case 15:
					end = 1;
					break;
				default:
					break;
			}
			
			ClearScreen();
			reload = 1;
		}
		
		if(keys.c[0].B)
			end = 1;
	}
	
	FreeImage(&back);
	FreeImage(&sd);
}

