#ifndef FONT_H
#define FONT_H

#include "system.h"

int loadFont();
void releaseFont();

void drawString(int x, int y, int r, int g, int b, char *str);
void drawStringS(int x, int y, int r, int g, int b, char *str);
void drawStringB(int x, int y, int r, int g, int b, char *str);

#endif