/* 
 * 240p Test Suite for Nintendo 64
 * Copyright (C)2016 Artemio Urbina
 *
 * This file is part of the 240p Test Suite
 *
 * The 240p Test Suite is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The 240p Test Suite is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	02111-1307	USA
 */

#include "patterns.h"

void DrawPLUGE()
{
	int end = 0;
	sprite_t *back = NULL;
	struct controller_data keys;
	
	back = LoadImage("/pluge.bin");
	if(!back)
		return;
		
    while(!end)
    {	
		GetDisplay();
		
		drawImageDMA(0, 0, back);
		
		WaitVsync();
		
		controller_scan();
		keys = get_keys_down();
		if(keys.c[0].B)
			end = 1;
	}
	
	FreeImage(&back);
}

void DrawColorBars()
{
	int end = 0, type;
	sprite_t *back[4];
	struct controller_data keys;
	
	for(type = 0; type < 4; type++)
		back[type] = NULL;
		
	type = 1;
	
	back[0] = LoadImage("/colorlow.bin");			
	back[1] = LoadImage("/color.bin");					
	back[2] = LoadImage("/colorhigh.bin");		
	back[3] = LoadImage("/color_grid.bin");
	
	current_bitdepth = DEPTH_32_BPP;
	set_video();
	
    while(!end)
    {	
		GetDisplay();
		
		drawImageDMA(0, 0, back[type]);
		
		WaitVsync();
		
		controller_scan();
		keys = get_keys_down();
		if(keys.c[0].B)
			end = 1;
			
		if(keys.c[0].left)
			type --;
		if(keys.c[0].right)
			type ++;
			
		if(type < 0)
			type = 0;
		if(type > 3)
			type = 3;
	}
	
	for(type = 0; type < 4; type++)
		FreeImage(&back[type]);
		
	current_bitdepth = DEPTH_16_BPP;
	set_video();
}
