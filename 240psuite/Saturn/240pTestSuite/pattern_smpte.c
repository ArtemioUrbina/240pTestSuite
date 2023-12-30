#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "svin.h"
#include "control.h"
#include "ire.h"

void draw_smpte(_svin_screen_mode_t screenmode, bool bIRE100)
{
	//removing text
	ClearTextLayer();
	
	_svin_set_cycle_patterns_cpu();
	//add colors to palette
	uint8_t IRE_top = (bIRE100) ? Get_IRE_Level(100.0) : Get_IRE_Level(75);
	uint8_t IRE_bot = Get_IRE_Level(7.5);
	rgb888_t Color = {0,0,0,0};
	Color.r = IRE_top;
	Color.g = IRE_top;
	Color.b = IRE_top;	
	_svin_set_palette_part(2,&Color,1,1); //palette 2 color 1 = IRE white
	Color.b = IRE_bot;
	_svin_set_palette_part(2,&Color,2,2); //palette 2 color 2 = IRE yellow
	Color.r = IRE_bot;
	Color.b = IRE_top;
	_svin_set_palette_part(2,&Color,3,3); //palette 2 color 3 = IRE cyan
	Color.b = IRE_bot;
	_svin_set_palette_part(2,&Color,4,4); //palette 2 color 4 = IRE green
	Color.r = IRE_top;
	Color.g = IRE_bot;
	Color.b = IRE_top;	
	_svin_set_palette_part(2,&Color,5,5); //palette 2 color 5 = IRE pink
	Color.b = IRE_bot;	
	_svin_set_palette_part(2,&Color,6,6); //palette 2 color 6 = IRE red
	Color.r = IRE_bot;
	Color.b = IRE_top;
	_svin_set_palette_part(2,&Color,7,7); //palette 2 color 7 = IRE blue
	Color.b = IRE_bot;
	_svin_set_palette_part(2,&Color,8,8); //palette 2 color 8 = IRE black
	Color.r = IRE_bot;
	Color.g = Get_IRE_Level(29.2);//33;
	Color.b = Get_IRE_Level(44.2);//74;	
	_svin_set_palette_part(2,&Color,9,9); //palette 2 color 9 = -I
	Color.r = Get_IRE_Level(100.0);
	Color.g = Get_IRE_Level(100.0);
	Color.b = Get_IRE_Level(100.0);	
	_svin_set_palette_part(2,&Color,10,10); //palette 2 color 10 = white
	Color.r = Get_IRE_Level(30.0);//49;
	Color.g = IRE_bot;
	Color.b = Get_IRE_Level(49.2);//107;	
	_svin_set_palette_part(2,&Color,11,11); //palette 2 color 11 = +Q
	Color.r = Get_IRE_Level(3.5);
	Color.g = Get_IRE_Level(3.5);
	Color.b = Get_IRE_Level(3.5);	
	_svin_set_palette_part(2,&Color,12,12); //palette 2 color 12 = black-4
	Color.r = Get_IRE_Level(11.5);
	Color.g = Get_IRE_Level(11.5);
	Color.b = Get_IRE_Level(11.5);	
	_svin_set_palette_part(2,&Color,13,13); //palette 2 color 13 = black+4

	//using tiled mode is a real PITA, so we switch to BMP mode now
	//setup nbg0
    struct vdp2_scrn_bitmap_format format;
    memset(&format, 0x00, sizeof(format));
    format.scroll_screen = VDP2_SCRN_NBG0;
    format.ccc = VDP2_SCRN_CCC_PALETTE_16;
    format.bitmap_size = VDP2_SCRN_BITMAP_SIZE_1024X512;
    format.palette_base = 0x800;
    format.bitmap_base = _SVIN_NBG0_CHPNDR_START;
    vdp2_scrn_bitmap_format_set(&format);

	int _size_x = (_SVIN_X_RESOLUTION_320 == screenmode.x_res) ? 320: 352;
	if (screenmode.x_res_doubled) _size_x*=2;
	int _size_y = (VDP2_TVMD_VERT_224 == screenmode.y_res) ? 224 : 
					(VDP2_TVMD_VERT_240 == screenmode.y_res) ? 240 : 256;
	if (_SVIN_SCANMODE_480I == screenmode.scanmode)
		_size_y *= 2;

	uint8_t *_pointer8 = (uint8_t *)_SVIN_NBG0_CHPNDR_START;

	//draw top bars
	for (int i=0;i<7;i++)
	{
		uint8_t color = i+1;
		for (int y=0;y<(_size_y*67)/100;y++)
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
	//draw mid bars
	for (int i=0;i<7;i++)
	{
		uint8_t icolor = (i%2) ? 8 : 7-i;
		for (int y=(_size_y*67)/100;y<(_size_y*75)/100;y++)
		{
			int line_start = y*512;
			for (int x=(_size_x*i)/7;x<(_size_x*(i+1))/7;x++)
			{
				if (x%2)
				{
					_pointer8[line_start+x/2] &= 0xF0;
					_pointer8[line_start+x/2] |= icolor;
				}
				else
				{
					_pointer8[line_start+x/2] &= 0x0F;
					_pointer8[line_start+x/2] |= icolor<<4;

				}
			}
		}
	}
	//draw 3 left bottom bars
	for (int i=0;i<3;i++)
	{
		uint8_t icolor = i+9;
		for (int y=(_size_y*75)/100;y<_size_y;y++)
		{
			int line_start = y*512;
			for (int x=(_size_x*i*5)/28;x<(_size_x*(i+1)*5)/28;x++)
			{
				if (x%2)
				{
					_pointer8[line_start+x/2] &= 0xF0;
					_pointer8[line_start+x/2] |= icolor;
				}
				else
				{
					_pointer8[line_start+x/2] &= 0x0F;
					_pointer8[line_start+x/2] |= icolor<<4;

				}
			}
		}
	}
	//draw bottom black
	for (int y=(_size_y*75)/100;y<_size_y;y++)
	{
		int line_start = y*512;
		for (int x=(_size_x*15)/28;x<_size_x;x++)
		{
			if (x%2)
			{
				_pointer8[line_start+x/2] &= 0xF0;
				_pointer8[line_start+x/2] |= 8;
			}
			else
			{
				_pointer8[line_start+x/2] &= 0x0F;
				_pointer8[line_start+x/2] |= 8<<4;
			}
		}
	}
	//draw bottom black - 4
	for (int y=(_size_y*75)/100;y<_size_y;y++)
	{
		int line_start = y*512;
		for (int x=(_size_x*5)/7;x<(_size_x*16)/21;x++)
		{
			if (x%2)
			{
				_pointer8[line_start+x/2] &= 0xF0;
				_pointer8[line_start+x/2] |= 12;
			}
			else
			{
				_pointer8[line_start+x/2] &= 0x0F;
				_pointer8[line_start+x/2] |= 12<<4;
			}
		}
	}
	//draw bottom black + 4
	for (int y=(_size_y*75)/100;y<_size_y;y++)
	{
		int line_start = y*512;
		for (int x=(_size_x*17)/21;x<(_size_x*6)/7;x++)
		{
			if (x%2)
			{
				_pointer8[line_start+x/2] &= 0xF0;
				_pointer8[line_start+x/2] |= 13;
			}
			else
			{
				_pointer8[line_start+x/2] &= 0x0F;
				_pointer8[line_start+x/2] |= 13<<4;
			}
		}
	}

	_svin_set_cycle_patterns_nbg();
}

void pattern_smpte(_svin_screen_mode_t screenmode)
{
	_svin_screen_mode_t curr_screenmode = screenmode;
	bool bIRE100 = false;
	draw_smpte(curr_screenmode,bIRE100);
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
			draw_smpte(curr_screenmode,bIRE100);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.r) )
		{
			curr_screenmode = next_screen_mode(curr_screenmode);
			update_screen_mode(curr_screenmode);
			draw_smpte(curr_screenmode,bIRE100);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.a) || (controller.pressed.button.c) )
		{
			//change the checkered mode
			bIRE100 = bIRE100 ? false : true;
			draw_smpte(curr_screenmode,bIRE100);
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
