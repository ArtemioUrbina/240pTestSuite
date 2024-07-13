#ifndef IMAGE_H
#define IMAGE_H

#include "system.h"

void rdpqStart();
void rdpqEnd();
void rdpqDrawImage(sprite_t* tiles, float x, float y);
void rdpqClearScreen();

#endif