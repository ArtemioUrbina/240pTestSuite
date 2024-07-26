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
 
#include "menu.h"

void drawDropShadow() {
	resolution_t oldVmode = current_resolution;
	int end = 0, show = 0, reload = 1;
	image *donna = NULL, *shadow = NULL;	
	joypad_buttons_t keys;
	
	while(!end) {
		if(reload) {
			if(!isSameRes(&oldVmode, &current_resolution)) {
				freeImage(&donna);
				oldVmode = current_resolution;
			}
			
			if(!donna) {
				if(vMode == SUITE_640x480) {
					donna = loadImage("rom:/donna-480.sprite");
					if(!donna) {
						freeImage(&shadow);
						return;
					}
					donna->scale = false;
				}
				else
					donna = loadImage("rom:/donna.sprite");
				if(!donna) {
					freeImage(&shadow);
					return;
				}
			}
			if(!shadow) {
				shadow = loadImage("rom:/shadow.sprite");
				if(!shadow)	{
					freeImage(&donna);
					return;
				}
				shadow->x = 160 - shadow->tiles->width/2;
				shadow->y = 120 - shadow->tiles->height/2;
			}

			reload = 0;
		}
		
		getDisplay();

		rdpqStart();
		
		rdpqDrawImage(donna);
		if(show)
			rdpqDrawImage(shadow);
		rdpqEnd();
		
		checkMenu(DROPSHADOW, &reload);
		waitVsync();
		
		show = !show;
		
		joypad_poll();
		keys = controllerButtonsHeld();
		
		if(keys.d_up)
			shadow->y--;
		if(keys.d_down)
			shadow->y++;
		if(shadow->y < 0)
			shadow->y = 0;
		if(shadow->y > dH - shadow->tiles->height)
			shadow->y = dH - shadow->tiles->height;
			
		if(keys.d_left)
			shadow->x--;
		if(keys.d_right)
			shadow->x++;		
		if(shadow->x < 0)
			shadow->x = 0;
		if(shadow->x > dW - shadow->tiles->width)
			shadow->x = dW - shadow->tiles->width;

		checkStart(keys);
		if(keys.b)
			end = 1;
		if(keys.a)
			show = !show;
	}
	
	freeImage(&donna);
	freeImage(&shadow);
}

void rotateWaterPalette(image *data) {
	uint16_t tmpCol = data->palette[2];
	
	data->palette[2] = data->palette[3];
	data->palette[3] = tmpCol;
	
	data_cache_hit_writeback_invalidate(data->palette, sizeof(uint16_t)*data->palSize);
}

void rotateFallPalette(image *data) {
	uint16_t tmpCol = data->palette[4];
	
	data->palette[4] = data->palette[3];
	data->palette[3] = data->palette[2];
	data->palette[2] = tmpCol;
	
	data_cache_hit_writeback_invalidate(data->palette, sizeof(uint16_t)*data->palSize);
}
 
void drawScroll() {
	int speed = 1, acc = -1, pause = 0, vertical = 0;
	int end = 0, x = 0, y = 0, currentframe = 0;
	int frameLen[] = { 8, 8, 8 }, animFrame = 0, fallFrame = 0;
	image 	*sonicTop = NULL, *sonicWater = NULL, *sonicFall = NULL;
	image	*overlay = NULL;	
	joypad_buttons_t keys;

	sonicTop = loadImage("rom:/sonicTop.sprite");
	if(!sonicTop)
		return;
	sonicWater = loadImage("rom:/sonicWater.sprite");
	if(!sonicWater) {
		freeImage(&sonicTop);
		return;
	}
	sonicFall = loadImage("rom:/sonicFall.sprite");
	if(!sonicFall) {
		freeImage(&sonicTop);
		freeImage(&sonicWater);
		return;
	}
	
	overlay = loadImage("rom:/sonicFloor.sprite");
	if(!overlay) {
		freeImage(&sonicTop);
		freeImage(&sonicWater);
		freeImage(&sonicFall);
		return;
	}
	
	sonicTop->scale = false;
	sonicWater->scale = false;
	sonicFall->scale = false;
	overlay->scale = false;
	
    while(!end) {		
		getDisplay();

		rdpqStart();
		
		rdpqClearScreen();
		
		/* Draw Background */
		if(x > 0) {
			rdpqDrawImageXY(sonicTop, x-256, 0);
			rdpqDrawImageXY(sonicWater, x-256, 156);
			rdpqDrawImageXY(sonicFall, x-177, 131);
		}
		rdpqDrawImageXY(sonicTop, x, 0);
		rdpqDrawImageXY(sonicWater, x, 156);
		rdpqDrawImageXY(sonicFall, x+79, 131);
		if(x < 64) {
			rdpqDrawImageXY(sonicTop, x+256, 0);
			rdpqDrawImageXY(sonicWater, x+256, 156);
			rdpqDrawImageXY(sonicFall, x+335, 131);
		}

		/* Draw Foreground */
		if(x > 0)
			rdpqDrawImageXY(overlay, 2*x-256, 48);
		rdpqDrawImageXY(overlay, 2*x, 48);
		if(x < 64)
			rdpqDrawImageXY(overlay, 2*x+256, 48);
		// Extra gap
		if(x < -96)
			rdpqDrawImageXY(overlay, 2*x+512, 48);

		rdpqEnd();
		
#ifdef DEBUG_BENCHMARK
		printPalette(sonicFall, 20, 20);
		printPalette(sonicWater, 100, 20);
#endif
		
		checkMenu(SCROLLHELP, NULL);
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

		checkStart(keys);

		if(!pause) {
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
			
		if(!vertical) {
			currentframe ++;
			if(currentframe > 25) {
				rotateWaterPalette(sonicWater);
				currentframe = 0;
			}
			fallFrame ++;
			if(fallFrame > frameLen[animFrame]) {
				rotateFallPalette(sonicFall);
				fallFrame = 0;
				animFrame ++;
				if(animFrame > 2)
					animFrame = 0;
			}
		}
	}
	
	freeImage(&sonicTop);
	freeImage(&sonicWater);
	freeImage(&sonicFall);
	freeImage(&overlay);
}
