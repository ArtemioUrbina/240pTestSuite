#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "video_vdp2.h"
#include "video.h"
#include "control.h"
#include "ire.h"

extern uint8_t asset_kiki_bg[];
extern uint8_t asset_kiki_bg_end[];

void update_scroll_vertical(video_screen_mode_t screenmode, int frame, int offset)
{
	//shift background1
	//vdp2_scrn_scroll_x_set(VDP2_SCRN_NBG0,fix16_int32_from(offset1));
	uint16_t * p16 = (uint16_t *)(VDP2_IOREG_BASE+SCYIN0);
	p16[0] = offset;
	//vdp2_sync();
}

void draw_scroll_vertical(video_screen_mode_t screenmode)
{
	char buf[256*256];	
	video_vdp2_set_cycle_patterns_cpu();

	uint8_t *p8 = (uint8_t *)(asset_kiki_bg);
	uint16_t *p16 = (uint16_t*)p8;
	uint16_t size_x = p16[2];
	uint16_t size_y = p16[3];
	//if ( (p8[2048] == 'L') && (p8[2048+1] == 'Z') && (p8[2048+2] == '7') && (p8[2048+3] == '7') )
	{
		//compressed, decompressing
		//getting size
		int * p32 = (int *)(&p8[2048]);
		int compressed_size = p32[1];
		assert(compressed_size > 0);
		assert(compressed_size < 0x1000000);
		int compressed_size_sectors = ((compressed_size-1)/2048)+1;
		//decompress
		bcl_lz_decompress(&(p8[2048+8]),(char*)VIDEO_VDP2_NBG0_CHPNDR_START,compressed_size);
		//palette in file is 24-bit, setting it color-by-color
		rgb888_t _color  = {0,0,0,0};
		for (int i = 0; i<256; i++)
		{
			_color.r = p8[2048*compressed_size_sectors+2048+i*3+0];
			_color.g = p8[2048*compressed_size_sectors+2048+i*3+1];
			_color.b = p8[2048*compressed_size_sectors+2048+i*3+2];
			video_vdp2_set_palette_part(2, &_color, i, i);
		}
	}

	video_vdp2_set_cycle_patterns_nbg(screenmode);
}

void videotest_scroll_vertical(video_screen_mode_t screenmode)
{
	//removing text
	ClearTextLayer();

	//using 240p screenmode, no switching supported
	video_screen_mode_t curr_screenmode =
	{
		.scanmode = VIDEO_SCANMODE_240P,
		.x_res = VIDEO_X_RESOLUTION_320,
		.y_res = VDP2_TVMD_VERT_240,
		.x_res_doubled = false,
		.colorsystem = VDP2_TVMD_TV_STANDARD_NTSC,
	};
	update_screen_mode(curr_screenmode,true);

	draw_scroll_vertical(curr_screenmode);
	bool key_pressed = false;
	int *_pointer32 = (int *)VIDEO_VDP2_NBG0_CHPNDR_START;
	int pattern = 0;
	int _size_x = get_screenmode_resolution_x(curr_screenmode);
	int _size_y = get_screenmode_resolution_y(curr_screenmode);
	int x=_size_x/2;
	int y=_size_y/2;

	wait_for_key_unpress();
	
	int mode_display_counter = 0;
	int frame_counter = 0;
	int offset = 0;
	char buf[32];
	int speed = 1;

	while (1)
	{
		smpc_peripheral_process();
		get_digital_keypress_anywhere(&controller);
		if ( (controller.pressed.button.up) )
		{
			if (false == key_pressed)
			{
				key_pressed = true;
				speed++;
				if (speed > 10) speed=10;
			}
		}
		else if ( (controller.pressed.button.down) )
		{
			if (false == key_pressed)
			{
				key_pressed = true;
				speed--;
				if (speed < 1) speed=1;
			}
		}
		else if ( (controller.pressed.button.b) )
		{
			//quit the pattern
			wait_for_key_unpress();
			update_screen_mode(screenmode,false);
			return;
		}
		else
			key_pressed = false;

		vdp2_tvmd_vblank_out_wait();
		vdp2_tvmd_vblank_in_wait();
		if (mode_display_counter > 0)
		{
			mode_display_counter--;
			if (0 == mode_display_counter)
				ClearTextLayer();
		}
		//changing the palette
		//disable pattern on every second frame
		frame_counter++;
		offset-=speed;
		offset %= 512;
		update_scroll_vertical(screenmode,frame_counter,offset);
	}
}