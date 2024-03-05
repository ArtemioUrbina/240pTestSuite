#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "svin.h"
#include "video.h"
#include "control.h"
#include "ire.h"

#define COLOR_WHITE 1
#define COLOR_RED 2
#define COLOR_BLACK 3
#define COLOR_LIGHTBLUE 4
#define COLOR_GREEN 5

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

void draw_dotted_line(int x1, int x2, int y, int color)
{
	uint8_t *_pointer8 = (uint8_t *)_SVIN_NBG0_CHPNDR_START;
	int _x = x1;
	while (_x<((x2/2)*2))
	{
		_pointer8[y*512+_x/2] &= 0xF0;
		_pointer8[y*512+_x/2] |= 0x10*color;
		_x+=2;
	}
	if (_x<x2)
	{
		_pointer8[y*512+_x/2] &= 0xF0;
		_pointer8[y*512+_x/2] |= 0x10*color;
		_pointer8[y*512+_x/2] |= 0x10*color;
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


void draw_cell_empty(int x, int y, int cell_x, int cell_y, int color)
{
	draw_rectangle(x,y,x+cell_x-1,y+cell_y-1,color);
}

void draw_cell_hlines(int x, int y, int cell_x, int cell_y, int color)
{
	clear_filled(x,y,x+cell_x-1,y+cell_y-1);
	draw_cell_empty(x,y,cell_x,cell_y,color);
	int _y;
	for (_y=3;_y<(cell_y-3);_y+=3)
		draw_rectangle(x+3,y+_y,x+cell_x-4,y+_y,color);
}

void draw_cell_hlines_small(int x, int y, int cell_x, int cell_y, int color)
{
	clear_filled(x,y,x+cell_x-1,y+cell_y-1);
	draw_cell_empty(x,y,cell_x,cell_y,color);
	int _y;
	for (_y=2;_y<(cell_y-2);_y+=3)
		draw_rectangle(x+3,y+_y,x+cell_x-4,y+_y,color);
}


void draw_cell_vlines(int x, int y, int cell_x, int cell_y, int color)
{
	clear_filled(x,y,x+cell_x-1,y+cell_y-1);
	draw_cell_empty(x,y,cell_x,cell_y,color);
	int _x;
	for (_x=3;_x<(cell_x-3);_x+=3)
		draw_rectangle(x+_x,y+3,x+_x,y+cell_y-4,color);
}

void draw_cell_funnypattern(int x, int y, int cell_x, int cell_y, int color)
{
	clear_filled(x,y,x+cell_x-1,y+cell_y-1);
	draw_cell_empty(x,y,cell_x,cell_y,color);
	x = (cell_x==16) ? x : x+8;
	y = (cell_y==16) ? y : y+8;
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


void draw_rectangle_set(int x, int y, int cell_x, int cell_y, int color)
{
	clear_filled(x,y,x+cell_x*2+1,y+cell_y*2+1);
	draw_rectangle(x+1,y+1,x+cell_x,y+cell_y,color);
	draw_rectangle(x+cell_x+1,y+1,x+cell_x*2,y+cell_y,color);
	draw_rectangle(x+1,y+cell_y+1,x+cell_x,y+cell_y*2,color);
	draw_rectangle(x+cell_x+1,y+cell_y+1,x+cell_x*2,y+cell_y*2,color);
	draw_rectangle(x,y,x+cell_x*2+1,y+cell_y*2+1,color);
	//crosshair
	draw_rectangle(x+cell_x/2-cell_x/4,y+cell_y+cell_y/2+1,x+cell_x/2+cell_x/4+2,y+cell_y+cell_y/2+1,color);
	draw_rectangle(x+cell_x/2+1,y+cell_y+cell_y/2-cell_y/4,x+cell_x/2+1,y+cell_y+cell_y/2+cell_y/4+2,color);
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
	_svin_set_palette_part(2,&Color,COLOR_WHITE,COLOR_WHITE); //palette 2 color 1 = IRE white
	Color.g = IRE_bot;
	Color.b = IRE_bot;	
	_svin_set_palette_part(2,&Color,COLOR_RED,COLOR_RED); //palette 2 color 2 = IRE red
	Color.r = IRE_bot;
	_svin_set_palette_part(2,&Color,0,0); //palette 2 color 0 = IRE black
	_svin_set_palette_part(2,&Color,COLOR_BLACK,COLOR_BLACK); //palette 2 color 3 = IRE black
	Color.r = (IRE_top+IRE_bot)/2;
	Color.g = (IRE_top+IRE_bot)/2;
	Color.b = IRE_top;	
	_svin_set_palette_part(2,&Color,COLOR_LIGHTBLUE,COLOR_LIGHTBLUE); //palette 2 color 4 = IRE lightblue
	Color.r = IRE_bot;
	Color.g = IRE_top;
	Color.b = IRE_bot;	
	_svin_set_palette_part(2,&Color,COLOR_GREEN,COLOR_GREEN); //palette 2 color 4 = IRE green

	//switching to BMP mode
	struct vdp2_scrn_bitmap_format format;

	if (0 == current_color_mode)
	{
		//480p high res, special setup
		//setup nbg0
		memset(&format, 0x00, sizeof(format));
		format.scroll_screen = VDP2_SCRN_NBG0;
		format.ccc = VDP2_SCRN_CCC_PALETTE_16;
		format.bitmap_size = VDP2_SCRN_BITMAP_SIZE_1024X512;
		format.palette_base = 0x400;
		format.bitmap_base = _SVIN_NBG0_CHPNDR_START;
		vdp2_scrn_bitmap_format_set(&format);
		//setup nbg1
		memset(&format, 0x00, sizeof(format));
		format.scroll_screen = VDP2_SCRN_NBG1;
		format.ccc = VDP2_SCRN_CCC_PALETTE_16;
		format.bitmap_size = VDP2_SCRN_BITMAP_SIZE_1024X512;
		format.palette_base = 0x400;
		format.bitmap_base = _SVIN_NBG0_CHPNDR_START;
		vdp2_scrn_bitmap_format_set(&format);

		vdp2_scrn_reduction_set(VDP2_SCRN_NBG0,VDP2_SCRN_REDUCTION_HALF);
    	vdp2_scrn_reduction_set(VDP2_SCRN_NBG1,VDP2_SCRN_REDUCTION_HALF);
    	vdp2_scrn_reduction_x_set(VDP2_SCRN_NBG0, FIX16(2.0f));
	    vdp2_scrn_reduction_x_set(VDP2_SCRN_NBG1, FIX16(2.0f));
    	vdp2_scrn_reduction_y_set(VDP2_SCRN_NBG0, FIX16(1.0f));
	    vdp2_scrn_reduction_y_set(VDP2_SCRN_NBG1, FIX16(1.0f));
		vdp2_scrn_scroll_x_set(VDP2_SCRN_NBG0 , FIX16(0.0f));
		vdp2_scrn_scroll_x_set(VDP2_SCRN_NBG1 , FIX16(1.0f));	
		vdp2_scrn_priority_set(VDP2_SCRN_NBG0, 5);
		vdp2_scrn_priority_set(VDP2_SCRN_NBG1, 5);
	}
	else
	{
		//normal setup
		//setup nbg0
		memset(&format, 0x00, sizeof(format));
		format.scroll_screen = VDP2_SCRN_NBG0;
		format.ccc = VDP2_SCRN_CCC_PALETTE_16;
		format.bitmap_size = VDP2_SCRN_BITMAP_SIZE_1024X512;
		format.palette_base = 0x800;
		format.bitmap_base = _SVIN_NBG0_CHPNDR_START;
		vdp2_scrn_bitmap_format_set(&format);
		//setup nbg1
		memset(&format, 0x00, sizeof(format));
		format.scroll_screen = VDP2_SCRN_NBG1;
		format.ccc = VDP2_SCRN_CCC_PALETTE_16;
		format.bitmap_size = VDP2_SCRN_BITMAP_SIZE_1024X512;
		format.palette_base = 0x800;
		format.bitmap_base = _SVIN_NBG1_CHPNDR_START;
		vdp2_scrn_bitmap_format_set(&format);

		vdp2_scrn_reduction_set(VDP2_SCRN_NBG0,VDP2_SCRN_REDUCTION_NONE);
    	vdp2_scrn_reduction_set(VDP2_SCRN_NBG1,VDP2_SCRN_REDUCTION_NONE);
    	vdp2_scrn_reduction_x_set(VDP2_SCRN_NBG0, FIX16(1.0f));
	    vdp2_scrn_reduction_x_set(VDP2_SCRN_NBG1, FIX16(1.0f));
    	vdp2_scrn_reduction_y_set(VDP2_SCRN_NBG0, FIX16(1.0f));
	    vdp2_scrn_reduction_y_set(VDP2_SCRN_NBG1, FIX16(1.0f));
		vdp2_scrn_scroll_x_set(VDP2_SCRN_NBG0 , FIX16(0.0f));
		vdp2_scrn_scroll_x_set(VDP2_SCRN_NBG1 , FIX16(0.0f));	
		vdp2_scrn_priority_set(VDP2_SCRN_NBG0, 3);
		vdp2_scrn_priority_set(VDP2_SCRN_NBG1, 0);
	}

	int _size_x = get_screenmode_resolution_x(screenmode);
	int _size_y = get_screenmode_resolution_y(screenmode);

	uint8_t *_pointer8 = (uint8_t *)_SVIN_NBG0_CHPNDR_START;

	//fill everything with black
	memset(_pointer8,0x00,512*_size_y);

	int cell_x=16;
	int cell_y=16;
	if (screenmode.x_res_doubled)
		cell_x=32;
	if ( (_SVIN_SCANMODE_480I == screenmode.scanmode) || (_SVIN_SCANMODE_480P == screenmode.scanmode) )
		cell_y=32;

	int offset_y = (VDP2_TVMD_VERT_240 == screenmode.y_res) ? cell_y/2 : 0;

	//draw borders rectangles
	if (offset_y)
	{
		//for 240 and 480 resolution, we add half-quads at the top and bottom
		for (int x=0;x<_size_x;x+=cell_x) draw_cell_hlines_small(x,0,cell_x,offset_y,COLOR_WHITE);
		for (int x=0;x<_size_x;x+=cell_x) draw_cell_hlines_small(x,_size_y-offset_y,cell_x,offset_y,COLOR_WHITE);
	}
	for (int x=0;x<_size_x;x+=cell_x) draw_cell_empty(x,offset_y,cell_x,cell_y,COLOR_WHITE);
	for (int x=0;x<_size_x;x+=cell_x) draw_cell_empty(x,_size_y-cell_y-offset_y,cell_x,cell_y,COLOR_WHITE);
	for (int y=offset_y;y<_size_y;y+=cell_y) draw_cell_empty(0,y,cell_x,cell_y,COLOR_WHITE);
	for (int y=offset_y;y<_size_y;y+=cell_y) draw_cell_empty(_size_x-cell_x,y,cell_x,cell_y,COLOR_WHITE);
	//internal border for border quads
	draw_rectangle(cell_x,cell_y+offset_y,_size_x-cell_x-1,_size_y-cell_y-1-offset_y,COLOR_WHITE);

	//draw dots
	for (int y=offset_y;y<_size_y;y+=cell_y) 
		for (int x=0;x<_size_x;x+=cell_x) 
			draw_rectangle(x+cell_x/2-1,y+cell_y/2-1,x+cell_x/2,y+cell_y/2,COLOR_WHITE);

	//draw corner rectangles - top left
	draw_cell_funnypattern(0,offset_y,cell_x,cell_y,COLOR_WHITE);
	draw_cell_hlines(cell_x,offset_y,cell_x,cell_y,COLOR_WHITE);
	draw_cell_vlines(0,cell_y+offset_y,cell_x,cell_y,COLOR_WHITE);
	draw_cell_funnypattern(cell_x,cell_y+offset_y,cell_x,cell_y,COLOR_WHITE);
	draw_rectangle(cell_x,cell_y+offset_y,cell_x*2,cell_y*2+offset_y,COLOR_WHITE);

	//draw corner rectangles - bottom left
	draw_cell_hlines(0,_size_y-cell_y*2-offset_y,cell_x,cell_y,COLOR_WHITE);
	draw_cell_funnypattern(cell_x,_size_y-cell_y*2-offset_y,cell_x,cell_y,COLOR_WHITE);
	draw_cell_funnypattern(0,_size_y-cell_y-offset_y,cell_x,cell_y,COLOR_WHITE);
	draw_cell_vlines(cell_x,_size_y-cell_y-offset_y,cell_x,cell_y,COLOR_WHITE);
	draw_rectangle(cell_x,_size_y-cell_y*2-1-offset_y,cell_x*2,_size_y-cell_y-offset_y,COLOR_WHITE);

	//draw corner rectangles - top right
	draw_cell_vlines(_size_x-cell_x*2,offset_y,cell_x,cell_y,COLOR_WHITE);
	draw_cell_funnypattern(_size_x-cell_x,offset_y,cell_x,cell_y,COLOR_WHITE);
	draw_cell_funnypattern(_size_x-cell_x*2,cell_y+offset_y,cell_x,cell_y,COLOR_WHITE);
	draw_cell_hlines(_size_x-cell_x,cell_y+offset_y,cell_x,cell_y,COLOR_WHITE);
	draw_rectangle(_size_x-cell_x*2-1,cell_y+offset_y,_size_x-cell_x,cell_y*2+offset_y,COLOR_WHITE);

	//draw corner rectangles - bottom right
	draw_cell_funnypattern(_size_x-cell_x*2,_size_y-cell_y*2-offset_y,cell_x,cell_y,COLOR_WHITE);
	draw_cell_vlines(_size_x-cell_x,_size_y-cell_y*2-offset_y,cell_x,cell_y,COLOR_WHITE);
	draw_cell_hlines(_size_x-cell_x*2,_size_y-cell_y-offset_y,cell_x,cell_y,COLOR_WHITE);
	draw_cell_funnypattern(_size_x-cell_x,_size_y-cell_y-offset_y,cell_x,cell_y,COLOR_WHITE);
	draw_rectangle(_size_x-cell_x*2-1,_size_y-cell_y*2-1-offset_y,_size_x-cell_x,_size_y-cell_y-offset_y,COLOR_WHITE);

	//draw top and bottom rectangles
	draw_cell_funnypattern(_size_x/2-cell_x,offset_y,cell_x,cell_y,COLOR_WHITE);
	draw_cell_funnypattern(_size_x/2,offset_y,cell_x,cell_y,COLOR_WHITE);
	//draw_cell_hlines(_size_x/2-cell_x,cell_y+offset_y,cell_x,cell_y,COLOR_WHITE);
	//draw_cell_vlines(_size_x/2,cell_y+offset_y,cell_x,cell_y,COLOR_WHITE);
	//draw_rectangle(_size_x/2-cell_x-1,cell_y+offset_y,_size_x/2+cell_x,cell_y*2+offset_y,COLOR_WHITE);

	draw_cell_funnypattern(_size_x/2-cell_x,_size_y-cell_y-offset_y,cell_x,cell_y,COLOR_WHITE);
	draw_cell_funnypattern(_size_x/2,_size_y-cell_y-offset_y,cell_x,cell_y,COLOR_WHITE);
	//draw_cell_vlines(_size_x/2-cell_x,_size_y-cell_y*2-offset_y,cell_x,cell_y,COLOR_WHITE);
	//draw_cell_hlines(_size_x/2,_size_y-cell_y*2-offset_y,cell_x,cell_y,COLOR_WHITE);
	//draw_rectangle(_size_x/2-cell_x-1,_size_y-cell_y*2-1-offset_y,_size_x/2+cell_x,_size_y-offset_y,COLOR_WHITE);

	//left and right rectangles
	draw_cell_funnypattern(0,_size_y/2-cell_y,cell_x,cell_y,COLOR_WHITE);
	draw_cell_vlines(cell_x,_size_y/2-cell_y,cell_x,cell_y,COLOR_WHITE);
	draw_cell_hlines(cell_x*2,_size_y/2-cell_y,cell_x,cell_y,COLOR_WHITE);
	draw_cell_funnypattern(0,_size_y/2,cell_x,cell_y,COLOR_WHITE);
	draw_cell_hlines(cell_x,_size_y/2,cell_x,cell_y,COLOR_WHITE);
	draw_cell_vlines(cell_x*2,_size_y/2,cell_x,cell_y,COLOR_WHITE);
	draw_rectangle(0,_size_y/2-cell_y-1,cell_x*3,_size_y/2+cell_y,COLOR_WHITE);
	draw_cell_hlines(_size_x-cell_x*3,_size_y/2-cell_y,cell_x,cell_y,COLOR_WHITE);
	draw_cell_vlines(_size_x-cell_x*2,_size_y/2-cell_y,cell_x,cell_y,COLOR_WHITE);
	draw_cell_funnypattern(_size_x-cell_x,_size_y/2-cell_y,cell_x,cell_y,COLOR_WHITE);
	draw_cell_vlines(_size_x-cell_x*3,_size_y/2,cell_x,cell_y,COLOR_WHITE);
	draw_cell_hlines(_size_x-cell_x*2,_size_y/2,cell_x,cell_y,COLOR_WHITE);
	draw_cell_funnypattern(_size_x-cell_x,_size_y/2,cell_x,cell_y,COLOR_WHITE);
	draw_rectangle(_size_x-cell_x*3-1,_size_y/2-cell_y-1,_size_x-1,_size_y/2+cell_y,COLOR_WHITE);

	//center cross lines
	draw_rectangle(cell_x,_size_y/2-1,_size_x-cell_x-1,_size_y/2,COLOR_WHITE);
	draw_rectangle(_size_x/2-1,cell_y,_size_x/2,_size_y-cell_y-1,COLOR_WHITE);

	//calculating square quad x size
	double dcell_y = cell_y;
	double ratio = get_screen_square_pixel_ratio(screenmode);
	int cell_x_fixed = ((dcell_y*ratio)+0.5);
	if ((cell_x_fixed) > 100)
		cell_x_fixed = 100;
	if ((cell_x_fixed) < 10)
		cell_x_fixed = 10;

	//close to corners quads
	draw_rectangle_set(cell_x*2+3,cell_y+3+offset_y,cell_x_fixed,cell_y,COLOR_WHITE);
	draw_rectangle_set(_size_x-cell_x*2-cell_x_fixed*2-4,cell_y+3+offset_y,cell_x_fixed,cell_y,COLOR_WHITE);
	draw_rectangle_set(cell_x*2+3,_size_y-cell_y*3-5-offset_y,cell_x_fixed,cell_y,COLOR_WHITE);
	draw_rectangle_set(_size_x-cell_x*2-cell_x_fixed*2-4,_size_y-cell_y*3-5-offset_y,cell_x_fixed,cell_y,COLOR_WHITE);

	int rect_y;
	int rect_x;
	//big red quad
	if (_SVIN_X_RESOLUTION_320 == screenmode.x_res)
	{
		rect_y = ( (_SVIN_SCANMODE_480I == screenmode.scanmode) /*|| (_SVIN_SCANMODE_480P == screenmode.scanmode)*/ ) ? 384 : 192;
		rect_x = ((rect_y*35)/32);
		if (ratio>1.5)
			rect_x= ((rect_y*70)/32);
		if (ratio<0.75)
			rect_x= ((rect_y*35)/64);
		clear_rectangle(_size_x/2-rect_x/2+1,_size_y/2-rect_y/2+1,_size_x/2+rect_x/2,_size_y/2+rect_y/2);
		draw_rectangle(_size_x/2-rect_x/2+1,_size_y/2-rect_y/2+1,_size_x/2+rect_x/2,_size_y/2+rect_y/2,COLOR_RED);
	}

	//big blue quad
	if (_SVIN_X_RESOLUTION_352 == screenmode.x_res)
	{
		rect_y = ( (_SVIN_SCANMODE_480I == screenmode.scanmode) /*|| (_SVIN_SCANMODE_480P == screenmode.scanmode)*/ ) ? 336 : 168;
		rect_x = ((rect_y*7)/6);
		if (ratio>1.5)
			rect_x= ((rect_y*14)/6);
		if (ratio<0.75)
			rect_x= ((rect_y*7)/12);
		clear_rectangle(_size_x/2-rect_x/2+1,_size_y/2-rect_y/2+1,_size_x/2+rect_x/2,_size_y/2+rect_y/2);
		draw_rectangle(_size_x/2-rect_x/2+1,_size_y/2-rect_y/2+1,_size_x/2+rect_x/2,_size_y/2+rect_y/2,COLOR_LIGHTBLUE);
	}

	//center quads
	/*draw_cell_empty(_size_x/2-cell_x_fixed,_size_y/2-cell_y,cell_x_fixed,cell_y,COLOR_WHITE);
	draw_cell_empty(_size_x/2,_size_y/2-cell_y,cell_x_fixed,cell_y,COLOR_WHITE);
	draw_cell_empty(_size_x/2-cell_x_fixed,_size_y/2,cell_x_fixed,cell_y,COLOR_WHITE);
	draw_cell_empty(_size_x/2,_size_y/2,cell_x_fixed,cell_y,COLOR_WHITE);
	draw_rectangle(_size_x/2-cell_x_fixed-1,_size_y/2-cell_y-1,_size_x/2+cell_x_fixed,_size_y/2+cell_y,COLOR_WHITE);*/

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
	//draw_rectangle_set(_size_x/2-cell_x_fixed-1,_size_y/2-cell_y*3-1,cell_x_fixed,cell_y,COLOR_WHITE);
	//draw_rectangle_set(_size_x/2-cell_x_fixed-1,_size_y/2+cell_y,cell_x_fixed,cell_y,COLOR_WHITE);
	//draw_rectangle_set(_size_x/2-cell_x_fixed*3-1,_size_y/2-cell_y-1,cell_x_fixed,cell_y,COLOR_WHITE);
	//draw_rectangle_set(_size_x/2+cell_x_fixed,_size_y/2-cell_y-1,cell_x_fixed,cell_y,COLOR_WHITE);

	//dotted lines
	if (_SVIN_X_RESOLUTION_320 == screenmode.x_res)
	{
		rect_y = ( (_SVIN_SCANMODE_480I == screenmode.scanmode) || (_SVIN_SCANMODE_480P == screenmode.scanmode) ) ? 384 : 192;
		rect_x = ((rect_y*35)/32);
	}
	else
	{
		rect_y = ( (_SVIN_SCANMODE_480I == screenmode.scanmode) || (_SVIN_SCANMODE_480P == screenmode.scanmode) ) ? 336 : 168;
		rect_x = ((rect_y*7)/6);
	}
	if (ratio>1.5)
		rect_x= ((rect_y*70)/32);
	if (ratio<0.75)
		rect_x= ((rect_y*35)/64);
	draw_dotted_line(_size_x/2-rect_x/2+2,_size_x/2-12,_size_y/2-4,COLOR_WHITE);
	draw_dotted_line(_size_x/2-rect_x/2+2,_size_x/2-12,_size_y/2+3,COLOR_WHITE);
	draw_dotted_line(_size_x/2+11,_size_x/2+rect_x/2+2,_size_y/2-4,COLOR_WHITE);
	draw_dotted_line(_size_x/2+11,_size_x/2+rect_x/2+2,_size_y/2+3,COLOR_WHITE);

	//small red quad
	/*red_quad_y = (red_quad_y*2.0)/3.0;
	red_quad_x = ((red_quad_y*ratio)+0.5);
	if ((red_quad_x) > 500)
		red_quad_x = 500;
	if ((red_quad_x) < 10)
		red_quad_x = 10;
	clear_rectangle(_size_x/2-red_quad_x/2-1,_size_y/2-red_quad_y/2-1,_size_x/2+red_quad_x/2,_size_y/2+red_quad_y/2);
	draw_rectangle(_size_x/2-red_quad_x/2-1,_size_y/2-red_quad_y/2-1,_size_x/2+red_quad_x/2,_size_y/2+red_quad_y/2,COLOR_RED);*/

	_svin_set_cycle_patterns_nbg();
}

void draw_monoscope_text(_svin_screen_mode_t screenmode)
{
	int _size_x = get_screenmode_resolution_x(screenmode);
	int _size_y = get_screenmode_resolution_y(screenmode);
	double ratio = get_screen_square_pixel_ratio(screenmode);
	int cell_x=16;
	int cell_y=16;
	if (screenmode.x_res_doubled)
		cell_x=32;
	if ( (_SVIN_SCANMODE_480I == screenmode.scanmode) || (_SVIN_SCANMODE_480P == screenmode.scanmode) )
		cell_y=32;
	int offset_y = (VDP2_TVMD_VERT_240 == screenmode.y_res) ? cell_y/2 : 0;
	int text_shift_y = (VDP2_TVMD_VERT_240 == screenmode.y_res) ? -8 :
					(VDP2_TVMD_VERT_256 == screenmode.y_res) ? -16 : 0;

	//big red quad
	int rect_y = ( (_SVIN_SCANMODE_480I == screenmode.scanmode) || (_SVIN_SCANMODE_480P == screenmode.scanmode) ) ? 384 : 192;
	int rect_x = ((rect_y*35)/32);
	if (ratio>1.5)
		rect_x= ((rect_y*70)/32);
	if (ratio<0.75)
		rect_x= ((rect_y*35)/64);
	char buf[32];
	sprintf(buf," %ix%i",rect_x,rect_y);
	int _text_x = _size_x/2-rect_x/2+2;
	if (_size_x > 512) _text_x/=2;
	int _text_y = _size_y/2-rect_y/2+2;
	if (_size_y > 256) _text_y/=2;
	DrawString(buf, _text_x, _text_y+text_shift_y, FONT_RED);

	//big green quad
	rect_y = ( (_SVIN_SCANMODE_480I == screenmode.scanmode) || (_SVIN_SCANMODE_480P == screenmode.scanmode) ) ? 288 : 144;
	rect_x = (int)(ratio*rect_y+0.5);
	rect_x /= 2; rect_x *= 2;
	sprintf(buf," %ix%i",rect_x,rect_y);
	_text_x = _size_x/2-rect_x/2+2;
	if (_size_x > 512) _text_x/=2;
	_text_y = _size_y/2-rect_y/2+2;
	if (_size_y > 256) _text_y/=2;
	DrawString(buf, _text_x, _text_y+text_shift_y, FONT_GREEN);

	//big blue quad
	rect_y = ( (_SVIN_SCANMODE_480I == screenmode.scanmode) || (_SVIN_SCANMODE_480P == screenmode.scanmode) ) ? 336 : 168;
	rect_x = ((rect_y*7)/6);
	if (ratio>1.5)
		rect_x= ((rect_y*14)/6);
	if (ratio<0.75)
		rect_x= ((rect_y*7)/12);
	sprintf(buf," %ix%i",rect_x,rect_y);
	_text_x = _size_x/2-rect_x/2+2;
	if (_size_x > 512) _text_x/=2;
	_text_y = _size_y/2-rect_y/2+2;
	if (_size_y > 256) _text_y/=2;
	DrawString(buf, _text_x, _text_y+text_shift_y, FONT_CYAN);
}

void pattern_monoscope(_svin_screen_mode_t screenmode)
{
	_svin_screen_mode_t curr_screenmode = screenmode;
	bool bIRE100 = true;
	draw_monoscope(curr_screenmode,bIRE100);
	bool key_pressed = false;
	bool size_printed = false;

	wait_for_key_unpress();
	
	int mode_display_counter = 0;

	while (1)
	{
		smpc_peripheral_process();
		get_digital_keypress_anywhere(&controller);
		if ( (controller.pressed.button.l) )
		{
			curr_screenmode = prev_screen_mode(curr_screenmode);
			update_screen_mode(curr_screenmode);
			draw_monoscope(curr_screenmode,bIRE100);
			if (size_printed) draw_monoscope_text(curr_screenmode);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.r) )
		{
			curr_screenmode = next_screen_mode(curr_screenmode);
			update_screen_mode(curr_screenmode);
			draw_monoscope(curr_screenmode,bIRE100);
			if (size_printed) draw_monoscope_text(curr_screenmode);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.a) || (controller.pressed.button.c) )
		{
			//change the checkered mode
			bIRE100 = bIRE100 ? false : true;
			draw_monoscope(curr_screenmode,bIRE100);
			if (size_printed) draw_monoscope_text(curr_screenmode);
			wait_for_key_unpress();
		}
		else if (controller.pressed.button.b)
		{
			//quit the pattern
			wait_for_key_unpress();
			update_screen_mode(screenmode);
			return;
		}
		else if (controller.pressed.button.x)
		{
			size_printed = size_printed ? false : true;
			if (size_printed) 
				draw_monoscope_text(curr_screenmode);
			else 
				ClearTextLayer();
			wait_for_key_unpress();
		}
		vdp2_tvmd_vblank_in_wait();
		vdp2_tvmd_vblank_out_wait();
		if (mode_display_counter > 0)
		{
			mode_display_counter--;
			if (0 == mode_display_counter)
			{
				ClearTextLayer();
				if (size_printed) draw_monoscope_text(curr_screenmode);
			}
		}
	}
}
