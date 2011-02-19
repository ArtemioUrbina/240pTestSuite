#ifndef RES_H
#define RES_H

#include "res/beep.h"

extern u32 red_tiles[8960/4]; 
extern u32 green_tiles[8960/4]; 
extern u32 blue_tiles[8960/4]; 
extern u32 white_tiles[8960/4]; 

extern u16 red_pal[16]; 
extern u16 green_pal[16]; 
extern u16 blue_pal[16]; 
extern u16 white_pal[16]; 

extern u32 cb601_tiles[35840/4]; 
extern u16 cb601_pal[16];

// Title screen
extern u32 title_tiles[35840/4]; 
extern u32 smenu_tiles[35840/4]; 
extern u32 back_tiles[35840/4]; 
extern u16 title_pal[16]; 

extern u32 gillian_tiles[4032/4]; 
extern u16 gillian_pal[16]; 

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
extern u32 sonicback_tiles[35840/4]; 
extern u16 sonicback_pal[16]; 

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

// Gray Ramp background
extern u32 grayramp_tiles[35840/4]; 

// Plunge
extern u32 pluge_tiles[35840/4]; 
extern u16 pluge_pal[16]; 

#endif
