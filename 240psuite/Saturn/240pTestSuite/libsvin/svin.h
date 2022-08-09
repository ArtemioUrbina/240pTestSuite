#ifndef _SVIN_H_
#define _SVIN_H_

//#define ROM_MODE

#include <yaul.h>
#include "svin_menu.h"
#include "svin_text.h"
#include "svin_textbox.h"
#include "svin_script.h"
#include "svin_filelist.h"
#include "svin_sprite.h"
#include "svin_background.h"
#include "svin_cd_access.h"
#include "svin_debug.h"
#include "svin_alloc.h"

typedef enum {
        _SVIN_X_RESOLUTION_320 = 0,
        _SVIN_X_RESOLUTION_352 = 1,
        _SVIN_X_RESOLUTION_640 = 2,
        _SVIN_X_RESOLUTION_704 = 3
} __packed _svin_x_resolution_t;

typedef enum {
        _SVIN_Y_RESOLUTION_224 = 0,
        _SVIN_Y_RESOLUTION_240 = 1,
        _SVIN_Y_RESOLUTION_256 = 2,
        _SVIN_Y_RESOLUTION_448 = 3,
        _SVIN_Y_RESOLUTION_480 = 4,
        _SVIN_Y_RESOLUTION_512 = 5
} __packed _svin_y_resolution_t;

extern int _svin_videomode_x_res;
extern int _svin_videomode_y_res;
extern bool _svin_videomode_scanlines;
extern int _svin_frame_count;

#define _SVIN_CHARACTER_HEIGHT   8
#define _SVIN_CHARACTER_WIDTH   8
#define _SVIN_CHARACTER_BYTES   (_SVIN_CHARACTER_HEIGHT*_SVIN_CHARACTER_WIDTH)
#define _SVIN_CHARACTER_UNITS   (_SVIN_CHARACTER_BYTES/32)

//using VDP2 VRAM static allocation with everything contingent
// Bank 0
//  0x00000000 - 0x00007FFF NBG1 pattern name data 128x64*4 = 0x8000
//  0x00008000 - 0x0001FFFF NBG0 character pattern name data (up to 1536 8x8 tiles) = 0x18000
// Bank 1
//  0x00020000 - 0x00037FFF NBG0 character pattern name data (up to 1536 8x8 tiles) = 0x18000
//  0x00038000 - 0x0003FFFF NBG2 pattern name data 128x64*4 = 0x8000
// Bank 2
//  0x00040000 - 0x00047FFF NBG0 pattern name data 128x64*4 = 0x8000
//  0x00048000 - 0x0005FFFF NBG1 character pattern name data (up to 1536 8x8 tiles) = 0x18000
// Bank 3
//  0x00060000 - 0x0006FFFF NBG2 character pattern name data (up to 512 8x8 tiles) = 0x10000
//  0x00070000 - 0x0007EFFF NBG1 rendered font data for dialog box 640x80 (40x5*16x16) = 0x????
//  0x0007F000 - 0x0007FFFF NBG1 character pattern name data specials


//#define _SVIN_NBG0_CHPNDR_START (VDP2_VRAM_ADDR(0,0x8000))
//#define _SVIN_NBG0_CHPNDR_SIZE (0x30000)
//#define _SVIN_NBG0_PNDR_START (VDP2_VRAM_ADDR(2,0))
//#define _SVIN_NBG0_PNDR_SIZE (128*64*4)

//#define _SVIN_NBG1_CHPNDR_START (VDP2_VRAM_ADDR(2,0x8000))
//#define _SVIN_NBG1_CHPNDR_SIZE (0x18000)
//#define _SVIN_NBG1_PNDR_START (VDP2_VRAM_ADDR(0,0))
//#define _SVIN_NBG1_PNDR_SIZE (128*64*4)

//#define _SVIN_NBG1_CHPNDR_TEXTBOX_ADDR (VDP2_VRAM_ADDR(3,0x10000))
//#define _SVIN_NBG1_CHPNDR_TEXTBOX_INDEX ((0x70000)/32)
//#define _SVIN_NBG0_CHPNDR_SPECIALS_ADDR (VDP2_VRAM_ADDR(0,0x1F000))
//#define _SVIN_NBG0_CHPNDR_SPECIALS_INDEX ((0x1F000)/32)
//#define _SVIN_NBG1_CHPNDR_SPECIALS_ADDR (VDP2_VRAM_ADDR(3,0x1F000))
//#define _SVIN_NBG1_CHPNDR_SPECIALS_INDEX ((0x7F000)/32)

//#define _SVIN_NBG2_CHPNDR_START (VDP2_VRAM_ADDR(3,0))
//#define _SVIN_NBG2_CHPNDR_SIZE (0x10000)
//#define _SVIN_NBG2_PNDR_START (VDP2_VRAM_ADDR(1,0x18000))
//#define _SVIN_NBG2_PNDR_SIZE (128*64*4)


//TAKE 2

//T0 - 0,1,2
//T1 - 1,2,3
//T2 - 0,2,3
//T3 - 0,1,3

// D0 D0 D2 D2
// D0 D0 D1 D1
// i0 i1 D1 D1
// -- i2 D2 D2




// Bank 0
//  0x00000000 - 0x00007FFF NBG2 character pattern name data (up to 256 8x8 tiles) = 0x8000
//  0x00008000 - 0x0001FFFF NBG0 character pattern name data (up to 1536 8x8 tiles) = 0x18000
// Bank 1
//  0x00020000 - 0x00037FFF NBG0 character pattern name data (up to 1536 8x8 tiles) = 0x18000
//  0x00038000 - 0x0003FFFF NBG1 character pattern name data (up to 256 8x8 tiles) = 0x8000
// Bank 2
//  0x00040000 - 0x0004EFFF NBG1 character pattern name data (up to 960 8x8 tiles) = 0xF000
//  0x0004F000 - 0x0004FFFF NBG1 character pattern name data specials (up to 64 8x8 tiles) = 0x1000
//  0x00050000 - 0x00057FFF NBG0 pattern name data 128x64*4 = 0x8000
//  0x00058000 - 0x0005FFFF NBG1 pattern name data 128x64*4 = 0x8000
// Bank 3
//  0x00060000 - 0x00067FFF NBG2 pattern name data 128x64*4 = 0x8000
//  0x00068000 - 0x0007EFFF NBG2 character pattern name data (up to 1472 8x8 tiles) = 0x17000
//  0x0007F000 - 0x0007FFFF NBG2 character pattern name data specials (up to 64 8x8 tiles) = 0x1000

#define _SVIN_NBG0_CHPNDR_START (VDP2_VRAM_ADDR(0,0x8000))
#define _SVIN_NBG0_CHPNDR_SIZE (0x2F000)
#define _SVIN_NBG0_PNDR_START (VDP2_VRAM_ADDR(2,0x10000))
#define _SVIN_NBG0_PNDR_SIZE (128*64*4)
#define _SVIN_NBG0_CHPNDR_SPECIALS_ADDR (VDP2_VRAM_ADDR(1,0x17000))
#define _SVIN_NBG0_CHPNDR_SPECIALS_INDEX ((0x37000)/32)

#define _SVIN_NBG1_CHPNDR_START (VDP2_VRAM_ADDR(1,0x18000))
#define _SVIN_NBG1_CHPNDR_SIZE (0x10000)
#define _SVIN_NBG1_PNDR_START (VDP2_VRAM_ADDR(2,0x18000))
#define _SVIN_NBG1_PNDR_SIZE (128*64*4)
#define _SVIN_NBG1_CHPNDR_SPECIALS_ADDR (VDP2_VRAM_ADDR(2,0x0F000))
#define _SVIN_NBG1_CHPNDR_SPECIALS_INDEX ((0x4F000)/32)

#define _SVIN_NBG2_CHPNDR_TEXTBOX_ADDR (VDP2_VRAM_ADDR(3,0x10000))
#define _SVIN_NBG2_CHPNDR_TEXTBOX_SIZE (640*80)
#define _SVIN_NBG2_CHPNDR_TEXTBOX_INDEX ((0x70000)/32)
#define _SVIN_NBG2_CHPNDR_START (VDP2_VRAM_ADDR(3,0x8000))
#define _SVIN_NBG2_CHPNDR_SIZE (0x8000)
#define _SVIN_NBG2_PNDR_START (VDP2_VRAM_ADDR(3,0))
#define _SVIN_NBG2_PNDR_SIZE (128*64*4)
#define _SVIN_NBG2_CHPNDR_SPECIALS_ADDR (VDP2_VRAM_ADDR(3,0x1F000))
#define _SVIN_NBG2_CHPNDR_SPECIALS_INDEX ((0x7F000)/32)

//VDP1 command list order
#define _SVIN_VDP1_ORDER_SYSTEM_CLIP_COORDS_INDEX  0
#define _SVIN_VDP1_ORDER_LOCAL_COORDS_A_INDEX      1
#define _SVIN_VDP1_ORDER_SPRITE_A0_INDEX           2
#define _SVIN_VDP1_ORDER_SPRITE_A1_INDEX           3
#define _SVIN_VDP1_ORDER_SPRITE_A2_INDEX           4
#define _SVIN_VDP1_ORDER_SPRITE_A3_INDEX           5
#define _SVIN_VDP1_ORDER_SPRITE_A4_INDEX           6
#define _SVIN_VDP1_ORDER_SPRITE_A5_INDEX           7
#define _SVIN_VDP1_ORDER_SPRITE_A6_INDEX           8
#define _SVIN_VDP1_ORDER_SPRITE_A7_INDEX           9
#define _SVIN_VDP1_ORDER_DRAW_END_A_INDEX          10
#define _SVIN_VDP1_ORDER_LOCAL_COORDS_B_INDEX      11
#define _SVIN_VDP1_ORDER_SPRITE_B0_INDEX           12
#define _SVIN_VDP1_ORDER_SPRITE_B1_INDEX           13
#define _SVIN_VDP1_ORDER_SPRITE_B2_INDEX           14
#define _SVIN_VDP1_ORDER_SPRITE_B3_INDEX           15
#define _SVIN_VDP1_ORDER_SPRITE_B4_INDEX           16
#define _SVIN_VDP1_ORDER_SPRITE_B5_INDEX           17
#define _SVIN_VDP1_ORDER_SPRITE_B6_INDEX           18
#define _SVIN_VDP1_ORDER_SPRITE_B7_INDEX           19
#define _SVIN_VDP1_ORDER_DRAW_END_B_INDEX          20
#define _SVIN_VDP1_ORDER_COUNT                     21

void _svin_init(_svin_x_resolution_t x_res, _svin_y_resolution_t y_res, bool scanlines);
void _svin_deinit();
void _svin_delay(int milliseconds);
int _svin_get_keys_state();
int _svin_wait_for_key_press_and_release();

void _svin_set_vdp1_cmdlist_toggle_at_vblank(bool enable);

void _svin_set_cycle_patterns_cpu();
void _svin_set_cycle_patterns_nbg();

void _svin_set_palette(int number, uint8_t * pointer);
void _svin_set_palette_part(int number, uint8_t * pointer, int start, int end);
void _svin_clear_palette(int number);
void _svin_clear_palette_part(int number, int start, int end);
void _svin_set_palette_24bpp();

#endif
