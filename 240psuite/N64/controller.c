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
#include "system.h"

#define MINFRAMES 5				// Frames to count joystick held as pad

int JoyCountX = 0;
int JoyCountY = 0;

#ifdef SOAK_TEST
// Random input for long run tests (make soak), hold L+R to stop it
int randomInput = 1;

// Fair and fast random generation (using xorshift32, with explicit seed)
// fixed seed, so a crash can be reproduced with the same input sequence
static uint32_t rand_state = 1;
static uint32_t myrand(void) {
	uint32_t x = rand_state;
	x ^= x << 13;
	x ^= x >> 7;
	x ^= x << 5;
	return rand_state = x;
}

#define RANDN(n) ({ \
	__builtin_constant_p((n)) ? \
		(myrand()%(n)) : \
		(uint32_t)(((uint64_t)myrand() * (n)) >> 32); \
})
#endif

joypad_buttons_t controllerButtonsDown() {
	joypad_inputs_t pad_inputs;
	joypad_buttons_t pad_pressed;
	int x, y;

	pad_inputs = joypad_get_inputs(JOYPAD_PORT_1);
	pad_pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);
		
	x = pad_inputs.stick_x;
	y = pad_inputs.stick_y;
	if (x > JOYTHSHLD) {
		if(JoyCountX < 0)
			JoyCountX = 0;
		JoyCountX++;
		if(JoyCountX > MINFRAMES) {
			pad_pressed.d_right = 1;
			JoyCountX = 0;
		}
	}
	if (x < -JOYTHSHLD)	{
		if(JoyCountX > 0)
			JoyCountX = 0;
		JoyCountX--;
		if(JoyCountX < -MINFRAMES) {
			pad_pressed.d_left = 1;
			JoyCountX = 0;
		}
	}
	if (y > JOYTHSHLD) { 
		if(JoyCountY < 0)
			JoyCountY = 0;
		JoyCountY++;
		if(JoyCountY > MINFRAMES) {
			pad_pressed.d_up = 1;
			JoyCountY = 0;
		}
	}
	if (y < -JOYTHSHLD)	{
		if(JoyCountY > 0)
			JoyCountY = 0;
		JoyCountY--;
		if(JoyCountY < -MINFRAMES)			
		{
			pad_pressed.d_down = 1;
			JoyCountY = 0;
		}
	}

#ifdef SOAK_TEST
	if(randomInput) {
		if(RANDN(2))
			pad_pressed.d_up = 1;
		if(RANDN(2))
			pad_pressed.d_down = 1;
		if(RANDN(2))
			pad_pressed.d_right = 1;
		if(RANDN(2))
			pad_pressed.d_left = 1;
		
		if(RANDN(2))
			pad_pressed.c_up = 1;
		if(RANDN(2))
			pad_pressed.c_down = 1;
		if(RANDN(2))
			pad_pressed.c_left = 1;
		if(RANDN(2))
			pad_pressed.c_right = 1;
		if(RANDN(100) == 7)
			pad_pressed.start = 1;
		if(RANDN(10) == 4)
			pad_pressed.a = 1;
		if(RANDN(10) == 8)
			pad_pressed.b = 1;
		if(RANDN(2))
			pad_pressed.z = 1;
		if(RANDN(2))
			pad_pressed.l = 1;
		if(RANDN(2))
			pad_pressed.r = 1;
	}
#endif

#ifdef DEBUG_BENCHMARK
	joypad_buttons_t pad_held;
	
	pad_held = joypad_get_buttons_held(JOYPAD_PORT_1);
	if(pad_held.l && pad_held.r) {
		resetIdle();
		resetHeapMin();
#ifdef SOAK_TEST
		randomInput = 0;
#endif
	}
#endif
	return pad_pressed;
}


joypad_buttons_t controllerButtonsHeld() {
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
	
	if(pad_held.c_up)
		pad_pressed.c_up = 1;
	if(pad_held.c_down)
		pad_pressed.c_down = 1;
	if(pad_held.c_left)
		pad_pressed.c_left = 1;
	if(pad_held.c_right)
		pad_pressed.c_right = 1;
	if(pad_held.a)
		pad_pressed.a = 1;
	if(pad_held.b)
		pad_pressed.b = 1;
	if(pad_held.z)
		pad_pressed.z = 1;
	if(pad_held.l)
		pad_pressed.l = 1;
	if(pad_held.r)
		pad_pressed.r = 1;

#ifdef DEBUG_BENCHMARK
	if(pad_held.l && pad_held.r)
		resetIdle();
#endif
	return pad_pressed;
}

int getJoyX() {
	joypad_inputs_t pad_inputs;

	pad_inputs = joypad_get_inputs(JOYPAD_PORT_1);
		
	return(pad_inputs.stick_x);
}

int getJoyY() {
	joypad_inputs_t pad_inputs;

	pad_inputs = joypad_get_inputs(JOYPAD_PORT_1);
		
	return(pad_inputs.stick_y);
}