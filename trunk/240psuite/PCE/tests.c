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
#include "graphics.h"
#include "video.h"

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

			Center224in240(); 
         
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
			
			Center224in240();
         
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
			
			Center224in240();
            redraw = 0;
			disp_on();
        }

        controller = joytrg(0);
        
		if (controller & JOY_II)
			end = 1;
    }
}

void SwapPalette(int pal, int index)
{
	int pos;
	int tmpcol1 = 0;
	int tmpcol2 = 0;
	int tmpcol3 = 0;
	int tmpcol4 = 0;
	
	pos = index+pal*16;
	tmpcol1 = get_color(pos);
	tmpcol2 = get_color(pos+1);
	tmpcol3 = get_color(pos+2);
	tmpcol4 = get_color(pos+3);
		
	set_color(pos, tmpcol4);
	set_color(pos+1, tmpcol1);
	set_color(pos+2, tmpcol2);
	set_color(pos+3, tmpcol3);		
}

void ScrollTest()
{
    int controller;   
    int read; 
    int redraw = 1;
	int end = 0;
	int x1 = 0;
	int x2 = 0;
	int x3 = 0;
	int x4 = 0;
	int pause = 0;
	int dir = 1;
	int spd = 1;
	int colswap = 0;

	set_screen_size(0);
	scroll(0, 0, 0, 0, 76, 0xC0);
	scroll(1, 0, 76, 76, 160, 0xC0);
	scroll(2, 0, 160, 160, 216, 0xC0);
	scroll(3, 0, 216, 216, 240, 0xC0);
	
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			load_background(sonic_bg, sonic_pal, sonic_map, 40, 30);
			
			Center224in240();
            redraw = 0;
			disp_on();
        }

        controller = joytrg(0);
		
		if (controller & JOY_UP)
			spd++;
		
		if (controller & JOY_DOWN)
			spd --;
			
		if(spd > 10)
			spd = 10;
			
		if(spd < 0)
			spd = 0;
        
		if (controller & JOY_II)
			end = 1;
			
		if (controller & JOY_I)
		{
			if(pause)
				pause = 0;
			else
				pause = 1;
		}
		
		if(colswap == 60)
		{
			SwapPalette(4, 2);
			SwapPalette(6, 3);
			
			colswap = 0;
		}
		colswap++;
		
		if (controller & JOY_SEL)
			dir *= -1;
		
		if(!pause)
		{
			x1 += 1*spd*dir;
			x2 += 2*spd*dir;
			x3 += 3*spd*dir;
			x4 += 4*spd*dir;
		}
		
		scroll(0, x1, 0, 0, 76, 0xC0);
		scroll(1, x2, 76, 76, 160, 0xC0);
		scroll(2, x3, 160, 160, 216, 0xC0);
		scroll(3, x4, 216, 216, 240, 0xC0);
    }
	scroll(0, 0, 0, 0, 76, 0xC0);
	scroll(1, 0, 76, 76, 160, 0xC0);
	scroll(2, 0, 160, 160, 216, 0xC0);
	scroll(3, 0, 216, 216, 240, 0xC0);
	set_screen_size(1);
}
