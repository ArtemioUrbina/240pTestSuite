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

uint8_t BrickPattern[64] = {
	3,3,2,2,2,2,2,2,
	2,2,3,3,2,2,2,2,
	2,2,2,2,3,3,2,2,
	1,2,2,2,2,1,3,3,
	3,3,1,2,2,2,1,3,
	2,2,3,3,2,1,1,3,
	2,2,2,2,3,3,1,3,
	2,2,2,2,2,2,3,3,	
};

static video_screen_mode_t curr_screenmode;

void draw_pixel(int x, int y, int color)
{
	int copies = 1;
	uint8_t *_pointer8[2];
	_pointer8[0] = (uint8_t *)VIDEO_VDP2_NBG0_CHPNDR_START;
	if (is_screenmode_special(curr_screenmode))
	{
		copies = 2;
		_pointer8[0] = (uint8_t *)VIDEO_VDP2_NBG0_SPECIAL_BMP_START;
		_pointer8[1] = (uint8_t *)VIDEO_VDP2_NBG1_SPECIAL_BMP_START;
	}
	for (int copy = 0; copy < copies; copy++)
	{
		if (x%2)
		{
			_pointer8[copy][y*512+x/2] &= 0xF0;
			_pointer8[copy][y*512+x/2] |= color;
		}
		else
		{
			_pointer8[copy][y*512+x/2] &= 0x0F;
			_pointer8[copy][y*512+x/2] |= color<<4;
		}
	}
}

void draw_sharpness(video_screen_mode_t screenmode, bool bIRE100)
{
	int x,y;
	//removing text
	ClearTextLayer();
	
	video_vdp2_set_cycle_patterns_cpu();
	//add colors to palette
	uint8_t IRE_top = (bIRE100) ?  Get_IRE_Level(100) : Get_IRE_Level(75);
	uint8_t IRE_bot = Get_IRE_Level(7.5);
	rgb888_t Color = {.cc=0,.r=0,.g=0,.b=0};
	Color.r = IRE_top;
	Color.g = IRE_top;
	Color.b = IRE_top;	
	video_vdp2_set_palette_part(2,&Color,1,1); //palette 2 color 1 = IRE white 100%
	Color.r = IRE_bot + (IRE_top-IRE_bot)/2;
	Color.g = IRE_bot + (IRE_top-IRE_bot)/2;
	Color.b = IRE_bot + (IRE_top-IRE_bot)/2;
	video_vdp2_set_palette_part(2,&Color,2,2); //palette 2 color 2 = IRE gray 50%
	Color.r = IRE_bot;
	Color.g = IRE_bot;
	Color.b = IRE_bot;
	video_vdp2_set_palette_part(2,&Color,3,3); //palette 2 color 3 = IRE black

	int _size_x = get_screenmode_resolution_x(screenmode);
	int _size_y = get_screenmode_resolution_y(screenmode);

	int copies = 1;
	uint8_t *_pointer8[2];
	_pointer8[0] = (uint8_t *)VIDEO_VDP2_NBG0_CHPNDR_START;
	if (is_screenmode_special(screenmode))
	{
		copies = 2;
		_pointer8[0] = (uint8_t *)VIDEO_VDP2_NBG0_SPECIAL_BMP_START;
		_pointer8[1] = (uint8_t *)VIDEO_VDP2_NBG1_SPECIAL_BMP_START;
	}

	//fill with gray
	for (int copy = 0; copy < copies; copy++)
		memset(_pointer8[copy], 0x22, _size_y*512);

	//center horizontal bars
	for (int i=0;i<5;i++)
	{
		for (y=(_size_y*(i+8)/20)-(i+1)/2;y<(_size_y*(i+8)/20)+(i+2)/2;y++)
		{
			for (x=(_size_x*7)/20;x<(_size_x*13)/20;x++)
			{
				draw_pixel(x,y,3);//using black color
			}
		}
	}

	//center vertical bars
	for (int i=0;i<5;i++)
	{
		for (y=(_size_y*11)/30;y<(_size_y*19)/30;y++)
		{
			for (x=(_size_x*(i+6)/16)-(i+1)/2;x<(_size_x*(i+6)/16)+(i+2)/2;x++)
			{
				draw_pixel(x,y,3);//using black color
			}
		}
	}

	//circle
	int r = 2*_size_x/11;
	int r_fullsize=0;
	if (screenmode.x_res_doubled)
		r_fullsize = r;
	else
		r_fullsize = r*2;
	int r2 = r_fullsize*r_fullsize;
	y = r_fullsize;
	for (x=0;x<r_fullsize;x++)
	{
		while(x*x+y*y > r2)
			y--;
		int _x = x;		
		if (false == screenmode.x_res_doubled) _x /= 2;
		int _y = y;
		if ( (VIDEO_SCANMODE_240I == screenmode.scanmode) || (VIDEO_SCANMODE_240P == screenmode.scanmode) ) _y /= 2;
		draw_pixel(_size_x/2+_x,_size_y/2+_y,3);//using black color
		draw_pixel(_size_x/2+_x,_size_y/2-_y,3);//using black color
		draw_pixel(_size_x/2-_x,_size_y/2+_y,3);//using black color
		draw_pixel(_size_x/2-_x,_size_y/2-_y,3);//using black color
	}
	x = r_fullsize;
	for (y=0;y<r_fullsize;y++)
	{
		while(x*x+y*y > r2)
			x--;
		int _x = x;		
		if (false == screenmode.x_res_doubled) _x /= 2;
		int _y = y;
		if ( (VIDEO_SCANMODE_240I == screenmode.scanmode) || (VIDEO_SCANMODE_240P == screenmode.scanmode) ) _y /= 2;
		draw_pixel(_size_x/2+_x,_size_y/2+_y,3);//using black color
		draw_pixel(_size_x/2+_x,_size_y/2-_y,3);//using black color
		draw_pixel(_size_x/2-_x,_size_y/2+_y,3);//using black color
		draw_pixel(_size_x/2-_x,_size_y/2-_y,3);//using black color
	}

	//cross
	int r_y = r;
	if ( (VIDEO_SCANMODE_240I == screenmode.scanmode) || (VIDEO_SCANMODE_240P == screenmode.scanmode) ) r_y /= 2;
	if (false == screenmode.x_res_doubled) r_y *= 2;
	for (x=0;x<_size_x/2-r;x++)
	{
		draw_pixel(x,_size_y/2,3);//using black color
		draw_pixel(_size_x-x,_size_y/2,3);//using black color
	}
	for (y=0;y<_size_y/2-r_y;y++)
	{
		draw_pixel(_size_x/2,y,3);//using black color
		draw_pixel(_size_x/2,_size_y-y,3);//using black color
	}

	//diagonal cross
	for (x=0;x<_size_x/2;x++)
	{
		y = x * _size_y/_size_x;
		int _x = _size_x/2-x;		
		if (false == screenmode.x_res_doubled) _x *= 2;
		int _y = _size_y/2-y;
		if ( (VIDEO_SCANMODE_480I == screenmode.scanmode) || (VIDEO_SCANMODE_480P == screenmode.scanmode) ) _y *= 2;
		if (_x*_x+_y*_y > r2)
		{
			draw_pixel(x,y,3);//using black color
			draw_pixel(_size_x-x,y,3);//using black color
			draw_pixel(x,_size_y-y,3);//using black color
			draw_pixel(_size_x-x,_size_y-y,3);//using black color
		}
	}

	//white borders
	for (x=_size_x/29;x<_size_x-_size_x/29;x++) draw_pixel(x,_size_y/29,1);
	for (x=_size_x/29;x<_size_x-_size_x/29;x++) draw_pixel(x,_size_y-_size_y/29,1);
	for (y=_size_y/29;y<_size_y-_size_y/29;y++) draw_pixel(_size_x/29,y,1);
	for (y=_size_y/29;y<_size_y-_size_y/29;y++) draw_pixel(_size_x-_size_x/29,y,1);
	for (x=_size_x/13;x<_size_x-_size_x/13;x++) draw_pixel(x,_size_y/13,1);
	for (x=_size_x/13;x<_size_x-_size_x/13;x++) draw_pixel(x,_size_y-_size_y/13,1);
	for (y=_size_y/13;y<_size_y-_size_y/13;y++) draw_pixel(_size_x/13,y,1);
	for (y=_size_y/13;y<_size_y-_size_y/13;y++) draw_pixel(_size_x-_size_x/13,y,1);

	video_vdp2_set_cycle_patterns_nbg_bmp(screenmode);
}

void draw_sharpness_pattern2(video_screen_mode_t screenmode)
{
	//removing text
	ClearTextLayer();
	
	video_vdp2_set_cycle_patterns_cpu();

	//add colors to palette
	rgb888_t Color;
	Color.r = 33;
	Color.g = 0;
	Color.b = 0;	
	video_vdp2_set_palette_part(2,&Color,1,1); //palette 2 color 1
	Color.r = 74;
	Color.g = 33;
	Color.b = 33;	
	video_vdp2_set_palette_part(2,&Color,2,2); //palette 2 color 2
	Color.r = 107;
	Color.g = 107;
	Color.b = 74;	
	video_vdp2_set_palette_part(2,&Color,3,3); //palette 2 color 3

	//create brick tile
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
	int *BrickPattern32 = (int *)BrickPattern;
	for (int copy = 0; copy < copies; copy++)
		for (unsigned int i = 0; i < 16; i++)
			_pointer32[copy][i] = BrickPattern32[i];

	//fill everything with a brick tile
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
    	for (unsigned int i = 0; i < VIDEO_VDP2_NBG0_PNDR_SIZE / sizeof(int); i++)
        	_pointer32[copy][i] = 0x00200000 + vram_offset[copy]/32; //palette 2
	video_vdp2_set_cycle_patterns_nbg(screenmode);
}

void pattern_sharpness(video_screen_mode_t screenmode)
{
	curr_screenmode = screenmode;
	int iPattern = 0;
	update_screen_mode(curr_screenmode,true); //re-initing in bmp mode
	draw_sharpness(curr_screenmode,false);

	wait_for_key_unpress();
	
	int mode_display_counter = 0;

	while (1)
	{
		smpc_peripheral_process();
		get_digital_keypress_anywhere(&controller);
		if ( (controller.pressed.button.l) )
		{
			curr_screenmode = prev_screen_mode(curr_screenmode);
			if (iPattern)
			{
				update_screen_mode(curr_screenmode,false);
				draw_sharpness_pattern2(curr_screenmode);
			}
			else
			{
				update_screen_mode(curr_screenmode,true);
				update_screen_mode(curr_screenmode,true);
				draw_sharpness(curr_screenmode,false);
			}
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.r) )
		{
			curr_screenmode = next_screen_mode(curr_screenmode);
			if (iPattern)
			{
				update_screen_mode(curr_screenmode,false);
				draw_sharpness_pattern2(curr_screenmode);
			}
			else
			{
				update_screen_mode(curr_screenmode,true);
				update_screen_mode(curr_screenmode,true);
				draw_sharpness(curr_screenmode,false);
			}
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.a) || (controller.pressed.button.c) )
		{
			//change the checkered mode
			iPattern = iPattern ? 0 : 1;
			if (iPattern)
			{
				update_screen_mode(curr_screenmode,false);
				draw_sharpness_pattern2(curr_screenmode);
			}
			else
			{
				update_screen_mode(curr_screenmode,true);
				update_screen_mode(curr_screenmode,true);
				draw_sharpness(curr_screenmode,false);
			}
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
