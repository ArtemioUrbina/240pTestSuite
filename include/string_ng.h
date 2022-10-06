/* 
 * 240p Test Suite for the Neo Geo
 * by Artemio Urbina and Dustin Dembrosky (Dasutin)
 * Copyright (C)2011-2022 Artemio Urbina
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

#ifndef _STRING_H_
#define _STRING_H_

#include "types.h"

#define isdigit(c)	((c) >= '0' && (c) <= '9')

size_t strlen(const char *str);

void setRandomSeed(u16 seed);
u16 random();

//char* strcpy(char *dest, const char *src);
//char* strcat(char *dest, const char *src);
//void* memcpy (volatile void *dest, const void *src, size_t len);
//void myMemSet(void* str, char ch, size_t n);

u32 intToHex(u32 value, char *str, u16 minsize);
u16 intToStr(s32 value, char *str, u16 minsize);
u16 uintToStr(u32 value, char *str, u16 minsize);
void fix32ToStr(fix32 value, char *str, u16 numdec);
int hexToDec(int hex);

// Clear screen
#define MAX_SPRITES	248
void gfxClear();
void suiteClearFixLayer();

#endif /* _STRING_H_ */