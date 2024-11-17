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

void draw_convergence(video_screen_mode_t screenmode, int iPattern)
{
	int x,y;
	//removing text
	ClearTextLayer();
	
	video_vdp2_set_cycle_patterns_cpu();
	//add colors to palette
	uint8_t IRE_top = Get_IRE_Level(100);
	uint8_t IRE_bot = Get_IRE_Level(7.5);
	rgb888_t Color = {.cc=0,.r=IRE_bot,.g=IRE_bot,.b=IRE_bot};
	Color.r = IRE_top;
	Color.g = IRE_top;
	Color.b = IRE_top;	
	video_vdp2_set_palette_part(2,&Color,1,1); //palette 2 color 1 = IRE white
	Color.r = IRE_bot;
	Color.g = IRE_bot;
	Color.b = IRE_bot;
	video_vdp2_set_palette_part(2,&Color,2,2); //palette 2 color 2 = IRE black
	Color.r = IRE_top;
	video_vdp2_set_palette_part(2,&Color,3,3); //palette 2 color 3 = IRE red
	Color.r = IRE_bot;
	Color.g = IRE_top;
	video_vdp2_set_palette_part(2,&Color,4,4); //palette 2 color 4 = IRE green
	Color.g = IRE_bot;
	Color.b = IRE_top;
	video_vdp2_set_palette_part(2,&Color,5,5); //palette 2 color 4 = IRE blue
	Color.r = IRE_top;
	Color.g = IRE_top;
	Color.b = IRE_top;	
	video_vdp2_set_palette_part(2,&Color,6,6); //palette 2 color 1 = IRE white

	int _size_x = get_screenmode_resolution_x(screenmode);
	int _size_y = get_screenmode_resolution_y(screenmode);

	int copies = 1;
	uint8_t *_pointer8[2];
	uint32_t *_pointer32[2];
	_pointer8[0] = (uint8_t *)VIDEO_VDP2_NBG0_CHPNDR_START;
	if (is_screenmode_special(screenmode))
	{
		copies = 2;
		_pointer8[0] = (uint8_t *)VIDEO_VDP2_NBG0_SPECIAL_BMP_START;
		_pointer8[1] = (uint8_t *)VIDEO_VDP2_NBG1_SPECIAL_BMP_START;
	}
	_pointer32[0] = (uint32_t *)_pointer8[0];
	_pointer32[1] = (uint32_t *)_pointer8[1];
	
	//fill with black
	for (int copy = 0; copy < copies; copy++)
	{
		for (y=0;y<_size_y;y++)
		{
			int line_start = y*128;
			for (x=0;x<_size_x/4;x++)
			{
				_pointer32[copy][line_start+x] = 0x22222222;
			}
		}
	}

	switch (iPattern)
	{
		case 0: 
			//pattern 0 - grid
			for (int copy = 0; copy < copies; copy++)
			{
				//horizontal lines
				for (y=8;y<_size_y;y+=16)
				{
					int line_start = y*128;
					for (x=0;x<_size_x/8;x++)
						_pointer32[copy][line_start+x] = 0x11111111;
				}
				//vertical lines
				for (y=0;y<_size_y;y++)
				{
					int line_start = y*512;
					for (x=4;x<_size_x/2;x+=8)
					{
						_pointer8[copy][line_start+x] &= 0xF0;
						_pointer8[copy][line_start+x] |= 0x01;
					}
				}
			}
			break;
		case 1: 
			//pattern 1 - crosses
			for (int copy = 0; copy < copies; copy++)
			{
				for (y=8;y<_size_y;y+=16)
					for (x=4;x<_size_x/2;x+=8)
					{
						_pointer8[copy][y*512+x+1] &= 0x0F;
						_pointer8[copy][y*512+x+1] |= 0x10;
						_pointer8[copy][y*512+x] = 0x11;
						_pointer8[copy][y*512+512+x] &= 0xF0;
						_pointer8[copy][y*512+512+x] |= 0x01;
						_pointer8[copy][y*512-512+x] &= 0xF0;
						_pointer8[copy][y*512-512+x] |= 0x01;
					}
			}
			break;
		case 2: 
			//pattern 2 - dots
			for (int copy = 0; copy < copies; copy++)
			{
				for (y=8;y<_size_y;y+=16)
					for (x=4;x<_size_x/2;x+=8)
					{
						_pointer8[copy][y*512+x] &= 0xF0;
						_pointer8[copy][y*512+x] |= 0x01;
					}
			}
			break;
		case 3: 
			//pattern 3 - color quads
			for (int copy = 0; copy < copies; copy++)
			{
				for (y=0;y<_size_y+64;y+=64)
					for (x=0;x<_size_x/2+32;x+=32)
					{
						int color = 6-(x/32)%4;
						if ((y/64)%2) color -= 2;
						if (color < 3) color +=4;
						color = color*0x11;
						for (int _y=0;_y<64;_y++)
						{
							int line_start = (y+_y-32)*512;
							for (int _x=0;_x<32;_x++)
								_pointer8[copy][line_start+x+_x-16] = color;
						}
					}
			}
			break;
		case 4: 
			//pattern 4 - color quads with borders
			for (int copy = 0; copy < copies; copy++)
			{
				for (y=0;y<_size_y+64;y+=64)
					for (x=0;x<_size_x/2+32;x+=32)
					{
						int color = 6-(x/32)%4;
						if ((y/64)%2) color -= 2;
						if (color < 3) color +=4;
						color = color*0x11;
						for (int _y=0;_y<64;_y++)
						{
							int line_start = (y+_y-32)*512;
							for (int _x=0;_x<32;_x++)
								_pointer8[copy][line_start+x+_x-16] = color;
						}
					}
				for (y=32;y<_size_y;y+=64)
				{
					int line_start = y*512;
					for (x=0;x<_size_x/2;x++)
					{
						_pointer8[copy][line_start-512+x] = 0;
						_pointer8[copy][line_start+x] = 0;
					}
				}
				for (x=16;x<_size_x/2;x+=32)
				{
					for (y=0;y<_size_y;y++)
					{
						_pointer8[copy][y*512+x-1] &= 0xF0;
						_pointer8[copy][y*512+x] &= 0x0F;
					}
				}
			}
			break;
	}

	video_vdp2_set_cycle_patterns_nbg_bmp(screenmode);
}

void pattern_convergence(video_screen_mode_t screenmode)
{
	video_screen_mode_t curr_screenmode = screenmode;
	int iPattern = 0;
	update_screen_mode(curr_screenmode,true); //re-initing in bmp mode
	draw_convergence(curr_screenmode,iPattern);

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
			update_screen_mode(curr_screenmode,true);
			draw_convergence(curr_screenmode,iPattern);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.r) )
		{
			curr_screenmode = next_screen_mode(curr_screenmode);
			update_screen_mode(curr_screenmode,true);
			update_screen_mode(curr_screenmode,true);
			draw_convergence(curr_screenmode,iPattern);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.a) || (controller.pressed.button.c) )
		{
			//change the checkered mode
			iPattern++;
			if (iPattern > 4) iPattern = 0;
			draw_convergence(curr_screenmode,iPattern);
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
