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

void drawIntro(void);
void drawPatternsMenu(void);
void drawPatternsColorMenu(void);
void drawPatternsGeometryMenu(void);
void drawVideoTestsMenu(void);
 
int main(void)
{
	int sel = 1, reload = 1;
	struct controller_data keys;
	sprite_t *bg = NULL, *sd = NULL;
	
	initN64();
	loadFont();

	//drawIntro();
	while(1) {
		int c = 1, x = 55, y = 90;
		int r = 0xFF, g = 0xFF, b = 0xFF;
		
		if(reload)
		{
			bg = sprite_load("rom:/mainbg.sprite");
			sd = sprite_load("rom:/sd.sprite");
			reload = 0;
		}
		
		getDisplay();
		
		rdpqStart();
		rdpqDrawImage(bg, 0, 0);
		rdpqDrawImage(sd, 225, 75);
		rdpqEnd();
		
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Test Patterns"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Video Tests"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Audio Tests"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Hardware Tests"); y += fh;
				
		waitVsync();
		
		controller_scan();
        keys = Controller_ButtonsDown();

		if(keys.c[0].up)
			sel--;

		if(keys.c[0].down)
			sel++;
			
		if(sel > c)
			sel = 1;
		if(sel < 1)
			sel = c;
		
		if(keys.c[0].A)
		{	
			freeImage(&bg);
			freeImage(&sd);
			
			switch(sel)
			{
				case 1:
					drawPatternsMenu();
					break;
				case 2:
					drawVideoTestsMenu();
					break;
			}
			reload = 1;
		}
	}
	freeImage(&bg);
	freeImage(&sd);
	return 0;
}

void drawPatternsMenu(void)
{
	int sel = 1, reload = 1, exit = 0;
	struct controller_data keys;
	sprite_t *bg = NULL, *sd = NULL;
	
	while(!exit) {
		int c = 1, x = 55, y = 90;
		int r = 0xFF, g = 0xFF, b = 0xFF;
		
		if(reload)
		{
			bg = sprite_load("rom:/mainbg.sprite");
			sd = sprite_load("rom:/sd.sprite");
			reload = 0;
		}
		
		getDisplay();
		
		rdpqStart();
		rdpqDrawImage(bg, 0, 0);
		rdpqDrawImage(sd, 225, 75);
		rdpqEnd();
		
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Color & Black Levels"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Geometry"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "HCFR Test Patterns"); y += fh; c++;
		drawStringS(x, y + fh, r * 0.8, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); y += fh;
				
		waitVsync();
		
		controller_scan();
        keys = Controller_ButtonsDown();

		if(keys.c[0].up)
			sel--;

		if(keys.c[0].down)
			sel++;
			
		if(sel > c)
			sel = 1;
		if(sel < 1)
			sel = c;
		
		if(keys.c[0].A)
		{	
			freeImage(&bg);
			freeImage(&sd);
			
			switch(sel)
			{
				case 1:
					drawPatternsColorMenu();
					break;
				case 2:
					drawPatternsGeometryMenu();
					break;
				case 4:
					exit = 1;
					break;
			}
			reload = 1;
		}
		
		if(keys.c[0].B)
			exit = 1;
	}
	freeImage(&bg);
	freeImage(&sd);
}

void drawPatternsColorMenu(void)
{
	int sel = 1, reload = 1, exit = 0;
	struct controller_data keys;
	sprite_t *bg = NULL, *sd = NULL;
	
	while(!exit) {
		int c = 1, x = 45, y = 70;
		int r = 0xFF, g = 0xFF, b = 0xFF;
		
		if(reload)
		{
			bg = sprite_load("rom:/mainbg.sprite");
			sd = sprite_load("rom:/sd.sprite");
			reload = 0;
		}
		
		getDisplay();
		
		rdpqStart();
		rdpqDrawImage(bg, 0, 0);
		rdpqDrawImage(sd, 225, 75);
		rdpqEnd();
		
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "PLUGE"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Color Bars"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "EBU Color Bars"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "SMPTE Color Bars"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Color Bars/Gray"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Color Bleed Check"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Gray Ramp"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "White & RGB screens"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "100 IRE"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Sharpness"); y += fh; c++;
		drawStringS(x, y + fh, r * 0.8, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Patterns Menu"); y += fh;
				
		waitVsync();
		
		controller_scan();
        keys = Controller_ButtonsDown();

		if(keys.c[0].up)
			sel--;

		if(keys.c[0].down)
			sel++;
			
		if(sel > c)
			sel = 1;
		if(sel < 1)
			sel = c;
		
		if(keys.c[0].A)
		{	
			freeImage(&bg);
			freeImage(&sd);
			
			switch(sel)
			{
				case 11:
					exit = 1;
					break;
			}
			reload = 1;
		}
		
		if(keys.c[0].B)
			exit = 1;
	}
	freeImage(&bg);
	freeImage(&sd);
}

void drawPatternsGeometryMenu(void)
{
	int sel = 1, reload = 1, exit = 0;
	struct controller_data keys;
	sprite_t *bg = NULL, *sd = NULL;
	
	while(!exit) {
		int c = 1, x = 55, y = 90;
		int r = 0xFF, g = 0xFF, b = 0xFF;
		
		if(reload)
		{
			bg = sprite_load("rom:/mainbg.sprite");
			sd = sprite_load("rom:/sd.sprite");
			reload = 0;
		}
		
		getDisplay();
		
		rdpqStart();
		rdpqDrawImage(bg, 0, 0);
		rdpqDrawImage(sd, 225, 75);
		rdpqEnd();
		
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Monoscope"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Grid"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Overscan"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Convergence"); y += fh; c++;
		drawStringS(x, y + fh, r * 0.8, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Patterns Menu"); y += fh;
				
		waitVsync();
		
		controller_scan();
        keys = Controller_ButtonsDown();

		if(keys.c[0].up)
			sel--;

		if(keys.c[0].down)
			sel++;
			
		if(sel > c)
			sel = 1;
		if(sel < 1)
			sel = c;
		
		if(keys.c[0].A)
		{	
			freeImage(&bg);
			freeImage(&sd);
			
			switch(sel)
			{
				case 5:
					exit = 1;
					break;
			}
			reload = 1;
		}
		
		if(keys.c[0].B)
			exit = 1;
	}
	freeImage(&bg);
	freeImage(&sd);
}

void drawVideoTestsMenu(void)
{
	int sel = 1, reload = 1, exit = 0;
	struct controller_data keys;
	sprite_t *bg = NULL, *sd = NULL;
	
	while(!exit) {
		int c = 1, x = 45, y = 55;
		int r = 0xFF, g = 0xFF, b = 0xFF;
		
		if(reload)
		{
			bg = sprite_load("rom:/mainbg.sprite");
			sd = sprite_load("rom:/sd.sprite");
			reload = 0;
		}
		
		getDisplay();
		
		rdpqStart();
		rdpqDrawImage(bg, 0, 0);
		rdpqDrawImage(sd, 225, 75);
		rdpqEnd();
		
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Drop Shadow Test"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Striped Sprite Test"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Lag Test"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Timing & Reflex Test"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Scroll Test"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Grid Scroll Test"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Horz./Vert. Stripes"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Checkerboard"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Phase & Sample Rate"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Disappearing Logo"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Backlit Zone Test"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Diagonal Test"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Alternating 240p/480i Test"); y += fh; c++;
		drawStringS(x, y + fh/2, r * 0.8, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Patterns Menu"); y += fh;
				
		waitVsync();
		
		controller_scan();
        keys = Controller_ButtonsDown();

		if(keys.c[0].up)
			sel--;

		if(keys.c[0].down)
			sel++;
			
		if(sel > c)
			sel = 1;
		if(sel < 1)
			sel = c;
		
		if(keys.c[0].A)
		{	
			freeImage(&bg);
			freeImage(&sd);
			
			switch(sel)
			{
				case 14:
					exit = 1;
					break;
			}
			reload = 1;
		}
		
		if(keys.c[0].B)
			exit = 1;
	}
	freeImage(&bg);
	freeImage(&sd);
}

void drawIntro()
{
	int delay = 90;
	struct controller_data keys;
	sprite_t *ld = NULL;
	
	ld = sprite_load("rom:/libdragon.sprite");
	if(!ld)
		return;
	
	while(delay) {
		getDisplay();
		
		rdpqStart();
		rdpqDrawImage(ld, (dW - ld->width)/2, (dH - ld->height)/2);
		rdpqEnd();
		
		waitVsync();
		
		controller_scan();
        keys = Controller_ButtonsDown();
		
		delay --;
		if(keys.c[0].A || keys.c[0].B)
			delay = 0;
	}
	
	freeImage(&ld);
}