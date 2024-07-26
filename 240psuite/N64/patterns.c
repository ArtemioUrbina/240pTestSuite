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

void drawPLUGE() {
	int end = 0, type = 0, showBorder = 0, showText = 0;
	image *back = NULL, *pal = NULL, *borderGreen = NULL, *borderRed = NULL;
	joypad_buttons_t keys;
	char msg[50];
	
	back = loadImage("rom:/pluge.sprite");
	if(!back)
		return;
	pal = loadImage("rom:/plugePAL.sprite");
	if(!pal)
		return;
	borderGreen = loadImage("rom:/PLUGEBorder.sprite");
	if(!borderGreen)
		return;
	borderRed = loadImage("rom:/PLUGEBorder.sprite");
	if(!borderRed)
		return;
	borderGreen->palette[1] = graphics_make_color(0xff, 0x00, 0x00, 0xff);
	data_cache_hit_writeback_invalidate(borderGreen->palette, sizeof(uint16_t)*borderGreen->palSize);
	borderRed->palette[1] = graphics_make_color(0x00, 0xff, 0x00, 0xff);
	data_cache_hit_writeback_invalidate(borderRed->palette, sizeof(uint16_t)*borderRed->palSize);
		
	while(!end) {
		getDisplay();

		rdpqStart();
		
		if(type)
			rdpqDrawImage(back);
		else
			rdpqDrawImage(pal);
		if(showBorder) {
			rdpqDrawImageXY(borderRed, 14, 39);
			rdpqDrawImageXY(borderRed, 44, 39);
			rdpqDrawImageXY(borderGreen, 74, 39);
			
			rdpqDrawImageXY(borderGreen, 228, 39);
			rdpqDrawImageXY(borderRed, 259, 39);
			rdpqDrawImageXY(borderRed, 289, 39);
		}
		rdpqEnd();
		
		if(showBorder) {
			drawString(10, 205, 0xff, 0x00, 0, type ? "11.5" : "3.5");
			drawString(40, 205, 0xff, 0x00, 0, type ? "7.5" : "2");
			drawString(70, 205, 0xff, 0x00, 0, type ? "3.5" : "1");
			
			drawString(224, 205, 0xff, 0x00, 0, type ? "3.5" : "1");
			drawString(255, 205, 0xff, 0x00, 0, type ? "7.5" : "2");
			drawString(285, 205, 0xff, 0x00, 0, type ? "11.5" : "3.5");
			showBorder --;
		}
		
		if(showText) {
			drawString(200, 20, 0, 0xff, 0, msg);
			showText --;
		}
		
		checkMenu(PLUGEHELP, NULL);
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsHeld();
		
		checkStart(keys);
		if(keys.a) {
			type = !type;
			sprintf(msg, type ? "NTSC 7.5 IRE" : "RGB Full Range");
			showText = 60;
		}
		if(keys.c_left)
			showBorder = 60;
		if(keys.b)
			end = 1;
	}
	
	freeImage(&borderGreen);
	freeImage(&borderRed);
	freeImage(&pal);
	freeImage(&back);
}

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


void drawGrayramp() {
	int end = 0;
	image *back = NULL;
	joypad_buttons_t keys;
	
	back = loadImage("rom:/grayramp.sprite");
	if(!back)
		return;
	while(!end) {
		getDisplay();

		rdpqStart();
		
		rdpqDrawImage(back);
		rdpqEnd();
		
		checkMenu(GRAYHELP, NULL);
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsHeld();
		
		checkStart(keys);
		if(keys.b)
			end = 1;
	}
	
	freeImage(&back);
}
