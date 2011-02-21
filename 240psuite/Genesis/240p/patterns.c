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

#include "genesis.h"
#include "res.h"
#include "patterns.h"


void DrawPluge()
{
    u16 size;
    u16 exit = 0;
    u16 buttons, oldButtons = 0xffff, pressedButtons;

    size = sizeof(pluge_tiles) / 32; 
    VDP_setPalette(PAL0, pluge_pal);
    VDP_loadTileData(pluge_tiles, TILE_USERINDEX, size, 1); 
    VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL0, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320/8, 224/8); 
    while(!exit)
    {
				buttons = JOY_readJoypad(JOY_1);
				pressedButtons = buttons & ~oldButtons;
				oldButtons = buttons;
		
				if (pressedButtons & BUTTON_START)
					exit = 1;

        VDP_waitVSync();
    }
}

void DrawGrayRamp()
{
    u16 size;
    u16 exit = 0;
    u16 buttons, oldButtons = 0xffff, pressedButtons;

    size = sizeof(grayramp_tiles) / 32; 
    VDP_setPalette(PAL0, white_pal);
    VDP_loadTileData(grayramp_tiles, TILE_USERINDEX, size, 1); 
    VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL0, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320/8, 224/8); 
    while(!exit)
    {
				buttons = JOY_readJoypad(JOY_1);
				pressedButtons = buttons & ~oldButtons;
				oldButtons = buttons;
		
				if (pressedButtons & BUTTON_START)
					exit = 1;

        VDP_waitVSync();
    }
}

void DrawWhiteScreen()
{
    u16 size;
    u16 exit = 0;
    u16 buttons, oldButtons = 0xffff, pressedButtons, redraw = 0;
		int	color = 0;

    size = sizeof(solid_tiles) / 32; 

    VDP_setPalette(PAL0, palette_grey);
    VDP_setPalette(PAL1, palette_red);
    VDP_setPalette(PAL2, palette_green);
    VDP_setPalette(PAL3, palette_blue);

    VDP_loadTileData(solid_tiles, TILE_USERINDEX, size, 1); 
    VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL0, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320/8, 224/8); 
    while(!exit)
    {
				if(redraw)
				{
					switch(color)
					{
						case 0:
							VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL0, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320/8, 224/8); 
							break;
						case 1:
							VDP_clearTileMapRect(APLAN, 0, 0, 320/8, 224/8);							
							break;
						case 2:
							VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320/8, 224/8); 
							break;
						case 3:
							VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL2, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320/8, 224/8); 
							break;
						case 4:
							VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL3, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320/8, 224/8); 
							break;
					}
					redraw = 0;
				}
				buttons = JOY_readJoypad(JOY_1);
				pressedButtons = buttons & ~oldButtons;
				oldButtons = buttons;
		
				if (pressedButtons & BUTTON_START)
					exit = 1;

				if (pressedButtons & BUTTON_A)
				{
					color ++;
					redraw = 1;
				}

				if (pressedButtons & BUTTON_B)
				{
					color --;
					redraw = 1;
				}	

				if(color > 4)
					color = 0;
		
				if(color < 0)
					color = 4;

        VDP_waitVSync();
    }
}

void Draw601ColorBars()
{
    u16 size;
    u16 exit = 0;
    u16 buttons, oldButtons = 0xffff, pressedButtons;

    size = sizeof(cb601_tiles) / 32; 
    VDP_setPalette(PAL0, cb601_pal);
    VDP_loadTileData(cb601_tiles, TILE_USERINDEX, size, 1); 
    VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL0, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320/8, 224/8); 
    while(!exit)
    {
				buttons = JOY_readJoypad(JOY_1);
				pressedButtons = buttons & ~oldButtons;
				oldButtons = buttons;
		
				if (pressedButtons & BUTTON_START)
					exit = 1;

        VDP_waitVSync();
    }
}

void DrawLinearity()
{
    u16 size, ind, grid2, redraw = 0;
    u16 exit = 0, showgrid = 1, gridpattern = 1;
    u16 buttons, oldButtons = 0xffff, pressedButtons;

    VDP_setPalette(PAL0, bw_pal);

    ind = TILE_USERINDEX; 
    size = sizeof(circles_grid_tiles) / 32; 
    VDP_loadTileData(circles_grid_tiles, ind, size, 1); 
    ind += size;
    size = sizeof(circles_griddot_tiles) / 32; 
    VDP_loadTileData(circles_griddot_tiles, ind, size, 1); 

    grid2 = ind;

    ind += size;
    size = sizeof(circles_tiles) / 32; 
    VDP_loadTileData(circles_tiles, ind, size, 1); 

    VDP_fillTileMapRect(BPLAN, TILE_ATTR(PAL0, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320/8, 224/8); 
    VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL0, 0, 0, 0) + ind, 0, 0, 320/8, 224/8); 
    
    while(!exit)
    {
        if(redraw)
        {
            if(showgrid)
            {
                if(gridpattern == 1)
                    VDP_fillTileMapRect(BPLAN, TILE_ATTR(PAL0, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320/8, 224/8); 
                else
                    VDP_fillTileMapRect(BPLAN, TILE_ATTR(PAL0, 0, 0, 0) + grid2, 0, 0, 320/8, 224/8); 
            }
            else
                VDP_clearTileMapRect(BPLAN, 0, 0, 320/8, 224/8);
            redraw = 0;
        }   

				buttons = JOY_readJoypad(JOY_1);
				pressedButtons = buttons & ~oldButtons;
				oldButtons = buttons;
		
				if (pressedButtons & BUTTON_START)
					exit = 1;

        if (pressedButtons & BUTTON_A)
        {
            showgrid = !showgrid;            
            redraw = 1;
        }
        
        if (pressedButtons & BUTTON_B)
				{
            if(gridpattern == 1)
                gridpattern = 2;
            else
                gridpattern = 1;

            redraw = 1;
        }

        VDP_waitVSync();
    }
}

void DrawGrid()
{
    u16 size;
    u16 exit = 0;
    u16 buttons, oldButtons = 0xffff, pressedButtons;

    size = sizeof(grid_tiles) / 32; 
    VDP_setPalette(PAL0, grid_pal);
    VDP_loadTileData(grid_tiles, TILE_USERINDEX, size, 1); 
    VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL0, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320/8, 224/8); 
    while(!exit)
    {
				buttons = JOY_readJoypad(JOY_1);
				pressedButtons = buttons & ~oldButtons;
				oldButtons = buttons;
		
				if (pressedButtons & BUTTON_START)
				exit = 1;

        VDP_waitVSync();
    }
}

void DrawColorBars()
{
    u16 ind; 
    u16 size;
    u16 exit;
    u16 buttons, oldButtons = 0xffff, pressedButtons;

    exit = 0;

    VDP_setPalette(PAL0, red_pal); 
    VDP_setPalette(PAL1, green_pal); 
    VDP_setPalette(PAL2, blue_pal); 
    VDP_setPalette(PAL3, white_pal); 


    ind = TILE_USERINDEX; 
    size = sizeof(red_tiles) / 32; 
    VDP_loadTileData(red_tiles, ind, size, 1); 
    ind += size; 
    size = sizeof(green_tiles) / 32; 
    VDP_loadTileData(green_tiles, ind, size, 1); 
    ind += size; 
    size = sizeof(blue_tiles) / 32; 
    VDP_loadTileData(blue_tiles, ind, size, 1); 
    ind += size; 
    size = sizeof(white_tiles) / 32; 
    VDP_loadTileData(white_tiles, ind, size, 1); 


    ind = TILE_USERINDEX; 
    VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL0, 0, 0, 0) + ind, 0, 0, 320/8, 56/8); 
    ind += sizeof(red_tiles) / 32; 
    VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + ind, 0, 56/8, 320/8, 56/8); 
    ind += sizeof(green_tiles) / 32; 
    VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL2, 0, 0, 0) + ind, 0, 112/8, 320/8, 56/8); 
    ind += sizeof(blue_tiles) / 32; 
    VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL3, 0, 0, 0) + ind, 0, 168/8, 320/8, 56/8); 

    while(!exit)
    {
				buttons = JOY_readJoypad(JOY_1);
				pressedButtons = buttons & ~oldButtons;
				oldButtons = buttons;
		
				if (pressedButtons & BUTTON_START)
					exit = 1;

        VDP_waitVSync();
    }
}