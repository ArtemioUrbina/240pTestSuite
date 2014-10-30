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
#include "help.h"
#include "font.h"

#ifndef CDROM1
#include "graphics.h"
#include "graphics_patterns.h"
#endif

#ifdef CDROM1
#include "res_patterns.h"
#endif

#include "video.h"

#define HPOS 5

#ifdef CDROM1
char palCD[32];
#endif
			
#ifndef CDROM1
void TestPatterns()
#else
void main()
#endif
{
    int sel = 0;
	unsigned char end = 0;

	redraw = 1;
	refresh = 1;
	
#ifdef CDROM1
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
            RefreshTestPatterns(sel);
            refresh = 0;
        }

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(GENERAL_HELP);
			redraw = 1;
		}
		
		if (controller & JOY_SEL)
		{
			Options();
			redraw = 1;
		}
        
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
					DrawGrid(1);
					break;
				case 6:
					DrawGrid(2);
					break;
				case 7:
					DrawGrid(3);
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
				case 12:
					Draw100IRE();
					break;
				case 13:
					DrawSharpness();
					break;
				case 14:
					DrawOverscan();
					break;
				case 15:
					end = 1;
					break;
			}
			redraw = 1;	
			disp_off();
		}
    }
#ifdef CDROM1
	xres_flags_g = xres_flags;
	Enabled240p_g = Enabled240p;
	UseDefault_g = UseDefault;
	EnabledSoft_g = EnabledSoft;
	Enabled_C_BW_g = Enabled_C_BW;
	
	ResetVideo();
	set_font_pal(13);
	put_string("Loading...", 27, 26);
	cd_execoverlay(2);
#endif
}

void RefreshTestPatterns(int sel)
{
    row = 7;
	
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
	RefreshTestPatternsAux(sel);
}
		
void RefreshTestPatternsAux(int sel)
{
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

}

void DrawPluge()
{
	unsigned char end = 0;
	unsigned char col = 0;

	redraw = 1;
	refresh = 0;
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			ResetVideo();
	
#ifndef CDROM1		
			set_map_data(pluge_map, 40, 30);
			set_tile_data(pluge_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, pluge_pal, 1);  
#else
			set_screen_size(SCR_SIZE_64x32); 
			//cd_loadvram(4, OFS_pluge_tile_bin, 0x1000, SIZE_pluge_tile_bin);
			//cd_loadvram(4, OFS_pluge_map_bin, 0, SIZE_pluge_map_bin);
#endif

			Center224in240();
         
            redraw = 0;
			refresh = 1;
			disp_on();
        }
		
		if(refresh)
		{
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
			refresh = 0;
		}

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(PLUGE_HELP);
			redraw = 1;
		}

        
		if (controller & JOY_II)
			end = 1;
		
		if (controller & JOY_I)
		{
			col ++;
			if(col > 3)
				col = 0;
			refresh = 1;
		}
    }
}

void DrawColor()
{
	unsigned char end = 0;

	redraw = 1;
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			ResetVideo();
#ifndef CDROM1	
			load_background(color_bg, color_pal, color_map, 40, 30);
#else
			set_screen_size(SCR_SIZE_64x32); 
			//cd_loadvram(4, OFS_color_DATA_bin, 0x1000, SIZE_color_DATA_bin);
			//cd_loadvram(4, OFS_color_BAT_bin, 0, SIZE_color_BAT_bin);
#endif
			Center224in240();
            redraw = 0;
			disp_on();
        }

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(COLORS_HELP);
			redraw = 1;
		}

        
		if (controller & JOY_II)
			end = 1;
    }
}

void DrawCB601()
{
	unsigned char end = 0;

	redraw = 1;
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			ResetVideo();

#ifndef CDROM1
			set_map_data(cb601_map, 40, 30);
			set_tile_data(cb601_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, cb601_pal, 1);  
#else
#endif
			Center224in240();
			
            redraw = 0;
			disp_on();
        }

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(COLOR601_HELP);
			redraw = 1;
		}
        
		if (controller & JOY_II)
			end = 1;
    }
}

void DrawColorBleed()
{
	unsigned char end = 0;
	unsigned char check = 0;

	redraw = 1;
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			ResetVideo();

#ifndef CDROM1			
			set_map_data(colorbleed_map, 40, 30);
			if(check)
				set_tile_data(colorbleedchk_bg);
			else
				set_tile_data(colorbleed_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, colorbleed_pal, 1);  
#else
#endif
			Center224in240();
			
            redraw = 0;
			disp_on();
        }

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(BLEED_HELP);
			redraw = 1;
		}
        
		if (controller & JOY_II)
			end = 1;
		if (controller & JOY_I)
		{
			if(check)
			{
#ifndef CDROM1
				set_tile_data(colorbleed_bg);
				load_tile(0x1000);
#else
#endif
				check = 0;
			}
			else
			{
#ifndef CDROM1
				set_tile_data(colorbleedchk_bg);
				load_tile(0x1000);
#else
#endif
				check = 1;
			}
		}
    }
}

void DrawSMPTE()
{
	unsigned char end = 0;
	unsigned char is100 = 0;
	unsigned char text = 0;

	redraw = 1;
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			ResetVideo();
			
			setupFont();
#ifndef CDROM1
			set_map_data(SMPTE75_map, 40, 30);	
			set_tile_data(SMPTE75_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			if(is100)
				load_palette(0, SMPTE100_pal, 1);  
			else
				load_palette(0, SMPTE75_pal, 1);
#else
#endif
			Center224in240();  
			
            redraw = 0;
			disp_on();
        }

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(SMPTECOLOR_HELP);
			redraw = 1;
		}
        
		if (controller & JOY_II)
			end = 1;
		if (controller & JOY_I)
		{
			if(is100)
			{
#ifndef CDROM1
				load_palette(0, SMPTE75_pal, 1);  
#else
#endif
				is100 = 0;
				put_string(" 75%", 30, 2);
				text = 30;
			}
			else
			{
#ifndef CDROM1
				load_palette(0, SMPTE100_pal, 1);  
#else
#endif
				is100 = 1;
				put_string("100%", 30, 2);
				text = 30;
			}
		}
		
		if(text)
		{
			text--;
			if(!text)
			{
#ifndef CDROM1
				set_map_data(SMPTE75_map, 40, 3);
				load_map(0, 0, 0, 0, 40, 3);
#else
#endif
			}
		}
    }
}


void DrawGrid(char type)
{
	unsigned char end = 0;
	unsigned char showcolor = 0;

	redraw = 1;
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			ResetVideo();

#ifndef CDROM1		
			set_tile_data(grid_bg);
			load_tile(0x1000);
			
			switch(type)
			{
				case 1:
					if(Enabled240p)
						set_map_data(grid256_240_map, 32, 30);
					else
						set_map_data(grid256_224_map, 32, 28);

					load_map(0, 0, 0, 0, 32, Enabled240p ? 30 : 28);
					break;
				case 2:
					if(Enabled240p)
						set_map_data(grid320_240_map, 40, 30);
					else
						set_map_data(grid320_224_map, 40, 28);

					load_map(0, 0, 0, 0, 40, Enabled240p ? 30 : 28);
					break;
				case 3:
					if(Enabled240p)
						set_map_data(grid512_240_map, 64, 30);
					else
						set_map_data(grid512_224_map, 64, 28);
					load_map(0, 0, 0, 0, 64, Enabled240p ? 30 : 28);
					break;
			}
			
			load_palette(0, grid_pal, 1);  
#else
#endif
         
            redraw = 0;
			disp_on();
			if(type == 1)
				set_xres(256, xres_flags);
			else if(type == 2)
				set_xres(320, xres_flags);
			else if(type == 3)
				set_xres(512, xres_flags);
        }

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(GRID_HELP);
			redraw = 1;
		}
        
		if (controller & JOY_II)
			end = 1;
			
		if(controller & JOY_I)
		{
			showcolor = !showcolor;
			if(showcolor)
				set_color_rgb(256, 7, 7, 7);
			else
				set_color_rgb(256, 0, 0, 0);
		}
    }
}


void DrawWhite()
{
	unsigned char end = 0;
	unsigned char color = 0;
	unsigned char edit = 0;
	int r = 7, g = 7, b = 7;
	int sel = 0;

	redraw = 1;
	refresh = 0;
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			RedrawWhite();
			
            redraw = 0;
			refresh = 1;
			disp_on();
        }
		
		if(refresh)
		{
			RefreshWhite(color, edit, r, g, b, sel);
			refresh = 0;
		}

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(WHITE_HELP);
			redraw = 1;
		}
        
		if (controller & JOY_II)
			end = 1;
		if (!edit && controller & JOY_I)
		{
			color++;
			if(color > 4)
				color = 0;
			refresh = 1;
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
			
			refresh = 1;
		}
    }
}

void RedrawWhite()
{
	ResetVideo();

#ifndef CDROM1
	set_map_data(fs_map, 40, 30);
	set_tile_data(white_bg);
	load_tile(0x1000);
	load_map(0, 0, 0, 0, 40, 30);
	load_palette(0, check_pal, 1);  
#else
#endif
	Center224in240();
}

void RefreshWhite(unsigned char color, unsigned char edit, int r, int g, int b, int sel)
{
	if(color == 0 && edit)
	{
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
	else
	{
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
}

void DrawLinearity()
{
	unsigned char end = 0;

	redraw = 1;
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			ResetVideo();
			
			if(Enabled240p)
			{
#ifndef CDROM1
				set_map_data(linearity240_map, 40, 30);
				set_tile_data(linearity240_bg);
				load_tile(0x1000);
				load_map(0, 0, 0, 0, 40, 30);
				load_palette(0, linearity240_pal, 1);  
#else
#endif
			}
			else
			{
#ifndef CDROM1
				set_map_data(linearity224_map, 40, 28);
				set_tile_data(linearity224_bg);
				load_tile(0x1000);
				load_map(0, 0, 0, 0, 40, 28);
				load_palette(0, linearity224_pal, 1);  
#else
#endif
			}
			
            redraw = 0;
			disp_on();
        }

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(LINEARITY_HELP);
			redraw = 1;
		}
        
		if (controller & JOY_II)
			end = 1;
    }
}

void DrawLinearity256()
{
	unsigned char end = 0;

	redraw = 1;
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			ResetVideo();

#ifndef CDROM1			
			set_map_data(linearity256_map, 32, 28);
			set_tile_data(linearity256_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 32, 28);
			load_palette(0, linearity256_pal, 1);  
#else
#endif
			
            redraw = 0;
			disp_on();
			set_xres(256, xres_flags);
			if(Enabled240p)
			{
				Set224p();
				Enabled240p = 1;
			}
        }

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			if(Enabled240p)
			{
				if(!UseDefault)
					Set240p();
				else
					Set239p();
			}
			showHelp(LINEARITY256_HELP);
			redraw = 1;
		}
        
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
	unsigned char end = 0;

	redraw = 1;
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			ResetVideo();
			
#ifndef CDROM1			
			set_map_data(sharpness_map, 40, 30);
			set_tile_data(sharpness_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, sharpness_pal, 1);  
#else
#endif
			Center224in240();

            redraw = 0;
			disp_on();
        }

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(SHARPNESS_HELP);
			redraw = 1;
		}
        
		if (controller & JOY_II)
			end = 1;
    }
}

void DrawGray()
{
	unsigned char end = 0;

	redraw = 1;
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			ResetVideo();

#ifndef CDROM1			
			set_map_data(gray_map, 32, 30);
			set_tile_data(gray_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 32, 30);
			load_palette(0, gray_pal, 1);  
#else
#endif
			Center224in240();

            redraw = 0;
			disp_on();
			set_xres(256, xres_flags);
        }

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(GRAY_HELP);
			redraw = 1;
		}
        
		if (controller & JOY_II)
			end = 1;
    }
}

void DrawOverscan()
{
	unsigned char end = 0;
	unsigned char draw = 0;
	int sel = 0;
	int top = 0;
	int bottom = 0;
	int left = 0;
	int right = 0;
	unsigned char previous = 0;
	unsigned char screen = 0;
	
	redraw = 1;
	refresh = 0;
	if(Enabled240p)
	{
		if(UseDefault)
			screen = 239;
		else
			screen = 240;
	}
	else
		screen = 224;
	

    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			RedrawOverscan();
            redraw = 0;
			refresh = 1;
        }
		
		if(draw)
		{
			DrawOverscanLines(sel, top, bottom, left, right, screen, previous);
			draw = 0;
		}
		
		if(refresh)
		{
			RefreshOverscan(sel, top, bottom, left, right, screen);
			
			refresh = 0;
		}

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(OVERSCAN_HELP);
			redraw = 1;
		}

		if (controller & JOY_II)
			end = 1;
		
		if (controller & JOY_I)
		{
			left = right = bottom = top = 0;
			redraw = 1;
		}
			
		if (controller & JOY_UP)
		{
			sel --;
			refresh = 1;
		}
		
		if (controller & JOY_DOWN)
		{
			sel ++;
			refresh = 1;
		}
		
		if(sel < 0)
			sel = 3;
		if(sel > 3)
			sel = 0;
		
		if (controller & JOY_LEFT)
		{
			int *data = 0;
			
			switch(sel)
			{
				case 0:
					data = &top;
					break;
				case 1:
					data = &bottom;
					break;
				case 2:
					data = &left;
					break;
				case 3:
					data = &right;
					break;
			} 
			
			previous = *data;
			
			if(data)
			{
				(*data) --;
				if(*data < 0)
					*data = 0;
			}
			refresh = 1;
			draw = 1;
		}
		
		if (controller & JOY_RIGHT)
		{
			int *data = 0;
			
			switch(sel)
			{
				case 0:
					data = &top;
					break;
				case 1:
					data = &bottom;
					break;
				case 2:
					data = &left;
					break;
				case 3:
					data = &right;
					break;
			} 
			
			previous = *data;
			
			if(data)
			{
				(*data) ++;
				if(*data > 99)
					*data = 99;
			}
			refresh = 1;
			draw = 1;
		}
    }
}

void RedrawOverscan()
{
	ResetVideo();
			
	set_screen_size(SCR_SIZE_32x32); 
	gfx_clear(0x1200);
	gfx_init(0x1200);
	
	setupFont();
	
	set_color_rgb(1, 7, 7, 7);
	disp_on();
	set_xres(256, xres_flags);
}

void RefreshOverscan(int sel, int top, int bottom, int left, int right, int screen)
{
	int val;
	
	setupFont();
			
	set_font_pal(sel == 0 ? 15 : 14);
	put_string("Top: ", 5, 12);
	put_number(top, 3, 12, 12);
	put_string("pixels (  %)", 16, 12);
	val = (top*100)/screen;
	put_number(val, 2, 24, 12);
	
	set_font_pal(sel == 1 ? 15 : 14);
	put_string("Bottom: ", 5, 13);
	put_number(bottom, 3, 12, 13);
	put_string("pixels (  %)", 16, 13);
	val = (bottom*100)/screen;
	put_number(val, 2, 24, 13);
	
	set_font_pal(sel == 2 ? 15 : 14);
	put_string("Left: ", 5, 14);
	put_number(left, 3, 12, 14);
	put_string("pixels (  %)", 16, 14);
	val = (left*100)/256;
	put_number(val, 2, 24, 14);
	
	set_font_pal(sel == 3 ? 15 : 14);
	put_string("Right: ", 5, 15);
	put_number(right, 3, 12, 15);
	put_string("pixels (  %)", 16, 15);
	val = (right*100)/256;
	put_number(val, 2, 24, 15);
}

void DrawOverscanLines(int sel, int top, int bottom, int left, int right, int screen, unsigned char previous)
{
	switch(sel)
	{
		case 0:
			DrawTopLines(top, bottom, left, right, previous);
			break;
		case 1:
			DrawBottomLines(top, bottom, left, right, previous, screen);
			break;
		case 2:
			DrawLeftLines(top, bottom, left, right, previous, screen);
			break;
		case 3:
			DrawRightLines(top, bottom, left, right, previous, screen);
			break;
	}
}

void DrawTopLines(int top, int bottom, int left, int right, unsigned char previous)
{
	if(previous < top)
		gfx_line(left, top-1, 255-right, top-1, 1);
	else
		gfx_line(left, previous-1, 255-right, previous-1, 0);
}

void DrawBottomLines(int top, int bottom, int left, int right, unsigned char previous, unsigned char screen)
{
	if(previous < bottom)
		gfx_line(left, screen-bottom, 255-right, screen-bottom, 1);
	else
		gfx_line(left, screen-previous, 255-right, screen-previous, 0);
}

void DrawLeftLines(int top, int bottom, int left, int right, unsigned char previous, unsigned char screen)
{
	if(previous < left)
		gfx_line(left-1, top, left-1, screen-bottom, 1);
	else
		gfx_line(previous-1, top, previous-1, screen-bottom, 0);
}

void DrawRightLines(int top, int bottom, int left, int right, unsigned char previous, unsigned char screen)
{
	if(previous < right)
		gfx_line(256-right, top, 256-right, screen-bottom, 1);
	else
		gfx_line(256-previous, top, 256-previous, screen-bottom, 0);
}
// 100 IRE 
// 720mV, 624mV, 512mV, 424mV, 312mV, 208mV, 96mV, 0mV
// 100, 86.6, 71.1, 58.8, 43.3, 28.8, 13.3, 0

void Draw100IRE()
{
	unsigned char end = 0;
	unsigned char mode = 0;
	unsigned char refresh = 0;
	int factor = 14; // aproximate for IRE

	text = 0;
	redraw = 1;
	color = 7;
	set_color_rgb(0, 0, 0, 0);
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			Redraw100IRE(mode, color);

            redraw = 0;
			refresh = 1;
			disp_on();
        }
		
		if(refresh)
		{
			Refresh100IRE(mode);
			refresh = 0;
		}

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(IRE100HELP);
			redraw = 1;			
		}
		
		if (controller & JOY_LEFT)
		{
			color --;
			text = 30;
			refresh = 1;
			
			if(color < 0)
				color = 0;
				
#ifndef CDROM1
			set_map_data(ire100_map, 40, 30);
			load_map(0, 0, 0, 0, 40, 30);
#else
#endif	
			if(!mode)
				put_number(factor*color+(color > 4 ? 2 : 0), 3, 28, 26);
			else
				put_number(100+(factor*color)-(color > 4 ? 2 : 0), 3, 28, 26);
			put_string("IRE", 32, 26);
		}
		
		if (controller & JOY_RIGHT)
		{
			color ++;
			text = 30;
			refresh = 1;
			
			if(color > 7)
				color = 7;
			
#ifndef CDROM1
			set_map_data(ire100_map, 40, 30);
			load_map(0, 0, 0, 0, 40, 30);
#else
#endif	
			if(!mode)
				put_number(factor*color+(color > 4 ? 2 : 0), 3, 28, 26);
			else
				put_number(100+(factor*color)-(color > 4 ? 2 : 0), 3, 28, 26);
				
			put_string("IRE", 32, 26);
		}
		
		if (controller & JOY_I)
		{
			mode = !mode;
			if(mode)
			{
				color = 0;
				set_color_rgb(1, 7, 7, 7);
				refresh = 1;
				put_string("RANGE 100-140 IRE", 20, 26);
				factor = 6;
			}
			else
			{
				color = 7;
				set_color(0, 0);
				refresh = 1;
				put_string("RANGE 0-100 IRE  ", 20, 26);
				factor = 14;
			}
			text = 30;
		}
        
		if (controller & JOY_II)
			end = 1;
		
		if(text)
		{
			text--;
			if(!text)
			{
#ifndef CDROM1
				set_map_data(ire100_map, 40, 30);
				load_map(0, 0, 0, 0, 40, 30);
#else
#endif
			}
		}
    }
}

void Refresh100IRE(unsigned char mode)
{
	set_color_rgb(!mode, color, color, color);	
	if(mode)
		SetFontColors(14, RGB(color, color, color), RGB(6, 6, 6), RGB(3, 3, 3));
	else
		SetFontColors(14, 0, RGB(6, 6, 6), RGB(3, 3, 3));		
}

void Redraw100IRE(unsigned char mode, unsigned char color)
{
	ResetVideo();
	setupFont();
	
	if(mode)
	{
		SetFontColors(14, RGB(color, color, color), RGB(6, 6, 6), RGB(3, 3, 3));
		set_color_rgb(1, 7, 7, 7);
	}
	else
	{
		SetFontColors(14, 0, RGB(6, 6, 6), RGB(3, 3, 3));
		set_color(0, 0);
	}
	
#ifndef CDROM1			
	set_map_data(ire100_map, 40, 30);
	set_tile_data(ire100_bg);
	load_tile(0x1000);
	load_map(0, 0, 0, 0, 40, 30);	
#else
#endif
	Center224in240();
}

