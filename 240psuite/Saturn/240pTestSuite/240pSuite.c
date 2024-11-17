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
#include "input.h"
#include "video.h"
#include "control.h"
#include "ire.h"
#include "video_vdp2.h"
#include "background.h"

#include "pattern_100ire.h"
#include "pattern_colorbars.h"
#include "pattern_colorbars_grayscale.h"
#include "pattern_colorbleed.h"
#include "pattern_convergence.h"
#include "pattern_ebu.h"
#include "pattern_pluge.h"
#include "pattern_grayramp.h"
#include "pattern_grid.h"
#include "pattern_monoscope.h"
#include "pattern_overscan.h"
#include "pattern_smpte.h"
#include "pattern_sharpness.h"
#include "pattern_whitescreen.h"

#include "videotest_backlit_zone.h"
#include "videotest_checkerboard.h"
#include "videotest_dropshadow.h"
#include "videotest_grid_scroll.h"
#include "videotest_striped_sprite.h"
#include "videotest_hor_vert_stripes.h"

#include "window_credits.h"

#define MENU_MAIN 0
#define MENU_PATTERNS 1
#define MENU_PATTERNS_COLOR_LEVELS 2
#define MENU_PATTERNS_GEOMETRY 3
#define MENU_VIDEO_TESTS 4
#define MENU_AUDIO_TESTS 5
#define MENU_HARDWARE_TESTS 6
#define MENU_CONFIGURATION 7
#define MENU_VIDEO_TEST_SCROLL_HV_SELECT 8

extern uint8_t asset_bootlogo_bg[];
extern uint8_t asset_bootlogo_bg_end[];

static smpc_peripheral_digital_t _digital;
int global_frame_count;

static void suite_vblank_out_handler(void *work __unused)
{
    global_frame_count++;
    
    if (0==video_is_inited())
        return;
    
    smpc_peripheral_intback_issue();
}

int main(void)
{
	int sel = 0;
	bool redrawMenu = true, redrawBG = true, key_pressed = false;
	int menu_id=MENU_MAIN;
	int menu_size=0;
	char string_buf[128];

	video_screen_mode_t screenMode =
	{
		.scanmode = VIDEO_SCANMODE_240P,
		.x_res = VIDEO_X_RESOLUTION_320,
		.y_res = VDP2_TVMD_VERT_240,
		.x_res_doubled = false,
		.colorsystem = VDP2_TVMD_TV_STANDARD_NTSC,
	};

	//show yaul logo in 240p
	video_init(screenMode,false);
	video_vdp2_set_cycle_patterns_cpu();
	background_set_from_assets(asset_bootlogo_bg,(int)(asset_bootlogo_bg_end-asset_bootlogo_bg),VIDEO_VDP2_NBG0_PNDR_START,VIDEO_VDP2_NBG0_CHPNDR_START);
	video_vdp2_set_cycle_patterns_nbg(screenMode);

	//wait for 60 frames, either 1s or 1.2s
	for (int i=0;i<60;i++)
		vdp2_tvmd_vblank_in_next_wait(1);

	background_fade_to_black();

	video_vdp2_clear_palette(0);
	SetFontPalette();

	//detect color system
	screenMode.colorsystem = vdp2_tvmd_tv_standard_get();

	//debug force mode
	//screenMode.scanmode = VIDEO_SCANMODE_480P;

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

	//register vblank handler
	vdp_sync_vblank_out_set(suite_vblank_out_handler, NULL);

	while(true)
	{
		vdp2_tvmd_vblank_out_wait();
		smpc_peripheral_process();
		get_digital_keypress_anywhere(&controller);

		if(redrawBG)
		{
			draw_bg_with_expansion(screenMode,true);
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
					DrawString("Video Tests", x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;	
					//DrawString("Audio Tests",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Hardware Tests",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					pos++;
					DrawString("Configuration",x, y+_fh*pos, sel == pos-1 ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Credits",x, y+_fh*pos, sel == pos-1 ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Help",x, y+_fh*pos, sel == pos-1 ? FONT_RED : FONT_WHITE); pos++;
					menu_size = 6;
					break;
				case MENU_PATTERNS:
					DrawString("Color & Black Levels",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Geometry",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					pos++;
					DrawString("Back to Main Menu",x, y+_fh*pos, sel == pos-1 ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Configuration",x, y+_fh*pos, sel == pos-1 ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Help",x, y+_fh*pos, sel == pos-1 ? FONT_RED : FONT_WHITE); pos++;
					menu_size = 5;
					break;
				case MENU_PATTERNS_COLOR_LEVELS:
					DrawString("PLUGE",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Color Bars",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("EBU Color Bars",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("SMPTE Color Bars",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Color Bars with Gray Scale",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Color Bleed Check",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Gray Ramp",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("White & RGB screens",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("100 IRE",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Sharpness",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Convergence",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					pos++;
					DrawString("Back to Patterns Menu",x, y+_fh*pos, sel == pos-1 ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Configuration",x, y+_fh*pos, sel == pos-1 ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Help",x, y+_fh*pos, sel == pos-1 ? FONT_RED : FONT_WHITE); pos++;
					menu_size = 14;
					break;
				case MENU_PATTERNS_GEOMETRY:
					DrawString("Monoscope",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Grid",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Overscan",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					pos++;
					DrawString("Back to Patterns Menu",x, y+_fh*pos, sel == pos-1 ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Configuration",x, y+_fh*pos, sel == pos-1 ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Help",x, y+_fh*pos, sel == pos-1 ? FONT_RED : FONT_WHITE); pos++;
					menu_size = 6;
					break;
				case MENU_VIDEO_TESTS:
					DrawString("Drop Shadow Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Striped Sprite Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Lag Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Timing & Reflex Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Scroll Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Grid Scroll Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Horizontal/Vertical Stripes",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Checkerboard",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Backlit Zone Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Diagonal Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					pos++;
					DrawString("Back to Main Menu",x, y+_fh*pos, sel == pos-1 ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Configuration",x, y+_fh*pos, sel == pos-1 ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Help",x, y+_fh*pos, sel == pos-1 ? FONT_RED : FONT_WHITE); pos++;
					menu_size = 13;
					break;
				case MENU_AUDIO_TESTS:
					DrawString("Sound Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Audio Sync Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					pos++;
					DrawString("Back to Main Menu",x, y+_fh*pos, sel == pos-1 ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Configuration",x, y+_fh*pos, sel == pos-1 ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Help",x, y+_fh*pos, sel == pos-1 ? FONT_RED : FONT_WHITE); pos++;
					menu_size = 5;
					break;
				case MENU_HARDWARE_TESTS:
					DrawString("Controller Test",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					//DrawString("Controller Ports Device List",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("System Information",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					//DrawString("Memory Viewer",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					pos++;
					DrawString("Back to Main Menu",x, y+_fh*pos, sel == pos-1 ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Configuration",x, y+_fh*pos, sel == pos-1 ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Help",x, y+_fh*pos, sel == pos-1 ? FONT_RED : FONT_WHITE); pos++;
					menu_size = 5;
					break;
				case MENU_CONFIGURATION:
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
					sprintf(string_buf,"Effective resolution  %sx%s",x_res_text_value(screenMode),y_res_text_value(screenMode));
					DrawString(string_buf,x, y+_fh*pos, FONT_CYAN); pos++;
					pos++;
					if (VDP2_TVMD_TV_STANDARD_NTSC == screenMode.colorsystem)
					{
						DrawString("Color system ........... NTSC",x, y+_fh*pos, FONT_CYAN); pos++;
						switch (frame_counter)
						{
							default:
								sprintf(string_buf,"Frame cal . %d (unknown)",frame_counter);
								break;
						}
						DrawString(string_buf, x, y+_fh*pos, FONT_CYAN); pos++;
						DrawString("Field rate ......... 59.94 Hz", x, y+_fh*pos, FONT_CYAN); pos++;//59.52 Hz if hacked from PAL
						if (VIDEO_X_RESOLUTION_320 == screenMode.x_res)
							DrawString("Pixel clock ......26.8426 MHz", x, y+_fh*pos, FONT_CYAN);//14.31818 MHz * 1706 / 910
						else
							DrawString("Pixel clock ......28.6364 MHz", x, y+_fh*pos, FONT_CYAN);//14.31818 MHz * 1820 / 910
						pos++;
					}
					else
					{
						DrawString("Color system ............ PAL",x, y+_fh*pos, FONT_CYAN); pos++;
						switch (frame_counter)
						{
							case 10165:
							case 10166:
							case 10167:
							case 10168:
							case 10169:
								sprintf(string_buf,"Frame cal ..... %d (PAL)",frame_counter);
								break;
							case 8528:
								sprintf(string_buf,"Frame cal  %d (PAL 60Hz)",frame_counter);
								break;
							default:
								sprintf(string_buf,"Frame cal . %d (unknown)",frame_counter);
								break;
						}
						DrawString(string_buf, x, y+_fh*pos, FONT_CYAN); pos++;
						DrawString("Field rate ............ 50 Hz", x, y+_fh*pos, FONT_CYAN); pos++;//50.35 Hz if hacked from NTSC
						if (VIDEO_X_RESOLUTION_320 == screenMode.x_res)
							DrawString("Pixel clock ......26.6564 MHz", x, y+_fh*pos, FONT_CYAN);//17.734475 MHz * 1706 / 1135
						else
							DrawString("Pixel clock ......28.4377 MHz", x, y+_fh*pos, FONT_CYAN);//17.734475 MHz * 1820 / 1135
						pos++;
					}
					menu_size = 5;
					break;
				case MENU_VIDEO_TEST_SCROLL_HV_SELECT:
					DrawString("Horizontal",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Vertical",x, y+_fh*pos, sel == pos ? FONT_RED : FONT_WHITE); pos++;
					pos++;
					DrawString("Back to Video Tests",x, y+_fh*pos, sel == pos-1 ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Configuration",x, y+_fh*pos, sel == pos-1 ? FONT_RED : FONT_WHITE); pos++;
					DrawString("Help",x, y+_fh*pos, sel == pos-1 ? FONT_RED : FONT_WHITE); pos++;
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
					if (MENU_CONFIGURATION == menu_id)
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
					if (MENU_CONFIGURATION == menu_id)
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
								menu_id = MENU_PATTERNS;
								sel = 0;
								redrawMenu = true;
								break;							
							case 1:
								menu_id = MENU_VIDEO_TESTS;
								sel = 0;
								redrawMenu = true;
								break;
							/*case 2:
								menu_id = MENU_AUDIO_TESTS;
								sel = 0;
								redrawMenu = true;
								break;*/
							case 2:
								menu_id = MENU_HARDWARE_TESTS;
								sel = 0;
								redrawMenu = true;
								break;
							case 3:
								menu_id = MENU_CONFIGURATION;
								sel = 0;
								redrawMenu = true;
								break;
							case 4:
								//go to credits
								vdp2_sync_wait();
								wait_for_key_unpress();
								window_credits(screenMode);
								redrawBG = true;
								redrawMenu = true;
								break;
							case 5:
								help(screenMode);
								redrawMenu = true;
								redrawBG = true;
								break;
						}
					}
					else if (MENU_PATTERNS == menu_id)
					{
						switch(sel)
						{
							case 0:
								menu_id = MENU_PATTERNS_COLOR_LEVELS;
								sel = 0;
								redrawMenu = true;
								break;							
							case 1:
								menu_id = MENU_PATTERNS_GEOMETRY;
								sel = 0;
								redrawMenu = true;
								break;
							case 2:
								menu_id = MENU_MAIN;
								sel = 0;
								redrawMenu = true;
								break;
							case 3:
								menu_id = MENU_CONFIGURATION;
								sel = 0;
								redrawMenu = true;
								break;
							case 6:
								help(screenMode);
								redrawMenu = true;
								redrawBG = true;
								break;
						}
					}
					else if (MENU_PATTERNS_COLOR_LEVELS == menu_id)
					{
						switch(sel)
						{
							case 0:
								pattern_pluge(screenMode);
								redrawBG = true;
								break;							
							case 1:
								pattern_colorbars(screenMode);
								break;
							case 2:
								pattern_ebu(screenMode);
								redrawBG = true;
								break;							
							case 3:
								pattern_smpte(screenMode);
								redrawBG = true;
								break;
							case 4:
								pattern_colorbars_grayscale(screenMode);
								redrawBG = true;
								break;
							case 5:
								pattern_colorbleed(screenMode);
								break;
							case 6:
								pattern_grayramp(screenMode);
								break;
							case 7:
								pattern_whitescreen(screenMode);
								break;
							case 8:
								pattern_100ire(screenMode);
								break;
							case 9:
								pattern_sharpness(screenMode);
								break;
							case 10:
								pattern_convergence(screenMode);
								break;
							case 11:
								menu_id = MENU_PATTERNS;
								sel = 0;
								redrawMenu = true;
								break;	
							case 12:
								menu_id = MENU_CONFIGURATION;
								sel = 0;
								redrawMenu = true;
								break;
							case 13:
								help(screenMode);
								redrawMenu = true;
								redrawBG = true;
								break;
						}
						redrawMenu = true;
					}
					else if (MENU_PATTERNS_GEOMETRY == menu_id)
					{
						switch(sel)
						{
							case 0:
								pattern_monoscope(screenMode);
								redrawBG = true;
								break;							
							case 1:
								pattern_grid(screenMode);
								break;
							case 2:
								pattern_overscan(screenMode);
								break;							
							case 3:
								menu_id = MENU_PATTERNS;
								sel = 1;
								redrawMenu = true;
								break;	
							case 4:
								menu_id = MENU_CONFIGURATION;
								sel = 0;
								redrawMenu = true;
								break;
							case 5:
								help(screenMode);
								redrawMenu = true;
								redrawBG = true;
								break;
						}
						redrawMenu = true;
					}
					else if (MENU_VIDEO_TESTS == menu_id)
					{
						switch(sel)
						{
							case 0:
								videotest_dropshadow(screenMode);
								break;							
							case 1:
								videotest_striped_sprite(screenMode);
								break;							
							case 2:
								videotest_lag(screenMode);
								break;							
							case 3:
								videotest_reflex(screenMode);
								break;							
							case 4:
								menu_id = MENU_VIDEO_TEST_SCROLL_HV_SELECT;
								sel = 0;
								break;							
							case 5:
								videotest_grid_scroll(screenMode);
								break;							
							case 6:
								videotest_hor_vert_stripes(screenMode);
								break;							
							case 7:
								videotest_checkerboard(screenMode);
								break;							
							case 8:
								videotest_backlit_zone(screenMode);
								break;
							case 9:
								videotest_diagonal(screenMode);
								break;
							case 10:
								menu_id = MENU_MAIN;
								sel = 1;
								break;	
							case 11:
								menu_id = MENU_CONFIGURATION;
								sel = 0;
								break;
							case 12:
								help(screenMode);
								redrawMenu = true;
								redrawBG = true;
								break;
						}
						redrawMenu = true;
					}
					else if (MENU_AUDIO_TESTS == menu_id)
					{
						switch(sel)
						{
							case 0:
								//sound
								break;							
							case 1:
								//audio sync
								break;							
							case 2:
								menu_id = MENU_MAIN;
								sel = 2;
								redrawMenu = true;
								break;	
							case 3:
								menu_id = MENU_CONFIGURATION;
								sel = 0;
								redrawMenu = true;
								break;
							case 4:
								help(screenMode);
								redrawMenu = true;
								redrawBG = true;
								break;
						}
						redrawMenu = true;
					}
					else if (MENU_HARDWARE_TESTS == menu_id)
					{
						switch(sel)
						{
							case 0:
								hwtest_controller(screenMode);
								break;							
							case 1:
								hwtest_sysinfo(screenMode);
								break;							
							case 2:
								menu_id = MENU_MAIN;
								sel = 2;
								redrawMenu = true;
								break;	
							case 3:
								menu_id = MENU_CONFIGURATION;
								sel = 0;
								redrawMenu = true;
								break;
							case 4:
								help(screenMode);
								redrawMenu = true;
								redrawBG = true;
								break;
						}
						redrawMenu = true;
					}
					else if (MENU_CONFIGURATION == menu_id)
					{
						switch(sel)
						{
							case 0:
								//change scanmode
								screenMode.scanmode++;
								if (screenMode.scanmode > VIDEO_SCANMODE_480P)
									screenMode.scanmode = VIDEO_SCANMODE_240I;
								if (VIDEO_SCANMODE_480P == screenMode.scanmode)
									screenMode.y_res = VDP2_TVMD_VERT_240; //only x480 in 480p mode
								update_screen_mode(screenMode,false);
								redrawBG = true;
								redrawMenu = true;
								break;
							case 1:
								//y lines
								screenMode.y_res++;
								if (VIDEO_SCANMODE_480P == screenMode.scanmode)
									screenMode.y_res = VDP2_TVMD_VERT_240; //only x480 in 480p mode
								else if (VDP2_TVMD_TV_STANDARD_NTSC == screenMode.colorsystem)
								{
									if (screenMode.y_res > VDP2_TVMD_VERT_240) //no 256 for NTSC
										screenMode.y_res = VDP2_TVMD_VERT_224;
								}
								else
								{
									if (screenMode.y_res > VDP2_TVMD_VERT_256)
										screenMode.y_res = VDP2_TVMD_VERT_224;
								}
								update_screen_mode(screenMode,false);
								redrawBG = true;
								redrawMenu = true;
								break;
							case 2:
								//x res
								screenMode.x_res++;
								if (screenMode.x_res > VIDEO_X_RESOLUTION_352)
								{
									screenMode.x_res = VIDEO_X_RESOLUTION_320;
									screenMode.x_res_doubled = !screenMode.x_res_doubled;
								}
								update_screen_mode(screenMode,false);
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
							case 5:
								menu_id = MENU_MAIN;
								sel = 3;
								redrawMenu = true;
								break;	
							case 6:
								menu_id = MENU_CONFIGURATION;
								sel = 0;
								redrawMenu = true;
								break;
							case 7:
								help(screenMode);
								redrawMenu = true;
								redrawBG = true;
								break;
						}
					}
					else if (MENU_VIDEO_TEST_SCROLL_HV_SELECT == menu_id)
					{
						switch(sel)
						{
							case 0:
								videotest_scroll_horizontal(screenMode);
								break;
							case 1:
								videotest_scroll_vertical(screenMode);
								break;
							case 2:
								menu_id = MENU_VIDEO_TESTS;
								sel = 4;
								break;	
							case 3:
								menu_id = MENU_CONFIGURATION;
								sel = 0;
								break;
							case 4:
								help(screenMode);
								redrawMenu = true;
								redrawBG = true;
								break;
						}
						redrawMenu = true;
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
							window_credits(screenMode);
							redrawBG = true;
							redrawMenu = true;
							break;
						case MENU_PATTERNS:
							menu_id = MENU_MAIN;
							sel = 0;
							break;
						case MENU_PATTERNS_COLOR_LEVELS:
							menu_id = MENU_PATTERNS;
							sel = 0;
							break;
						case MENU_PATTERNS_GEOMETRY:
							menu_id = MENU_PATTERNS;
							sel = 1;
							break;
						case MENU_VIDEO_TESTS:
							menu_id = MENU_MAIN;
							sel = 1;
							break;
						case MENU_AUDIO_TESTS:
							menu_id = MENU_MAIN;
							sel = 2;
							break;
						case MENU_HARDWARE_TESTS:
							menu_id = MENU_MAIN;
							sel = 3;
							break;
						case MENU_CONFIGURATION:
							menu_id = MENU_MAIN;
							sel = 4;
							break;
						case MENU_VIDEO_TEST_SCROLL_HV_SELECT:
							menu_id = MENU_VIDEO_TESTS;
							sel = 4;
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
