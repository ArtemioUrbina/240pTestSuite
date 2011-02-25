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

#ifndef RES_H
#define RES_H

#include "res/beep.h"

extern u32 color_tiles[224/4]; ; 

extern u32 cb601_tiles[35840/4]; 
extern u16 cb601_pal[16];

// Title screen
extern u32 back_tiles[35840/4]; 
extern u16 back_pal[16]; 

extern u32 gillian_tiles[4032/4]; 
extern u16 gillian_pal[16]; 

// Font
extern u32 font_tiles[3072/4];
// Checkerboard
extern u32 check_tile[32/4]; 
extern u32 check_tile_inv[32/4]; 

// Stripes
extern u32 bw_tile[32/4]; 
extern u32 wb_tile[32/4]; 

extern u16 bw_pal[16]; 

// Grid
extern u32 circles_tiles[35840/4]; 
extern u32 circles_grid_tiles[32/4]; 
extern u32 circles_griddot_tiles[32/4]; 
extern u32 grid_tiles[35840/4]; 

extern u16 grid_pal[16]; 

// Sonic background
extern u32 sonicfloor_tiles[16384/4]; 
extern u32 sonicback_tiles[19456/4]; 
extern u16 sonicback_pal[16]; 
extern u32 sonicwater_tiles[6144/4]; 
extern u16 sonicwater_pal[16]; 
extern u16 sonicwater2_pal[16]; 
extern u16 sonicwater3_pal[16]; 

// Motoko background
extern u32 motoko_tiles[35840/4]; 

extern u16 motoko_pal[16]; 

//Striped Sprite
extern u32 striped_tiles[512/4]; 

// Shadow Sprite
extern u32 shadow_tiles[512/4]; 
extern u16 wb_pal[16]; 

// buzz Sprite
extern u32 buzz_tiles[512/4]; 
extern u32 buzzShadow_tiles[512/4]; 
extern u16 buzz_pal[16]; 

// Lag sprite
extern u32 lag_tiles[512/4]; 

// Solid Color (White)
extern u32 solid_tiles[32/4]; 

// Plunge
extern u32 pluge_tiles[35840/4]; 
extern u16 pluge_pal[16]; 

#endif
