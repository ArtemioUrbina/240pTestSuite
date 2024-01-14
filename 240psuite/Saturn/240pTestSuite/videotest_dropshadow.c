#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "svin.h"
#include "video.h"
#include "control.h"
#include "ire.h"

void draw_dropshadow(_svin_screen_mode_t screenmode)
{	
	_svin_set_cycle_patterns_cpu();
	draw_bg_donna(screenmode);
	//constructing pattern
	uint8_t pattern_color = FONT_WHITE*4;
	uint8_t pattern [32*32];
	memset(pattern,0,32*32);
	for (int y=5;y<28;y++)
		for (int x=5;x<28;x++)
			pattern[32*y+x] = pattern_color;
	for (int y=0;y<16;y++)
		for (int x=14-y;x<16;x++)
			if (x>=0)
				{
					pattern[32*y+x] = pattern_color;
					pattern[32*y+31-x] = pattern_color;
					pattern[32*(31-y)+x] = pattern_color;
					pattern[32*(31-y)+31-x] = pattern_color;
				}

	//copying pattern to NBG1
	uint8_t * _p8 = (uint8_t *)_SVIN_NBG1_CHPNDR_START;
	for (int y=0;y<4;y++)
		for (int x=0;x<4;x++)
			for (int _y=0;_y<8;_y++)
				memcpy(&(_p8[y*64*4+x*64+_y*8]),&(pattern[(y*8+_y)*32+x*8]),8);

	//generating names for pattern in NBG1
	int * _pointer32 = (int *)_SVIN_NBG1_PNDR_START;
	for (int y=0;y<4;y++)
		for (int x=0;x<4;x++)
    		_pointer32[y*64+x] = 0x00000000 + 0x60000/32 + y*4*2 + x*2; //palette 0

	_svin_set_cycle_patterns_nbg();
}

void videotest_dropshadow(_svin_screen_mode_t screenmode)
{
	//removing text
	ClearTextLayer();

	_svin_screen_mode_t curr_screenmode = screenmode;
	draw_dropshadow(curr_screenmode);
	bool key_pressed = false;
	int *_pointer32 = (int *)_SVIN_NBG0_CHPNDR_START;
	int pattern = 0;
	int _size_x = get_screenmode_resolution_x(screenmode);
	int _size_y = get_screenmode_resolution_y(screenmode);
	int x=_size_x/2;
	int y=_size_y/2;

	wait_for_key_unpress();
	
	int mode_display_counter = 0;
	bool bOddField = false;
	char buf[32];

	while (1)
	{
		smpc_peripheral_process();
		smpc_peripheral_digital_port(1, &controller);
		if ( (controller.pressed.button.up) )
		{
			y--;
			if (y<0) y=0;
		}
		if ( (controller.pressed.button.down) )
		{
			y++;
			if (y > _size_y-32) y=_size_y-32;
		}
		if ( (controller.pressed.button.left) )
		{
			x--;
			if (x<0) x=0;
		}
		if ( (controller.pressed.button.right) )
		{
			x++;
			if (x > _size_x-32) x=_size_x-32;
		}

		if ( (controller.pressed.button.l) )
		{
			curr_screenmode = prev_screen_mode(curr_screenmode);
			update_screen_mode(curr_screenmode);
			draw_dropshadow(curr_screenmode);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
			_size_x = get_screenmode_resolution_x(curr_screenmode);
			_size_y = get_screenmode_resolution_y(curr_screenmode);
			if (x > _size_x-32) x=_size_x-32;
			if (y > _size_y-32) y=_size_y-32;
		}
		else if ( (controller.pressed.button.r) )
		{
			curr_screenmode = next_screen_mode(curr_screenmode);
			update_screen_mode(curr_screenmode);
			draw_dropshadow(curr_screenmode);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
			_size_x = get_screenmode_resolution_x(curr_screenmode);
			_size_y = get_screenmode_resolution_y(curr_screenmode);
			if (x > _size_x-32) x=_size_x-32;
			if (y > _size_y-32) y=_size_y-32;
		}
		else if ( (controller.pressed.button.a) || (controller.pressed.button.c) )
		{
			//change the active field
			bOddField = (bOddField) ? false : true;
			if (bOddField)
				sprintf(buf, " Odd fields");
			else
				sprintf(buf, "Even fields");
			DrawStringWithBackground(buf, 160-strlen(buf)*_fw/2, 120, FONT_WHITE,FONT_YELLOW);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.b) )
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
		//disable pattern on every second frame
		if ( ( (VDP2_TVMD_TV_FIELD_SCAN_ODD == vdp2_tvmd_field_scan_get()) && (bOddField) ) ||
			 ( (VDP2_TVMD_TV_FIELD_SCAN_EVEN == vdp2_tvmd_field_scan_get()) && (!bOddField) ) )
		{
			vdp2_scrn_scroll_x_set(VDP2_SCRN_NBG1,fix16_int32_from(-x));
			vdp2_scrn_scroll_y_set(VDP2_SCRN_NBG1,fix16_int32_from(-y));
			vdp2_sync();
		}
		else
		{
			vdp2_scrn_scroll_x_set(VDP2_SCRN_NBG1,fix16_int32_from(140));
			vdp2_scrn_scroll_y_set(VDP2_SCRN_NBG1,fix16_int32_from(140));
			vdp2_sync();
		}
	}
}