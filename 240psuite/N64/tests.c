/* 
 * 240p Test Suite for Nintendo 64
 * Copyright (C)2024 Artemio Urbina
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
 
#include "system.h"
#include "video.h"
#include "font.h"
#include "image.h"
#include "controller.h"
#include "tests.h"
 
void drawScroll()
{
	int end = 0, x = 0, y = 0, currentframe = 0;
	int speed = 1, acc = -1, pause = 0, vertical = 0;
	sprite_t *sonicback, *overlay;	
	joypad_buttons_t keys;
	
	sonicback = sprite_load("rom:/sonicback.sprite");
	overlay = sprite_load("rom:/sonicfloor.sprite");
	
    while(!end)
    {	
		getDisplay();

		rdpqStart();
		if(x > 0)
			rdpqDrawImage(sonicback, x-256, 0);
		rdpqDrawImage(sonicback, x, 0);
		if(x < 64)
			rdpqDrawImage(sonicback, x+256, 0);

		if(x > 0)
			rdpqDrawImage(overlay, 2*x-256, 48);
		rdpqDrawImage(overlay, 2*x, 48);
		if(x < 64)
			rdpqDrawImage(overlay, 2*x+256, 48);
		// Extra gap
		if(x < -96)
			rdpqDrawImage(overlay, 2*x+512, 48);
		
		rdpqEnd();
		
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsDown();
			
		if(keys.d_up)
			speed += 1;

		if(keys.d_down)
			speed -= 1;
			
		if(keys.b)
			end = 1;
		
		if(keys.a)
			pause = !pause;
			
		if(keys.c_left)
			acc *= -1;

		if(keys.c_right)
			vertical = !vertical;
		
		if(speed > 16)
			speed = 16;

		if(speed < 1)
			speed = 1;

		if(!pause)
		{
			if(!vertical)
				x += speed * acc;
			else
				y -= speed * acc;
		}
		
		if(x < -128)
			x = 128 - speed;
		if(x > 128)
			x = -128 + speed;
			
		if(y < -7)
			y = 0;
		if(y > 7)
			y = 0;
			
		if(!vertical)
		{
			currentframe ++;
			if(currentframe > 10)
			{
				currentframe = 0;
			}
		}
	}
	
	freeImage(&sonicback);
	freeImage(&overlay);
}
