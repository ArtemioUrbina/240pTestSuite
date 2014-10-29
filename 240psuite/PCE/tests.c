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

#ifdef CDROM
extern int xres_flags;
extern unsigned char Enabled240p;
extern unsigned char UseDefault;
extern unsigned char EnabledSoft;
extern unsigned char Enabled_C_BW;
#endif

void DrawCheck()
{
    int controller;   
    int read; 
    unsigned char redraw = 1;
	unsigned char end = 0;
	unsigned char alternate = 0;
	unsigned char pos = 0;
	unsigned char drawframe = 0;
	int frame = 0;

    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			ResetVideo();
			setupFont();

#ifndef CDROM1			
			set_map_data(fs_map, 40, 30);
			set_tile_data(check_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, check_pal, 1); 
#else
#endif

			Center224in240(); 
         
            redraw = 0;
			disp_on();
        }
		
		if(drawframe)
		{
			put_string("Frame: ", 2, 26);
			put_number(frame, 2, 8, 26); 
			frame ++;
			if(frame == 60)
				frame = 0;
		}

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(CHECK_HELP);
			redraw = 1;
		}
		
		if(alternate || controller & JOY_SEL)
		{
			if(pos)
			{
				set_color(0, 0);
				set_color(1, RGB(7, 7, 7));
			}
			else
			{
				set_color(1, 0);
				set_color(0, RGB(7, 7, 7));
			}
			pos = !pos;
		}
		
		if (controller & JOY_I)
			alternate = !alternate;
        
		if (controller & JOY_LEFT)
		{
			drawframe = !drawframe;
			if(!drawframe)
			{
#ifndef CDROM1
				set_map_data(fs_map, 40, 30);
				load_map(0, 0, 0, 0, 40, 30);
#else
#endif
			}
		}
        
		if (controller & JOY_II)
			end = 1;
    }
}


void DrawStripes()
{
    int controller;   
    int read; 
    unsigned char redraw = 1;
	unsigned char end = 0;
	unsigned char alternate = 0;
	unsigned char pos = 0;
	unsigned char drawframe = 0;
	int frame = 0;

    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			ResetVideo();
			setupFont();

#ifndef CDROM1			
			set_map_data(fs_map, 40, 30);
			set_tile_data(hstripes_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, check_pal, 1);  
#else
#endif
			
			Center224in240();
         
            redraw = 0;
			disp_on();
        }
		
		if(drawframe)
		{
			put_string("Frame: ", 2, 26);
			put_number(frame, 2, 8, 26); 
			frame ++;
			if(frame == 60)
				frame = 0;
		}
		
		controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(GENERAL_HELP);
			redraw = 1;
		}
		
		if(alternate || controller & JOY_SEL)
		{
			if(pos)
			{
				set_color(0, 0);
				set_color(1, RGB(7, 7, 7));
			}
			else
			{
				set_color(1, 0);
				set_color(0, RGB(7, 7, 7));
			}
			pos = !pos;
		}
		
		if (controller & JOY_I)
			alternate = !alternate;
        
		if (controller & JOY_II)
			end = 1;
		
		if (controller & JOY_LEFT)
		{
			drawframe = !drawframe;
			if(!drawframe)
			{
#ifndef CDROM1
				set_map_data(fs_map, 40, 30);
				load_map(0, 0, 0, 0, 40, 30);
#else
#endif
			}
		}
			
		if (controller & JOY_UP)
		{
#ifndef CDROM1
			set_tile_data(vstripes_bg);
			load_tile(0x1000);
#else
#endif
		}
		if (controller & JOY_DOWN)
		{
#ifndef CDROM1
			set_tile_data(hstripes_bg);
			load_tile(0x1000);
#else
#endif
		}
    }
}

void DropShadow()
{
    int controller;   
    int read; 
    unsigned char redraw = 1;
	unsigned char end = 0;
	int x = 144, y = 100;
	unsigned char show = 1;
	unsigned char text = 0;
	unsigned char refresh = 0;
	unsigned char back = 0;
	int colswap = 0;

    while(!end)
    {   
		vsync();
		
        if(redraw)
        {	
			ResetVideo();
			RedrawDropShadow(back, x, y);
			redraw = 0;
        }
		
		if(refresh)
		{
			refresh = 0;
			switch(back)
			{
				case 0:
#ifndef CDROM1
					load_bat(0, motoko_map, 40, 3);
#else
#endif
					break;
				case 1:
#ifndef CDROM1
					load_bat(0, sonic_map, 40, 30);
#else
#endif
					break;
				case 2:
#ifndef CDROM1
					set_map_data(fs_map, 40, 3);
					load_map(0, 0, 0, 0, 40, 3);
#else
#endif
					break;
				case 3:
#ifndef CDROM1
					set_map_data(fs_map, 40, 3);
					load_map(0, 0, 0, 0, 40, 3);
#else
#endif
					break;
			}
		}
		
		if(back == 1)
		{
			if(colswap == 60)
			{
				SwapPalette(6, 4);
				SwapPalette(7, 8);
				
				colswap = 0;
			}
			colswap++;
			
			scroll(0, x, 0, 0, 76, 0xC0);
			scroll(1, 2*x, 76, 76, 160, 0xC0);
			scroll(2, 3*x, 160, 160, 208, 0xC0);
			scroll(3, 4*x, 208, 208, 240, 0xC0);
		
			MovePalm(4*x);
		}

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(GENERAL_HELP);
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
			
		if (controller & JOY_SEL)
		{
			if(show)
			{
				show = 0;
				put_string("Even frames", (back == 1) ? 0 : 26, 2);
			}
			else
			{
				show = 1;
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
			x--;
		
		if (controller & JOY_RIGHT)
			x++;
			
		if(x < 0)
			x = 0;
		if(x > 288)
			x = 288;
		if(y < 0)
			y = 0;
		if(y > (Enabled240p ? 208 : 192))
			y = (Enabled240p ? 208 : 192);
		
		spr_set(0);	
		if(show)
		{
			spr_x(x);
			spr_y(y);
			show = 0;
		}
		else
		{
			spr_x(512);
			spr_y(512);
			show = 1;
		}
		satb_update();
    }
}

void RedrawDropShadow(unsigned char back, int x, int y)
{
	switch(back)
	{
		case 0:
#ifndef CDROM1
			load_background(motoko_bg, motoko_pal, motoko_map, 40, 30);
#else
			set_screen_size(SCR_SIZE_64x32); 
			cd_loadvram(4, OFS_motoko_DATA_bin, 0x1000, SIZE_motoko_DATA_bin);
			cd_loadvram(4, OFS_motoko_BAT_bin, 0, SIZE_motoko_BAT_bin);
#endif
			break;
		case 1:
			set_screen_size(SCR_SIZE_32x32);

			scroll(0, 0, y, 0, 76, 0xC0);
			scroll(1, 0, 76, 76, 160, 0xC0);
			scroll(2, 0, 160, 160, 208, 0xC0);
			scroll(3, 0, 208, 208, 240, 0xC0);
#ifndef CDROM1
			load_background(sonic_bg, sonic_pal, sonic_map, 40, 30);
#else
			set_screen_size(SCR_SIZE_64x32); 
			cd_loadvram(4, OFS_sonic_DATA_bin, 0x1000, SIZE_sonic_DATA_bin);
			cd_loadvram(4, OFS_sonic_BAT_bin, 0, SIZE_sonic_BAT_bin);
#endif
			break;
		case 2:
#ifndef CDROM1
			set_map_data(fs_map, 40, 30);
			set_tile_data(hstripes_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, check_pal, 1); 
#else
#endif
			break;
		case 3:
#ifndef CDROM1
			set_map_data(fs_map, 40, 30);
			set_tile_data(check_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			load_palette(0, check_pal, 1);
#else
#endif
			break;
	}
	setupFont();
	
	init_satb();
	set_color_rgb(240, 0, 0, 0); 
	set_color_rgb(241, 0, 0, 0); 
#ifndef CDROM1		
	load_vram(0x6000, shadow_sp, 0x100);
	spr_make(0, x, y, 0x6000, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_32x32, 0, 1);
#else
#endif
	if(back == 1)
		DrawPalm();
	satb_update();
	
	Center224in240();
	disp_on();
}

void StripedSprite()
{
    int controller;   
    int read; 
    unsigned char redraw = 1;
	unsigned char end = 0;
	int x = 144, y = 100;
	unsigned char back = 0;
	int colswap = 0;

    while(!end)
    {   
		vsync();
		
        if(redraw)
        {	
			ResetVideo();
			switch(back)
			{
				case 0:
#ifndef CDROM1
					load_background(motoko_bg, motoko_pal, motoko_map, 40, 30);
#else
					set_screen_size(SCR_SIZE_64x32); 
					cd_loadvram(4, OFS_motoko_DATA_bin, 0x1000, SIZE_motoko_DATA_bin);
					cd_loadvram(4, OFS_motoko_BAT_bin, 0, SIZE_motoko_BAT_bin);
#endif
					break;
				case 1:
					set_screen_size(SCR_SIZE_32x32);
		
					scroll(0, 0, y, 0, 76, 0xC0);
					scroll(1, 0, 76, 76, 160, 0xC0);
					scroll(2, 0, 160, 160, 208, 0xC0);
					scroll(3, 0, 208, 208, 240, 0xC0);
#ifndef CDROM1
					load_background(sonic_bg, sonic_pal, sonic_map, 40, 30);
#else
					set_screen_size(SCR_SIZE_64x32); 
					cd_loadvram(4, OFS_sonic_DATA_bin, 0x1000, SIZE_sonic_DATA_bin);
					cd_loadvram(4, OFS_sonic_BAT_bin, 0, SIZE_sonic_BAT_bin);
#endif
					break;
				case 2:
#ifndef CDROM1
					set_map_data(fs_map, 40, 30);
					set_tile_data(hstripes_bg);
					load_tile(0x1000);
					load_map(0, 0, 0, 0, 40, 30);
					load_palette(0, check_pal, 1);
#else
#endif
					break;
				case 3:
#ifndef CDROM1
					set_map_data(fs_map, 40, 30);
					set_tile_data(check_bg);
					load_tile(0x1000);
					load_map(0, 0, 0, 0, 40, 30);
					load_palette(0, check_pal, 1);
#else
#endif
					break;
			}
			setupFont();
			
			init_satb();
			set_color_rgb(240, 0, 0, 0); 
			set_color_rgb(241, 0, 0, 0);
#ifndef CDROM1		 
			load_vram(0x6000, striped_sp, 0x100);
			spr_make(0, x, y, 0x6000, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_32x32, 0, 1);
#else
#endif
			if(back == 1)
				DrawPalm();
			satb_update();
			
			Center224in240();
            redraw = 0;
			disp_on();
        }
		
		if(back == 1)
		{
			if(colswap == 60)
			{
				SwapPalette(6, 4);
				SwapPalette(7, 8);
				
				colswap = 0;
			}
			colswap++;
			
			scroll(0, x, 0, 0, 76, 0xC0);
			scroll(1, 2*x, 76, 76, 160, 0xC0);
			scroll(2, 3*x, 160, 160, 208, 0xC0);
			scroll(3, 4*x, 208, 208, 240, 0xC0);
		
			MovePalm(4*x);
		}

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(GENERAL_HELP);
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
		if(x > 288)
			x = 288;
		if(y < 0)
			y = 0;
		if(y > (Enabled240p ? 208 : 192))
			y = (Enabled240p ? 208 : 192);
		
		spr_set(0);	
		
		spr_x(x);
		spr_y(y);
		
		satb_update();
    }
}

void SwapPalette(int pal, int index)
{
	int pos;
	int tmpcol1 = 0;
	int tmpcol2 = 0;
	int tmpcol3 = 0;
	int tmpcol4 = 0;
	
	pos = index+pal*16;
	tmpcol1 = get_color(pos);
	tmpcol2 = get_color(pos+1);
	tmpcol3 = get_color(pos+2);
	tmpcol4 = get_color(pos+3);
		
	set_color(pos, tmpcol4);
	set_color(pos+1, tmpcol1);
	set_color(pos+2, tmpcol2);
	set_color(pos+3, tmpcol3);		
}

void DrawPalm()
{
	int x = 500;
	int y = 104;
	int vram = 0x5000;
	int pos = 0;
	unsigned char row = 0;
	unsigned char count = 0;
	
#ifndef CDROM1
	load_palette(16, palm_pal, 1);
	load_vram(vram, palm_sp, 0x8C0);
#else
#endif

	for(row = 0; row < 7; row++)
	{
		for(pos = 0; pos < 5; pos++)
		{
			spr_make(count+2, pos*16+x, row*16+y, 0x40*count+vram, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_16x16, 0, 1);
			count ++;
		}
	}
}

void MovePalm(int x)
{
	int row = 0;
	unsigned char pos = 0;
	unsigned char count = 0;
	
	x = 4+x*-1;
	for(row = 0; row < 7; row++)
	{
		for(pos = 0; pos < 5; pos++)
		{
			spr_set(count+2);
			spr_x(pos*16+x);
			count ++;
		}
	}
	
	satb_update();
}

void ScrollTest()
{
    int controller;   
    int read; 
    unsigned char redraw = 1;
	unsigned char end = 0;
	int x1 = 0;
	int x2 = 0;
	int x3 = 0;
	int x4 = 0;
	int y = 0;
	unsigned char pause = 0;
	int dir = 1;
	int spd = 1;
	int colswap = 0;

    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			ResetVideo();
			set_screen_size(SCR_SIZE_32x32);
	
			scroll(0, x1, 0, 0, 76, 0xC0);
			scroll(1, x2, 76, 76, 160, 0xC0);
			scroll(2, x3, 160, 160, 208, 0xC0);
			scroll(3, x4, 208, 208, 240, 0xC0);
	
#ifndef CDROM1
			load_background(sonic_bg, sonic_pal, sonic_map, 40, 30);
#else
			set_screen_size(SCR_SIZE_64x32); 
			cd_loadvram(4, OFS_sonic_DATA_bin, 0x1000, SIZE_sonic_DATA_bin);
			cd_loadvram(4, OFS_sonic_BAT_bin, 0, SIZE_sonic_BAT_bin);
#endif
			
			init_satb();
			DrawPalm();
			satb_update();
			
			MovePalm(4*spd*dir);
			
            redraw = 0;
			disp_on();
        }

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(GENERAL_HELP);
			redraw = 1;
		}
		
		if (controller & JOY_UP)
			spd++;
		
		if (controller & JOY_DOWN)
			spd --;
			
		if(spd > 10)
			spd = 10;
			
		if(spd < 0)
			spd = 0;
        
		if (controller & JOY_II)
			end = 1;
			
		if (controller & JOY_I)
		{
			if(pause)
				pause = 0;
			else
				pause = 1;
		}
		
		if(colswap == 60)
		{
			SwapPalette(6, 4);
			SwapPalette(7, 8);
			
			colswap = 0;
		}
		colswap++;
		
		if (controller & JOY_SEL)
			dir *= -1;
		
		if(!pause)
		{
			x1 += 1*spd*dir;
			x2 += 2*spd*dir;
			x3 += 3*spd*dir;
			x4 += 4*spd*dir;
		}
		
		MovePalm(x4);
		
		scroll(0, x1, 0, 0, 76, 0xC0);
		scroll(1, x2, 76, 76, 160, 0xC0);
		scroll(2, x3, 160, 160, 208, 0xC0);
		scroll(3, x4, 208, 208, 240, 0xC0);
    }
	set_screen_size(SCR_SIZE_64x32);
}

void LEDZoneTest()
{
    int controller;   
    int read; 
    unsigned char redraw = 1;
	unsigned char end = 0;
	int sel = 0;
	int x = 144, y = 120;
	unsigned char refresh = 0;
	unsigned char visible = 1;
	int size[4];

	size[0] = 1;
	size[1] = 2;
	size[2] = 4;
	size[3] = 8;
	
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			ResetVideo();

#ifndef CDROM1			
			set_map_data(fs_map, 40, 30);
			set_tile_data(white_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 30);
			set_color_rgb(1, 0, 0, 0); 
#else
#endif

			Center224in240();
			
			refresh = 1;
			
            redraw = 0;
			disp_on();
        }
		
		if(refresh)
		{
			init_satb();
			set_color_rgb(256, 0, 0, 0); 
			set_color_rgb(257, 7, 7, 7); 
#ifndef CDROM1		
			load_vram(0x5000, LED_sp, 0x100);
			spr_make(0, x, y, 0x5000+0x40*sel, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_16x16, 0, 1);
#else
#endif
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
			visible = !visible;
			if(!visible)
				set_color_rgb(257, 0, 0, 0); 
			else
				set_color_rgb(257, 7, 7, 7); 
		}
			
		if (controller & JOY_I)
		{
			sel++;
			if(sel > 3)
				sel = 0;
			refresh = 1;
		}
			
		if(visible)
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
			if(x > 320 - size[sel])
				x = 320 - size[sel];
			if(y < 0)
				y = 0;
			if(y > (Enabled240p ? 240 - size[sel] : 224 - size[sel]))
				y = Enabled240p ? 240 - size[sel] : 224 - size[sel];
		
			spr_set(0);	
		
			spr_x(x);
			spr_y(y);
		
			satb_update();
		}
    }
}

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
	int x = 0;
	int y = 78;
	
	if(pos > 4)
		x = 64*(pos-5);
	else
		x = 64*(pos-1);
	
	if(pos > 4)
		y = 148;
		
	spr_make(16, x, y, 0x5A00, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_32x32, 2, 2);
	spr_make(17, x+32, y, 0x5A00, FLIP_MAS|SIZE_MAS, FLIP_X|SZ_32x32, 2, 2);
	y+=32;
	spr_make(18, x, y, 0x5A00, FLIP_MAS|SIZE_MAS, FLIP_Y|SZ_32x32, 2, 2);
	spr_make(19, x+32, y, 0x5A00, FLIP_MAS|SIZE_MAS, FLIP_X|FLIP_Y|SZ_32x32, 2, 2);
}

void LoadNumbers()
{
	int count = 0;
	int number = 1;

	set_color_rgb(256, 0, 0, 0);
	set_color_rgb(258, 0, 0, 0);
	
	set_color_rgb(272, 7, 7, 7);
	set_color_rgb(274, 7, 7, 7);
	
	set_color_rgb(288, 7, 7, 7);
	set_color_rgb(289, 7, 0, 0);
	
#ifndef CDROM1		
	load_vram(0x5000, numbers_sp, 0xB00);
#else
	cd_loadvram(4, OFS_numbers_tile_bin, 0x5000, SIZE_numbers_tile_bin);
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

void LagTest()
{
    int controller;   
    int read; 
    unsigned char redraw = 1;
	unsigned char end = 0;
	int framecnt = 0;
	int frames = 0;
	int seconds = 0;
	int minutes = 0;
	int hours = 0;
	unsigned char running = 0;
	int lsd = 0;
	int msd = 0;
	unsigned char update = 0;

    while(!end)
    {   
		vsync();
        if(redraw)
        {
			ResetVideo();
			setupFont();
			SetFontColors(14, RGB(7, 7, 7), 0, RGB(7, 7, 7));
			
#ifndef CDROM1
			set_map_data(lagback_map, 32, 30);
			set_tile_data(lagback_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 32, 30);
			load_palette(0, lagback_pal, 1); 
#else
#endif
			put_string("hours", 1, 3);
			put_string("minutes", 9, 3);
			put_string("seconds", 17, 3);
			put_string("frames", 25, 3);
			LoadNumbers();
	
			Center224in240(); 
         
            redraw = 0;
			disp_on();
			set_xres(256, xres_flags);
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
		}
			
        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(GENERAL_HELP);
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
    int controller;   
    int read; 
    unsigned char redraw = 1;
	unsigned char end = 0;
	int x = 0;
	int y = 0;
	int speed = 1;
	int *pos = 0;
	unsigned char pause = 0;
	int acc = 1;

	pos = &x;
    while(!end)
    {   
		vsync();
        if(redraw)
        {
			ResetVideo();
			setupFont();
			
			set_screen_size(SCR_SIZE_32x32);
			scroll(0, x, y, 0, 240, 0xC0);
#ifndef CDROM1
			set_map_data(fs_map, 40, 32);
			set_tile_data(cgrid_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 32);
			load_palette(0, check_pal, 1); 
#else
#endif

			Center224in240(); 
         
            redraw = 0;
			disp_on();
        }

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(GENERAL_HELP);
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
			pause = !pause;

		if (controller & JOY_LEFT || controller & JOY_RIGHT)
			acc *= -1;

		if(controller & JOY_SEL)
		{
			if(pos == &x)
				pos = &y;
			else
				pos = &x;
		}

		if(!pause)
			*pos += acc*speed;
			
		scroll(0, x, y, 0, 240, 0xC0);
    }
	set_screen_size(SCR_SIZE_64x32);
}

void LoadWave()
{
#asm
		lda     #0
		sta     $0800
		lda     #%010_00000
		sta     $0804
		lda     #%000_00000
		sta     $0804
		
		clx   
				
load:   lda     sine,x
		sta     $0806 
		inx           
		cpx     #32   
		bne     load
		
		
		lda     #LOW(112)
		sta     $0802
		lda     #HIGH(112)
		sta     $0803
		
		lda     #$EE
		sta     $0801		
		
		rts
		
sine:   db      18,22,24,26,28,28,30,30
		db      30,30,28,28,26,24,22,18
		db      12,8,6,4,2,2,0,0
		db      0,0,2,2,4,6,8,12
#endasm		
}

void PlayLeft()
{
#asm
		lda     #$F0
		sta     $0805
		
		lda     #%100_11111
		sta     $0804
		rts
#endasm
}

void PlayRight()
{
#asm
		lda     #$0F
		sta     $0805
		
		lda     #%100_11111
		sta     $0804
		rts
#endasm
}

void PlayCenter()
{
#asm
		lda     #$FF
		sta     $0805
		
		lda     #%100_11111
		sta     $0804
		rts
#endasm
}

void StopAudio()
{
#asm
		lda     #%000_11111
		sta     $0804
		rts
#endasm
}

void SoundTest()
{
    int controller;   
    int read; 
    unsigned char redraw = 1;
	unsigned char refresh = 0;
	unsigned char end = 0;
	int count = 0;
	int sel = 1;

	LoadWave();
	
    while(!end)
    {   
		vsync();
        if(redraw)
        {
			ResetVideo();
			setupFont();

			SetFontColors(13, RGB(2, 2, 2), RGB(0, 6, 0), 0);
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
			
			set_font_pal(13);
            put_string("Sound Test", 15, 5);
			
			Center224in240();
			
            redraw = 0;
			refresh = 1;
			disp_on();
		}
		
		if(refresh)
		{
			set_font_pal(sel == 0 ? 15 : 14);
            put_string("Left", 9, 14);
            set_font_pal(sel == 1 ? 15 : 14);
            put_string("Center", 17, 16);
            set_font_pal(sel == 2 ? 15 : 14);
            put_string("Right", 26, 14);
		}

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			StopAudio();
			showHelp(GENERAL_HELP);
			redraw = 1;
		}
        
		if (controller & JOY_II)
			end = 1;
			
		if (controller & JOY_I)
		{
			switch(sel)
			{
				case 0:
					PlayLeft();
					break;
				case 1:
					PlayCenter();
					break;
				case 2:
					PlayRight();
					break;
			}
			count = 20;
		}
			
		if (controller & JOY_LEFT)
			sel --;
			
		if (controller & JOY_RIGHT)
			sel ++;
	
		if(sel < 0)
			sel = 0;
		if(sel > 2)
			sel = 2;
			
		if(count)
			count--;
			
		if(count == 1)
		{
			StopAudio();
			count = 0;
		}
    }
	StopAudio();
}


void RedrawManualLagTest(int x)
{
	ResetVideo();
	setupFont();
	SetFontColors(13, 0, RGB(0, 7, 0), 0);
	SetFontColors(14, 0, RGB(7, 7, 7), 0);
	SetFontColors(15, 0, RGB(7, 0, 0), 0);

#ifndef CDROM1			
	set_map_data(fs_map, 40, 30);
	set_tile_data(white_bg);
	load_tile(0x1000);
	load_map(0, 0, 0, 0, 40, 30);
	set_color_rgb(1, 0, 0, 0);   
#else
#endif
	ManualLagTestSprites(x);
	ManualLagTestText();
}

void ManualLagTestSprites(int x)
{
	init_satb();
	set_color_rgb(256, 0, 0, 0); 
	set_color_rgb(257, 7, 7, 7); 
	set_color_rgb(273, 7, 0, 0); 
	set_color_rgb(289, 0, 7, 0);

#ifndef CDROM1		
	load_vram(0x5000, lagspr_sp, 0x100);
	spr_make(0, x, 300, 0x5000, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_32x32, 0, 1);
	spr_make(1, x, 300, 0x5000, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_32x32, 0, 1);
	spr_make(2, x, 96, 0x5000, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_32x32, 0, 2);
#else
#endif
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

void ManualLagTestResults(int *clicks[])
{
	int controller;   
    int read; 
    unsigned char redraw = 1;
	unsigned char end = 0;
	int x = 0;
	int total = 0;
	int totalms = 0;
	int i = 0;
	int val = 0;
	
	end = 0;
	redraw = 1;
	x = 0;
	
	while(!end)
	{   	
		vsync();

		if(redraw)
		{
			ManualLagTestResultsBack();
			
			set_font_pal(14);
			for(i = 0; i < 10; i++)
			{
				val = clicks[i];
				if(val != 0xFF)
				{
					put_number(val, 2, 10, 8+i); 
					if(val >= 0)
					{
						total += val;
						x ++;
					}
				}
			}
			
			set_font_pal(15);
			put_string("+", 8, 14);
			put_string("----", 8, 18);
			
			totalms = total/x;
			
			set_font_pal(14);
			put_number(total, 7, 5, 19);
			put_string("/", 12, 19);
			put_number(x, 2, 13, 19);
			set_font_pal(15);
			put_string("=", 15, 19);
			set_font_pal(14);
			put_number(totalms, 2, 16, 19);
			put_string("frames", 19, 19);
			if(totalms == 1)
				put_string(" ", 19, 19);
			totalms = total/x*16;
			put_number(totalms, 2, 16, 20);
			put_string("milliseconds", 19, 20);
			
			set_font_pal(13);
			put_string("Keep in mind that a frame is", 6, 21);
			put_string("16.67 milliseconds.", 6, 22);
			
			if(total == 10)
			{
				x = 1;
				for(i = 0; i < 10; i++)
				{
					if(clicks[i] != 1)
						x = 0;
				}
				if(x)
					put_string("Smells like turbo...", 14, 13);
			}
			if(total < 5)
				put_string("EXCELLENT REFLEXES!", 14, 13);
			if(total == 0)
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
	ResetVideo();
	setupFont();
	SetFontColors(13, RGB(3, 3, 3), RGB(0, 7, 0), 0);

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
}

void ManualLagTest()
{
    int controller;   
    int read; 
    unsigned char redraw = 1;
	unsigned char end = 0;
	int x = 0;
	int y = 0;
	int x2 = 0;
	int y2 = 0;
	int clicks[10];
	int vary = 0;
	unsigned char variation = 1;
	unsigned char change = 1;
	int speed = 1;
	unsigned char audio = 1;
	unsigned char view = 0;
	unsigned char refresh = 0;
	int pos = 0;
	int i = 0;

	srand(clock_tt());
	
	for(i = 0; i < 10; i++)
		clicks[i] = 0xFF;
	
	x = 144;
	y = 60;
	x2 = 108;
	y2 = 96;
	
	LoadWave();
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			RedrawManualLagTest(x);
            redraw = 0;
			refresh = 1;
			disp_on();
        }
		
		if(refresh)
		{
			set_font_pal(13);
			put_string("Audio:", 25, 1);
			if(audio)
				put_string("on ", 32, 1); 
			else
				put_string("off", 32, 1);
				
			put_string("Timing:", 24, 2);
			if(variation)
				put_string("random  ", 32, 2); 
			else
				put_string("rhythmic", 32, 2);
		}
		
		controller = joytrg(0);
		
		if (controller & JOY_I)
		{
			if(change)
			{
				clicks[pos] = (y - 96) *speed;
	
				if(clicks[pos] >= 0)
				{
					change = 0;
					pos ++;
				}
		
				if(pos > 9)
					end = 1;
			}
		}
		
		if (controller & JOY_RUN)
		{
			showHelp(GENERAL_HELP);
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
		
		for(i = 0; i < 10; i++)
		{
			if(clicks[i] != 0xFF)
			{
				set_font_pal(14);
				put_string("Offset  :", 2, 1+i);
				put_number(i+1, 2, 8, 1+i); 
				
				if(clicks[i] >= 0)
				{
					if(clicks[i] == 0)
						set_font_pal(13);
					else
						set_font_pal(14);
				}
				else
					set_font_pal(15);
				
				put_number(clicks[i], 2, 11, 1+i); 
				put_string("frames", 14, 1+i);
				if(clicks[i] == 1 || clicks[i] == -1)
					put_string(" ", 19, 1+i);
			}
		}
		
		if(y > 132 + vary)
		{
			speed = -1;
			change = 1;
			if(variation)
			{
				if(random(1))
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
				if(random(1))
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
				PlayCenter();
			
			spr_set(0);
			spr_pal(1);
			
			spr_set(1);
			spr_pal(1);
		}
		else
		{
			if(y == 97 || y == 95) // one pixel off
			{
				StopAudio();
				
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
	
	StopAudio();
	
	if(pos > 9)
		ManualLagTestResults(clicks);
}
