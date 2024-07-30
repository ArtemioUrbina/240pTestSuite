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
	updatePalette(borderGreen);
	borderRed->palette[1] = graphics_make_color(0x00, 0xff, 0x00, 0xff);
	updatePalette(borderRed);
		
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
		keys = controllerButtonsDown();
		
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

void drawColorbars() {
	int end = 0, pattern = 0;
	image *back = NULL, *grid = NULL;
	joypad_buttons_t keys;
	
	back = loadImage("rom:/color.sprite");
	if(!back)
		return;
	grid = loadImage("rom:/color_grid.sprite");
	if(!grid) {
		freeImage(&back);
		return;
	}

	while(!end) {
		getDisplay();

		rdpqStart();
		if(!pattern)
			rdpqDrawImage(back);
		else
			rdpqDrawImage(grid);
		rdpqEnd();

		checkMenu(COLORBARSHELP, NULL);
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsDown();
		
		checkStart(keys);
		if(keys.a)
			pattern = !pattern;
		if(keys.b)
			end = 1;
	}
	
	freeImage(&back);
	freeImage(&grid);
}

void swapPalette100to75(image *data) {
	if(!data || !data->palette)
		return;
	
	for(unsigned int c = 0; c < data->palSize; c++) {
		color_t color = color_from_packed16(data->palette[c]);
		if(color.r == COLOR_100)
			color.r = COLOR_75;
		if(color.g == COLOR_100)
			color.g = COLOR_75;
		if(color.b == COLOR_100)
			color.b = COLOR_75;
		data->palette[c] = color_to_packed16(color);
	}
	updatePalette(data);
}

void swapPalette75to100(image *data) {
	if(!data || !data->palette)
		return;
	
	for(unsigned int c = 0; c < data->palSize; c++) {
		color_t color = color_from_packed16(data->palette[c]);
		if(color.r == COLOR_75)
			color.r = COLOR_100;
		if(color.g == COLOR_75)
			color.g = COLOR_100;
		if(color.b == COLOR_75)
			color.b = COLOR_100;
		data->palette[c] = color_to_packed16(color);
	}
	updatePalette(data);
}

void drawEBUSMPTE(unsigned int ebu) {
	int end = 0, show75 = 0, showText = 0;
	image *back = NULL;
	joypad_buttons_t keys;
	char msg[50];
	
	if(ebu)
		back = loadImage("rom:/EBU75.sprite");
	else
		back = loadImage("rom:/SMPTE75.sprite");
	if(!back)
		return;
	
	
	swapPalette75to100(back);
	while(!end) {
		getDisplay();
		
		rdpqStart();
		rdpqDrawImage(back);
		rdpqEnd();
		
		if(showText) {
			drawStringB(240, 20, 0, 0xff, 0, msg);
			showText --;
		}
		
		checkMenu(SMPTECOLOR, NULL);
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsDown();
		
		checkStart(keys);
		if(keys.a) {
			show75 = !show75;
			if(show75) {
				swapPalette100to75(back);
				sprintf(msg, " 75%%");
			}
			else {
				swapPalette75to100(back);
				sprintf(msg, "100%%");
			}
			showText = 60;
		}
		if(keys.b)
			end = 1;
	}
	
	freeImage(&back);
}

void drawCBGray() {
	int end = 0;
	image *back = NULL;
	joypad_buttons_t keys;
	
	back = loadImage("rom:/601701cb.sprite");
	if(!back)
		return;
	while(!end) {
		getDisplay();

		rdpqStart();
		
		rdpqDrawImage(back);
		rdpqEnd();
		
		checkMenu(COLOR601, NULL);
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsDown();
		
		checkStart(keys);
		if(keys.b)
			end = 1;
	}
	
	freeImage(&back);
}

void drawColorbleed() {
	int end = 0, pattern = 0;
	image *back = NULL, *check = NULL;
	joypad_buttons_t keys;
	
	back = loadImage("rom:/colorbleed.sprite");
	if(!back)
		return;
	check = loadImage("rom:/colorbleedchk.sprite");
	if(!check) {
		freeImage(&back);
		return;
	}

	while(!end) {
		getDisplay();

		rdpqStart();
		if(!pattern)
			rdpqDrawImage(back);
		else
			rdpqDrawImage(check);
		rdpqEnd();

		checkMenu(COLORBLEEDHELP, NULL);
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsDown();
		
		checkStart(keys);
		if(keys.a)
			pattern = !pattern;
		if(keys.b)
			end = 1;
	}
	
	freeImage(&back);
	freeImage(&check);
}

void drawWhiteScreen() {
	int end = 0, color = 0, blackLevel = 0x00, text = 0;
	int	cr, cb, cg, sel = 1, editmode = 0, er, eb, eg;
	char msg[100], *mode[5] = { "White", "Black", "Red", "Green", "Blue" };
	joypad_buttons_t keys;
	
	/*
	if(!IsPAL)
		blackLevel = 0x13; // 7.5 IRE TODO: Needs vectorscope confirmation ported from Wii ATM
	else
	*/
		blackLevel = 0x00; // 0 IRE
	
	// white
	cr = cg = cb = COLOR_100;
	er = eb = eg = COLOR_100;
	while(!end) {
		getDisplay();
		
		graphics_fill_screen(__disp, graphics_make_color(cr, cg, cb, 0xff));
		if(text) {
			drawStringB(140, 20, 0xff, 0xff, 0xff, msg);
			text --;
		}
		
		waitVsync();
		
		checkMenu(WHITEHELP, NULL);
		
		joypad_poll();
		keys = controllerButtonsDown();

		checkStart(keys);
		if(keys.b)
			end = 1;
			
		if(keys.a && color == 1) {
			if(!blackLevel)	{
				blackLevel = 0x13;
				sprintf(msg, "#GBlack Level: 7.5 IRE#G");
			}
			else {
				blackLevel = 0x0;
				sprintf(msg, "#GBlack Level: 0 IRE#G");
			}
			text = 140;
		}
		
		if (keys.a && color == 0)
			editmode = !editmode;
		
		if(editmode) {
			int *current = NULL;
							
			sprintf(msg, "#%cR:0x%x#W #%cG:0x%x#W #%cB:0x%x#W", 
					sel == 1 ? 'G' : 'W', er,
					sel == 2 ? 'G' : 'W', eg, 
					sel == 3 ? 'G' : 'W', eb);
			text = 1;

			if(keys.d_left) {
				sel --;
				if(sel < 1)
					sel = 3;
			}
			
			if(keys.d_right)	{
				sel ++;
				if(sel > 3)
					sel = 1;
			}
			
			switch(sel)	{
				case 1:
					current = &er;
					break;
				case 2:
					current = &eg;
					break;
				case 3:
					current = &eb;
					break;
			}
			
			if(keys.d_up) {
				if(current)	{
					(*current) ++;
					if(*current > 0xff)
						*current = 0xff;
				}
			}
			
			if(keys.d_down) {
				if(current)	{
					(*current) --;
					if(*current < 0)
						*current = 0;
				}
			}	

			if(keys.c_down) {
				if(current)
					*current  = 0;
			}
			
			if(keys.c_up)	{
				if(current)	
					*current = 0xff;
			}
			
			if(keys.c_right) {
				if(current)	{
					(*current) += 0x10;
					if(*current > 0xff)
						*current = 0xff;
				}
			}
			
			if(keys.c_left) {			
				if(current)	{
					(*current) -= 0x10;
					if(*current < 0)
						*current = 0;
				}
			}	
		}
		
		if(keys.r) {
			color ++;
			if(color > 4)
				color = 0;
			
			editmode = 0;
			if(color == 0 && er + eb + eg != 3*COLOR_100)
				sprintf(msg, "%s [EDITED]", mode[color]);
			else
				sprintf(msg, "%s", mode[color]);
			text = 30;
		}

		if(keys.l) {			
			color --;
			if(color < 0)
				color = 4;
				
			editmode = 0;
			if(color == 0 && er + eb + eg != 3*COLOR_100)
				sprintf(msg, "%s [edited]", mode[color]);
			else
				sprintf(msg, "%s", mode[color]);
			text = 30;
		}
		
		switch(color) {
				case 0:
					cr = er;
					cg = eg;
					cb = eb;
					break;
				case 1:
					cr = cg = cb = blackLevel;
					break;
				case 2:
					cr = COLOR_100;
					cb = cg = 0;
					break;
				case 3:
					cg = COLOR_100;
					cr = cb = 0;
					break;
				case 4:
					cb = COLOR_100;
					cr = cg = 0;
					break;
		}
	}
}

void drawSharpness() {
	int end = 0;
	image *back = NULL;
	joypad_buttons_t keys;
	
	back = loadImage("rom:/sharpness.sprite");
	if(!back)
		return;
	while(!end) {
		getDisplay();

		rdpqStart();
		
		rdpqDrawImage(back);
		rdpqEnd();
		
		checkMenu(SHARPNESSHELP, NULL);
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsDown();
		
		checkStart(keys);
		if(keys.b)
			end = 1;
	}
	
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
		keys = controllerButtonsDown();
		
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
					back->scale = 0;
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
		keys = controllerButtonsDown();
		
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
		keys = controllerButtonsDown();
		
		checkStart(keys);
		if(keys.b)
			end = 1;
	}
	
	freeImage(&back);
}
