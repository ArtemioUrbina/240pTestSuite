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
int enableVideoOption = 1;

void checkMenu(char *help, int *reload) {
	if(showMenuSet)	{
		int return256 = 0;
		
		if(isVMode256()) {
			changeToH320onVBlank();
			return256 = 1;
		}
		
		helpData = help;
		showMenu();
		
		if(return256) {
			changeToH256onVBlank();
			return256 = 0;
		}
		
		if(reload)
			*reload = 1;
		showMenuSet = 0;
	}
}

void checkStart(joypad_buttons_t keys) {
	if(keys.start) {
		showMenuSet = 1;
	}
}

void setMenuVideo(int showVideoOption) {
	enableVideoOption = showVideoOption;
}

void showMenu() {	
	int end = 0, sel = 1;
	image *menu = NULL;
	joypad_buttons_t keys;
	
	copyMenuFB();
	waitVsync();
	
	darkenMenuFB(0x30);
	
	menu = loadImage("rom:/menu.sprite");
	if(!menu) {
		freeMenuFB();
		return;
	}
	menu->center = 1;
	
	while(!end) {
		char str[20];
		int c = 1, x, y;
		int r = 0xFF, g = 0xFF, b = 0xFF;
		
		getDisplay();

		drawMenuFB();
		
		rdpqStart();
		rdpqDrawImage(menu);
		rdpqEnd();
		
		drawStringC(menu->y+8, 0x00, 0xff, 0x00, VERSION_NUMBER); y += 2*fh; 		
		
		x = menu->x+16;
		y = menu->y+20;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Help"); y += fh; c++;
		sprintf(str, "Video");
		getVideoModeStr(str+5, 1);
		if(enableVideoOption) {
			drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, str); y += fh; c++;
		}
		else {
			drawStringS(x, y, sel == c ? 0xff:0x98, sel == c ? 0xaa:0x98, sel == c ? 0xaa:0x98, str); y += fh; c++;
		}
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
					if(enableVideoOption)
						selectVideoMode(1);
					break;
				case 4:
					drawCredits(1);
					break;
				case 5:
					end = 1;
					break;
			}
		}
	}
	
	freeMenuFB();
	freeImage(&menu);	
}

void selectVideoMode(int useBack) {
	resolution_t 	oldVmode = current_resolution;
	int 			sel = 1, close = 0;
	image	 		*back = NULL;
	joypad_buttons_t keys;
	
	back = loadImage("rom:/help.sprite");
	if(!back)
		return;
	back->center = 1;
	
	sel = videoModeToInt(&current_resolution) + 1;
	while(!close) {		
		int     r = 0xff;
		int     g = 0xff;
		int     b = 0xff;
		int   	c = 1, x, y;
				
		getDisplay();

		if(useBack)
			drawMenuFB();
		rdpqStart();
		rdpqDrawImage(back);
		rdpqEnd();

		x = back->x + 48;
		y = back->y + 17;
		
		drawStringC(y, 0xff, 0xff, 0xff, "240p Test Suite Video Modes"); y += 3*fh; 
		
		drawStringC(y, 0x00, 0xff, 0x00, "Please select the desired mode"); y += 3*fh; 
			
		drawStringS(x - 10, y + videoModeToInt(&current_resolution)*fh, 0x00, 0xff, 0x00, ">"); 
		
		drawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "240p"); y += fh; c++;
		//drawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "480i scaled 240p assets (NTSC)"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "480i mixed 480/240 assets"); y += fh; c++;				
		
		y += fh/2;
		if(enablePAL) {
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
				
		drawStringC(back->y+216, r, g, b, "Press START for help");
	
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
					setVideo(RESOLUTION_320x240);
					break;
				case 2:
					setVideo(RESOLUTION_640x480);
					break;
				case 3:
					if(enablePAL)
						setVideo(RESOLUTION_320x240);
					break;
				case 4:
					if(enablePAL)
						setVideo(RESOLUTION_640x480);
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
		setClearScreen();
}

void drawCredits(int usebuffer) {
	int 		done = 0;	
    int     	counter = 1;
	char		data[50];
	image		*back = NULL, *qr = NULL;
	joypad_buttons_t keys;
	
	back = loadImage("rom:/help.sprite");
	if(!back)
		return;
		
	qr = loadImage("rom:/qr.sprite");
		
	while(!done) {
		int x = 35, y = 40, x2 = 150, y2 = 0;

		getDisplay();

		if(usebuffer)
			drawMenuFB();
		rdpqStart();
		rdpqDrawImage(back);
		rdpqDrawImageXY(qr, 236, 50);
		rdpqEnd();
		
        drawStringS(x, y, 0x00, 0xff, 0x00, "Code & Patterns:"); y += fh; 
		if(counter == 1)
			sprintf(data, "Artemio Urbina");			
		if(counter == 60*4)
			sprintf(data, "@Artemio (twitter)");					
		if(counter == 60*8)
			counter = 0;

		drawStringS(x, y, 0x00, 0xff, 0x00, "Support & suggestions:"); y += fh; 
		drawStringS(x+5, y, 0xff, 0xff, 0xff, data); y += fh; 

		drawStringS(x, y, 0x00, 0xff, 0x00, "SDK:"); y += fh; 
		drawStringS(x+5, y, 0xff, 0xff, 0xff, "libDragon"); y += fh; y2 = y;
		drawStringS(x2, y2, 0x00, 0xff, 0x00, "Monoscope Pattern:"); y2 += fh; 
		drawStringS(x2+5, y2, 0xff, 0xff, 0xff, "Keith Raney\n(@khmr33)"); y2 += 2*fh;
		drawStringS(x, y, 0x00, 0xff, 0x00, "Donna Art:"); y += fh; 
		drawStringS(x+5, y, 0xff, 0xff, 0xff, "Jose Salot\n(@pepe_salot)"); y += 2*fh;
		drawStringS(x2, y2, 0x00, 0xff, 0x00, "Menu Pixel Art:"); y2 += fh; 
		drawStringS(x2+5, y2, 0xff, 0xff, 0xff, "Asher"); y2 += fh;
		drawStringS(x, y, 0x00, 0xff, 0x00, "Advisor:"); y += fh; 
		drawStringS(x+5, y, 0xff, 0xff, 0xff, "Fudoh"); y += fh; 
		drawStringS(x2, y2, 0x00, 0xff, 0x00, "Collaboration:"); y2 += fh; 
		drawStringS(x2+5, y2, 0xff, 0xff, 0xff, "shmups regulars"); y2 += fh+5;
		y = y2;
	
		drawStringS(x, y, 0x00, 0xff, 0x00, "Info on using this suite:"); y += fh; 
		drawStringS(x+5, y, 0xff, 0xff, 0xff, "#Yhttp://junkerhq.net/240p/#Y"); y += fh+5; 

		drawStringS(x-8, y, 0x00, 0xba, 0xba, "This program is free Software");  y += fh;
		drawStringS(x-8, y, 0x00, 0xba, 0xba, "Source code is available under GPL");
		
		drawStringS(140, 216, 0x00, 0xba, 0xba, "Dedicated to Elisa.");
		
		y = 28;
		
		drawStringS(208, y, 0x0f, 0xff, 0xff, VERSION_NUMBER); y += fh;
		drawStringS(208, y, 0x0f, 0xff, 0xff, VERSION_DATE); y += 2*fh;

		waitVsync();

		joypad_poll();
		keys = controllerButtonsDown();
		
		if(keys.b)
			done =	1;		
		
		if(keys.z) {
			int 	check = 1;
			image 	*nish = NULL;	
			
			nish = loadImage("rom:/nish.sprite");
			if(nish) {
				while(check) {
					getDisplay();
					rdpqStart();
					rdpqDrawImage(nish);
					rdpqEnd();
					waitVsync();
					
					joypad_poll();
					keys = controllerButtonsHeld();
					if(!keys.z)
						check =	0;	
				}
				freeImage(&nish);
			}
		}
		
		counter ++;			
	}
	
	freeImage(&back);
	freeImage(&qr);
}

/* Floating Menu functions */

int selectMenu(char *title, fmenuData *menuData, int numOptions, int selectedOption) {
	return(selectMenuEx(title, menuData, numOptions, selectedOption, NULL));
}

int selectMenuEx(char *title, fmenuData *menuData, int numOptions, int selectedOption, char *helpFile) {
	int 		sel = selectedOption, close = 0, value = MENU_CANCEL;
	image		*back = NULL;
	
	back = loadImage("rom:/menu.sprite");
	if(back)
		back->center = 1;
	  
	setClearScreen();
	while(!close) {		
		uint8_t		r = 0xff;
		uint8_t		g = 0xff;
		uint8_t		b = 0xff;
		uint8_t		c = 1, i = 0;
		uint16_t 	x = 0;
		uint16_t 	y = 0;
		joypad_buttons_t keys;
		
		getDisplay();
		
		rdpqStart();
		rdpqDrawImage(back);
		rdpqEnd();

		x = back->x + 8;
		y = back->y + 8;
		drawStringC(y, 0x00, 0xff, 0x00, title); y += 3*fh;

		if(numOptions > 6)
			y -= fh;	
		
		for(i = 0; i < numOptions; i++) {
			drawStringS(x+8, y, r, sel == c ? 0 : g, sel == c ? 0 : b, menuData[i].optionText);
			y += fh; c++;		
		}
		
		y += fh;
		
		if(numOptions <= 6)
			y += fh;
		
		drawStringC(back->y+80, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Close Menu");
		
		if(helpFile)
			drawStringC(y+2*fh, 0xff, 0xff, 0xff, "Press #YSTART#Y for help");

		waitVsync();

		joypad_poll();
		keys = controllerButtonsDown();
		
		if(keys.d_up) {
			sel --;
			if(sel < 1)
				sel = c;
		}
		
		if(keys.d_down)	{
			sel ++;
			if(sel > c)
				sel = 1;
		}			

		if(keys.b || (!helpFile && keys.start)) {
			close = 1;	
			value = MENU_CANCEL;
		}
		
		if(helpFile && keys.start) {
			helpWindow(helpFile);
			helpData = GENERALHELP;
		}
	
		if(keys.a) {
			close = 1;

			if(sel == c)
				value = MENU_CANCEL;
			else
				value = menuData[sel - 1].optionValue;
		}		
	}
	
	freeImage(&back);

	return value;
}

void drawMessageBox(char *msg) {	
	int 		done = 0;	
	image		*back = NULL;
	
	back = loadImage("rom:/message.sprite");
	if(back)
		back->center = 1;
	  
	setClearScreen();
	while(!done) {			
		uint8_t		r = 0xff;
		uint8_t		g = 0xff;
		uint8_t		b = 0xff;
		joypad_buttons_t keys;
				
		getDisplay();
		
		rdpqStart();
		rdpqDrawImage(back);
		rdpqEnd();

		drawStringC(113, r, g, b, msg);
				
		drawStringC(182, 0, g, b, "Press B to continue"); 
		
		waitVsync();
			
		joypad_poll();
		keys = controllerButtonsDown();
		
		if(keys.b || keys.start)
			done = 1;
	}
	
	freeImage(&back);
}

image *SD_b1 = NULL;
image *SD_b2 = NULL;
	
void SD_blink_cycle(image *sd) {
	static int blink_counter = 0;
	static int is_blinking = 0;
	
	if(!sd)
		return;
		
	if(!SD_b1)
		SD_b1 = loadImage("rom:/sd_b1.sprite");

	if(SD_b1) {
		SD_b1->x = sd->x+16;
		SD_b1->y = sd->y+32;
	}
	
	if(!SD_b2)
		SD_b2 = loadImage("rom:/sd_b2.sprite");
	
	if(SD_b2) {
		SD_b2->x = sd->x+16;
		SD_b2->y = sd->y+32;
	}
	
	blink_counter++;	
	if(SD_b1 && SD_b2 && blink_counter > 230)
	{
		if(!is_blinking) {
			// 2% chance every frame after 240
			if(RANDN(100) < 98)	{
				is_blinking = 1;
				blink_counter = 230;
				rdpqDrawImage(SD_b1);
			}
		}
		else {
			if(blink_counter >= 232 && blink_counter < 234)
				rdpqDrawImage(SD_b2);
				
			if(blink_counter >= 234 && blink_counter < 238)
				rdpqDrawImage(SD_b1);
	
			if(blink_counter >= 238) {	
				blink_counter = 0;
				is_blinking = 0;
			}
		}
	}
}

void SD_release() {
	freeImage(&SD_b1);
	freeImage(&SD_b2);
}