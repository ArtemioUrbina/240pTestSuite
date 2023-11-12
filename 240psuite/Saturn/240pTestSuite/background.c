#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "svin.h"

extern uint8_t asset_back2_bg[];
extern uint8_t asset_back2_bg_end[];

void draw_bg_with_expansion(_svin_screen_mode_t screenmode)
{
	int stretch_amount_x = 0; //stretch amount in quads
	int stretch_amount_y = 0;
	//load the image with minimal resolution 320x224
	//_svin_background_set_no_filelist("BACK2.BG");
	_svin_background_set_from_assets(asset_back2_bg,(int)(asset_back2_bg_end-asset_back2_bg));

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
		case _SVIN_Y_RESOLUTION_224:
			if (screenmode.scanmode == _SVIN_SCANMODE_480I)
				stretch_amount_y = 28;
			else
				stretch_amount_y = 0;
			break;
		case _SVIN_Y_RESOLUTION_240:
			if (screenmode.scanmode == _SVIN_SCANMODE_480I)
				stretch_amount_y = 32;
			else
				stretch_amount_y = 2;
			break;
		case _SVIN_Y_RESOLUTION_256:
			if (screenmode.scanmode == _SVIN_SCANMODE_480I)
				stretch_amount_y = 36;
			else
				stretch_amount_y = 4;
			break;
	}

	//step 2 - shifting x
	_svin_set_cycle_patterns_cpu();
    int *_pointer32 = (int *)_SVIN_NBG0_PNDR_START;
    for (int y = 0; y < 30 ; y++)
    {
    	for (int x = 7; x >= 0; x--)
        {
			if (stretch_amount_x < 40)   
            	_pointer32[y*64+32+x+stretch_amount_x] = _pointer32[y*64+32+x];
			else
            	_pointer32[64*63 + y*64+32+x+stretch_amount_x] = _pointer32[y*64+32+x];
        }
    }

	//step 3 - stretching x
    for (int y = 0; y < 30 ; y++)
    {
		if (stretch_amount_x < 40) 
		{
			for (int x = 0; x < stretch_amount_x; x++)
				_pointer32[y*64+32+x] = _pointer32[y*64+31];
		}
		else
		{
			for (int x = 0; x < 32; x++)
				_pointer32[y*64+32+x] = _pointer32[y*64+31];
			for (int x = 0; x <= (stretch_amount_x-32); x++)
				_pointer32[64*64 + y*64+x] = _pointer32[y*64+31];			
		}
    }

	//step 4 - shifting y
	if (stretch_amount_x < 40) 
	{	
		for (int x = 0; x < 40 + stretch_amount_x ; x++)
		{
			for (int y = 27; y >= 16; y--)
				_pointer32[(y+stretch_amount_y)*64+x] = _pointer32[y*64+x];
		}
	}
	else
	{
		for (int x = 0; x < 64 ; x++)
		{
			for (int y = 27; y >= 16; y--)
				_pointer32[(y+stretch_amount_y)*64+x] = _pointer32[y*64+x];
		}
		for (int x = 0; x < stretch_amount_x-24 ; x++)
		{
			for (int y = 27; y >= 16; y--)
				_pointer32[64*64 + (y+stretch_amount_y)*64+x] = _pointer32[64*64 + y*64+x];
		}
	}

	//step 5 - stretching y
	if (stretch_amount_x < 40) 
	{	
		for (int x = 0; x < 40 + stretch_amount_x ; x++)
		{
			for (int y = 0; y < stretch_amount_y; y++)
				_pointer32[(y+16)*64+x] = _pointer32[15*64+x];
		}
	}
	else
	{
		for (int x = 0; x < 64 ; x++)
		{
			for (int y = 0; y < stretch_amount_y; y++)
				_pointer32[(y+16)*64+x] = _pointer32[15*64+x];
		}
		for (int x = 0; x < stretch_amount_x-24 ; x++)
		{
			for (int y = 0; y < stretch_amount_y; y++)
				_pointer32[64*64+(y+16)*64+x] = _pointer32[64*64+15*64+x];
		}
	}

    _svin_set_cycle_patterns_nbg();
}