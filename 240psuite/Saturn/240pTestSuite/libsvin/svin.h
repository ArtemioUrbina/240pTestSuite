#ifndef _SVIN_H_
#define _SVIN_H_

//#define ROM_MODE

#include <yaul.h>
#include "svin_background.h"

typedef enum {
        _SVIN_SCANMODE_240I = 0,
        _SVIN_SCANMODE_240P = 1,
        _SVIN_SCANMODE_480I = 2,
        _SVIN_SCANMODE_480P = 3
} __packed _svin_scanmode_t;

typedef enum {
        _SVIN_X_RESOLUTION_320 = 0,
        _SVIN_X_RESOLUTION_352 = 1,
} __packed _svin_x_resolution_t;

typedef struct {
        _svin_scanmode_t scanmode;
        _svin_x_resolution_t x_res;
        vdp2_tvmd_vert_t y_res;
        bool x_res_doubled;
        vdp2_tvmd_tv_standard_t colorsystem;
} __packed _svin_screen_mode_t;

extern bool _svin_videomode_scanlines;
extern int _svin_frame_count;

#define _SVIN_CHARACTER_HEIGHT   8
#define _SVIN_CHARACTER_WIDTH   8
#define _SVIN_CHARACTER_BYTES   (_SVIN_CHARACTER_HEIGHT*_SVIN_CHARACTER_WIDTH)
#define _SVIN_CHARACTER_UNITS   (_SVIN_CHARACTER_BYTES/32)

//using VDP2 VRAM static allocation with everything contingent

//i(T0) -> 0,1,2
//i(T1) -> 1,2,3
//i(T2) -> 0,2,3
//i(T3) -> 0,1,3

// Worst cases: 
// 1) 1024x512 4bpp bitmap for NBG0+NBG1 in 480p mode
//              bitmap 0-40000
// 2) 2*64*64 8x8 8bpp cells for NBG0 and NBG1 independently
//              names0 78000-80000 patterns0 0-40000
//              names1 70000-78000 patterns1 60000-70000
//
//      T0 T1 T2 T3
// A0 : D0 D0 D1 D1
// A1 : D0 D0 D1 D1
// B0 : D0 D0 D1 D1
// B1 : i0 i1 D1 D1

// Bank 0
//  0x00000000 - 0x0001FFFF NBG0 character pattern name data (up to 2048 8x8 tiles) = 0x20000
// Bank 1
//  0x00020000 - 0x0003FFFF NBG0 character pattern name data (up to 2048 8x8 tiles) = 0x20000
// Bank 2
//  0x00040000 - 0x0005FFFF NBG0 character pattern name data (up to 2048 8x8 tiles) = 0x20000
// Bank 3
//  0x00060000 - 0x0006FFFF NBG1 character pattern name data (up to 1024 8x8 tiles) = 0x10000
//  0x00070000 - 0x00077FFF NBG1 pattern name data 128x64*4 = 0x8000
//  0x00078000 - 0x0007FFFF NBG0 pattern name data 128x64*4 = 0x8000

#define _SVIN_NBG0_CHPNDR_START (VDP2_VRAM_ADDR(0,0))
#define _SVIN_NBG0_CHPNDR_SIZE (0x60000)
#define _SVIN_NBG0_PNDR_START (VDP2_VRAM_ADDR(3,0x18000))
#define _SVIN_NBG0_PNDR_SIZE (128*64*4)
#define _SVIN_NBG0_CHPNDR_SPECIALS_ADDR (VDP2_VRAM_ADDR(2,0x1F000))
#define _SVIN_NBG0_CHPNDR_SPECIALS_INDEX ((0x5F000)/32)

#define _SVIN_NBG1_CHPNDR_START (VDP2_VRAM_ADDR(3,0))
#define _SVIN_NBG1_CHPNDR_SIZE (0x10000)
#define _SVIN_NBG1_PNDR_START (VDP2_VRAM_ADDR(3,0x10000))
#define _SVIN_NBG1_PNDR_SIZE (128*64*4)
#define _SVIN_NBG1_CHPNDR_SPECIALS_ADDR (VDP2_VRAM_ADDR(3,0xF000))
#define _SVIN_NBG1_CHPNDR_SPECIALS_INDEX ((0x6F000)/32)

// Special setup for 480p cell highres
// 3) 2*64*64 8x8 8bpp cells for NBG0+NBG1 in 480p mode
//
//      T0 T1 T2 T3
// A0 : D0 D0 D0 D0
// A1 : i0 i0  x  x
// B0 : i1 i1  x  x
// B1 : D1 D1 D1 D1

// Bank 0
//  0x00000000 - 0x0001FFFF NBG0 character pattern name data (up to 2048 8x8 tiles) = 0x20000
// Bank 1
//  0x00020000 - 0x00027FFF NBG0 pattern name data 128x64*4 = 0x8000
// Bank 2
//  0x00040000 - 0x00047FFF NBG1 pattern name data 128x64*4 = 0x8000
// Bank 3
//  0x00060000 - 0x0007FFFF NBG1 character pattern name data (up to 2048 8x8 tiles) = 0x20000
//  

#define _SVIN_NBG0_SPECIAL_CHPNDR_START (VDP2_VRAM_ADDR(0,0))
#define _SVIN_NBG0_SPECIAL_CHPNDR_SIZE (0x20000)
#define _SVIN_NBG0_SPECIAL_PNDR_START (VDP2_VRAM_ADDR(1,0))
#define _SVIN_NBG0_SPECIAL_CHPNDR_SPECIALS_ADDR (VDP2_VRAM_ADDR(0,0x1F000))
#define _SVIN_NBG0_SPECIAL_CHPNDR_SPECIALS_INDEX ((0x1F000)/32)

#define _SVIN_NBG1_SPECIAL_CHPNDR_START (VDP2_VRAM_ADDR(3,0))
#define _SVIN_NBG1_SPECIAL_CHPNDR_SIZE (0x20000)
#define _SVIN_NBG1_SPECIAL_PNDR_START (VDP2_VRAM_ADDR(2,0))
#define _SVIN_NBG1_SPECIAL_CHPNDR_SPECIALS_ADDR (VDP2_VRAM_ADDR(3,0x1F000))
#define _SVIN_NBG1_SPECIAL_CHPNDR_SPECIALS_INDEX ((0x7F000)/32)

//VDP1 command list order
#define _SVIN_VDP1_ORDER_SYSTEM_CLIP_COORDS_INDEX  0
#define _SVIN_VDP1_ORDER_LOCAL_COORDS_INDEX        1
#define _SVIN_VDP1_ORDER_TEXT_SPRITE_0_INDEX       2
#define _SVIN_VDP1_ORDER_TEXT_SPRITE_1_INDEX       3
#define _SVIN_VDP1_ORDER_LIMIT                    4

void _svin_init(_svin_screen_mode_t screen_mode, bool bmp_mode);
void _svin_deinit();

void _svin_set_vdp1_cmdlist_toggle_at_vblank(bool enable);

void _svin_set_cycle_patterns_cpu();
void _svin_set_cycle_patterns_nbg(_svin_screen_mode_t screen_mode);

void _svin_set_palette(int number, rgb888_t * pointer);
void _svin_set_palette_part(int number, rgb888_t * pointer, int start, int end);
void _svin_clear_palette(int number);
void _svin_clear_palette_part(int number, int start, int end);

#endif
