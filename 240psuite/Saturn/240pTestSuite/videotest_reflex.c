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

void update_reflex(_svin_screen_mode_t screenmode, int offset, int mode)
{
	vdp1_cmdt_t * cmdlist = (vdp1_cmdt_t*)VDP1_VRAM(0);
	vdp1_vram_partitions_t vdp1_vram_partitions;
    vdp1_vram_partitions_get(&vdp1_vram_partitions);
	
	if (mode & 0x1)
		cmdlist[4].cmd_xa = 160 - 16 + offset;
	else
		cmdlist[4].cmd_xa = 160 - 16;
	if (mode & 0x2)
		cmdlist[5].cmd_ya = 120 - 16 + offset;
	else
		cmdlist[5].cmd_ya = 120 - 16;
}


void draw_reflex(_svin_screen_mode_t screenmode)
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
		_pointer32[16+j] = 0x02020202;
	//fill everything with our tile
    _pointer32 = (int *)_SVIN_NBG0_PNDR_START;
    for (unsigned int i = 0; i < _SVIN_NBG0_PNDR_SIZE / sizeof(int); i++)
    {
        _pointer32[i] = 0x00200002; //palette 2, transparency on, black
    }
	_svin_set_cycle_patterns_nbg();

	//generating VDP1 sprite in VRAM
	vdp1_vram_partitions_t vdp1_vram_partitions;
    vdp1_vram_partitions_get(&vdp1_vram_partitions);
	uint8_t * p8 = (uint8_t*)vdp1_vram_partitions.texture_base+0x11800;
	uint8_t pattern_color = 1 | FONT_WHITE*4;
	memset(p8,0,32*32);
	for (int y=5;y<28;y++)
		for (int x=5;x<28;x++)
			p8[32*y+x] = pattern_color;
	for (int y=0;y<16;y++)
		for (int x=14-y;x<16;x++)
			if (x>=0)
				{
					p8[32*y+x] = pattern_color;
					p8[32*y+31-x] = pattern_color;
					p8[32*(31-y)+x] = pattern_color;
					p8[32*(31-y)+31-x] = pattern_color;
				}
	for (int y=6;y<27;y++)
		for (int x=6;x<27;x++)
			p8[32*y+x] = 0;
	for (int y=1;y<16;y++)
		for (int x=15-y;x<16;x++)
			if (x>=1)
				{
					p8[32*y+x] = 0;
					p8[32*y+31-x] = 0;
					p8[32*(31-y)+x] = 0;
					p8[32*(31-y)+31-x] = 0;
				}

	vdp1_cmdt_t * cmdlist = (vdp1_cmdt_t*)VDP1_VRAM(0);
	vdp1_cmdt_draw_mode_t sprite_draw_mode = {.raw = 0x0000, .pre_clipping_disable = true};
    vdp1_cmdt_color_bank_t font_color_bank = {.raw = 0};
	//leaving command 3 as-is
	//adding 2 commands for patterns
	for (int i=0;i<3;i++)
	{
		memset(&(cmdlist[i+3]),0,sizeof(vdp1_cmdt_t));
	    vdp1_cmdt_normal_sprite_set(&(cmdlist[i+3]));
		vdp1_cmdt_draw_mode_set(&(cmdlist[i+3]), sprite_draw_mode);
		vdp1_cmdt_color_mode4_set(&(cmdlist[i+3]),font_color_bank);//8bpp
		cmdlist[i+3].cmd_xa = 160 - 16;
    	cmdlist[i+3].cmd_ya = 120 - 16;
		vdp1_cmdt_char_base_set(&(cmdlist[i+3]),vdp1_vram_partitions.texture_base+0x11800);
		cmdlist[i+3].cmd_size=((32/8)<<8)|(32);
	}
    //end of commands list
    vdp1_cmdt_end_set(&(cmdlist[6]));

	//drawing text
	DrawString("Press A when the sprites are aligned", 10, 180, FONT_GREEN);
	DrawString("Negative values mean you pressed A prematurely", 10, 190, FONT_GREEN);
	DrawString("X button toggles horizontal and vertical movement", 10, 200, FONT_GREEN);
}

void videotest_reflex(_svin_screen_mode_t screenmode)
{
	bool key_pressed = false;
	int results[10];
	int results_number = 0;

	//using 240p screenmode, no switching supported
	_svin_screen_mode_t curr_screenmode =
	{
		.scanmode = _SVIN_SCANMODE_240P,
		.x_res = _SVIN_X_RESOLUTION_320,
		.y_res = VDP2_TVMD_VERT_240,
		.x_res_doubled = false,
		.colorsystem = VDP2_TVMD_TV_STANDARD_NTSC,
	};
	update_screen_mode(curr_screenmode,false);
	
	int offset = 0;
	int offset_dir = 1;
	int mode = 1;
	char buf[64];
	draw_reflex(curr_screenmode);
	update_reflex(screenmode,offset,mode);
	wait_for_key_unpress();
	vdp1_cmdt_t * cmdlist = (vdp1_cmdt_t*)VDP1_VRAM(0);

	while (1)
	{
		smpc_peripheral_process();
		get_digital_keypress_anywhere(&controller);
		if ( (controller.pressed.button.a) || (controller.pressed.button.c) ) 
		{
			if (false == key_pressed)
			{
				results[results_number] = offset;
				sprintf(buf,"Offset %d: %2d frames",results_number,offset);
				DrawString(buf, 10, 10+results_number*10, FONT_WHITE);
				results_number++;
				if (10 == results_number)
				{
					wait_for_key_unpress();
					//clear screen
					ClearTextLayer();
					for (int i=0;i<3;i++)
						cmdlist[i+3].cmd_xa = -160;
					//print report
					int sum=0;
					for (int i=0;i<10;i++)
					{
						sprintf(buf,"%2d",results[i]);
						DrawString(buf, 20, 10+i*10, FONT_WHITE);
						sum += (results[i]>0) ? results[i] : -results[i];
					}
					DrawString("+", 10, 10+45, FONT_RED);
					DrawString("-----", 10, 10+100, FONT_RED);
					double fps = (VDP2_TVMD_TV_STANDARD_NTSC == screenmode.colorsystem) ? 60000.0/1001.0 : 50.0;
					double flen = 1000.0 / fps;	
					double avrg = 0.1 * sum;
					sprintf(buf,"%d/10 = %d.%02d average frames ~= %3d.%02d ms",sum,(int)avrg,(int)((int)(avrg*100.0))%100,(int)(avrg*flen),(int)((int)(avrg*flen*100.0))%100 );
					DrawString(buf, 20, 10+110, FONT_WHITE);
					DrawString("Keep in mind that this is not a lag test", 20, 10+120, FONT_CYAN);
					sprintf(buf,"A frame is around %d.%2d ms",(int)flen,(int)((int)(flen*100.0))%100);
					DrawString(buf, 20, 10+130, FONT_CYAN);
					wait_for_key_press();
					wait_for_key_unpress();
					update_screen_mode(screenmode,false);
					return;
				}
				key_pressed = true;
			}
		}
		else if (controller.pressed.button.x)
		{
			if (false == key_pressed)
			{
				mode = (1 == mode) ? 2 :
							(2 == mode) ? 3 : 1;
				update_reflex(screenmode,offset,mode);
				key_pressed = true;
			}
		}
		else if (controller.pressed.button.b)
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
		offset+=offset_dir;
		if ( (offset > 35) || (offset < -35) )
		{
			offset_dir = - offset_dir;
			offset += 2*offset_dir;
		}
		update_reflex(screenmode,offset,mode);
	}
}
