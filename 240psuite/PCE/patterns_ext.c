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

#ifdef CDROM1 
#include "huc.h"
#include "patterns.h"
#include "help.h"
#include "font.h"
#include "float.h"
#include "res_patterns.h"

#include "video.h"

char palCD[512];

extern int xres_flags;
extern unsigned char Enabled240p;
extern unsigned char UseDefault;
extern unsigned char EnabledSoft;
extern unsigned char Enabled_C_BW;
#endif

#ifdef CDROM1
void main()
{
	RestoreGlobals();
	
	switch(ToolItem)
	{
		case TOOL_OVERSCAN:
			DrawOverscan();
			break;
		case TOOL_IRE:
			Draw100IRE();
			break;
		case TOOL_CONVERG:
			DrawConvergence();
			break;
		case TOOL_WHITE:
			DrawWhite();
			break;
	}
	cd_execoverlay(PATTERNS_OVERLAY);
}
#endif

void DrawWhite()
{
	x = 0;  // color
	y = 0;  // edit
	x1 = 7;	// R
	x2 = 7; // G
	x3 = 7; // B

	option = 0;
	redraw = 1;
	refresh = 0;
	end = 0;
	while(!end)
	{	
		vsync();
		
		if(redraw)
		{
			RedrawWhite();
			SetFontColors(14, RGB(3, 3, 3), RGB(0, 6, 0), 0);
			SetFontColors(15, RGB(2, 2, 2), RGB(0, 6, 0), 0);
			
			redraw = 0;
			refresh = 1;
			disp_on();
		}
		
		if(refresh)
		{
			RefreshWhite(x, y, x1, x2, x3);
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
		if (!y && controller & JOY_I)
		{
			x++;
			if(x > 4)
				x = 0;
			refresh = 1;
		}
		
		if (controller & JOY_SEL)
		{
			if(y)
			{
				redraw = 1;
				y = 0;
			}
			else
				y = 1;
		}
			
		if(x == 0 && y)
		{
			if (controller & JOY_LEFT)
			{
				if(option > 1)
					option --;
				else
					option = 0;
			}
			
			if (controller & JOY_RIGHT)
			{
				option ++;
				if(option > 2)
					option = 2;
			}
			
			if (controller & JOY_UP)
			{
				switch(option)
				{
					case 0:
						x1++;
						break;
					case 1:
						x2++;
						break;
					case 2:
						x3++;
						break;
				}
				
				if(x1 > 7)
					x1 = 7;
				if(x2 > 7)
					x2 = 7;
				if(x3 > 7)
					x3 = 7;
			}
			
			if (controller & JOY_DOWN)
			{
				switch(option)
				{
					case 0:
						x1--;
						break;
					case 1:
						x2--;
						break;
					case 2:
						x3--;
						break;
				}
				
				if(x1 < 0)
					x1 = 0;
				if(x2 < 0)
					x2 = 0;
				if(x3 < 0)
					x3 = 0;
			}
			
			refresh = 1;
		}
	}
}

void RedrawWhite()
{
	ResetVideo();

	set_map_data(fs_map, 64, 32);
	set_tile_data(white_bg);
	load_tile(0x1000);
	load_map(0, 0, 0, 0, 64, 32);
	load_palette(0, check_pal, 1);	
	Center224in240();
}

void RefreshWhite(unsigned char color, unsigned char edit, int r, int g, int b)
{
	if(color == 0 && edit)
	{
		set_font_pal(option == 0 ? 15 : 14);
		put_string("R:", 24, 2);
		put_digit(r, 26, 2);
		set_font_pal(option == 1 ? 15 : 14);
		put_string(" G:", 27, 2);
		put_digit(g, 30, 2);
		set_font_pal(option == 2 ? 15 : 14);
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

void DrawOverscan()
{
	int continuous = 0;
	
	draw = 0;
	option = 0;
	top = 0;
	bottom = 0;
	left = 0;
	right = 0;
	previous = 0;
	screen = 0;
	end = 0;
	
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
			DrawOverscanLines();
			draw = 0;
		}
		
		if(refresh)
		{
			RefreshOverscan();
			
			refresh = 0;
		}

		controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
#ifdef CDROM
			RedrawWhite();
#endif
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
		
		if (controller & JOY_SEL)
			continuous = !continuous;
			
		if (controller & JOY_UP)
		{
			option --;
			refresh = 1;
		}
		
		if (controller & JOY_DOWN)
		{
			option ++;
			refresh = 1;
		}
		
		if(option < 0)
			option = 3;
		if(option > 3)
			option = 0;
		
		if(continuous)
			controller = joy(0);
			
		if (controller & JOY_LEFT)
		{
			int *data = 0;
			
			switch(option)
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
				if(*data > 0)
				{
					(*data) --;
					refresh = 1;
					draw = 1;
				}
			}
		}
		
		if (controller & JOY_RIGHT)
		{
			int *data = 0;
			
			switch(option)
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
				if(*data < 30)
				{
					(*data) ++;
					refresh = 1;
					draw = 1;
				}
			}
		}
	}
#ifdef CDROM
	RedrawWhite();
#endif
}

void RedrawOverscan()
{
	ResetVideo();
			
	set_screen_size(SCR_SIZE_32x32); 
	gfx_clear(0x1200);
	gfx_init(0x1200);
	
	setupFont();
	
	set_color_rgb(0, 3, 3, 3);
	set_color_rgb(1, 7, 7, 7);
	disp_on();
	Set256H();
}

void RefreshOverscan()
{
	set_font_pal(option == 0 ? 15 : 14);
	put_string("Top: ", 5, 12);
	put_number(top, 3, 12, 12);
	put_string("pixels (  %)", 16, 12);
	x3 = (top*100)/screen;
	put_number(x3, 2, 24, 12);
	
	set_font_pal(option == 1 ? 15 : 14);
	put_string("Bottom: ", 5, 13);
	put_number(bottom, 3, 12, 13);
	put_string("pixels (  %)", 16, 13);
	x3 = (bottom*100)/screen;
	put_number(x3, 2, 24, 13);
	
	set_font_pal(option == 2 ? 15 : 14);
	put_string("Left: ", 5, 14);
	put_number(left, 3, 12, 14);
	put_string("pixels (  %)", 16, 14);
	x3 = (left*100)/256;
	put_number(x3, 2, 24, 14);
	
	set_font_pal(option == 3 ? 15 : 14);
	put_string("Right: ", 5, 15);
	put_number(right, 3, 12, 15);
	put_string("pixels (  %)", 16, 15);
	x3 = (right*100)/256;
	put_number(x3, 2, 24, 15);
}

void DrawOverscanLines()
{
	switch(option)
	{
		case 0:
			DrawTopLines();
			break;
		case 1:
			DrawBottomLines();
			break;
		case 2:
			DrawLeftLines();
			break;
		case 3:
			DrawRightLines();
			break;
	}
}

void DrawTopLines()
{
	if(previous < top)
		gfx_line(left, top-1, 255-right, top-1, 1);
	else
		gfx_line(left, previous-1, 255-right, previous-1, 0);
}

void DrawBottomLines()
{
	if(previous < bottom)
		gfx_line(left, screen-bottom, 255-right, screen-bottom, 1);
	else
		gfx_line(left, screen-previous, 255-right, screen-previous, 0);
}

void DrawLeftLines()
{
	if(previous < left)
		gfx_line(left-1, top, left-1, screen-bottom-1, 1);
	else
		gfx_line(previous-1, top, previous-1, screen-bottom-1, 0);
}

void DrawRightLines()
{
	if(previous < right)
		gfx_line(256-right, top, 256-right, screen-bottom-1, 1);
	else
		gfx_line(256-previous, top, 256-previous, screen-bottom-1, 0);
}

// 100 IRE 
// 720mV, 624mV, 512mV, 424mV, 312mV, 208mV, 96mV, 0mV
// 100, 86.6, 71.1, 58.8, 43.3, 28.8, 13.3, 0

void Draw100IRE()
{
	x1 = 14; // aproximate for IRE (factor)
	text = 0;
	redraw = 1;
	refresh = 0;
	color = 7;
	end = 0;
	option = 0;
	set_color_rgb(0, 0, 0, 0);
	
	while(!end)
	{	
		vsync();
		
		if(redraw)
		{
			Redraw100IRE(option, color);

			redraw = 0;
			refresh = 1;
			disp_on();
		}
		
		if(refresh)
		{
			Refresh100IRE(option);
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
			cd_loadvram(GPHX_OVERLAY, OFS_100IRE_BAT_bin, 0x0000, SIZE_100IRE_BAT_bin);
#endif	
			if(!option)
				put_number(x1*color+(color > 4 ? 2 : 0), 3, 28, 26);
			else
				put_number(100+(x1*color)-(color > 4 ? 2 : 0), 3, 28, 26);
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
			cd_loadvram(GPHX_OVERLAY, OFS_100IRE_BAT_bin, 0x0000, SIZE_100IRE_BAT_bin);
#endif	
			if(!option)
				put_number(x1*color+(color > 4 ? 2 : 0), 3, 28, 26);
			else
				put_number(100+(x1*color)-(color > 4 ? 2 : 0), 3, 28, 26);
				
			put_string("IRE", 32, 26);
		}
		
		if (controller & JOY_I)
		{
			option = !option;
			if(option)
			{
				color = 0;
#ifndef CDROM1
				set_color_rgb(1, 7, 7, 7);
#else
				set_color_rgb(2, 7, 7, 7);
#endif
				refresh = 1;
				put_string("RANGE 100-140 IRE", 20, 26);
				x1 = 6;
			}
			else
			{
				color = 7;
				set_color(0, 0);
				refresh = 1;
				put_string("RANGE 0-100 IRE  ", 20, 26);
				x1 = 14;
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
				cd_loadvram(GPHX_OVERLAY, OFS_100IRE_BAT_bin, 0x0000, SIZE_100IRE_BAT_bin);
#endif
			}
		}
	}
}

void Refresh100IRE(unsigned char mode)
{
#ifndef CDROM1
	set_color_rgb(!mode, color, color, color);	
#else
	if(!mode)
		set_color_rgb(2, color, color, color);	
	else
		set_color_rgb(0, color, color, color);	
#endif
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
	set_screen_size(SCR_SIZE_64x32); 
	cd_loaddata(GPHX_OVERLAY, OFS_100IRE_PAL_bin, palCD, SIZE_100IRE_PAL_bin); 
	set_bgpal(0, palCD); 
	cd_loadvram(GPHX_OVERLAY, OFS_100IRE_DATA_bin, 0x1000, SIZE_100IRE_DATA_bin);
	cd_loadvram(GPHX_OVERLAY, OFS_100IRE_BAT_bin, 0x0000, SIZE_100IRE_BAT_bin);
#endif
	Center224in240();
}

void RefreshConvergence()
{
	if(x1 < 3)
	{
		switch(x1)
		{
			case 0:
				set_tile_data(convlinestile_bg);
				break;
			case 1:
				set_tile_data(convcrosstile_bg);
				break;
			case 2:
				set_tile_data(convdottile_bg);
				break;
		}
		load_tile(0x1000);
		load_palette(0, convlinestile_pal, 1);  
	}
	else
	{
#ifndef CDROM1	
		load_palette(0, convcolor_pal, 1); 
#else
		set_bgpal(0, palCD); 
#endif

#ifndef CDROM1		
		if(x1 == 3)
		{
			set_color_rgb(0, 0, 0, 7);
			set_color_rgb(2, 0, 7, 0);
			set_color_rgb(4, 7, 0, 0);
			set_color_rgb(5, 7, 7, 7);
		}
		else
		{
			set_color(0, 0);
			set_color(2, 0);
			set_color(4, 0);
			set_color(5, 0);
		}
#else
		if(x1 == 3)
		{
			set_color_rgb(1, 7, 7, 7);
			set_color_rgb(4, 7, 0, 0);
			set_color_rgb(6, 0, 7, 0);
			set_color_rgb(8, 0, 0, 7);
		}
		else
		{
			set_color(1, 0);
			set_color(4, 0);
			set_color(6, 0);
			set_color(8, 0);
		}
#endif
	}
	if(type == RES_320)
		Set320H();
	if(type == RES_256)
		Set256H();
}

void RedrawConvergence()
{
	RefreshConvergence();
	if(x1 < 3)
	{
		set_map_data(fs_map, 40, 30);
		load_map(0, 0, 0, 0, 40, 30);
	}
	else
	{
#ifndef CDROM1			
		set_map_data(convcolor_map, 40, 30);
		set_tile_data(convcolor_bg);
		load_tile(0x1000);
		load_map(0, 0, 0, 0, 40, 30);
#else
		set_screen_size(SCR_SIZE_64x32); 
		cd_loadvram(GPHX_OVERLAY, OFS_convcolor_DATA_bin, 0x1000, SIZE_convcolor_DATA_bin);
		cd_loadvram(GPHX_OVERLAY, OFS_convcolor_BAT_bin, 0x0000, SIZE_convcolor_BAT_bin);
		cd_loaddata(GPHX_OVERLAY, OFS_convcolor_PAL_bin, palCD, SIZE_convcolor_PAL_bin); 
#endif
	}

	Center224in240();
}

void DrawConvergence()
{	
	type = FloatMenuRes320n256(1);
	
	end = 0;
	redraw = 1;
	option = 0;
	x1 = 0;
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			ResetVideo();
			RedrawConvergence();
            redraw = 0;
			refresh = 1;
			disp_on();
        }
		
		if(refresh)
		{
			RefreshConvergence();
			refresh = 0;
		}
		
		controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(CONVERGENCE_HELP);
			redraw = 1;
		}
		
		if (controller & JOY_I || controller & JOY_RIGHT)
		{
			x1++;
			if(x1 > 4)
				x1 = 0;
			if(x1 == 3 || x1 == 0)
				redraw = 1;
			refresh = 1;
		}
        
		if (controller & JOY_II)
			end = 1;
			
		if (controller & JOY_LEFT)
		{
			x1--;
			if(x1 < 0)
				x1 = 4;
			if(x1 == 2 || x1 == 4)
				redraw = 1;
			refresh = 1;
		}
		
		if(controller & JOY_SEL)
		{
			type = FloatMenuRes320n256(type);
			redraw = 1;
		}
    }
}