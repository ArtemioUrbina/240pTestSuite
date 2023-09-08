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
#define VERSION_DATE __DATE__

void DrawCredits();

char * scanmode_text_value(_svin_screen_mode_t screenmode)
{
	switch (screenmode.scanmode)
	{
		case _SVIN_SCANMODE_240I:
			return "240i";
			break;
		case _SVIN_SCANMODE_240P:
			return "240p";
			break;
		case _SVIN_SCANMODE_480I:
			return "480i";
			break;
		/*case _SVIN_SCANMODE_480P:
			return "480p";
			break;*/
	}
	return "????";
}

char * x_res_text_value(_svin_screen_mode_t screenmode)
{
	switch (screenmode.x_res)
	{
		case _SVIN_X_RESOLUTION_320:
			return (screenmode.x_res_doubled) ? "640":"320";
			break;
		case _SVIN_X_RESOLUTION_352:
			return (screenmode.x_res_doubled) ? "704":"352";
			break;
	}
	return "???";
}

char * y_res_text_value(_svin_screen_mode_t screenmode)
{
	switch (screenmode.y_res)
	{
		case _SVIN_Y_RESOLUTION_224:
			return (_SVIN_SCANMODE_480I == screenmode.scanmode) ? "448":"224";
			break;
		case _SVIN_Y_RESOLUTION_240:
			return (_SVIN_SCANMODE_480I == screenmode.scanmode) ? "480":"240";
			break;
		case _SVIN_Y_RESOLUTION_256:
			return (_SVIN_SCANMODE_480I == screenmode.scanmode) ? "512":"256";
			break;
	}
	return "???";
}

char * y_lines_text_value(_svin_screen_mode_t screenmode)
{
	switch (screenmode.y_res)
	{
		case _SVIN_Y_RESOLUTION_224:
			return "224";
			break;
		case _SVIN_Y_RESOLUTION_240:
			return "240";
			break;
		case _SVIN_Y_RESOLUTION_256:
			return "256";
			break;
	}
	return "????";
}

void update_screen_mode(_svin_screen_mode_t screenmode)
{
	_svin_deinit();
	_svin_init(screenmode);
	_svin_clear_palette(0);
	LoadFont();
}

void draw_bg_with_expansion(_svin_screen_mode_t screenmode)
{
	int stretch_amount_x = 0; //stretch amount in quads
	int stretch_amount_y = 0;
	//load the image with minimal resolution 320x224
	_svin_background_set_no_filelist("BACK2.BG");

	//now mess with VDP2 tiles to stretch the image among the screen accordingly
	//the image is broken into 4 tile blocks as a 2x2 table, 256+64(32+8) horisontally, 128+96(16+12) vertically
	//left table elements are stretched (rightmost tile copied) right to fit
	//right table elements are shifted accordingly
	//top elements are stretched
	//bottom elements are shifted
	//to prevent data corruption, we shift first and stretch later
	//step 1 - deciding shift and stretch amount
	switch (screenmode.x_res)
	{
		case _SVIN_X_RESOLUTION_320:
			if (screenmode.x_res_doubled)
				stretch_amount_x = 40;
			else
				stretch_amount_x = 0;
			break;
		case _SVIN_X_RESOLUTION_352:
			if (screenmode.x_res_doubled)
				stretch_amount_x = 48;
			else
				stretch_amount_x = 4;
			break;
	}
	switch (screenmode.y_res)
	{
		case _SVIN_Y_RESOLUTION_224:
			if (screenmode.scanmode == _SVIN_SCANMODE_480I)
				stretch_amount_y = 28;
			else
				stretch_amount_y = 0;
			break;
		case _SVIN_Y_RESOLUTION_240:
			if (screenmode.scanmode == _SVIN_SCANMODE_480I)
				stretch_amount_y = 32;
			else
				stretch_amount_y = 2;
			break;
		case _SVIN_Y_RESOLUTION_256:
			if (screenmode.scanmode == _SVIN_SCANMODE_480I)
				stretch_amount_y = 36;
			else
				stretch_amount_y = 4;
			break;
	}

	//step 2 - shifting x
	_svin_set_cycle_patterns_cpu();
    int *_pointer32 = (int *)_SVIN_NBG0_PNDR_START;
    for (int y = 0; y < 30 ; y++)
    {
    	for (int x = 7; x >= 0; x--)
        {
			if (stretch_amount_x < 40)   
            	_pointer32[y*64+32+x+stretch_amount_x] = _pointer32[y*64+32+x];
			else
            	_pointer32[64*63 + y*64+32+x+stretch_amount_x] = _pointer32[y*64+32+x];
        }
    }

	//step 3 - stretching x
    for (int y = 0; y < 30 ; y++)
    {
		if (stretch_amount_x < 40) 
		{
			for (int x = 0; x < stretch_amount_x; x++)
				_pointer32[y*64+32+x] = _pointer32[y*64+31];
		}
		else
		{
			for (int x = 0; x < 32; x++)
				_pointer32[y*64+32+x] = _pointer32[y*64+31];
			for (int x = 0; x <= (stretch_amount_x-32); x++)
				_pointer32[64*64 + y*64+x] = _pointer32[y*64+31];			
		}
    }

	//step 4 - shifting y
	if (stretch_amount_x < 40) 
	{	
		for (int x = 0; x < 40 + stretch_amount_x ; x++)
		{
			for (int y = 27; y >= 16; y--)
				_pointer32[(y+stretch_amount_y)*64+x] = _pointer32[y*64+x];
		}
	}
	else
	{
		for (int x = 0; x < 64 ; x++)
		{
			for (int y = 27; y >= 16; y--)
				_pointer32[(y+stretch_amount_y)*64+x] = _pointer32[y*64+x];
		}
		for (int x = 0; x < stretch_amount_x-24 ; x++)
		{
			for (int y = 27; y >= 16; y--)
				_pointer32[64*64 + (y+stretch_amount_y)*64+x] = _pointer32[64*64 + y*64+x];
		}
	}

	//step 5 - stretching y
	if (stretch_amount_x < 40) 
	{	
		for (int x = 0; x < 40 + stretch_amount_x ; x++)
		{
			for (int y = 0; y < stretch_amount_y; y++)
				_pointer32[(y+16)*64+x] = _pointer32[15*64+x];
		}
	}
	else
	{
		for (int x = 0; x < 64 ; x++)
		{
			for (int y = 0; y < stretch_amount_y; y++)
				_pointer32[(y+16)*64+x] = _pointer32[15*64+x];
		}
		for (int x = 0; x < stretch_amount_x-24 ; x++)
		{
			for (int y = 0; y < stretch_amount_y; y++)
				_pointer32[64*64+(y+16)*64+x] = _pointer32[64*64+15*64+x];
		}
	}

    _svin_set_cycle_patterns_nbg();
}

void draw_checkerboard(_svin_screen_mode_t screenmode)
{
	//removing text
	ClearTextLayer();
	
	_svin_set_cycle_patterns_cpu();
	//add colors to palette
	uint16_t *_pointer16 = (uint16_t *)VDP2_CRAM(0x200*2);
	_pointer16[1] = 0x0000;
	_pointer16[2] = 0xFFFF;
	//create checkerboard tile at index 0
	int *_pointer32 = (int *)_SVIN_NBG0_CHPNDR_START;
	for (int i=0; i<4; i++)
	{
		_pointer32[i*4  ] = 0x01020102;
		_pointer32[i*4+1] = 0x01020102;
		_pointer32[i*4+2] = 0x02010201;
		_pointer32[i*4+3] = 0x02010201;
	}
	//fill everything with this tile
    _pointer32 = (int *)_SVIN_NBG0_PNDR_START;
    for (unsigned int i = 0; i < _SVIN_NBG0_PNDR_SIZE / sizeof(int); i++)
    //for (unsigned int i = 0; i < 64; i++)
    {
        //_pointer32[i] = 0x00000000 + _SVIN_NBG0_CHPNDR_SPECIALS_INDEX; //palette 0, transparency on
        _pointer32[i] = 0x00200000; //palette 2, transparency on
    }
	_svin_set_cycle_patterns_nbg();
	//wait for unpress
	while (controller.pressed.raw != 0)
	{
		vdp2_tvmd_vblank_out_wait();
		smpc_peripheral_process();
		smpc_peripheral_digital_port(1, &controller);
	}
	//wait for keypress
	while (controller.pressed.raw == 0)
	{
		vdp2_tvmd_vblank_out_wait();
		smpc_peripheral_process();
		smpc_peripheral_digital_port(1, &controller);
	}
	//wait for unpress
	while (controller.pressed.raw != 0)
	{
		vdp2_tvmd_vblank_out_wait();
		smpc_peripheral_process();
		smpc_peripheral_digital_port(1, &controller);
	}
}

int main(void)
{
	int sel = 0;
	bool redrawMenu = true, redrawBG = true, key_pressed = false;
	int menu_id=0;
	int menu_size=0;
	char string_buf[128];

	_svin_screen_mode_t screenMode =
	{
		.scanmode = _SVIN_SCANMODE_480I,
		.x_res = _SVIN_X_RESOLUTION_352,
		.y_res = _SVIN_Y_RESOLUTION_224,
		.x_res_doubled = true,
	};

	//show yaul logo in 480i (should switch to 240p later)
	_svin_init(screenMode);
    //_svin_textbox_disable(); //filling textbox tiles with invisible data
    _svin_background_set_no_filelist("BOOTLOGO.BG");
	_svin_delay(1000);
	_svin_background_fade_to_black();

	//reset to 240p
	_svin_deinit();
	screenMode.scanmode = _SVIN_SCANMODE_240P;
	screenMode.x_res = _SVIN_X_RESOLUTION_320;
    screenMode.y_res = _SVIN_Y_RESOLUTION_240;
	screenMode.x_res_doubled = false;
	_svin_init(screenMode);

	_svin_clear_palette(0);
	LoadFont();

	//if(!fs_init())
	//	DrawString("FS INIT FAILED!\n", 120, 20, 1);
	redrawMenu = true;
	redrawBG = true;

	while(true)
	{
		vdp2_tvmd_vblank_out_wait();
		smpc_peripheral_process();
		smpc_peripheral_digital_port(1, &controller);

		if(redrawBG)
		{
			draw_bg_with_expansion(screenMode);
			redrawBG = false;
		}

		if(redrawMenu)
		{
			int x = 36, y = 104, pos = 0;
			ClearTextLayer();
			switch (menu_id)
			{
				case 0:
					/*DrawString("Test Patterns", x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;	
					DrawString("Drop Shadow Test", x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Striped Sprite Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Lag Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Manual Lag Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Scroll Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Grid Scroll Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Horizontal Stripes",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;*/
					DrawString("Checkerboard",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					/*DrawString("Backlit Zone Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Sound Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Audio Sync Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;*/
					DrawString("Video Options",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Help",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Credits",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					menu_size = 4;//15;
					break;
				case 1:
					sprintf(string_buf,"Scan mode .............. %s",scanmode_text_value(screenMode));
					DrawString(string_buf, x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					sprintf(string_buf,"Active lines ............ %s",y_lines_text_value(screenMode));
					DrawString(string_buf, x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					sprintf(string_buf,"Horisontal resoluton .... %s",x_res_text_value(screenMode));
					DrawString(string_buf, x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					pos++;
					sprintf(string_buf,"Effective resolition  %sx%s",x_res_text_value(screenMode),y_res_text_value(screenMode));
					DrawString(string_buf,x, y+_fh*pos, FONT_CYAN); pos++;
					pos++;
					DrawString("Color system ........... ????",x, y+_fh*pos, FONT_CYAN); pos++;
					DrawString("Frame clock ........ ??.?? Hz", x, y+_fh*pos, FONT_CYAN); pos++;	
					DrawString("Pixel clock ........ ??.? MHz", x, y+_fh*pos, FONT_CYAN); pos++;	
					menu_size = 3;
					break;
				default:
					DrawString("Exit", x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					break;
			}
			redrawMenu = false;
		}

		if (controller.connected == 1)
		{
			if (key_pressed == true)
			{
				//waiting for key unpress and not moving further until it's unpressed
				if (controller.pressed.raw == 0)
					key_pressed = false;
			}
			else
			{

				if(controller.pressed.button.up)
				{
					sel --;
					if(sel < 0)
						sel = menu_size - 1;
					redrawMenu = true;
					key_pressed = true;
				}

				if(controller.pressed.button.down)
				{
					sel ++;
					if(sel >= menu_size)
						sel = 0;
					redrawMenu = true;
					key_pressed = true;
				}

				if(controller.pressed.button.a)
				{
					if (0 == menu_id)
					{
						switch(sel)
						{
							case 0:
								//go into video settings
								draw_checkerboard(screenMode);
								redrawMenu = true;
								break;							
							case 1:
								//go into video settings
								menu_id = 1;
								sel = 0;
								redrawMenu = true;
								break;
							case 3:
								//go to credits
								vdp2_sync_wait();
								DrawCredits();
								redrawMenu = true;
								break;
						}
					}
					else if (1 == menu_id)
					{
						switch(sel)
						{
							case 0:
								//change scanmode
								screenMode.scanmode++;
								if (screenMode.scanmode > _SVIN_SCANMODE_480I)
									screenMode.scanmode = _SVIN_SCANMODE_240I;
								update_screen_mode(screenMode);
								redrawBG = true;
								redrawMenu = true;
								break;
							case 1:
								//y lines
								screenMode.y_res++;
								if (screenMode.y_res > _SVIN_Y_RESOLUTION_240) //no 256 for now
								{
									screenMode.y_res = _SVIN_Y_RESOLUTION_224;
								}
								update_screen_mode(screenMode);
								redrawBG = true;
								redrawMenu = true;
								break;
							case 2:
								//x res
								screenMode.x_res++;
								if (screenMode.x_res > _SVIN_X_RESOLUTION_352)
								{
									screenMode.x_res = _SVIN_X_RESOLUTION_320;
									screenMode.x_res_doubled = !screenMode.x_res_doubled;
								}
								update_screen_mode(screenMode);
								redrawBG = true;
								redrawMenu = true;
								break;
						}
					}
					redrawBG = true;
					key_pressed = true;
				}

				if(controller.pressed.button.start)
					abort();

				if(controller.pressed.button.b)
				{
					//vdp2_tvmd_vblank_in_wait();
					vdp2_sync_wait();
					//vdp1_cmdt_list_commit();

					//vdp2_scrn_display_clear();

					if (0 == menu_id)
						DrawCredits();
					else
						menu_id = 0;

					//vdp2_scrn_display_clear();
					//redrawBG = true;
					redrawMenu = true;
					key_pressed = true;
				}
			}
		}
		//vdp2_tvmd_vblank_in_wait();
		//vdp1_cmdt_list_commit();
		vdp2_sync();
		vdp2_sync_wait();
	}
}

void DrawCredits()
{
	bool doexit = false;
	char data[25];
	int counter = 1;

	ClearTextLayer();

	_svin_background_set_no_filelist("BACK2.BG");

	while(!doexit)
	{
		int x = 32, y = 104, pos = 0;

		smpc_peripheral_process();
		smpc_peripheral_digital_port(1, &controller);

		DrawString("Code and Patterns:", x, y+_fh*pos, FONT_GREEN); pos++;	
		DrawString("Artemio Urbina", x+5, y+_fh*pos, FONT_WHITE); pos++;	
		
		if(counter == 1)
			snprintf(data, 25, "aurbina@junkerhq.net");
		if(counter == 60*4)
			snprintf(data, 25, "@Artemio (twitter)");
		if(counter == 60*8)
			counter = 0;
	
		ClearText(x+5, y+_fh*pos, 25*_fw, _fh);
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
			if(controller.pressed.raw)
				doexit = true;
		}
	
		//vdp2_tvmd_vblank_in_wait();
		//vdp1_cmdt_list_commit();
		vdp2_sync();
		vdp2_sync_wait();
		counter ++;
	}

	return;
}
