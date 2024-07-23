#ifndef IMAGE_H
#define IMAGE_H

#include "system.h"

struct image_st {
	sprite_t *tiles;
	int	x;
	int y;
	bool center;
	bool scale;
};
typedef struct image_st image;

extern bool clearScreen;

void rdpqStart();
void rdpqEnd();
void rdpqDrawImage(image* data);
void rdpqDrawImageXY(image* data, int x, int y);
void rdpqClearScreen();

image *loadImage(char *name);
void freeImage(image **data);

bool copyFrameBuffer();
void freeFrameBuffer();
void displayFrameBuffer();
void darkenBuffer(int amount);

#endif