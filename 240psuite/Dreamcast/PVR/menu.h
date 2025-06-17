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

#include <stdlib.h>
#include "image.h"

#define VERSION_NUMBER "Dreamcast Ver. 2.07"
#define VERSION_DATE "04/07/2024"

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
	
#define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

#ifndef DCLOAD
 #define DEFAULT_OPTIONS { 0, 0, PAL_CENTERED, 0, 0, 0, 0.7, 0.7, 0.7, 0, 0, 0, 0.3f, 0, 0 };
#else
 #define DEFAULT_OPTIONS { 0, 0, PAL_CENTERED, 0, 0, 1, 0.7, 0.0, 0.7, 0, 0, 0, 0.3f, 0, 0 };
#endif

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
	int IgnoreFrameBuffer;
	float scanlines;
	int	scanlines_even;
	int	matchIRE;
};

extern struct settings_st settings;
extern int is_system_mono;

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
void SD_blink_cycle(ImagePtr sd);

#ifndef MENUDATA
#define MENUDATA
typedef struct menu_data {
    int		option_value;
	char	*option_text;
} fmenudata;
#endif

#define MENU_CANCEL -1

int SelectMenu(char *title, fmenudata *menu_data, int num_options, int selected_option);
int SelectMenuEx(char *title, fmenudata *menu_data, int num_options, int selected_option, char *helpfile);
int AskQuestion(char *msg);
int AskQuestionText(char *msg, char *replies);
void DrawMessage(char *msg);
void DrawMessageOnce(char *msg);
int DrawMessageInternal(char *msg, int waitinput, int isquestion, char *replies);

#define FB_MENU_NORMAL		1
#define FB_MENU_HELP		2
#define FB_MENU_CREDITS		3
extern ImagePtr	fbtexture;

uint8 BackupFBTexture();
void FreeTextureFB();
void ShowLCDVMUWarning();

#endif
