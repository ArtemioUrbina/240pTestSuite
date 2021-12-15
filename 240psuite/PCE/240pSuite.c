/* 
 * 240p Test Suite
 * Copyright (C)2014-2021 Artemio Urbina (PC Engine/TurboGrafx-16)
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

void RefreshCredits();
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

#ifdef CDROM
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

#ifdef MDF_AT_BOOT
#ifdef CDROM
	runmdf = 2;
#endif
	MDFourier(2);
#endif

#ifndef CDROM
	DrawIntro();
#endif

    while(1)
    {   	
		vsync();
	
#ifdef CDROM
		if(!HelpItem && !ToolItem)
		{
#endif

        if(redraw)
        {
			RedrawMain();
            redraw = 0;
			refresh = 1;
			disp_sync_on();
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
#ifdef CDROM
		}
		else
		{
			if(HelpItem)
			{
				if(HelpItem < PATTERNSHELP)
				{
					sel = HelpItem;
					HelpItem = 0;
					if(sel != 5 && HelpItem < PATTERNSHELP)
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
#ifdef CDROM
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
					HardwareTests();
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
	drawmenutext(0, "Test Patterns");
	drawmenutext(1, "Video tests");
	drawmenutext(2, "Audio tests");
	drawmenutext(3, "Hardware tools");
	
	row = 19;
	DrawMenuBottom(4, 1);
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

	drawmenutext(0, "Drop Shadow Test");
	drawmenutext(1, "Striped Sprite Test");
	drawmenutext(2, "Lag Test");
	drawmenutext(3, "Timing & Reflex Test");
	drawmenutext(4, "Scroll Test");
	drawmenutext(5, "Grid Scroll Test");
	drawmenutext(6, "Vertical Stripes");
	drawmenutext(7, "Horizontal Stripes");
	drawmenutext(8, "Checkerboard");
	drawmenutext(9, "Backlit Zone Test");
	row++;
	DrawMenuBottom(10, 0);
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
		
#ifdef CDROM
		if(!HelpItem && !ToolItem)
		{
#endif
		if(redraw)
		{
			RedrawMain();

			refresh = 1;
			redraw = 0;
			disp_sync_on();
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
#ifdef CDROM
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
#ifdef CDROM
			prev_select = sel;
#endif
			switch(sel)
			{
				case 0:
					DropShadow(0);
					break;
				case 1:
					DropShadow(1);
					break;
				case 2:
					LagTest();
					break;
				case 3:
					ReflexTest();
					break;
				case 4:
					ScrollTest();
					break;
				case 5:
					VScrollTest();
					break;
				case 6:
					DrawStripes(1);
					break;
				case 7:
					DrawStripes(0);
					break;
				case 8:
					DrawCheck();
					break;
				case 9:
					LEDZoneTest();
					break;			
				case 10:
#ifdef CDROM1
					x_g = OPTIONS_VID_HELP;
#endif
					Options();
					break;
				case 11:
					showHelp(GENERAL_VID_HELP);
					break;
				case 12:
					end = 1;
					break;
			}
			if(sel != 12)
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

	drawmenutext(0, "Sound Test");
	drawmenutext(1, "Audio Sync Test");
	drawmenutext(2, "MDFourier");
	drawmenutext(3, "Audio Clipping");
	
	row++;
	DrawMenuBottom(4, 0);
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
		
#ifdef CDROM
		if(!HelpItem && !ToolItem)
		{
#endif
		if(redraw)
		{
			RedrawMain();

			refresh = 1;
			redraw = 0;
			disp_sync_on();
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
#ifdef CDROM
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
					MDFourier(0);
					break;
				case 3:
					ConstrInterf();
					break;
				case 4:
#ifdef CDROM1
					x_g = OPTIONS_AUD_HELP;
#endif
					Options();
					break;
				case 5:
					showHelp(GENERAL_AUD_HELP);
					break;
				case 6:
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
			scroll(0, -48, -72, 0, 240, 0xC0);		
#ifndef CDROM1
			load_background(n_bg, n_pal, n_map, 20, 12);
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
			disp_sync_on();
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
			disp_sync_on();
        }
		
		if(refresh)
		{
			RefreshCredits();

			refresh = 0;
		}
		
		set_font_pal(14);
		if(i == 1)
			put_string("Artemio Urbina      ", HPOS+2, CREDIT_START+1);
		if(i == 60*4)
			put_string("@Artemio (twitter)  ", HPOS+2, CREDIT_START+1);
		if(i == 60*8)
			put_string("aurbina@junkerhq.net", HPOS+2, CREDIT_START+1);
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
	row = CREDIT_START;
			
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
	put_string("Monoscope:", HPOS+2, row++);
	set_font_pal(14);
	put_string("Keith Raney", HPOS+2, row++);
	
	set_font_pal(15);
	put_string("Donna Art by:", HPOS+25, row-2);
	set_font_pal(14);
	put_string("Jose Salot (@pepe_salot)", HPOS+25, row-1);
	row++;
	
	set_font_pal(15);
	put_string("Menu Pixel Art:", HPOS+2, row++);
	set_font_pal(14);
	put_string("Asher", HPOS+2, row++);
	
	set_font_pal(15);
	put_string("Advisor:", HPOS+25, row-2);
	set_font_pal(14);
	put_string("Fudoh", HPOS+25, row-1);
	row++;
	
	set_font_pal(15);
	put_string("Collaboration:", HPOS+2, row++);
	set_font_pal(14);
	put_string("shmups regulars", HPOS+2, row++);
	
	set_font_pal(15);
	put_string("Info on using this suite:", HPOS+25, row-2);
	set_font_pal(14);
	put_string("http://junkerhq.net/240p/", HPOS+25, row-1);
	
	
	set_font_pal(13);
	put_string("This is free software and is open source under GPL.", HPOS+1, 23);

	set_font_pal(13);
	put_string("Ver. 1.12", 50, 6);
	set_font_pal(14);
	put_string("12/09/2021", 49, 7);
	
#ifdef CDROM
	x = cd_getver() >> 8;
	put_string("System Card", 46, 8);
	put_number(x, 1, 58, 8);
#endif

	set_font_pal(15);
	put_string("Dedicated to Elisa", 40, 24);
	
	i = 1;
}

#ifndef CDROM1
#ifndef SCDROM

void CheckStart(int sync)
{
	vsync(sync);
	controller = joy(0);
	if(controller & JOY_RUN)
		runmdf = 1;
}

void DrawIntro()
{	
	disp_off();
	ResetVideo();
	setupFont();
	
	CheckStart(1);
	set_color(1, 0);
	SetFontColors(14, 0, 0, 0);
	put_string("KORDAMP PRESENTS", 12, 14);
	disp_sync_on();
	for(i = 0; i < 7; i ++)
	{
		SetFontColors(14, 0, RGB(i, i, i), 0);
		CheckStart(3);
	}
	
	for(i = 7; i > 0; i --)
	{
		SetFontColors(14, 0, RGB(i, i, i), 0);
		CheckStart(3);
	}
	disp_off();
	if(runmdf)
		MDFourier(1);
}

#endif
#endif