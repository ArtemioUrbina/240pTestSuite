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
 
#include "huc.h"
#include "patterns.h"

extern int Enabled240p;
extern int OldButtonsInternal;

extern char pluge_map[];
extern int pluge_bg[];
extern int pluge_pal[];

extern char color_map[];
extern int color_bg[];
extern int color_pal[];

extern char fs_map[];

extern int white_bg[];

extern int check_pal[];

extern int grid_bg[];
extern int grid_pal[];

extern char grid256_224_map[];
extern char grid256_240_map[];
extern char grid320_224_map[];
extern char grid320_240_map[];
extern char grid512_224_map[];
extern char grid512_240_map[];

void DrawPluge()
{
    int controller;   
    int read; 
    int redraw = 1;
	int end = 0;

    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			set_map_data(pluge_map, 40, 30);
			set_tile_data(pluge_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, pluge_pal, 1);  
         
            redraw = 0;
			disp_on();
        }

        read = joy(0);
        controller =  read & ~OldButtonsInternal;
        OldButtonsInternal = read;
        
		if (controller & JOY_II)
			end = 1;
    }
}

void DrawColor()
{
    int controller;   
    int read; 
    int redraw = 1;
	int end = 0;

    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			load_background(color_bg, color_pal, color_map, 40, 30);
            redraw = 0;
			disp_on();
        }

        read = joy(0);
        controller =  read & ~OldButtonsInternal;
        OldButtonsInternal = read;
        
		if (controller & JOY_II)
			end = 1;
    }
}

void DrawGrid256()
{
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
			if(Enabled240p)
				set_map_data(grid256_240_map, 32, 30);
			else
				set_map_data(grid256_224_map, 32, 28);
			set_tile_data(grid_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 32, Enabled240p ? 30 : 28);
			load_palette(0, grid_pal, 1);  
         
            redraw = 0;
			disp_on();
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
			if(Enabled240p)
				set_map_data(grid320_240_map, 40, 30);
			else
				set_map_data(grid320_224_map, 40, 28);
			set_tile_data(grid_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, Enabled240p ? 30 : 28);
			load_palette(0, grid_pal, 1);  
         
            redraw = 0;
			disp_on();
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
			if(Enabled240p)
				set_map_data(grid512_240_map, 64, 30);
			else
				set_map_data(grid512_224_map, 64, 28);
			set_tile_data(grid_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 64, Enabled240p ? 30 : 28);
			load_palette(0, grid_pal, 1);  
         
            redraw = 0;
			disp_on();
        }

        read = joy(0);
        controller =  read & ~OldButtonsInternal;
        OldButtonsInternal = read;
        
		if (controller & JOY_II)
			end = 1;
    }
	set_xres(320, XRES_SHARP);
}

void DrawWhite()
{
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
			set_tile_data(white_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, check_pal, 1);  
         
            redraw = 0;
			disp_on();
        }

        read = joy(0);
        controller =  read & ~OldButtonsInternal;
        OldButtonsInternal = read;
        
		if (controller & JOY_II)
			end = 1;
    }
}