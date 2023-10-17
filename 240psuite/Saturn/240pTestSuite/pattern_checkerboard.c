#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "svin.h"

void draw_checkerboard(_svin_screen_mode_t screenmode)
{
	//removing text
	ClearTextLayer();
	
	_svin_set_cycle_patterns_cpu();
	//add colors to palette
	rgb888_t Color = {0,0,0,0};
	_svin_set_palette_part(2,&Color,1,1); //palette 2 color 1 = black
	Color.r = 255;
	Color.g = 255;
	Color.b = 255;	
	_svin_set_palette_part(2,&Color,2,2); //palette 2 color 2 = white
	//create checkerboard tile at index 0
	int *_pointer32 = (int *)_SVIN_NBG0_CHPNDR_START;
	for (int i=0; i<4; i++)
	{
		_pointer32[i*4  ] = 0x01020102;
		_pointer32[i*4+1] = 0x01020102;
		_pointer32[i*4+2] = 0x02010201;
		_pointer32[i*4+3] = 0x02010201;
	}
	//fill everything with this tile
    _pointer32 = (int *)_SVIN_NBG0_PNDR_START;
    for (unsigned int i = 0; i < _SVIN_NBG0_PNDR_SIZE / sizeof(int); i++)
    //for (unsigned int i = 0; i < 64; i++)
    {
        //_pointer32[i] = 0x00000000 + _SVIN_NBG0_CHPNDR_SPECIALS_INDEX; //palette 0, transparency on
        _pointer32[i] = 0x00200000; //palette 2, transparency on
    }
	_svin_set_cycle_patterns_nbg();
}