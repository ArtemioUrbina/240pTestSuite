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

#include "tests.h"
#include "utils.h"

void DropShadowTest()
{
	int end = 0, show = 1;
	int x = 0, y = 0, xlast = 0, ylast = 0;
	sprite_t *back = NULL, *shadow = NULL;
	struct controller_data keys;
	
	back = LoadImage("/motoko.bin");
	shadow = LoadImage("/shadow.bin");
    while(!end)
    {	
		GetDisplay();
		
		drawImageDMA(0, 0, back);
			
		if(show)
		{
			rdp_texture_start();
			rdp_DrawImage(x, y, shadow);
			rdp_end();
		}
		
		show = !show;
		
		WaitVsync();
		
		xlast = x;
		ylast = y;

        controller_scan();
		keys = Controller_ButtonsHeld();

		if(keys.c[0].up)
			y--;

		if(keys.c[0].down)
			y++;
		
		if(y > dH - shadow->height)
			y = dH - shadow->height;
		if(y < 0)
			y = 0;
			
		if(keys.c[0].left)
			x--;

		if(keys.c[0].right)
			x++;
			
		if(x > dW - shadow->width)
			x = dW - shadow->width;
		if(x < 0)
			x = 0;
		
		keys = Controller_ButtonsDown();
		if(keys.c[0].B)
			end = 1;
		if(keys.c[0].A)
			show = !show;
	}
	FreeImage(&back);
	FreeImage(&shadow);
}

void DrawCheckerboard()
{
	int end = 0, type = 0;
	sprite_t *pos = NULL, *neg = NULL;
	struct controller_data keys;
	
	if(!pos)
		pos = LoadImage("/checkpos.bin");
	if(!neg)
		neg = LoadImage("/checkneg.bin");
			
    while(!end)
    {	
		GetDisplay();

		rdp_texture_start();
		if(type)
			rdp_FillScreenWithTexture(pos);
		else
			rdp_FillScreenWithTexture(neg);
		rdp_end();
		
		WaitVsync();
		
		controller_scan();
		keys = Controller_ButtonsDown();
		
		if(keys.c[0].A)
			type = !type;
			
		if(keys.c[0].B)
			end = 1;
	}
	
	FreeImage(&pos);
	FreeImage(&neg);
}


