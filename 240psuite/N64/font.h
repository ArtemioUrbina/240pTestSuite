#ifndef FONT_H
#define FONT_H

#include "system.h"

extern int fw; // font width
extern int fh; // font height
 
int loadFont();
void releaseFont();

unsigned int measureString(char *str);

void drawString(int x, int y, int r, int g, int b, char *str);
void drawStringS(int x, int y, int r, int g, int b, char *str);
void drawStringB(int x, int y, int r, int g, int b, char *str);

#endif