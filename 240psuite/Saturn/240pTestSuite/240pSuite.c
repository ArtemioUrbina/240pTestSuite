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
#include "ire.h"

#include "svin.h"
#include "background.h"
#include "pattern_colorbars.h"
#include "pattern_colorbleed.h"
#include "pattern_checkerboard.h"

#define VERSION_NUMBER "Ver. 0.0.1"
#define VERSION_DATE __DATE__

#define MENU_MAIN 0
#define MENU_PATTERNS 1
#define MENU_VIDEO_OPTIONS 2

extern uint8_t asset_bootlogo_bg[];
extern uint8_t asset_bootlogo_bg_end[];

void DrawCredits();

void wait_for_key_press()
{
	//wait for keypress
	while (controller.pressed.raw == 0)
	{
		vdp2_tvmd_vblank_out_wait();
		smpc_peripheral_process();
		smpc_peripheral_digital_port(1, &controller);
	}
}

void wait_for_key_unpress()
{
	//wait for unpress
	while (controller.pressed.raw != 0)
	{
		vdp2_tvmd_vblank_out_wait();
		smpc_peripheral_process();
		smpc_peripheral_digital_port(1, &controller);
	}
}

void wait_for_next_key()
{
	wait_for_key_unpress();
	wait_for_key_press();
	wait_for_key_unpress();
}

char * scanmode_text_value(_svin_screen_mode_t screenmode)
{
	switch (screenmode.scanmode)
	{
		case _SVIN_SCANMODE_240I:
			return "480i same fields";
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
	//ClearTextLayer();
	_svin_deinit();
	_svin_init(screenmode);
	_svin_clear_palette(0);
	LoadFont();
}

int main(void)
{
	int sel = 0;
	bool redrawMenu = true, redrawBG = true, key_pressed = false;
	int menu_id=MENU_MAIN;
	int menu_size=0;
	char string_buf[128];
	bool b = false;
	bool bExit = false;

	_svin_screen_mode_t screenMode =
	{
		.scanmode = _SVIN_SCANMODE_480I,
		.x_res = _SVIN_X_RESOLUTION_352,
		.y_res = _SVIN_Y_RESOLUTION_224,
		.x_res_doubled = true,
	};

	//show yaul logo in 480i (should switch to 240p later)
	_svin_init(screenMode);
    //_svin_background_set_no_filelist("BOOTLOGO.BG");
	_svin_background_set_from_assets(asset_bootlogo_bg,(int)(asset_bootlogo_bg_end-asset_bootlogo_bg));

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

	//detect color system
	vdp2_tvmd_tv_standard_t colorsystem = vdp2_tvmd_tv_standard_get();

	//measure frame clock
	volatile int frame_counter=0;
	while (vdp2_tvmd_vblank_out())
		;
	while (vdp2_tvmd_vblank_in())
		frame_counter++;
	while (vdp2_tvmd_vblank_out())
		frame_counter++;

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
			int x = 36, y = 56, pos = 0;
			ClearTextLayer();
			switch (menu_id)
			{
				case MENU_MAIN:
					DrawString("Test Patterns", x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;	
					DrawString("Video Options",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Help",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Credits",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					menu_size = 4;
					break;
				case MENU_PATTERNS:
					/*DrawString("Drop Shadow Test", x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Striped Sprite Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Lag Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Manual Lag Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Scroll Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Grid Scroll Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Horizontal Stripes",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;*/
					DrawString("Color Bars",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Checkerboard",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Colorbleed",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Grid",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("SMPTE",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					/*DrawString("Backlit Zone Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Sound Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Audio Sync Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;*/
					menu_size = 5;
					break;
				case MENU_VIDEO_OPTIONS:
					sprintf(string_buf,scanmode_text_value(screenMode));
					if (strlen(string_buf) > 4)
						sprintf(string_buf,"Scan mode .. %s",scanmode_text_value(screenMode));
					else
						sprintf(string_buf,"Scan mode .............. %s",scanmode_text_value(screenMode));
					DrawString(string_buf, x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					sprintf(string_buf,"Active lines ............ %s",y_lines_text_value(screenMode));
					DrawString(string_buf, x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					sprintf(string_buf,"Horisontal resoluton .... %s",x_res_text_value(screenMode));
					DrawString(string_buf, x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					if (ire_level_0 < 10)
						sprintf(string_buf,"IRE 0 level ............... %d",ire_level_0);
					else
						sprintf(string_buf,"IRE 0 level .............. %d",ire_level_0);
					DrawString(string_buf, x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					sprintf(string_buf,"IRE 100 level ........... %d",ire_level_100);
					DrawString(string_buf, x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					pos++;
					sprintf(string_buf,"Effective resolition  %sx%s",x_res_text_value(screenMode),y_res_text_value(screenMode));
					DrawString(string_buf,x, y+_fh*pos, FONT_CYAN); pos++;
					pos++;
					if (VDP2_TVMD_TV_STANDARD_NTSC == colorsystem)
					{
						DrawString("Color system ........... NTSC",x, y+_fh*pos, FONT_CYAN); pos++;
						sprintf(string_buf,"(%d)",frame_counter);
						DrawString(string_buf, x, y+_fh*pos, FONT_CYAN); pos++;
						DrawString("Field rate ......... 59.94 Hz", x, y+_fh*pos, FONT_CYAN); pos++;//59.52 Hz if hacked from PAL
						if (_SVIN_X_RESOLUTION_320 == screenMode.x_res)
							DrawString("Pixel clock ......26.8426 MHz", x, y+_fh*pos, FONT_CYAN);//1706 clock per line, 525 lines, 29.97 fps
						else
							DrawString("Pixel clock ......28.6364 MHz", x, y+_fh*pos, FONT_CYAN);//1820 clock per line, 525 lines, 29.97 fps
						pos++;
					}
					else
					{
						DrawString("Color system ............ PAL",x, y+_fh*pos, FONT_CYAN); pos++;
						sprintf(string_buf,"(%d)",frame_counter);
						DrawString(string_buf, x, y+_fh*pos, FONT_CYAN); pos++;
						DrawString("Field rate ............ 50 Hz", x, y+_fh*pos, FONT_CYAN); pos++;//50.35 Hz if hacked from NTSC
						if (_SVIN_X_RESOLUTION_320 == screenMode.x_res)
							DrawString("Pixel clock ......26.6564 MHz", x, y+_fh*pos, FONT_CYAN);//1706 clock per line, 625 lines, 25 fps
						else
							DrawString("Pixel clock ......28.4375 MHz", x, y+_fh*pos, FONT_CYAN);//1820 clock per line, 625 lines, 25 fps
						pos++;
					}
					menu_size = 5;
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

				if(controller.pressed.button.left)
				{
					if (MENU_VIDEO_OPTIONS == menu_id)
					{
						switch(sel)
						{
							case 3:
								//ire 0 level
								ire_level_0--;
								if (ire_level_0 < 0)
									ire_level_0 = 0;
								redrawMenu = true;
								break;
							case 4:
								//ire 100 level
								ire_level_100--;
								if (ire_level_100 <= ire_level_0)
									ire_level_100 = ire_level_0+1;
								redrawMenu = true;
								break;
						}
					}
					key_pressed = true;
				}

				if(controller.pressed.button.right)
				{
					if (MENU_VIDEO_OPTIONS == menu_id)
					{
						switch(sel)
						{
							case 3:
								//ire 0 level
								ire_level_0++;
								if (ire_level_0 >= ire_level_100)
									ire_level_0 = ire_level_100-1;
								redrawMenu = true;
								break;
							case 4:
								//ire 100 level
								ire_level_100++;
								if (ire_level_100 > 255)
									ire_level_100 = 255;
								redrawMenu = true;
								break;
						}
					}
					key_pressed = true;
				}

				if ( (controller.pressed.button.a) || (controller.pressed.button.c) )
				{
					if (MENU_MAIN == menu_id)
					{
						switch(sel)
						{
							case 0:
								//go into patterns
								menu_id = MENU_PATTERNS;
								sel = 0;
								redrawMenu = true;
								break;							
							case 1:
								//go into video settings
								menu_id = MENU_VIDEO_OPTIONS;
								sel = 0;
								redrawMenu = true;
								break;
							case 3:
								//go to credits
								vdp2_sync_wait();
								wait_for_key_unpress();
								DrawCredits();
								redrawMenu = true;
								break;
						}
					}
					else if (MENU_PATTERNS == menu_id)
					{
						switch(sel)
						{
							case 0:
								//colorbars
								draw_colorbars(screenMode);
								wait_for_next_key();
								redrawMenu = true;
								break;							
							case 1:
								//checkerboard
								pattern_checkerboard(screenMode);
								redrawMenu = true;
								break;
							case 2:
								//colorbleed
								b = false;
								bExit = false;
								while (false == bExit) {
									draw_colorbleed(screenMode,b);
									wait_for_key_unpress();
									wait_for_key_press();
									if (controller.pressed.button.a)
									{
										b = !b;
									}
									else
									{
										bExit = true;
									}
								}
								wait_for_key_unpress();
								redrawMenu = true;
								break;
							case 3:
								//checkerboard
								draw_grid(screenMode);
								wait_for_next_key();
								redrawMenu = true;
								break;
							case 4:
								//SMPTE
								b = false;
								bExit = false;
								while (false == bExit) {
									draw_smpte(screenMode,b);
									wait_for_key_unpress();
									wait_for_key_press();
									if (controller.pressed.button.a)
									{
										b = !b;
									}
									else
									{
										bExit = true;
									}
								}
								wait_for_key_unpress();
								update_screen_mode(screenMode);
								redrawBG = true;
								redrawMenu = true;
								break;
							default:
								//checkerboard
								pattern_checkerboard(screenMode);
								redrawMenu = true;
								break;
						}
					}
					else if (MENU_VIDEO_OPTIONS == menu_id)
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
							case 3:
								//ire 0 level
								ire_level_0++;
								if (ire_level_0 >= ire_level_100)
									ire_level_0 = ire_level_100-1;
								redrawMenu = true;
								break;
							case 4:
								//ire 100 level
								ire_level_100++;
								if (ire_level_100 > 255)
									ire_level_100 = 255;
								redrawMenu = true;
								break;
						}
					}
					redrawBG = true;
					key_pressed = true;
				}

				if(controller.pressed.button.start)
				{
					//exit to top level
					menu_id = MENU_MAIN;
					sel = 0;
					redrawMenu = true;
					key_pressed = true;
				}

				if(controller.pressed.button.b)
				{
					vdp2_sync_wait();

					switch(menu_id)
					{
						case MENU_MAIN:
							//go to credits
							vdp2_sync_wait();
							wait_for_key_unpress();
							DrawCredits();
							redrawMenu = true;
							break;
						case MENU_PATTERNS:
							menu_id = MENU_MAIN;
							sel = 0;
							break;
						case MENU_VIDEO_OPTIONS:
							menu_id = MENU_MAIN;
							sel = 1;
							break;
					}
					redrawMenu = true;
					key_pressed = true;
				}
			}
		}
		vdp2_sync();
		vdp2_sync_wait();
	}
}

void DrawCredits()
{
	bool doexit = false;
	char data[25];
	int counter = 1;
	int x = 32, y = 48, pos = 0;

	ClearTextLayer();
	
	DrawString("Code and Patterns:", x, y+_fh*pos, FONT_GREEN); pos++;
	DrawString(VERSION_NUMBER, 208, y+_fh*pos, FONT_GREEN);
	DrawString("Artemio Urbina", x+5, y+_fh*pos, FONT_WHITE); pos++;
	DrawString(VERSION_DATE, 208, y+_fh*pos, FONT_WHITE); 
	pos++;
	DrawString("SDK & Consultant:", x, y+_fh*pos, FONT_GREEN); pos++;	
	DrawString("libyaul by Israel Jacquez (yaul.org)", x+5, y+_fh*pos, FONT_WHITE); pos++;
	DrawString("USB DevCart:", x, y+_fh*pos, FONT_GREEN); pos++;	
	DrawString("cafe-alpha", x+5, y+_fh*pos, FONT_WHITE); pos++;	
	DrawString("Menu Pixel Art:", x, y+_fh*pos, FONT_GREEN); pos++;	
	DrawString("Asher", x+5, y+_fh*pos, FONT_WHITE); pos++;	
	DrawString("Advisor:", x, y+_fh*pos, FONT_GREEN); pos++;	
	DrawString("Fudoh", x+5, y+_fh*pos, FONT_WHITE); pos++;	
	DrawString("Info on using this suite:", x, y+_fh*pos, FONT_GREEN); pos++;	
	DrawString("http://junkerhq.net/240p/", x+5, y+_fh*pos, FONT_WHITE); pos++;	
	DrawString("This is free software.", x+5, y+_fh*pos, FONT_CYAN); pos++;	
	DrawString("Source code available under GPL", x+5, y+_fh*pos, FONT_CYAN); pos++;
	DrawString("Dedicated to Elisa", 154, y+_fh*pos, FONT_GREEN); 

	while(!doexit)
	{
		int x = 32, y = 48, pos = 0;

		smpc_peripheral_process();
		smpc_peripheral_digital_port(1, &controller);

		if(counter == 1)
			snprintf(data, 25, "aurbina@junkerhq.net");
		if(counter == 60*4)
			snprintf(data, 25, "@Artemio (twitter)");
		if(counter == 60*8)
			counter = 0;
	
		pos = 2;
		ClearText(x+5, y+_fh*pos, 25*_fw, _fh);
		DrawString(data, x+5, y+_fh*pos, FONT_WHITE); pos++;	
	
		if (controller.connected == 1)
		{
			if(controller.pressed.raw)
				doexit = true;
		}
	
		vdp2_sync();
		vdp2_sync_wait();
		counter ++;
	}

	return;
}


void print_screen_mode(_svin_screen_mode_t screenmode)
{
	char buf[128];
	strcpy(buf, "");
	strcat(buf, scanmode_text_value(screenmode));
	strcat(buf, " ");
	strcat(buf, x_res_text_value(screenmode));
	strcat(buf, "x");
	strcat(buf, y_res_text_value(screenmode));
	strcat(buf, "");
	DrawStringWithBackground(buf, 160-strlen(buf)*_fw/2, 120, FONT_WHITE,FONT_YELLOW);
}