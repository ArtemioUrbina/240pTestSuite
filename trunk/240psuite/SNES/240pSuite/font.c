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
 
#include "font.h"

extern unsigned char pvsneslibfont_map[];
extern u8 pvsneslibdirty;
extern unsigned char text_buffer[];

void drawText(u16 x, u16 y, u8 palette, char *fmt, ...) 
{	
	va_list ap;
	va_start(ap, fmt);
	vsprintf(text_buffer, fmt, ap);
	va_end(ap);
	
	consoleDrawTextMap(x, y, pvsneslibfont_map, (palette<<2) | (1<<5), text_buffer);	
	pvsneslibdirty = 1;
}