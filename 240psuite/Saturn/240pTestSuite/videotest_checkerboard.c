#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "video_vdp2.h"
#include "video.h"
#include "control.h"
#include "ire.h"

void draw_checkerboard(video_screen_mode_t screenmode)
{
	//removing text
	ClearTextLayer();
	
	video_vdp2_set_cycle_patterns_cpu();
	//add colors to palette
	uint8_t IRE_top = Get_IRE_Level(100.0);
	uint8_t IRE_bot = Get_IRE_Level(7.5);
	rgb888_t Color = {0,IRE_bot,IRE_bot,IRE_bot};
	video_vdp2_set_palette_part(2,&Color,1,1); //palette 2 color 1 = black
	Color.r = IRE_top;
	Color.g = IRE_top;
	Color.b = IRE_top;	
	video_vdp2_set_palette_part(2,&Color,2,2); //palette 2 color 2 = white
	//create checkerboard tile at index 0
	int *_pointer32 = (int *)VIDEO_VDP2_NBG0_CHPNDR_START;
	for (int i=0; i<4; i++)
	{
		_pointer32[i*4  ] = 0x01020102;
		_pointer32[i*4+1] = 0x01020102;
		_pointer32[i*4+2] = 0x02010201;
		_pointer32[i*4+3] = 0x02010201;
	}
	//fill everything with this tile
    _pointer32 = (int *)VIDEO_VDP2_NBG0_PNDR_START;
    for (unsigned int i = 0; i < VIDEO_VDP2_NBG0_PNDR_SIZE / sizeof(int); i++)
    {
        _pointer32[i] = 0x00200000; //palette 2, transparency on
    }
	video_vdp2_set_cycle_patterns_nbg(screenmode);
}

void videotest_checkerboard(video_screen_mode_t screenmode)
{
	video_screen_mode_t curr_screenmode = screenmode;
	draw_checkerboard(curr_screenmode);
	bool key_pressed = false;
	int *_pointer32 = (int *)VIDEO_VDP2_NBG0_CHPNDR_START;
	int pattern = 0;

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
			draw_checkerboard(curr_screenmode);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.r) )
		{
			curr_screenmode = next_screen_mode(curr_screenmode);
			update_screen_mode(curr_screenmode,false);
			draw_checkerboard(curr_screenmode);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.a) || (controller.pressed.button.b) || (controller.pressed.button.c) )
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
				ClearTextLayer();
		}
		//change the checkerboard pattern
		//create checkerboard tile at index 0
		pattern++;
		pattern = pattern % 2;
		if (pattern)
		{
			for (int i=0; i<4; i++)
			{
				_pointer32[i*4  ] = 0x01020102;
				_pointer32[i*4+1] = 0x01020102;
				_pointer32[i*4+2] = 0x02010201;
				_pointer32[i*4+3] = 0x02010201;
			}
		}
		else
		{
			for (int i=0; i<4; i++)
			{
				_pointer32[i*4  ] = 0x02010201;
				_pointer32[i*4+1] = 0x02010201;
				_pointer32[i*4+2] = 0x01020102;
				_pointer32[i*4+3] = 0x01020102;
			}
		}
	}
}