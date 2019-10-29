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
	sel = 0;
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
	DrawIntro();
#endif

    while(1)
    {   	
		vsync();
	
#ifdef CDROM1
		if(!HelpItem && !ToolItem)
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
            RefreshMain();

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
            if(sel > 6)
                sel = 0;
            refresh = 1;
        }

        if (controller & JOY_UP) 
        {
            sel--;
            if(sel < 0)
                sel = 6;
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
			if(HelpItem)
			{
				if(HelpItem < PATTERNSHELP)
				{
					sel = HelpItem;
					HelpItem = 0;
					if(HelpItem < PATTERNSHELP)
						controller = JOY_I;
				}
				else
				{
					if(HelpItem < AUDIOHELP)
						sel = 1;
					else if(HelpItem < HARDWAREHELP)
						sel = 2;
					else
						sel = 3;
					controller = JOY_I;
				}
			}
			
			if(ToolItem)
			{
				sel = 0;
				if(ToolItem < SOUNDTOOL)
					sel = 1;
				else if(ToolItem < HARDWARETOOL)
					sel = 2;
				else
					sel = 3;
				
				controller = JOY_I;
			}
		}
#endif
		
		if (controller & JOY_I)
		{
			disp_off();
			ResetVideo();
			switch(sel)
			{
#ifdef CDROM1
				prev_select = sel;
#endif
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
					VideoTests();
					break;
				case 2:
					AudioTests();
					break;
				case 3:
					break;
				case 4:
#ifdef CDROM1
					x_g = 0;
#endif
					Options();
					break;
				case 5:
					showHelp(GENERAL_HELP);
					break;
				case 6:
					DrawCredits();
					break;
			}
#ifdef CDROM1	
			if(prev_select)
			{
				sel = prev_select;
				prev_select = 0;
			}
#endif
			redraw = 1;			
			disp_off();
		}
    }
}

void RefreshMain()
{
	row = 11;
	
	set_font_pal(sel == 0 ? 15 : 14);
	put_string("Test Patterns", HPOS, row++);
	set_font_pal(sel == 1 ? 15 : 14);
	put_string("Video tests", HPOS, row++);
	set_font_pal(sel == 2 ? 15 : 14);
	put_string("Audio tests", HPOS, row++);
	set_font_pal(sel == 3 ? 15 : 14);
	put_string("Hardware tools", HPOS, row++);
	
	row = 19;
	
	set_font_pal(sel == 4 ? 15 : 14);
	put_string("Options", HPOS, row++);
	set_font_pal(sel == 5 ? 15 : 14);
	put_string("Help", HPOS, row++);
	set_font_pal(sel == 6 ? 15 : 14);
	put_string("Credits", HPOS, row++);
}

/*
 *
 *		Video Menu
 *
 *
 */
 

void RefreshVideoTests()
{
	//set_font_pal(12);
	//put_string("Video Tests", 14, 6);
	
	row = 9;

	set_font_pal(sel == 0 ? 15 : 14);
	put_string("Drop Shadow Test", HPOS, row++);
	set_font_pal(sel == 1 ? 15 : 14);
	put_string("Striped Sprite Test", HPOS, row++);
	set_font_pal(sel == 2 ? 15 : 14);
	put_string("Lag Test", HPOS, row++);
	set_font_pal(sel == 3 ? 15 : 14);
	put_string("Manual Lag Test", HPOS, row++);
	set_font_pal(sel == 4 ? 15 : 14);
	put_string("Scroll Test", HPOS, row++);
	set_font_pal(sel == 5 ? 15 : 14);
	put_string("Grid Scroll Test", HPOS, row++);
	
	RefreshVideoTestsAux(row);
}

void RefreshVideoTestsAux(int row)
{
	set_font_pal(sel == 6 ? 15 : 14);
	put_string("Horiz/Vert Stripes", HPOS, row++);
	set_font_pal(sel == 7 ? 15 : 14);
	put_string("Checkerboard", HPOS, row++);
	set_font_pal(sel == 8 ? 15 : 14);
	put_string("Backlit Zone Test", HPOS, row++);
	
	set_font_pal(sel == 9 ? 15 : 14);
	put_string("Options", HPOS, ++row);
	
	set_font_pal(sel == 10 ? 15 : 14);
	put_string("Help", HPOS, ++row);
	
	set_font_pal(sel == 11 ? 15 : 14);
	put_string("Back to Main Menu", HPOS, ++row);
}


void VideoTests()
{
	redraw = 1;
	refresh = 1;
	
	sel = 0;
	end = 0;

	controller = 0;
	disp_off();
	while(!end)
	{		
		vsync();
		
#ifdef CDROM1
		if(!HelpItem && !ToolItem)
		{
#endif
		if(redraw)
		{
			RedrawMain();

			refresh = 1;
			redraw = 0;
			disp_on();
		}
		
		if(refresh)
		{	
			RefreshVideoTests();
			refresh = 0;
		}

		controller = joytrg(0);
		
		if (controller & JOY_II)
			end = 1;
		
		if (controller & JOY_SEL)
		{
#ifdef CDROM1
			x_g = OPTIONS_VID_HELP;
#endif
			Options();
			redraw = 1;
		}
		
		if (controller & JOY_DOWN) 
		{
			sel++;
			if(sel > 11)
				sel = 0;
			refresh = 1;
		}

		if (controller & JOY_UP) 
		{
			sel--;
			if(sel < 0)
				sel = 11;
			refresh = 1;
		}
		
		if (controller & JOY_RUN)
		{
			showHelp(GENERAL_VID_HELP);
			redraw = 1;
		}
#ifdef CDROM1
		}
		else
		{
			if(HelpItem)
			{
				sel = HelpItem - VIDEOHELP;
				if(HelpItem != GENERAL_VID_HELP)
					controller = JOY_I;
				HelpItem = 0;
			}
			
			if(ToolItem)
			{
				sel = ToolItem - VIDEOTOOL;
				ToolItem = 0;
			}
		}
#endif
		
		if (controller & JOY_I)
		{
			disp_off();
			ResetVideo();
#ifdef CDROM1
			prev_select = sel;
#endif
			switch(sel)
			{
				case 0:
					DropShadow();
					break;
				case 1:
					StripedSprite();
					break;
				case 2:
					LagTest();
					break;
				case 3:
					ManualLagTest();
					break;
				case 4:
					ScrollTest();
					break;
				case 5:
					VScrollTest();
					break;
				case 6:
					DrawStripes();
					break;
				case 7:
					DrawCheck();
					break;
				case 8:
					LEDZoneTest();
					break;			
				case 9:
#ifdef CDROM1
					x_g = OPTIONS_VID_HELP;
#endif
					Options();
					break;
				case 10:
					showHelp(GENERAL_VID_HELP);
					break;
				case 11:
					end = 1;
					break;
			}
			if(sel != 11)
				end = 0;
				
			redraw = 1;	
			disp_off();
			
#ifdef CDROM1	
			sel = prev_select;
			prev_select = 0;
#endif
		}
	}
	end = 0;
#ifdef CDROM1
	prev_select = 1;
#else
	sel = 1;
#endif
}

/*
 *
 *		Audio Menu
 *
 *
 */
 

void RefreshAudioTests()
{
	//set_font_pal(12);
	//put_string("Audio Tests", 14, 6);
	
	row = 12;

	set_font_pal(sel == 0 ? 15 : 14);
	put_string("Sound Test", HPOS, row++);
	set_font_pal(sel == 1 ? 15 : 14);
	put_string("Audio Sync Test", HPOS, row++);
	set_font_pal(sel == 2 ? 15 : 14);
	put_string("MDFourier", HPOS, row++);
	
	set_font_pal(sel == 3 ? 15 : 14);
	put_string("Options", HPOS, ++row);
	set_font_pal(sel == 4 ? 15 : 14);
	put_string("Help", HPOS, ++row);
	
	set_font_pal(sel == 5 ? 15 : 14);
	put_string("Back to Main Menu", HPOS, ++row);
}


void AudioTests()
{
	redraw = 1;
	refresh = 1;
	
	sel = 0;
	end = 0;
	controller = 0;
	
	disp_off();
	while(!end)
	{		
		vsync();
		
#ifdef CDROM1
		if(!HelpItem && !ToolItem)
		{
#endif
		if(redraw)
		{
			RedrawMain();

			refresh = 1;
			redraw = 0;
			disp_on();
		}
		
		if(refresh)
		{	
			RefreshAudioTests();
			refresh = 0;
		}

		controller = joytrg(0);
		
		if (controller & JOY_II)
			end = 1;
		
		if (controller & JOY_SEL)
		{
#ifdef CDROM1
			x_g = OPTIONS_AUD_HELP;
#endif
			Options();
			redraw = 1;
		}
		
		if (controller & JOY_DOWN) 
		{
			sel++;
			if(sel > 5)
				sel = 0;
			refresh = 1;
		}

		if (controller & JOY_UP) 
		{
			sel--;
			if(sel < 0)
				sel = 5;
			refresh = 1;
		}
		
		if (controller & JOY_RUN)
		{
			showHelp(GENERAL_AUD_HELP);
			redraw = 1;
		}
#ifdef CDROM1
		}
		else
		{
			if(HelpItem)
			{
				sel = HelpItem - AUDIOHELP;
				if(HelpItem != GENERAL_AUD_HELP)
					controller = JOY_I;
				HelpItem = 0;
			}
			if(ToolItem)
			{
				sel = ToolItem - SOUNDTOOL;
				ToolItem = 0;
			}
		}
#endif
		
		if (controller & JOY_I)
		{
			disp_off();
			ResetVideo();
#ifdef CDROM1
			prev_select = sel;
#endif
			switch(sel)
			{
				case 0:
					SoundTest();
					break;
				case 1:
					AudioSyncTest();
					break;
				case 2:
					MDFourier();
					break;
				case 3:
#ifdef CDROM1
					x_g = OPTIONS_AUD_HELP;
#endif
					Options();
					break;
				case 4:
					showHelp(GENERAL_AUD_HELP);
					break;
				case 5:
					end = 1;
					break;
			}
			if(sel != 5)
				end = 0;
				
			redraw = 1;	
			disp_off();
			
#ifdef CDROM1	
			sel = prev_select;
			prev_select = 0;
#endif
		}
	}
	end = 0;
#ifdef CDROM1
	prev_select = 2;
#else
	sel = 2;
#endif
}

/*
 *
 *		Menu Functions
 *
 *
 */
 
 
void DrawN()
{
	end = 1;
	redraw = 1;
    do
    {   
		vsync();
		
		end = 1;
        if(redraw)
        {
			ResetVideo();
			Set256H();
#ifndef CDROM1
			scroll(0, -26, -47, 0, 240, 0xC0);		
			load_background(n_bg, n_pal, n_map, 25, 18);
#else
			scroll(0, 0, -32, 0, 240, 0xC0);
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
	end = 0;
}

void DrawCredits()
{	
	end = 0;
	redraw = 1;
	refresh = 1;
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {	
			Rewdraw512Menu();
			
			SetFontColors(13, RGB(3, 3, 3), RGB(1, 6, 6), 0);
			SetFontColors(15, RGB(3, 3, 3), RGB(0, 6, 0), 0);
			
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
		if(i == 1)
			put_string("Artemio Urbina      ", HPOS+2, 7);
		if(i == 60*4)
			put_string("@Artemio (twitter)  ", HPOS+2, 7);
		if(i == 60*8)
			put_string("aurbina@junkerhq.net", HPOS+2, 7);
		if(i == 60*16)
			i = 0;
			
		i++;

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

void RefreshCredits()
{
	row = 6;
			
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
	put_string("shmups regulars", HPOS+2, row++);
	row++;
	
	set_font_pal(15);
	put_string("Info on using this suite:", HPOS+2, row++);
	set_font_pal(14);
	put_string("http://junkerhq.net/240p/", HPOS+2, row++);
	set_font_pal(13);
	put_string("This is free software and is open source under GPL.", HPOS+1, row++);

	set_font_pal(14);	
	put_string("Ver. 1.06", 50, 7);
	put_string("01/08/2019", 49, 8);
	
#ifdef CDROM
	x = cd_getver() >> 8;
	put_string("System Card", 46, 9);
	put_number(x, 1, 58, 9);
#endif

	set_font_pal(15);
	put_string("Dedicated to Elisa", 40, 24);
	
	i = 1;
}

#ifndef CDROM1
#ifndef SCDROM

void DrawIntro()
{	
	disp_off();
	ResetVideo();
	setupFont();
	
	vsync();
	set_color(1, 0);
	SetFontColors(14, 0, 0, 0);
	put_string("KORDAMP PRESENTS", 12, 14);
	disp_on();
	for(i = 0; i < 7; i ++)
	{
		SetFontColors(14, 0, RGB(i, i, i), 0);
		vsync(3);
	}
	
	for(i = 7; i > 0; i --)
	{
		SetFontColors(14, 0, RGB(i, i, i), 0);
		vsync(3);
	}
	disp_off();
}

#endif
#endif