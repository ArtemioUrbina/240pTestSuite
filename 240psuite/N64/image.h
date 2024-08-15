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
 
#ifndef IMAGE_H
#define IMAGE_H

#include "system.h"

/*
	Verified with vectorscope and Waveform monitor via composite
	on a US001 NTSC console. Equipment used was a VT-150 for waveform 
	and a Tek 1720 color vectorscope.
	Max White is around 94 IRE, and max white form the N64 is 0xf8
	7.5 IRE matched with the ~94% is around 0x17
*/

#define IRE_000		0x00
#define IRE_007_5	0x13
#define IRE_075		0xc0
#define IRE_100		0xf8

// From the PNG it was 0x00, 0x20, 0x48. From vectorscope measurments it is the following
#define COLOR_I		graphics_make_color(0x00, 0x28, 0x40, 0xff)
#define COLOR_Q		graphics_make_color(0x28, 0x00, 0x48, 0xff)

#define COLOR_W		graphics_make_color(0xff, 0xff, 0xff, 0xff)
#define COLOR_R		graphics_make_color(0xff, 0x00, 0x00, 0xff)
#define COLOR_G		graphics_make_color(0x00, 0xff, 0x00, 0xff)
#define COLOR_B		graphics_make_color(0x00, 0x00, 0xff, 0xff)
#define COLOR_M		graphics_make_color(0xff, 0x00, 0xff, 0xff)
#define COLOR_C		graphics_make_color(0x00, 0xff, 0xff, 0xff)
#define COLOR_Y		graphics_make_color(0xff, 0xff, 0x00, 0xff)

#define GREY_25		graphics_make_color(0x2b, 0x2b, 0x2b, 0xff)
#define GREY_50		graphics_make_color(0x80, 0x80, 0x80, 0xff)

typedef struct image_st {
	sprite_t *tiles;
	int	x;
	int y;
	int center;
	int scale;
	int flipH;
	int flipV;
	
	uint16_t *palette;
	uint16_t *origPalette;
	unsigned int palSize;
	unsigned int fadeSteps;
} image;

void setClearScreen();
void frameEnableUpscaler(int enable);

int getDispWidth();
int getDispHeight();

/* Drawing functions */
void rdpqStart();
void rdpqEnd();
void rdpqDrawImage(image* data);
void rdpqDrawImageXY(image* data, int x, int y);
void rdpqFillWithImageXY(image* data, float x, float y);
void rdpqFillWithImage(image* data);
void rdpqClearScreen();
void rdpqClearScreenWhite();
void rdpqDrawRectangle(int tx, int ty, int bx, int by, int r, int g, int b);

image *loadImage(char *name);
void freeImage(image **data);

/* Menu Frame Buffer functions */

int copyMenuFB();
void freeMenuFB();
void drawMenuFB();
void darkenMenuFB(int amount);
int hasMenuFB();

/* Upscaler Frame Buffer functions */
void rdpqUpscalePrepareFB();
int createUpscaleFB();
void freeUpscaleFB();
void executeUpscaleFB();

/* Palette functions */
#define LOGO_HOLD	60

void swapPaletteColors(image *data, unsigned int color1, unsigned int color2);
void swapPalette100to75(image *data);
void swapPalette75to100(image *data);
void updatePalette(image *data);
void setPaletteFX(image *data);
void fadeInit(image *data, unsigned int steps);
void fadeImageStep(image *data);
int drawSplash(char *name, int delay, int paleteSize);


/* Big Numbers */

#define NUMBER_WHITE	0
#define NUMBER_BLACK	1
#define NUMBER_RED		2
#define NUMBER_BLUE		3

void loadNumbers();
void releaseNumbers();
void drawDigit(uint16_t x, uint16_t y, uint16_t color, uint16_t digit);
void drawNumber(uint16_t x, uint16_t y, uint16_t color, uint16_t number);
void drawBlackBox(int x, int y, int width, int height);

#ifdef DEBUG_BENCHMARK
void printPalette(image *data, int x, int y);
#endif

#endif