/* 
 * 240p Test Suite
 * Copyright (C)2014 Artemio Urbina (PC Engine/TurboGrafx-16)
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
 
 
 This version of the suite is compiled with HuC from https://github.com/uli/huc
 
 */
 
#include <huc.h>
#include "res.h"
#include "font.h"

void TestPatterns();

#define HPOS 5

void main()
{
    int OldButtonsInternal = 0;
    int controller;   
    int read; 
    int redraw = 1;
	int refresh = 1;
    int sel = 0;

    //vreg(0xB, 0x0428);	
	//vreg(0xD, 0x00EF);
	//vreg(0xE, 0x0003);
	set_xres(320);

    while(1)
    {   
		vsync();
		
        if(redraw)
        {
			init_satb();
			satb_update();
			
			setupFont();

			set_map_data(MB_map, 40, 30);
			set_tile_data(MB_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0,MB_pal,1);  
           
            redraw = 0;
			refresh = 1;
        }
		
		if(refresh)
        {
            int row = 8;

            set_font_pal(sel == 0 ? 15 : 14);
            put_string("Test Patterns", HPOS, row++);
            set_font_pal(sel == 1 ? 15 : 14);
            put_string("Drop Shadow Test", HPOS, row++);
            set_font_pal(sel == 2 ? 15 : 14);
            put_string("Striped Sprite Test", HPOS, row++);
            set_font_pal(sel == 3 ? 15 : 14);
            put_string("Lag Test", HPOS, row++);
            set_font_pal(sel == 4 ? 15 : 14);
            put_string("Manual Lag Test", HPOS, row++);
            set_font_pal(sel == 5 ? 15 : 14);
            put_string("Scroll Test", HPOS, row++);
            set_font_pal(sel == 6 ? 15 : 14);
            put_string("Grid Scroll Test", HPOS, row++);
            set_font_pal(sel == 7 ? 15 : 14);
            put_string("Horizontal Stripes", HPOS, row++);
            set_font_pal(sel == 8 ? 15 : 14);
            put_string("Checkerboard", HPOS, row++);
            set_font_pal(sel == 9 ? 15 : 14);
            put_string("Backlit Zone Test", HPOS, row++);
            set_font_pal(sel == 10 ? 15 : 14);
            put_string("Sound Test", HPOS, row++);
            set_font_pal(sel == 11 ? 15 : 14);
            put_string("Help", HPOS, ++row);
			set_font_pal(sel == 12 ? 15 : 14);
            put_string("Credits", HPOS, ++row);

            refresh = 0;
        }

        read = joy(0);
        controller =  read & ~OldButtonsInternal;
        OldButtonsInternal = read;
        
        if (controller & JOY_DOWN) 
        {
            sel++;
            if(sel > 12)
                sel = 0;
            refresh = 1;
        }

        if (controller & JOY_UP) 
        {
            sel--;
            if(sel < 0)
                sel = 12;
            refresh = 1;
        }
		
		if (controller & JOY_I)
		{
			switch(sel)
			{
				case 0:
					TestPatterns();
					break;
			}
			redraw = 1;
		}
    }
}

/*
 *
 *		Menu Functions
 *
 *
 */

void TestPatterns()
{
	int OldButtonsInternal = 0;
    int controller;   
    int read; 
    int redraw = 1;
	int refresh = 1;
    int sel = 0;
	int end = 0;

    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			init_satb();
			satb_update();
			
			setupFont();
			
			set_map_data(MB_map, 40, 30);
			set_tile_data(MB_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0,MB_pal,1);  
         
			refresh = 1;
            redraw = 0;
        }
		
		if(refresh)
        {
            int row = 8;
            
            set_font_pal(sel == 0 ? 15 : 14);
            put_string("Pluge", HPOS, row++);
            set_font_pal(sel == 1 ? 15 : 14);
            put_string("Color Bars", HPOS, row++);
            set_font_pal(sel == 2 ? 15 : 14);
            put_string("SMPTE Color Bars", HPOS, row++);
            set_font_pal(sel == 3 ? 15 : 14);
            put_string("Color Bars w/ Gray Ref", HPOS, row++);
            set_font_pal(sel == 4 ? 15 : 14);
            put_string("Color Bleed Check", HPOS, row++);
            set_font_pal(sel == 5 ? 15 : 14);
            put_string("Grid 256x224", HPOS, row++);
            set_font_pal(sel == 6 ? 15 : 14);
            put_string("Linearity", HPOS, row++);
            set_font_pal(sel == 7 ? 15 : 14);
            put_string("Gray Ramp", HPOS, row++);
            set_font_pal(sel == 8 ? 15 : 14);
            put_string("White & RGB Screen", HPOS, row++);
            set_font_pal(sel == 9 ? 15 : 14);
            put_string("100 IRE", HPOS, row++);
            set_font_pal(sel == 10 ? 15 : 14);
            put_string("Sharpness", HPOS, row++);
            set_font_pal(sel == 11 ? 15 : 14);
            put_string("Overscan", HPOS, row++);
			set_font_pal(sel == 12 ? 15 : 14);
            put_string("Back to Main Menu", HPOS, ++row);

            refresh = 0;
        }

        read = joy(0);
        controller =  read & ~OldButtonsInternal;
        OldButtonsInternal = read;
        
        if (controller & JOY_DOWN) 
        {
            sel++;
            if(sel > 12)
                sel = 0;
            refresh = 1;
        }

        if (controller & JOY_UP) 
        {
            sel--;
            if(sel < 0)
                sel = 12;
            refresh = 1;
        }
		
		if (controller & JOY_I)
		{
			switch(sel)
			{
				case 12:
					end = 1;
					break;
			}
			redraw = 1;
		}
		
		if (controller & JOY_II)
			end = 1;
    }
}
