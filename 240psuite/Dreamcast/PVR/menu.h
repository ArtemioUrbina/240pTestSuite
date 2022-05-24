/* 
 * 240p Test Suite
 * Copyright (C)2011-2022 Artemio Urbina
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef MENU_H
#define MENU_H

#include "image.h"

#define VERSION_NUMBER "Dreamcast Ver. 1.27"
#define VERSION_DATE "24/05/2022"

#define DEFAULT_OPTIONS { 0, 0, PAL_CENTERED, 0, 0, 0, 0.7, 0.7, 0.7, 0, 0};

#define MENUSIZE_W 116
#define MENUSIZE_H 123

#define FB_TEX_H		1024
#define FB_TEX_V		512
#define FB_TEX_BYTES	sizeof(uint16)

#define rotr(value, shift) \
	(value >> shift) | (value << (16 - shift))
	
#define VMURefresh(msg1, msg2) \
	if(refreshVMU)\
	{\
		updateVMU(msg1, msg2, 1);\
		refreshVMU = 0;\
	}

struct settings_st {
	int drawborder;
	int EnablePAL;
	int PALStart;
	int ChangeVideoRegs;
	int Deflicker;
	int EnablePALBG;
	float PalBackR;
	float PalBackG;
	float PalBackB;
	int Dithering;
	int UseKOSDefaults;
};

extern struct settings_st settings;

extern uint8 EndProgram;
extern uint8 refreshVMU;
void ChangeOptions(ImagePtr screen);
void SelectVideoMode(ImagePtr screen);
void DrawShowMenu();
void ShowMenu(char *HelpData);
uint8 ReloadFBTexture();
void DrawCreditsOnFB();
void DrawCredits(ImagePtr back);
void DrawIntro();
void ShowHelpWindow(char *Data);
void ChangePALBackgroundColor(ImagePtr title);

#ifndef MENUDATA
#define MENUDATA
typedef struct menu_data {
    int		option_value;
	char	*option_text;
} fmenudata;
#endif

#define MENU_CANCEL -1

int SelectMenu(char *title, fmenudata *menu_data, int num_options, int selected_option);

#endif
