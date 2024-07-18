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
 
#include "controller.h"
#include "video.h"

#define JOYTHSHLD 15			// min value for joystick to be a read
#define MINFRAMES 5				// Frames to count joystick held as pad

int JoyCountX = 0;
int JoyCountY = 0;

joypad_buttons_t controllerButtonsDown()
{
	joypad_inputs_t pad_inputs;
	joypad_buttons_t pad_pressed;
	int x, y;

	pad_inputs = joypad_get_inputs(JOYPAD_PORT_1);
	pad_pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);
		
	x = pad_inputs.stick_x;
	y = pad_inputs.stick_y;
	if (x > JOYTHSHLD)
	{
		if(JoyCountX < 0)
			JoyCountX = 0;
		JoyCountX++;
		if(JoyCountX > MINFRAMES)			
		{
			pad_pressed.d_right = 1;
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
			pad_pressed.d_left = 1;
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
			pad_pressed.d_up = 1;
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
			pad_pressed.d_down = 1;
			JoyCountY = 0;
		}
	}

#ifdef DEBUG_BENCHMARK
	joypad_buttons_t pad_held;
	
	pad_held = joypad_get_buttons_held(JOYPAD_PORT_1);
	if(pad_held.l && pad_held.r)
		resetVideo();
#endif
	return pad_pressed;
}


joypad_buttons_t controllerButtonsHeld()
{
	joypad_inputs_t pad_inputs;
	joypad_buttons_t pad_pressed;
	joypad_buttons_t pad_held;
	int x, y;

	pad_inputs = joypad_get_inputs(JOYPAD_PORT_1);
	pad_held = joypad_get_buttons_held(JOYPAD_PORT_1);
	pad_pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);
		
	x = pad_inputs.stick_x;
	y = pad_inputs.stick_y;
	if (x > JOYTHSHLD)
		pad_pressed.d_right = 1;
	if (x < -JOYTHSHLD) 
		pad_pressed.d_left = 1;
	if (y > JOYTHSHLD)  
		pad_pressed.d_up = 1;
	if (y < -JOYTHSHLD) 
		pad_pressed.d_down = 1;
		
	if(pad_held.d_up)
		pad_pressed.d_up = 1;
	if(pad_held.d_down)
		pad_pressed.d_down = 1;
	if(pad_held.d_right)
		pad_pressed.d_right = 1;
	if(pad_held.d_left)
		pad_pressed.d_left = 1;

#ifdef DEBUG_BENCHMARK
	if(pad_held.l && pad_held.r)
		resetVideo();
#endif
	return pad_pressed;
}
