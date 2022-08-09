#include <yaul.h>

//#include <assert.h>
#include <stdlib.h>

#include "svin.h"

#include <mcufont.h>

#define UNUSED(x) (void)(x)

#define _SVIN_SPRITE_TILES_WIDTH 44
#define _SVIN_SPRITE_TILES_HEIGTH 56

//int iLastIndex[3];

char * _svin_sprite_cache_NBG0_tiles;
char * _svin_sprite_cache_NBG1_tiles;
char * _svin_sprite_cache_NBG0_names;
char * _svin_sprite_cache_NBG1_names;
int _svin_sprite_cache_NBG0_tiles_free;
int _svin_sprite_cache_NBG1_tiles_free;
_svin_sprite_cache_entry_t * _svin_sprite_cache_sprites;

void 
_svin_sprite_init()
{
    /*_svin_sprite_NBG0_usage = malloc(_SVIN_NBG0_CHPNDR_SIZE/_SVIN_CHARACTER_BYTES);
    memset(_svin_sprite_NBG0_usage,0,_SVIN_NBG0_CHPNDR_SIZE/_SVIN_CHARACTER_BYTES);
    _svin_sprite_NBG1_usage = malloc(_SVIN_NBG1_CHPNDR_SIZE/_SVIN_CHARACTER_BYTES);
    memset(_svin_sprite_NBG1_usage,0,_SVIN_NBG1_CHPNDR_SIZE/_SVIN_CHARACTER_BYTES);
    _svin_sprite_NBG2_usage = malloc(_SVIN_NBG2_CHPNDR_SIZE/_SVIN_CHARACTER_BYTES);
    memset(_svin_sprite_NBG2_usage,0,_SVIN_NBG2_CHPNDR_SIZE/_SVIN_CHARACTER_BYTES);*/
    //iLastIndex[0] = 0;
    //iLastIndex[1] = 0;
    //iLastIndex[2] = 0;
	_svin_sprite_cache_NBG0_tiles_free = SVIN_SPRITE_CACHE_TILES_NBG0_SIZE;
	_svin_sprite_cache_NBG1_tiles_free = SVIN_SPRITE_CACHE_TILES_NBG1_SIZE;
	//allocate cache tiles
    _svin_sprite_cache_NBG0_tiles = malloc(4096);//SVIN_SPRITE_CACHE_TILES_NBG0_SIZE);
    _svin_sprite_cache_NBG1_tiles = malloc(2048);//SVIN_SPRITE_CACHE_TILES_NBG1_SIZE);
	//mark all tiles as unused (-1 id)
	for (int i=0;i<SVIN_SPRITE_CACHE_TILES_NBG0_SIZE;i++)
		_svin_sprite_cache_NBG0_tiles[i] = -1;
	for (int i=0;i<SVIN_SPRITE_CACHE_TILES_NBG1_SIZE;i++)
		_svin_sprite_cache_NBG1_tiles[i] = -1;
	//allocate cache names
    _svin_sprite_cache_NBG0_names = malloc(SVIN_SPRITE_CACHE_NAMES_NBG0_SIZE);
    _svin_sprite_cache_NBG1_names = malloc(SVIN_SPRITE_CACHE_NAMES_NBG1_SIZE);
	//mark all names as unused (-1 id)
	for (int i=0;i<SVIN_SPRITE_CACHE_NAMES_NBG0_SIZE;i++)
		_svin_sprite_cache_NBG0_names[i] = -1;
	for (int i=0;i<SVIN_SPRITE_CACHE_NAMES_NBG1_SIZE;i++)
		_svin_sprite_cache_NBG1_names[i] = -1;
	//allocate sprite cache
    _svin_sprite_cache_sprites = _svin_alloc_lwram(sizeof(_svin_sprite_cache_entry_t)*SVIN_SPRITE_CACHE_SPRITES_SIZE,0x20280000);
	//mark all sprites as unused
	for (int i=0;i<SVIN_SPRITE_CACHE_SPRITES_SIZE;i++)
		_svin_sprite_cache_sprites[i].status = SVIN_SPRITE_CACHE_UNUSED;
}

void
_svin_sprite_deinit()
{
    free(_svin_sprite_cache_NBG0_tiles);
    free(_svin_sprite_cache_NBG1_tiles);
    free(_svin_sprite_cache_NBG0_names);
    free(_svin_sprite_cache_NBG1_names);
	//allocate sprite cache
    _svin_free_lwram(0x20280000);
}

void 
_svin_sprite_clear(int iPosition)
{
    int * p32;
    //int x,y;
	int i;
    //clear the previous image
    _svin_set_cycle_patterns_cpu();

	//searching for all sprites in the specified position
	for (i=0;i<SVIN_SPRITE_CACHE_SPRITES_SIZE;i++)
	{
		if ( (_svin_sprite_cache_sprites[i].status == SVIN_SPRITE_CACHE_SHOWN) && (_svin_sprite_cache_sprites[i].position == iPosition) )
		{
			//found shown sprite at current position, clearing it from names, but leaving in tiles and in sprites
            if (_svin_sprite_cache_sprites[i].layer == 0)
            {
                //clearing at NBG0
                p32 = (int*)_SVIN_NBG0_PNDR_START;
                for (int j=0; j < SVIN_SPRITE_CACHE_NAMES_NBG0_SIZE; j++)
                {
                    if (_svin_sprite_cache_NBG0_names[j] == i)
                    {
                        //found a match in names, clearing it
                        _svin_sprite_cache_NBG0_names[j] = -1; //i.e. unused
                        //now clean the actual tables values in VDP2
                        p32[j] = 0x10000000 + _SVIN_NBG0_CHPNDR_SPECIALS_INDEX;
                    }
                }
            }
            else
            {
                //clearing at NBG1
                p32 = (int*)_SVIN_NBG1_PNDR_START;
                for (int j=0; j < SVIN_SPRITE_CACHE_NAMES_NBG1_SIZE; j++)
                {
                    if (_svin_sprite_cache_NBG1_names[j] == i)
                    {
                        //found a match in names, clearing it
                        _svin_sprite_cache_NBG1_names[j] = -1; //i.e. unused
                        //now clean the actual tables values in VDP2
                        p32[j] = 0x10000000 + _SVIN_NBG1_CHPNDR_SPECIALS_INDEX;
                    }
                }                
            }
		}
	}
		
    _svin_set_cycle_patterns_nbg();
}

void 
_svin_sprite_cache_purge_oldest(int iLayer)
{
    int iOldestEntryIndex = -1;
    int iOldestEntryAge = -1;
    //deleting the oldest inactive cache entry, only LOADED, don't touch UNUSED (already purged) and SHOWN (currently used, can't delete)
    for (int i=0;i<SVIN_SPRITE_CACHE_SPRITES_SIZE;i++)
	{
		if ( ( _svin_sprite_cache_sprites[i].status == SVIN_SPRITE_CACHE_LOADED ) && (_svin_sprite_cache_sprites[i].layer == iLayer) )
		{
            if (_svin_sprite_cache_sprites[i].age > iOldestEntryAge)
            {
                iOldestEntryAge = _svin_sprite_cache_sprites[i].age;
                iOldestEntryIndex = i;
            }
        }
    }
    //if didn't find anything, everything's probably unused, going out
    if (-1 == iOldestEntryIndex)
        return;
    //now purge all cache tables for the entry
    for (int i=0;i<SVIN_SPRITE_CACHE_TILES_NBG0_SIZE;i++)
    {
        if (_svin_sprite_cache_NBG0_tiles[i] == iOldestEntryIndex)
        {
            _svin_sprite_cache_NBG0_tiles[i] = -1;
            _svin_sprite_cache_NBG0_tiles_free++;
        }
    }
    for (int i=0;i<SVIN_SPRITE_CACHE_TILES_NBG1_SIZE;i++)
    {
        if (_svin_sprite_cache_NBG1_tiles[i] == iOldestEntryIndex)
        {
            _svin_sprite_cache_NBG1_tiles[i] = -1;
            _svin_sprite_cache_NBG1_tiles_free++;
        }
    }
    for (int i=0;i<SVIN_SPRITE_CACHE_NAMES_NBG0_SIZE;i++)
    {
        if (_svin_sprite_cache_NBG0_names[i] == iOldestEntryIndex)
        {
            _svin_sprite_cache_NBG0_names[i] = -1;
        }
    }
    for (int i=0;i<SVIN_SPRITE_CACHE_NAMES_NBG1_SIZE;i++)
    {
        if (_svin_sprite_cache_NBG1_names[i] == iOldestEntryIndex)
        {
            _svin_sprite_cache_NBG1_names[i] = -1;
        }
    }
    _svin_sprite_cache_sprites[iOldestEntryIndex].status = SVIN_SPRITE_CACHE_UNUSED;
}

void 
_svin_sprite_cache_purge_all()
{
    //now purge all cache tables
    for (int i=0;i<SVIN_SPRITE_CACHE_TILES_NBG0_SIZE;i++)
    {
        _svin_sprite_cache_NBG0_tiles[i] = -1;
    }
    for (int i=0;i<SVIN_SPRITE_CACHE_TILES_NBG1_SIZE;i++)
    {
        _svin_sprite_cache_NBG1_tiles[i] = -1;
    }
    for (int i=0;i<SVIN_SPRITE_CACHE_NAMES_NBG0_SIZE;i++)
    {
        _svin_sprite_cache_NBG0_names[i] = -1;
    }
    for (int i=0;i<SVIN_SPRITE_CACHE_NAMES_NBG1_SIZE;i++)
    {
        _svin_sprite_cache_NBG1_names[i] = -1;
    }
    for (int i=0;i<SVIN_SPRITE_CACHE_SPRITES_SIZE;i++)
    {
        _svin_sprite_cache_sprites[i].status = SVIN_SPRITE_CACHE_UNUSED;
    }
}


void 
_svin_sprite_draw(char * filename, int iLayer, int iPosition, int iPalette)
{
    uint8_t * big_buffer;
    int i,x,y;
    int iPointer;
    //int iFree;
    //char c;
    int * p32;
    bool bFound;
    int iFound;
    int iSize, iSize_Fixed;
    int iOffset;
    int iTilesCacheSize;
    uint8_t iSizeX,iSizeY;
    char * _svin_sprite_cache_tiles;
    char * _svin_sprite_cache_names;
	
	switch(iLayer)
    {
        case 0:
            iTilesCacheSize = SVIN_SPRITE_CACHE_TILES_NBG0_SIZE;
            _svin_sprite_cache_tiles = _svin_sprite_cache_NBG0_tiles;
            _svin_sprite_cache_names = _svin_sprite_cache_NBG0_names;
            break;
        case 1:
            iTilesCacheSize = SVIN_SPRITE_CACHE_TILES_NBG1_SIZE;
            _svin_sprite_cache_tiles = _svin_sprite_cache_NBG1_tiles;
            _svin_sprite_cache_names = _svin_sprite_cache_NBG1_names;
            break;
        default:
            assert(0);
            break;
    }
	
	//using palettes 1 thru 6
    int iPaletteIndex=1;
    switch (iPosition)
    {
        case 0:
            //left, only layers NBG0 and NBG1
            iPaletteIndex = 1+iLayer;
            break;
        case 1:
            //mid, only layers NBG0 and NBG1
            iPaletteIndex = 3+iLayer;
            break;
        case 2:
            //right, only layers NBG0 and NBG1
            iPaletteIndex = 5+iLayer;
            break;
    }
	
	assert (iPaletteIndex > 0);
    assert (iPaletteIndex < 7);
	
	int x_start=0,x_end=0;
	
	//searching for sprite in the cache
    bool bFoundInCache = false;
    bool bCacheError = false;
    int iCurrentTile=0;
    uint8_t small_buffer[4096];
	for (int iCacheIndex=0;iCacheIndex<SVIN_SPRITE_CACHE_SPRITES_SIZE;iCacheIndex++)
	{
		if ( (_svin_sprite_cache_sprites[iCacheIndex].status != SVIN_SPRITE_CACHE_UNUSED) && (0==strcmp(_svin_sprite_cache_sprites[iCacheIndex].filename,filename)) )
		{
            bFoundInCache = true;
			//found sprite for this filename in VRAM, not loading anything, just using existing VRAM data
			//sprites in VRAM are sorted by address, even if fragmented, using this feature.
            //also, layer never changes for each sprite, so ignoring sprite layer property
            //get position first
            switch (iPosition)
            {
                case 0:
                    x_start = 0;
                    break;
                case 1:
                    x_start = _SVIN_SPRITE_TILES_WIDTH - (_svin_sprite_cache_sprites[iCacheIndex].size_x/2);
                    break;
                case 2:
                    x_start = _SVIN_SPRITE_TILES_WIDTH*2 - _svin_sprite_cache_sprites[iCacheIndex].size_x;
                    break;
            }
			x_end = x_start + _svin_sprite_cache_sprites[iCacheIndex].size_x;
            iSizeX = _svin_sprite_cache_sprites[iCacheIndex].size_x;
            iSizeY = _svin_sprite_cache_sprites[iCacheIndex].size_y;

            //now unpack usage data into small_buffer
            for (i=0;i<384;i++)
            {
                for (int j=0; j<8; j++)
                {
                    if ( (1<<j)&_svin_sprite_cache_sprites[iCacheIndex].usage[i])
                        small_buffer[i*8+j+2] = 1;
                    else
                        small_buffer[i*8+j+2] = 0;
                }
            }

            //write the names
            iCurrentTile = 0;
			for (y=0;y<_svin_sprite_cache_sprites[iCacheIndex].size_y;y++)
			{
				for (x=x_start;x<x_end;x++)
				{
					if (small_buffer[y*iSizeX+(x-x_start)+2])
					{
                        //searching for next tile in tiles cache
                        while (_svin_sprite_cache_tiles[iCurrentTile] != iCacheIndex)
                            iCurrentTile++;
                        if  (iCurrentTile > iTilesCacheSize)
						{
							//out of cache, not fixing it now, just purging the cache and loading from cd
							bCacheError = true;
							x = x_end;
							y = _svin_sprite_cache_sprites[iCacheIndex].size_y;
							iCurrentTile = 0;
							continue;
						}
                        switch (iLayer)
                        {
                            case 0:
                                p32 = (int*)_SVIN_NBG0_PNDR_START;
                                break;
                            case 1:
                                p32 = (int*)_SVIN_NBG1_PNDR_START;
                                break;
                        }
    					//setting tile index
						//switch (iCurrLayer)
						switch (iLayer)
                        {
							case 0:
								iOffset = (_SVIN_NBG0_CHPNDR_START - VDP2_VRAM_ADDR(0,0))/32;
								if (x<64)
                                {
									p32[y*64+x] = 0x00000000 | 0x100000*(iPaletteIndex) | (iOffset + iCurrentTile*2); //palette 0, transparency on
                                    _svin_sprite_cache_names[y*64+x] = iCacheIndex;
                                }
								else
                                {
									p32[(y+64)*64+x-64] = 0x00000000 | 0x100000*(iPaletteIndex) | (iOffset + iCurrentTile*2); //palette 0, transparency on
                                    _svin_sprite_cache_names[(y+64)*64+x-64] = iCacheIndex;
                                }
								break;
							case 1:
								iOffset = (_SVIN_NBG1_CHPNDR_START - VDP2_VRAM_ADDR(0,0))/32;
								if (x<64)
                                {
									p32[y*64+x] = 0x00000000 | 0x100000*(iPaletteIndex) | (iOffset + iCurrentTile*2); //palette 0, transparency on
                                    _svin_sprite_cache_names[y*64+x] = iCacheIndex;
                                }
								else
                                {
									p32[(y+64)*64+x-64] = 0x00000000 | 0x100000*(iPaletteIndex) | (iOffset + iCurrentTile*2); //palette 0, transparency on
                                    _svin_sprite_cache_names[(y+64)*64+x-64] = iCacheIndex;
                                }
								break;
						}
                        iCurrentTile++;
					}
				}
			}

            //load palette
            switch (iPalette)
            {
                case 0:
                    _svin_set_palette(iPaletteIndex,_svin_sprite_cache_sprites[iCacheIndex].palette);
                    break;
                case 1:
                    _svin_set_palette_part(iPaletteIndex,_svin_sprite_cache_sprites[iCacheIndex].palette,128,255);
                    break;
                case 2:
                    _svin_set_palette_part(iPaletteIndex,_svin_sprite_cache_sprites[iCacheIndex].palette,0,127);
                    break;
            }
		}
	}
	
	if (true == bCacheError)
	{
		_svin_sprite_cache_purge_all();
		bFoundInCache = false;
	}

    if (false == bFoundInCache)
    {
        //if not found in cache, loading and saving to cache
        //first let's find sprite FAD
        fad_t _sprite_fad;
        bool b = _svin_filelist_search(filename,&_sprite_fad,&iSize);
        assert(true == b);
        iSize_Fixed = ((iSize/2048)+1)*2048;

        big_buffer = malloc(iSize_Fixed);

        //reading whole file at once
        _svin_cd_block_sectors_read(_sprite_fad, big_buffer, iSize_Fixed);

        //getting sizes in tiles
        iSizeX = big_buffer[0];
        iSizeY = big_buffer[1];

        assert (iSizeY == _SVIN_SPRITE_TILES_HEIGTH);
        //assert (iSizeX <= _SVIN_SPRITE_TILES_WIDTH);

        switch (iPosition)
                {
                    case 0:
                        x_start = 0;
                        break;
                    case 1:
                        x_start = _SVIN_SPRITE_TILES_WIDTH - (iSizeX/2);
                        break;
                    case 2:
                        x_start = _SVIN_SPRITE_TILES_WIDTH*2 -iSizeX;
                        break;
                }

        //calculating tiles number
        int iTilesNumber = 0;
        for (i=0;i<iSizeX*iSizeY;i++)
        {
            if (big_buffer[i+2])
                iTilesNumber++;
        }

        //int iLastIndex_to_free = iLastIndex[iLayer];

        //preparing free space in tiles
        switch(iLayer)
        {
            case 0:
                
                while (iTilesNumber > _svin_sprite_cache_NBG0_tiles_free)
                {
                    _svin_sprite_cache_purge_oldest(iLayer);
                }
                break;
            case 1:
                while (iTilesNumber > _svin_sprite_cache_NBG1_tiles_free)
                {
                    _svin_sprite_cache_purge_oldest(iLayer);
                }
                break;
        }
 
        /*iFree = 0;
        while (iFree < iTilesNumber)
        {
            iLastIndex_to_free++;
            if (iLastIndex_to_free > 255) iLastIndex_to_free = 1;
            for (i=0;i<iTilesCacheSize;i++)
            {
                c = pGlobalUsage[iLayer][i];
                if (c==iLastIndex_to_free){
                    pGlobalUsage[iLayer][i] = 0; //freeing
                }
            }
            //recalculate free
            iFree = 0;
            for (i=0;i<iTilesCacheSize;i++)
            {
                c = pGlobalUsage[iLayer][i];
                if (c==0){
                    iFree++;
                }
            }
        }*/

        iPointer = iSizeX*iSizeY+2; //position within buffer

        //VRAM available, fill it
        //choose next fill index
        /*iLastIndex[iLayer]++;
        if (iLastIndex[iLayer] > 255)
            iLastIndex[iLayer] = 1;*/

        //now prepare new cache entry
        int iCurrentCacheEntry = -1;
        for (i=0; i<SVIN_SPRITE_CACHE_SPRITES_SIZE; i++)
        {
            //if there is unallocated cache, use it
            if (_svin_sprite_cache_sprites[i].status == SVIN_SPRITE_CACHE_UNUSED)
                iCurrentCacheEntry = i;
        }
        //if not found, selecting from the oldest
        if (iCurrentCacheEntry == -1)
        {
            //if we're out of cache entries, we should purge something and repeat
            _svin_sprite_cache_purge_oldest(iLayer);
            for (i=0; i<SVIN_SPRITE_CACHE_SPRITES_SIZE; i++)
            {
                //if there is unallocated cache, use it
                if (_svin_sprite_cache_sprites[i].status == SVIN_SPRITE_CACHE_UNUSED)
                    iCurrentCacheEntry = i;
            }
            /*
            iCurrentCacheEntry = 0;
            for (i=0; i<SVIN_SPRITE_CACHE_SPRITES_SIZE; i++)
            {
                //if there is unallocated cache, use it
                if (_svin_sprite_cache_sprites[i].age > _svin_sprite_cache_sprites[iCurrentCacheEntry].age)
                    iCurrentCacheEntry = i;
            } 
            //found the oldest one, use it, increase age for everyone else
            for (i=0; i<SVIN_SPRITE_CACHE_SPRITES_SIZE; i++)
            {
                _svin_sprite_cache_sprites[i].age = _svin_sprite_cache_sprites[i].age + 1;
            } 
            */
        }

        //found some entry, use it, increase age for everyone else
        for (i=0; i<SVIN_SPRITE_CACHE_SPRITES_SIZE; i++)
        {
            _svin_sprite_cache_sprites[i].age = _svin_sprite_cache_sprites[i].age + 1;
        } 

        //prepare cache entry
        strcpy(_svin_sprite_cache_sprites[iCurrentCacheEntry].filename,filename);
        _svin_sprite_cache_sprites[iCurrentCacheEntry].position = iPosition;
        _svin_sprite_cache_sprites[iCurrentCacheEntry].status = SVIN_SPRITE_CACHE_SHOWN;
        _svin_sprite_cache_sprites[iCurrentCacheEntry].age = 0;
        _svin_sprite_cache_sprites[iCurrentCacheEntry].size_x = iSizeX;
        _svin_sprite_cache_sprites[iCurrentCacheEntry].size_y = iSizeY;
        _svin_sprite_cache_sprites[iCurrentCacheEntry].layer = iLayer;

        //now pack usage data from big buffer
        for (i=0;i<384;i++)
        {
            _svin_sprite_cache_sprites[iCurrentCacheEntry].usage[i] = 0;
            for (int j=0; j<8; j++)
            {                
                if (big_buffer[i*8+j+2] != 0)
                    _svin_sprite_cache_sprites[iCurrentCacheEntry].usage[i] |= 1<<j;
            }
        }
      
            
        //fill data

        iFound = 0;

        _svin_set_cycle_patterns_cpu();


        x_end = x_start + iSizeX;

        for (y=0;y<iSizeY;y++)
        {
            for (x=x_start;x<x_end;x++)
            {
                if (big_buffer[y*iSizeX+(x-x_start)+2])
                {
                    //searching first free tile data slot
                    bFound = false;    
                    for (i=0;(i<iTilesCacheSize)&&(bFound == false);i++)
                    {
                        if (-1 == _svin_sprite_cache_tiles[i]) {
                            bFound = true;
                            iFound = i;
                            _svin_sprite_cache_tiles[i] = iCurrentCacheEntry;
                        }
                    }
                    //copying tile data
                    switch (iLayer)
                    {
                        case 0:
                            memcpy((char*)(_SVIN_NBG0_CHPNDR_START+iFound*64),big_buffer+iPointer,64);
                            p32 = (int*)_SVIN_NBG0_PNDR_START;
                            break;
                        case 1:
                            memcpy((char*)(_SVIN_NBG1_CHPNDR_START+iFound*64),big_buffer+iPointer,64);
                            p32 = (int*)_SVIN_NBG1_PNDR_START;
                            break;
                    }
                    //setting tile index
                    switch (iLayer)
                    {
                        case 0:
                            iOffset = (_SVIN_NBG0_CHPNDR_START - VDP2_VRAM_ADDR(0,0))/32;
                            if (x<64)
                            {
                                p32[y*64+x] = 0x00000000 | 0x100000*(iPaletteIndex) | (iOffset + iFound*2); //palette 0, transparency on
                                _svin_sprite_cache_names[y*64+x] = iCurrentCacheEntry;
                                assert (iFound < SVIN_SPRITE_CACHE_TILES_NBG0_SIZE);
                                _svin_sprite_cache_tiles[iFound] = iCurrentCacheEntry;
                            }
                            else
                            {
                                p32[(y+64)*64+x-64] = 0x00000000 | 0x100000*(iPaletteIndex) | (iOffset + iFound*2); //palette 0, transparency on
                                _svin_sprite_cache_names[(y+64)*64+x-64] = iCurrentCacheEntry;
                                assert (iFound < SVIN_SPRITE_CACHE_TILES_NBG0_SIZE);
                                _svin_sprite_cache_tiles[iFound] = iCurrentCacheEntry;
                            }
                            break;
                        case 1:
                            {
                                iOffset = (_SVIN_NBG1_CHPNDR_START - VDP2_VRAM_ADDR(0,0))/32;
                                if (x<64)
                                {
                                    p32[y*64+x] = 0x00000000 | 0x100000*(iPaletteIndex) | (iOffset + iFound*2); //palette 0, transparency on
                                    _svin_sprite_cache_names[y*64+x] = iCurrentCacheEntry;
                                    assert (iFound < SVIN_SPRITE_CACHE_TILES_NBG1_SIZE);
                                    _svin_sprite_cache_tiles[iFound] = iCurrentCacheEntry;
                                }
                                else
                                {
                                    p32[(y+64)*64+x-64] = 0x00000000 | 0x100000*(iPaletteIndex) | (iOffset + iFound*2); //palette 0, transparency on
                                    _svin_sprite_cache_names[(y+64)*64+x-64] = iCurrentCacheEntry;
                                    assert (iFound < SVIN_SPRITE_CACHE_TILES_NBG1_SIZE);
                                    _svin_sprite_cache_tiles[iFound] = iCurrentCacheEntry;
                                }
                            }
                            break;
                    }
                    //moving pointer
                    iPointer+=64;
                }
            }
        }

        //load palette
        switch (iPalette)
        {
            case 0:
                _svin_set_palette(iPaletteIndex,big_buffer+iPointer);
                break;
            case 1:
                _svin_set_palette_part(iPaletteIndex,big_buffer+iPointer,128,255);
                break;
            case 2:
                _svin_set_palette_part(iPaletteIndex,big_buffer+iPointer,0,127);
                break;
        }

        //copy palette into cache
        memcpy(_svin_sprite_cache_sprites[iCurrentCacheEntry].palette,big_buffer+iPointer,768);
        
        free(big_buffer);
    }

    _svin_set_cycle_patterns_nbg();

    
}