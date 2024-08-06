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
#include "hcfr.h"

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
			rdpqDrawImageXY(borderGreen, 44, 39);
			rdpqDrawImageXY(borderRed, 74, 39);
			
			rdpqDrawImageXY(borderRed, 228, 39);
			rdpqDrawImageXY(borderGreen, 259, 39);
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

#define MAX_COLORS	8

void drawWhiteScreen() {
	int end = 0, color = 0, blackLevel = IRE_000, text = 0;
	int	cr, cb, cg, sel = 1, editmode = 0, er, eb, eg;
	char msg[100], *mode[MAX_COLORS] = { "White", "Black", "Red", "Green", "Blue", "Yellow", "Cyan", "Magenta"};
	joypad_buttons_t keys;
	
	if(!isPAL)
		blackLevel = IRE_007_5; // 7.5 IRE 
	else
		blackLevel = IRE_000; // 0 IRE
	
	// white
	cr = cg = cb = IRE_100;
	er = eb = eg = IRE_100;
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
				blackLevel = IRE_007_5;
				sprintf(msg, "#GBlack Level: 7.5 IRE#G");
			}
			else {
				blackLevel = IRE_000;
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
					if(*current > IRE_100)
						*current = IRE_100;
				}
			}
			
			if(keys.d_down) {
				if(current)	{
					(*current) --;
					if(*current < IRE_000)
						*current = IRE_000;
				}
			}	

			if(keys.c_down) {
				if(current)
					*current = IRE_000;
			}
			
			if(keys.c_up)	{
				if(current)	
					*current = IRE_100;
			}
			
			if(keys.c_right) {
				if(current)	{
					(*current) += 0x10;
					if(*current > IRE_100)
						*current = IRE_100;
				}
			}
			
			if(keys.c_left) {			
				if(current)	{
					(*current) -= 0x10;
					if(*current < IRE_000)
						*current = IRE_000;
				}
			}	
		}
		
		if(keys.r) {
			color ++;
			if(color > MAX_COLORS-1)
				color = 0;
			
			editmode = 0;
			if(color == 0 && er + eb + eg != 3*IRE_100)
				sprintf(msg, "%s [EDITED]", mode[color]);
			else
				sprintf(msg, "%s", mode[color]);
			text = 30;
		}

		if(keys.l) {			
			color --;
			if(color < 0)
				color = MAX_COLORS-1;
				
			editmode = 0;
			if(color == 0 && er + eb + eg != 3*IRE_100)
				sprintf(msg, "%s [edited]", mode[color]);
			else
				sprintf(msg, "%s", mode[color]);
			text = 30;
		}
		
		switch(color) {
				case 0:	// Custom or White
					cr = er;
					cg = eg;
					cb = eb;
					break;
				case 1:	// Black
					cr = cg = cb = blackLevel;
					break;
				case 2:	// Red
					cr = IRE_100;
					cb = cg = IRE_000;
					break;
				case 3:	// Green
					cg = IRE_100;
					cr = cb = IRE_000;
					break;
				case 4:	// Blue
					cb = IRE_100;
					cr = cg = IRE_000;
					break;
				case 5:  // Yellow
					cr = cg = IRE_100;
					cb = IRE_000;
					break;
				case 6:  // Cyan
					cb = cg = IRE_100;
					cr = IRE_000;
					break;
				case 7:  // Magenta
					cr = cb = IRE_100;
					cg = IRE_000;
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
	int		end = 0, pattern = 0, changed = 1;
	image	*fbx = NULL, *keith = NULL;
	int		whiteGrid = 0xff, greenBorder = 0xff;
	joypad_buttons_t keys;
	
	fbx = loadImage("rom:/monoscopeFBX.sprite");
	if(!fbx)
		return;
	keith = loadImage("rom:/monoscope.sprite");
	if(!keith) {
		freeImage(&fbx);
		return;
	}
	
	while(!end) {
		getDisplay();

		rdpqStart();
		if(!pattern)
			rdpqDrawImage(fbx);
		else
			rdpqDrawImage(keith);
		rdpqEnd();

		checkMenu(MONOSCOPEHLP, NULL);
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsDown();
		
		checkStart(keys);
		if(keys.a)
			pattern = !pattern;
			
		if(keys.l) {
			whiteGrid -= 0x10;
			changed = 1;
		}
		
		if(keys.r) {
			whiteGrid += 0x10;
			changed = 1;
		}
		
		if(keys.c_left && !pattern) {
			greenBorder = 0x00;
			changed = 1;
		}
		
		if(keys.c_right && !pattern) {
			greenBorder = 0xff;
			changed = 1;
		}
		
		if(keys.c_up) {
			whiteGrid = 0xff;
			greenBorder = 0xff;
			changed = 1;
		}
		
		if(keys.c_down) {
			whiteGrid = 0;
			greenBorder = 0;
			changed = 1;
		}
		
		if(changed) {
			if(whiteGrid < 0)
				whiteGrid = 0;
			if(whiteGrid > 0xff)
				whiteGrid = 0xff;
			
			fbx->palette[1] = graphics_make_color(0, greenBorder, 0, 0xff);
			fbx->palette[3] = graphics_make_color(whiteGrid, whiteGrid, whiteGrid, 0xff);
			updatePalette(fbx);
			
			keith->palette[3] = graphics_make_color(whiteGrid, whiteGrid, whiteGrid, 0xff);
			updatePalette(keith);
			changed = 0;
		}
		
		if(keys.b)
			end = 1;
	}
	
	freeImage(&fbx);
	freeImage(&keith);
}

#define NUM_RES 4

void drawGrid() {
	int end = 1, gridResolution = 0, tmpRes = 0;
	image *back[NUM_RES] = { NULL, NULL, NULL, NULL };
	joypad_buttons_t keys;
	fmenuData	verMenuData[NUM_RES] = { 
										{ 0, "320x240" },
										{ 1, "256x240" },
										{ 2, "640x480" },
										{ 3, "512x480" } };
	char *grids[NUM_RES] = { 			"rom:/grid.sprite", 
										"rom:/grid-256.sprite", 
										"rom:/grid-480.sprite",
										"rom:/grid-512.sprite" };
	resolution_t targetRes[NUM_RES] = {	RESOLUTION_320x240,
										RESOLUTION_256x240,
										RESOLUTION_640x480,
										RESOLUTION_512x480 };
	resolution_t	oldVMode = current_resolution;
	
	for(unsigned int res = 0; res < NUM_RES; res ++) {
		back[res] = loadImage(grids[res]);
		if(!back[res])
			return;
	}
	
	if(back[2])
		back[2]->scale = 0;
	if(back[3])
		back[3]->scale = 0;
	
	tmpRes = selectMenu("Select", verMenuData, NUM_RES, gridResolution+1);
	if(tmpRes != MENU_CANCEL) {
		gridResolution = tmpRes;
		end = 0;
	}
	
	setMenuVideo(0);
	changeVMode(targetRes[gridResolution]);
	while(!end) {
		getDisplay();

		rdpqStart();
		
		rdpqDrawImage(back[gridResolution]);
		rdpqEnd();
		
		checkMenu(GRIDHELP, NULL);
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsDown();
		
		checkStart(keys);
		if(keys.b)
			end = 1;
			
		if(keys.r) {
			tmpRes = selectMenu("Select", verMenuData, NUM_RES, gridResolution+1);
			if(tmpRes != MENU_CANCEL) {
				gridResolution = tmpRes;
				changeVMode(targetRes[gridResolution]);
			}
		}
	}
	
	for(unsigned int res = 0; res < NUM_RES; res ++)
		freeImage(&back[res]);
	
	changeVMode(oldVMode);
	setMenuVideo(1);
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
	
	drawSplash("rom:/grayramp.sprite", 0, 32);
}

void draw100IRE() {
	int 			done = 0;
	int				irecount = 10, iremax = 10;  
	int				text = 0, invert = 0, ireValue = 0;
	char			msg[50];
	int				*irevalues = NULL;
	int				irevalues100[11] = { 0, 26, 51, 77, 102, 128, 153, 179, 204, 230, 255};
	int				irevalues140[5] = { 0, 63, 127, 190, 255 };	
	joypad_buttons_t keys;
	
	irevalues = irevalues100;
	ireValue = irevalues[irecount];
	while(!done) {		
		getDisplay();
		
		rdpqStart();
		rdpqClearScreen();
		rdpqDrawRectangle(80, 60, 240, 180, ireValue, ireValue, ireValue);
		rdpqEnd();
		
		if(text) {
			if(!invert) {
				if(text > 30)
					sprintf(msg, "RANGE 0-100 IRE");
				else
					sprintf(msg, "XXX IRE");  // TODO IRE VALUE
			  	drawStringS(180, 225, 0xff, 0xff, 0xff, msg);
			  	text --;
	  		}
	  		else {
				if(text > 30)
					sprintf(msg, "RANGE 100-140 IRE");
				else
					sprintf(msg, "XXX IRE");  // TODO IRE VALUE
			  	drawStringS(180, 225, 0xff, 0xff, 0xff, msg);
			  	text --;
	  		}
		}
		checkMenu(IREHELP, NULL);
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsDown();
		
		if(keys.l) {
	  		if(!invert)	{
				irecount --;
				
				if(irecount < 0)
					irecount = 0;
			}
			else {
				irecount ++;

				if(irecount > iremax)
					irecount = iremax;
			}

			text = 30;
			ireValue = irevalues[irecount];
		}
	
		if(keys.r) {
	  		if(invert) {
				irecount --;
				
				if(irecount < 0)
					irecount = 0;
			}
			else {	 
				irecount ++;
				
				if(irecount > iremax)
					irecount = iremax;
			}

			text = 30;
			ireValue  = irevalues[irecount];
		}
		
		if(keys.a) {
			invert = !invert;
			ireValue = 0xff;
			text = 60;

			if(invert) {
				irevalues = irevalues140;
				iremax = 4;
				irecount = 4;
			}
			else {
				irevalues = irevalues100;
				iremax = 10;
				irecount = 10;
			}
		}
	
		if(keys.b)
			done =	1;

		checkStart(keys);
	}
}

void drawHCFR() {
	char 			msg[50];
	int 			done = 0, hcfr_num = 0, hcfr_type = 0, i = 0;
	fmenuData		fmenu[HCFR_TYPES+1];
	joypad_buttons_t keys;

	for(i = 0; i < HCFR_TYPES; i++)
	{
		fmenu[i].optionValue = i;
		fmenu[i].optionText = hcfr_data[i].name;
	}

	hcfr_type = selectMenuEx("Select Grid", fmenu, HCFR_TYPES, hcfr_type+1, HCFRMENU);
	if(hcfr_type == MENU_CANCEL)
		return;
	
	while(!done) {
		int	r, g, b;
		
		r = hcfr_data[hcfr_type].data[hcfr_num].r;
		g = hcfr_data[hcfr_type].data[hcfr_num].g;
		b = hcfr_data[hcfr_type].data[hcfr_num].b;
		
		getDisplay();
		
		rdpqStart();
		rdpqClearScreen();
		rdpqDrawRectangle(106, 80, 212, 160, r, g, b);  // 1/3th
		rdpqEnd();
		
		sprintf(msg, "%s %03d,%03d,%03d", 
				hcfr_data[hcfr_type].data[hcfr_num].name, r, g, b);

		drawStringC(2*getDispHeight()/3+fh, 0x7f, 0x7f, 0x7f, msg);
		
		checkMenu(HCFR, NULL);
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsDown();
		
		if(keys.b)
			done =	1;
		
		if(keys.d_left || keys.l) {
			if (hcfr_num > 0)
				hcfr_num--;
		}

		if(keys.d_right || keys.r) {
			if (hcfr_num < HCFR_LEN)
				hcfr_num++;
		}

		if(keys.c_right) {
			int tmp_hcfr_type = 0;

			tmp_hcfr_type = selectMenuEx("Select Standard", fmenu, HCFR_TYPES, hcfr_type+1, HCFRMENU);
			if(tmp_hcfr_type != MENU_CANCEL && tmp_hcfr_type != hcfr_type)			{
				hcfr_type = tmp_hcfr_type;
				hcfr_num = 0;
			}
		}

		if(keys.c_left)
			hcfr_num = 0;
			
		checkStart(keys);
	}
}

#define	NUM_CONV	5
void drawConvergence()
{
	int 		done = 0, i = 0, current = 0;
	char		*patterns[NUM_CONV] = { "rom:/convergence-01-grid.sprite",
										"rom:/convergence-02-cross.sprite",
										"rom:/convergence-03-dots.sprite",
										"rom:/convergence-04-colors.sprite",
										"rom:/convergence-05-colorsbl.sprite" };
	image		*back[NUM_CONV];
	joypad_buttons_t keys;
	
	for(i = 0; i < NUM_CONV; i++) {
		back[i] = loadImage(patterns[i]);
		if(!back[i])
			return;
	}
	
	while(!done) {	
		getDisplay();

		rdpqStart();
		rdpqFillWithImage(back[current]);
		rdpqEnd();
		
		checkMenu(CONVERHELP, NULL);
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsDown();
		
		checkStart(keys);

		if(keys.b)
			done =	1;
		
		if(keys.a || keys.r) {
			current ++;
			if(current >= NUM_CONV)
				current = 0;
		}
		
		if (keys.l)	{
			current --;
			if(current < 0)
				current = NUM_CONV - 1;
		}
	}
	
	for(i = 0; i < NUM_CONV; i++)
		freeImage(&back[i]);
	return;
}