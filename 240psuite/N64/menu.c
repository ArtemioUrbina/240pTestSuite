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
#include "menu.h"

int showMenuSet = 0;

inline void checkMenu(char *help) {
	if(showMenuSet)
	{
		//helpData = help;
		showMenu();
		showMenuSet = 0;
	}
}

inline void checkStart(joypad_buttons_t keys) {
	if(keys.start)
		showMenuSet = 1;
}

void showMenu() {	
	int end = 0, px = 0, py = 0;
	sprite_t *menu;
	joypad_buttons_t keys;
	
	copyFrameBuffer();
	waitVsync();
	
	darkenBuffer(0x30);
	
	menu = sprite_load("rom:/menu.sprite");
	if(!menu) {
		freeFrameBuffer();
		return;
	}
	
	px = (dW - menu->width) / 2;
	py = (dH - menu->height) / 2;
	while(!end) {
		getDisplay();

		displayFrameBuffer();
		
		rdpqStart();
		rdpqDrawImage(menu, px, py);
		rdpqEnd();
		
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsDown();
		
		if(keys.b)
			end = 1;
	}
	
	freeFrameBuffer();
	freeImage(&menu);
}