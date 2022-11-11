/* 
 * 240p Test Suite
 * Copyright (C)2016 Artemio Urbina (N64)
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 
#include "controller.h"
#include "video.h"

#define JOYTHSHLD 15			// min value for joystick to be a read
#define MINFRAMES 5				// Frames to count joystick held as pad

int JoyCountX = 0;
int JoyCountY = 0;

struct controller_data Controller_ButtonsDown()
{
	struct controller_data keys, held;
	int x, y;

	held = get_keys_held();
	keys = get_keys_down();
		
	x  = held.c[0].x;
	y  = held.c[0].y;
	if (x > JOYTHSHLD)
	{
		if(JoyCountX < 0)
			JoyCountX = 0;
		JoyCountX++;
		if(JoyCountX > MINFRAMES)			
		{
			keys.c[0].right = 1;
			JoyCountX = 0;
		}
	}
	if (x < -JOYTHSHLD) 
	{
		if(JoyCountX > 0)
			JoyCountX = 0;
		JoyCountX--;
		if(JoyCountX < -MINFRAMES)			
		{
			keys.c[0].left = 1;
			JoyCountX = 0;
		}
	}
	if (y > JOYTHSHLD) 
	{ 
		if(JoyCountY < 0)
			JoyCountY = 0;
		JoyCountY++;
		if(JoyCountY > MINFRAMES)			
		{
			keys.c[0].up = 1;
			JoyCountY = 0;
		}
	}
	if (y < -JOYTHSHLD) 
	{
		if(JoyCountY > 0)
			JoyCountY = 0;
		JoyCountY--;
		if(JoyCountY < -MINFRAMES)			
		{
			keys.c[0].down = 1;
			JoyCountY = 0;
		}
	}

	if(held.c[0].C_down && held.c[0].R && held.c[0].L)
		reset_video();
	return keys;
}


struct controller_data Controller_ButtonsHeld()
{
	struct controller_data held;
	int x, y;

	held = get_keys_held();
		
	x  = held.c[0].x;
	y  = held.c[0].y;
	if (x > JOYTHSHLD)
		held.c[0].right = 1;
	if (x < -JOYTHSHLD) 
		held.c[0].left = 1;
	if (y > JOYTHSHLD)  
		held.c[0].up = 1;
	if (y < -JOYTHSHLD) 
		held.c[0].down = 1;
	
	if(held.c[0].C_down && held.c[0].R && held.c[0].L)
		reset_video();
	return held;
}
