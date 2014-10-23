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

#ifndef CDROM
#include "graphics.h"
#else
#include "res_patterns.h"
#include "graphics_patterns.h"
#endif

#include "video.h"

#define HPOS 5

#ifndef CDROM
void TestPatterns()
#else
void main()
#endif
{
    int controller;   
    int read; 
    int redraw = 1;
	int refresh = 1;
    int sel = 0;
	int end = 0;

#ifdef CDROM
	/* global values are not holding their data */
	xres_flags = xres_flags_g;
	Enabled240p = Enabled240p_g;
	UseDefault = UseDefault_g;
	EnabledSoft = EnabledSoft_g;
	Enabled_C_BW = Enabled_C_BW_g;
#endif

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
			if(Enabled240p)
				put_string("Linearity 320x240", HPOS, row++);
			else
				put_string("Linearity 320x224", HPOS, row++);
			set_font_pal(sel == 9 ? 15 : 14);
            put_string("Linearity 256x224", HPOS, row++);
            set_font_pal(sel == 10 ? 15 : 14);
            put_string("Gray Ramp", HPOS, row++);
            set_font_pal(sel == 11 ? 15 : 14);
            put_string("White & RGB Screen", HPOS, row++);
            set_font_pal(sel == 12 ? 15 : 14);
            put_string("100 IRE", HPOS, row++);
            set_font_pal(sel == 13 ? 15 : 14);
            put_string("Sharpness", HPOS, row++);
            set_font_pal(sel == 14 ? 15 : 14);
            put_string("Overscan", HPOS, row++);
			
			set_font_pal(sel == 15 ? 15 : 14);
            put_string("Back to Main Menu", HPOS, ++row);
			
			DisplaySystemInfo();

            refresh = 0;
        }

        controller = joytrg(0);
        
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
					DrawLinearity256();
					break;
				case 10:
					DrawGray();
					break;
				case 11:
					DrawWhite();
					break;
				case 13:
					DrawSharpness();
					break;
				case 15:
					end = 1;
					break;
			}
			redraw = 1;	
			disp_off();
		}
    }
#ifdef CDROM
	xres_flags_g = xres_flags;
	Enabled240p_g = Enabled240p;
	UseDefault_g = UseDefault;
	EnabledSoft_g = EnabledSoft;
	Enabled_C_BW_g = Enabled_C_BW;
	
	cd_execoverlay(2);
#endif
}

void DrawPluge()
{
    int controller;   
    int read; 
    int redraw = 1;
	int end = 0;
	int col = 0;

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
			Center224in240();
         
            redraw = 0;
			disp_on();
        }

        controller = joytrg(0);
        
		if (controller & JOY_II)
			end = 1;
		
		if (controller & JOY_I)
		{
			col ++;
			if(col > 3)
				col = 0;
			switch(col)
			{
				case 0:
					set_color_rgb(1, 1, 1, 1);
					break;
				case 1:
					set_color_rgb(1, 0, 0, 1);
					break;
				case 2:
					set_color_rgb(1, 0, 1, 0);
					break;
				case 3:
					set_color_rgb(1, 1, 0, 0);
					break;
			}
		}
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
			Center224in240();
            redraw = 0;
			disp_on();
        }

        controller = joytrg(0);
        
		if (controller & JOY_II)
			end = 1;
    }
}

void DrawCB601()
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
			set_map_data(cb601_map, 40, 30);
			set_tile_data(cb601_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, cb601_pal, 1);  
			Center224in240();
			
            redraw = 0;
			disp_on();
        }

        controller = joytrg(0);
        
		if (controller & JOY_II)
			end = 1;
    }
}

void DrawColorBleed()
{
    int controller;   
    int read; 
    int redraw = 1;
	int end = 0;
	int check = 0;

    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			set_map_data(colorbleed_map, 40, 30);
			if(check)
				set_tile_data(colorbleedchk_bg);
			else
				set_tile_data(colorbleed_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, colorbleed_pal, 1);  
			Center224in240();
			
            redraw = 0;
			disp_on();
        }

        controller = joytrg(0);
        
		if (controller & JOY_II)
			end = 1;
		if (controller & JOY_I)
		{
			if(check)
			{
				set_tile_data(colorbleed_bg);
				load_tile(0x1000);
				check = 0;
			}
			else
			{
				set_tile_data(colorbleedchk_bg);
				load_tile(0x1000);
				check = 1;
			}
		}
    }
}

void DrawSMPTE()
{
    int controller;   
    int read; 
    int redraw = 1;
	int end = 0;
	int is100 = 0;
	int text = 0;

    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			set_map_data(SMPTE75_map, 40, 30);	
			set_tile_data(SMPTE75_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			if(is100)
				load_palette(0, SMPTE100_pal, 1);  
			else
				load_palette(0, SMPTE75_pal, 1);
			Center224in240();  
			
            redraw = 0;
			disp_on();
        }

        controller = joytrg(0);
        
		if (controller & JOY_II)
			end = 1;
		if (controller & JOY_I)
		{
			if(is100)
			{
				load_palette(0, SMPTE75_pal, 1);  
				is100 = 0;
				put_string(" 75%", 30, 2);
				text = 30;
			}
			else
			{
				load_palette(0, SMPTE100_pal, 1);  
				is100 = 1;
				put_string("100%", 30, 2);
				text = 30;
			}
		}
		
		if(text)
		{
			text--;
			if(!text)
				redraw = 1;
		}
    }
}


void DrawGrid256()
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
			set_xres(256, xres_flags);
        }

        controller = joytrg(0);
        
		if (controller & JOY_II)
			end = 1;
    }
}

void DrawGrid320()
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
			set_xres(320, xres_flags);
        }

        controller = joytrg(0);
        
		if (controller & JOY_II)
			end = 1;
    }
}

void DrawGrid512()
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
			set_xres(512, xres_flags);
        }

        controller = joytrg(0);
        
		if (controller & JOY_II)
			end = 1;
    }
}

void DrawWhite()
{
    int controller;   
    int read; 
    int redraw = 1;
	int end = 0;
	int color = 0;
	int edit = 0;
	int r = 7, g = 7, b = 7;
	int sel = 0;

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
			Center224in240();
         
            redraw = 0;
			disp_on();
        }

        controller = joytrg(0);
        
		if (controller & JOY_II)
			end = 1;
		if (!edit && controller & JOY_I)
		{
			color++;
			if(color > 4)
				color = 0;
			switch(color)
			{
				case 0:
					set_color_rgb(1, 7, 7, 7);
					break;
				case 1:
					set_color_rgb(1, 0, 0, 0);
					break;
				case 2:
					set_color_rgb(1, 7, 0, 0);
					break;
				case 3:
					set_color_rgb(1, 0, 7, 0);
					break;
				case 4:
					set_color_rgb(1, 0, 0, 7);
					break;
			}
		}
		
		if (controller & JOY_SEL)
		{
			if(edit)
			{
				redraw = 1;
				edit = 0;
			}
			else
				edit = 1;
		}
			
		if(color == 0 && edit)
		{
			if (controller & JOY_LEFT)
			{
				if(sel > 1)
					sel --;
				else
					sel = 0;
			}
			
			if (controller & JOY_RIGHT)
			{
				sel ++;
				if(sel > 2)
					sel = 2;
			}
			
			if (controller & JOY_UP)
			{
				switch(sel)
				{
					case 0:
						r++;
						break;
					case 1:
						g++;
						break;
					case 2:
						b++;
						break;
				}
				
				if(r > 7)
					r = 7;
				if(g > 7)
					g = 7;
				if(b > 7)
					b = 7;
			}
			
			if (controller & JOY_DOWN)
			{
				switch(sel)
				{
					case 0:
						r--;
						break;
					case 1:
						g--;
						break;
					case 2:
						b--;
						break;
				}
				
				if(r < 0)
					r = 0;
				if(g < 0)
					g = 0;
				if(b < 0)
					b = 0;
			}
			
			set_font_pal(sel == 0 ? 15 : 14);
			put_string("R:", 24, 2);
			put_digit(r, 26, 2);
			set_font_pal(sel == 1 ? 15 : 14);
			put_string(" G:", 27, 2);
			put_digit(g, 30, 2);
			set_font_pal(sel == 2 ? 15 : 14);
			put_string(" B:", 31, 2);
			put_digit(b, 34, 2);
			
			set_color_rgb(1, r, g, b);
		}
    }
}

void DrawLinearity()
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
			if(Enabled240p)
			{
				set_map_data(linearity240_map, 40, 30);
				set_tile_data(linearity240_bg);
				load_tile(0x1000);
				load_map(0, 0, 0, 0, 40, 30);
				load_palette(0, linearity240_pal, 1);  
			}
			else
			{
				set_map_data(linearity224_map, 40, 28);
				set_tile_data(linearity224_bg);
				load_tile(0x1000);
				load_map(0, 0, 0, 0, 40, 28);
				load_palette(0, linearity224_pal, 1);  
			}
			
            redraw = 0;
			disp_on();
        }

        controller = joytrg(0);
        
		if (controller & JOY_II)
			end = 1;
    }
}

void DrawLinearity256()
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
			set_map_data(linearity256_map, 32, 28);
			set_tile_data(linearity256_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 32, 28);
			load_palette(0, linearity256_pal, 1);  
			
            redraw = 0;
			disp_on();
			set_xres(256, xres_flags);
			if(Enabled240p)
				Set224p();
        }

        controller = joytrg(0);
        
		if (controller & JOY_II)
			end = 1;
    }
	if(Enabled240p)
	{
		if(!UseDefault)
			Set240p();
		else
			Set239p();
	}
}

void DrawSharpness()
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
			set_map_data(sharpness_map, 40, 30);
			set_tile_data(sharpness_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, sharpness_pal, 1);  
			Center224in240();

            redraw = 0;
			disp_on();
        }

        controller = joytrg(0);
        
		if (controller & JOY_II)
			end = 1;
    }
}

void DrawGray()
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
			set_map_data(gray_map, 32, 30);
			set_tile_data(gray_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 32, 30);
			load_palette(0, gray_pal, 1);  
			Center224in240();

            redraw = 0;
			disp_on();
			set_xres(256, xres_flags);
        }

        controller = joytrg(0);
        
		if (controller & JOY_II)
			end = 1;
    }
}