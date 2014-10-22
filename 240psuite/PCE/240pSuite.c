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
void DisplaySystemInfo();

#define HPOS 5

void main()
{
    int controller;   
    int read; 
    int redraw = 1;
	int refresh = 1;
    int sel = 0;

	disp_off();
	set_xres(320, xres_flags);
	Set240p();
	
    while(1)
    {   	
		vsync();
	
        if(redraw)
        {
			ResetVideo();
			setupFont();

			set_map_data(MB_map, 40, 30);
			set_tile_data(MB_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, MB_pal, 1);  
           
			init_satb();
			DrawSP();
			satb_update();
			
			Center224in240();
			
            redraw = 0;
			refresh = 1;
			disp_on();
        }
		
		set_font_pal(15);
		
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
			
			DisplaySystemInfo();

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
			ResetVideo();
			switch(sel)
			{
				case 0:
					TestPatterns();
					break;
				case 1:
					DropShadow();
					break;
				case 2:
					StripedSprite();
					break;
				case 5:
					ScrollTest();
					break;
				case 7:
					DrawStripes();
					break;
				case 8:
					DrawCheck();
					break;
				case 9:
					LEDZoneTest();
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
			ResetVideo();
			setupFont();
			
			set_map_data(MB_map, 40, 30);
			set_tile_data(MB_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, MB_pal, 1);  
			
			Center224in240();
         
			init_satb();
			DrawSP();
			satb_update();

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
			
			DisplaySystemInfo();

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
			ResetVideo();
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
				case 8:
					DrawLinearity();
					break;
				case 9:
					DrawGray();
					break;
				case 10:
					DrawWhite();
					break;
				case 12:
					DrawSharpness();
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

    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			set_xres(512, xres_flags);
			ResetVideo();
			setupFont();
			
			SetFontColors(12, RGB(3, 3, 3), RGB(4, 4, 4), RGB(0, 0, 0));
			SetFontColors(13, RGB(3, 3, 3), RGB(5, 5, 5), RGB(0, 0, 0));
			set_map_data(MB512_map, 64, 30);
			set_tile_data(MB512_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 64, 30);
			load_palette(0, MB512_pal, 1);  
			
			Center224in240();
         
			refresh = 1;
            redraw = 0;
			disp_on();
        }
		
		if(refresh)
        {
            int row = 14;
            
            set_font_pal(sel == 0 ? 15 : 14);
            put_string("Vertical Resolution:", HPOS+2, row);
			if(Enabled240p)
				put_string("240p", HPOS+27, row);
			else
				put_string("224p", HPOS+27, row);
			row++;
			
			if(Enabled240p)
				set_font_pal(sel == 1 ? 15 : 14);
			else
				set_font_pal(sel == 1 ? 13 : 12);
			put_string("Start at line:", HPOS+4, row);
			if(UseDefault)
				put_string("24 [Standard use in games]", HPOS+27, row);
			else
				put_string("22 [Full 240 visible lines]", HPOS+27, row);
			
			row++;
			
			set_font_pal(sel == 2 ? 15 : 14);
            put_string("Composite filter:", HPOS+2, row);
			if(EnabledSoft)
				put_string("On ", HPOS+27, row);
			else
				put_string("Off", HPOS+27, row);
			row++;
			
			set_font_pal(sel == 3 ? 15 : 14);
            put_string("Composite B&W:", HPOS+2, row);
			if(Enabled_C_BW)
				put_string("On ", HPOS+27, row);
			else
				put_string("Off", HPOS+27, row);
			row++;
			
			set_font_pal(sel == 4 ? 15 : 14);
            put_string("Back to Main Menu", HPOS+2, ++row);

            refresh = 0;
        }

        controller = joytrg(0);
        
        if (controller & JOY_DOWN) 
        {
            sel++;
            if(sel > 4)
                sel = 0;
            refresh = 1;
        }

        if (controller & JOY_UP) 
        {
            sel--;
            if(sel < 0)
                sel = 4;
            refresh = 1;
        }
		
				
		if (controller & JOY_II)
			end = 1;
		
		if (controller & JOY_I)
		{
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
					disp_off();
					redraw = 1;	
					break;
				case 1:
					if(Enabled240p)
					{
						if(UseDefault)
						{
							UseDefault = 0;
							Set240p();
						}
						else
						{
							UseDefault = 1;
							Set239p();
						}
						disp_off();
						redraw = 1;	
					}
					break;
				case 2:
					if(EnabledSoft)
					{
						EnabledSoft = 0;
						xres_flags = XRES_SHARP;
					}
					else
					{
						EnabledSoft = 1;
						xres_flags = XRES_SOFT;
					}
					
					if(Enabled_C_BW)
						xres_flags |= XRES_BW;
					
					refresh = 1;
					
					set_xres(512, xres_flags);
					break;
				case 3:
					if(Enabled_C_BW)
					{
						Enabled_C_BW = 0;
						xres_flags = EnabledSoft ? XRES_SOFT : XRES_SHARP;
					}
					else
					{
						Enabled_C_BW = 1;
						xres_flags = (EnabledSoft ? XRES_SOFT : XRES_SHARP) | XRES_BW;
					}
					refresh = 1;
					
					set_xres(512, xres_flags);
					break;
				case 4:
					end = 1;
					break;
			}
		}

    }
	set_xres(320, xres_flags);
}

void DrawN()
{
    int controller;   
    int read; 
    int redraw = 1;
	int end = 1;

    do
    {   
		vsync();
		
		end = 1;
        if(redraw)
        {
			cls();
			set_xres(256, xres_flags);
			load_background(n_bg, n_pal, n_map, 32, 22);
			scroll(0, 0, -32, 0, 240, 0xC0);
            redraw = 0;
			disp_on();
        }

        controller = joy(0);
        
		if(controller & JOY_SEL)
			end = 0;
    }while(!end);
}

void DrawCredits()
{
	int controller;   
    int read; 
    int redraw = 1;
	int refresh = 1;
	int end = 0;
	int counter = 0;

    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			set_xres(512, xres_flags);
			
			ResetVideo();
			setupFont();
			
			SetFontColors(15, RGB(3, 3, 3), RGB(0, 6, 0), RGB(0, 0, 0));
			SetFontColors(13, RGB(3, 3, 3), RGB(1, 6, 6), RGB(0, 0, 0));
			
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
			int row = 7;
			
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
		
			set_font_pal(14);	
			put_string("Ver. 0.01", 50, 7);
			put_string("20/10/2014", 49, 8);

			refresh = 0;
		}
		
		if(counter == 1)
			put_string("Artemio Urbina      ", HPOS+2, 8);
		if(counter == 60*4)
			put_string("@Artemio (twitter)  ", HPOS+2, 8);
		if(counter == 60*8)
			put_string("aurbina@junkerhq.net", HPOS+2, 8);
		if(counter == 60*16)
			counter = 0;
			
		counter++;

        controller = joytrg(0);
        
		if (controller & JOY_II)
			end = 1;
			
		if (controller & JOY_SEL)
		{
			DrawN();
			redraw = 1;
		}
    }	
	set_xres(320, xres_flags);
}

void DrawSP()
{
	int x = 210;
	int y = 84;
	int vram = 0x5000;
	int pos = 0;
	int row = 0;
	int count = 0;
	
	load_palette(16, SD_pal, 1);
	load_vram(0x5000, SD_sp, 0x700);

	for(row = 0; row < 7; row++)
	{
		for(pos = 0; pos < 4; pos++)
		{
			spr_make(count, pos*16+x, row*16+y, 0x40*count+vram, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_16x16, 0, 1);
			count ++;
		}
	}
}

// returns 1 if US system
char DetectTG16()
{
	char region;
	char *io = 0x1000;
	
	region = *io;
	
	if((region & 0x40) == 0)
		return 1;
	else
		return 0;
}

// returns 1 if CD detected
char DetectCDROM()
{
	char region;
	char *io = 0x1000;
	
	region = *io;
	if((region & 0x80) == 0)
		return 1;
	else
		return 0;
}

void DisplaySystemInfo()
{
	SetFontColors(13, RGB(2, 4, 7), RGB(7, 7, 7), RGB(1, 3, 7));
	set_font_pal(13);
	if(DetectTG16())
	{
		if(DetectCDROM())
			put_string("TG-16+CDROM", 26, 27);
		else
			put_string("TG-16", 32, 27);
	}
	else
	{
		if(DetectCDROM())
			put_string("PCE+CDROM2", 26, 27);
		else
			put_string("PCE", 34, 27);
	}
	if(Enabled240p)
	{
		if(UseDefault)
			put_string("240p", 33, 28);
		else
			put_string("Full 240p", 28, 28);
	}
	else
		put_string("224p", 33, 28);	
}