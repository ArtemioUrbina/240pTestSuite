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
 
#include "tests.h"
#include "patterns.h"
#include "hardware.h"
#include "audio.h"
#include "menu.h"
#include "mcs.h"

void drawIntro(void);
void drawPatternsMenu(void);
void drawPatternsColorMenu(void);
void drawPatternsGeometryMenu(void);
void drawVideoTestsMenu(void);
void drawFooter(int x, int *y, int *sel, int *c);

int main(void) {
	int sel = 1, reload = 1;
	joypad_buttons_t keys;
	image *bg = NULL, *sd = NULL;
	
	initN64();

#ifndef DEBUG_BENCHMARK
	drawIntro();
#else
	debug_init_isviewer();
#endif
	
	while(1) {
		int c = 1, x = 55, y = 80;
		int r = 0xFF, g = 0xFF, b = 0xFF;
		
		if(reload) {
			freeImage(&bg);
			freeImage(&sd);
			bg = loadImage("rom:/mainbg.sprite");
			sd = loadImage("rom:/sd.sprite");
			if(sd) {
				sd->x = 225;
				sd->y = 75;
			}
			reload = 0;
		}
		
		getDisplay();
		
		rdpqStart();
		rdpqDrawImage(bg);
		rdpqDrawImage(sd);
		SD_blink_cycle(sd);
		rdpqEnd();
		
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Test Patterns"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Video Tests"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Audio Tests"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Hardware Tests"); y += 2*fh; c++;
		
		drawFooter(x, &y, &sel, &c);
		
		drawSysData();
		checkMenu(GENERALHELP, NULL);
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsDown();

		if(keys.d_up)
			sel--;

		if(keys.d_down)
			sel++;
			
		if(sel > c)
			sel = 1;
		if(sel < 1)
			sel = c;
			
		checkStart(keys);
		
		if(keys.a) {	
			freeImage(&bg);
			freeImage(&sd);
			SD_release();
			
			switch(sel)	{
				case 1:
					drawPatternsMenu();
					break;
				case 2:
					drawVideoTestsMenu();
					break;
				case 3:
					drawMDFourier();
					break;
				case 4:
					drawMemoryViewer(0);
					break;
				case 5:					
					
					break;
				case 6:
					helpWindow(GENERALHELP);
					break;
				case 7:
					drawCredits(1);
					break;
			}
			reload = 1;
		}
	}
	freeImage(&bg);
	freeImage(&sd);
	SD_release();
	return 0;
}

void drawPatternsMenu(void) {
	int sel = 1, reload = 1, exit = 0;
	joypad_buttons_t keys;
	image *bg = NULL, *sd = NULL;
	
	while(!exit) {
		int c = 1, x = 55, y = 80;
		int r = 0xFF, g = 0xFF, b = 0xFF;
		
		if(reload) {
			freeImage(&bg);
			freeImage(&sd);
			bg = loadImage("rom:/mainbg.sprite");
			sd = loadImage("rom:/sd.sprite");
			if(sd) {
				sd->x = 225;
				sd->y = 75;
			}
			reload = 0;
		}
		
		getDisplay();
		
		rdpqStart();
		rdpqDrawImage(bg);
		rdpqDrawImage(sd);
		SD_blink_cycle(sd);
		rdpqEnd();
		
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Color & Black Levels"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Geometry"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "HCFR Test Patterns"); y += 2*fh; c++;
		drawFooter(x, &y, &sel, &c);
		drawStringS(x, y + fh, r * 0.8, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); y += fh;

		drawSysData();
		checkMenu(GENERALHELP, NULL);
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsDown();

		if(keys.d_up)
			sel--;

		if(keys.d_down)
			sel++;
			
		if(sel > c)
			sel = 1;
		if(sel < 1)
			sel = c;
		
		checkStart(keys);

		if(keys.a) {	
			freeImage(&bg);
			freeImage(&sd);
			SD_release();
			
			switch(sel)	{
				case 1:
					drawPatternsColorMenu();
					break;
				case 2:
					drawPatternsGeometryMenu();
					break;
				case 3:
					drawHCFR();
					break;
				case 4:
					
					break;
				case 5:					
					helpWindow(GENERALHELP);
					break;
				case 6:
					drawCredits(1);
					break;
				case 7:
					exit = 1;
					break;
			}
			reload = 1;
		}
		
		if(keys.b)
			exit = 1;
	}
	freeImage(&bg);
	freeImage(&sd);
	SD_release();
}

void drawPatternsColorMenu(void) {
	int sel = 1, reload = 1, exit = 0;
	joypad_buttons_t keys;
	image *bg = NULL, *sd = NULL;
	
	while(!exit) {
		int c = 1, x = 45, y = 70;
		int r = 0xFF, g = 0xFF, b = 0xFF;
		
		if(reload) {
			freeImage(&bg);
			freeImage(&sd);
			bg = loadImage("rom:/mainbg.sprite");
			sd = loadImage("rom:/sd.sprite");
			if(sd) {
				sd->x = 225;
				sd->y = 75;
			}
			reload = 0;
		}
		
		getDisplay();
		
		rdpqStart();
		rdpqDrawImage(bg);
		rdpqDrawImage(sd);
		SD_blink_cycle(sd);
		rdpqEnd();
		
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "PLUGE"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Color Bars"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "EBU Color Bars"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "SMPTE Color Bars"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Color Bars w/Gray"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Color Bleed Check"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Gray Ramp"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "White & RGB screens"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "100 IRE"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Sharpness"); y += fh; c++;
		drawStringS(x, y + fh, r * 0.8, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Patterns Menu"); y += fh;
		
		drawSysData();
		checkMenu(GENERALHELP, NULL);
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsDown();

		if(keys.d_up)
			sel--;

		if(keys.d_down)
			sel++;
			
		if(sel > c)
			sel = 1;
		if(sel < 1)
			sel = c;
		
		checkStart(keys);
		
		if(keys.a) {	
			freeImage(&bg);
			freeImage(&sd);
			SD_release();
			
			switch(sel)	{
				case 1:
					drawPLUGE();
					break;
				case 2:
					drawColorbars();
					break;
				case 3:
					drawEBUSMPTE(1);
					break;
				case 4:
					drawEBUSMPTE(0);
					break;
				case 5:
					drawCBGray();
					break;
				case 6:
					drawColorbleed();
					break;
				case 7:
					drawGrayramp();
					break;
				case 8:
					drawWhiteScreen();
					break;
				case 9:
					draw100IRE();
					break;
				case 10:
					drawSharpness();
					break;
				case 11:
					exit = 1;
					break;
			}
			reload = 1;
		}
		
		if(keys.b)
			exit = 1;
	}
	freeImage(&bg);
	freeImage(&sd);
	SD_release();
}

void drawPatternsGeometryMenu(void) {
	int sel = 1, reload = 1, exit = 0;
	joypad_buttons_t keys;
	image *bg = NULL, *sd = NULL;
	
	while(!exit) {
		int c = 1, x = 55, y = 90;
		int r = 0xFF, g = 0xFF, b = 0xFF;
		
		if(reload) {
			freeImage(&bg);
			freeImage(&sd);
			bg = loadImage("rom:/mainbg.sprite");
			sd = loadImage("rom:/sd.sprite");
			if(sd) {
				sd->x = 225;
				sd->y = 75;
			}
			reload = 0;
		}

		getDisplay();

		rdpqStart();
		rdpqDrawImage(bg);
		rdpqDrawImage(sd);
		SD_blink_cycle(sd);
		rdpqEnd();
		
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Monoscope"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Grid"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Overscan"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Convergence"); y += fh; c++;
		drawStringS(x, y + fh, r * 0.8, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Patterns Menu"); y += fh;

		drawSysData();
		checkMenu(GENERALHELP, NULL);
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsDown();

		if(keys.d_up)
			sel--;

		if(keys.d_down)
			sel++;
			
		if(sel > c)
			sel = 1;
		if(sel < 1)
			sel = c;

		checkStart(keys);

		if(keys.a) {	
			freeImage(&bg);
			freeImage(&sd);
			SD_release();
			
			switch(sel)	{
				case 1:
					drawMonoscope();
					break;
				case 2:
					drawGrid();
					break;
				case 3:
					drawOverscan();
					break;
				case 4:
					drawConvergence();
					break;
				case 5:
					exit = 1;
					break;
			}
			reload = 1;
		}
		
		if(keys.b)
			exit = 1;
	}
	freeImage(&bg);
	freeImage(&sd);
	SD_release();
}

void drawVideoTestsMenu(void) {
	int sel = 1, reload = 1, exit = 0;
	joypad_buttons_t keys;
	image *bg = NULL, *sd = NULL;
	
	while(!exit) {
		int c = 1, x = 45, y = 55;
		int r = 0xFF, g = 0xFF, b = 0xFF;
		
		if(reload) {
			freeImage(&bg);
			freeImage(&sd);
			bg = loadImage("rom:/mainbg.sprite");
			sd = loadImage("rom:/sd.sprite");
			if(sd) {
				sd->x = 225;
				sd->y = 75;
			}
			reload = 0;
		}
		
		getDisplay();
		
		rdpqStart();
		rdpqDrawImage(bg);
		rdpqDrawImage(sd);
		SD_blink_cycle(sd);
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
		drawStringS(x, y + fh/2, r * 0.8, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); y += fh;

		drawSysData();
		checkMenu(GENERALHELP, NULL);
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsDown();

		if(keys.d_up)
			sel--;

		if(keys.d_down)
			sel++;
			
		if(sel > c)
			sel = 1;
		if(sel < 1)
			sel = c;
		
		checkStart(keys);
		
		if(keys.a) {	
			freeImage(&bg);
			freeImage(&sd);
			SD_release();
			
			switch(sel)	{
				case 1:
					drawDropShadow(0);
					break;
				case 2:
					drawDropShadow(1);
					break;
				case 3:
					drawLagTest();
					break;
				case 4:
					drawTimingReflexTest();
					break;
				case 5:
					drawScroll();
					break;
				case 6:
					drawGridScroll();
					break;
				case 7:
					drawStripes();
					break;
				case 8:
					drawCheckerBoard();
					break;
				case 9:
					drawPhase();
					break;
				case 10:
					drawDisappear();
					break;
				case 11:
					drawLEDZoneTest();
					break;
				case 12:
					drawDiagonalTest();
					break;
				case 13:
					drawAlternate240p480i();
					break;
				case 14:
					exit = 1;
					break;
			}
			reload = 1;
		}
		
		if(keys.b)
			exit = 1;
	}
	freeImage(&bg);
	freeImage(&sd);
	SD_release();
}

void drawIntro() {
	int cancel = 0;
	
	//cancel = drawMCSScreen(100, 0);
	cancel = drawSplash("rom:/libdragon.sprite", cancel ? 0 : LOGO_HOLD, 16);
	cancel = drawSplash("rom:/tiny3d.sprite", cancel ? 0: LOGO_HOLD, 16);
}

void drawFooter(int x, int *y, int *sel, int *c) {
	drawStringS(x, *y, 0xFF, *sel == *c ? 0 : 0xFF, *sel == *c ? 0 : 0xFF, "Options"); *y += fh; (*c)++;
	drawStringS(x, *y, 0xFF, *sel == *c ? 0 : 0xFF, *sel == *c ? 0 : 0xFF, "Help"); *y += fh; (*c)++;
	drawStringS(x, *y, 0xFF, *sel == *c ? 0 : 0xFF, *sel == *c ? 0 : 0xFF, "Credits"); *y += fh; (*c)++;
}