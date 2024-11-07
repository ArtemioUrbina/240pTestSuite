#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "video_vdp2.h"
#include "video.h"
#include "control.h"
#include "ire.h"

void draw_colorbars_grayscale(video_screen_mode_t screenmode, bool bIRE100)
{
	//removing text
	ClearTextLayer();
	
	video_vdp2_set_cycle_patterns_cpu();
	//add colors to palette
	uint8_t IRE_top = (bIRE100) ? Get_IRE_Level(100.0) : Get_IRE_Level(75);
	uint8_t IRE_bot = Get_IRE_Level(7.5);
	rgb888_t Color = {0,0,0,0};
	Color.r = IRE_top;
	Color.g = IRE_top;
	Color.b = IRE_top;	
	video_vdp2_set_palette_part(2,&Color,1,1); //palette 2 color 1 = IRE white
	Color.b = IRE_bot;
	video_vdp2_set_palette_part(2,&Color,2,2); //palette 2 color 2 = IRE yellow
	Color.r = IRE_bot;
	Color.b = IRE_top;
	video_vdp2_set_palette_part(2,&Color,3,3); //palette 2 color 3 = IRE cyan
	Color.b = IRE_bot;
	video_vdp2_set_palette_part(2,&Color,4,4); //palette 2 color 4 = IRE green
	Color.r = IRE_top;
	Color.g = IRE_bot;
	Color.b = IRE_top;	
	video_vdp2_set_palette_part(2,&Color,5,5); //palette 2 color 5 = IRE pink
	Color.b = IRE_bot;	
	video_vdp2_set_palette_part(2,&Color,6,6); //palette 2 color 6 = IRE red
	Color.r = IRE_bot;
	Color.b = IRE_top;
	video_vdp2_set_palette_part(2,&Color,7,7); //palette 2 color 7 = IRE blue

	int _size_x = get_screenmode_resolution_x(screenmode);
	int _size_y = get_screenmode_resolution_y(screenmode);

	uint8_t *_pointer8 = (uint8_t *)VIDEO_VDP2_NBG0_CHPNDR_START;

	//fill everything with smpte white
	for (int y=0;y<_size_y;y++)
	{
		int line_start = y*512;
		for (int x=0;x<_size_x;x++)
		{
			if (x%2)
			{
				_pointer8[line_start+x/2] &= 0xF0;
				_pointer8[line_start+x/2] |= 1;
			}
			else
			{
				_pointer8[line_start+x/2] &= 0x0F;
				_pointer8[line_start+x/2] |= 1<<4;

			}
		}
	}

	//draw top bars
	for (int i=0;i<7;i++)
	{
		uint8_t color = i+1;
		for (int y=(_size_y*20)/100;y<(_size_y*40)/100;y++)
		{
			int line_start = y*512;
			for (int x=(_size_x*i)/7;x<(_size_x*(i+1))/7;x++)
			{
				if (x%2)
				{
					_pointer8[line_start+x/2] &= 0xF0;
					_pointer8[line_start+x/2] |= color;
				}
				else
				{
					_pointer8[line_start+x/2] &= 0x0F;
					_pointer8[line_start+x/2] |= color<<4;

				}
			}
		}
	}

	//draw bottom bars
	for (int i=0;i<7;i++)
	{
		uint8_t color = 7-i;
		for (int y=(_size_y*60)/100;y<(_size_y*80)/100;y++)
		{
			int line_start = y*512;
			for (int x=(_size_x*i)/7;x<(_size_x*(i+1))/7;x++)
			{
				if (x%2)
				{
					_pointer8[line_start+x/2] &= 0xF0;
					_pointer8[line_start+x/2] |= color;
				}
				else
				{
					_pointer8[line_start+x/2] &= 0x0F;
					_pointer8[line_start+x/2] |= color<<4;

				}
			}
		}
	}

	video_vdp2_set_cycle_patterns_nbg(screenmode);
}

void pattern_colorbars_grayscale(video_screen_mode_t screenmode)
{
	video_screen_mode_t curr_screenmode = screenmode;
	bool bIRE100 = false;
	update_screen_mode(curr_screenmode,true); //re-initing in bmp mode
	draw_colorbars_grayscale(curr_screenmode,bIRE100);
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
			update_screen_mode(curr_screenmode,true);
			draw_colorbars_grayscale(curr_screenmode,bIRE100);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.r) )
		{
			curr_screenmode = next_screen_mode(curr_screenmode);
			update_screen_mode(curr_screenmode,true);
			draw_colorbars_grayscale(curr_screenmode,bIRE100);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.a) || (controller.pressed.button.c) )
		{
			//change the checkered mode
			bIRE100 = bIRE100 ? false : true;
			draw_colorbars_grayscale(curr_screenmode,bIRE100);
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
