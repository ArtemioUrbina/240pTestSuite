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

void draw_grid(video_screen_mode_t screenmode)
{
	//removing text
	ClearTextLayer();
	
	video_vdp2_set_cycle_patterns_cpu();
	//add colors to palette
	uint8_t IRE_top = Get_IRE_Level(100.0);
	uint8_t IRE_bot = Get_IRE_Level(7.5);
	rgb888_t Color = {.cc=0,.r=IRE_bot,.g=IRE_bot,.b=IRE_bot};
	video_vdp2_set_palette_part(2,&Color,1,1); //palette 2 color 1 = black
	Color.r = IRE_top;
	Color.g = IRE_top;
	Color.b = IRE_top;	
	video_vdp2_set_palette_part(2,&Color,2,2); //palette 2 color 2 = white
	Color.g = IRE_bot;
	Color.b = IRE_bot;
	video_vdp2_set_palette_part(2,&Color,3,3); //palette 2 color 3 = red
	//clear all 8 tiles with black
	int copies = 1;
	uint8_t *_pointer8[2];
	int vram_offset[2];
	_pointer8[0] = (uint8_t *)VIDEO_VDP2_NBG0_CHPNDR_START;
	if (is_screenmode_special(screenmode))
	{
		copies = 2;
		_pointer8[0] = (uint8_t *)VIDEO_VDP2_NBG0_SPECIAL_CHPNDR_START;
		_pointer8[1] = (uint8_t *)VIDEO_VDP2_NBG1_SPECIAL_CHPNDR_START;
	}
	for (int copy = 0; copy < copies; copy++)
		for (int i=0; i<64*8; i++)
			_pointer8[copy][i] = 0x01;
	//create grid border and center tiles 0 at index 0, 4
	for (int copy = 0; copy < copies; copy++)
	{
		for (int i=0; i<8; i++)
		{
			_pointer8[copy][i] = 0x03;
			_pointer8[copy][8*i] = 0x03;
			_pointer8[copy][64*4+i] = 0x02;
			_pointer8[copy][64*4+8*i] = 0x02;
		}
		_pointer8[copy][63] = 0x03;
		_pointer8[copy][64*4+63] = 0x02;
	}
	//create grid border and center tiles 1 at index 1, 5
	for (int copy = 0; copy < copies; copy++)
	{
		for (int i=0; i<8; i++)
		{
			_pointer8[copy][64+i] = 0x03;
			_pointer8[copy][64+8*i+7] = 0x03;
			_pointer8[copy][64*5+i] = 0x02;
			_pointer8[copy][64*5+8*i+7] = 0x02;
		}
		_pointer8[copy][64+8*7] = 0x03;
		_pointer8[copy][64*5+8*7] = 0x02;
	}
	//create grid border and center tiles 2 at index 2, 6
	for (int copy = 0; copy < copies; copy++)
	{
		for (int i=0; i<8; i++)
		{
			_pointer8[copy][64*2+8*7+i] = 0x03;
			_pointer8[copy][64*2+8*i] = 0x03;
			_pointer8[copy][64*6+8*7+i] = 0x02;
			_pointer8[copy][64*6+8*i] = 0x02;
		}
		_pointer8[copy][64*2+7] = 0x03;
		_pointer8[copy][64*6+7] = 0x02;
	}
	//create grid border and center tiles 3 at index 3, 7
	for (int copy = 0; copy < copies; copy++)
	{
		for (int i=0; i<8; i++)
		{
			_pointer8[copy][64*3+8*7+i] = 0x03;
			_pointer8[copy][64*3+8*i+7] = 0x03;
			_pointer8[copy][64*7+8*7+i] = 0x02;
			_pointer8[copy][64*7+8*i+7] = 0x02;
		}
		_pointer8[copy][64*3] = 0x03;
		_pointer8[copy][64*7] = 0x02;
	}
	//fill planes with center tile
    uint32_t *_pointer32[2];
	_pointer32[0] = (uint32_t *)VIDEO_VDP2_NBG0_PNDR_START;
	vram_offset[0] = VIDEO_VDP2_NBG0_CHPNDR_START - VDP2_VRAM_ADDR(0,0);
	if (is_screenmode_special(screenmode))
	{
		_pointer32[0] = (uint32_t *)VIDEO_VDP2_NBG0_SPECIAL_PNDR_START;
		_pointer32[1] = (uint32_t *)VIDEO_VDP2_NBG1_SPECIAL_PNDR_START;
		vram_offset[0] = VIDEO_VDP2_NBG0_SPECIAL_CHPNDR_START - VDP2_VRAM_ADDR(0,0);
		vram_offset[1] = VIDEO_VDP2_NBG1_SPECIAL_CHPNDR_START - VDP2_VRAM_ADDR(0,0);
	}
	for (int copy = 0; copy < copies; copy++)
	{
		for (unsigned int y = 0; y < 32; y++)
		{
			for (unsigned int x = 0; x < 32; x++)
			{
				_pointer32[copy][y*128+x*2] = 0x00200008 + vram_offset[copy]/32; 
				_pointer32[copy][y*128+x*2+1] = 0x0020000A + vram_offset[copy]/32;
				_pointer32[copy][y*128+64+x*2] = 0x0020000C + vram_offset[copy]/32; 
				_pointer32[copy][y*128+64+x*2+1] = 0x0020000E + vram_offset[copy]/32;
				_pointer32[copy][64*64+y*128+x*2] = 0x00200008 + vram_offset[copy]/32; 
				_pointer32[copy][64*64+y*128+x*2+1] = 0x0020000A + vram_offset[copy]/32;
				_pointer32[copy][64*64+y*128+64+x*2] = 0x0020000C + vram_offset[copy]/32; 
				_pointer32[copy][64*64+y*128+64+x*2+1] = 0x0020000E + vram_offset[copy]/32;
			}
		}
	}
	//fill border
	int _size_x = (VIDEO_X_RESOLUTION_320 == screenmode.x_res) ? 20 : 22;
	int _size_y = get_screenmode_resolution_y(screenmode)/16;
	for (int copy = 0; copy < copies; copy++)
	{
		if (screenmode.x_res_doubled)
		{
			//plane0
			for (int x=0; x<32; x++)
			{
				//top
				_pointer32[copy][x*2] = 0x00200000 + vram_offset[copy]/32; 
				_pointer32[copy][x*2+1] = 0x00200002 + vram_offset[copy]/32;
				_pointer32[copy][x*2+64] = 0x00200004 + vram_offset[copy]/32; 
				_pointer32[copy][x*2+64+1] = 0x00200006 + vram_offset[copy]/32;
				//bottom
				_pointer32[copy][128*_size_y-128+x*2] = 0x00200000 + vram_offset[copy]/32; 
				_pointer32[copy][128*_size_y-128+x*2+1] = 0x00200002 + vram_offset[copy]/32;
				_pointer32[copy][128*_size_y-128+x*2+64] = 0x00200004 + vram_offset[copy]/32; 
				_pointer32[copy][128*_size_y-128+x*2+64+1] = 0x00200006 + vram_offset[copy]/32;
			}
			for (int y=0; y<_size_y; y++)
			{
				//left
				_pointer32[copy][y*128] = 0x00200000 + vram_offset[copy]/32; 
				_pointer32[copy][y*128+1] = 0x00200002 + vram_offset[copy]/32;
				_pointer32[copy][y*128+64] = 0x00200004 + vram_offset[copy]/32; 
				_pointer32[copy][y*128+64+1] = 0x00200006 + vram_offset[copy]/32;
			}
			//plane1
			for (int x=0; x<(_size_x*2-32); x++)
			{
				//top
				_pointer32[copy][64*64+x*2] = 0x00200000 + vram_offset[copy]/32; 
				_pointer32[copy][64*64+x*2+1] = 0x00200002 + vram_offset[copy]/32;
				_pointer32[copy][64*64+x*2+64] = 0x00200004 + vram_offset[copy]/32; 
				_pointer32[copy][64*64+x*2+64+1] = 0x00200006 + vram_offset[copy]/32;
				//bottom
				_pointer32[copy][64*64+128*_size_y-128+x*2] = 0x00200000 + vram_offset[copy]/32; 
				_pointer32[copy][64*64+128*_size_y-128+x*2+1] = 0x00200002 + vram_offset[copy]/32;
				_pointer32[copy][64*64+128*_size_y-128+x*2+64] = 0x00200004 + vram_offset[copy]/32; 
				_pointer32[copy][64*64+128*_size_y-128+x*2+64+1] = 0x00200006 + vram_offset[copy]/32;
			}
			for (int y=0; y<_size_y; y++)
			{
				//right
				_pointer32[copy][64*64+y*128+(_size_x*4-64)-2] = 0x00200000 + vram_offset[copy]/32; 
				_pointer32[copy][64*64+y*128+(_size_x*4-64)-1] = 0x00200002 + vram_offset[copy]/32;
				_pointer32[copy][64*64+y*128+(_size_x*4-64)-2+64] = 0x00200004 + vram_offset[copy]/32; 
				_pointer32[copy][64*64+y*128+(_size_x*4-64)-1+64] = 0x00200006 + vram_offset[copy]/32;
			}
		}
		else
		{
			//everything on a single plane
			for (int x=0; x<_size_x; x++)
			{
				//top
				_pointer32[copy][x*2] = 0x00200000 + vram_offset[copy]/32; 
				_pointer32[copy][x*2+1] = 0x00200002 + vram_offset[copy]/32;
				_pointer32[copy][x*2+64] = 0x00200004 + vram_offset[copy]/32; 
				_pointer32[copy][x*2+64+1] = 0x00200006 + vram_offset[copy]/32;
				//bottom
				_pointer32[copy][128*_size_y-128+x*2] = 0x00200000 + vram_offset[copy]/32; 
				_pointer32[copy][128*_size_y-128+x*2+1] = 0x00200002 + vram_offset[copy]/32;
				_pointer32[copy][128*_size_y-128+x*2+64] = 0x00200004 + vram_offset[copy]/32; 
				_pointer32[copy][128*_size_y-128+x*2+64+1] = 0x00200006 + vram_offset[copy]/32;
			}
			for (int y=0; y<_size_y; y++)
			{
				//left
				_pointer32[copy][y*128] = 0x00200000 + vram_offset[copy]/32; 
				_pointer32[copy][y*128+1] = 0x00200002 + vram_offset[copy]/32;
				_pointer32[copy][y*128+64] = 0x00200004 + vram_offset[copy]/32; 
				_pointer32[copy][y*128+64+1] = 0x00200006 + vram_offset[copy]/32;
				//right
				_pointer32[copy][y*128+_size_x*2-2] = 0x00200000 + vram_offset[copy]/32; 
				_pointer32[copy][y*128+_size_x*2-1] = 0x00200002 + vram_offset[copy]/32;
				_pointer32[copy][y*128+_size_x*2-2+64] = 0x00200004 + vram_offset[copy]/32; 
				_pointer32[copy][y*128+_size_x*2-1+64] = 0x00200006 + vram_offset[copy]/32;
			}
		}
	}
	video_vdp2_set_cycle_patterns_nbg(screenmode);
}

void pattern_grid(video_screen_mode_t screenmode)
{
	video_screen_mode_t curr_screenmode = screenmode;
	draw_grid(curr_screenmode);

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
			draw_grid(curr_screenmode);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.r) )
		{
			curr_screenmode = next_screen_mode(curr_screenmode);
			update_screen_mode(curr_screenmode,false);
			draw_grid(curr_screenmode);
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
	}
}