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

#ifndef MYVDP_H
#define MYVDP_H

#define USE_DMA 0

void VDP_setSpriteAttr(u16 index, u16 tile_attr);
void VDP_setMyTileMapRect(u16 plan, const u16 *data, u16 basetile, u16 x, u16 y, u16 w, u16 h);
u16 Detect_VDP_PAL();
u8 VDP_Detect_Interlace();

int getVerticalRes();
inline int isVertical240();

void VDP_Start();
void VDP_End();

extern u16	enable_PAL240;
extern u16	enable_256;
extern u8	IsPALVDP;

extern u8	enable_cntrlsrt;
extern u8	debug_controls;

#endif
