#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "svin.h"
#include "video.h"
#include "control.h"
#include "ire.h"

extern uint8_t asset_sonicfloor_bg[];
extern uint8_t asset_sonicfloor_bg_end[];
extern uint8_t asset_sonicback1_bg[];
extern uint8_t asset_sonicback1_bg_end[];
extern uint8_t asset_sonicback2_bg[];
extern uint8_t asset_sonicback2_bg_end[];
extern uint8_t asset_sonicback3_bg[];
extern uint8_t asset_sonicback3_bg_end[];
extern uint8_t asset_sonicback4_bg[];
extern uint8_t asset_sonicback4_bg_end[];

void update_scroll_horizontal(_svin_screen_mode_t screenmode, int frame, int offset1, int offset2)
{
	vdp1_cmdt_t * cmdlist = (vdp1_cmdt_t*)VDP1_VRAM(0);
	vdp1_vram_partitions_t vdp1_vram_partitions;
    vdp1_vram_partitions_get(&vdp1_vram_partitions);
	//update patterns for backgound 2
	vdp1_cmdt_char_base_set(&(cmdlist[3]),vdp1_vram_partitions.texture_base+0x12000+256*256*(frame%4));
	vdp1_cmdt_char_base_set(&(cmdlist[4]),vdp1_vram_partitions.texture_base+0x12000+256*256*(frame%4));
	vdp1_cmdt_char_base_set(&(cmdlist[5]),vdp1_vram_partitions.texture_base+0x12000+256*256*(frame%4));
	//shift background 2
	for (int i=0;i<3;i++)
		cmdlist[i+3].cmd_xa = -256 + i*256+offset2;

	//shift background1
	//vdp2_scrn_scroll_x_set(VDP2_SCRN_NBG0,fix16_int32_from(offset1));
	uint16_t * p16 = (uint16_t *)(VDP2_IOREG_BASE+SCXIN0);
	p16[0] = offset1;
	//vdp2_sync();
}

void draw_scroll_horizontal(_svin_screen_mode_t screenmode)
{
	char buf[256*256];	
	_svin_set_cycle_patterns_cpu();

	uint8_t *p8 = (uint8_t *)(asset_sonicfloor_bg);
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
		bcl_lz_decompress(&(p8[2048+8]),(char*)_SVIN_NBG0_CHPNDR_START,compressed_size);
		//palette in file is 24-bit, setting it color-by-color
		rgb888_t _color  = {0,0,0,0};
		for (int i = 0; i<256; i++)
		{
			_color.r = p8[2048*compressed_size_sectors+2048+i*3+0];
			_color.g = p8[2048*compressed_size_sectors+2048+i*3+1];
			_color.b = p8[2048*compressed_size_sectors+2048+i*3+2];
			_svin_set_palette_part(2, &_color, i, i);
		}
	}

	_svin_set_cycle_patterns_nbg(screenmode);

	//enable NBG0 and NBG1 transparency
	vdp2_scrn_display_set(VDP2_SCRN_DISPTP_NBG0 | VDP2_SCRN_DISPTP_NBG1);

	//using VDP1 for 2nd background
	vdp1_vram_partitions_t vdp1_vram_partitions;
    vdp1_vram_partitions_get(&vdp1_vram_partitions);
	//unpack 2nd background assets to VRAM
	for (int frame = 0; frame < 4; frame++)
	//int frame = 0;
	{
		uint8_t * p8;
		switch(frame)
		{
			case 0:
				p8 = (uint8_t *)(asset_sonicback1_bg);
				break;
			case 1:
				p8 = (uint8_t *)(asset_sonicback2_bg);
				break;
			case 2:
				p8 = (uint8_t *)(asset_sonicback3_bg);
				break;
			case 3:
				p8 = (uint8_t *)(asset_sonicback4_bg);
				break;
		}
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
			bcl_lz_decompress(&(p8[2048+8]),buf,compressed_size);
			//shift data
			if (frame > 0)
				for (int i=0;i<256*256;i++)
					if (buf[i])
						buf[i] += 0x40*frame;
			//copy to VRAM
			memcpy(((char *)(vdp1_vram_partitions.texture_base+0x12000+size_x*size_y*frame)),buf,256*256);

			//set palette, using palette 0
			//palette in file is 24-bit, setting it color-by-color
			rgb888_t _color  = {0,0,0,0};
			for (int i = 0; i<64; i++)
			{
				_color.r = p8[2048*compressed_size_sectors+2048+i*3+0];
				_color.g = p8[2048*compressed_size_sectors+2048+i*3+1];
				_color.b = p8[2048*compressed_size_sectors+2048+i*3+2];
				_svin_set_palette_part(0, &_color, i+frame*64, i+frame*64);
			}
		}
	}

	//adding 3 commands for background 2
	vdp1_cmdt_t * cmdlist = (vdp1_cmdt_t*)VDP1_VRAM(0);
	vdp1_cmdt_draw_mode_t sprite_draw_mode = {.raw = 0x0000, .pre_clipping_disable = true};
    vdp1_cmdt_color_bank_t font_color_bank = {.raw = 0};
	for (int i=0;i<3;i++)
	{
		memset(&(cmdlist[i+3]),0,sizeof(vdp1_cmdt_t));
	    vdp1_cmdt_normal_sprite_set(&(cmdlist[i+3]));
		vdp1_cmdt_draw_mode_set(&(cmdlist[i+3]), sprite_draw_mode);
		vdp1_cmdt_color_mode4_set(&(cmdlist[i+3]),font_color_bank);//8bpp
		cmdlist[i+3].cmd_xa = -256 + i*256;
    	cmdlist[i+3].cmd_ya = 0;
		vdp1_cmdt_char_base_set(&(cmdlist[i+3]),vdp1_vram_partitions.texture_base+0x12000);
		cmdlist[i+3].cmd_size=((256/8)<<8)|(255);
   		cmdlist[i+3].cmd_draw_mode.end_code_disable = 1;
	}
	vdp1_cmdt_end_set(&(cmdlist[6]));

	//changing sprite priority in VDP2 to 1
    p16 = (uint16_t *)(VDP2_IOREG_BASE+PRISA);
    p16[0] = 0x0101;
    p16[1] = 0x0101;
    p16[2] = 0x0101;
    p16[3] = 0x0101;
}

void videotest_scroll_horizontal(_svin_screen_mode_t screenmode)
{
	//removing text
	ClearTextLayer();

	//using 240p screenmode, no switching supported
	_svin_screen_mode_t curr_screenmode =
	{
		.scanmode = _SVIN_SCANMODE_240P,
		.x_res = _SVIN_X_RESOLUTION_320,
		.y_res = VDP2_TVMD_VERT_240,
		.x_res_doubled = false,
		.colorsystem = VDP2_TVMD_TV_STANDARD_NTSC,
	};
	update_screen_mode(curr_screenmode,true);

	draw_scroll_horizontal(curr_screenmode);
	bool key_pressed = false;
	int *_pointer32 = (int *)_SVIN_NBG0_CHPNDR_START;
	int pattern = 0;
	int _size_x = get_screenmode_resolution_x(curr_screenmode);
	int _size_y = get_screenmode_resolution_y(curr_screenmode);
	int x=_size_x/2;
	int y=_size_y/2;

	wait_for_key_unpress();
	
	int mode_display_counter = 0;
	int frame_counter = 0;
	int offset1 = 0;
	int offset2 = 0;
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
		offset1+=2*speed;
		offset1 %= 1024;
		offset2-=speed;
		if (offset2<-128) offset2+=256;
		update_scroll_horizontal(screenmode,frame_counter,offset1,offset2);
	}
}