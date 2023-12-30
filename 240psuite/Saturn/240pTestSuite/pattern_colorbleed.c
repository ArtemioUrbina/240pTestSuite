#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "svin.h"
#include "video.h"
#include "control.h"
#include "ire.h"

void draw_colorbleed(_svin_screen_mode_t screenmode, bool checkered)
{
	//removing text
	ClearTextLayer();
	
	_svin_set_cycle_patterns_cpu();
	//add colors to palette
	uint8_t IRE_top = Get_IRE_Level(100.0);
	uint8_t IRE_bot = Get_IRE_Level(7.5);
	rgb888_t Color = {0,IRE_bot,IRE_bot,IRE_bot};
	Color.r = IRE_top;
	_svin_set_palette_part(2,&Color,1,1); //palette 2 color 1 = red
	Color.r = IRE_bot;
	Color.g = IRE_top;
	_svin_set_palette_part(2,&Color,2,2); //palette 2 color 2 = green
	Color.g = IRE_bot;
	Color.b = IRE_top;
	_svin_set_palette_part(2,&Color,3,3); //palette 2 color 3 = blue
	Color.r = IRE_top;
	Color.g = IRE_top;
	Color.b = IRE_top;	
	_svin_set_palette_part(2,&Color,4,4); //palette 2 color 4 = white
	//create single-color tiles for each color, 4 tiles in total
	int *_pointer32 = (int *)_SVIN_NBG0_CHPNDR_START;
	if (checkered)
	{
		for (int i=1; i<5; i++)
			for (int j=0; j<16; j+=4)
			{
				_pointer32[i*16+j] = 0x01000100*i;
				_pointer32[i*16+j+1] = 0x01000100*i;
				_pointer32[i*16+j+2] = 0x00010001*i;
				_pointer32[i*16+j+3] = 0x00010001*i;
			}
	}
	else
	{
		for (int i=1; i<5; i++)
			for (int j=0; j<16; j++)
				_pointer32[i*16+j] = 0x01000100*i;
	}
	//black tile
	for (int j=0; j<16; j++)
		_pointer32[j] = 0x0;
	//fill everything with pitch-black
    _pointer32 = (int *)_SVIN_NBG0_PNDR_START;
    for (unsigned int i = 0; i < _SVIN_NBG0_PNDR_SIZE / sizeof(int); i++)
    {
        _pointer32[i] = 0x00200000; //palette 2, transparency on, black
    }
	//draw bars depending on screen mode
	_pointer32 = (int *)_SVIN_NBG0_PNDR_START;
	int offset = (_SVIN_X_RESOLUTION_320 == screenmode.x_res) ? 6 : 10;
	int y_ratio = (_SVIN_SCANMODE_480I == screenmode.scanmode) ? 2 : 1;
    if (screenmode.x_res_doubled)
	{
		//high-x-res mode
		for (int i = 0;i<64;i++)
		{
			for (int j=0;j<4*y_ratio;j++)
			{
				if (offset+i < 64)
				{
					_pointer32[64*(3+j) + offset + i] = 0x00200000 | 2; //palette 2, red
					_pointer32[64*(3+5*y_ratio+j) + offset + i] = 0x00200000 | 4; //palette 2, green
					_pointer32[64*(3+10*y_ratio+j) + offset + i] = 0x00200000 | 6; //palette 2, blue
					_pointer32[64*(3+15*y_ratio+j) + offset + i] = 0x00200000 | 8; //palette 2, white
				}
				else
				{
					_pointer32[64*63+64*(3+j) + offset + i] = 0x00200000 | 2; //palette 2, red
					_pointer32[64*63+64*(3+5*y_ratio+j) + offset + i] = 0x00200000 | 4; //palette 2, green
					_pointer32[64*63+64*(3+10*y_ratio+j) + offset + i] = 0x00200000 | 6; //palette 2, blue
					_pointer32[64*63+64*(3+15*y_ratio+j) + offset + i] = 0x00200000 | 8; //palette 2, white
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
				_pointer32[64*(3+j) + offset + i] = 0x00200000 | 2; //palette 2, red
				_pointer32[64*(3+5*y_ratio+j) + offset + i] = 0x00200000 | 4; //palette 2, green
				_pointer32[64*(3+10*y_ratio+j) + offset + i] = 0x00200000 | 6; //palette 2, blue
				_pointer32[64*(3+15*y_ratio+j) + offset + i] = 0x00200000 | 8; //palette 2, black
			}
		}	
	}
	_svin_set_cycle_patterns_nbg();
}

void pattern_colorbleed(_svin_screen_mode_t screenmode)
{
	_svin_screen_mode_t curr_screenmode = screenmode;
	bool bCheckered = false;
	draw_colorbleed(curr_screenmode,bCheckered);
	bool key_pressed = false;

	wait_for_key_unpress();
	
	int mode_display_counter = 0;

	while (1)
	{
		smpc_peripheral_process();
		smpc_peripheral_digital_port(1, &controller);
		if ( (controller.pressed.button.l) )
		{
			curr_screenmode = prev_screen_mode(curr_screenmode);
			update_screen_mode(curr_screenmode);
			draw_colorbleed(curr_screenmode,bCheckered);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.r) )
		{
			curr_screenmode = next_screen_mode(curr_screenmode);
			update_screen_mode(curr_screenmode);
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
			update_screen_mode(screenmode);
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
