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

#include "video.h"

extern float fw;
extern float fh;
extern float f_size;

void DrawChar(u16 x, u16 y, char c);
void DrawString(u16 x, u16 y, u8 r, u8 g, u8 b, char *str);
void DrawStringS(u16 x, u16 y, u8 r, u8 g, u8 b, char *str);
void DrawStringB(u16 x, u16 y, u8 r, u8 g, u8 b, char *str);

void LoadFont();
void ReleaseFont();

/* Big Numbers */
/* Big Numbers */

void LoadNumbers();
void ReleaseNumbers();
void DrawDigit(u16 x, u16 y, u8 r, u8 g, u8 b, int digit) ;
void DrawNumber(u16 x, u16 y,u8 r, u8 g, u8 b, int number);

#endif
