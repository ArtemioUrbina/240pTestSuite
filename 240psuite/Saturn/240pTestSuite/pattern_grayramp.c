#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "video_vdp2.h"
#include "video.h"
#include "control.h"
#include "ire.h"

void draw_grayramp(video_screen_mode_t screenmode)
{
	//removing text
	ClearTextLayer();
	
	video_vdp2_set_cycle_patterns_cpu();
	//add colors to palette
	uint8_t IRE_top = Get_IRE_Level(100.0);
	uint8_t IRE_bot = Get_IRE_Level(7.5);
	rgb888_t Color = {0,IRE_bot,IRE_bot,IRE_bot};
	for (int i=0;i<32;i++) 
	{
		Color.r = IRE_bot + (i*(IRE_top-IRE_bot)+16)/32;
		Color.g = IRE_bot + (i*(IRE_top-IRE_bot)+16)/32;
		Color.b = IRE_bot + (i*(IRE_top-IRE_bot)+16)/32;	
		video_vdp2_set_palette_part(2,&Color,1+i,1+i); //palette 2 colors 1..32 = white gradient
	}

	//create single-color tiles for each color, 32 tiles in total
	int *_pointer32 = (int *)VIDEO_VDP2_NBG0_CHPNDR_START;
	for (int i=1; i<33; i++)
		for (int j=0; j<16; j++)
			_pointer32[i*16+j] = 0x01010101*i;
	//fill everything with pitch-black
    _pointer32 = (int *)VIDEO_VDP2_NBG0_PNDR_START;
    for (unsigned int i = 0; i < VIDEO_VDP2_NBG0_PNDR_SIZE / sizeof(int); i++)
    {
        _pointer32[i] = 0x00200002; //palette 2, transparency on, black from white gradient
    }
	//draw bars depending on screen mode
	_pointer32 = (int *)VIDEO_VDP2_NBG0_PNDR_START;
	int offset = (VIDEO_X_RESOLUTION_320 == screenmode.x_res) ? 6 : 10;
	int y_ratio = ( (VIDEO_SCANMODE_480I == screenmode.scanmode) || (VIDEO_SCANMODE_480P == screenmode.scanmode) ) ? 2 : 1;
    if (screenmode.x_res_doubled)
	{
		//high-x-res mode
		for (int i = 0;i<64;i++)
		{
			for (int j=0;j<12*y_ratio;j++)
			{
				if (offset+i < 64)
				{
					_pointer32[64*(3+j) + offset + i] = 0x00200000 | (i/2+1)*2; //palette 2
				}
				else
				{
					_pointer32[64*63+64*(3+j) + offset + i] = 0x00200000 | (i/2+1)*2; //palette 2
				}
			}
			for (int j=12*y_ratio;j<24*y_ratio;j++)
			{
				if (offset+i < 64)
				{
					_pointer32[64*(3+j) + offset + i] = 0x00200000 | (32-i/2)*2; //palette 2
				}
				else
				{
					_pointer32[64*63+64*(3+j) + offset + i] = 0x00200000 | (32-i/2)*2; //palette 2
				}
			}
		}	
	}
	else
	{
		//normal x mode
		for (int i = 0;i<32;i++)
		{
			for (int j=0;j<12*y_ratio;j++)
			{
				_pointer32[64*(3+j) + offset + i] = 0x00200000 | (i+1)*2; //palette 2
			}
			for (int j=12*y_ratio;j<24*y_ratio;j++)
			{
				_pointer32[64*(3+j) + offset + i] = 0x00200000 | (32-i)*2; //palette 2
			}
		}	
	}
	video_vdp2_set_cycle_patterns_nbg(screenmode);
}

void pattern_grayramp(video_screen_mode_t screenmode)
{
	video_screen_mode_t curr_screenmode = screenmode;
	draw_grayramp(curr_screenmode);
	bool key_pressed = false;

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
			draw_grayramp(curr_screenmode);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.r) )
		{
			curr_screenmode = next_screen_mode(curr_screenmode);
			update_screen_mode(curr_screenmode,false);
			draw_grayramp(curr_screenmode);
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
			{
				ClearTextLayer();
			}
		}
	}
}
