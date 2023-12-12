#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "svin.h"
#include "ire.h"

void draw_smpte(_svin_screen_mode_t screenmode, bool bIRE100)
{
	//removing text
	ClearTextLayer();
	
	_svin_set_cycle_patterns_cpu();
	//add colors to palette
	uint8_t IRE_top = (bIRE100) ? Get_IRE_Level(100.0) : Get_IRE_Level(75);
	uint8_t IRE_bot = Get_IRE_Level(7.5);
	rgb888_t Color = {0,0,0,0};
	Color.r = IRE_top;
	Color.g = IRE_top;
	Color.b = IRE_top;	
	_svin_set_palette_part(2,&Color,1,1); //palette 2 color 1 = IRE white
	Color.b = IRE_bot;
	_svin_set_palette_part(2,&Color,2,2); //palette 2 color 2 = IRE yellow
	Color.r = IRE_bot;
	Color.b = IRE_top;
	_svin_set_palette_part(2,&Color,3,3); //palette 2 color 3 = IRE cyan
	Color.b = IRE_bot;
	_svin_set_palette_part(2,&Color,4,4); //palette 2 color 4 = IRE green
	Color.r = IRE_top;
	Color.g = IRE_bot;
	Color.b = IRE_top;	
	_svin_set_palette_part(2,&Color,5,5); //palette 2 color 5 = IRE pink
	Color.b = IRE_bot;	
	_svin_set_palette_part(2,&Color,6,6); //palette 2 color 6 = IRE red
	Color.r = IRE_bot;
	Color.b = IRE_top;
	_svin_set_palette_part(2,&Color,7,7); //palette 2 color 7 = IRE blue
	Color.b = IRE_bot;
	_svin_set_palette_part(2,&Color,8,8); //palette 2 color 8 = IRE black
	Color.r = IRE_bot;
	Color.g = Get_IRE_Level(29.2);//33;
	Color.b = Get_IRE_Level(44.2);//74;	
	_svin_set_palette_part(2,&Color,9,9); //palette 2 color 9 = -I
	Color.r = Get_IRE_Level(100.0);
	Color.g = Get_IRE_Level(100.0);
	Color.b = Get_IRE_Level(100.0);	
	_svin_set_palette_part(2,&Color,10,10); //palette 2 color 10 = white
	Color.r = Get_IRE_Level(30.0);//49;
	Color.g = IRE_bot;
	Color.b = Get_IRE_Level(49.2);//107;	
	_svin_set_palette_part(2,&Color,11,11); //palette 2 color 11 = +Q
	Color.r = Get_IRE_Level(3.5);
	Color.g = Get_IRE_Level(3.5);
	Color.b = Get_IRE_Level(3.5);	
	_svin_set_palette_part(2,&Color,12,12); //palette 2 color 12 = black-4
	Color.r = Get_IRE_Level(11.5);
	Color.g = Get_IRE_Level(11.5);
	Color.b = Get_IRE_Level(11.5);	
	_svin_set_palette_part(2,&Color,13,13); //palette 2 color 13 = black+4

	//using tiled mode is a real PITA, so we switch to BMP mode now
	//setup nbg0
    struct vdp2_scrn_bitmap_format format;
    memset(&format, 0x00, sizeof(format));
    format.scroll_screen = VDP2_SCRN_NBG0;
    format.ccc = VDP2_SCRN_CCC_PALETTE_16;
    format.bitmap_size = VDP2_SCRN_BITMAP_SIZE_1024X512;
    format.palette_base = 0x800;
    format.bitmap_base = _SVIN_NBG0_CHPNDR_START;
    vdp2_scrn_bitmap_format_set(&format);

	int _size_x = (_SVIN_X_RESOLUTION_320 == screenmode.x_res) ? 320: 352;
	if (screenmode.x_res_doubled) _size_x*=2;
	int _size_y = (_SVIN_Y_RESOLUTION_224 == screenmode.y_res) ? 224 : 
					(_SVIN_Y_RESOLUTION_240 == screenmode.y_res) ? 240 : 256;
	if (_SVIN_SCANMODE_480I == screenmode.scanmode)
		_size_y *= 2;

	uint8_t *_pointer8 = (uint8_t *)_SVIN_NBG0_CHPNDR_START;

	//draw top bars
	for (int i=0;i<7;i++)
	{
		uint8_t color = i+1;
		for (int y=0;y<(_size_y*67)/100;y++)
		{
			int line_start = y*512;
			for (int x=(_size_x*i)/7;x<(_size_x*(i+1))/7;x++)
			{
				if (x%2)
				{
					_pointer8[line_start+x/2] &= 0xF0;
					_pointer8[line_start+x/2] |= color;
				}
				else
				{
					_pointer8[line_start+x/2] &= 0x0F;
					_pointer8[line_start+x/2] |= color<<4;

				}
			}
		}
	}
	//draw mid bars
	for (int i=0;i<7;i++)
	{
		uint8_t icolor = (i%2) ? 8 : 7-i;
		for (int y=(_size_y*67)/100;y<(_size_y*75)/100;y++)
		{
			int line_start = y*512;
			for (int x=(_size_x*i)/7;x<(_size_x*(i+1))/7;x++)
			{
				if (x%2)
				{
					_pointer8[line_start+x/2] &= 0xF0;
					_pointer8[line_start+x/2] |= icolor;
				}
				else
				{
					_pointer8[line_start+x/2] &= 0x0F;
					_pointer8[line_start+x/2] |= icolor<<4;

				}
			}
		}
	}
	//draw 3 left bottom bars
	for (int i=0;i<3;i++)
	{
		uint8_t icolor = i+9;
		for (int y=(_size_y*75)/100;y<_size_y;y++)
		{
			int line_start = y*512;
			for (int x=(_size_x*i*5)/28;x<(_size_x*(i+1)*5)/28;x++)
			{
				if (x%2)
				{
					_pointer8[line_start+x/2] &= 0xF0;
					_pointer8[line_start+x/2] |= icolor;
				}
				else
				{
					_pointer8[line_start+x/2] &= 0x0F;
					_pointer8[line_start+x/2] |= icolor<<4;

				}
			}
		}
	}
	//draw bottom black
	for (int y=(_size_y*75)/100;y<_size_y;y++)
	{
		int line_start = y*512;
		for (int x=(_size_x*15)/28;x<_size_x;x++)
		{
			if (x%2)
			{
				_pointer8[line_start+x/2] &= 0xF0;
				_pointer8[line_start+x/2] |= 8;
			}
			else
			{
				_pointer8[line_start+x/2] &= 0x0F;
				_pointer8[line_start+x/2] |= 8<<4;
			}
		}
	}
	//draw bottom black - 4
	for (int y=(_size_y*75)/100;y<_size_y;y++)
	{
		int line_start = y*512;
		for (int x=(_size_x*5)/7;x<(_size_x*16)/21;x++)
		{
			if (x%2)
			{
				_pointer8[line_start+x/2] &= 0xF0;
				_pointer8[line_start+x/2] |= 12;
			}
			else
			{
				_pointer8[line_start+x/2] &= 0x0F;
				_pointer8[line_start+x/2] |= 12<<4;
			}
		}
	}
	//draw bottom black + 4
	for (int y=(_size_y*75)/100;y<_size_y;y++)
	{
		int line_start = y*512;
		for (int x=(_size_x*17)/21;x<(_size_x*6)/7;x++)
		{
			if (x%2)
			{
				_pointer8[line_start+x/2] &= 0xF0;
				_pointer8[line_start+x/2] |= 13;
			}
			else
			{
				_pointer8[line_start+x/2] &= 0x0F;
				_pointer8[line_start+x/2] |= 13<<4;
			}
		}
	}

	//vdp2_tvmd_display_set();
    //vdp2_sync();

	//create single-color tiles for each color, tiles 1-12
	/*uint32_t *_pointer32 = (uint32_t *)_SVIN_NBG0_CHPNDR_START;
	uint8_t *_pointer8 = (uint8_t *)_pointer32;
	for (int i=1; i<13; i++)
		for (int j=0; j<16; j++)
			_pointer32[i*16+j] = 0x01010101*i;
	//extra tiles for top 7 bars, tiles 13-18
	int x_size = (_SVIN_X_RESOLUTION_320 == screenmode.x_res) ? 320: 352;
	if (screenmode.x_res_doubled) x_size*=2;
	int remaining[6];
	for (int i=0; i<6; i++)
	{
		remaining[i] = ((x_size*(i+1))/7) % 8;
		for (int j=0; j<8; j++)
			for (int i=0; i<remaining[i]; i++)
				_pointer8[(i+13)*64+j*8+i] = i;
		for (int j=0; j<8; j++)
			for (int i=remaining[i]; i<8; i++)
				_pointer8[(i+13)*64+j*8+i] = i+1;
	}
	//extra tiles for mid 7 bars, tiles 19-24
	for (int i=0; i<6; i++)
	{
		for (int j=0; j<8; j++)
			for (int i=0; i<remaining[i]; i++)
				_pointer8[(i+19)*64+j*8+i] = (i%2 == 0) ? 6-i : 0;
		for (int j=0; j<8; j++)
			for (int i=remaining[i]; i<8; i++)
				_pointer8[(i+19)*64+j*8+i] = (i%2 == 0) ? 0 : 6-i;
	}

	//fill everything with pitch-black
    _pointer32 = (int *)_SVIN_NBG0_PNDR_START;
    for (unsigned int i = 0; i < _SVIN_NBG0_PNDR_SIZE / sizeof(int); i++)
    {
        _pointer32[i] = 0x00200002; //palette 2, transparency on, black from red gradient
    }
	//draw top bars depending on screen mode
	_pointer32 = (int *)_SVIN_NBG0_PNDR_START;
	int bar_size = (_SVIN_X_RESOLUTION_320 == screenmode.x_res) ? 6 : 10;
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
	}*/
	_svin_set_cycle_patterns_nbg();
}
