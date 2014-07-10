/*---------------------------------------------------------------------------------


	240p Suite
	Artemio Urbina 2014


---------------------------------------------------------------------------------*/
#include <snes.h>
#include "res.h"

#define REG_SETINI (*(vuint8*)0x2133)

//---------------------------------------------------------------------------------
void DrawGrid();

void Set240pMode()
{
	REG_SETINI |= 0x04;
}

void Set224pMode()
{
	REG_SETINI = 0;
}

void LoadPalette(u8 bgNumber, u8 *tilePalette, u8 paletteEntry, u16 paletteSize, u16 colorMode) 
{
	u16 palEntry;
	
	// If mode 0, compute palette entry with separate subpalettes in entries 0-31, 32-63, 64-95, and 96-127
	if (colorMode == BG_4COLORS0)
		palEntry = bgNumber*32 + paletteEntry*BG_4COLORS;
	else
		palEntry = paletteEntry*colorMode;			
	
  	dmaCopyCGram(tilePalette, palEntry, paletteSize);
}

//---------------------------------------------------------------------------------
int main(void) 
{
	u16 sel = 0, redraw = 1;
	u16 pad0, oldpad = 0xffff, pressed;
	    
	setBrightness(0);
	
	consoleInit();	
	consoleInitText(0, 1, &font);	

	// Main loop
	while(1) 
	{
		u16 change = 0;				
		
		if(redraw)
		{
			u16 vrampos = 0x4000, size = 0, mappos = 0x1000;				
					
			setBrightness(0);	
			
			size = (&back_tiles_end - &back_tiles);
			bgInitTileSet(1, &back_tiles, &back_pal, 0, size, 16*2, BG_16COLORS, vrampos);
			vrampos += size;			
			
			size = (&back_map_end - &back_map);
			bgInitMapSet(1, &back_map, size, SC_32x32, mappos);
			mappos += size;			
			
			consoleDrawText(4, 6, "Grid");
			consoleDrawText(4, 7, "Horizontal stripes");	
			
			/*
			size = (&gillian_tiles_end - &gillian_tiles);
			bgInitTileSet(2, &gillian_tiles, &gillian_pal, 2, size, 16*2, BG_16COLORS, vrampos);
			vrampos += size;			
			
			size = (&gillian_map_end - &gillian_map);
			bgInitMapSet(2, &gillian_map, size, SC_32x32, mappos);
			mappos += size;			
			*/
						
			setMode(BG_MODE1,0); 	
			bgSetDisable(2);		
			
			bgSetScroll(1, 0, -1);			
			
			setBrightness(0xF);
			WaitForVBlank();						
			redraw = 0;
		}			
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
					
		if(pressed == KEY_DOWN)
		{
			sel+=1;
			change = 1;
		}
		if(pressed == KEY_UP)
		{			
			sel-=1;
			change = 1;		
		}	

		if(sel != 0)
			sel = 0;
			
		if(pressed == KEY_B)
		{	
			setFadeEffect(FADE_OUT);		
			DrawGrid();
			redraw = 1;
		}
		
		WaitForVBlank();
	}
	return 0;
}

void DrawGrid(void) 
{	
	u16 pad0, oldpad = 0xffff, pressed, end = 0;	 
	u16 toggle = 0, redraw = 1;
	
	bgInitTileSet(1, &grid_tiles, &grid_pal, 0, (&grid_tiles_end - &grid_tiles), 16*2, BG_16COLORS, 0x4000);	
	while(!end) 
	{		
		if(redraw)
		{
			setBrightness(0);
			
			if(toggle)
			{					
				bgInitMapSet(1, &grid240_map, (&grid240_map_end - &grid240_map), SC_32x32, 0x1000);
			
				Set240pMode();
			}
			else
			{				
				bgInitMapSet(1, &grid_map, (&grid_map_end - &grid_map), SC_32x32, 0x1000);
			
				Set224pMode();
			}
			setMode(BG_MODE1,0); 
			bgSetDisable(0);		
			bgSetDisable(2);
			
			bgSetScroll(1, 0, -1);
			setBrightness(0xF);
			redraw = 0;
		}
		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
		
		if(pressed == KEY_A)
			end = 1;		
			
		if(pressed == KEY_B)
		{
			toggle = !toggle;
			redraw = 1;
		}
		
		WaitForVBlank();
	}	
	setFadeEffect(FADE_OUT);
	Set224pMode();
	
	return;
}