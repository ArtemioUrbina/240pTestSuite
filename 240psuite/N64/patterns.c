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
 
#include "patterns.h"
#include "menu.h"

void drawMonoscope() {
	int end = 0, pattern = 0;
	image *back = NULL, *orig = NULL;
	joypad_buttons_t keys;
	
	back = loadImage("rom:/monoscopeFBX.sprite");
	if(!back)
		return;
	orig = loadImage("rom:/monoscope.sprite");
	if(!orig) {
		freeImage(&back);
		return;
	}

	while(!end) {
		getDisplay();

		rdpqStart();
		if(!pattern)
			rdpqDrawImage(back);
		else
			rdpqDrawImage(orig);
		rdpqEnd();

#ifdef DEBUG_BENCHMARK
		printPalette(back, 20, 20);
		printPalette(orig, 100, 20);
#endif
		checkMenu(NULL, NULL);
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsHeld();
		
		checkStart(keys);
		if(keys.a)
			pattern = !pattern;
		if(keys.b)
			end = 1;
	}
	
	freeImage(&back);
	freeImage(&orig);
}

void drawGrid() {
	resolution_t oldVmode = current_resolution;
	int end = 0, reload = 1;
	image *back = NULL;
	joypad_buttons_t keys;
	
	while(!end) {
		if(reload) {
			if(!isSameRes(&oldVmode, &current_resolution)) {
				freeImage(&back);
				oldVmode = current_resolution;
			}
			
			if(!back) {
				if(vMode == SUITE_640x480) {
					back = loadImage("rom:/grid-480.sprite");
					if(!back)
						return;
					back->scale = false;
				}
				else
					back = loadImage("rom:/grid.sprite");
				if(!back)
					return;
			}
			
			reload = 0;
		}
		
		getDisplay();

		rdpqStart();
		
		rdpqDrawImage(back);
		rdpqEnd();
		
		checkMenu(GRIDHELP, &reload);
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsHeld();
		
		checkStart(keys);
		if(keys.b)
			end = 1;
	}
	
	freeImage(&back);
}
