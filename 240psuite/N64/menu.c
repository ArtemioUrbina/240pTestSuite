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

void checkMenu(char *help, int *reload) {
	if(showMenuSet)
	{
		helpData = help;
		showMenu();
		showMenuSet = 0;
		if(reload)
			*reload = 1;
	}
}

void checkStart(joypad_buttons_t keys) {
	if(keys.start) {
		showMenuSet = 1;
	}
}

void showMenu() {	
	int end = 0, sel = 1;
	image *menu = NULL;
	joypad_buttons_t keys;
	
	copyFrameBuffer();
	waitVsync();
	
	darkenBuffer(0x30);
	
	menu = loadImage("rom:/menu.sprite");
	if(!menu) {
		freeFrameBuffer();
		return;
	}
	menu->center = true;
	
	while(!end) {
		int c = 1, x, y;
		int r = 0xFF, g = 0xFF, b = 0xFF;
		
		getDisplay();

		displayFrameBuffer();
		
		rdpqStart();
		rdpqDrawImage(menu);
		rdpqEnd();
		
		x = menu->x+16;
		y = menu->y+20;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Help"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Video"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Options"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Credits"); y += 2*fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Close"); 
		
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
		
		if(keys.b || keys.start)
			end = 1;
		if(keys.a) {
			switch(sel)	{
				case 1:
					helpWindow(helpData);
					break;
				case 2:
					selectVideoMode();
					break;
				case 5:
					end = 1;
					break;
			}
		}
	}
	
	freeFrameBuffer();
	freeImage(&menu);	
}

void selectVideoMode() {
	resolution_t 	oldVmode = current_resolution;
	int 			sel = 1, close = 0;
	image	 		*back = NULL;
	joypad_buttons_t keys;
	
	back = loadImage("rom:/help.sprite");
	if(!back)
		return;
	
	sel = videoModeToInt(&current_resolution) + 1;
	while(!close) {		
		int     r = 0xff;
		int     g = 0xff;
		int     b = 0xff;
		int   	c = 1;   
		int     x = 40;
		int     y = 58;
				
		getDisplay();

		displayFrameBuffer();
		rdpqStart();
		rdpqDrawImage(back);
		rdpqEnd();

		drawStringS(x - 20, y, 0x00, 0xff, 0x00, "Please select the desired video mode"); y += 3*fh; 
			
		drawStringS(x - 10, y + videoModeToInt(&current_resolution)*fh, 0x00, 0xff, 0x00, ">"); 
		
		drawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "240p"); y += fh; c++;
		//drawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "480i scaled 240p assets (NTSC)"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "480i mixed 480/240 assets (1:1/NTSC)"); y += fh; c++;				
		
		y += fh/2;
		if(EnablePAL) {
			drawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "288p"); y += fh; c++;
			//drawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "576i scaled 264/240p assets (PAL)"); y += fh; c++;
			drawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "576i 528/480/240 assets (PAL)"); c++;
		}
		else {
			drawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "288p"); y += fh; c++;
			//drawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "576i scaled 264/240p assets (stretched/PAL)"); y += fh; c++;
			drawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "576i 528/480/240 assets (PAL)"); c++;			
		}			
		
		y += fh/2;
			
		drawStringS(x, y + fh, r-0x40, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); 		
				
		drawStringS(x+40, 200, r, g, b, "Press START for help");
	
		waitVsync();

		joypad_poll();
		keys = controllerButtonsDown();
		
		if(keys.d_up) {
		    sel --;
		    if(sel < 1)
			    sel = c;		
	    }
	    
	    if(keys.d_down) {
		    sel ++;
		    if(sel > c)
			    sel = 1;	
	    }	

		if(keys.start) {
			char *oldHelp = helpData;
			
			helpWindow(VIDEOHELP);
			helpData = oldHelp;
		}		
			
		if(keys.b)
			close = 1;	
	
		if(keys.a) {     
			switch(sel)	{			
				case 1:						
					useNTSC = 1;
					setVideo(RESOLUTION_320x240);
					break;
				case 2:
					useNTSC = 1;
					setVideo(RESOLUTION_640x480);
					break;
				case 3:
					if(EnablePAL) {
						useNTSC = 0;
						setVideo(RESOLUTION_320x240);
					}
					break;
				case 4:
					if(EnablePAL) {
						useNTSC = 0;
						setVideo(RESOLUTION_640x480);
					}
					break;		
				case 5:
					close = 1;
					break;
				default:
					break;
			}
		}		
	}
	freeImage(&back);
	if(!isSameRes(&oldVmode, &current_resolution))
		clearScreen = true;
}