/* 
 * 240p Test Suite for Nintendo 64
 * Copyright (C)2018 Artemio Urbina
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	02111-1307	USA

 * 240p Test Suite for Sega Saturn
 * Copyright (c) 2018 Artemio Urbina
 * See LICENSE for details.
 * Uses libyaul
 */

#include <yaul.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "fs.h"
#include "font.h"
#include "image.h"
#include "video.h"
#include "control.h"

#include "svin.h"

#define VERSION_NUMBER "Ver. 0.0.1"
#define VERSION_DATE "15/3/2018"

void DrawCredits();

int main(void)
{
	int sel = 0;
	bool redrawMenu = true, redrawBG = true;

	extern int _svin_videomode_x_res;
	extern int _svin_videomode_y_res;
	_svin_x_resolution_t X_Res = _SVIN_X_RESOLUTION_704;
    _svin_y_resolution_t Y_Res = _SVIN_Y_RESOLUTION_448;
    bool scanlines = false;

	_svin_init(X_Res,Y_Res,scanlines);
    _svin_textbox_disable(); //filling textbox tiles with invisible data
    _svin_background_set_no_filelist("BOOTLOGO.BG");
	_svin_delay(1000);
	_svin_background_fade_to_black();


	DrawCredits();
	while(1);

	//InitVideo();
	//InitControllers();

	//LoadFont();

	//if(!fs_init())
	//	DrawString("FS INIT FAILED!\n", 120, 20, 1);

	while(true)
	{
		vdp2_tvmd_vblank_out_wait();
		smpc_peripheral_digital_port(1, &controller);

		if(redrawBG)
		{
			DisplayImage("/BACK.TGA", S_NBG0, 512, 256, 1, false);
			redrawBG = false;
		}

		if(redrawMenu)
		{
			int x = 36, y = 56, pos = 0;

			//vdp1_cmdt_list_begin(0);
			DrawString("Test Patterns", x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;	
			DrawString("Drop Shadow Test", x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
			DrawString("Striped Sprite Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
			DrawString("Lag Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
			DrawString("Manual Lag Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
			DrawString("Scroll Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
			DrawString("Grid Scroll Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
			DrawString("Horizontal Stripes",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
			DrawString("Checkerboard",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
			DrawString("Backlit Zone Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
			DrawString("Sound Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
			DrawString("Audio Sync Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
			DrawString("Video Options",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
			DrawString("Help",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
			DrawString("Credits",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
			//vdp1_cmdt_list_end(0);
			redrawMenu = false;
		}

		if (controller.connected == 1)
		{
			if(controller.pressed.button.up)
			{
				sel --;
				if(sel < 0)
					sel = 15;
				redrawMenu = true;
			}

			if(controller.pressed.button.down)
			{
				sel ++;
				if(sel > 15)
					sel = 0;
				redrawMenu = true;
			}

			if(controller.pressed.button.a)
			{
				redrawBG = true;
			}

			if(controller.pressed.button.start)
				abort();

			if(controller.pressed.button.b)
			{
				vdp2_tvmd_vblank_in_wait();
				//vdp1_cmdt_list_commit();

				//vdp2_scrn_display_clear();

				DrawCredits();

				//vdp2_scrn_display_clear();
				//redrawBG = true;
				redrawMenu = true;
			}
		}
		vdp2_tvmd_vblank_in_wait();
		//vdp1_cmdt_list_commit();
	}
}

void DrawCredits()
{
	bool doexit = false;
	char data[25];
	int counter = 1;

	//DisplayImage("/BACK.TGA", S_NBG0, 512, 256, 1, false);

	_svin_x_resolution_t X_Res = _SVIN_X_RESOLUTION_320;
    _svin_y_resolution_t Y_Res = _SVIN_Y_RESOLUTION_240;
    bool scanlines = true;

	_svin_init(X_Res,Y_Res,scanlines);

	_svin_background_set_no_filelist("BACK320.BG");
/*
	while(!doexit)
	{
		int x = 32, y = 56, pos = 0;

		vdp2_tvmd_vblank_out_wait();
		smpc_peripheral_digital_port(1, &controller);

		//vdp1_cmdt_list_begin(0);
		DrawString("Code and Patterns:", x, y+_fh*pos, FONT_GREEN); pos++;	
		DrawString("Artemio Urbina", x+5, y+_fh*pos, FONT_WHITE); pos++;	
		
		if(counter == 1)
			snprintf(data, 25, "aurbina@junkerhq.net");
		if(counter == 60*4)
			snprintf(data, 25, "@Artemio (twitter)");
		if(counter == 60*8)
			counter = 0;
	
		DrawString(data, x+5, y+_fh*pos, FONT_WHITE); pos++;	
		DrawString("SDK & Consultant:", x, y+_fh*pos, FONT_GREEN); pos++;	
		DrawString("libyaul by Israel Jacquez", x+5, y+_fh*pos, FONT_WHITE); pos++;	

		DrawString("USB DevCart:", x, y+_fh*pos, FONT_GREEN); pos++;	
		DrawString("cafe-alpha", x+5, y+_fh*pos, FONT_WHITE); pos++;	
		DrawString("Menu Pixel Art:", x, y+_fh*pos, FONT_GREEN); pos++;	
		DrawString("Asher", x+5, y+_fh*pos, FONT_WHITE); pos++;	
		DrawString("Advisor:", x, y+_fh*pos, FONT_GREEN); pos++;	
		DrawString("Fudoh", x+5, y+_fh*pos, FONT_WHITE); pos++;	
	
		DrawString("Info on using this suite:", x, y+_fh*pos, FONT_GREEN); pos++;	
		DrawString("http://junkerhq.net/240p/", x+5, y+_fh*pos, FONT_WHITE); pos++;	
	
		DrawString("This is free software.", x+5, y+_fh*pos, FONT_CYAN); pos++;	
		DrawString("Source code available under GPL", x+5, y+_fh*pos, FONT_CYAN);
	
		pos = 0;
		DrawString(VERSION_NUMBER, 208, y+_fh*pos, FONT_GREEN); pos++;	
		DrawString(VERSION_DATE, 208, y+_fh*pos, FONT_WHITE); 
	
		y += _fh*18;
		DrawString("Dedicated to Elisa", 154, y, FONT_GREEN); 
		//vdp1_cmdt_list_end(0);
	
		if (controller.connected == 1)
		{
			if(controller.pressed.button.c)
				doexit = true;
		}
	
		vdp2_tvmd_vblank_in_wait();
		//vdp1_cmdt_list_commit();
		counter ++;
	}*/

	return;
}
