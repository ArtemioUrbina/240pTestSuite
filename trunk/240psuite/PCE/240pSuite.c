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
#include "res.h"

void setupFont();
void TestPatterns();

void main()
{
    int OldButtonsInternal = 0;
    int controller;   
    int read; 
    int redraw = 1;
    int sel = 0;

    /* 0428 to HDR */
    //vreg(11, 0x0428);

    while(1)
    {   
        if(redraw)
        {
            int row = 7;

			init_satb();
			satb_update();

			load_background(MB_bg, MB_pal, MB_bat, 32, 28);    
			setupFont();
            
            set_font_pal(sel == 0 ? 15 : 14);
            put_string("Test Patterns", 3, row++);
            set_font_pal(sel == 1 ? 15 : 14);
            put_string("Drop Shadow Test", 3, row++);
            set_font_pal(sel == 2 ? 15 : 14);
            put_string("Striped Sprite Test", 3, row++);
            set_font_pal(sel == 3 ? 15 : 14);
            put_string("Lag Test", 3, row++);
            set_font_pal(sel == 4 ? 15 : 14);
            put_string("Manual Lag Test", 3, row++);
            set_font_pal(sel == 5 ? 15 : 14);
            put_string("Scroll Test", 3, row++);
            set_font_pal(sel == 6 ? 15 : 14);
            put_string("Grid Scroll Test", 3, row++);
            set_font_pal(sel == 7 ? 15 : 14);
            put_string("Horizontal Stripes", 3, row++);
            set_font_pal(sel == 8 ? 15 : 14);
            put_string("Checkerboard", 3, row++);
            set_font_pal(sel == 9 ? 15 : 14);
            put_string("Backlit Zone Test", 3, row++);
            set_font_pal(sel == 10 ? 15 : 14);
            put_string("Sound Test", 3, row++);
            set_font_pal(sel == 11 ? 15 : 14);
            put_string("Help", 3, ++row);

            redraw = 0;
        }

        read = joy(0);
        controller =  read & ~OldButtonsInternal;
        OldButtonsInternal = read;
        
        if (controller & JOY_DOWN) 
        {
            sel++;
            if(sel > 11)
                sel = 0;
            redraw = 1;
        }

        if (controller & JOY_UP) 
        {
            sel--;
            if(sel < 0)
                sel = 11;
            redraw = 1;
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

        vsync();
    }
}

/*
 *
 *		Helper Functions
 *
 *
 */

void setupFont()
{	
    load_font(my_font, 96);
    set_font_pal(14);        
    set_color_rgb(225, 255, 255, 255);
    set_color_rgb(226, 0, 0, 0);
    set_color_rgb(241, 255, 0, 0);
    set_color_rgb(242, 0, 0, 0);
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
    int sel = 0;
	int end = 0;

    while(!end)
    {   
        if(redraw)
        {
            int row = 7;

			init_satb();
			satb_update();

			load_background(MB_bg, MB_pal, MB_bat, 32, 28);    
			setupFont();
            
            set_font_pal(sel == 0 ? 15 : 14);
            put_string("Pluge", 3, row++);
            set_font_pal(sel == 1 ? 15 : 14);
            put_string("Color Bars", 3, row++);
            set_font_pal(sel == 2 ? 15 : 14);
            put_string("SMPTE Color Bars", 3, row++);
            set_font_pal(sel == 3 ? 15 : 14);
            put_string("Color Bars w/ Gray Ref", 3, row++);
            set_font_pal(sel == 4 ? 15 : 14);
            put_string("Color Bleed Check", 3, row++);
            set_font_pal(sel == 5 ? 15 : 14);
            put_string("Grid 256x224", 3, row++);
            set_font_pal(sel == 6 ? 15 : 14);
            put_string("Linearity", 3, row++);
            set_font_pal(sel == 7 ? 15 : 14);
            put_string("Gray Ramp", 3, row++);
            set_font_pal(sel == 8 ? 15 : 14);
            put_string("White & RGB Screen", 3, row++);
            set_font_pal(sel == 9 ? 15 : 14);
            put_string("100 IRE", 3, row++);
            set_font_pal(sel == 10 ? 15 : 14);
            put_string("Sharpness", 3, row++);
            set_font_pal(sel == 11 ? 15 : 14);
            put_string("Overscan", 3, row++);
			set_font_pal(sel == 12 ? 15 : 14);
            put_string("Back to Main Menu", 3, ++row);

            redraw = 0;
        }

        read = joy(0);
        controller =  read & ~OldButtonsInternal;
        OldButtonsInternal = read;
        
        if (controller & JOY_DOWN) 
        {
            sel++;
            if(sel > 12)
                sel = 0;
            redraw = 1;
        }

        if (controller & JOY_UP) 
        {
            sel--;
            if(sel < 0)
                sel = 12;
            redraw = 1;
        }
		
		if (controller & JOY_I)
		{
			switch(sel)
			{
				case 12:
					end = 1;
					break;
			}
		}
		
		if (controller & JOY_II)
			end = 1;

        vsync();
    }
}