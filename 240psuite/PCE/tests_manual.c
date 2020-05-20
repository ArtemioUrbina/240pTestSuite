 /*
 * 240p Test Suite
 * Copyright (C)2014-2019 Artemio Urbina (PC Engine/TurboGrafx-16)
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

#ifdef SCDROM2
#include "huc.h"
#include "res.h"
#include "font.h"
#include "video.h"
#include "tests.h"
#include "help.h"
#include "tools.h"
#include "float.h"
#include "tests_audio_aux.c"

void main()
{
	xres_flags = xres_flags_g;
	Enabled240p = Enabled240p_g;
	UseDefault = UseDefault_g;
	EnabledSoft = EnabledSoft_g;
	Enabled_C_BW = Enabled_C_BW_g;
	
	ManualLagTest();
	
	ToolItem = TOOL_MANUAL;
	cd_execoverlay(MAIN_OVERLAY);
}

#endif


void RedrawManualLagTest()
{
	ResetVideo();
	setupFont();
	SetFontColors(13, 0, RGB(0, 7, 0), 0);
	SetFontColors(14, 0, RGB(7, 7, 7), 0);
	SetFontColors(15, 0, RGB(7, 0, 0), 0);

#ifndef CDROM1			
	set_map_data(fs_map, 64, 32);
	set_tile_data(white_bg);
	load_tile(0x1000);
	load_map(0, 0, 0, 0, 64, 32);
	set_color_rgb(1, 0, 0, 0);   
#else
#endif
	ManualLagTestSprites();
	ManualLagTestText();
}

void ManualLagTestSprites()
{
	init_satb();
	set_color_rgb(256, 0, 0, 0); 
	set_color_rgb(257, 7, 7, 7); 
	set_color_rgb(273, 7, 0, 0); 
	set_color_rgb(289, 0, 7, 0);

#ifndef CDROM1		
	load_vram(0x5000, lagspr_sp, 0x100);
#else
	cd_loadvram(GPHX_OVERLAY, OFS_lagspr_tile_bin, 0x5000, SIZE_lagspr_tile_bin);
#endif
	spr_make(0, x, 300, 0x5000, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_32x32, 0, 1);
	spr_make(1, x, 300, 0x5000, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_32x32, 0, 1);
	spr_make(2, x,  96, 0x5000, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_32x32, 0, 2);
	
	satb_update();
}


void ManualLagTestText()
{
	set_font_pal(13);
	put_string("Press \"I\" when the sprite is aligned", 2, 21);
	put_string("with the background.", 3, 22);
	put_string("Negative values mean you pressed \"I\"", 2, 23);
	put_string("before they intersected.", 3, 24);
	put_string("SELECT toggles horizontal and vertical movement.", 2, 25);
	put_string("movement.", 3, 26);
	put_string("D-pad up toggles audio feedback.", 2, 27);
	put_string("D-pad down toggles rhythmic timing.", 2, 28);
		
	Center224in240();
}

void ManualLagTestResults()
{	
	redraw = 1;
	end = 0;
	x = 0;

	y = 0;	 // val
	x3 = 0;  // total	
	x4 = 0;  // x3ms
	
	while(!end)
	{   	
		vsync();

		if(redraw)
		{
			ManualLagTestResultsBack();
			
			set_font_pal(14);
			for(x2 = 0; x2 < 10; x2++)
			{
				y = clicks[x2];
				if(y != 0xFF)
				{
					put_number(y, 2, 10, 8+x2); 
					if(y >= 0)
					{
						x3 += y;
						x ++;
					}
				}
			}
			
			set_font_pal(15);
			put_string("+", 8, 14);
			put_string("----", 8, 18);
			
			x4 = x3/x;
			
			set_font_pal(14);
			put_number(x3, 7, 5, 19);
			put_string("/", 12, 19);
			put_number(x, 2, 13, 19);
			set_font_pal(15);
			put_string("=", 15, 19);
			set_font_pal(14);
			put_number(x4, 2, 16, 19);
			put_string("frames", 19, 19);
			if(x4 == 1)
				put_string(" ", 24, 19);
			x4 = x3/x*16;
			put_number(x4, 2, 16, 20);
			put_string("milliseconds", 19, 20);
			
			set_font_pal(13);
			put_string("Keep in mind that a frame is", 6, 21);
			put_string("16.67 milliseconds.", 6, 22);
			
			if(x3 == 10)
			{
				x = 1;
				for(x2 = 0; x2 < 10; x2++)
				{
					if(clicks[x2] != 1)
						x = 0;
				}
				if(x)
					put_string("Smells like turbo...", 14, 13);
			}
			if(x3 < 5)
				put_string("EXCELLENT REFLEXES!", 14, 13);
			if(x3 == 0)
				put_string("INCREDIBLE REFLEXES!", 14, 13);
			
			redraw = 0;
			disp_on();
		}
		
		controller = joytrg(0);
		
		if (controller & JOY_II)
			end = 1;
	}
}

void ManualLagTestResultsBack()
{
	RedrawBG();
	SetFontColors(13, RGB(3, 3, 3), RGB(0, 7, 0), 0);
}

void ManualLagTest()
{
	option = 0;
	end = 0;

#ifndef CDROM1			
	showHelp(MANUALLAG_HELP);
#endif
	
	speed = 1;
	
	variation = 1;
	change = 1;
	audio = 1;
	view = 0;
	vary = 0;
	
	refresh = 0;
	redraw = 1;
	srand(clock_tt());
	
	for(x2 = 0; x2 < 10; x2++)
		clicks[x2] = 0xFF;
	
	x = 144;
	y = 60;
	x2 = 108;
	y2 = 96;
	
	LoadWave(0, sine1x);
	
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			RedrawManualLagTest();
            redraw = 0;
			refresh = 1;
			disp_on();
        }
		
		if(refresh)
		{
			RefreshManualLagTest();
			refresh = 0;
		}
		
		if(audio) // n more that one frame with audio
			StopAudio(0);
				
		if(y == 96) // remove full screen flash
			set_color_rgb(1, 0, 0, 0);
		
		controller = joytrg(0);
		
		if (controller & JOY_I)
		{
			if(change)
			{
				clicks[option] = (y - 96) *speed;
				
				if(audio && clicks[option] != 0)
				{
					SetWaveFreq(0, 224);
					PlayCenter(0);
				}
	
				if(clicks[option] >= 0)
				{
					change = 0;
					option ++;
				}
		
				if(option > 9)
					end = 1;
			}
		}
		
		if (controller & JOY_RUN)
		{
			showHelp(MANUALLAG_HELP);
			redraw = 1;
		}
        
		if (controller & JOY_II)
			end = 1;
			
		if (controller & JOY_SEL)
		{
			view ++;
			if(view > 2)
				view = 0;
			if(view == 0)
			{
				spr_set(1);
				spr_y(300);
			}
			if(view == 1)
			{
				spr_set(0);
				spr_y(300);
			}
		}
		
		if (controller & JOY_UP)
		{
			audio = !audio;
			refresh = 1;
		}
		
		if (controller & JOY_DOWN)
		{
			variation = !variation;
			refresh = 1;
			if(!variation)
				vary = 0;
		}
		
		ManualLagTestClickRefresh();
		
		if(y > 132 + vary)
		{
			speed = -1;
			change = 1;
			if(variation)
			{
				if(random(2))
					vary = random(7);
				else
					vary = -1 * random(7);
			}
		}

		if(y < 60 + vary)
		{
			speed = 1;
			change = 1;
			if(variation)
			{
				if(random(2))
					vary = random(7);
				else
					vary = -1 * random(7);
			}
		}
		
		y += speed;
		x2 += speed;
		
		if(view == 0 || view == 2)
		{
			spr_set(0);
			spr_x(x);
			spr_y(y);
		}
		
		if(view == 1 || view == 2)
		{
			spr_set(1);
			spr_x(x2);
			spr_y(y2);
		}
		
		if(y == 96)
		{			
			if(audio)
			{
				SetWaveFreq(0, 112);
				PlayCenter(0);
			}
			
			spr_set(0);
			spr_pal(1);
			
			spr_set(1);
			spr_pal(1);
			set_color_rgb(1, 7, 7, 7);   
		}
		else
		{
			if(y == 97 || y == 95) // one pixel off
			{
				//StopAudio(0);
				
				spr_set(0);
				spr_pal(2);
			
				spr_set(1);
				spr_pal(2);
			}

			if(y == 98 || y == 94) // two pixels off
			{
				spr_set(0);
				spr_pal(0);
			
				spr_set(1);
				spr_pal(0);
			}
		}
		
		satb_update();
    }
	
	StopAudio(0);
	
	if(option > 9)
		ManualLagTestResults();
}

void RefreshManualLagTest()
{
	set_font_pal(13);
	put_string("Audio:", 25, 2);
	if(audio)
		put_string("on ", 32, 2); 
	else
		put_string("off", 32, 2);
		
	put_string("Timing:", 24, 3);
	if(variation)
		put_string("random  ", 32, 3); 
	else
		put_string("rhythmic", 32, 3);
}

void ManualLagTestClickRefresh()
{
	for(x3 = 0; x3 < 10; x3++)
	{
		if(clicks[x3] != 0xFF)
		{
			set_font_pal(14);
			put_string("Offset  :", 2, 2+x3);
			put_number(x3+1, 2, 8, 2+x3); 
			
			if(clicks[x3] >= 0)
			{
				if(clicks[x3] == 0)
					set_font_pal(13);
				else
					set_font_pal(14);
			}
			else
				set_font_pal(15);
			
			put_number(clicks[x3], 2, 11, 2+x3); 
			put_string("frames", 14, 2+x3);
			if(clicks[x3] == 1 || clicks[x3] == -1)
				put_string(" ", 19, 2+x3);
		}
	}
}