#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "video_vdp2.h"
#include "video.h"
#include "control.h"
#include "ire.h"

void print_ire_level(int ire_level)
{
	char buf[128];
	sprintf(buf, "%i IRE", ire_level);
	DrawStringWithBackground(buf, 160-strlen(buf)*_fw/2, 120, FONT_WHITE,FONT_BLUE);
}

void draw_100ire(video_screen_mode_t screenmode, int ire_level)
{
	//removing text
	ClearTextLayer();
	
	video_vdp2_set_cycle_patterns_cpu();
	//adding a single color to palette
	uint8_t IRE_top = Get_IRE_Level(ire_level);
	uint8_t IRE_bot = Get_IRE_Level(0);
	rgb888_t Color = {0,IRE_bot,IRE_bot,IRE_bot};
	Color.r = IRE_top;
	Color.g = IRE_top;
	Color.b = IRE_top;	
	video_vdp2_set_palette_part(2,&Color,1,1); //palette 2 color 1 = IRE white
	Color.r = IRE_bot;
	Color.g = IRE_bot;
	Color.b = IRE_bot;	
	video_vdp2_set_palette_part(2,&Color,2,2); //palette 2 color 2 = IRE black
	//create two tiles for the colors 1 and 2
	int copies = 1;
	int *_pointer32[2];
	int vram_offset[2];
	_pointer32[0] = (int *)VIDEO_VDP2_NBG0_CHPNDR_START;
	if (is_screenmode_special(screenmode))
	{
		copies = 2;
		_pointer32[0] = (int *)VIDEO_VDP2_NBG0_SPECIAL_CHPNDR_START;
		_pointer32[1] = (int *)VIDEO_VDP2_NBG1_SPECIAL_CHPNDR_START;
	}
	for (int copy = 0; copy < copies; copy++)
	{
		for (int j=0; j<16; j++)
		{
			_pointer32[copy][16+j] = 0x01010101;
			_pointer32[copy][32+j] = 0x02020202;
		}
	}
	//fill everything with our black tile
	_pointer32[0] = (int *)VIDEO_VDP2_NBG0_PNDR_START;
	vram_offset[0] = VIDEO_VDP2_NBG0_CHPNDR_START - VDP2_VRAM_ADDR(0,0);
	if (is_screenmode_special(screenmode))
	{
		_pointer32[0] = (int *)VIDEO_VDP2_NBG0_SPECIAL_PNDR_START;
		_pointer32[1] = (int *)VIDEO_VDP2_NBG1_SPECIAL_PNDR_START;
		vram_offset[0] = VIDEO_VDP2_NBG0_SPECIAL_CHPNDR_START - VDP2_VRAM_ADDR(0,0);
		vram_offset[1] = VIDEO_VDP2_NBG1_SPECIAL_CHPNDR_START - VDP2_VRAM_ADDR(0,0);
	}
	for (int copy = 0; copy < copies; copy++)
	{
    	for (unsigned int i = 0; i < VIDEO_VDP2_NBG0_PNDR_SIZE / sizeof(int); i++)
    	{
    	    _pointer32[copy][i] = 0x00200004 + vram_offset[copy]/32; //palette 2
    	}
	}
	//fill center with white tile
	int x_start = (VIDEO_X_RESOLUTION_320 == screenmode.x_res) ? 10 : 11;
	x_start = (screenmode.x_res_doubled) ? x_start*2 : x_start;
	int y_start = (VDP2_TVMD_VERT_224 == screenmode.y_res) ? 7 : 
					(VDP2_TVMD_VERT_240 == screenmode.y_res) ? 7 : 8;
	y_start = ( (VIDEO_SCANMODE_480I == screenmode.scanmode) || (VIDEO_SCANMODE_480P == screenmode.scanmode) ) ? y_start*2 : y_start;
	for (int copy = 0; copy < copies; copy++)
	{
		for (int i = x_start; i<x_start*3; i++)
		{
			for (int j=y_start; j<y_start*3; j++)
			{
				_pointer32[copy][64*j + i] = 0x00200002 + vram_offset[copy]/32; //palette 2
			}
		}
	}
	video_vdp2_set_cycle_patterns_nbg(screenmode);
}

void pattern_100ire(video_screen_mode_t screenmode)
{
	video_screen_mode_t curr_screenmode = screenmode;
	int ire_level = 100;
	draw_100ire(curr_screenmode,ire_level);
	print_ire_level(ire_level);
	bool key_pressed = false;

	wait_for_key_unpress();
	
	int mode_display_counter = 120;

	while (1)
	{
		smpc_peripheral_process();
		get_digital_keypress_anywhere(&controller);
		if ( (controller.pressed.button.l) )
		{
			curr_screenmode = prev_screen_mode(curr_screenmode);
			update_screen_mode(curr_screenmode,false);
			draw_100ire(curr_screenmode,ire_level);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.r) )
		{
			curr_screenmode = next_screen_mode(curr_screenmode);
			update_screen_mode(curr_screenmode,false);
			draw_100ire(curr_screenmode,ire_level);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.a) || (controller.pressed.button.c) )
		{
			//change the checkered mode
			ire_level-=10;
			if (ire_level < 0) ire_level = 100;
			draw_100ire(curr_screenmode,ire_level);
			print_ire_level(ire_level);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if (controller.pressed.button.b)
		{
			//quit the pattern
			wait_for_key_unpress();
			update_screen_mode(screenmode,false);
			return;
		}
		vdp2_tvmd_vblank_in_wait();
		vdp2_tvmd_vblank_out_wait();
		if (mode_display_counter > 0)
		{
			mode_display_counter--;
			if (0 == mode_display_counter)
			{
				ClearTextLayer();
			}
		}
	}
}
