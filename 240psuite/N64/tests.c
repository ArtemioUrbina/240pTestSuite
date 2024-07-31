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

typedef struct timecode {
	int hours;
	int minutes;
	int seconds;
	int frames;
	int type;
	int res;
} timecode;

uint16_t ConvertToFrames(timecode *time) {
	uint16_t	frames = 0;

	if(!time)
		return frames;

	frames = time->frames;
	if(isPAL)
		frames += time->seconds*50;
	else
		frames += time->seconds*60;
	frames += time->minutes*3600;
	frames += time->hours*216000;
	return frames;
}

void ConvertFromFrames(timecode *value, uint16_t Frames) {
	if(!value)
		return;
	value->hours = Frames / 216000;
	Frames = Frames % 216000;
	value->minutes = Frames / 3600;
	Frames = Frames % 3600;
	if(isPAL) {
		value->seconds = Frames / 50;
		value->frames = Frames % 50;
	}
	else {
		value->seconds = Frames / 60;
		value->frames = Frames % 60;
	}
}

void drawSonicBG(int x, int y, image *sonicTop, image *sonicWater, image *sonicFall) {	
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
}

void drawSonicFG(int x, int y, image *overlay) {	
	/* Draw Foreground */
	if(x > 0)
		rdpqDrawImageXY(overlay, 2*x-256, 48);
	rdpqDrawImageXY(overlay, 2*x, 48);
	if(x < 64)
		rdpqDrawImageXY(overlay, 2*x+256, 48);
	// Extra gap
	if(x < -96)
		rdpqDrawImageXY(overlay, 2*x+512, 48);
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

void drawDropShadow() {
	resolution_t oldVmode = current_resolution;
	int end = 0, invert = 0, frame = 0, reload = 1;
	int x = 0, y = 0, showText = 0, sprite = 0, selback = 0;
	int frameLen[] = { 8, 8, 8 }, animFrame = 0;
	int fallFrame = 0, currentframe = 0;
	image *donna = NULL, *shadow = NULL, *stripes = NULL, *check = NULL;
	image *buzz = NULL, *buzzshadow = NULL, *sshadow = NULL;
	image *sonicTop = NULL, *sonicWater = NULL, *sonicFall = NULL;
	image *overlay = NULL;	
	joypad_buttons_t keys;
	char msg[25];
		
	sshadow = loadImage("rom:/shadow.sprite");
	if(sshadow)	{
		x = dW/2- sshadow->tiles->width/2;
		y = dH/2 - sshadow->tiles->height/2;
	}

	buzz = loadImage("rom:/buzzbomber.sprite");
	buzzshadow = loadImage("rom:/buzzbomberShadow.sprite");
	
	sprite = RANDN(2);
	if(sprite == 0)
		shadow = sshadow;
	else
		shadow = buzzshadow;
		
	sonicTop = loadImage("rom:/sonicTop.sprite");
	sonicWater = loadImage("rom:/sonicWater.sprite");
	sonicFall = loadImage("rom:/sonicFall.sprite");
	overlay = loadImage("rom:/sonicFloor.sprite");
	
	check = loadImage("rom:/check.sprite");
	stripes = loadImage("rom:/stripes_v.sprite");

	while(!end) {
		if(reload) {
			if(!isSameRes(&oldVmode, &current_resolution)) {
				freeImage(&donna);
				oldVmode = current_resolution;
			}
			
			if(!donna) {
				if(vMode == SUITE_640x480) {
					donna = loadImage("rom:/donna-480.sprite");
					if(!donna)
						end = 1;
					else
						donna->scale = 0;
				}
				else
					donna = loadImage("rom:/donna.sprite");
				if(!donna)
					end = 1;
			}

			reload = 0;
		}
		
		
		getDisplay();

		rdpqStart();
		
		switch(selback) {
			case 0:
				rdpqDrawImage(donna);
				break;
			case 1:
				drawSonicBG(-(x-196), y, sonicTop, sonicWater, sonicFall);
				break;
			case 2:
				rdpqFillWithImage(stripes);
				break;
			case 3:
				rdpqFillWithImage(check);
				break;
			default:
				break;
		}
		
		if(frame == invert)
			rdpqDrawImageXY(shadow, x, y);
		if(sprite == 1 && buzz)	{
			rdpqDrawImageXY(buzz, x-20, y-20);
		}
		if(selback == 1)
			drawSonicFG(-(x-128), y, overlay);
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
			y--;
		if(keys.d_down)
			y++;
		if(y < 0)
			y = 0;
		if(y > dH - shadow->tiles->height)
			y = dH - shadow->tiles->height;
			
		if(keys.d_left) {
			x--;
			buzz->flipH = 0;
			buzzshadow->flipH = 0;
		}
		if(keys.d_right) {
			x++;
			buzz->flipH = 1;
			buzzshadow->flipH = 1;
		}
		if(x < 0)
			x = 0;
		if(x > dW - shadow->tiles->width)
			x = dW - shadow->tiles->width;
			
		if(selback == 1) {
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

		keys = controllerButtonsDown();
		checkStart(keys);
		if(keys.b)
			end = 1;
		
		if(keys.r) {
			invert = !invert;
			if(invert)
				sprintf(msg, "Shadow on odd frames");				
			else
				sprintf(msg, "Shadow on even frames");
			showText = 60;
		}
		
		if(keys.l) {
			sprite = !sprite;
			if(sprite == 0)
				shadow = sshadow;
			else
				shadow = buzzshadow;
		}
		
		if(keys.a) {
			selback ++;
			if(selback > 3)
				selback = 0;
		}
	}
	
	freeImage(&sshadow);
	freeImage(&buzzshadow);
	freeImage(&buzz);
	freeImage(&donna);
	
	freeImage(&sonicTop);
	freeImage(&sonicWater);
	freeImage(&sonicFall);
	freeImage(&overlay);
	
	freeImage(&stripes);
	freeImage(&check);
}

void drawLagTest() {
	uint16_t	frames = 0, seconds = 0, minutes = 0, hours = 0, framecnt = 1, done =  0;
	uint16_t	lsd, msd, pause = 0, toggle = 0;		
	image		*blueCircle = NULL, *redCircle = NULL, *circle = NULL;
	joypad_buttons_t keys;

	blueCircle = loadImage("rom:/circle.sprite");
	if(!blueCircle)
		return;
	
	redCircle = loadImage("rom:/circle.sprite");
	if(!redCircle)
		return;
	
	loadNumbers();
	
	blueCircle->palette[2] = graphics_make_color(0x00, 0x00, IRE_100, 0xff);
	updatePalette(blueCircle);
	redCircle->palette[2] = graphics_make_color(IRE_100, 0x00, 0x00, 0xff);
	updatePalette(redCircle);

	while(!done) {		
		if(!pause) {
			frames ++;
			framecnt ++;
			if(framecnt > 8)
				framecnt = 1;
			toggle = !toggle;
		}
		
		if(isPAL) {
			if(frames > 49)	{
				frames = 0;
				seconds ++;
			}
		}
		else {
			if(frames > 59) {
				frames = 0;
				seconds ++;
			}
		}

		if(seconds > 59) {
			seconds = 0;
			minutes ++;
		}

		if(minutes > 59) {
			minutes = 0;
			hours ++;
		}

		if(hours > 99)
			hours = 0;

		getDisplay();

		rdpqStart();
		rdpqClearScreenWhite();

		// Counter Separators
		drawDigit(80, 16, NUMBER_BLACK, 10);
		drawDigit(152, 16, NUMBER_BLACK, 10);
		drawDigit(224, 16, NUMBER_BLACK, 10);

		// Circles 1st row
		if(framecnt == 1)
			circle = redCircle;
		else
			circle = blueCircle;
			
		rdpqDrawImageXY(circle, 8, 56);
		drawDigit(28, 68, NUMBER_WHITE, 1);

		if(framecnt == 2)
			circle = redCircle;
		else
			circle = blueCircle;
			
		rdpqDrawImageXY(circle, 88, 56);
		drawDigit(108, 68, NUMBER_WHITE, 2);

		if(framecnt == 3)
			circle = redCircle;
		else
			circle = blueCircle;
			
		rdpqDrawImageXY(circle, 168, 56);
		drawDigit(188, 68, NUMBER_WHITE, 3);

		if(framecnt == 4)
			circle = redCircle;
		else
			circle = blueCircle;
			
		rdpqDrawImageXY(circle, 248, 56);
		drawDigit(268, 68, NUMBER_WHITE, 4);

		// Circles 2nd row
		if(framecnt == 5)
			circle = redCircle;
		else
			circle = blueCircle;
			
		rdpqDrawImageXY(circle, 8, 136);
		drawDigit(28, 148, NUMBER_WHITE, 5);

		if(framecnt == 6)
			circle = redCircle;
		else
			circle = blueCircle;
			
		rdpqDrawImageXY(circle, 88, 136);
		drawDigit(108, 148, NUMBER_WHITE, 6);

		if(framecnt == 7)
			circle = redCircle;
		else
			circle = blueCircle;
			
		rdpqDrawImageXY(circle, 168, 136);
		drawDigit(188, 148, NUMBER_WHITE, 7);

		if(framecnt == 8)
			circle = redCircle;
		else
			circle = blueCircle;
			
		rdpqDrawImageXY(circle, 248, 136);
		drawDigit(268, 148, NUMBER_WHITE, 8);

		// Draw Hours
		lsd = hours % 10;
		msd = hours / 10;
		drawDigit(32, 16, NUMBER_BLACK, msd);
		drawDigit(56, 16, NUMBER_BLACK, lsd);

		// Draw Minutes
		lsd = minutes % 10;
		msd = minutes / 10;
		drawDigit(104, 16, NUMBER_BLACK, msd);
		drawDigit(128, 16, NUMBER_BLACK, lsd);

		// Draw Seconds
		lsd = seconds % 10;
		msd = seconds / 10;
		drawDigit(176, 16, NUMBER_BLACK, msd);
		drawDigit(200, 16, NUMBER_BLACK, lsd);

		// Draw Frames
		lsd = frames % 10;
		msd = frames / 10;
		drawDigit(248, 16, toggle ? NUMBER_RED : NUMBER_BLUE, msd);
		drawDigit(272, 16, toggle? NUMBER_RED : NUMBER_BLUE, lsd);

		rdpqEnd();
		
		if(toggle) {
			drawBlackBox(0, 0, 6, dH);
			drawBlackBox(314, 0, 6, dH);
		}

		drawString(32, 8, 0, 0,	0, "hours");
		drawString(104, 8, 0, 0, 0, "minutes");
		drawString(176, 8, 0, 0, 0, "seconds");
		drawString(248, 8, 0, 0, 0, "frames");
		
		checkMenu(PASSIVELAG, NULL);
		waitVsync();

		joypad_poll();
		keys = controllerButtonsDown();
					
		if(keys.b)
			done =	1;				
					
		if(keys.c_left && !pause) {
			frames = hours = minutes = seconds = 0;
			framecnt = 1;
		}

		if(keys.a)
			pause = !pause;
		
		checkStart(keys);
	}
	
	freeImage(&redCircle);
	freeImage(&blueCircle);
	releaseNumbers();
}
 
void drawScroll() {
	int speed = 1, acc = -1, pause = 0, vertical = 0;
	int end = 0, x = 0, y = 0, currentframe = 0;
	int frameLen[] = { 8, 8, 8 }, animFrame = 0, fallFrame = 0;
	image 	*sonicTop = NULL, *sonicWater = NULL, *sonicFall = NULL;
	image	*overlay = NULL, *kiki = NULL;
	joypad_buttons_t keys;

	sonicTop = loadImage("rom:/sonicTop.sprite");
	sonicWater = loadImage("rom:/sonicWater.sprite");
	sonicFall = loadImage("rom:/sonicFall.sprite");
	overlay = loadImage("rom:/sonicFloor.sprite");
	kiki = loadImage("rom:/kiki.sprite");

    while(!end) {		
		getDisplay();

		rdpqStart();
		if(!vertical) {
			drawSonicBG(x, y, sonicTop, sonicWater, sonicFall);
			drawSonicFG(x, y, overlay);
		}
		else {
			if(y > 0)
				rdpqDrawImageXY(kiki, (dW-256)/2, y-512);
			rdpqDrawImageXY(kiki, (dW-256)/2, y);
			if(y <= -272)
				rdpqDrawImageXY(kiki, (dW-256)/2, 512+y);
		}
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

		if(keys.c_right) {
			vertical = !vertical;
			setClearScreen();
		}
		
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
			
		if(y >= 512)
			y = 0 + speed;
		if(y <= -512)
			y = 0 - speed;
			
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
	freeImage(&kiki);
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