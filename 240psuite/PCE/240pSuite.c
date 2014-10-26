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
#include "font.h"
#include "video.h"
#include "tests.h"
#include "help.h"
#include "tools.h"

void DrawCredits();
void DisplaySystemInfo();
void DrawIntro();


#ifdef CDROM1
char palCD[32];
#endif

#define HPOS 5

void main()
{
    int controller;   
    int read; 
    int redraw = 1;
	int refresh = 1;
    int sel = 0;

	if(!global_init)
	{
		xres_flags = XRES_SOFT;
		Enabled240p = 1;
		UseDefault = 0;
		EnabledSoft = 1;
		Enabled_C_BW = 0;
		global_init = 1;
	}
#ifdef CDROM
	else
	{
		xres_flags = xres_flags_g;
		Enabled240p = Enabled240p_g;
		UseDefault = UseDefault_g;
		EnabledSoft = EnabledSoft_g;
		Enabled_C_BW = Enabled_C_BW_g;
	}
#endif

	disp_off();
	set_xres(320, xres_flags);
	if(Enabled240p)
		Set240p();

#ifndef CDROM
#ifndef ARCADE
	disp_on();
	DrawIntro();
	disp_off();
#endif
#endif

    while(1)
    {   	
		vsync();
	
        if(redraw)
        {
			ResetVideo();
			setupFont();

#ifndef CDROM1
			set_map_data(MB_map, 40, 30);
			set_tile_data(MB_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, MB_pal, 1);  
#else
			set_screen_size(SCR_SIZE_64x32); 
			cd_loadvram(4, OFS_mainbg_BAT_bin, 0x0000, SIZE_mainbg_BAT_bin);
			cd_loadvram(4, OFS_mainbg_DATA_bin, 0x1000, SIZE_mainbg_DATA_bin);
			cd_loaddata(4, OFS_mainbg_PAL_bin, palCD, SIZE_mainbg_PAL_bin); 
			set_bgpal(0, palCD); 
#endif
           
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
		
		if (controller & JOY_SEL)
		{
			Options();
			redraw = 1;
		}
        
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
		
		if (controller & JOY_RUN)
		{
			showHelp(GENERAL_HELP);
			redraw = 1;
		}
		
		if (controller & JOY_I)
		{
			disp_off();
			ResetVideo();
			switch(sel)
			{
				case 0:
#ifndef CDROM
					TestPatterns();
#else
					xres_flags_g = xres_flags;
					Enabled240p_g = Enabled240p;
					UseDefault_g = UseDefault;
					EnabledSoft_g = EnabledSoft;
					Enabled_C_BW_g = Enabled_C_BW;
					
					set_font_pal(14);
					put_string("Loading...", 27, 26);
					cd_execoverlay(3);
#endif
					break;
				case 1:
					DropShadow();
					break;
				case 2:
					StripedSprite();
					break;
				case 3:
					LagTest();
					break;
				case 5:
					ScrollTest();
					break;
				case 6:
					VScrollTest();
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
				case 10:
					SoundTest();
					break;
				case 11:
					Options();
					break;
				case 12:
					showHelp(GENERAL_HELP);
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
#ifndef CDROM1
			load_background(n_bg, n_pal, n_map, 32, 22);
#else
			set_screen_size(SCR_SIZE_32x32); 
			cd_loadvram(4, OFS_N_DATA_bin, 0x1000, SIZE_N_DATA_bin);
			cd_loadvram(4, OFS_N_BAT_bin, 0, SIZE_N_BAT_bin);
#endif

			scroll(0, 0, -32, 0, 240, 0xC0);
            redraw = 0;
			disp_on();
			set_xres(256, xres_flags);
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
			ResetVideo();
			setupFont();
			
			SetFontColors(15, RGB(3, 3, 3), RGB(0, 6, 0), RGB(0, 0, 0));
			SetFontColors(13, RGB(3, 3, 3), RGB(1, 6, 6), RGB(0, 0, 0));

#ifndef CDROM1			
			set_map_data(MB512_map, 64, 30);
			set_tile_data(MB512_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 64, 30);
			load_palette(0, MB512_pal, 1);  
#else
			set_screen_size(SCR_SIZE_64x32); 
			cd_loadvram(4, OFS_back512_BAT_bin, 0, SIZE_back512_BAT_bin);
			cd_loadvram(4, OFS_back512_DATA_bin, 0x1000, SIZE_back512_DATA_bin);
			cd_loaddata(4, OFS_back512_PAL_bin, palCD, SIZE_back512_PAL_bin); 
			set_bgpal(0, palCD); 
#endif
			
			Center224in240();
			
            redraw = 0;
			refresh = 1;
			disp_on();
			set_xres(512, xres_flags);
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
}

#ifndef CDROM

void DrawIntro()
{
	int frame;
	
	ResetVideo();
	setupFont();
	
	vsync();
	set_color(1, 0);
	SetFontColors(14, RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0));
	put_string("KORDAMP PRESENTS", 12, 14);
	for(frame = 0; frame < 7; frame ++)
	{
		SetFontColors(14, RGB(0, 0, 0), RGB(frame, frame, frame), RGB(0, 0, 0));
		vsync(3);
	}
	
	frame = 5;
	while(frame)
		frame --;
	
	for(frame = 7; frame > 0; frame --)
	{
		SetFontColors(14, RGB(0, 0, 0), RGB(frame, frame, frame), RGB(0, 0, 0));
		vsync(3);
	}
}

#endif