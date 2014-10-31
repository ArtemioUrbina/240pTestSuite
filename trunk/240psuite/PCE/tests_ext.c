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
		case TOOL_SOUND:
			SoundTest();
			break;
		case TOOL_MANUAL:
			ManualLagTest();
			break;
		case TOOL_PASSIVE:
			LagTest();
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
			
			set_xres(256, xres_flags);
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
         
            redraw = 0;
			disp_on();
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

void VScrollTest()
{
	unsigned char end = 0;
	int x = 0;
	int y = 0;
	int *pos = 0;
	unsigned char pause = 0;
	int acc = 1;

	redraw = 1;
	pos = &x;
	speed = 1;
    while(!end)
    {   
		vsync();
        if(redraw)
        {
			ResetVideo();
			setupFont();
			
			set_screen_size(SCR_SIZE_32x32);
			scroll(0, x, y, 0, 240, 0xC0);
			set_map_data(fs_map, 40, 32);
			set_tile_data(cgrid_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 32);
			load_palette(0, check_pal, 1); 

			Center224in240(); 
         
            redraw = 0;
			disp_on();
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
	unsigned char end = 0;
	int count = 0;
	int sel = 1;

	redraw = 1;
	refresh = 0;
	
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
			cd_loaddata(GPHX_OVERLAY, OFS_mainbg_PAL_bin, palCD, SIZE_mainbg_PAL_bin); 
			set_bgpal(0, palCD); 
			cd_loadvram(GPHX_OVERLAY, OFS_mainbg_DATA_bin, 0x1000, SIZE_mainbg_DATA_bin);
			cd_loadvram(GPHX_OVERLAY, OFS_mainbg_BAT_bin, 0x0000, SIZE_mainbg_BAT_bin);
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
			showHelp(SOUND_HELP);
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


void RedrawManualLagTest()
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
	spr_make(2, x, 96, 0x5000, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_32x32, 0, 2);
	
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
	unsigned char end = 0;
	int total = 0;
	int totalms = 0;
	int val = 0;
	
	redraw = 1;
	end = 0;
	x = 0;
	
	while(!end)
	{   	
		vsync();

		if(redraw)
		{
			ManualLagTestResultsBack();
			
			set_font_pal(14);
			for(x2 = 0; x2 < 10; x2++)
			{
				val = clicks[x2];
				if(val != 0xFF)
				{
					put_number(val, 2, 10, 8+x2); 
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
				for(x2 = 0; x2 < 10; x2++)
				{
					if(clicks[x2] != 1)
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
	cd_loaddata(GPHX_OVERLAY, OFS_mainbg_PAL_bin, palCD, SIZE_mainbg_PAL_bin); 
	set_bgpal(0, palCD); 
	cd_loadvram(GPHX_OVERLAY, OFS_mainbg_DATA_bin, 0x1000, SIZE_mainbg_DATA_bin);
	cd_loadvram(GPHX_OVERLAY, OFS_mainbg_BAT_bin, 0x0000, SIZE_mainbg_BAT_bin);
#endif
	Center224in240();
}

void ManualLagTest()
{
	unsigned char end = 0;
	int pos = 0;

	x = 0;
	y = 0;
	x2 = 0;
	y2 = 0;
	
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
	
	LoadWave();
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
	for(x2 = 0; x2 < 10; x2++)
	{
		if(clicks[x2] != 0xFF)
		{
			set_font_pal(14);
			put_string("Offset  :", 2, 2+x2);
			put_number(x2+1, 2, 8, 2+x2); 
			
			if(clicks[x2] >= 0)
			{
				if(clicks[x2] == 0)
					set_font_pal(13);
				else
					set_font_pal(14);
			}
			else
				set_font_pal(15);
			
			put_number(clicks[x2], 2, 11, 2+x2); 
			put_string("frames", 14, 2+x2);
			if(clicks[x2] == 1 || clicks[x2] == -1)
				put_string(" ", 19, 2+x2);
		}
	}
}
