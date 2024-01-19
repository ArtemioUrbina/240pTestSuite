#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "svin.h"
#include "video.h"
#include "control.h"
#include "ire.h"

void draw_backlit_zone(_svin_screen_mode_t screenmode)
{	
	_svin_set_cycle_patterns_cpu();
	//adding a single color to palette
	uint8_t IRE_top = Get_IRE_Level(100);
	uint8_t IRE_bot = Get_IRE_Level(0);
	rgb888_t Color = {0,IRE_bot,IRE_bot,IRE_bot};
	Color.r = IRE_top;
	Color.g = IRE_top;
	Color.b = IRE_top;	
	_svin_set_palette_part(2,&Color,1,1); //palette 2 color 1 = IRE white
	Color.r = IRE_bot;
	Color.g = IRE_bot;
	Color.b = IRE_bot;	
	_svin_set_palette_part(2,&Color,2,2); //palette 2 color 2 = IRE black
	//create two tiles for the colors 1 and 2
	int *_pointer32 = (int *)_SVIN_NBG0_CHPNDR_START;
	for (int j=0; j<16; j++)
	{
		_pointer32[16+j] = 0x01010101;
		_pointer32[32+j] = 0x02020202;
	}
	//fill NBG0 with our black tile
    _pointer32 = (int *)_SVIN_NBG0_PNDR_START;
    for (unsigned int i = 0; i < _SVIN_NBG0_PNDR_SIZE / sizeof(int); i++)
    {
        _pointer32[i] = 0x00200004; //palette 2
    }
	//constructing pattern
	uint8_t pattern_color = 1;
	uint8_t pattern [8*8];
	memset(pattern,0,8*8);
	for (int y=4;y<6;y++)
		for (int x=4;x<6;x++)
			pattern[8*y+x] = pattern_color;

	//copying pattern to NBG1
	uint8_t * _p8 = (uint8_t *)_SVIN_NBG1_CHPNDR_START;
	for (int _y=0;_y<8;_y++)
		memcpy(&(_p8[_y*8]),&(pattern[_y*8]),8);

	//generating names for pattern in NBG1
	_pointer32 = (int *)_SVIN_NBG1_PNDR_START;
    _pointer32[0] = 0x00200000 + 0x60000/32; //palette 2

	_svin_set_cycle_patterns_nbg();
}

void videotest_backlit_zone(_svin_screen_mode_t screenmode)
{
	//removing text
	ClearTextLayer();

	_svin_screen_mode_t curr_screenmode = screenmode;
	draw_backlit_zone(curr_screenmode);
	bool key_pressed = false;
	int *_pointer32 = (int *)_SVIN_NBG0_CHPNDR_START;
	int pattern = 0;
	int _size_x = get_screenmode_resolution_x(curr_screenmode);
	int _size_y = get_screenmode_resolution_y(curr_screenmode);
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
		vdp2_scrn_scroll_x_set(VDP2_SCRN_NBG1,fix16_int32_from(-x));
		vdp2_scrn_scroll_y_set(VDP2_SCRN_NBG1,fix16_int32_from(-y));
		vdp2_sync();

		if ( (controller.pressed.button.l) )
		{
			curr_screenmode = prev_screen_mode(curr_screenmode);
			update_screen_mode(curr_screenmode);
			draw_backlit_zone(curr_screenmode);
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
			draw_backlit_zone(curr_screenmode);
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
			DrawStringWithBackground(buf, 160-strlen(buf)*_fw/2, 120, FONT_WHITE,FONT_BLUE);
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
	}
}