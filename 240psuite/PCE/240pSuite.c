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
#ifdef SGFX
#include <sgx.h>
#endif

void DrawCredits();
void DisplaySystemInfo();
void DrawIntro();


#ifdef CDROM1
char palCD[512];
#endif

#define HPOS 5

void main()
{
    int sel = 0;

	redraw = 1;
	refresh = 1;
	

#ifndef CDROM
	xres_flags = XRES_SOFT;
	Enabled240p = 1;
	UseDefault = 0;
	EnabledSoft = 1;
	Enabled_C_BW = 0;
#endif

#ifdef CDROM
	RestoreGlobals();
#endif

#ifdef CDROM1	
	if(prev_select)
	{
		sel = prev_select;
		prev_select = 0;
	}
#endif

	disp_off();
	Set320H();

	if(Enabled240p)
		Set240p();

#ifndef CDROM
	disp_on();
	DrawIntro();
	disp_off();
#endif

    while(1)
    {   	
		vsync();
	
#ifdef CDROM1
		if(!HelpItem)
		{
#endif

        if(redraw)
        {
			RedrawMain();
            redraw = 0;
			refresh = 1;
			disp_on();
        }
		
		set_font_pal(15);
		
		if(refresh)
        {
            RefreshMain(sel);

            refresh = 0;
        }

        controller = joytrg(0);
		
		if (controller & JOY_SEL)
		{
#ifdef CDROM1
			x_g = 0;
#endif
			Options();
			redraw = 1;
		}
        
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
		
		if (controller & JOY_RUN)
		{
			showHelp(GENERAL_HELP);
			redraw = 1;
		}
#ifdef CDROM1
		}
		else
		{
			if(HelpItem <= OPTIONS_HELP)
			{
				sel = HelpItem;
				controller = JOY_I;
			}
				
			HelpItem = 0;
		}
#endif
		
		if (controller & JOY_I)
		{
			disp_off();
			ResetVideo();
			switch(sel)
			{
				case 0:
#ifndef CDROM1
					TestPatterns();
#else
					xres_flags_g = xres_flags;
					Enabled240p_g = Enabled240p;
					UseDefault_g = UseDefault;
					EnabledSoft_g = EnabledSoft;
					Enabled_C_BW_g = Enabled_C_BW;
					
					set_font_pal(14);
					put_string("Loading...", 27, 26);
					cd_execoverlay(PATTERNS_OVERLAY);
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
				case 4:
					ManualLagTest();
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
					AudioSyncTest();
					break;
				case 12:
#ifdef CDROM1
					prev_select = sel;
#endif
					Options();
					break;
				case 13:
#ifdef CDROM1
					prev_select = sel;
#endif
					showHelp(GENERAL_HELP);
					break;
				case 14:
					DrawCredits();
					break;
			}
			redraw = 1;			
			disp_off();
		}
    }
}

void RedrawMain()
{
	ResetVideo();
	setupFont();

#ifndef CDROM1
	set_tile_data(MB_bg);
	load_tile(0x1000);
	load_palette(0, MB_pal, 1);  
	set_map_data(MB_map, 40, 30);
	load_map(0, 0, 0, 0, 40, 30);	
#ifdef SGFX
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
#else
	set_screen_size(SCR_SIZE_64x32); 
	cd_loaddata(GPHX_OVERLAY, OFS_mainbg_PAL_bin, palCD, SIZE_mainbg_PAL_bin); 
	load_palette(0, palCD, 1); 
	cd_loadvram(GPHX_OVERLAY, OFS_mainbg_DATA_bin, 0x1000, SIZE_mainbg_DATA_bin);
	cd_loadvram(GPHX_OVERLAY, OFS_mainbg_BAT_bin, 0x0000, SIZE_mainbg_BAT_bin);
#endif
   
	init_satb();
	DrawSP();
	satb_update();
	
	Center224in240();
	DisplaySystemInfo();		
}

void RefreshMain(int sel)
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
	
	RefreshMainAux(sel, row);
}

void RefreshMainAux(int sel, int row)
{
	set_font_pal(sel == 7 ? 15 : 14);
	put_string("Horizontal Stripes", HPOS, row++);
	set_font_pal(sel == 8 ? 15 : 14);
	put_string("Checkerboard", HPOS, row++);
	set_font_pal(sel == 9 ? 15 : 14);
	put_string("Backlit Zone Test", HPOS, row++);
	set_font_pal(sel == 10 ? 15 : 14);
	put_string("Sound Test", HPOS, row++);
	set_font_pal(sel == 11 ? 15 : 14);
	put_string("Audio Sync Test", HPOS, row++);
	
	set_font_pal(sel == 12 ? 15 : 14);
	put_string("Video Options", HPOS, ++row);
	
	set_font_pal(sel == 13 ? 15 : 14);
	put_string("Help", HPOS, ++row);
	set_font_pal(sel == 14 ? 15 : 14);
	put_string("Credits", HPOS, ++row);
}


/*
 *
 *		Menu Functions
 *
 *
 */
 
 
void DrawN()
{
	unsigned char end = 1;

	redraw = 1;
    do
    {   
		vsync();
		
		end = 1;
        if(redraw)
        {
			ResetVideo();
			Set256H();
			scroll(0, 0, -32, 0, 240, 0xC0);
#ifndef CDROM1
			load_background(n_bg, n_pal, n_map, 32, 20);
#else
			set_screen_size(SCR_SIZE_32x32); 
			cd_loaddata(GPHX_OVERLAY, OFS_N_PAL_bin, palCD, SIZE_N_PAL_bin); 
			load_palette(0, palCD, 16); 
			cd_loadvram(GPHX_OVERLAY, OFS_N_DATA_bin, 0x1000, SIZE_N_DATA_bin);
			cd_loadvram(GPHX_OVERLAY, OFS_N_BAT_bin, 0, SIZE_N_BAT_bin);
			// Why these get zapped to 0 when in CDROM1...
			RestoreGlobals();
#endif
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
	unsigned char end = 0;
	int counter = 0;

	redraw = 1;
	refresh = 1;
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			ResetVideo();
			setupFont();
			
			SetFontColors(15, RGB(3, 3, 3), RGB(0, 6, 0), 0);
			SetFontColors(13, RGB(3, 3, 3), RGB(1, 6, 6), 0);

			Set512H();
			
#ifndef CDROM1			
			set_map_data(MB512_map, 64, 30);
			set_tile_data(MB512_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 64, 30);
			load_palette(0, MB512_pal, 1);  
#else		
			set_screen_size(SCR_SIZE_64x32); 
			cd_loaddata(GPHX_OVERLAY, OFS_back512_PAL_bin, palCD, SIZE_back512_PAL_bin); 
			load_palette(0, palCD, 1);
			cd_loadvram(GPHX_OVERLAY, OFS_back512_DATA_bin, 0x1000, SIZE_back512_DATA_bin);			
			cd_loadvram(GPHX_OVERLAY, OFS_back512_BAT_bin, 0, SIZE_back512_BAT_bin);
#endif
			
			Center224in240();
			
            redraw = 0;
			refresh = 1;
			disp_on();
        }
		
		if(refresh)
		{
			RefreshCredits();

			refresh = 0;
		}
		
		set_font_pal(14);
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

//#define DISTRIBUTE

void RefreshCredits()
{
	row = 7;
			
	set_font_pal(15);
	put_string("Code and Patterns:", HPOS+2, row++);
	set_font_pal(14);
	row+=2;
	
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
	put_string("This is free software and is open source under GPL.", HPOS+1, row++);
#ifdef DISTRIBUTE
	put_string("To be distributed for free", HPOS+1, row++);
#endif

	set_font_pal(14);	
	put_string("Ver. 1.05", 50, 7);
	put_string("24/02/2019", 49, 8);
	
#ifdef CDROM
	x = cd_getver() >> 8;
	put_string("System Card", 46, 9);
	put_number(x, 1, 58, 9);
#endif

	set_font_pal(15);
	put_string("Dedicated to Elisa", 40, 25);
	
}

#ifndef CDROM1
#ifndef SCDROM

void DrawIntro()
{
	unsigned char frame;
	
	ResetVideo();
	setupFont();
	
	vsync();
	set_color(1, 0);
	SetFontColors(14, 0, 0, 0);
	put_string("KORDAMP PRESENTS", 12, 14);
	for(frame = 0; frame < 7; frame ++)
	{
		SetFontColors(14, 0, RGB(frame, frame, frame), 0);
		vsync(3);
	}
	
	frame = 5;
	while(frame)
		frame --;
	
	for(frame = 7; frame > 0; frame --)
	{
		SetFontColors(14, 0, RGB(frame, frame, frame), 0);
		vsync(3);
	}
}

#endif
#endif