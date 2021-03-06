/* 
 * 240p Test Suite
 * Copyright (C)2014 Artemio Urbina (SNES)
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
 
#include <snes.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#define ENABLE_MOSAIC 1

void drawText(u16 x, u16 y, u8 palette, char *fmt, ...);
void InitTextColor(u8 layer, u8 palette, u16 front, u16 back);
void AddTextColor(u8 palette, u16 front, u16 back);
void CleanFontMap();
void DrawNumber(u16 x, u16 y, u16 spriteIndex, u16 tileIndex, u16 pal);
void ChangeNumber(u16 x, u16 y, u16 spriteIndex, u16 tileIndex, u16 pal);
void DrawCircle(u16 x, u16 y, u16 spriteIndex, u16 tileIndex, u16 pal);
void ChangeCircle(u16 x, u16 y, u16 spriteIndex, u16 tileIndex, u16 pal);