/* 
 * 240p Test Suite
 * Copyright (C)2011-2014 Artemio Urbina
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

#ifndef PATTERNS_H
#define PATTERNS_H

#define GRID_320 0
#define GRID_256 1

void DrawPluge();
void DrawColorBars();
void Draw601ColorBars();
void DrawGrid(u16 gridtype);
void DrawColorBleed();
void DrawLinearity();
void DrawLinearity256();
void DrawGrayRamp();
void DrawWhiteScreen();
void Draw100IRE();
void DrawSMPTE();
void DrawSharpness();
void DrawOverscan();

void DrawColorTilesAt(u16 plan, u16 pal, u16 x, u16 y, u16 tiles, u16 w, u16 h);
void DrawColorTilesAtInv(u16 plan, u16 pal, u16 x, u16 y, u16 tiles, u16 w, u16 h);

#endif
