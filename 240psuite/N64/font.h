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
 
#ifndef FONT_H
#define FONT_H

#include "system.h"

extern int fw; // font width
extern int fh; // font height
extern int fhR; // font height real
 
int loadFont();
void releaseFont();

unsigned int measureString(char *str);

void useReducedWidthSpace(int enable);

void drawString(int x, int y, int r, int g, int b, char *str);
void drawStringS(int x, int y, int r, int g, int b, char *str);
void drawStringB(int x, int y, int r, int g, int b, char *str);
void drawStringC(int y, int r, int g, int b, char *text);
void drawStringCB(int y, int r, int g, int b, char *text);

#endif