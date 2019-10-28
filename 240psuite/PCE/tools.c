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
#include "tools.h"

#ifdef CDROM1
extern int xres_flags;
extern unsigned char Enabled240p;
extern unsigned char UseDefault;
extern unsigned char EnabledSoft;
extern unsigned char Enabled_C_BW;

extern int x_g;
#endif

#define HPOS 5

#ifndef HELP_OVL
void RedrawMain()
{
#ifndef SGFX
	RedrawBG();
#else
	if(sgx_detect())
	{
		vpc_win_size(VPC_WIN_A, 0x01ff);
		vpc_win_size(VPC_WIN_B, 0x01ff);
		vpc_win_reg(VPC_WIN_A, VDC_ON+VPC_NORM);
		vpc_win_reg(VPC_WIN_B, VDC_ON+VPC_NORM);
		vpc_win_reg(VPC_WIN_AB, VDC_ON+VPC_NORM);
		vpc_win_reg(VPC_WIN_NONE, VDC_ON+VPC_NORM);

		sgx_set_screen_size(SCR_SIZE_32x32);
		sgx_disp_off();
		
		sgx_set_tile_data(SMPTE75_bg);
		sgx_load_tile(0x1000);		
		sgx_set_map_data(SMPTE75_map, 40, 30);
		sgx_load_map(0, 0, 0, 0, 40, 30);	
		//load_palette(0, SMPTE75_pal, 16);
		sgx_disp_on();
		sgx_scroll(40, 40);
	}
#endif

	init_satb();
	DrawSP();
	satb_update();
	DisplaySystemInfo();	
}

void RedrawBG()
{
	ResetVideo();
	setupFont();

#ifndef CDROM1
	set_tile_data(MB_bg);
	load_tile(0x1000);
	set_map_data(MB_map, 40, 30);
	load_map(0, 0, 0, 0, 40, 30);
	load_palette(0, MB_pal, 1);  
#else
	set_screen_size(SCR_SIZE_64x32); 
	cd_loaddata(GPHX_OVERLAY, OFS_mainbg_PAL_bin, palCD, SIZE_mainbg_PAL_bin);  
	cd_loadvram(GPHX_OVERLAY, OFS_mainbg_DATA_bin, 0x1000, SIZE_mainbg_DATA_bin);
	cd_loadvram(GPHX_OVERLAY, OFS_mainbg_BAT_bin, 0x0000, SIZE_mainbg_BAT_bin);
	set_bgpal(0, palCD);
#endif
	
	set_font_pal(13);
	SetFontColors(12, RGB(3, 3, 3), RGB(0, 6, 0), 0);
	SetFontColors(13, RGB(2, 2, 2), RGB(0, 6, 0), 0);
	Center224in240();
}

#endif

#ifdef CDROM1
	#define LOAD_OVERLAY
#endif
#ifdef HELP_OVL
	#define LOAD_OVERLAY
	#include "gdata.h"
	extern char palCD[];
#endif

void Rewdraw512Menu()
{
	ResetVideo();
	setupFont();
	
	Set512H();
	
#ifdef LOAD_OVERLAY
	set_screen_size(SCR_SIZE_64x32); 
	cd_loaddata(GPHX_OVERLAY, OFS_back512_PAL_bin, palCD, SIZE_back512_PAL_bin); 
	load_palette(0, palCD, 1);
	cd_loadvram(GPHX_OVERLAY, OFS_back512_DATA_bin, 0x1000, SIZE_back512_DATA_bin);			
	cd_loadvram(GPHX_OVERLAY, OFS_back512_BAT_bin, 0, SIZE_back512_BAT_bin);
#else
	set_map_data(MB512_map, 64, 30);
	set_tile_data(MB_bg);
	load_tile(0x1000);
	load_map(0, 0, 0, 0, 64, 30);
	load_palette(0, MB_pal, 1);  
#endif
	
	Center224in240();
}

#ifndef HELP_OVL
void DrawSP()
{
	// pos x3
	// count x4

	x3 = 0;
	x4 = 0;
	
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
		for(x3 = 0; x3 < 4; x3++)
		{
			spr_make(x4, x3*16+x2, row*16+y2, 0x40*x4+0x5000, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_16x16, 0, 1);
			x4 ++;
		}
	}
}
#endif

unsigned char region;
unsigned char cdrom;
unsigned char *io;


void DetectPCEType()
{
	io = 0x1000;
	region = (((*io) & 0xF0)>>4);
}

// returns 1 if CD detected
void DetectCDROM()
{
	io = 0x1800;
	cdrom = !(*io);
}


void DisplaySystemInfo()
{	
	SetFontColors(13, RGB(2, 4, 7), RGB(7, 7, 7), RGB(1, 3, 7));
	set_font_pal(13);
	
	DetectPCEType();
	DetectCDROM();
#ifdef SGFX
	if(sgx_detect())
	{
		if(cdrom)
		{
			put_string("SuperGrafx+CDROM2", 20, 27);
			left = 20;
		}
		else
			put_string("SuperGrafx", 27, 27);
	}
	else
	{
#endif
		switch(region)
		{
			case	0x0F: // Stand alone original PCE
					{
						put_string("PC Engine", 28, 27);
					}
					break;
			case	0x07: // PCE+IFU30
					{
						put_string("PCE+CDROM2", 27, 27);
						left = 27;
					}
					break;
			case	0x03: // Duo or TG-16
					{
						put_string("Duo|TG-16+CD", 25, 27);
						left = 25;
					}
					break;
			case 	0x0B: // TG-16
					{
						put_string("TG-16", 32, 27);
					}
					break;
		}
#ifdef SGFX
	}
#endif
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
		put_string("AC+", left-3, 27);	
#endif
}

#ifndef HELP_OVL
#ifndef EXT_TOOLS
void Options()
{
    option = 0;
	end = 0;
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
        }
		
		if(refresh)
        {
			RefreshOptions();
            refresh = 0;
        }

        controller = joytrg(0);
		
		if (controller & JOY_SEL)
			end = 1;

		if (controller & JOY_RUN)
		{
#ifdef CDROM1
			if(x_g)
				showHelp(x_g);
			else
#endif
			showHelp(OPTIONS_HELP);
			redraw = 1;
		}
        
        if (controller & JOY_DOWN) 
        {
            option++;
            if(option > 4)
                option = 0;
            refresh = 1;
        }

        if (controller & JOY_UP) 
        {
            option--;
            if(option < 0)
                option = 4;
            refresh = 1;
        }
		
				
		if (controller & JOY_II)
			end = 1;
		
		if (controller & JOY_I)
		{
			i = ExecuteOptions();
			if(i == 1)
				redraw = 1;
			if(i == 2)
				refresh = 1;
			if(i == 3)
				end = 1;
		}
    }
}

void RedrawOptions()
{
	Rewdraw512Menu();
	
	SetFontColors(11, RGB(3, 3, 3), RGB(0, 6, 0), 0);
	SetFontColors(12, RGB(3, 3, 3), RGB(4, 4, 4), 0);
	SetFontColors(13, RGB(3, 3, 3), RGB(5, 5, 5), 0);
}

void RefreshOptions()
{
	row = 14;
            
	set_font_pal(11);
	put_string("Video Options", 23, 10);
			
	set_font_pal(option == 0 ? 15 : 14);
	put_string("Vertical Resolution:", HPOS+1, row);
	if(Enabled240p)
		put_string("240p", HPOS+26, row);
	else
		put_string("224p", HPOS+26, row);
	row++;
	
	if(Enabled240p)
		set_font_pal(option == 1 ? 15 : 14);
	else
		set_font_pal(option == 1 ? 13 : 12);
	put_string("Start at line:", HPOS+3, row);
	if(UseDefault)
		put_string("24 [Standard use in games]", HPOS+26, row);
	else
		put_string("22 [Full 240 visible lines]", HPOS+26, row);
	
	row++;
	
	set_font_pal(option == 2 ? 15 : 14);
	put_string("Composite filter:", HPOS+1, row);
	if(EnabledSoft)
		put_string("On ", HPOS+26, row);
	else
		put_string("Off", HPOS+26, row);
	row++;
	
	
	set_font_pal(option == 3 ? 15 : 14);
	put_string("Composite B&W:", HPOS+1, row);
	if(Enabled_C_BW)
		put_string("On ", HPOS+26, row);
	else
		put_string("Off", HPOS+26, row);
	row++;
	
	set_font_pal(option == 4 ? 15 : 14);

	put_string("Back to Main Menu", HPOS+1, ++row);
}

int ExecuteOptions()
{	
	switch(option)
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
			x3 = 1;	
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
				x3 = 1;	
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
			
			x3 = 2;
			
			Set512H();
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
			x3 = 2;
			
			Set512H();
			break;
		case 4:
			x3 = 3;
			break;
	}
	return x3;
}

#endif
#endif