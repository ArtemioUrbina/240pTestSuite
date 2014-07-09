/*---------------------------------------------------------------------------------


	240p Suite
	Artemio Urbina 2014


---------------------------------------------------------------------------------*/
#include <snes.h>	
#include "res.h"

//---------------------------------------------------------------------------------
void DrawGrid();

//---------------------------------------------------------------------------------
int main(void) 
{
	u16 sel = 0, redraw = 1;
	u16 pad0, oldpad = 0xffff, pressed;
	    
	consoleInit();	
	consoleInitText(0, 1, &font);		
	
	// Main loop
	while(1) 
	{
		u16 change = 0;
		
		if(redraw)
		{
			u16 vrampos = 0x4000, size = 0, mappos = 0x1000;				
			
			// Copy tiles to VRAM
			size = (&back_tiles_end - &back_tiles);
			bgInitTileSet(1, &back_tiles, &back_pal, 0, size, 16*2, BG_16COLORS, vrampos);
			vrampos += size;			

			// Copy Map to VRAM
			size = (&back_map_end - &back_map);
			bgInitMapSet(1, &back_map, size, SC_32x32, mappos);
			mappos += size;			
			
			consoleSetTextCol(RGB15(31,0,0), RGB15(0,0,0));
			consoleDrawText(4, 6, "Grid");
			
			/*
			size = (&gillian_tiles_end - &gillian_tiles);
			oamInitGfxSet(&gillian_tiles, size, &gillian_pal, 0, vrampos, OBJ_SIZE32);
			oamSet(0, 10, 10, 3, 0, 0, 0, 2);
			oamSetEx(0, OBJ_LARGE, OBJ_SHOW);
			*/
						
			setMode(BG_MODE1,0); 	
			bgSetDisable(2);		
			
			bgSetScroll(1, 0, -1);			
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
	
	bgInitTileSet(1, &grid_tiles, &grid_pal, 0, (&grid_tiles_end - &grid_tiles), 16*2, BG_16COLORS, 0x4000);	
	bgInitMapSet(1, &grid_map, (&grid_map_end - &grid_map), SC_32x32, 0x1000);
	
	setMode(BG_MODE1,0); 
	bgSetDisable(0);		
	bgSetDisable(2);
	
	bgSetScroll(1, 0, -1);
		
	while(!end) 
	{		
		scanPads();
		pad0 = padsCurrent(0);
		
		pressed = pad0 & ~oldpad;
		oldpad = pad0;
		
					
		if(pressed == KEY_A)				
			end = 1;		
				
		
		WaitForVBlank();
	}
	
	return;
}