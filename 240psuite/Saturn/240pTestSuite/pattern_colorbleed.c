#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "video_vdp2.h"
#include "video.h"
#include "control.h"
#include "ire.h"
#include "input.h"

void draw_colorbleed(video_screen_mode_t screenmode, bool checkered)
{
	//removing text
	ClearTextLayer();
	
	video_vdp2_set_cycle_patterns_cpu();
	//add colors to palette
	uint8_t IRE_top = Get_IRE_Level(100.0);
	uint8_t IRE_bot = Get_IRE_Level(7.5);
	rgb888_t Color = {.cc=0,.r=IRE_bot,.g=IRE_bot,.b=IRE_bot};
	Color.r = IRE_top;
	video_vdp2_set_palette_part(2,&Color,1,1); //palette 2 color 1 = red
	Color.r = IRE_bot;
	Color.g = IRE_top;
	video_vdp2_set_palette_part(2,&Color,2,2); //palette 2 color 2 = green
	Color.g = IRE_bot;
	Color.b = IRE_top;
	video_vdp2_set_palette_part(2,&Color,3,3); //palette 2 color 3 = blue
	Color.r = IRE_top;
	Color.g = IRE_top;
	Color.b = IRE_top;	
	video_vdp2_set_palette_part(2,&Color,4,4); //palette 2 color 4 = white
	//create single-color tiles for each color, 4 tiles in total
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
		if (checkered)
		{
			for (int i=1; i<5; i++)
				for (int j=0; j<16; j+=4)
				{
					_pointer32[copy][i*16+j] = 0x01000100*i;
					_pointer32[copy][i*16+j+1] = 0x01000100*i;
					_pointer32[copy][i*16+j+2] = 0x00010001*i;
					_pointer32[copy][i*16+j+3] = 0x00010001*i;
				}
		}
		else
		{
			for (int i=1; i<5; i++)
				for (int j=0; j<16; j++)
					_pointer32[copy][i*16+j] = 0x01000100*i;
		}
		//black tile
		for (int j=0; j<16; j++)
			_pointer32[copy][j] = 0x0;
	}
	//fill everything with pitch-black
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
			_pointer32[copy][i] = 0x00200000 + vram_offset[copy]/32; //palette 2, transparency on, black
		}
	}
	//draw bars depending on screen mode
	int offset = (VIDEO_X_RESOLUTION_320 == screenmode.x_res) ? 6 : 10;
	int y_ratio = ( (VIDEO_SCANMODE_480I == screenmode.scanmode) || (VIDEO_SCANMODE_480P == screenmode.scanmode) ) ? 2 : 1;
	for (int copy = 0; copy < copies; copy++)
	{
		if (screenmode.x_res_doubled)
		{
			//high-x-res mode
			for (int i = 0;i<64;i++)
			{
				for (int j=0;j<4*y_ratio;j++)
				{
					if (offset+i < 64)
					{
						_pointer32[copy][64*(3+j) + offset + i] = 0x00200000 + 2 + vram_offset[copy]/32; //palette 2, red
						_pointer32[copy][64*(3+5*y_ratio+j) + offset + i] = 0x00200000 + 4 + vram_offset[copy]/32; //palette 2, green
						_pointer32[copy][64*(3+10*y_ratio+j) + offset + i] = 0x00200000 + 6 + vram_offset[copy]/32; //palette 2, blue
						_pointer32[copy][64*(3+15*y_ratio+j) + offset + i] = 0x00200000 + 8 + vram_offset[copy]/32; //palette 2, white
					}
					else
					{
						_pointer32[copy][64*63+64*(3+j) + offset + i] = 0x00200000 + 2 + vram_offset[copy]/32; //palette 2, red
						_pointer32[copy][64*63+64*(3+5*y_ratio+j) + offset + i] = 0x00200000 + 4 + vram_offset[copy]/32; //palette 2, green
						_pointer32[copy][64*63+64*(3+10*y_ratio+j) + offset + i] = 0x00200000 + 6 + vram_offset[copy]/32; //palette 2, blue
						_pointer32[copy][64*63+64*(3+15*y_ratio+j) + offset + i] = 0x00200000 + 8 + vram_offset[copy]/32; //palette 2, white
					}
				}
			}	
		}
		else
		{
			//normal x mode
			for (int i = 0;i<32;i++)
			{
				for (int j=0;j<4*y_ratio;j++)
				{
					_pointer32[copy][64*(3+j) + offset + i] = 0x00200000 + 2 + vram_offset[copy]/32; //palette 2, red
					_pointer32[copy][64*(3+5*y_ratio+j) + offset + i] = 0x00200000 + 4 + vram_offset[copy]/32; //palette 2, green
					_pointer32[copy][64*(3+10*y_ratio+j) + offset + i] = 0x00200000 + 6 + vram_offset[copy]/32; //palette 2, blue
					_pointer32[copy][64*(3+15*y_ratio+j) + offset + i] = 0x00200000 + 8 + vram_offset[copy]/32; //palette 2, black
				}
			}	
		}
	}
	video_vdp2_set_cycle_patterns_nbg(screenmode);
}

void pattern_colorbleed(video_screen_mode_t screenmode)
{
	video_screen_mode_t curr_screenmode = screenmode;
	bool bCheckered = false;
	draw_colorbleed(curr_screenmode,bCheckered);

	wait_for_key_unpress();
	
	int mode_display_counter = 0;

	while (1)
	{
		smpc_peripheral_process();
		get_digital_keypress_anywhere(&controller);
		if ( (controller.pressed.button.l) )
		{
			curr_screenmode = prev_screen_mode(curr_screenmode);
			update_screen_mode(curr_screenmode,false);
			draw_colorbleed(curr_screenmode,bCheckered);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.r) )
		{
			curr_screenmode = next_screen_mode(curr_screenmode);
			update_screen_mode(curr_screenmode,false);
			draw_colorbleed(curr_screenmode,bCheckered);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.a) || (controller.pressed.button.c) )
		{
			//change the checkered mode
			bCheckered = bCheckered ? false : true;
			draw_colorbleed(curr_screenmode,bCheckered);
			wait_for_key_unpress();
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
