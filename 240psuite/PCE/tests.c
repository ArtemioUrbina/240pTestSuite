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

extern char fs_map[];

extern int hstripes_bg[];
extern int vstripes_bg[];

extern int check_bg[];
extern int check_pal[];

extern char motoko_map[];
extern int motoko_bg[];
extern int motoko_pal[];

void DrawCheck()
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
			set_tile_data(check_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, check_pal, 1);  
         
            redraw = 0;
			disp_on();
        }

        controller = joytrg(0);
        
		if (controller & JOY_II)
			end = 1;
    }
}


void DrawStripes()
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
			set_tile_data(hstripes_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, check_pal, 1);  
         
            redraw = 0;
			disp_on();
        }

        controller = joytrg(0);
        
		if (controller & JOY_II)
			end = 1;
		if (controller & JOY_DOWN)
		{
			set_tile_data(vstripes_bg);
			load_tile(0x1000);
		}
		if (controller & JOY_UP)
		{
			set_tile_data(hstripes_bg);
			load_tile(0x1000);
		}
    }
}

void DropShadow()
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
			load_background(motoko_bg, motoko_pal, motoko_map, 40, 30);
            redraw = 0;
			disp_on();
        }

        controller = joytrg(0);
        
		if (controller & JOY_II)
			end = 1;
    }
}
