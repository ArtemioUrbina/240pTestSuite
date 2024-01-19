#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "svin.h"
#include "video.h"
#include "control.h"
#include "ire.h"
#include "image_big_digits.h"

void update_lag(_svin_screen_mode_t screenmode, int frame)
{
	vdp1_cmdt_t * cmdlist = (vdp1_cmdt_t*)VDP1_VRAM(0);
	vdp1_vram_partitions_t vdp1_vram_partitions;
    vdp1_vram_partitions_get(&vdp1_vram_partitions);
	//calculate time
	int hours,minutes,seconds,frame_xtra;
	if (VDP2_TVMD_TV_STANDARD_NTSC == screenmode.colorsystem)
	{
		frame_xtra = frame;
		hours = (frame_xtra * 1001) / (1000 * 60 * 3600);	
		//let's suppose nobody runs the test for 100 hours 0_0
		frame_xtra -= (hours * 3600 * 60 *1000) / 1001;
		minutes = (frame_xtra * 1001) / (1000 * 60 * 60);
		frame_xtra -= (minutes * 60 * 60 *1000) / 1001;
		seconds = (frame_xtra * 1001) / (1000 * 60);
		frame_xtra -= (seconds * 60 *1000) / 1001;
	}
	else
	{
		//PAL
		frame_xtra = frame;
		hours = frame_xtra / (50 * 3600);	
		//let's suppose nobody runs the test for 100 hours 0_0
		frame_xtra -= hours * 3600 * 50;
		minutes = frame_xtra / (50 * 60);
		frame_xtra -= minutes * 60 * 50;
		seconds = frame_xtra / 50;
		frame_xtra -= seconds * 50;
	}

	//draw time
	vdp1_cmdt_char_base_set(&(cmdlist[4]),vdp1_vram_partitions.texture_base+0x16000+34*24*((hours/10)%10));
	vdp1_cmdt_char_base_set(&(cmdlist[5]),vdp1_vram_partitions.texture_base+0x16000+34*24*((hours)%10));
	vdp1_cmdt_char_base_set(&(cmdlist[7]),vdp1_vram_partitions.texture_base+0x16000+34*24*((minutes/10)%10));
	vdp1_cmdt_char_base_set(&(cmdlist[8]),vdp1_vram_partitions.texture_base+0x16000+34*24*((minutes)%10));
	vdp1_cmdt_char_base_set(&(cmdlist[10]),vdp1_vram_partitions.texture_base+0x16000+34*24*((seconds/10)%10));
	vdp1_cmdt_char_base_set(&(cmdlist[11]),vdp1_vram_partitions.texture_base+0x16000+34*24*((seconds)%10));
	if (frame_xtra%2==0)
	{
		//blue frames
		vdp1_cmdt_char_base_set(&(cmdlist[13]),vdp1_vram_partitions.texture_base+0x1C000+34*24*((frame_xtra/10)%10));
		vdp1_cmdt_char_base_set(&(cmdlist[14]),vdp1_vram_partitions.texture_base+0x1C000+34*24*((frame_xtra)%10));
	}
	else
	{
		//red frames
		vdp1_cmdt_char_base_set(&(cmdlist[13]),vdp1_vram_partitions.texture_base+0x1A000+34*24*((frame_xtra/10)%10));
		vdp1_cmdt_char_base_set(&(cmdlist[14]),vdp1_vram_partitions.texture_base+0x1A000+34*24*((frame_xtra)%10));
	}

	//select blue circles
	for (int i=0;i<8;i++)
	{
		vdp1_cmdt_char_base_set(&(cmdlist[i+15]),vdp1_vram_partitions.texture_base+0x13000);
	}
	//now mark red circle
	vdp1_cmdt_char_base_set(&(cmdlist[frame%8+15]),vdp1_vram_partitions.texture_base+0x12000);
}


void draw_lag(_svin_screen_mode_t screenmode)
{
	//removing text
	ClearTextLayer();
	
	//add colors to palette
	uint8_t IRE_top = Get_IRE_Level(100.0);
	uint8_t IRE_bot = Get_IRE_Level(7.5);
	rgb888_t Color = {0,0,0,0};
	Color.r = IRE_top;
	Color.g = IRE_top;
	Color.b = IRE_top;	
	_svin_set_palette_part(2,&Color,1,1); //palette 2 color 1 = IRE white
	Color.r = IRE_bot;
	Color.g = IRE_bot;
	Color.b = IRE_bot;
	_svin_set_palette_part(2,&Color,2,2); //palette 2 color 2 = IRE black
	Color.r = IRE_top;

	_svin_set_cycle_patterns_cpu();
	//create single-color tile for the color
	int *_pointer32 = (int *)_SVIN_NBG0_CHPNDR_START;
	for (int j=0; j<16; j++)
		_pointer32[16+j] = 0x01010101;
	//fill everything with our tile
    _pointer32 = (int *)_SVIN_NBG0_PNDR_START;
    for (unsigned int i = 0; i < _SVIN_NBG0_PNDR_SIZE / sizeof(int); i++)
    {
        _pointer32[i] = 0x00200002; //palette 2, transparency on, black
    }
	_svin_set_cycle_patterns_nbg();

	//generating VDP1 tile in VRAM: red circle
	vdp1_vram_partitions_t vdp1_vram_partitions;
    vdp1_vram_partitions_get(&vdp1_vram_partitions);
	const int RADIUS = 28;
	const int RADIUS_Q = 28*28;
	uint8_t * p8 = (uint8_t*)vdp1_vram_partitions.texture_base+0x12000;
	for (int y=0;y<RADIUS*2;y++)
		for (int x=0;x<RADIUS*2;x++)
			if ((x-RADIUS)*(x-RADIUS)+(y-RADIUS)*(y-RADIUS) < RADIUS_Q)
				p8[y*RADIUS*2+x] = 1 | FONT_RED*4;
			else
				p8[y*RADIUS*2+x] = 1 | FONT_WHITE*4;

	//generating VDP1 tile in VRAM: blue circle with
	p8 = (uint8_t*)vdp1_vram_partitions.texture_base+0x13000;
	for (int y=0;y<RADIUS*2;y++)
		for (int x=0;x<RADIUS*2;x++)
			if ((x-RADIUS)*(x-RADIUS)+(y-RADIUS)*(y-RADIUS) < RADIUS_Q)
				p8[y*RADIUS*2+x] = 1 | FONT_BLUE*4;
			else
				p8[y*RADIUS*2+x] = 1 | FONT_WHITE*4;

	//generating VDP1 tile in VRAM: black colon
	p8 = (uint8_t*)vdp1_vram_partitions.texture_base+0x14000;
	memset(p8,0,34*24);
	uint8_t color = 1 | FONT_BLACK*4;
	for (int y=0;y<34;y++)
		for (int x=0;x<22;x++)
			if (image_big_digits_data[34*22*10+y*22+x])
				p8[y*24+x+1] = color;

	//generating VDP1 tile in VRAM: black big digits
	p8 = (uint8_t*)vdp1_vram_partitions.texture_base+0x16000;
	memset(p8,0,34*10*22);
	color = 1 | FONT_BLACK*4;
	for (int y=0;y<34*10;y++)
		for (int x=0;x<22;x++)
			if (image_big_digits_data[y*22+x])
				p8[y*24+x+1] = color;

	//generating VDP1 tile in VRAM: white big digits
	p8 = (uint8_t*)vdp1_vram_partitions.texture_base+0x18000;
	memset(p8,0,34*10*22);
	color = 1 | FONT_WHITE*4;
	for (int y=0;y<34*10;y++)
		for (int x=0;x<22;x++)
			if (image_big_digits_data[y*22+x])
				p8[y*24+x+1] = color;

	//generating VDP1 tile in VRAM: red big digits
	p8 = (uint8_t*)vdp1_vram_partitions.texture_base+0x1A000;
	memset(p8,0,34*10*22);
	color = 1 | FONT_RED*4;
	for (int y=0;y<34*10;y++)
		for (int x=0;x<22;x++)
			if (image_big_digits_data[y*22+x])
				p8[y*24+x+1] = color;

	//generating VDP1 tile in VRAM: blue big digits
	p8 = (uint8_t*)vdp1_vram_partitions.texture_base+0x1C000;
	memset(p8,0,34*10*22);
	color = 1 | FONT_BLUE*4;
	for (int y=0;y<34*10;y++)
		for (int x=0;x<22;x++)
			if (image_big_digits_data[y*22+x])
				p8[y*24+x+1] = color;

	vdp1_cmdt_t * cmdlist = (vdp1_cmdt_t*)VDP1_VRAM(0);
	vdp1_cmdt_draw_mode_t sprite_draw_mode = {.raw = 0x0000, .pre_clipping_disable = true};
    vdp1_cmdt_color_bank_t font_color_bank = {.raw = 0};
	//for text layer, decreasing the size ti minimize overdraw
	cmdlist[2].cmd_size=((320/8)<<8)|(40);
	//leaving command 3 as-is
	//adding 11 commands for digits
	for (int i=0;i<11;i++)
	{
		memset(&(cmdlist[i+4]),0,sizeof(vdp1_cmdt_t));
	    vdp1_cmdt_normal_sprite_set(&(cmdlist[i+4]));
		vdp1_cmdt_draw_mode_set(&(cmdlist[i+4]), sprite_draw_mode);
		vdp1_cmdt_color_mode4_set(&(cmdlist[i+4]),font_color_bank);//8bpp
		cmdlist[i+4].cmd_xa = 10 + i*26;
    	cmdlist[i+4].cmd_ya = 30;
		vdp1_cmdt_char_base_set(&(cmdlist[i+4]),vdp1_vram_partitions.texture_base+0x14000);
		cmdlist[i+4].cmd_size=((24/8)<<8)|(34);
	}
	//adding 8 commands for circles
	for (int i=0;i<8;i++)
	{
		memset(&(cmdlist[i+15]),0,sizeof(vdp1_cmdt_t));
	    vdp1_cmdt_normal_sprite_set(&(cmdlist[i+15]));
		vdp1_cmdt_draw_mode_set(&(cmdlist[i+15]), sprite_draw_mode);
		vdp1_cmdt_color_mode4_set(&(cmdlist[i+15]),font_color_bank);//8bpp
		cmdlist[i+15].cmd_xa = 10 + (i%4)*80;
    	cmdlist[i+15].cmd_ya = 80+(i/4)*80;
		vdp1_cmdt_char_base_set(&(cmdlist[i+15]),vdp1_vram_partitions.texture_base+0x13000);
		cmdlist[i+15].cmd_size=((RADIUS/4)<<8)|(RADIUS*2);
	}
	//adding 8 commands for white digits over circles
	for (int i=0;i<8;i++)
	{
		memset(&(cmdlist[i+23]),0,sizeof(vdp1_cmdt_t));
	    vdp1_cmdt_normal_sprite_set(&(cmdlist[i+23]));
		vdp1_cmdt_draw_mode_set(&(cmdlist[i+23]), sprite_draw_mode);
		vdp1_cmdt_color_mode4_set(&(cmdlist[i+23]),font_color_bank);//8bpp
		cmdlist[i+23].cmd_xa = 10 + RADIUS - 12 + (i%4)*80;
    	cmdlist[i+23].cmd_ya = 80 + RADIUS - 17 + (i/4)*80;
		vdp1_cmdt_char_base_set(&(cmdlist[i+23]),vdp1_vram_partitions.texture_base+0x18000+34*24*(i+1));
		cmdlist[i+23].cmd_size=((24/8)<<8)|(34);
	}
    //end of commands list
    vdp1_cmdt_end_set(&(cmdlist[31]));

	//drawing text
	DrawStringWithBackground("HOURS", 20, 10, FONT_WHITE, FONT_BLUE);
	DrawStringWithBackground("MINUTES", 90, 10, FONT_WHITE, FONT_BLUE);
	DrawStringWithBackground("SECONDS", 165, 10, FONT_WHITE, FONT_BLUE);
	DrawStringWithBackground("FRAMES", 240, 10, FONT_WHITE, FONT_BLUE);
}

void videotest_lag(_svin_screen_mode_t screenmode)
{
	bool key_pressed = false;
	bool running = true;

	//using 240p screenmode, no switching supported
	_svin_screen_mode_t curr_screenmode =
	{
		.scanmode = _SVIN_SCANMODE_240P,
		.x_res = _SVIN_X_RESOLUTION_320,
		.y_res = VDP2_TVMD_VERT_240,
		.x_res_doubled = false,
		.colorsystem = VDP2_TVMD_TV_STANDARD_NTSC,
	};
	update_screen_mode(curr_screenmode);
	
	int mode_run_counter = 0;
	draw_lag(curr_screenmode);
	update_lag(screenmode,mode_run_counter);
	wait_for_key_unpress();

	while (1)
	{
		smpc_peripheral_process();
		smpc_peripheral_digital_port(1, &controller);
		if ( (controller.pressed.button.a) || (controller.pressed.button.c) ) 
		{
			if (false == key_pressed)
			{
				running = (running) ? false : true;
				key_pressed = true;
			}
		}
		else if (controller.pressed.button.x)
		{
			mode_run_counter=0;
			running = false;
			update_lag(screenmode,mode_run_counter);
			wait_for_key_unpress();
		}
		else if (controller.pressed.button.b)
		{
			//quit the pattern
			wait_for_key_unpress();
			update_screen_mode(screenmode);
			return;
		}
		else
			key_pressed = false;
		vdp2_tvmd_vblank_out_wait();
		vdp2_tvmd_vblank_in_wait();
		if (running)
		{
			update_lag(screenmode,mode_run_counter);
		}
		mode_run_counter++;
	}
}
