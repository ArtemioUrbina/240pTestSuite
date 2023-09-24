#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "svin.h"

void draw_colorbars(_svin_screen_mode_t screenmode)
{
	//removing text
	ClearTextLayer();
	
	_svin_set_cycle_patterns_cpu();
	//add colors to palette
	uint16_t *_pointer16 = (uint16_t *)VDP2_CRAM(0x200*2);//palette 2
	for (int i=0;i<32;i++) 
	{
		_pointer16[1+i] = 0x0001*i; //red
		_pointer16[33+i] = 0x0020*i; //green
		_pointer16[65+i] = 0x0400*i; //blue
		_pointer16[97+i] = 0x0421*i; //red
	}
	//create single-color tiles for each color, 128 tiles in total
	int *_pointer32 = (int *)_SVIN_NBG0_CHPNDR_START;
	for (int i=1; i<129; i++)
		for (int j=0; j<16; j++)
			_pointer32[i*16+j] = 0x01010101*i;
	//fill everything with pitch-black
    _pointer32 = (int *)_SVIN_NBG0_PNDR_START;
    for (unsigned int i = 0; i < _SVIN_NBG0_PNDR_SIZE / sizeof(int); i++)
    {
        _pointer32[i] = 0x00200002; //palette 2, transparency on, black from red gradient
    }
	//draw bars depending on screen mode
	_pointer32 = (int *)_SVIN_NBG0_PNDR_START;
	int offset = (_SVIN_X_RESOLUTION_320 == screenmode.x_res) ? 6 : 10;
	int y_ratio = (_SVIN_SCANMODE_480I == screenmode.scanmode) ? 2 : 1;
    if (screenmode.x_res_doubled)
	{
		//high-x-res mode
		for (int i = 0;i<64;i++)
		{
			for (int j=0;j<4*y_ratio;j++)
			{
				if (offset+i < 64)
				{
					_pointer32[64*(3+j) + offset + i] = 0x00200000 | (i/2+1)*2; //palette 2, red
					_pointer32[64*(3+5*y_ratio+j) + offset + i] = 0x00200000 | (i/2+33)*2; //palette 2, green
					_pointer32[64*(3+10*y_ratio+j) + offset + i] = 0x00200000 | (i/2+65)*2; //palette 2, blue
					_pointer32[64*(3+15*y_ratio+j) + offset + i] = 0x00200000 | (i/2+97)*2; //palette 2, black
				}
				else
				{
					_pointer32[64*63+64*(3+j) + offset + i] = 0x00200000 | (i/2+1)*2; //palette 2, red
					_pointer32[64*63+64*(3+5*y_ratio+j) + offset + i] = 0x00200000 | (i/2+33)*2; //palette 2, green
					_pointer32[64*63+64*(3+10*y_ratio+j) + offset + i] = 0x00200000 | (i/2+65)*2; //palette 2, blue
					_pointer32[64*63+64*(3+15*y_ratio+j) + offset + i] = 0x00200000 | (i/2+97)*2; //palette 2, black
				}
			}
		}	
	}
	else
	{
		//normal x mode
		for (int i = 0;i<32;i++)
		{
			for (int j=0;j<4*y_ratio;j++)
			{
				_pointer32[64*(3+j) + offset + i] = 0x00200000 | (i+1)*2; //palette 2, red
				_pointer32[64*(3+5*y_ratio+j) + offset + i] = 0x00200000 | (i+33)*2; //palette 2, green
				_pointer32[64*(3+10*y_ratio+j) + offset + i] = 0x00200000 | (i+65)*2; //palette 2, blue
				_pointer32[64*(3+15*y_ratio+j) + offset + i] = 0x00200000 | (i+97)*2; //palette 2, black
			}
		}	
	}
	_svin_set_cycle_patterns_nbg();
	//draw text
	DrawString("RED", 10, 40-10*y_ratio, FONT_RED);
	DrawString("GREEN", 10, 40-10*y_ratio+40, FONT_GREEN);
	DrawString("BLUE", 10, 40-10*y_ratio+80, FONT_CYAN); 
	DrawString("WHITE", 10, 40-10*y_ratio+120, FONT_WHITE); 
}
