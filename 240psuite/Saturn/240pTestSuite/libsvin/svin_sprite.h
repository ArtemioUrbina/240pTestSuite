#ifndef _SVIN_SPRITE_H_
#define _SVIN_SPRITE_H_

#include <yaul.h>

//#include <assert.h>
#include <stdlib.h>

#include "svin.h"

typedef struct {
    char filename[252];
    uint8_t layer;
	uint8_t position;
    uint8_t status;
    uint8_t age;
    uint16_t size_x;
    uint16_t size_y;
    char usage[384];
    uint8_t palette[768];
} _svin_sprite_cache_entry_t;

//there are 3 tables in VDP2 sprite cache
//
//first table includes VDP2 tiles data for sprites (NBG0 and NBG1), 
//  size for NBG0 is (0x18000+0x17000)/64 = 3008, not 3072 entries, 1 byte/tile
//  size for NBG1 is (0x8000+0xF000)/64 = 1472, not 1536 entries, reserving 8192 bytes, 1 byte/tile
#define SVIN_SPRITE_CACHE_TILES_NBG0_SIZE 3008
#define SVIN_SPRITE_CACHE_TILES_NBG1_SIZE 1472
//
//second table include VDP2 tiles names for sprites (NBG0 and NBG1), 
//  size is (0x8000)/4 = 8192 entries, 1 byte/tile
#define SVIN_SPRITE_CACHE_NAMES_NBG0_SIZE 8192
#define SVIN_SPRITE_CACHE_NAMES_NBG1_SIZE 8192
//
//third table include all sprites the tiles correspond to with IDs and names
#define SVIN_SPRITE_CACHE_SPRITES_SIZE 64

//statuses fro sprites
#define SVIN_SPRITE_CACHE_UNUSED 0
#define SVIN_SPRITE_CACHE_LOADED 1
#define SVIN_SPRITE_CACHE_SHOWN 2

void _svin_sprite_init();
void _svin_sprite_deinit();
void _svin_sprite_clear(int iPosition);
void _svin_sprite_draw(char * filename, int iLayer, int iPosition, int iPalette);
void _svin_sprite_cache_purge_oldest(int iLayer);
void _svin_sprite_cache_purge_all();

#endif
