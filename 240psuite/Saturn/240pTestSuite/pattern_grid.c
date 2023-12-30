#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "svin.h"
#include "video.h"
#include "control.h"
#include "ire.h"

void draw_grid(_svin_screen_mode_t screenmode)
{
	//removing text
	ClearTextLayer();
	
	_svin_set_cycle_patterns_cpu();
	//add colors to palette
	uint8_t IRE_top = Get_IRE_Level(100.0);
	uint8_t IRE_bot = Get_IRE_Level(7.5);
	rgb888_t Color = {0,IRE_bot,IRE_bot,IRE_bot};
	_svin_set_palette_part(2,&Color,1,1); //palette 2 color 1 = black
	Color.r = IRE_top;
	Color.g = IRE_top;
	Color.b = IRE_top;	
	_svin_set_palette_part(2,&Color,2,2); //palette 2 color 2 = white
	Color.g = IRE_bot;
	Color.b = IRE_bot;
	_svin_set_palette_part(2,&Color,3,3); //palette 2 color 3 = red
	//clear all 8 tiles with black
	uint8_t *_pointer8 = (uint8_t *)_SVIN_NBG0_CHPNDR_START;
	for (int i=0; i<64*8; i++)
	{
		_pointer8[i] = 0x01;
	}
	//create grid border and center tiles 0 at index 0, 4
	for (int i=0; i<8; i++)
	{
		_pointer8[i] = 0x03;
		_pointer8[8*i] = 0x03;
		_pointer8[64*4+i] = 0x02;
		_pointer8[64*4+8*i] = 0x02;
	}
	_pointer8[63] = 0x03;
	_pointer8[64*4+63] = 0x02;
	//create grid border and center tiles 1 at index 1, 5
	for (int i=0; i<8; i++)
	{
		_pointer8[64+i] = 0x03;
		_pointer8[64+8*i+7] = 0x03;
		_pointer8[64*5+i] = 0x02;
		_pointer8[64*5+8*i+7] = 0x02;
	}
	_pointer8[64+8*7] = 0x03;
	_pointer8[64*5+8*7] = 0x02;
	//create grid border and center tiles 2 at index 2, 6
	for (int i=0; i<8; i++)
	{
		_pointer8[64*2+8*7+i] = 0x03;
		_pointer8[64*2+8*i] = 0x03;
		_pointer8[64*6+8*7+i] = 0x02;
		_pointer8[64*6+8*i] = 0x02;
	}
	_pointer8[64*2+7] = 0x03;
	_pointer8[64*6+7] = 0x02;
	//create grid border and center tiles 3 at index 3, 7
	for (int i=0; i<8; i++)
	{
		_pointer8[64*3+8*7+i] = 0x03;
		_pointer8[64*3+8*i+7] = 0x03;
		_pointer8[64*7+8*7+i] = 0x02;
		_pointer8[64*7+8*i+7] = 0x02;
	}
	_pointer8[64*3] = 0x03;
	_pointer8[64*7] = 0x02;
	//fill planes with center tile
    uint32_t *_pointer32 = (int *)_SVIN_NBG0_PNDR_START;
    for (unsigned int y = 0; y < 32; y++)
    {
		for (unsigned int x = 0; x < 32; x++)
		{
        	_pointer32[y*128+x*2] = 0x00200008; 
        	_pointer32[y*128+x*2+1] = 0x0020000A;
        	_pointer32[y*128+64+x*2] = 0x0020000C; 
        	_pointer32[y*128+64+x*2+1] = 0x0020000E;
			_pointer32[64*64+y*128+x*2] = 0x00200008; 
        	_pointer32[64*64+y*128+x*2+1] = 0x0020000A;
        	_pointer32[64*64+y*128+64+x*2] = 0x0020000C; 
        	_pointer32[64*64+y*128+64+x*2+1] = 0x0020000E;
		}
    }
	//fill border
	int _size_x = (_SVIN_X_RESOLUTION_320 == screenmode.x_res) ? 20 : 22;
	int _size_y = (VDP2_TVMD_VERT_224 == screenmode.y_res) ? 14 : 
					(VDP2_TVMD_VERT_240 == screenmode.y_res) ? 15 : 16;
	if (_SVIN_SCANMODE_480I == screenmode.scanmode)
		_size_y *= 2;
	if (screenmode.x_res_doubled)
	{
		//plane0
		for (int x=0; x<32; x++)
		{
			//top
			_pointer32[x*2] = 0x00200000; 
			_pointer32[x*2+1] = 0x00200002;
			_pointer32[x*2+64] = 0x00200004; 
			_pointer32[x*2+64+1] = 0x00200006;
			//bottom
			_pointer32[128*_size_y-128+x*2] = 0x00200000; 
			_pointer32[128*_size_y-128+x*2+1] = 0x00200002;
			_pointer32[128*_size_y-128+x*2+64] = 0x00200004; 
			_pointer32[128*_size_y-128+x*2+64+1] = 0x00200006;
		}
		for (int y=0; y<_size_y; y++)
		{
			//left
			_pointer32[y*128] = 0x00200000; 
			_pointer32[y*128+1] = 0x00200002;
			_pointer32[y*128+64] = 0x00200004; 
			_pointer32[y*128+64+1] = 0x00200006;
		}
		//plane1
		for (int x=0; x<(_size_x*2-32); x++)
		{
			//top
			_pointer32[64*64+x*2] = 0x00200000; 
			_pointer32[64*64+x*2+1] = 0x00200002;
			_pointer32[64*64+x*2+64] = 0x00200004; 
			_pointer32[64*64+x*2+64+1] = 0x00200006;
			//bottom
			_pointer32[64*64+128*_size_y-128+x*2] = 0x00200000; 
			_pointer32[64*64+128*_size_y-128+x*2+1] = 0x00200002;
			_pointer32[64*64+128*_size_y-128+x*2+64] = 0x00200004; 
			_pointer32[64*64+128*_size_y-128+x*2+64+1] = 0x00200006;
		}
		for (int y=0; y<_size_y; y++)
		{
			//right
			_pointer32[64*64+y*128+(_size_x*4-64)-2] = 0x00200000; 
			_pointer32[64*64+y*128+(_size_x*4-64)-1] = 0x00200002;
			_pointer32[64*64+y*128+(_size_x*4-64)-2+64] = 0x00200004; 
			_pointer32[64*64+y*128+(_size_x*4-64)-1+64] = 0x00200006;
		}
	}
	else
	{
		//everything on a single plane
		for (int x=0; x<_size_x; x++)
		{
			//top
			_pointer32[x*2] = 0x00200000; 
			_pointer32[x*2+1] = 0x00200002;
			_pointer32[x*2+64] = 0x00200004; 
			_pointer32[x*2+64+1] = 0x00200006;
			//bottom
			_pointer32[128*_size_y-128+x*2] = 0x00200000; 
			_pointer32[128*_size_y-128+x*2+1] = 0x00200002;
			_pointer32[128*_size_y-128+x*2+64] = 0x00200004; 
			_pointer32[128*_size_y-128+x*2+64+1] = 0x00200006;
		}
		for (int y=0; y<_size_y; y++)
		{
			//left
			_pointer32[y*128] = 0x00200000; 
			_pointer32[y*128+1] = 0x00200002;
			_pointer32[y*128+64] = 0x00200004; 
			_pointer32[y*128+64+1] = 0x00200006;
			//right
			_pointer32[y*128+_size_x*2-2] = 0x00200000; 
			_pointer32[y*128+_size_x*2-1] = 0x00200002;
			_pointer32[y*128+_size_x*2-2+64] = 0x00200004; 
			_pointer32[y*128+_size_x*2-1+64] = 0x00200006;
		}
	}
	_svin_set_cycle_patterns_nbg();
}

void pattern_grid(_svin_screen_mode_t screenmode)
{
	_svin_screen_mode_t curr_screenmode = screenmode;
	draw_grid(curr_screenmode);
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
			draw_grid(curr_screenmode);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.r) )
		{
			curr_screenmode = next_screen_mode(curr_screenmode);
			update_screen_mode(curr_screenmode);
			draw_grid(curr_screenmode);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.a) || (controller.pressed.button.b) || (controller.pressed.button.c) )
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
				ClearTextLayer();
		}
	}
}