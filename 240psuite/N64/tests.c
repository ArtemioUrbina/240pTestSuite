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
	int end = 0, invert = 0, frame = 0, reload = 1, showText = 0;
	image *donna = NULL, *shadow = NULL;	
	joypad_buttons_t keys;
	char msg[25];
	
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
					donna->scale = 0;
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
				shadow->x = dW/2- shadow->tiles->width/2;
				shadow->y = dH/2 - shadow->tiles->height/2;
			}

			reload = 0;
		}
		
		getDisplay();

		rdpqStart();
		
		rdpqDrawImage(donna);
		if(frame == invert)
			rdpqDrawImage(shadow);
		rdpqEnd();
		
		if(showText) {
			if(vMode == SUITE_640x480)
				drawStringB(450, 40, 0, 0xff, 0, msg);
			else
				drawStringB(140, 30, 0, 0xff, 0, msg);
			showText--;
		}
		
		checkMenu(DROPSHADOW, &reload);
		waitVsync();
		
		frame = !frame;
		
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
		if(keys.a) {
			invert = !invert;
			if(invert)
				sprintf(msg, "Shadow on odd frames");				
			else
				sprintf(msg, "Shadow on even frames");
			showText = 60;
		}
	}
	
	freeImage(&donna);
	freeImage(&shadow);
}

void rotateWaterPalette(image *data) {
	uint16_t tmpCol = data->palette[2];
	
	data->palette[2] = data->palette[3];
	data->palette[3] = tmpCol;
	
	updatePalette(data);
}

void rotateFallPalette(image *data) {
	uint16_t tmpCol = data->palette[4];
	
	data->palette[4] = data->palette[3];
	data->palette[3] = data->palette[2];
	data->palette[2] = tmpCol;
	
	updatePalette(data);
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
	
	/*
	sonicTop->scale = 0;
	sonicWater->scale = 0;
	sonicFall->scale = 0;
	overlay->scale = 0;
	*/
	
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

void drawStripes() {
	int end = 0, alternate = 0, isVertical = 0;
	int frame = 0, dframe = 0;
	image *back = NULL, *horizontal = NULL, *vertical = NULL;;
	joypad_buttons_t keys;
	fmenuData	verMenuData[] = { { 0, "Horizontal" }, {1, "Vertical"} };
	
	horizontal = loadImage("rom:/stripes.sprite");
	if(!horizontal)
		return;
	
	vertical = loadImage("rom:/stripes_v.sprite");
	if(!vertical)
		return;
	
	isVertical = selectMenu("Select Type", verMenuData, 2, isVertical+1);
	if(isVertical == MENU_CANCEL)
		end = 1;
	
	while(!end) {
		getDisplay();

		back = isVertical ? vertical : horizontal;
		rdpqStart();
		rdpqFillWithImage(back);
		rdpqEnd();
		
		if(dframe) {
			char msg[20];

			sprintf(msg, "Frame: %02d", frame);
			drawStringB(20, vMode == SUITE_640x480 ? 460 : 210, 0xff, 0xff, 0xff, msg);
			frame ++;
			if(isPAL) {
				if(frame > 49)
					frame = 0;
			}
			else {
				if(frame > 59)
					frame = 0;
			}
		}
		checkMenu(STRIPESHELP, NULL);
		waitVsync();
		
		if(alternate)
			swapPaletteColors(back, 0, 1);
		
		joypad_poll();
		keys = controllerButtonsDown();
		
		checkStart(keys);
		if(keys.b)
			end = 1;
		if(keys.a)
			alternate = !alternate;
		
		if(keys.c_left && !alternate)
			swapPaletteColors(back, 0, 1);
		
		if(keys.c_right) {
			dframe = !dframe;
			frame = 0;
		}
		
		if(keys.r) {
			int oldOption = isVertical;
			
			isVertical = selectMenu("Select Type", verMenuData, 2, isVertical+1);
			if(isVertical == MENU_CANCEL)
				isVertical = oldOption;
		}
	}
	
	back = NULL;
	freeImage(&horizontal);
	freeImage(&vertical);
}

void drawCheckerBoard() {
	int end = 0, alternate = 0;
	int frame = 0, dframe = 0;
	image *back = NULL;
	joypad_buttons_t keys;
	
	back = loadImage("rom:/check.sprite");
	if(!back)
		return;
	while(!end) {
		getDisplay();

		rdpqStart();
		rdpqFillWithImage(back);
		rdpqEnd();
		
		if(dframe) {
			char msg[20];

			sprintf(msg, "Frame: %02d", frame);
			drawStringB(20, vMode == SUITE_640x480 ? 460 : 210, 0xff, 0xff, 0xff, msg);
			frame ++;
			if(isPAL) {
				if(frame > 49)
					frame = 0;
			}
			else {
				if(frame > 59)
					frame = 0;
			}
		}
		checkMenu(CHECKHELP, NULL);
		waitVsync();
		
		if(alternate)
			swapPaletteColors(back, 0, 1);
		
		joypad_poll();
		keys = controllerButtonsDown();
		
		checkStart(keys);
		if(keys.b)
			end = 1;
		if(keys.a)
			alternate = !alternate;
		
		if(keys.c_left && !alternate)
			swapPaletteColors(back, 0, 1);
		
		if(keys.c_right) {
			dframe = !dframe;
			frame = 0;
		}
	}
	
	freeImage(&back);
}