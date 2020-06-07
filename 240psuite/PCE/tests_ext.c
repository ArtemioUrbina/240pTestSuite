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
#include "res.h"
#include "font.h"
#include "video.h"
#include "tests.h"
#include "help.h"
#include "tools.h"
#include "float.h"

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
		case TOOL_VSCROLL:
			VScrollTest();
			break;
		case TOOL_PASSIVE:
			LagTest();
			break;
		case TOOL_LEDZONE:
			LEDZoneTest();
			break;
		case TOOL_CHECK:
			DrawCheck();
			break;
		case TOOL_H_STRIPES:
			DrawStripes(0);
			break;
		case TOOL_V_STRIPES:
			DrawStripes(1);
			break;
	}
	cd_execoverlay(MAIN_OVERLAY);
}
#endif

void DrawNumber(int x, int y, int sprite, int number, int palette)
{
	spr_make(sprite, x, y, 0x100*number+0x5000, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_32x32, palette, 1);
}

void ChangeNumber(int sprite, int number)
{
	spr_set(sprite);
	spr_pattern(0x100*number+0x5000);
}

void DrawCircle(int pos)
{
	x2 = 0;
	y2 = 78;
	
	if(pos > 4)
		x2 = 64*(pos-5);
	else
		x2 = 64*(pos-1);
	
	if(pos > 4)
		y2 = 148;
		
	spr_make(16, x2, y2, 0x5A00, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_32x32, 2, 2);
	spr_make(17, x2+32, y2, 0x5A00, FLIP_MAS|SIZE_MAS, FLIP_X|SZ_32x32, 2, 2);
	y2+=32;
	spr_make(18, x2, y2, 0x5A00, FLIP_MAS|SIZE_MAS, FLIP_Y|SZ_32x32, 2, 2);
	spr_make(19, x2+32, y2, 0x5A00, FLIP_MAS|SIZE_MAS, FLIP_X|FLIP_Y|SZ_32x32, 2, 2);
}

void LoadNumbers()
{
	set_color_rgb(256, 0, 0, 0);
	set_color_rgb(258, 0, 0, 0);
	
	set_color_rgb(272, 7, 7, 7);
	set_color_rgb(274, 7, 7, 7);
	
	set_color_rgb(288, 7, 7, 7);
	set_color_rgb(289, 7, 0, 0);
	
#ifndef CDROM1		
	load_vram(0x5000, numbers_sp, 0xB00);
#else
	cd_loadvram(GPHX_OVERLAY, OFS_numbers_tile_bin, 0x5000, SIZE_numbers_tile_bin);
#endif

	init_satb();
	
	// Hours			
	DrawNumber(5, 32, 0, 0, 0);	
	DrawNumber(30, 32, 1, 0, 0);	
	
	// Minutes
	DrawNumber(70, 32, 2, 0, 0);	
	DrawNumber(95, 32, 3, 0, 0);	
	
	//Seconds
	DrawNumber(135, 32, 4, 0, 0);	
	DrawNumber(160, 32, 5, 0, 0);	
	
	// Frames
	DrawNumber(200, 32, 6, 0, 0);	
	DrawNumber(225, 32, 7, 0, 0);	
	
	/*****Numbers on Circles*****/
	DrawNumber(20, 92, 8, 1, 1);	
	DrawNumber(84, 92, 9, 2, 1);
	DrawNumber(148, 92, 10, 3, 1);	
	DrawNumber(212, 92, 11, 4, 1);	
	
	DrawNumber(20, 162, 12, 5, 1);	
	DrawNumber(84, 162, 13, 6, 1);	
	DrawNumber(148, 162, 14, 7, 1);	
	DrawNumber(212, 162, 15, 8, 1);	
		
	DrawCircle(1);
	
	satb_update();
}

int framecnt = 0;
int frames = 0;
int seconds = 0;
int minutes = 0;
int hours = 0;
int lsd = 0;
int msd = 0;
unsigned char running = 0;
unsigned char update = 0;

void LagTest()
{
	end = 0;
	redraw = 1;
	option = 0;
    while(!end)
    {   
		vsync();
        if(redraw)
        {
			ResetVideo();
			setupFont();
			SetFontColors(14, RGB(7, 7, 7), 0, RGB(7, 7, 7));
			
			Set256H();
#ifndef CDROM1
			set_map_data(lagback_map, 32, 30);
			set_tile_data(lagback_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 32, 30);
			load_palette(0, lagback_pal, 1); 
#else
			set_screen_size(SCR_SIZE_32x32); 
			cd_loaddata(GPHX_OVERLAY, OFS_lagback_PAL_bin, palCD, SIZE_lagback_PAL_bin); 
			set_bgpal(0, palCD);
			cd_loadvram(GPHX_OVERLAY, OFS_lagback_DATA_bin, 0x1000, SIZE_lagback_DATA_bin);
			cd_loadvram(GPHX_OVERLAY, OFS_lagback_BAT_bin, 0x0000, SIZE_lagback_BAT_bin);
#endif
			put_string("hours", 1, 3);
			put_string("minutes", 9, 3);
			put_string("seconds", 17, 3);
			put_string("frames", 25, 3);
			LoadNumbers();

#ifndef CDROM1		
			set_color(2, RGB(7, 7, 7));
#else
			set_color(4, RGB(7, 7, 7));
#endif
         
            redraw = 0;
			disp_sync_on();
        }
		
		if(running || update)
		{	
			if(framecnt > 7)
				framecnt = 0;
				
			if(frames > 59)
			{
				frames = 0;
				seconds ++;	
			}
			
			if(seconds > 59)
			{
				seconds = 0;
				minutes ++;
			}

			if(minutes > 59)
			{
				minutes = 0;
				hours ++;
			}

			if(hours > 99)
				hours = 0;
			
			lsd = hours % 10;
			msd = hours / 10;
			ChangeNumber(0, msd);
			ChangeNumber(1, lsd);
			
			lsd = minutes % 10;
			msd = minutes / 10;
			ChangeNumber(2, msd);
			ChangeNumber(3, lsd);
			
			lsd = seconds % 10;
			msd = seconds / 10;
			ChangeNumber(4, msd);
			ChangeNumber(5, lsd);
			
			lsd = frames % 10;
			msd = frames / 10;
			ChangeNumber(6, msd);
			ChangeNumber(7, lsd);
			
			DrawCircle(framecnt+1);
			
			satb_update();
		
			if(running)
			{
				frames ++;
				framecnt ++;
			}
			
			update = 0;
			
			option = !option;
#ifndef CDROM1
			if(option)
				set_color(2, RGB(0, 0, 0));
			else
				set_color(2, RGB(7, 7, 7));
#else
			if(option)
				set_color(4, RGB(0, 0, 0));
			else
				set_color(4, RGB(7, 7, 7));
#endif
		}
			
        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(PASSIVE_HELP);
			redraw = 1;
			update = 1;
		}
        
		if (controller & JOY_II)
			end = 1;
			
		if (controller & JOY_I)
			running = !running;
			
		if (controller & JOY_SEL && !running)
		{
			frames = hours = minutes = seconds = 0;
			framecnt = 0;
			update = 1;
		}
    }
}

void VScrollTest()
{
	int *pos = 0;
	
	type = FloatMenuRes320n256(1);
	if(type == FLOAT_CANCEL)
		return;
		
	x = 0;
	y = 0;
	end = 0;
	redraw = 1;
	pos = &x;
	speed = 1;
	option = 1;  // acc
    while(!end)
    {   
		vsync();
        if(redraw)
        {
			ResetVideo();
			setupFont();
			
			if(type == RES_256)
				set_screen_size(SCR_SIZE_32x32);
			else
				set_screen_size(SCR_SIZE_64x32);
			
			scroll(0, x, y, 0, 240, 0xC0);
			set_map_data(fs_map, 64, 32);
			set_tile_data(cgrid_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 64, 32);
			load_palette(0, check_pal, 1); 

			Center224in240(); 
         
            redraw = 0;
			ChangeResType();
			disp_sync_on();
        }

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(GRIDSCROLL_HELP);
			redraw = 1;
		}
        
		if (controller & JOY_II)
			end = 1;
		
		if (controller & JOY_UP)
			speed++;

		if (controller & JOY_DOWN)
			speed--;

		if (speed > 5)        
			speed = 5;          

		if (speed < 0)        
			speed = 0;          

		if (controller & JOY_I)
		{
			if(pos == &x)
				pos = &y;
			else
				pos = &x;
		}
		

		if (controller & JOY_LEFT || controller & JOY_RIGHT)
			option *= -1;

		if(controller & JOY_SEL)
		{
			ntype = FloatMenuRes320n256(type);
			if(ntype != FLOAT_CANCEL)
				type = ntype;
			redraw = 1;
		}

		*pos += option*speed;
			
		scroll(0, x, y, 0, 240, 0xC0);
    }
}

void LEDZoneTest()
{
	// visible = option

	x2 = 0;
	end = 0;
	x = 144;
	y = 120;
	redraw = 1;
	option = 1;
	
	clicks[0] = 1;
	clicks[1] = 2;
	clicks[2] = 4;
	clicks[3] = 8;
	
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			ResetVideo();

			set_map_data(fs_map, 64, 32);
			set_tile_data(white_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 64, 32);
			set_color_rgb(1, 0, 0, 0); 

			Center224in240();
			
			refresh = 1;
			
            redraw = 0;
			disp_sync_on();
        }
		
		if(refresh)
		{
			init_satb();
			set_color_rgb(256, 0, 0, 0); 
			set_color_rgb(257, 7, 7, 7); 
#ifndef CDROM1		
			load_vram(0x5000, LED_sp, 0x100);
#else
			cd_loadvram(GPHX_OVERLAY, OFS_LEDsprites_tile_bin, 0x5000, SIZE_LEDsprites_tile_bin);
#endif
			spr_make(0, x, y, 0x5000+0x40*x2, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_16x16, 0, 1);
			satb_update();
			refresh = 0;
		}

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(BACKLIT_HELP);
			redraw = 1;
		}
        
		if (controller & JOY_II)
			end = 1;
			
		if (controller & JOY_SEL)
		{
			option = !option;
			if(!option)
				set_color_rgb(257, 0, 0, 0); 
			else
				set_color_rgb(257, 7, 7, 7); 
		}
			
		if (controller & JOY_I)
		{
			x2++;
			if(x2 > 3)
				x2 = 0;
			refresh = 1;
		}
			
		if(option)
		{
			controller = joy(0);
			
			if (controller & JOY_UP)
				y--;
				
			if (controller & JOY_DOWN)
				y++;
			
			if (controller & JOY_LEFT)
				x--;
			
			if (controller & JOY_RIGHT)
				x++;
				
			if(x < 0)
				x = 0;
			if(x > 320 - clicks[x2])
				x = 320 - clicks[x2];
			if(y < 0)
				y = 0;
			if(y > (Enabled240p ? 240 - clicks[x2] : 224 - clicks[x2]))
				y = Enabled240p ? 240 - clicks[x2] : 224 - clicks[x2];
		
			spr_set(0);	
		
			spr_x(x);
			spr_y(y);
		
			satb_update();
		}
    }
}

void DrawCheck()
{
	// alternate == option
	// vary == toggle
	// drawframe == x2
	// frame == x3

	type = FloatMenuRes(1);
	if(type == FLOAT_CANCEL)
		return;
	
	option = 0;
	vary = 0;
	x2 = 0;
	x3 = 0;
	end = 0;
	redraw = 1;
	
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			RedrawCheck();
            redraw = 0;
			disp_sync_on();
        }
		
		if(x2)
		{
			put_string("frame: ", 2, 26);
			put_number(x3, 2, 8, 26); 
			x3 ++;
			if(x3 == 60)
				x3 = 0;
		}

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(CHECK_HELP);
			redraw = 1;
		}
		
		if(option || controller & JOY_LEFT || controller & JOY_RIGHT)
		{
			if(vary)
			{
				set_color(0, 0);
				set_color(1, RGB(7, 7, 7));
			}
			else
			{
				set_color(1, 0);
				set_color(0, RGB(7, 7, 7));
			}
			vary = !vary;
		}
		
		if (controller & JOY_UP)
		{
			x2 = !x2;
			if(!x2)
				RefreshFS();
		}
		
		if (controller & JOY_SEL)
		{
			ntype = FloatMenuRes(type);
			if(ntype != FLOAT_CANCEL)
				type = ntype;
			redraw = 1;
		}

		if (controller & JOY_I)
			option = !option;
        
		if (controller & JOY_II)
			end = 1;
    }
}

void RedrawCheck()
{
	ResetVideo();
	setupFont();

	ChangeResType();
	
	set_screen_size(SCR_SIZE_64x32); 
	
	set_map_data(fs_map, 64, 32);
	set_tile_data(check_bg);
	load_tile(0x1000);
	load_map(0, 0, 0, 0, 64, 32);
	load_palette(0, check_pal, 1); 
	
	Center224in240(); 
}


void RefreshFS()
{
	set_map_data(fs_map, 64, 32);
	load_map(0, 0, 0, 0, 64, 32);
}

void DrawStripes(int vert)
{
	type = FloatMenuRes(1);
	if(type == FLOAT_CANCEL)
		return;

	option = 0; // frame
	x = 0; 		// pos
	vary = 0; 	// toggle
	draw = 0; 	// alternate
	y = vert;	// vertical
	end = 0;
	redraw = 1;
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			RedrawStripes();
            redraw = 0;
			disp_sync_on();
        }
		
		if(draw)
		{
			put_string("Frame: ", 2, 26);
			put_number(option, 2, 8, 26); 
			option ++;
			if(option == 60)
				option = 0;
		}
		
		controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(vert ? VERT_STRIPES_HELP : HORI_STRIPES_HELP);
			redraw = 1;
		}
		
		if(vary || controller & JOY_RIGHT)
		{
			if(x)
			{
				set_color(0, 0);
				set_color(1, RGB(7, 7, 7));
			}
			else
			{
				set_color(1, 0);
				set_color(0, RGB(7, 7, 7));
			}
			x = !x;
		}
		
		if (controller & JOY_I)
			vary = !vary;
        
		if (controller & JOY_II)
			end = 1;
		
		if (controller & JOY_LEFT)
		{
			draw = !draw;
			if(!draw)
			{
				set_map_data(fs_map, 64, 32);
				load_map(0, 0, 0, 0, 64, 32);
			}
		}
		
		if (controller & JOY_SEL)
		{
			ntype = FloatMenuRes(type);
			if(ntype != FLOAT_CANCEL)
				type = ntype;
			redraw = 1;
		}
    }
}

void LoadStripesTile()
{
	if(y)
	{
		set_tile_data(vstripes_bg);
		load_tile(0x1000);
	}
	else
	{
		set_tile_data(hstripes_bg);
		load_tile(0x1000);
	}
}
	
void RedrawStripes()
{
	ResetVideo();
	setupFont();
	
	ChangeResType();
	
	set_screen_size(SCR_SIZE_64x32); 
	set_map_data(fs_map, 64, 32);
	LoadStripesTile();
	load_map(0, 0, 0, 0, 64, 32);
	load_palette(0, check_pal, 1);  
	
	Center224in240();
}

