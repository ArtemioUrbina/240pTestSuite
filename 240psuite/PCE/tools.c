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
#include "graphics.h"
#include "help.h"

#ifdef CDROM1
extern int xres_flags;
extern unsigned char Enabled240p;
extern unsigned char UseDefault;
extern unsigned char EnabledSoft;
extern unsigned char Enabled_C_BW;
#endif

#define HPOS 5
 
void DrawSP()
{
	int pos = 0;
	int count = 0;

	x2 = 210;
	y2 = 84;	
	
	load_palette(16, SD_pal, 1);
#ifndef CDROM1		
	load_vram(0x5000, SD_sp, 0x700);
#else
	cd_loadvram(GPHX_OVERLAY, OFS_SD_tile_bin, 0x5000, SIZE_SD_tile_bin);
#endif

	for(row = 0; row < 7; row++)
	{
		for(pos = 0; pos < 4; pos++)
		{
			spr_make(count, pos*16+x2, row*16+y2, 0x40*count+0x5000, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_16x16, 0, 1);
			count ++;
		}
	}
}

unsigned char region;
unsigned char *io;

// returns 1 if US system
char DetectTG16()
{
	io = 0x1000;
	region = *io;
	
	if((region & 0x40) == 0)
		return 1;
	else
		return 0;
}

// returns 1 if CD detected
char DetectCDROM()
{
	io = 0x1000;
	region = *io;
	
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
		
#ifdef SCDROM
	if(ac_exists())
		put_string("AC+", 23, 27);	
#endif
}

void Options()
{
    int sel = 0;
	unsigned char end = 0;

	redraw = 1;
	refresh = 1;
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			RedrawOptions();
         
			refresh = 1;
            redraw = 0;
			disp_on();
			set_xres(512, xres_flags);
        }
		
		if(refresh)
        {
			RefreshOptions(sel);
            refresh = 0;
        }

        controller = joytrg(0);
		
		if (controller & JOY_SEL)
			end = 1;

		if (controller & JOY_RUN)
		{
			showHelp(OPTIONS_HELP);
			redraw = 1;
		}
        
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
			int val = 0;
			
			val = ExecuteOptions(sel);
			if(val == 1)
				redraw = 1;
			if(val == 2)
				refresh = 1;
			if(val == 3)
				end = 1;
		}
    }
}

void RedrawOptions()
{
	ResetVideo();
	setupFont();
	
	SetFontColors(12, RGB(3, 3, 3), RGB(4, 4, 4), 0);
	SetFontColors(13, RGB(3, 3, 3), RGB(5, 5, 5), 0);
	SetFontColors(11, RGB(3, 3, 3), RGB(0, 6, 0), 0);
#ifndef CDROM1
	set_map_data(MB512_map, 64, 30);
	set_tile_data(MB512_bg);
	load_tile(0x1000);
	load_map(0, 0, 0, 0, 64, 30);
	load_palette(0, MB512_pal, 1);  
#else
	set_screen_size(SCR_SIZE_64x32); 
	cd_loadvram(GPHX_OVERLAY, OFS_back512_BAT_bin, 0, SIZE_back512_BAT_bin);
	cd_loadvram(GPHX_OVERLAY, OFS_back512_DATA_bin, 0x1000, SIZE_back512_DATA_bin);
	cd_loaddata(GPHX_OVERLAY, OFS_back512_PAL_bin, palCD, SIZE_back512_PAL_bin); 
	set_bgpal(0, palCD); 
#endif
	
	Center224in240();
}

void RefreshOptions(int sel)
{
	row = 14;
            
	set_font_pal(11);
	put_string("Video Options", 23, 10);
			
	set_font_pal(sel == 0 ? 15 : 14);
	put_string("Vertical Resolution:", HPOS+1, row);
	if(Enabled240p)
		put_string("240p", HPOS+26, row);
	else
		put_string("224p", HPOS+26, row);
	row++;
	
	if(Enabled240p)
		set_font_pal(sel == 1 ? 15 : 14);
	else
		set_font_pal(sel == 1 ? 13 : 12);
	put_string("Start at line:", HPOS+3, row);
	if(UseDefault)
		put_string("24 [Standard use in games]", HPOS+26, row);
	else
		put_string("22 [Full 240 visible lines]", HPOS+26, row);
	
	row++;
	
	set_font_pal(sel == 2 ? 15 : 14);
	put_string("Composite filter:", HPOS+1, row);
	if(EnabledSoft)
		put_string("On ", HPOS+26, row);
	else
		put_string("Off", HPOS+26, row);
	row++;
	
	set_font_pal(sel == 3 ? 15 : 14);
	put_string("Composite B&W:", HPOS+1, row);
	if(Enabled_C_BW)
		put_string("On ", HPOS+26, row);
	else
		put_string("Off", HPOS+26, row);
	row++;
	
	set_font_pal(sel == 4 ? 15 : 14);
	put_string("Back to Main Menu", HPOS+1, ++row);

}

int ExecuteOptions(int sel)
{
	int val = 0;
	
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
			val = 1;	
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
				val = 1;	
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
			
			val = 2;
			
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
			val = 2;
			
			set_xres(512, xres_flags);
			break;
		case 4:
			val = 3;
			break;
	}
	return val;
}