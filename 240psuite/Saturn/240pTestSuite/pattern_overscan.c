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

void draw_overscan_constant_text()
{
	DrawString("Top Overscan :", 40, 100, FONT_WHITE);
	DrawString("Bottom Overscan :", 40, 110, FONT_WHITE);
	DrawString("Left Overscan :", 40, 120, FONT_WHITE);
	DrawString("Right Overscan :", 40, 130, FONT_WHITE);
}

void redraw_overscan_text(int _size_x, int _size_y, int top, int bottom, int left, int right, int cursor)
{
	char buf[64];
	sprintf(buf,"%d pixels (%3d.%1d",top,(100*top)/_size_y,((1000*top)/_size_y)%10);
	strcat(buf,"\%)");
	DrawString(buf, 160, 100, FONT_WHITE);
	sprintf(buf,"%d pixels (%3d.%1d",bottom,(100*bottom)/_size_y,((1000*bottom)/_size_y)%10);
	strcat(buf,"\%)");
	DrawString(buf, 160, 110, FONT_WHITE);
	sprintf(buf,"%d pixels (%3d.%1d",left,(100*left)/_size_x,((1000*left)/_size_x)%10);
	strcat(buf,"\%)");
	DrawString(buf, 160, 120, FONT_WHITE);
	sprintf(buf,"%d pixels (%3d.%1d",right,(100*right)/_size_x,((1000*right)/_size_x)%10);
	strcat(buf,"\%)");
	DrawString(buf, 160, 130, FONT_WHITE);
	strcpy(buf,">");
	DrawString(buf, 30, 100+10*cursor, FONT_WHITE);
}

void redraw_overscan(video_screen_mode_t screenmode, int top, int bottom, int left, int right, int cursor)
{
	int _size_x = get_screenmode_resolution_x(screenmode);
	int _size_y = get_screenmode_resolution_y(screenmode);

	video_vdp2_set_cycle_patterns_cpu();
	//add colors to palette
	int copies = 1;
	uint8_t *_pointer8[2];
	_pointer8[0] = (uint8_t *)VIDEO_VDP2_NBG0_CHPNDR_START;
	if (is_screenmode_special(screenmode))
	{
		copies = 2;
		_pointer8[0] = (uint8_t *)VIDEO_VDP2_NBG0_SPECIAL_BMP_START;
		_pointer8[1] = (uint8_t *)VIDEO_VDP2_NBG1_SPECIAL_BMP_START;
	}

	//fill center with gray
	for (int copy = 0; copy < copies; copy++)
		for (int y=top;y<_size_y-bottom;y++)
			memset(&(_pointer8[copy][y*512+left/2]), 0x22, (_size_x-right-left)/2);


	//draw top
	for (int copy = 0; copy < copies; copy++)
		for (int y=0;y<top;y++) 
			memset(&(_pointer8[copy][y*512]), 0x11, 512);

	//draw bot
	for (int y=_size_y-bottom;y<_size_y;y++) memset(&(_pointer8[y*512]), 0x11, 512);

	//draw left
	for (int copy = 0; copy < copies; copy++)
	{
		for (int y=0;y<_size_y;y++)
			memset(&(_pointer8[copy][y*512]), 0x11, left/2);
		if (left%2 == 1)
			for (int y=0;y<_size_y;y++)
			{
				_pointer8[copy][y*512+left/2] &= 0x0F;
				_pointer8[copy][y*512+left/2] |= 0x10;
			}
	}

	//draw right
	for (int copy = 0; copy < copies; copy++)
	{
		for (int y=0;y<_size_y;y++)
			memset(&(_pointer8[y*512+_size_x/2-right/2]), 0x11, right/2);
		if (right%2 == 1)
			for (int y=0;y<_size_y;y++)
			{
				_pointer8[copy][y*512+_size_x/2-right/2-1] &= 0xF0;
				_pointer8[copy][y*512+_size_x/2-right/2-1] |= 0x01;
			}
	}

	video_vdp2_set_cycle_patterns_nbg_bmp(screenmode);

	ClearText(160,100,160,40);	
	ClearText(30,100,10,50);	
	redraw_overscan_text(_size_x,_size_y, top, bottom, left, right, cursor);
}

void draw_overscan(video_screen_mode_t screenmode, int top, int bottom, int left, int right, int cursor)
{
	//removing text
	ClearTextLayer();
	
	video_vdp2_set_cycle_patterns_cpu();
	//add colors to palette
	uint8_t IRE_top = Get_IRE_Level(100.0);
	uint8_t IRE_bot = Get_IRE_Level(7.5);
	rgb888_t Color = {.cc=0,.r=0,.g=0,.b=0};
	Color.r = IRE_top;
	Color.g = IRE_top;
	Color.b = IRE_top;	
	video_vdp2_set_palette_part(2,&Color,1,1); //palette 2 color 1 = IRE white
	Color.r = IRE_bot + (IRE_top-IRE_bot)/2;
	Color.g = IRE_bot + (IRE_top-IRE_bot)/2;
	Color.b = IRE_bot + (IRE_top-IRE_bot)/2;
	video_vdp2_set_palette_part(2,&Color,2,2); //palette 2 color 2 = IRE gray 50%

	draw_overscan_constant_text();
	redraw_overscan(screenmode, top, bottom, left, right, cursor);

	video_vdp2_set_cycle_patterns_nbg_bmp(screenmode);
}

void pattern_overscan(video_screen_mode_t screenmode)
{
	video_screen_mode_t curr_screenmode = screenmode;
	int _left = 0;
	int _right = 0;
	int _top = 0;
	int _bottom = 0;
	int _cursor = 0;
	update_screen_mode(curr_screenmode,true); //re-initing in bmp mode
	draw_overscan(curr_screenmode,_top,_bottom,_left,_right,_cursor);

	wait_for_key_unpress();
	
	int mode_display_counter = 0;

	while (1)
	{
		smpc_peripheral_process();
		get_digital_keypress_anywhere(&controller);
		vdp2_tvmd_vblank_in_wait();
		vdp2_tvmd_vblank_out_wait();
		if ( (controller.pressed.button.l) )
		{
			curr_screenmode = prev_screen_mode(curr_screenmode);
			update_screen_mode(curr_screenmode,true);
			update_screen_mode(curr_screenmode,true);
			draw_overscan(curr_screenmode,_top,_bottom,_left,_right,_cursor);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.r) )
		{
			curr_screenmode = next_screen_mode(curr_screenmode);
			update_screen_mode(curr_screenmode,true);
			update_screen_mode(curr_screenmode,true);
			draw_overscan(curr_screenmode,_top,_bottom,_left,_right,_cursor);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.up) )
		{
			_cursor--;
			if (_cursor < 0) _cursor = 3; 
			redraw_overscan(curr_screenmode,_top,_bottom,_left,_right,_cursor);
			wait_for_key_unpress();
		}
		else if ( (controller.pressed.button.down) )
		{
			_cursor++;
			if (_cursor > 3) _cursor = 0; 
			redraw_overscan(curr_screenmode,_top,_bottom,_left,_right,_cursor);
			wait_for_key_unpress();
		}
		else if ( (controller.pressed.button.left) )
		{
			switch(_cursor)
			{
				case 0:
					_top--;
					if (_top < 0) _top = 0;
					break;
				case 1:
					_bottom--;
					if (_bottom < 0) _bottom = 0;
					break;
				case 2:
					_left--;
					if (_left < 0) _left = 0;
					break;
				case 3:
					_right--;
					if (_right < 0) _right = 0;
					break;
			}
			redraw_overscan(curr_screenmode,_top,_bottom,_left,_right,_cursor);
			wait_for_key_unpress();
		}
		else if ( (controller.pressed.button.right) )
		{
			switch(_cursor)
			{
				case 0:
					_top++;
					if (_top > 255) _top = 255;
					break;
				case 1:
					_bottom++;
					if (_bottom > 255) _bottom = 255;
					break;
				case 2:
					_left++;
					if (_left > 352) _left = 352;
					break;
				case 3:
					_right++;
					if (_right > 352) _right = 352;
					break;
			}
			redraw_overscan(curr_screenmode,_top,_bottom,_left,_right,_cursor);
			wait_for_key_unpress();
		}
		else if ( (controller.pressed.button.a) || (controller.pressed.button.c) )
		{
			//reset overscan
			_left = 0;
			_right = 0;
			_top = 0;
			_bottom = 0;
			redraw_overscan(curr_screenmode,_top,_bottom,_left,_right,_cursor);
			wait_for_key_unpress();
		}
		else if (controller.pressed.button.b)
		{
			//quit the pattern
			wait_for_key_unpress();
			update_screen_mode(screenmode,false);
			return;
		}
		if (mode_display_counter > 0)
		{
			mode_display_counter--;
			if (0 == mode_display_counter)
			{
				ClearTextLayer();
				draw_overscan(curr_screenmode,_top,_bottom,_left,_right,_cursor);
			}
		}
	}
}
