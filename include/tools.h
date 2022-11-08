/* 
 * 240p Test Suite for the Neo Geo
 * by Artemio Urbina and Dustin Dembrosky (Dasutin)
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

#ifndef _TOOLS_H_
#define _TOOLS_H_

#include "types.h"

void setRandomSeed(u16 seed);
u16 random();

int bcdToDec(int bcd);

// Clear screen
#define MAX_SPRITES	248
void gfxClear();
void suiteClearFixLayer();

void check_systype();

typedef struct blinker{
	picture blink1;
	picture blink2;
	int blink_counter;
	int is_blinking;
} blinker;

void load_blinkdata(blinker* blinkdata, int *index, int *palindex, int x, int y);
int SD_blink_cycle(blinker *blinkdata);
int draw_background_w_gil(blinker *blinkdata);
int draw_background();
void draw_message(char *title, char *msg, int index, int palindex, int clearback);
void draw_warning(char* msg, int index, int palindex, int clearback);

void menu_footer();
void menu_options();

void readController();
void clearController();

void fixPrintC(int y, int color, int font, char *str);

// get Sprite Count from picture
int getPicSprites(pictureInfo *picinfo);
// Scroller Data
#define SCROLLER_SIZE		21

WORD PackColor(short r, short g, short b, BYTE dark);
void UnPackColor(WORD color, short *r, short *g, short *b, short *dark);
void darken_palette(short *pal, short factor);

void waitVLine(WORD line);

// Sound
int sendZ80command(u8 command);
void sendZ80commandAtVideoStart(u8 command);
void sendZ80commandAtLine(WORD line, u8 command);
void sendZ80commandnoWait(u8 command);

#ifdef __cd__
void playCDDA(BYTE track);
void pauseCDDA();
void unPauseCDDA();
void stopCDDA();
#endif

// This is unbuffered, so if a palJobPut is called in the same frame, this will be overwritten
#define VRAM_PAL(palette, color) volMEMWORD(0x400000+palette*32+color*2)

// Edge is Single Press
#define PRESSED_RIGHT	(p1e & JOY_RIGHT)
#define PRESSED_LEFT	(p1e & JOY_LEFT)
#define PRESSED_UP		(p1e & JOY_UP)
#define PRESSED_DOWN	(p1e & JOY_DOWN)
#define PRESSED_A		(p1e & JOY_A)
#define PRESSED_B		(p1e & JOY_B)
#define PRESSED_C		(p1e & JOY_C)
#define PRESSED_D		(p1e & JOY_D)
#define PRESSED_START	(pse & P1_START)
#define PRESSED_SELECT	(pse & P1_SELECT)

// Regular is Held
#define HELD_RIGHT		(p1 & JOY_RIGHT)
#define HELD_LEFT		(p1 & JOY_LEFT)
#define HELD_UP			(p1 & JOY_UP)
#define HELD_DOWN		(p1 & JOY_DOWN)
#define HELD_A			(p1 & JOY_A)
#define HELD_B			(p1 & JOY_B)
#define HELD_C			(p1 & JOY_C)
#define HELD_D			(p1 & JOY_D)
#define HELD_START		(ps & P1_START)
#define HELD_SELECT		(ps & P1_SELECT)

#define BTTN_MAIN		PRESSED_A
#define BTTN_EXIT		PRESSED_B
#define BTTN_OPTION_1	PRESSED_C
#define BTTN_OPTION_2	PRESSED_D
#define BTTN_HELP		PRESSED_START

#define NTSC_304		0
#define NTSC_320		320
#define PAL_304			640
#define PAL_320			960

#define	PATTERN_SCROLL	16

int getHorScroll();
int getHorScrollAspect();

WORD getVideoline();
void getScreenLimits(int* x, int* y);
void displayRegByte(u16 x, u16 y, char *dispname, u32 regAddr);
void displayRegWord(u16 x, u16 y, char *dispname, u32 regAddr);
void displayValue(u16 x, u16 y, char *dispname, int value);
int getCreditCount();
BYTE getHardDipValue(BYTE harddip);
int getSoftDipvalue(u32 softdip);
#endif /* _TOOLS_H_ */