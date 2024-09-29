#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "svin.h"
#include <tga.h>

extern uint8_t asset_back2_bg[];
extern uint8_t asset_back2_bg_end[];
extern uint8_t asset_mascot_bg[];
extern uint8_t asset_mascot_bg_end[];
extern uint8_t asset_donna_bg[];
extern uint8_t asset_donna_bg_end[];

void draw_bg_with_expansion(_svin_screen_mode_t screenmode, bool bMascot)
{
	int stretch_amount_x = 0; //stretch amount in quads
	int stretch_amount_y = 0;
	int bSpecialMode = ( (_SVIN_SCANMODE_480P == screenmode.scanmode) && (screenmode.x_res_doubled) ) ? 1 : 0;
	//load the image with minimal resolution 320x224
	_svin_set_cycle_patterns_cpu();
	//for special mode making a copy for NBG1
	if (bSpecialMode)
	{
		_svin_background_set_from_assets(asset_back2_bg,(int)(asset_back2_bg_end-asset_back2_bg),_SVIN_NBG0_SPECIAL_PNDR_START,_SVIN_NBG0_SPECIAL_CHPNDR_START);
		_svin_background_set_from_assets(asset_back2_bg,(int)(asset_back2_bg_end-asset_back2_bg),_SVIN_NBG1_SPECIAL_PNDR_START,_SVIN_NBG1_SPECIAL_CHPNDR_START);
	}
	else
		_svin_background_set_from_assets(asset_back2_bg,(int)(asset_back2_bg_end-asset_back2_bg),_SVIN_NBG0_PNDR_START,_SVIN_NBG0_CHPNDR_START);
	_svin_set_cycle_patterns_nbg(screenmode);

	//now mess with VDP2 tiles to stretch the image among the screen accordingly
	//the image is broken into 4 tile blocks as a 2x2 table, 256+64(32+8) horisontally, 128+96(16+12) vertically
	//left table elements are stretched (rightmost tile copied) right to fit
	//right table elements are shifted accordingly
	//top elements are stretched
	//bottom elements are shifted
	//to prevent data corruption, we shift first and stretch later
	//step 1 - deciding shift and stretch amount
	switch (screenmode.x_res)
	{
		case _SVIN_X_RESOLUTION_320:
			if (screenmode.x_res_doubled)
				stretch_amount_x = 40;
			else
				stretch_amount_x = 0;
			break;
		case _SVIN_X_RESOLUTION_352:
			if (screenmode.x_res_doubled)
				stretch_amount_x = 48;
			else
				stretch_amount_x = 4;
			break;
	}
	switch (screenmode.y_res)
	{
		case VDP2_TVMD_VERT_224:
			if ( (_SVIN_SCANMODE_480I == screenmode.scanmode) || (_SVIN_SCANMODE_480P == screenmode.scanmode) )
				stretch_amount_y = 28;
			else
				stretch_amount_y = 0;
			break;
		case VDP2_TVMD_VERT_240:
			if ( (_SVIN_SCANMODE_480I == screenmode.scanmode) || (_SVIN_SCANMODE_480P == screenmode.scanmode) )
				stretch_amount_y = 32;
			else
				stretch_amount_y = 2;
			break;
		case VDP2_TVMD_VERT_256:
			if ( (_SVIN_SCANMODE_480I == screenmode.scanmode) || (_SVIN_SCANMODE_480P == screenmode.scanmode) )
				stretch_amount_y = 36;
			else
				stretch_amount_y = 4;
			break;
	}

	int copies = 1;
	if (bSpecialMode) copies = 2;
	int *_pointer32[2];
	int vram_offset[2];
	_pointer32[0] = (int *)_SVIN_NBG0_PNDR_START;
	vram_offset[0] = _SVIN_NBG0_CHPNDR_START - VDP2_VRAM_ADDR(0,0);
	if (bSpecialMode)
	{
		_pointer32[0] = (int *)_SVIN_NBG0_SPECIAL_PNDR_START;
		_pointer32[1] = (int *)_SVIN_NBG1_SPECIAL_PNDR_START;
		vram_offset[0] = _SVIN_NBG0_SPECIAL_CHPNDR_START - VDP2_VRAM_ADDR(0,0);
		vram_offset[1] = _SVIN_NBG1_SPECIAL_CHPNDR_START - VDP2_VRAM_ADDR(0,0);
	}

	//step 2 - shifting x
	_svin_set_cycle_patterns_cpu();
	for (int copy = 0; copy < copies; copy++)
	{
		for (int y = 0; y < 30 ; y++)
		{
			for (int x = 7; x >= 0; x--)
			{
				if (stretch_amount_x < 40)   
					_pointer32[copy][y*64+32+x+stretch_amount_x] = _pointer32[copy][y*64+32+x];
				else
					_pointer32[copy][64*63 + y*64+32+x+stretch_amount_x] = _pointer32[copy][y*64+32+x];
			}
		}
	}

	//step 3 - stretching x
	for (int copy = 0; copy < copies; copy++)
	{
		for (int y = 0; y < 30 ; y++)
		{
			if (stretch_amount_x < 40) 
			{
				for (int x = 0; x < stretch_amount_x; x++)
					_pointer32[copy][y*64+32+x] = _pointer32[copy][y*64+31];
			}
			else
			{
				for (int x = 0; x < 32; x++)
					_pointer32[copy][y*64+32+x] = _pointer32[copy][y*64+31];
				for (int x = 0; x <= (stretch_amount_x-32); x++)
					_pointer32[copy][64*64 + y*64+x] = _pointer32[copy][y*64+31];			
			}
		}
	}

	//step 4 - shifting y
	for (int copy = 0; copy < copies; copy++)
	{
		if (stretch_amount_x < 40) 
		{	
			for (int x = 0; x < 40 + stretch_amount_x ; x++)
			{
				for (int y = 27; y >= 16; y--)
					_pointer32[copy][(y+stretch_amount_y)*64+x] = _pointer32[copy][y*64+x];
			}
		}
		else
		{
			for (int x = 0; x < 64 ; x++)
			{
				for (int y = 27; y >= 16; y--)
					_pointer32[copy][(y+stretch_amount_y)*64+x] = _pointer32[copy][y*64+x];
			}
			for (int x = 0; x < stretch_amount_x-24 ; x++)
			{
				for (int y = 27; y >= 16; y--)
					_pointer32[copy][64*64 + (y+stretch_amount_y)*64+x] = _pointer32[copy][64*64 + y*64+x];
			}
		}
	}

	//step 5 - stretching y
	for (int copy = 0; copy < copies; copy++)
	{
		if (stretch_amount_x < 40) 
		{	
			for (int x = 0; x < 40 + stretch_amount_x ; x++)
			{
				for (int y = 0; y < stretch_amount_y; y++)
					_pointer32[copy][(y+16)*64+x] = _pointer32[copy][15*64+x];
			}
		}
		else
		{
			for (int x = 0; x < 64 ; x++)
			{
				for (int y = 0; y < stretch_amount_y; y++)
					_pointer32[copy][(y+16)*64+x] = _pointer32[copy][15*64+x];
			}
			for (int x = 0; x < stretch_amount_x-24 ; x++)
			{
				for (int y = 0; y < stretch_amount_y; y++)
					_pointer32[copy][64*64+(y+16)*64+x] = _pointer32[copy][64*64+15*64+x];
			}
		}
	}

	if (bMascot)
	{
		//select mascot location
		int mascot_x = (screenmode.x_res == _SVIN_X_RESOLUTION_320) ? 28 : 32;
		mascot_x =  (screenmode.x_res_doubled) ? (mascot_x+6)*2 : mascot_x;
		int mascot_y = (screenmode.y_res == VDP2_TVMD_VERT_224) ? 9 :
							(screenmode.y_res == VDP2_TVMD_VERT_240) ? 11 : 13;
		mascot_y = ( (_SVIN_SCANMODE_480I == screenmode.scanmode) || (_SVIN_SCANMODE_480P == screenmode.scanmode) ) ? (mascot_y+9)*2 : mascot_y;
		//copy mascot patterns, adding 128 to each color
		//decompress
		int * p32 = (int *)(&asset_mascot_bg[2048]);
		int compressed_size = p32[1];
		int compressed_size_sectors = ((compressed_size-1)/2048)+1;
		if (bSpecialMode)
		{
			bcl_lz_decompress(&(asset_mascot_bg[2048+8]),(char*)(_SVIN_NBG0_SPECIAL_CHPNDR_START+0x12C00),compressed_size);
			bcl_lz_decompress(&(asset_mascot_bg[2048+8]),(char*)(_SVIN_NBG1_SPECIAL_CHPNDR_START+0x12C00),compressed_size);
		}
		else
			bcl_lz_decompress(&(asset_mascot_bg[2048+8]),(char*)(_SVIN_NBG0_CHPNDR_START+0x12C00),compressed_size);
		//fill mascot pattern names 
		int size_x = 7;
		for (int copy = 0; copy < copies; copy++)
		{
			for (int y=mascot_y; y<mascot_y+13; y++)
			{
				for (int x=mascot_x; x<mascot_x+size_x; x++)
				{
					if (x>=64)
					{
						_pointer32[copy][64*64+y*64+x-64] = 0x00300000  + (0x12C00/32)+(y-mascot_y)*size_x*2+(x-mascot_x)*2 + vram_offset[copy]/32; //palette 3, transparency on
					}
					else
					{
						_pointer32[copy][y*64+x] = 0x00300000  + (0x12C00/32)+(y-mascot_y)*size_x*2+(x-mascot_x)*2 + vram_offset[copy]/32; //palette 3, transparency on
					}
				}
			}
		}
		//set palette, using palette 3 for mascot
		//palette in file is 24-bit, setting it color-by-color
		rgb888_t _color  = {0,0,0,0};
		for (int i = 0; i<256; i++)
		{
			_color.r = asset_mascot_bg[2048*compressed_size_sectors+2048+i*3+0];//palette[i*3+0];
			_color.g = asset_mascot_bg[2048*compressed_size_sectors+2048+i*3+1];//palette[i*3+1];
			_color.b = asset_mascot_bg[2048*compressed_size_sectors+2048+i*3+2];//palette[i*3+2];
			_svin_set_palette_part(3, &_color, i, i);
		}
	}

    _svin_set_cycle_patterns_nbg(screenmode);
}

void draw_bg_donna(_svin_screen_mode_t screenmode)
{
	int bSpecialMode = ( (_SVIN_SCANMODE_480P == screenmode.scanmode) && (screenmode.x_res_doubled) ) ? 1 : 0;
	//loading the image with maximal resolution 704x512
	_svin_set_cycle_patterns_cpu();
	//for special mode making a copy for NBG1
	if (bSpecialMode)
	{
		_svin_background_set_from_assets(asset_donna_bg,(int)(asset_donna_bg_end-asset_donna_bg),_SVIN_NBG0_SPECIAL_PNDR_START,_SVIN_NBG0_SPECIAL_CHPNDR_START);
		_svin_background_set_from_assets(asset_donna_bg,(int)(asset_donna_bg_end-asset_donna_bg),_SVIN_NBG1_SPECIAL_PNDR_START,_SVIN_NBG1_SPECIAL_CHPNDR_START);
	}
	else
		_svin_background_set_from_assets(asset_donna_bg,(int)(asset_donna_bg_end-asset_donna_bg),_SVIN_NBG0_PNDR_START,_SVIN_NBG0_CHPNDR_START);
	
	_svin_set_cycle_patterns_nbg(screenmode);

	//now shift the VDP2 tiles according to the current screenmode
	int x_shift =  (screenmode.x_res_doubled) ? 0 :
					(screenmode.x_res == _SVIN_X_RESOLUTION_320) ? 30 : 32;
	int y_shift = ( (_SVIN_SCANMODE_480I == screenmode.scanmode) || (_SVIN_SCANMODE_480P == screenmode.scanmode) )  ? 0 : 12;
	int copies = 1;
	if (bSpecialMode) copies = 2;
	int *_pointer32[2];
	_pointer32[0] = (int *)_SVIN_NBG0_PNDR_START;
	if (bSpecialMode)
	{
		_pointer32[0] = (int *)_SVIN_NBG0_SPECIAL_PNDR_START;
		_pointer32[1] = (int *)_SVIN_NBG1_SPECIAL_PNDR_START;
	}
	for (int copy = 0; copy < copies; copy++)
	{
		//shifting plane 0
		for (unsigned int y = 0; y<64; y++)
		{
			for (unsigned int x = 0; x<64 ; x++)
			{
				if (x+x_shift < 64) 
					_pointer32[copy][y*64+x] = _pointer32[copy][(y+y_shift)*64+(x+x_shift)]; //palette 0, transparency on
				else
					_pointer32[copy][y*64+x] = _pointer32[copy][(y+y_shift)*64+64*63+(x+x_shift)]; //palette 0, transparency on
			}
		}
		//shifting plane 1
		for (unsigned int y = 0; y<64; y++)
		{
			for (unsigned int x = 0; x<64 ; x++)
			{
				_pointer32[copy][y*64+64*63+x] = _pointer32[copy][(y+y_shift)*64+64*63+(x+x_shift)]; //palette 0, transparency on
			}
		}
	}
}