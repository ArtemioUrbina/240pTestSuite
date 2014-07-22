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

extern u8 interlaced;
extern u8 mode240p;

void Set240pMode();
void Set224pMode();
void SetH512Mode();
void SetInterlaced();
void ClearInterlaced();
void ClearH512Mode();

void ChangeVideo();
void DrawTilesWithSprites(u16 X, u16 Y, u16 width, u16 height, u8 *tiles, u16 tileSize, u8 * pal);
void ClearScreen(u8 layer);
void ClearScreen256(u8 layer);
inline void Transition();

void StartDMA();
void bgInitTileSetMine(u8 bgNumber, u8 *tileSource, u8 *tilePalette, u8 paletteEntry, u16 tileSize, u16 paletteSize, u16 colorMode, u16 address);
void bgInitMapSetMine(u8 bgNumber, u8 *mapSource, u16 mapSize, u8 sizeMode, u16 address);
void oamInitGfxSetMine(u8 *tileSource, u16 tileSize, u8 *tilePalette, u16 paletteSize, u8 tilePaletteNumber, u16 address, u8 oamsize);
void CopyExtraTiles(u8 * source, u16 address, u16 size);
void consoleInitTextMine(u8 bgNumber,u8 paletteNumber, u8 *gfxText);
void EndDMA();