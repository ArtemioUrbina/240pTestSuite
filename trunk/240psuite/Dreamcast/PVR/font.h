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
#include <plx/font.h>
#include <plx/sprite.h>
#include <plx/list.h>
#include <plx/dr.h>

extern float fw;
extern float fh;
extern float f_size;

void DrawString(float x, float y, float r, float g, float b, char *str);
void DrawStringS(float x, float y, float r, float g, float b, char *str);
void DrawStringB(float x, float y, float r, float g, float b, char *str);

void LoadFont();
void ReleaseFont();

#endif
