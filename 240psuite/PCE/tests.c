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
#include "graphics.h"
#include "video.h"
#include "font.h"
#include "help.h"
#include "tests.h"
#include "float.h"

#ifdef CDROM1
extern int xres_flags;
extern unsigned char Enabled240p;
extern unsigned char UseDefault;
extern unsigned char EnabledSoft;
extern unsigned char Enabled_C_BW;

extern int HelpItem;
extern int ToolItem;

extern int prev_select;
extern int x_g;
extern int y_g;
#endif

void DropShadow(int strip)
{	
	view = 0;
	back = 0;
	colswap = 0;
	end = 0;
	text = 0;
	redraw = 1;
	speed = 1;
	x = 144;
	y = 100;
	
	if(!strip)
	{
		srand(clock_tt());
		option = random(2);
	}
	else
		option = 1;
	change = 0;
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {	
			ResetVideo();
			RedrawDropShadow(strip);
			redraw = 0;
        }
		
		if(refresh)
		{
			refresh = 0;
			switch(back)
			{
				case 0:
#ifndef CDROM1
					load_bat(0, donna_map, 40, 30);
#else
					cd_loadvram(GPHX_OVERLAY, OFS_donna_BAT_bin, 0, SIZE_donna_BAT_bin);
#endif
					break;
				case 1:
#ifndef CDROM1
					load_bat(0, sonic_map, 40, 30);
#else
					cd_loadvram(GPHX_OVERLAY, OFS_sonic_BAT_bin, 0, SIZE_sonic_BAT_bin);
#endif
					break;
				case 2:
					set_map_data(fs_map, 64, 3);
					load_map(0, 0, 0, 0, 64, 3);
					break;
				case 3:
					set_map_data(fs_map, 64, 3);
					load_map(0, 0, 0, 0, 64, 3);
					break;
			}
		}
		
		if(back == 1)
		{
			x1 = x2 = x3 = x4 = 0;
			DoSonicScroll();
		}

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			if(strip)
				showHelp(STRIPED_HELP);
			else
				showHelp(DSHADOW_HELP);
			redraw = 1;
		}
		
		if (controller & JOY_I)
		{
			back++;
			if(back > 3)
				back = 0;
			redraw = 1;
			disp_off();
		}
        
		if (controller & JOY_II)
			end = 1;
			
		if (!strip && controller & JOY_SEL)
		{
			if(view)
			{
				view = 0;
				put_string("Even frames", (back == 1) ? 0 : 26, 2);
			}
			else
			{
				view = 1;
				put_string("Odd frames ", (back == 1) ? 0 : 26, 2);
			}
			text = 60;
		}
		
		if(text)
		{
			text--;
			if(!text)
				refresh = 1;
		}
			
		controller = joy(0);
		
		if (controller & JOY_UP)
			y--;
			
		if (controller & JOY_DOWN)
			y++;
		
		if (controller & JOY_LEFT)
		{
			x--;
			if(!strip)
			{
				flipSprites();
				change = 0;
			}
		}
		
		if (controller & JOY_RIGHT)
		{
			x++;
			if(!strip)
			{
				flipSprites();
				change = 1;
			}
		}
			
		if(x < 0)
			x = 0;
		if(x > 288)
			x = 288;
		if(y < 0)
			y = 0;
		if(y > (Enabled240p ? 208 : 192))
			y = (Enabled240p ? 208 : 192);
		
		if(!strip)
		{
			spr_set(!option);
			if(view)
			{
				spr_x(x);
				spr_y(y);
				view = 0;
			}
			else
			{
				spr_x(512);
				spr_y(512);
				view = 1;
			}
		}
		else
		{
			spr_set(0);
			spr_x(x);
			spr_y(y);
		}
		if(!option)
		{
			spr_set(0);	
			spr_x(x-10);
			spr_y(y-10);
		}
		satb_update();
    }
}

void flipSprites()
{
	spr_set(0);
	spr_ctrl(FLIP_MAS|SIZE_MAS, (change ? FLIP_X : NO_FLIP)|SZ_32x32);
	spr_set(1);
	spr_ctrl(FLIP_MAS|SIZE_MAS, (change ? FLIP_X : NO_FLIP)|SZ_32x32);
}

void DoSonicScroll()
{
	if(colswap == 60)
	{
		SwapPalette(6, 4);
		SwapPalette(7, 8);
		
		colswap = 0;
	}
	colswap++;
				
	x1 += 1*speed*x;
	x2 += 2*speed*x;
	x3 += 3*speed*x;
	x4 += 4*speed*x;
	
	MovePalm(x4);
	
	scroll(0, x1, 0, 0, 76, 0xC0);
	scroll(1, x2, 76, 76, 160, 0xC0);
	scroll(2, x3, 160, 160, 208, 0xC0);
	scroll(3, x4, 208, 208, 240, 0xC0);
}

void RedrawDropShadow(int strip)
{
	switch(back)
	{
 		case 0:
#ifndef CDROM1
			load_background(donna_bg, donna_pal, donna_map, 40, 30);
#else
			set_screen_size(SCR_SIZE_64x32); 
			cd_loaddata(GPHX_OVERLAY, OFS_donna_PAL_bin, palCD, SIZE_donna_PAL_bin); 
			load_palette(0, palCD, 16); 
			cd_loadvram(GPHX_OVERLAY, OFS_donna_DATA_bin, 0x1000, SIZE_donna_DATA_bin);
			cd_loadvram(GPHX_OVERLAY, OFS_donna_BAT_bin, 0, SIZE_donna_BAT_bin);
			RestoreGlobals();
#endif
			break;
		case 1:
			set_screen_size(SCR_SIZE_32x32);

			scroll(0, x, 0, 0, 76, 0xC0);
			scroll(1, 2*x, 76, 76, 160, 0xC0);
			scroll(2, 3*x, 160, 160, 208, 0xC0);
			scroll(3, 4*x, 208, 208, 240, 0xC0);
#ifndef CDROM1
			load_background(sonic_bg, sonic_pal, sonic_map, 40, 30);
#else
			x_g = x;
			y_g = y;
			set_screen_size(SCR_SIZE_32x32); 
			cd_loaddata(GPHX_OVERLAY, OFS_sonic_PAL_bin, palCD, SIZE_sonic_PAL_bin); 
			load_palette(0, palCD, 16);
			cd_loadvram(GPHX_OVERLAY, OFS_sonic_DATA_bin, 0x1000, SIZE_sonic_DATA_bin);
			cd_loadvram(GPHX_OVERLAY, OFS_sonic_BAT_bin, 0, SIZE_sonic_BAT_bin);
			RestoreGlobals();
			x = x_g;
			y = y_g;
#endif
			break;
		case 2:
			set_map_data(fs_map, 64, 32);
			set_tile_data(hstripes_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 64, 32);
			load_palette(0, check_pal, 1); 
			break;
		case 3:
			set_map_data(fs_map, 64, 32);
			set_tile_data(check_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 64, 32);
			load_palette(0, check_pal, 1);
			break;
	}
	setupFont();
	
	init_satb();
	
	set_color_rgb(240, 0, 0, 0); 
	set_color_rgb(241, 0, 0, 0);
	
	if(strip)
	{
#ifndef CDROM1		 
		load_vram(0x6000, striped_sp, 0x100);
#else
		cd_loadvram(GPHX_OVERLAY, OFS_striped_tile_bin, 0x6000, SIZE_striped_tile_bin);
#endif
		spr_make(0, x, y, 0x6000, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_32x32, 0, 1);
	}
	else
	{
#ifndef CDROM1	
		if(option)
			load_vram(0x6000, shadow_sp, 0x100);
		else
			load_vram(0x6000, bee_sp, 0x200);
#else
		if(option)
			cd_loadvram(GPHX_OVERLAY, OFS_shadow_tile_bin, 0x6000, SIZE_shadow_tile_bin);
		else
			cd_loadvram(GPHX_OVERLAY, OFS_bee_tile_bin, 0x6000, SIZE_bee_tile_bin);
#endif
		load_palette(17, bee_pal, 1); 
		
		spr_make(0, x, y, 0x6000, FLIP_MAS|SIZE_MAS, (!change ? NO_FLIP : FLIP_X)|SZ_32x32, 1, 1);
		if(!option)
			spr_make(1, x+10, y+10, 0x6100, FLIP_MAS|SIZE_MAS, (!change ? NO_FLIP : FLIP_X)|SZ_32x32, 1, 2);
	}

	if(back == 1)
		DrawPalm();
	satb_update();
	
	Center224in240();
	disp_sync_on();
}

void SwapPalette(int pal, int index)
{	
	color = index+pal*16;
	
	top = get_color(color);
	bottom = get_color(color+1);
	left = get_color(color+2);
	right = get_color(color+3);
		
	set_color(color, right);
	set_color(color+1, top);
	set_color(color+2, bottom);
	set_color(color+3, left);		
}

void DrawPalm()
{
	draw = 0;
	
	load_palette(16, palm_pal, 1);
#ifndef CDROM1
	load_vram(0x5000, palm_sp, 0x8C0);
#else
	cd_loadvram(GPHX_OVERLAY, OFS_sonicpalm_tile_bin, 0x5000, SIZE_sonicpalm_tile_bin);
#endif

	x2 = 500;
	y2 = 104;
	for(row = 0; row < 7; row++)
	{
		for(i = 0; i < 5; i++)
		{
			spr_make(draw+2, i*16+x2, row*16+y2, 0x40*draw+0x5000, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_16x16, 0, 1);
			draw ++;
		}
	}
}

void MovePalm(int x)
{
	draw = 0;
	
	x = 4+x*-1;
	for(row = 0; row < 7; row++)
	{
		for(i = 0; i < 5; i++)
		{
			spr_set(draw+2);
			spr_x(i*16+x);
			draw ++;
		}
	}
	
	satb_update();
}

void ScrollTest()
{	
	type = FloatMenuRes320n256(1);
	if(type == FLOAT_CANCEL)
		return;
	
	option = 0;
	colswap = 0;
	end = 0;
	x1 = 0;
	x2 = 0;
	x3 = 0;
	x4 = 0;
	
	x = 1;	// dir
	speed = 1;  
	
	y = 0;
	redraw = 1;
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			ResetVideo();
			
			if(type == RES_320)
			{
				Set320H();
				if(option)
					vary = -32;
			}
			if(type == RES_256)
			{
				Set256H();
				if(option)
					vary = 0;
			}
			
			disp_off();
			
			if(!option)
			{
				set_screen_size(SCR_SIZE_32x32);
#ifndef CDROM1
				load_background(sonic_bg, sonic_pal, sonic_map, 40, 30);
#else
				cd_loaddata(GPHX_OVERLAY, OFS_sonic_PAL_bin, palCD, SIZE_sonic_PAL_bin); 
				load_palette(0, palCD, 16);
				cd_loadvram(GPHX_OVERLAY, OFS_sonic_DATA_bin, 0x1000, SIZE_sonic_DATA_bin);
				cd_loadvram(GPHX_OVERLAY, OFS_sonic_BAT_bin, 0, SIZE_sonic_BAT_bin);
				RestoreGlobals();
#endif

				init_satb();
				DrawPalm();
				satb_update();
				
				DoSonicScroll();							
				//MovePalm(4*speed*x);
			}
			else
			{
				set_screen_size(SCR_SIZE_32x64);
#ifndef CDROM1
				load_background(kiki_bg, kiki_pal, kiki_map, 32, 64);
#else
				cd_loaddata(GPHX_OVERLAY, OFS_kiki_PAL_bin, palCD, SIZE_kiki_PAL_bin); 
				load_palette(0, palCD, 16);
				cd_loadvram(GPHX_OVERLAY, OFS_kiki_DATA_bin, 0x1000, SIZE_kiki_DATA_bin);
				cd_loadvram(GPHX_OVERLAY, OFS_kiki_BAT_bin, 0, SIZE_kiki_BAT_bin);
				RestoreGlobals();
#endif
			}
			
            redraw = 0;
			disp_sync_on();
        }

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(SCROLL_HELP);
			redraw = 1;
		}
		
		if (controller & JOY_UP)
			speed++;
		
		if (controller & JOY_DOWN)
			speed --;
			
		if(speed > 10)
			speed = 10;
			
		if(speed < 0)
			speed = 0;
        
		if (controller & JOY_II)
			end = 1;
			
		if (controller & JOY_I)
		{
			option = !option;
			redraw = 1;
		}
		
		if (controller & JOY_SEL)
		{
			ntype = FloatMenuRes320n256(type);
			if(ntype != FLOAT_CANCEL)
				type = ntype;
			redraw = 1;
		}
		
		if (controller & JOY_LEFT)
			x = 1;
			
		if (controller & JOY_RIGHT)
			x = -1;
		
		if(!option)
			DoSonicScroll();
		else
		{
			y -= speed*x;
				
			if(y > 512)
				y = 0;
			if(y < 0)
				y = 512;
			
			scroll(0, vary, y, 0, 240, 0xC0);
		}
    }
}


#ifndef CDROM1
#include "tests_ext.c"
#else

void DrawCheck()
{
	ToolItem = TOOL_CHECK;
	cd_execoverlay(TEST_EXT_OVERLAY);
}

void LEDZoneTest()
{
	ToolItem = TOOL_LEDZONE;
	cd_execoverlay(TEST_EXT_OVERLAY);
}

void VScrollTest()
{
	ToolItem = TOOL_VSCROLL;
	cd_execoverlay(TEST_EXT_OVERLAY);
}

void LagTest()
{
	ToolItem = TOOL_PASSIVE;
	cd_execoverlay(TEST_EXT_OVERLAY);
}

void DrawStripes(int type)
{
	if(type)
		ToolItem = TOOL_V_STRIPES;
	else
		ToolItem = TOOL_H_STRIPES;
	cd_execoverlay(TEST_EXT_OVERLAY);
}

#endif


#ifndef CDROM1
#include "tests_sound.c"
#else

void SoundTest()
{
	ToolItem = TOOL_SOUND;
	cd_execoverlay(TEST_SND_OVERLAY);
}

void MDFourier()
{
	ToolItem = TOOL_MDFOURIER;
	cd_execoverlay(TEST_SND_OVERLAY);
}

void ConstrInterf()
{
	ToolItem = TOOL_CON_INTER;
	cd_execoverlay(TEST_SND_OVERLAY);
}

void AudioSyncTest()
{
	ToolItem = TOOL_AUDIOSYNC;
	cd_execoverlay(TEST_SND_OVERLAY);
}

void ReflexTest()
{
	ToolItem = TOOL_REFLEX;
	cd_execoverlay(TEST_SND_OVERLAY);
}

void HardwareTests()
{
	ToolItem = HARDWARETOOL;
	cd_execoverlay(TEST_SND_OVERLAY);
}

#endif