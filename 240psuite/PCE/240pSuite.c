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
#include "patterns.h"
#include "tests.h"
#include "help.h"

void TestPatterns();
void Options();
void DrawCredits();

int Enabled240p = 0;
int UseDefault = 0;

#define HPOS 5

void main()
{
    int controller;   
    int read; 
    int redraw = 1;
	int refresh = 1;
    int sel = 0;

	disp_off();
	set_xres(320, XRES_SHARP);
	Set240p();
	
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
			disp_on();
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
			put_string("Options", HPOS, ++row);
			
			set_font_pal(sel == 12 ? 15 : 14);
            put_string("Help", HPOS, ++row);
			set_font_pal(sel == 13 ? 15 : 14);
            put_string("Credits", HPOS, ++row);

            refresh = 0;
        }

        controller = joytrg(0);
        
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
			disp_off();
			switch(sel)
			{
				case 0:
					TestPatterns();
					break;
				case 1:
					DropShadow();
					break;
				case 7:
					DrawStripes();
					break;
				case 8:
					DrawCheck();
					break;
				case 11:
					Options();
					break;
				case 12:
					showHelp();
					break;
				case 13:
					DrawCredits();
					break;
			}
			redraw = 1;			
			disp_off();
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
    int controller;   
    int read; 
    int redraw = 1;
	int refresh = 1;
    int sel = 0;
	int end = 0;

	disp_off();
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
			disp_on();
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
            put_string("Back to Main Menu", HPOS, ++row);

            refresh = 0;
        }

        controller = joytrg(0);
        
        if (controller & JOY_DOWN) 
        {
            sel++;
            if(sel > 14)
                sel = 0;
            refresh = 1;
        }

        if (controller & JOY_UP) 
        {
            sel--;
            if(sel < 0)
                sel = 14;
            refresh = 1;
        }
		
				
		if (controller & JOY_II)
			end = 1;
		
		if (controller & JOY_I)
		{
			disp_off();
			switch(sel)
			{
				case 0:
					DrawPluge();
					break;
				case 1:
					DrawColor();
					break;
				case 2:
					DrawSMPTE();
					break;
				case 3:
					DrawCB601();
					break;
				case 4:
					DrawColorBleed();
					break;
				case 5:
					DrawGrid256();
					break;
				case 6:
					DrawGrid320();
					break;
				case 7:
					DrawGrid512();
					break;
				case 10:
					DrawWhite();
					break;
				case 14:
					end = 1;
					break;
			}
			redraw = 1;	
			disp_off();
		}

    }
}


void Options()
{
    int controller;   
    int read; 
    int redraw = 1;
	int refresh = 1;
    int sel = 0;
	int end = 0;

	disp_off();
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
			disp_on();
        }
		
		if(refresh)
        {
            int row = 14;
            
            set_font_pal(sel == 0 ? 15 : 14);
            put_string("Vertical Resolution:", HPOS, row);
			if(Enabled240p)
				put_string("240p", HPOS+22, row);
			else
				put_string("224p", HPOS+22, row);
			row++;
			
			if(Enabled240p)
			{
				set_font_pal(sel == 1 ? 15 : 14);
				put_string("Start at line:", HPOS, row);
				if(UseDefault)
					put_string("24", HPOS+22, row);
				else
					put_string("22", HPOS+22, row);
			}
			else
			{
				set_font_pal(sel == 1 ? 13 : 12);
				put_string("Start at line:", HPOS, row);
				if(UseDefault)
					put_string("24", HPOS+22, row);
				else
					put_string("22", HPOS+22, row);
			}
			
			row++;
			
			
			set_font_pal(sel == 2 ? 15 : 14);
            put_string("Back to Main Menu", HPOS, ++row);

            refresh = 0;
        }

        controller = joytrg(0);
        
        if (controller & JOY_DOWN) 
        {
            sel++;
            if(sel > 2)
                sel = 0;
            refresh = 1;
        }

        if (controller & JOY_UP) 
        {
            sel--;
            if(sel < 0)
                sel = 2;
            refresh = 1;
        }
		
				
		if (controller & JOY_II)
			end = 1;
		
		if (controller & JOY_I)
		{
			disp_off();
			switch(sel)
			{
				case 0:
					if(Enabled240p)
						Set224p();
					else
						if(UseDefault)
							Set239p();
						else
							Set240p();
					break;
				case 1:
					if(UseDefault)
					{
						UseDefault = 0;
						if(Enabled240p)
							Set240p();
					}
					else
					{
						UseDefault = 1;
						if(Enabled240p)
							Set239p();
					}
					break;
				case 2:
					end = 1;
					break;
			}
			redraw = 1;	
			disp_off();
		}

    }
}


void DrawCredits()
{
	int controller;   
    int read; 
    int redraw = 1;
	int refresh = 1;
	int end = 0;

	set_xres(512, XRES_SHARP);
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			setupFont();
			set_color_rgb(241, 0, 6, 0);
			set_color_rgb(209, 1, 6, 6);
			
			set_map_data(MB512_map, 64, 30);
			set_tile_data(MB512_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 64, 30);
			load_palette(0, MB512_pal, 1);  
			
            redraw = 0;
			refresh = 1;
			disp_on();
        }
		
		if(refresh)
		{
			int row = 6;
			
			set_font_pal(15);
			put_string("Code and Patterns:", HPOS+2, row++);
			set_font_pal(14);
			put_string("Artemio Urbina", HPOS+2, row++);
			row++;
			
			set_font_pal(15);
			put_string("SDK:", HPOS+2, row++);
			set_font_pal(14);
			put_string("Enhanced HuC https://github.com/uli/huc", HPOS+2, row++);
			row++;
			
			set_font_pal(15);
			put_string("Menu Pixel Art:", HPOS+2, row++);
			set_font_pal(14);
			put_string("Asher", HPOS+2, row++);
			row++;
			
			set_font_pal(15);
			put_string("Advisor:", HPOS+2, row++);
			set_font_pal(14);
			put_string("Fudoh", HPOS+2, row++);
			row++;
			
			set_font_pal(15);
			put_string("Collaboration:", HPOS+2, row++);
			set_font_pal(14);
			put_string("Konsolkongen & shmups regulars", HPOS+2, row++);
			row++;
			
			set_font_pal(15);
			put_string("Info on using this suite:", HPOS+2, row++);
			set_font_pal(14);
			put_string("http://junkerhq.net/240p/", HPOS+2, row++);
			set_font_pal(13);
			put_string("This program is free software and open source.", HPOS+2, row++);
			put_string("Source code is available under GPL.", HPOS+2, row++);
			row++;

			refresh = 0;
		}

        controller = joytrg(0);
        
		if (controller & JOY_II)
			end = 1;
    }	
	set_xres(320, XRES_SHARP);
}