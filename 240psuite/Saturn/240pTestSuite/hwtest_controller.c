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

void update_controller(_svin_screen_mode_t screenmode)
{
	int x,y;
	for (int port = 1; port < 3; port++)
	{
		x = 10;
		y = 10+port*10;
		char buf[16];
		smpc_peripheral_digital_port(port, &controller);
		sprintf(buf,"Port %d :",port);
		/*smpc_peripheral_port_t * _port = smpc_peripheral_raw_port(port);
		smpc_peripheral_t *peripheral;
        smpc_peripheral_t *tmp_peripheral;
		int per=0;
        for (peripheral = TAILQ_FIRST(&_port->peripherals);
			(peripheral != NULL) && (tmp_peripheral = TAILQ_NEXT(peripheral, peripherals), 1);
            	peripheral = tmp_peripheral) {
					sprintf(buf,"Port 1 per %i",per);
					DrawString(buf, x, y, FONT_WHITE);
					x+=_fw*10;
					per++;
				}*/
		DrawString(buf, x, y, FONT_MAGENTA);
		x+=_fw*10;
		if (controller.pressed.button.a)
			DrawString("A", x, y, FONT_GREEN);
		else
			DrawString("A", x, y, FONT_WHITE);
		x+=_fw*2;
		if (controller.pressed.button.b)
			DrawString("B", x, y, FONT_GREEN);
		else
			DrawString("B", x, y, FONT_WHITE);
		x+=_fw*2;
		if (controller.pressed.button.c)
			DrawString("C", x, y, FONT_GREEN);
		else
			DrawString("C", x, y, FONT_WHITE);
		x+=_fw*2;
		if (controller.pressed.button.x)
			DrawString("X", x, y, FONT_GREEN);
		else
			DrawString("X", x, y, FONT_WHITE);
		x+=_fw*2;
		if (controller.pressed.button.y)
			DrawString("Y", x, y, FONT_GREEN);
		else
			DrawString("Y", x, y, FONT_WHITE);
		x+=_fw*2;
		if (controller.pressed.button.z)
			DrawString("Z", x, y, FONT_GREEN);
		else
			DrawString("Z", x, y, FONT_WHITE);
		x+=_fw*2;
		if (controller.pressed.button.l)
			DrawString("L", x, y, FONT_GREEN);
		else
			DrawString("L", x, y, FONT_WHITE);
		x+=_fw*2;
		if (controller.pressed.button.r)
			DrawString("R", x, y, FONT_GREEN);
		else
			DrawString("R", x, y, FONT_WHITE);
		x+=_fw*2;
		if (controller.pressed.button.up)
			DrawString("Up", x, y, FONT_GREEN);
		else
			DrawString("Up", x, y, FONT_WHITE);
		x+=_fw*3;
		if (controller.pressed.button.down)
			DrawString("Down", x, y, FONT_GREEN);
		else
			DrawString("Down", x, y, FONT_WHITE);
		x+=_fw*5;
		if (controller.pressed.button.left)
			DrawString("Left", x, y, FONT_GREEN);
		else
			DrawString("Left", x, y, FONT_WHITE);
		x+=_fw*5;
		if (controller.pressed.button.right)
			DrawString("Right", x, y, FONT_GREEN);
		else
			DrawString("Right", x, y, FONT_WHITE);
		x+=_fw*6;
		if (controller.pressed.button.start)
			DrawString("Start", x, y, FONT_GREEN);
		else
			DrawString("Start", x, y, FONT_WHITE);
	}
	DrawString("Press Left + Start on controller 1 to Exit", 20, 200, FONT_WHITE);
}


void draw_controller(_svin_screen_mode_t screenmode)
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
	Color.r = IRE_bot + (IRE_top-IRE_bot)/5;
	Color.g = IRE_bot + (IRE_top-IRE_bot)/5;
	Color.b = IRE_top;
	_svin_set_palette_part(2,&Color,2,2); //palette 2 color 2 = IRE lightblue

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
	_svin_set_cycle_patterns_nbg(screenmode);
}

void hwtest_controller(_svin_screen_mode_t screenmode)
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
	draw_controller(curr_screenmode);
	update_controller(curr_screenmode);
	wait_for_key_unpress();
	vdp1_cmdt_t * cmdlist = (vdp1_cmdt_t*)VDP1_VRAM(0);

	while (1)
	{
		smpc_peripheral_process();
		get_digital_keypress_anywhere(&controller);
		if ( (controller.pressed.button.left) && (controller.pressed.button.start) )
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

		update_controller(curr_screenmode);
	}
}
