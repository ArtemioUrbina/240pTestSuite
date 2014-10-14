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
#include "video.h"

void TestPatterns();

void DrawCheck();

void DrawGrid256();
void DrawGrid320();
void DrawGrid512();

int Enabled240p = 0;

#define HPOS 5

void main()
{
    int OldButtonsInternal = 0;
    int controller;   
    int read; 
    int redraw = 1;
	int refresh = 1;
    int sel = 0;
	
	set_xres(320, XRES_SHARP);

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
			load_palette(0, MB_pal, 1);  
           
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
			if(Enabled240p)
				put_string("VDC 320x240", HPOS, row++);			
			else
				put_string("VDC 320x224", HPOS, row++);			
			
			set_font_pal(sel == 12 ? 15 : 14);
            put_string("Help", HPOS, ++row);
			set_font_pal(sel == 13 ? 15 : 14);
            put_string("Credits", HPOS, ++row);

            refresh = 0;
        }

        read = joy(0);
        controller =  read & ~OldButtonsInternal;
        OldButtonsInternal = read;
        
        if (controller & JOY_DOWN) 
        {
            sel++;
            if(sel > 13)
                sel = 0;
            refresh = 1;
        }

        if (controller & JOY_UP) 
        {
            sel--;
            if(sel < 0)
                sel = 13;
            refresh = 1;
        }
		
		if (controller & JOY_I)
		{
			switch(sel)
			{
				case 0:
					TestPatterns();
					break;
				case 11:
					if(Enabled240p)
						Set224p();
					else
						Set240p();
					break;
				case 8:
					DrawCheck();
					break;
			}
			redraw = 1;
			OldButtonsInternal = joy(0);
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
			load_palette(0, MB_pal, 1);  
         
			refresh = 1;
            redraw = 0;
        }
		
		if(refresh)
        {
            int row = 7;
            
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
			if(Enabled240p)
				put_string("Grid 256x240", HPOS, row++);
			else
				put_string("Grid 256x224", HPOS, row++);
			set_font_pal(sel == 6 ? 15 : 14);
			if(Enabled240p)
				put_string("Grid 320x240", HPOS, row++);
			else
				put_string("Grid 320x224", HPOS, row++);
			set_font_pal(sel == 7 ? 15 : 14);
			if(Enabled240p)
				put_string("Grid 512x240", HPOS, row++);
			else
				put_string("Grid 512x224", HPOS, row++);
            set_font_pal(sel == 8 ? 15 : 14);
            put_string("Linearity", HPOS, row++);
            set_font_pal(sel == 9 ? 15 : 14);
            put_string("Gray Ramp", HPOS, row++);
            set_font_pal(sel == 10 ? 15 : 14);
            put_string("White & RGB Screen", HPOS, row++);
            set_font_pal(sel == 11 ? 15 : 14);
            put_string("100 IRE", HPOS, row++);
            set_font_pal(sel == 12 ? 15 : 14);
            put_string("Sharpness", HPOS, row++);
            set_font_pal(sel == 13 ? 15 : 14);
            put_string("Overscan", HPOS, row++);
			
			set_font_pal(sel == 14 ? 15 : 14);
			if(Enabled240p)
				put_string("VDC 320x240", HPOS, row++);			
			else
				put_string("VDC 320x224", HPOS, row++);			
			
			set_font_pal(sel == 15 ? 15 : 14);
            put_string("Back to Main Menu", HPOS, ++row);

            refresh = 0;
        }

        read = joy(0);
        controller =  read & ~OldButtonsInternal;
        OldButtonsInternal = read;
        
        if (controller & JOY_DOWN) 
        {
            sel++;
            if(sel > 15)
                sel = 0;
            refresh = 1;
        }

        if (controller & JOY_UP) 
        {
            sel--;
            if(sel < 0)
                sel = 15;
            refresh = 1;
        }
		
				
		if (controller & JOY_II)
			end = 1;
		
		if (controller & JOY_I)
		{
			switch(sel)
			{
				case 5:
					DrawGrid256();
					break;
				case 6:
					DrawGrid320();
					break;
				case 7:
					DrawGrid512();
					break;
				case 14:
					if(Enabled240p)
						Set224p();
					else
						Set240p();
					break;
				case 15:
					end = 1;
					break;
			}
			OldButtonsInternal = joy(0);
			redraw = 1;			
		}

    }
}

/*
 *
 *		Test Functions
 *
 *
 */

void DrawCheck()
{
	int OldButtonsInternal = 0;
    int controller;   
    int read; 
    int redraw = 1;
	int end = 0;

    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			set_map_data(fs_map, 40, 30);
			set_tile_data(check_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, check_pal, 1);  
         
            redraw = 0;
        }

        read = joy(0);
        controller =  read & ~OldButtonsInternal;
        OldButtonsInternal = read;
        
		if (controller & JOY_II)
			end = 1;
    }
}


/*
 *
 *		Pattern Functions
 *
 *
 */

void DrawGrid256()
{
	int OldButtonsInternal = 0;
    int controller;   
    int read; 
    int redraw = 1;
	int end = 0;

	set_xres(256, XRES_SHARP);
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			set_map_data(grid_map, 40, 30);
			set_tile_data(grid_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, grid_pal, 1);  
         
            redraw = 0;
        }

        read = joy(0);
        controller =  read & ~OldButtonsInternal;
        OldButtonsInternal = read;
        
		if (controller & JOY_II)
			end = 1;
    }
	set_xres(320, XRES_SHARP);
}

void DrawGrid320()
{
	int OldButtonsInternal = 0;
    int controller;   
    int read; 
    int redraw = 1;
	int end = 0;

	set_xres(320, XRES_SHARP);
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			set_map_data(grid_map, 40, 30);
			set_tile_data(grid_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, grid_pal, 1);  
         
            redraw = 0;
        }

        read = joy(0);
        controller =  read & ~OldButtonsInternal;
        OldButtonsInternal = read;
        
		if (controller & JOY_II)
			end = 1;
    }
	set_xres(320, XRES_SHARP);
}

void DrawGrid512()
{
	int OldButtonsInternal = 0;
    int controller;   
    int read; 
    int redraw = 1;
	int end = 0;

	set_xres(512, XRES_SHARP);
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			set_map_data(grid_map, 40, 30);
			set_tile_data(grid_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, grid_pal, 1);  
         
            redraw = 0;
        }

        read = joy(0);
        controller =  read & ~OldButtonsInternal;
        OldButtonsInternal = read;
        
		if (controller & JOY_II)
			end = 1;
    }
	set_xres(320, XRES_SHARP);
}