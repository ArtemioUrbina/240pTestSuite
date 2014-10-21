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
#include "font.h"
#include "video.h"

#define HPOS 6

extern char MB512_map[];
extern int MB512_bg[];
extern int MB512_pal[];

void showHelp()
{
	int controller;   
    int read; 
    int redraw = 1;
	int refresh = 1;
	int end = 0;

    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			setupFont();
			set_xres(512, xres_flags);
			SetFontColors(15, RGB(2, 2, 2), RGB(0, 6, 0), RGB(0, 0, 0));
			
			set_map_data(MB512_map, 64, 30);
			set_tile_data(MB512_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 64, 30);
			load_palette(0, MB512_pal, 1);  
			
			Center224in240();
			
            redraw = 0;
			refresh = 1;
			disp_on();
        }
		
		if(refresh)
		{
			int row = 5;
			
			set_font_pal(15);
			put_string("HELP", HPOS+23, row++);
			set_font_pal(14);
			row++;
			put_string("The 240p Test Suite was designed with two goals in", HPOS, row++);
			put_string("mind:", HPOS, row++);
			row++;
			put_string("1) Evaluate 240p signal processing on modern TV", HPOS, row++);
			put_string("sets and video processing equipment; and", HPOS, row++);
			row++;
			put_string("2) Provide calibration patterns generated on your", HPOS, row++);
			put_string("game console to help in properly calibrating the", HPOS, row++);
			put_string("display's black, white and color levels.", HPOS, row++);
			row++;
			put_string("Help is available everywhere by pressing the Start", HPOS, row++);
			put_string("button. Use right and left in the d-pad to navigate", HPOS, row++);
			put_string("between help pages.", HPOS, row++);
			row++;
			put_string("This version of the suite is in 320x240p by", HPOS, row++);
			put_string("default; but can be changed to 320x224p. Grids are", HPOS, row++);
			put_string("also available in 256x224, 256x240, 512x224 and", HPOS, row++);
			put_string("512x240 resolutions.", HPOS, row++);

			refresh = 0;
		}

        controller = joytrg(0);
        
		if (controller & JOY_II)
			end = 1;
    }	
	set_xres(320, xres_flags);
}