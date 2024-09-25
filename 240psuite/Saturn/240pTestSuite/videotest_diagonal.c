#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "svin.h"
#include "video.h"
#include "control.h"
#include "ire.h"

void update_diagonal(_svin_screen_mode_t screenmode, double angle, int angle_step_div)
{
	int _size_x = get_screenmode_resolution_x(screenmode);
	int _size_y = get_screenmode_resolution_y(screenmode);
	angle_t _angle = DEG2ANGLE(-angle);

	int bar_x = (_size_y*4)/5;
	int bar_y = (_size_y/35)*7;//should be divisible by 7

	//calculating the sincos
	fix16_t sin_value;
    fix16_t cos_value;

    fix16_sincos(_angle, &sin_value, &cos_value);

    fix16_t bar_x_f = fix16_int32_from(bar_x);
    fix16_t bar_y_f = fix16_int32_from(bar_y);
	bar_x_f = fix16_div(bar_x_f,fix16_int32_from(2));
    bar_y_f = fix16_div(bar_y_f,fix16_int32_from(2));

	//patching the command
	int index = _SVIN_VDP1_ORDER_TEXT_SPRITE_1_INDEX;
	vdp1_cmdt_t * cmdlist = (vdp1_cmdt_t*)VDP1_VRAM(0);
    vdp1_cmdt_distorted_sprite_set(&cmdlist[index]);
    cmdlist[index].cmd_xa = _size_x/2 + fix16_int32_to( - fix16_mul(bar_x_f, cos_value) - fix16_mul(bar_y_f, sin_value));
    cmdlist[index].cmd_ya = _size_y/2 + fix16_int32_to( - fix16_mul(bar_x_f, sin_value) + fix16_mul(bar_y_f, cos_value));
    cmdlist[index].cmd_xb = _size_x/2 + fix16_int32_to( + fix16_mul(bar_x_f, cos_value) - fix16_mul(bar_y_f, sin_value));
    cmdlist[index].cmd_yb = _size_y/2 + fix16_int32_to( + fix16_mul(bar_x_f, sin_value) + fix16_mul(bar_y_f, cos_value));
    cmdlist[index].cmd_xc = _size_x/2 + fix16_int32_to( + fix16_mul(bar_x_f, cos_value) + fix16_mul(bar_y_f, sin_value));
    cmdlist[index].cmd_yc = _size_y/2 + fix16_int32_to( + fix16_mul(bar_x_f, sin_value) - fix16_mul(bar_y_f, cos_value));
    cmdlist[index].cmd_xd = _size_x/2 + fix16_int32_to( - fix16_mul(bar_x_f, cos_value) + fix16_mul(bar_y_f, sin_value));
    cmdlist[index].cmd_yd = _size_y/2 + fix16_int32_to( - fix16_mul(bar_x_f, sin_value) - fix16_mul(bar_y_f, cos_value));
    cmdlist[index].cmd_size=((bar_x/8)<<8)|(bar_y);

	//update text
	char buf[32];
	ClearText(0,0,FONT_QUAD_WIDTH,40);
	sprintf(buf,"Angle : %3d.%2d",(int)(angle),(((int)(100*angle))%100));
	DrawString(buf, 30, 20, FONT_GREEN);
	if (1==angle_step_div)
		sprintf(buf,"Angle increment : 1");
	else
		sprintf(buf,"Angle increment : 1/%d",angle_step_div);
	DrawString(buf, 30, 30, FONT_GREEN);

}


void draw_diagonal(_svin_screen_mode_t screenmode)
{
	//removing text
	ClearTextLayer();
	
	_svin_set_cycle_patterns_cpu();
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

	int _size_x = get_screenmode_resolution_x(screenmode);
	int _size_y = get_screenmode_resolution_y(screenmode);

	uint8_t *_pointer8 = (uint8_t *)_SVIN_NBG0_CHPNDR_START;

	//creating VDP1 tile in VRAM
	vdp1_vram_partitions_t vdp1_vram_partitions;
    vdp1_vram_partitions_get(&vdp1_vram_partitions);
	uint8_t * p8 = (uint8_t*)vdp1_vram_partitions.texture_base+0x11800;
	int bar_x = (_size_y*4)/5;
	int bar_y = (_size_y/35)*7;//should be divisible by 7
	memset(p8,FONT_WHITE*4,bar_x*bar_y);//fill with black
	for (int i=0;i<4;i++) memset(&(p8[(bar_x*bar_y*i*2)/7]),FONT_BLACK*4,bar_x*bar_y/7);//stripes
}

void videotest_diagonal(_svin_screen_mode_t screenmode)
{
	_svin_screen_mode_t curr_screenmode = screenmode;
	bool key_pressed = false;
	bool rotating = false;
	double angle = 45.0;
	int angle_step_div = 1;
	
	draw_diagonal(curr_screenmode);
	update_diagonal(screenmode,angle,angle_step_div);
	wait_for_key_unpress();
	
	int mode_display_counter = 0;

	while (1)
	{
		smpc_peripheral_process();
		get_digital_keypress_anywhere(&controller);
		if ( (controller.pressed.button.l) )
		{
			curr_screenmode = prev_screen_mode(curr_screenmode);
			update_screen_mode(curr_screenmode,false);
			update_diagonal(screenmode,angle,angle_step_div);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.r) )
		{
			curr_screenmode = next_screen_mode(curr_screenmode);
			update_screen_mode(curr_screenmode,false);
			update_diagonal(screenmode,angle,angle_step_div);
			print_screen_mode(curr_screenmode);
			wait_for_key_unpress();
			mode_display_counter=120;
		}
		else if ( (controller.pressed.button.a) || (controller.pressed.button.c) ) 
		{
			//change the checkered mode
			rotating = (rotating) ? false : true;
			wait_for_key_unpress();
		}
		else if (controller.pressed.button.y)
		{
			//decrease step divider
			angle_step_div--;
			if (angle_step_div < 1) angle_step_div = 1;
			update_diagonal(screenmode,angle,angle_step_div);
			wait_for_key_unpress();
		}
		else if (controller.pressed.button.z)
		{
			//increase step divider
			angle_step_div++;
			if (angle_step_div > 20) angle_step_div = 20;
			update_diagonal(screenmode,angle,angle_step_div);
			wait_for_key_unpress();
		}
		else if (controller.pressed.button.b)
		{
			//quit the pattern
			wait_for_key_unpress();
			update_screen_mode(screenmode,false);
			return;
		}
		vdp2_tvmd_vblank_in_wait();
		vdp2_tvmd_vblank_out_wait();
		if (mode_display_counter > 0)
		{
			mode_display_counter--;
			if (0 == mode_display_counter)
			{
				ClearTextLayer();
			}
		}
		if (rotating)
		{
			angle += 1.0/angle_step_div;
			if (angle > 360) angle -= 360;
			if (angle < -360) angle += 360;
			update_diagonal(screenmode,angle,angle_step_div);
		}
	}
}
