#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "svin.h"
#include "control.h"
#include "ire.h"

#define VERSION_NUMBER "Saturn Ver. 0.3 beta 1"
#define VERSION_DATE __DATE__

//there is no way to fit all credits on-screen, so scrolling is implemented
//scrolling is implemented as a change of VDP1 text quad start address

void draw_credits()
{
	bool doexit = false;
	char data[25];
	int counter = 1;
	int x = 32, y = 16, pos = 0;

	ClearTextLayer();
	//additional cleaning
	ClearText(0,FONT_QUAD_HEIGHT,FONT_QUAD_WIDTH,FONT_QUAD_HEIGHT);
	
	pos=17;
	//DrawString(VERSION_NUMBER, 208, y+_fh*0, FONT_WHITE);
	//DrawString(VERSION_DATE, 208, y+_fh*1, FONT_WHITE); 
	DrawString(VERSION_NUMBER, x, y+_fh*pos, FONT_WHITE); pos++;
	DrawString(VERSION_DATE, x, y+_fh*pos, FONT_WHITE); pos++;
	pos++;
	DrawString("Code and Patterns:", x, y+_fh*pos, FONT_GREEN); pos++;
	DrawString("Artemio Urbina", x+5, y+_fh*pos, FONT_WHITE);
	DrawString("@Artemio", x+5+17*_fw, y+_fh*pos, FONT_YELLOW); pos++;
	DrawString("aurbina@junkerhq.net", x+5, y+_fh*pos, FONT_CYAN); pos++;
	pos++;
	DrawString("Yaul SDK & Consultant:", x, y+_fh*pos, FONT_GREEN); pos++;	
	DrawString("Israel Jacquez", x+5, y+_fh*pos, FONT_WHITE);
	DrawString("www.yaul.org", x+5+17*_fw, y+_fh*pos, FONT_CYAN); pos++;
	DrawString("Porting:", x, y+_fh*pos, FONT_GREEN); pos++;	
	DrawString("hitomi2500", x+5, y+_fh*pos, FONT_WHITE); pos++;	
	DrawString("USB DevCart:", x, y+_fh*pos, FONT_GREEN); pos++;	
	DrawString("cafe-alpha", x+5, y+_fh*pos, FONT_WHITE); pos++;	
	pos++;
	DrawString("Menu Pixel Art:", x, y+_fh*pos, FONT_GREEN); pos++;	
	DrawString("Asher", x+5, y+_fh*pos, FONT_WHITE); pos++;	
	DrawString("Advisor:", x, y+_fh*pos, FONT_GREEN); pos++;	
	DrawString("Fudoh", x+5, y+_fh*pos, FONT_WHITE); pos++;	
	DrawString("Monoscope Patterns:", x, y+_fh*pos, FONT_GREEN); pos++;	
	DrawString("Keith Raney", x+5, y+_fh*pos, FONT_WHITE);
	DrawString("@khmr33", x+5+14*_fw, y+_fh*pos, FONT_YELLOW); pos++;	
	DrawString("Donna Art:", x, y+_fh*pos, FONT_GREEN); pos++;	
	DrawString("Jose Salot", x+5, y+_fh*pos, FONT_WHITE);
	DrawString("@pepe_salot", x+5+13*_fw, y+_fh*pos, FONT_YELLOW); pos++;	
	pos++;
	DrawString("Info on using this suite:", x, y+_fh*pos, FONT_GREEN); pos++;
	DrawString("http://junkerhq.net/240p/", x+5, y+_fh*pos, FONT_CYAN); pos++;
	DrawString("This is free software.", x, y+_fh*pos, FONT_WHITE); pos++;	
	DrawString("Source code available under GPL.", x, y+_fh*pos, FONT_WHITE); pos++;	
	pos++;
	DrawString("Dedicated to Elisa", x, y+_fh*pos, FONT_GREEN);
}

void window_credits(_svin_screen_mode_t screenmode)
{
	vdp1_cmdt_t *_svin_cmdt_p = (vdp1_cmdt_t *)(VDP1_VRAM(0));
	vdp1_vram_partitions_t vdp1_vram_partitions;
    vdp1_vram_partitions_get(&vdp1_vram_partitions);
	
	//ClearTextLayer();
	uint8_t * p8_vram = (uint8_t *)(vdp1_vram_partitions.texture_base);
	for (int _x = 0; _x < 320*256*4 ; _x++)
	{
		p8_vram[_x] = 0;
	}

	wait_for_key_unpress();
	int scroll_counter = 0;

	_svin_cmdt_p[_SVIN_VDP1_ORDER_TEXT_SPRITE_0_INDEX].cmd_ya=-300;//move the text out of screen before it is printed

	_svin_screen_mode_t creditsScreenMode =
	{
		.scanmode = _SVIN_SCANMODE_240P,
		.x_res = _SVIN_X_RESOLUTION_320,
		.y_res = VDP2_TVMD_VERT_240,
		.x_res_doubled = false,
		.colorsystem = VDP2_TVMD_TV_STANDARD_NTSC,
	};
	update_screen_mode(creditsScreenMode);
	draw_bg_with_expansion(creditsScreenMode,true);
	int _size_y = 240;

	//resize the text window to implement scrolling
	_svin_cmdt_p[_SVIN_VDP1_ORDER_TEXT_SPRITE_0_INDEX].cmd_size=((320/8)<<8)|(_size_y-80);
    _svin_cmdt_p[_SVIN_VDP1_ORDER_TEXT_SPRITE_0_INDEX].cmd_ya= (VDP2_TVMD_VERT_224 == screenmode.y_res) ? 40 : 
                                                (VDP2_TVMD_VERT_240 == screenmode.y_res) ? 48 : 56;
	vdp1_cmdt_char_base_set(&_svin_cmdt_p[_SVIN_VDP1_ORDER_TEXT_SPRITE_0_INDEX],
								vdp1_vram_partitions.texture_base+FONT_QUAD_WIDTH*scroll_counter);

	draw_credits();
	bool key_pressed = false;


	while (1)
	{
		smpc_peripheral_process();
		smpc_peripheral_digital_port(1, &controller);
		if ( (controller.pressed.button.a) || (controller.pressed.button.b) || (controller.pressed.button.c) )
		{
			ClearTextLayer();
			//quit the pattern
			wait_for_key_unpress();
			//restoring the screenmode
			update_screen_mode(screenmode);
			return;
		}
		vdp2_tvmd_vblank_in_wait();
		vdp2_tvmd_vblank_out_wait();
		scroll_counter++;
		if (scroll_counter > 430*4) scroll_counter = 0;
		vdp1_cmdt_char_base_set(&_svin_cmdt_p[_SVIN_VDP1_ORDER_TEXT_SPRITE_0_INDEX],
								vdp1_vram_partitions.texture_base+FONT_QUAD_WIDTH*(scroll_counter>>2));
	}
}
