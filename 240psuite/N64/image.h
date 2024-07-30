#ifndef IMAGE_H
#define IMAGE_H

#include "system.h"

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

void rdpqStart();
void rdpqEnd();
void rdpqDrawImage(image* data);
void rdpqDrawImageXY(image* data, int x, int y);
void rdpqFillWithImage(image* data);
void rdpqClearScreen();

image *loadImage(char *name);
void freeImage(image **data);

int copyMenuFB();
void freeMenuFB();
void drawMenuFB();
void darkenMenuFB(int amount);

void rdpqUpscalePrepareFB();
int createUpscaleFB();
void freeUpscaleFB();
void executeUpscaleFB();

void swapPaletteColors(image *data, unsigned int color1, unsigned int color2);
void updatePalette(image *data);
void setPaletteFX(image *data);
void fadeInit(image *data, unsigned int steps);
void fadeImageStep(image *data);

#ifdef DEBUG_BENCHMARK
void printPalette(image *data, int x, int y);
#endif

#endif