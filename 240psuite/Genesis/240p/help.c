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
#include "help.h"

void DrawHelp(int option)
{
  u16 ind = 0, size = 0, exit = 0;
  u16 buttons, oldButtons = 0xffff, pressedButtons;

  VDP_setPalette(PAL0, palette_grey);
  VDP_setPalette(PAL1, back_pal); 
  VDP_setPalette(PAL2, palette_green);
  VDP_setPalette(PAL3, bw_pal);    
  
  ind = TILE_USERINDEX; 
  size = sizeof(back_tiles) / 32; 
  VDP_loadTileData(back_tiles, ind, size, 1); 
  
  VDP_fillTileMapRectInc(BPLAN, TILE_ATTR(PAL1, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320/8, 224/8);    

	switch(option)
	{
		case HELP_GENERAL:			
			VDP_drawTextBG(APLAN, "             HELP", TILE_ATTR(PAL2, 0, 0, 0), 4, 6);
			VDP_drawTextBG(APLAN, "The 240p Test Suite was designed", TILE_ATTR(PAL0, 0, 0, 0), 4, 8);
			VDP_drawTextBG(APLAN, "with two goals in mind:", TILE_ATTR(PAL0, 0, 0, 0), 4, 9);
			VDP_drawTextBG(APLAN, "1) Evaluate 240p signals on TV", TILE_ATTR(PAL0, 0, 0, 0), 4, 11);
			VDP_drawTextBG(APLAN, "sets and video procesors; and", TILE_ATTR(PAL0, 0, 0, 0), 4, 12);
			VDP_drawTextBG(APLAN, "2) Provide calibration patterns", TILE_ATTR(PAL0, 0, 0, 0), 4, 14);
			VDP_drawTextBG(APLAN, "from a game console to help in", TILE_ATTR(PAL0, 0, 0, 0), 4, 15);
			VDP_drawTextBG(APLAN, "properly calibrating the display", TILE_ATTR(PAL0, 0, 0, 0), 4, 16);
			VDP_drawTextBG(APLAN, "black, white and color levels.", TILE_ATTR(PAL0, 0, 0, 0), 4, 17);
			break;
		case HELP_PLUGE:			
			
			break;
		case HELP_COLORS:			
			
			break;
		case HELP_GRID:			
			
			break;
	}
  
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

