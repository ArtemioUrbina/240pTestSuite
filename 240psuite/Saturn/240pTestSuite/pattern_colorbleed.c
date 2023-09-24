#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "svin.h"

void draw_colorbleed(_svin_screen_mode_t screenmode, bool checkered)
{
	//removing text
	ClearTextLayer();
	
	_svin_set_cycle_patterns_cpu();
	//add colors to palette
	uint16_t *_pointer16 = (uint16_t *)VDP2_CRAM(0x200*2);//palette 2
	_pointer16[1] = 0x001F; //red
	_pointer16[2] = 0x03E0; //green
	_pointer16[3] = 0x7C00; //blue
	_pointer16[4] = 0x7FFF; //white
	//create single-color tiles for each color, 4 tiles in total
	int *_pointer32 = (int *)_SVIN_NBG0_CHPNDR_START;
	if (checkered)
	{
		for (int i=1; i<5; i++)
			for (int j=0; j<16; j+=4)
			{
				_pointer32[i*16+j] = 0x01000100*i;
				_pointer32[i*16+j+1] = 0x01000100*i;
				_pointer32[i*16+j+2] = 0x00010001*i;
				_pointer32[i*16+j+3] = 0x00010001*i;
			}
	}
	else
	{
		for (int i=1; i<5; i++)
			for (int j=0; j<16; j++)
				_pointer32[i*16+j] = 0x01000100*i;
	}
	//black tile
	for (int j=0; j<16; j++)
		_pointer32[j] = 0x0;
	//fill everything with pitch-black
    _pointer32 = (int *)_SVIN_NBG0_PNDR_START;
    for (unsigned int i = 0; i < _SVIN_NBG0_PNDR_SIZE / sizeof(int); i++)
    {
        _pointer32[i] = 0x00200000; //palette 2, transparency on, black
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
					_pointer32[64*(3+j) + offset + i] = 0x00200000 | 2; //palette 2, red
					_pointer32[64*(3+5*y_ratio+j) + offset + i] = 0x00200000 | 4; //palette 2, green
					_pointer32[64*(3+10*y_ratio+j) + offset + i] = 0x00200000 | 6; //palette 2, blue
					_pointer32[64*(3+15*y_ratio+j) + offset + i] = 0x00200000 | 8; //palette 2, white
				}
				else
				{
					_pointer32[64*63+64*(3+j) + offset + i] = 0x00200000 | 2; //palette 2, red
					_pointer32[64*63+64*(3+5*y_ratio+j) + offset + i] = 0x00200000 | 4; //palette 2, green
					_pointer32[64*63+64*(3+10*y_ratio+j) + offset + i] = 0x00200000 | 6; //palette 2, blue
					_pointer32[64*63+64*(3+15*y_ratio+j) + offset + i] = 0x00200000 | 8; //palette 2, white
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
				_pointer32[64*(3+j) + offset + i] = 0x00200000 | 2; //palette 2, red
				_pointer32[64*(3+5*y_ratio+j) + offset + i] = 0x00200000 | 4; //palette 2, green
				_pointer32[64*(3+10*y_ratio+j) + offset + i] = 0x00200000 | 6; //palette 2, blue
				_pointer32[64*(3+15*y_ratio+j) + offset + i] = 0x00200000 | 8; //palette 2, black
			}
		}	
	}
	_svin_set_cycle_patterns_nbg();
}
