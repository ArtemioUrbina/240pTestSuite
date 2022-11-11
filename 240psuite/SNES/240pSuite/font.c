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
#include "res.h"
#include "video.h"

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

void InitTextColor(u8 layer, u8 palette, u16 front, u16 back)
{
	consoleInitTextMine(layer, palette, &fontback);
	setPaletteColor(palette * 0x10, RGB5(31, 0, 31));
	setPaletteColor(palette * 0x10 + 1, front);
	setPaletteColor(palette * 0x10 + 2, back);
}

void AddTextColor(u8 palette, u16 front, u16 back)
{
	setPaletteColor(palette * 0x10, RGB5(31, 0, 31));
	setPaletteColor(palette * 0x10 + 1, front);
	setPaletteColor(palette * 0x10 + 2, back);
}

void CleanFontMap()
{
	u16 x;
	for (x=0;x<0x800;x++)
		pvsneslibfont_map[x] = 0x00;	
	pvsneslibdirty = 1;	
}

void DrawNumber(u16 x, u16 y, u16 spriteIndex, u16 tileIndex, u16 pal)
{
	oamSet(spriteIndex, x, y, 3, 0, 0, tileIndex, pal); 
	oamSetEx(spriteIndex, OBJ_SMALL, OBJ_SHOW);
	oamSetVisible(spriteIndex, OBJ_SHOW);
	
	oamSet(spriteIndex+32, x, y+32, 3, 0, 0, tileIndex+4, pal); 
	oamSetEx(spriteIndex+32, OBJ_SMALL, OBJ_SHOW);
	oamSetVisible(spriteIndex+32, OBJ_SHOW);	
}

void ChangeNumber(u16 x, u16 y, u16 spriteIndex, u16 tileIndex, u16 pal)
{
	oamSet(spriteIndex, x, y, 3, 0, 0, tileIndex, pal); 		
	oamSet(spriteIndex+(4*8), x, y+32, 3, 0, 0, tileIndex+4, pal); 	
}

void DrawCircle(u16 x, u16 y, u16 spriteIndex, u16 tileIndex, u16 pal)
{
	oamSet(spriteIndex, x, y, 2, 0, 0, tileIndex, pal); 
	oamSetEx(spriteIndex, OBJ_SMALL, OBJ_SHOW);
	oamSetVisible(spriteIndex, OBJ_SHOW);
	
	oamSet(spriteIndex+32, x, y+32, 2, 0, 1, tileIndex, pal); 
	oamSetEx(spriteIndex+32, OBJ_SMALL, OBJ_SHOW);
	oamSetVisible(spriteIndex+32, OBJ_SHOW);	
	
	oamSet(spriteIndex+64, x+32, y, 2, 1, 0, tileIndex, pal); 
	oamSetEx(spriteIndex+64, OBJ_SMALL, OBJ_SHOW);
	oamSetVisible(spriteIndex+64, OBJ_SHOW);	
	
	oamSet(spriteIndex+96, x+32, y+32, 2, 1, 1, tileIndex, pal); 
	oamSetEx(spriteIndex+96, OBJ_SMALL, OBJ_SHOW);
	oamSetVisible(spriteIndex+96, OBJ_SHOW);	
}

void ChangeCircle(u16 x, u16 y, u16 spriteIndex, u16 tileIndex, u16 pal)
{
	oamSet(spriteIndex, x, y, 2, 0, 0, tileIndex, pal); 
	oamSet(spriteIndex+32, x, y+32, 2, 0, 1, tileIndex, pal); 
	oamSet(spriteIndex+64, x+32, y, 2, 1, 0, tileIndex, pal); 	
	oamSet(spriteIndex+96, x+32, y+32, 2, 1, 1, tileIndex, pal); 	
}
