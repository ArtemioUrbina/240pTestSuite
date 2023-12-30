#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "svin.h"
#include "control.h"
#include "ire.h"

#define COLOR_WHITE 1
#define COLOR_RED 2
#define COLOR_BLACK 3

void draw_point(int x, int y, int color)
{
	uint8_t *_pointer8 = (uint8_t *)_SVIN_NBG0_CHPNDR_START;
	if (x%2==1)
	{
		_pointer8[y*512+x/2] |= 0x01*color;
	}
	else
	{
		_pointer8[y*512+x/2] |= 0x10*color;
	}
}

void draw_rectangle(int x1, int y1, int x2, int y2, int color)
{
	uint8_t *_pointer8 = (uint8_t *)_SVIN_NBG0_CHPNDR_START;
	int _x = x1;
	if (_x%2==1)
	{
		_pointer8[y1*512+_x/2] |= 0x01*color;
		_pointer8[y2*512+_x/2] |= 0x01*color;
		_x++;
	}
	while (_x<((x2/2)*2))
	{
		_pointer8[y1*512+_x/2] |= 0x11*color;
		_pointer8[y2*512+_x/2] |= 0x11*color;
		_x+=2;
	}
	//_x-=2;
	if (_x<x2)
	{
		_pointer8[y1*512+_x/2] |= 0x10*color;
		_pointer8[y2*512+_x/2] |= 0x10*color;
	}
	for (int _y=y1;_y<=y2;_y++)
	{
		if (x1%2==0)
			_pointer8[_y*512+x1/2] |= 0x10*color;
		else
			_pointer8[_y*512+x1/2] |= 0x01*color;

		if (x2%2==0)
			_pointer8[_y*512+x2/2] |= 0x10*color;
		else
			_pointer8[_y*512+x2/2] |= 0x01*color;
	}
}

void clear_rectangle(int x1, int y1, int x2, int y2)
{
	uint8_t *_pointer8 = (uint8_t *)_SVIN_NBG0_CHPNDR_START;
	int _x = x1;
	if (_x%2==1)
	{
		_pointer8[y1*512+_x/2] &= ~0x0F;
		_pointer8[y2*512+_x/2] &= ~0x0F;
		_x++;
	}
	while (_x<((x2/2)*2))
	{
		_pointer8[y1*512+_x/2] &= ~0xFF;
		_pointer8[y2*512+_x/2] &= ~0xFF;
		_x+=2;
	}
	//_x-=2;
	//if (_x<x2)
	if (0==(x2%2))
	{
		_pointer8[y1*512+_x/2] &= ~0xF0;
		_pointer8[y2*512+_x/2] &= ~0xF0;
	}
	for (int _y=y1;_y<=y2;_y++)
	{
		if (x1%2==0)
			_pointer8[_y*512+x1/2] &= ~0xF0;
		else
			_pointer8[_y*512+x1/2] &= ~0x0F;

		if (x2%2==0)
			_pointer8[_y*512+x2/2] &= ~0xF0;
		else
			_pointer8[_y*512+x2/2] &= ~0x0F;
	}
}

void clear_filled(int x1, int y1, int x2, int y2)
{
	uint8_t *_pointer8 = (uint8_t *)_SVIN_NBG0_CHPNDR_START;
	for (int _y=y1;_y<=y2;_y++)
	{
		int _x = x1;
		if (_x%2==1)
		{
			_pointer8[_y*512+_x/2] &= ~0x0F;
			//_pointer8[_y*512+_x/2] |= 0x02;
			_x++;
		}
		while (_x<x2)
		{
			_pointer8[_y*512+_x/2] &= ~0xFF;
			//_pointer8[_y*512+_x/2] |= 0x22;
			_x+=2;
		}
		if (0==(x2%2))
		{
			_pointer8[_y*512+x2/2] &= ~0xF0;
			//_pointer8[_y*512+x2/2] |= 0x20;
		}
	}
}


void draw_rectangle_16x16(int x, int y, int color)
{
	draw_rectangle(x,y,x+15,y+15,color);
}

void draw_rectangle_16x16_hlines(int x, int y, int color)
{
	clear_filled(x,y,x+15,y+15);
	draw_rectangle_16x16(x,y,color);
	draw_rectangle(x+3,y+3,x+12,y+3,color);
	draw_rectangle(x+3,y+6,x+12,y+6,color);
	draw_rectangle(x+3,y+9,x+12,y+9,color);
	draw_rectangle(x+3,y+12,x+12,y+12,color);
}

void draw_rectangle_16x16_vlines(int x, int y, int color)
{
	clear_filled(x,y,x+15,y+15);
	draw_rectangle_16x16(x,y,color);
	draw_rectangle(x+3,y+3,x+3,y+12,color);
	draw_rectangle(x+6,y+3,x+6,y+12,color);
	draw_rectangle(x+9,y+3,x+9,y+12,color);
	draw_rectangle(x+12,y+3,x+12,y+12,color);
}

void draw_rectangle_16x16_funnypattern(int x, int y, int color)
{
	clear_filled(x,y,x+15,y+15);
	draw_rectangle_16x16(x,y,color);
	draw_point(x+3,y+4,color);
	draw_point(x+4,y+3,color);
	draw_point(x+3,y+11,color);
	draw_point(x+4,y+12,color);
	draw_point(x+11,y+3,color);
	draw_point(x+12,y+4,color);
	draw_point(x+11,y+12,color);
	draw_point(x+12,y+11,color);

	draw_point(x+5,y+5,color);
	draw_point(x+10,y+5,color);
	draw_point(x+5,y+10,color);
	draw_point(x+10,y+10,color);

	draw_rectangle(x+7,y+3,x+8,y+3,color);
	draw_rectangle(x+3,y+7,x+3,y+8,color);
	draw_rectangle(x+7,y+12,x+8,y+12,color);
	draw_rectangle(x+12,y+7,x+12,y+8,color);

	draw_rectangle(x+7,y+7,x+8,y+8,color);

	draw_point(x+3,y+4,color);
	draw_point(x+4,y+3,color);
	draw_point(x+5,y+5,color);
}


void draw_rectangle_set_17x16x4(int x, int y, int color)
{
	clear_filled(x,y,x+34,y+32);
	draw_rectangle(x+1,y+1,x+17,y+15,color);
	draw_rectangle(x+17,y+1,x+33,y+15,color);
	draw_rectangle(x+1,y+16,x+17,y+31,color);
	draw_rectangle(x+17,y+16,x+33,y+31,color);
	draw_rectangle(x,y,x+34,y+32,color);
	draw_rectangle(x,y,x+34,y+32,color);
	//crosshair
	draw_rectangle(x+4,y+24,x+14,y+24,color);
	draw_rectangle(x+9,y+20,x+9,y+28,color);
}

void draw_monoscope(_svin_screen_mode_t screenmode, bool bIRE100)
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
	Color.g = IRE_bot;
	Color.b = IRE_bot;	
	_svin_set_palette_part(2,&Color,2,2); //palette 2 color 2 = IRE red
	Color.r = IRE_bot;
	_svin_set_palette_part(2,&Color,0,0); //palette 2 color 0 = IRE black
	_svin_set_palette_part(2,&Color,3,3); //palette 2 color 3 = IRE black

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

	//fill everything with black
	memset(_pointer8,0x00,512*_size_y);

	//draw borders rectangles
	for (int x=0;x<_size_x;x+=16) draw_rectangle_16x16(x,0,COLOR_WHITE);
	for (int x=0;x<_size_x;x+=16) draw_rectangle_16x16(x,_size_y-16,COLOR_WHITE);
	for (int y=0;y<_size_y;y+=16) draw_rectangle_16x16(0,y,COLOR_WHITE);
	for (int y=0;y<_size_y;y+=16) draw_rectangle_16x16(_size_x-16,y,COLOR_WHITE);
	draw_rectangle(16,16,_size_x-17,_size_y-17,COLOR_WHITE);

	//draw dots
	for (int y=0;y<_size_y;y+=16) 
		for (int x=0;x<_size_x;x+=16) 
			draw_rectangle(x+7,y+7,x+8,y+8,COLOR_WHITE);

	//draw corner rectangles
	draw_rectangle_16x16_funnypattern(0,0,COLOR_WHITE);
	draw_rectangle_16x16_hlines(16,0,COLOR_WHITE);
	draw_rectangle_16x16_vlines(0,16,COLOR_WHITE);
	draw_rectangle_16x16_funnypattern(16,16,COLOR_WHITE);
	draw_rectangle(16,16,32,32,COLOR_WHITE);

	draw_rectangle_16x16_hlines(0,_size_y-32,COLOR_WHITE);
	draw_rectangle_16x16_funnypattern(16,_size_y-32,COLOR_WHITE);
	draw_rectangle_16x16_funnypattern(0,_size_y-16,COLOR_WHITE);
	draw_rectangle_16x16_vlines(16,_size_y-16,COLOR_WHITE);
	draw_rectangle(16,_size_y-33,32,_size_y-16,COLOR_WHITE);

	draw_rectangle_16x16_vlines(_size_x-32,0,COLOR_WHITE);
	draw_rectangle_16x16_funnypattern(_size_x-16,0,COLOR_WHITE);
	draw_rectangle_16x16_funnypattern(_size_x-32,16,COLOR_WHITE);
	draw_rectangle_16x16_hlines(_size_x-16,16,COLOR_WHITE);
	draw_rectangle(_size_x-33,16,_size_x-16,32,COLOR_WHITE);

	draw_rectangle_16x16_funnypattern(_size_x-32,_size_y-32,COLOR_WHITE);
	draw_rectangle_16x16_vlines(_size_x-16,_size_y-32,COLOR_WHITE);
	draw_rectangle_16x16_hlines(_size_x-32,_size_y-16,COLOR_WHITE);
	draw_rectangle_16x16_funnypattern(_size_x-16,_size_y-16,COLOR_WHITE);
	draw_rectangle(_size_x-33,_size_y-33,_size_x-16,_size_y-16,COLOR_WHITE);

	//draw top and bottom rectangles
	draw_rectangle_16x16_funnypattern(_size_x/2-16,0,COLOR_WHITE);
	draw_rectangle_16x16_funnypattern(_size_x/2,0,COLOR_WHITE);
	draw_rectangle_16x16_hlines(_size_x/2-16,16,COLOR_WHITE);
	draw_rectangle_16x16_vlines(_size_x/2,16,COLOR_WHITE);
	draw_rectangle(_size_x/2-17,16,_size_x/2+16,32,COLOR_WHITE);

	draw_rectangle_16x16_funnypattern(_size_x/2-16,_size_y-16,COLOR_WHITE);
	draw_rectangle_16x16_funnypattern(_size_x/2,_size_y-16,COLOR_WHITE);
	draw_rectangle_16x16_vlines(_size_x/2-16,_size_y-32,COLOR_WHITE);
	draw_rectangle_16x16_hlines(_size_x/2,_size_y-32,COLOR_WHITE);
	draw_rectangle(_size_x/2-17,_size_y-33,_size_x/2+16,_size_y,COLOR_WHITE);

	//left and right rectangles
	draw_rectangle_16x16_funnypattern(0,_size_y/2-16,COLOR_WHITE);
	draw_rectangle_16x16_vlines(16,_size_y/2-16,COLOR_WHITE);
	draw_rectangle_16x16_hlines(32,_size_y/2-16,COLOR_WHITE);
	draw_rectangle_16x16_funnypattern(0,_size_y/2,COLOR_WHITE);
	draw_rectangle_16x16_hlines(16,_size_y/2,COLOR_WHITE);
	draw_rectangle_16x16_vlines(32,_size_y/2,COLOR_WHITE);
	draw_rectangle(0,_size_y/2-17,48,_size_y/2+16,COLOR_WHITE);
	draw_rectangle_16x16_hlines(_size_x-48,_size_y/2-16,COLOR_WHITE);
	draw_rectangle_16x16_vlines(_size_x-32,_size_y/2-16,COLOR_WHITE);
	draw_rectangle_16x16_funnypattern(_size_x-16,_size_y/2-16,COLOR_WHITE);
	draw_rectangle_16x16_vlines(_size_x-48,_size_y/2,COLOR_WHITE);
	draw_rectangle_16x16_hlines(_size_x-32,_size_y/2,COLOR_WHITE);
	draw_rectangle_16x16_funnypattern(_size_x-16,_size_y/2,COLOR_WHITE);
	draw_rectangle(_size_x-49,_size_y/2-17,_size_x-1,_size_y/2+16,COLOR_WHITE);

	//center cross lines
	draw_rectangle(16,_size_y/2-1,_size_x-17,_size_y/2,COLOR_WHITE);
	draw_rectangle(_size_x/2-1,16,_size_x/2,_size_y-17,COLOR_WHITE);

	//big red quad
	clear_rectangle(56,16,_size_x-57,_size_y-17);
	draw_rectangle(56,16,_size_x-57,_size_y-17,COLOR_RED);

	//center quads
	draw_rectangle_16x16(_size_x/2-16,_size_y/2-16,COLOR_WHITE);
	draw_rectangle_16x16(_size_x/2,_size_y/2-16,COLOR_WHITE);
	draw_rectangle_16x16(_size_x/2-16,_size_y/2,COLOR_WHITE);
	draw_rectangle_16x16(_size_x/2,_size_y/2,COLOR_WHITE);
	draw_rectangle(_size_x/2-17,_size_y/2-17,_size_x/2+16,_size_y/2+16,COLOR_WHITE);

	//center crosshair
	clear_filled(_size_x/2-7,_size_y/2-7, _size_x/2+6, _size_y/2+6);
	draw_rectangle(_size_x/2-1,_size_y/2-1,_size_x/2,_size_y/2,COLOR_WHITE);
	draw_rectangle(_size_x/2-5,_size_y/2-1,_size_x/2-4,_size_y/2,COLOR_WHITE);
	draw_rectangle(_size_x/2+3,_size_y/2-1,_size_x/2+4,_size_y/2,COLOR_WHITE);
	draw_rectangle(_size_x/2-1,_size_y/2-5,_size_x/2,_size_y/2-4,COLOR_WHITE);
	draw_rectangle(_size_x/2-1,_size_y/2+3,_size_x/2,_size_y/2+4,COLOR_WHITE);
	draw_point(_size_x/2-4,_size_y/2-4,COLOR_WHITE);
	draw_point(_size_x/2+3,_size_y/2-4,COLOR_WHITE);
	draw_point(_size_x/2-4,_size_y/2+3,COLOR_WHITE);
	draw_point(_size_x/2+3,_size_y/2+3,COLOR_WHITE);
	draw_rectangle(_size_x/2-11,_size_y/2-4,_size_x/2-6,_size_y/2-4,COLOR_WHITE);
	draw_rectangle(_size_x/2-11,_size_y/2+3,_size_x/2-6,_size_y/2+3,COLOR_WHITE);
	draw_rectangle(_size_x/2+5,_size_y/2-4,_size_x/2+10,_size_y/2-4,COLOR_WHITE);
	draw_rectangle(_size_x/2+5,_size_y/2+3,_size_x/2+10,_size_y/2+3,COLOR_WHITE);
	draw_rectangle(_size_x/2-4,_size_y/2-11,_size_x/2-4,_size_y/2-6,COLOR_WHITE);
	draw_rectangle(_size_x/2+3,_size_y/2-11,_size_x/2+3,_size_y/2-6,COLOR_WHITE);
	draw_rectangle(_size_x/2-4,_size_y/2+5,_size_x/2-4,_size_y/2+10,COLOR_WHITE);
	draw_rectangle(_size_x/2+3,_size_y/2+5,_size_x/2+3,_size_y/2+10,COLOR_WHITE);

	//center-around quads
	draw_rectangle_set_17x16x4(_size_x/2-18,_size_y/2-48,COLOR_WHITE);
	draw_rectangle_set_17x16x4(_size_x/2-18,_size_y/2+16,COLOR_WHITE);
	draw_rectangle_set_17x16x4(_size_x/2-52,_size_y/2-16,COLOR_WHITE);
	draw_rectangle_set_17x16x4(_size_x/2+17,_size_y/2-16,COLOR_WHITE);

	//small red quad
	clear_rectangle(_size_x/2-52,_size_y/2-48,_size_x/2+51,_size_y/2+48);
	draw_rectangle(_size_x/2-52,_size_y/2-48,_size_x/2+51,_size_y/2+48,COLOR_RED);

	//close to corners quads
	draw_rectangle_set_17x16x4(35,19,COLOR_WHITE);
	draw_rectangle_set_17x16x4(_size_x-70,19,COLOR_WHITE);
	draw_rectangle_set_17x16x4(35,_size_y-52,COLOR_WHITE);
	draw_rectangle_set_17x16x4(_size_x-70,_size_y-52,COLOR_WHITE);

	_svin_set_cycle_patterns_nbg();
}

void pattern_monoscope(_svin_screen_mode_t screenmode)
{
	_svin_screen_mode_t curr_screenmode = screenmode;
	bool bIRE100 = true;
	draw_monoscope(curr_screenmode,bIRE100);
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
			draw_monoscope(curr_screenmode,bIRE100);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.r) )
		{
			curr_screenmode = next_screen_mode(curr_screenmode);
			update_screen_mode(curr_screenmode);
			draw_monoscope(curr_screenmode,bIRE100);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.a) || (controller.pressed.button.c) )
		{
			//change the checkered mode
			bIRE100 = bIRE100 ? false : true;
			draw_monoscope(curr_screenmode,bIRE100);
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
