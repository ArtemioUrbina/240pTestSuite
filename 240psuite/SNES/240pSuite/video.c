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
#include "res.h"
#include "font.h"
#include "control.h"
#include "help.h"

#define REG_SETINI (*(vuint8*)0x2133)

u8 interlaced = 0;
u8 mode240p = 0;
u8 pseudo512 = 0;
u16 bitflags = 0;

void Set240pMode()
{
	mode240p = 1;
	
	bitflags |= BIT(2);
	REG_SETINI = bitflags;
}

void Set224pMode()
{
	mode240p = 0;
	
	bitflags &= ~BIT(2);
	REG_SETINI = bitflags;
}

void SetInterlaced()
{
	interlaced = 1;
	
	bitflags |= BIT(0);
	REG_SETINI = bitflags;
}

void ClearInterlaced()
{
	interlaced = 0;
	
	bitflags &= ~BIT(0);
	REG_SETINI = bitflags;
}

void SetH512Mode()
{
	pseudo512 = 1;
	
	bitflags |= BIT(3);
	REG_SETINI = bitflags;	
}

void ClearH512Mode()
{
	pseudo512 = 0;
	
	bitflags &= ~BIT(3);
	REG_SETINI = bitflags;	
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

void ClearScreen(u8 layer)
{	
	bgInitTileSetMine(layer, &fullscreen_tiles, &grid_pal, 0, (&fullscreen_tiles_end - &fullscreen_tiles), 16*2, BG_16COLORS, 0x6000);	
	bgInitMapSetMine(layer, &fullscreen_map, (&fullscreen_map_end - &fullscreen_map), SC_32x32, 0x7000);
}

void ClearScreen256(u8 layer)
{	
	bgInitTileSetMine(layer, &fullscreen256_tiles, &grid_pal, 0, (&fullscreen256_tiles_end - &fullscreen256_tiles), 16*2, BG_256COLORS, 0x6000);	
	bgInitMapSetMine(layer, &fullscreen_map, (&fullscreen_map_end - &fullscreen_map), SC_32x32, 0x7000);
}

inline void Transition()
{
	//setFadeEffect(FADE_OUT);
}

void ChangeVideo()
{
	u16 redraw = 1, change = 0, end = 0;
	u16 pressed;	
	s16 sel = 0;

	while(!end) 
	{
		if(redraw)
		{
			u16 size = 0;
					
			setBrightness(0);	
			
			CleanFontMap();
			consoleInitText(0, 7, &font);
			AddTextColor(7, RGB5(31, 31, 31), RGB5(0, 0, 0));
			AddTextColor(6, RGB5(0, 31, 0), RGB5(0, 0, 0));	
			AddTextColor(5, RGB5(0, 27, 27), RGB5(0, 0, 0));	
			
			size = (&back_tiles_end - &back_tiles);
			bgInitTileSet(1, &back_tiles, &back_pal, 1, size, 16*2, BG_16COLORS, 0x6000);			
			
			size = (&back_map_end - &back_map);	
			bgInitMapSet(1, &back_map, size, SC_32x32, 0x2000);
						
			setMode(BG_MODE1,0); 	
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);					
			
			redraw = 0;
			change = 1;
		}			
		
		if(change)
		{
			u16 y = 12;						
			
			CleanFontMap();
			drawText(5, y++, sel == 0 ? 6 : 7, "%s interlaced mode", interlaced ? "Disable" : "Enable");
			//drawText(5, y++, sel == 1 ? 6 : 7, "%s pseudo 512 mode", pseudo512 ? "Disable" : "Enable");
			y+=4;
			drawText(5, y, sel == 1 ? 6 : 5, "Back to main menu");
			
			change = 0;
		}	

		spcProcess();	
		WaitForVBlank();	
		
		pressed = PadPressed(0);
		
		if(pressed == KEY_START)
		{
			DrawHelp(HELP_VIDEO);
			redraw = 1;
		}
		
		if(pressed == KEY_A)
		{
			switch(sel)
			{
				case 0:
					if(interlaced)
						ClearInterlaced();
					else
						SetInterlaced();
					break;
				case 1:
				/*
					if(pseudo512)
						ClearH512Mode();
					else
						SetH512Mode();
					break;
				case 2:
				*/
					end = 1;
					break;
			}
			change = 1;
		}
			
		if(pressed == KEY_B)
			end = 1;	
		
		if(pressed == KEY_UP)
		{
			sel --;
			change = 1;
		}
			
		if(pressed == KEY_DOWN)
		{
			sel ++;
			change = 1;
		}

		if(sel < 0)
			sel = 1;
		if(sel > 1)
			sel = 0;		
		
	}
	spcStop();
	Transition();
	return;
}

//#define DEBUG_DMA

#ifdef DEBUG_DMA
char msg[100];
#endif
u16 dmadata = 0;

#define DMA_LIMIT 40000

void StartDMA()
{
#ifdef DEBUG_DMA
	consoleNocashMessage("-----------------------\n");
#endif
	
	setBrightness(0);  // Force VBlank Interrupt
	WaitForVBlank(); 
	
	dmadata = 0;
}

void EndDMA()
{
#ifdef DEBUG_DMA
	sprintf(msg, "Total %d\n", dmadata);
	consoleNocashMessage(msg);
#endif
	
	setBrightness(0xF);  // Screen with all brightness
}

void bgInitTileSetMine(u8 bgNumber, u8 *tileSource, u8 *tilePalette, u8 paletteEntry, u16 tileSize, u16 paletteSize, u16 colorMode, u16 address) 
{
	u16 palEntry;

#ifdef DEBUG_DMA
	sprintf(msg, "tile size: %d\npalette size: %d\n", tileSize, paletteSize);
	consoleNocashMessage(msg);
#endif
	
	dmadata += tileSize;
	dmadata += paletteSize;
	
	// If mode 0, compute palette entry with separate subpalettes in entries 0-31, 32-63, 64-95, and 96-127
	if (colorMode == BG_4COLORS0)
		palEntry = bgNumber*32 + paletteEntry*BG_4COLORS;
	else
		palEntry = paletteEntry*colorMode;
	
	if(dmadata >= DMA_LIMIT)
	{
#ifdef DEBUG_DMA
		consoleNocashMessage("Forced vblank\n");
#endif
		setBrightness(0);  // Force VBlank Interrupt
		WaitForVBlank(); 
		dmadata = 0;
		dmadata += tileSize;
		dmadata += paletteSize;
	}	
	
	// Send to VRAM and CGRAM
	dmaCopyVram(tileSource, address, tileSize);
  	dmaCopyCGram(tilePalette, palEntry, paletteSize);
	bgSetGfxPtr(bgNumber, address);
}

void CopyExtraTiles(u8 * source, u16 address, u16 size)
{
#ifdef DEBUG_DMA
	sprintf(msg, "extra tiles size: %d\n", size);
	consoleNocashMessage(msg);
#endif
	
	dmadata += size;
	
	if(dmadata >= DMA_LIMIT)
	{
#ifdef DEBUG_DMA
		consoleNocashMessage("Forced vblank\n");
#endif
		setBrightness(0);  // Force VBlank Interrupt
		WaitForVBlank(); 
		dmadata = 0;
		dmadata += size;
	}
	
	dmaCopyVram(source, address, size);
}

void bgInitMapSetMine(u8 bgNumber, u8 *mapSource, u16 mapSize, u8 sizeMode, u16 address) 
{
#ifdef DEBUG_DMA
	sprintf(msg, "map size: %d\n", mapSize);
	consoleNocashMessage(msg);
#endif
	
	dmadata += mapSize;
	
	if(dmadata >= DMA_LIMIT)
	{
#ifdef DEBUG_DMA
		consoleNocashMessage("Forced vblank\n");
#endif
		setBrightness(0);  // Force VBlank Interrupt
		dmadata = 0;
		dmadata += mapSize;
	}
	
	dmaCopyVram(mapSource, address,mapSize);
	if (bgNumber != 0xff)
		bgSetMapPtr(bgNumber, address, sizeMode);
}

void oamInitGfxSetMine(u8 *tileSource, u16 tileSize, u8 *tilePalette, u16 paletteSize, u8 tilePaletteNumber, u16 address, u8 oamsize) 
{
	unsigned char palEntry;
#ifdef DEBUG_DMA
	sprintf(msg, "sprite size: %d\nSprite palette: %d\n", tileSize, paletteSize);
	consoleNocashMessage(msg);
#endif
	
	dmadata += tileSize;
	dmadata += paletteSize;

	if(dmadata >= DMA_LIMIT)
	{
#ifdef DEBUG_DMA
		consoleNocashMessage("Forced vblank\n");
#endif
		WaitForVBlank(); 
		dmadata = 0;
		dmadata += tileSize;
		dmadata += paletteSize;
	}

	// Init tiles
	dmaCopyVram(tileSource, (address), tileSize);
	
	// Init palette
	palEntry = (128+tilePaletteNumber*16);
  	dmaCopyCGram(tilePalette, palEntry, paletteSize);

	// Update base adress if needed (16k byte aligned)
	REG_OBSEL = oamsize | (address >> 13);
}

extern u16  maptext_adress;
extern u8   palette_adress, palette_number;

void consoleInitTextMine(u8 bgNumber,u8 paletteNumber, u8 *gfxText) 
{
	u16 x;
	
	// Copy data to VRAM (also clear screen)
	for (x=0;x<0x800;x++) pvsneslibfont_map[x] = 0x00;

#ifdef DEBUG_DMA
	sprintf(msg, "font size: %d\nfont map: %d\n", 3072, 0x800);
	consoleNocashMessage(msg);
#endif
	
	dmadata += 3072;
	dmadata += 0x800;

	if(dmadata >= DMA_LIMIT)
	{
#ifdef DEBUG_DMA
		consoleNocashMessage("Forced vblank\n");
#endif
		setBrightness(0); // Force VBkank
		dmadata = 0;
		dmadata += 3072;
		dmadata += 0x800;
	}
	
	dmaCopyVram(gfxText, 0x000, 3072); 
	dmaCopyVram(pvsneslibfont_map, 0x800, 0x800);

	// Initialize background tile & map
	maptext_adress = 0x800;
	palette_number = paletteNumber;
	palette_adress = (palette_number<<2) | (1<<5); // (10-7) because only high byte are addressed
    bgSetGfxPtr(bgNumber, 0x000);
    bgSetMapPtr(bgNumber, 0x800, SC_32x32);

	// Font Border Color
	REG_CGADD = 0x00+(paletteNumber<<4); *CGRAM_PALETTE = RGB5(0,0,0) & 0xFF; *CGRAM_PALETTE = RGB5(0,0,0)>>8;

	// Font Color
	REG_CGADD = 0x01+(paletteNumber<<4); *CGRAM_PALETTE = RGB5(31,31,31)  & 0xFF; *CGRAM_PALETTE = RGB5(31,31,31)>>8;
}
