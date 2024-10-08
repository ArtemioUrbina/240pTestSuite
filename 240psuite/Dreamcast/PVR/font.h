/* 
 * 240p Test Suite
 * Copyright (C)2011 Artemio Urbina
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

#ifndef FONT_H
#define FONT_H

#include <kos.h>

extern float fw;
extern float fh;
extern float f_size;

void DrawChar(float x, float y, char c);
void DrawString(float x, float y, float r, float g, float b, char *str);
void DrawStringS(float x, float y, float r, float g, float b, char *str);
void DrawStringNH(float x, float y, float r, float g, float b, char *str);
void DrawStringSCentered(float y, float r, float g, float b, char *str);
void DrawStringSCenteredFull(float y, float r, float g, float b, char *str);
void DrawStringSCenteredXY(float r, float g, float b, char *str);
void DrawStringB(float x, float y, float r, float g, float b, char *str);
void DrawStringBCentered(float y, float r, float g, float b, char *str);
void DrawStringBCenteredFull(float y, float r, float g, float b, char *str);
void DrawStringSKB(float x, float y, float r, float g, float b, char *str, int max_len, int key_pos);
int MeasureString(char *str);
int countLineFeeds(char *str);
int getLongestLineLen(char *str);

void LoadFont();
void ReleaseFont();

/* Big Numbers */
/* Big Numbers */

void LoadNumbers();
void ReleaseNumbers();
void DrawDigit(float x, float y, float r, float g, float b, int digit) ;
void DrawNumber(float x, float y, float r, float g, float b, int number);

#endif
