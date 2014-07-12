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
 
#include "video.h"

#define REG_SETINI (*(vuint8*)0x2133)

void Set240pMode()
{
	REG_SETINI |= 0x04;
}

void Set224pMode()
{
	REG_SETINI = 0;
}

void SetH512Mode()
{
	REG_SETINI |= 0x08;
}

void DrawTilesWithSprites(u16 X, u16 Y, u16 width, u16 height, u8 *tiles, u16 tileSize, u8 * pal)
{
	u16 baseX = X, baseY = Y;
	u16 spriteIndex = 0, tileIndex = 0;
	u16 row = 0, column = 0, sizeX = 0, sizeY = 0;
	
	sizeX = width / 16;
	sizeY = height / 16;
	
	oamInitGfxSet(tiles, tileSize,	pal, 16*2, 0, 0x4000, OBJ_SIZE16);

	for(row = 0; row < sizeY; row++)
	{
		for(column = 0; column < sizeX; column ++)
		{
			oamSet(spriteIndex, baseX+column*16, baseY+row*16, 3, 0, 0, tileIndex, 0); 
			oamSetEx(spriteIndex, OBJ_SMALL, OBJ_SHOW);
			oamSetVisible(spriteIndex, OBJ_SHOW);
		
			spriteIndex += 4;
			tileIndex += 2;
		}
		if(row % 2 != 0)
			tileIndex += 4*sizeX;
	}
}
