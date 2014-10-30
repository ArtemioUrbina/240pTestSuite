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

#ifdef CDROM1
extern int xres_flags;
extern unsigned char Enabled240p;
extern unsigned char UseDefault;
extern unsigned char EnabledSoft;
extern unsigned char Enabled_C_BW;
#endif

void DrawCheck()
{
	unsigned char end = 0;
	unsigned char alternate = 0;
	unsigned char pos = 0;
	unsigned char drawframe = 0;
	int frame = 0;

	redraw = 1;
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			RedrawCheck();
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

void RedrawCheck()
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
}


void DrawStripes()
{
	unsigned char end = 0;
	unsigned char alternate = 0;
	unsigned char pos = 0;
	unsigned char drawframe = 0;
	int frame = 0;

	redraw = 1;
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			RedrawStripes();
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
			showHelp(STRIPES_HELP);
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

void RedrawStripes()
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
}

void DropShadow()
{
	unsigned char end = 0;
	unsigned char show = 1;
	unsigned char back = 0;
	int colswap = 0;

	text = 0;
	redraw = 1;
	x = 144;
	y = 100;
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
			cd_loadvram(GPHX_OVERLAY, OFS_motoko_DATA_bin, 0x1000, SIZE_motoko_DATA_bin);
			cd_loadvram(GPHX_OVERLAY, OFS_motoko_BAT_bin, 0, SIZE_motoko_BAT_bin);
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
			cd_loadvram(GPHX_OVERLAY, OFS_sonic_DATA_bin, 0x1000, SIZE_sonic_DATA_bin);
			cd_loadvram(GPHX_OVERLAY, OFS_sonic_BAT_bin, 0, SIZE_sonic_BAT_bin);
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
	unsigned char end = 0;
	unsigned char back = 0;
	int colswap = 0;

	x = 144;
	y = 100;
	redraw = 1;
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {	
			RefreshStriped(back);
			
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
			showHelp(STRIPED_HELP);
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

void RefreshStriped(char back)
{
	ResetVideo();
	switch(back)
	{
		case 0:
#ifndef CDROM1
			load_background(motoko_bg, motoko_pal, motoko_map, 40, 30);
#else
			set_screen_size(SCR_SIZE_64x32); 
			cd_loadvram(GPHX_OVERLAY, OFS_motoko_DATA_bin, 0x1000, SIZE_motoko_DATA_bin);
			cd_loadvram(GPHX_OVERLAY, OFS_motoko_BAT_bin, 0, SIZE_motoko_BAT_bin);
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
			cd_loadvram(GPHX_OVERLAY, OFS_sonic_DATA_bin, 0x1000, SIZE_sonic_DATA_bin);
			cd_loadvram(GPHX_OVERLAY, OFS_sonic_BAT_bin, 0, SIZE_sonic_BAT_bin);
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
	int vram = 0x5000;
	int pos = 0;
	unsigned char count = 0;
	
#ifndef CDROM1
	load_palette(16, palm_pal, 1);
	load_vram(vram, palm_sp, 0x8C0);
#else
#endif

	x2 = 500;
	y2 = 104;
	for(row = 0; row < 7; row++)
	{
		for(pos = 0; pos < 5; pos++)
		{
			spr_make(count+2, pos*16+x2, row*16+y2, 0x40*count+vram, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_16x16, 0, 1);
			count ++;
		}
	}
}

void MovePalm(int x)
{
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
	unsigned char end = 0;
	unsigned char pause = 0;
	int dir = 1;
	int spd = 1;
	int colswap = 0;

	x1 = 0;
	x2 = 0;
	x3 = 0;
	x4 = 0;
	
	y = 0;
	
	redraw = 1;
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
			cd_loadvram(GPHX_OVERLAY, OFS_sonic_DATA_bin, 0x1000, SIZE_sonic_DATA_bin);
			cd_loadvram(GPHX_OVERLAY, OFS_sonic_BAT_bin, 0, SIZE_sonic_BAT_bin);
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
			showHelp(SCROLL_HELP);
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
	unsigned char end = 0;
	int sel = 0;
	unsigned char refresh = 0;
	unsigned char visible = 1;
	int size[4];

	x = 144;
	y = 120;
	redraw = 1;
	
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

void LagTest()
{
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

	redraw = 1;
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

#ifndef CDROM1
#include "tests_ext.c"
#else

extern int HelpItem;
extern int ToolItem;

void VScrollTest()
{
	ToolItem = TOOL_VSCROLL;
	cd_execoverlay(TEST_EXT_OVERLAY);
}

void SoundTest()
{
	ToolItem = TOOL_SOUND;
	cd_execoverlay(TEST_EXT_OVERLAY);
}

void ManualLagTest()
{
	ToolItem = TOOL_MANUAL;
	cd_execoverlay(TEST_EXT_OVERLAY);
}

#endif