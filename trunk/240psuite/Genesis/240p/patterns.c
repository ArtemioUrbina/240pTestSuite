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
#include "help.h"


void DrawPluge()
{
  u16 size, tiles;
  u16 exit = 0;
  u16 buttons, oldButtons = 0xffff, pressedButtons;

  if(showhelp)
    DrawHelp(HELP_PLUGE);

  VDP_setHilightShadow(1);

  tiles = TILE_USERINDEX;
  VDP_setPalette(PAL0, palette_grey);
  size = sizeof(color_tiles) / 32; 
  VDP_loadTileData(color_tiles, tiles, size, 1);   

  // 6% & 13% black
  VDP_fillTileMapRect(APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, 0) + tiles, 5, 4, 2, 20);     
  VDP_fillTileMapRect(APLAN, TILE_ATTR_FULL(PAL0, 0, 0, 0, 0) + tiles, 9, 4, 2, 20);
  VDP_fillTileMapRect(APLAN, TILE_ATTR_FULL(PAL0, 0, 0, 0, 0) + tiles, 29, 4, 2, 20);
  VDP_fillTileMapRect(APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, 0) + tiles, 33, 4, 2, 20);

  VDP_fillTileMapRect(APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, 6) + tiles, 15, 4, 10, 5);
  VDP_fillTileMapRect(APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, 4) + tiles, 15, 9, 10, 5);
  VDP_fillTileMapRect(APLAN, TILE_ATTR_FULL(PAL0, 1, 0, 0, 1) + tiles, 15, 14, 10, 5);
  VDP_fillTileMapRect(APLAN, TILE_ATTR_FULL(PAL0, 0, 0, 0, 2) + tiles, 15, 19, 10, 5);

  while(!exit)
  {
    buttons = JOY_readJoypad(JOY_1);
    pressedButtons = buttons & ~oldButtons;
    oldButtons = buttons;

    if (pressedButtons & BUTTON_START)      
      exit = 1;

    VDP_waitVSync();
  }
  VDP_setHilightShadow(0);
}

void DrawGrayRamp()
{
  u16 size;
  u16 exit = 0;
  u16 buttons, oldButtons = 0xffff, pressedButtons;

  VDP_setHilightShadow(1);

  VDP_setPalette(PAL0, palette_grey);
  size = sizeof(color_tiles) / 32; 
  VDP_loadTileData(color_tiles, TILE_USERINDEX, size, 1);   
  DrawColorTilesAt(APLAN, PAL0, 0, 0, TILE_USERINDEX, 3, 14);
  DrawColorTilesAtInv(APLAN, PAL0, 0, 14, TILE_USERINDEX, 3, 14);

  //Center it, we are missinga few tiles tiles, so scroll 16 to center the pattern
  VDP_setHorizontalScroll(APLAN, 0, 16);
  while(!exit)
  {
    buttons = JOY_readJoypad(JOY_1);
    pressedButtons = buttons & ~oldButtons;
    oldButtons = buttons;

    if (pressedButtons & BUTTON_START)
      exit = 1;

    VDP_waitVSync();
  }
  VDP_setHilightShadow(0);
  VDP_setHorizontalScroll(APLAN, 0, 0);
}

void DrawWhiteScreen()
{
  u16 size;
  u16 exit = 0;
  u16 buttons, oldButtons = 0xffff, pressedButtons, redraw = 0;
  int  color = 0;

  size = sizeof(solid_tiles) / 32; 

  VDP_loadTileData(solid_tiles, TILE_USERINDEX, size, 1); 
  VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL0, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320/8, 224/8); 
  while(!exit)
  {
    if(redraw)
    {
      switch(color)
      {
        case 0:
          VDP_setPalette(PAL0, palette_grey);           
          break;
        case 1:
          VDP_setPalette(PAL0, palette_black);          
          break;
        case 2:
          VDP_setPalette(PAL0, palette_red);          
          break;
        case 3:
          VDP_setPalette(PAL0, palette_green);          
          break;
        case 4:
          VDP_setPalette(PAL0, palette_blue);          
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
  u16 size, i = 0, j = 0;
  u16 exit = 0;
  u16 buttons, oldButtons = 0xffff, pressedButtons;
  u16 posx = 0, posy = 0, pal = PAL0;

  if(showhelp)
    DrawHelp(HELP_GRID);
  
  VDP_setPalette(PAL0, palette_grey);
  VDP_setPalette(PAL1, palette_red);

  size = sizeof(grid_tiles) / 32; 
  VDP_loadTileData(grid_tiles, TILE_USERINDEX, size, 1); 
  
  // 40 x 28 tiles, tiles on row 0 and 28 are red, 
  // tiles on column 0 and 40 are red, the rest are white

  for(i = 0; i < 20; i++)
  {
    for(j = 0; j < 14; j++)
    {      
      posx = i*2;
      posy = j*2;
      if(i*j == 0 || i == 19 || j == 13)
        pal = PAL1;
      else
        pal = PAL0;
      VDP_setTileMap(APLAN, TILE_ATTR(pal, 0, 0, 0) + TILE_USERINDEX, posx, posy); 
      VDP_setTileMap(APLAN, TILE_ATTR(pal, 0, 0, 1) + TILE_USERINDEX, posx+1, posy); 
      VDP_setTileMap(APLAN, TILE_ATTR(pal, 0, 1, 0) + TILE_USERINDEX, posx, posy+1); 
      VDP_setTileMap(APLAN, TILE_ATTR(pal, 0, 1, 1) + TILE_USERINDEX, posx+1, posy+1);       
    }
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

void DrawColorBars()
{
    u16 ind;
    u16 size;
    u16 exit;
    u16 buttons, oldButtons = 0xffff, pressedButtons;

    exit = 0;

    if(showhelp)
      DrawHelp(HELP_COLORS);
    VDP_setPalette(PAL0, palette_red); 
    VDP_setPalette(PAL1, palette_green); 
    VDP_setPalette(PAL2, palette_blue); 
    VDP_setPalette(PAL3, palette_grey); 

    VDP_setHilightShadow(1);
    ind = TILE_USERINDEX; 
    size = sizeof(color_tiles) / 32; 
    VDP_loadTileData(color_tiles, ind, size, 1); 

    VDP_drawTextBG(APLAN, "  0  1  2  3  4  5  6  7  8  A  C  E", TILE_ATTR(PAL0, 1, 0, 0), 2, 4);        
    VDP_drawTextBG(APLAN, "RED", TILE_ATTR(PAL0, 1, 0, 0), 2, 9);        
    VDP_drawTextBG(APLAN, "GREEN", TILE_ATTR(PAL1, 1, 0, 0), 2, 14);        
    VDP_drawTextBG(APLAN, "BLUE", TILE_ATTR(PAL2, 1, 0, 0), 2, 19);        
    VDP_drawTextBG(APLAN, "WHITE", TILE_ATTR(PAL3, 1, 0, 0), 2, 24);        
    DrawColorTilesAt(APLAN, PAL0, 2, 5, ind, 3, 4);
    DrawColorTilesAt(APLAN, PAL1, 2, 10, ind, 3, 4);
    DrawColorTilesAt(APLAN, PAL2, 2, 15, ind, 3, 4);
    DrawColorTilesAt(APLAN, PAL3, 2, 20, ind, 3, 4);    

    while(!exit)
    {
        buttons = JOY_readJoypad(JOY_1);
        pressedButtons = buttons & ~oldButtons;
        oldButtons = buttons;
    
        if (pressedButtons & BUTTON_START)
          exit = 1;

        VDP_waitVSync();
    }
    VDP_setHilightShadow(0);
}

void DrawColorTilesAt(u16 plan, u16 pal, u16 x, u16 y, u16 tiles, u16 w, u16 h)
{    
    VDP_fillTileMapRect(plan, TILE_ATTR_FULL(pal, 0, 0, 0, 0) + tiles, x+w*1, y, w, h);     
    VDP_fillTileMapRect(plan, TILE_ATTR_FULL(pal, 1, 0, 0, 0) + tiles, x+w*2, y, w, h);     

    VDP_fillTileMapRect(plan, TILE_ATTR_FULL(pal, 0, 0, 0, 2) + tiles, x+w*3, y, w, h);     
    VDP_fillTileMapRect(plan, TILE_ATTR_FULL(pal, 1, 0, 0, 1) + tiles, x+w*4, y, w, h);     

    VDP_fillTileMapRect(plan, TILE_ATTR_FULL(pal, 0, 0, 0, 4) + tiles, x+w*5, y, w, h);     
    VDP_fillTileMapRect(plan, TILE_ATTR_FULL(pal, 1, 0, 0, 2) + tiles, x+w*6, y, w, h);     

    VDP_fillTileMapRect(plan, TILE_ATTR_FULL(pal, 0, 0, 0, 6) + tiles, x+w*7, y, w, h);     
    VDP_fillTileMapRect(plan, TILE_ATTR_FULL(pal, 1, 0, 0, 3) + tiles, x+w*8, y, w, h);     
    
    VDP_fillTileMapRect(plan, TILE_ATTR_FULL(pal, 1, 0, 0, 4) + tiles, x+w*9, y, w, h);     
    VDP_fillTileMapRect(plan, TILE_ATTR_FULL(pal, 1, 0, 0, 5) + tiles, x+w*10, y, w, h);     
    VDP_fillTileMapRect(plan, TILE_ATTR_FULL(pal, 1, 0, 0, 6) + tiles, x+w*11, y, w, h);     
}

void DrawColorTilesAtInv(u16 plan, u16 pal, u16 x, u16 y, u16 tiles, u16 w, u16 h)
{    
    VDP_fillTileMapRect(plan, TILE_ATTR_FULL(pal, 0, 0, 0, 0) + tiles, x+w*10, y, w, h);     
    VDP_fillTileMapRect(plan, TILE_ATTR_FULL(pal, 1, 0, 0, 0) + tiles, x+w*9, y, w, h);     

    VDP_fillTileMapRect(plan, TILE_ATTR_FULL(pal, 0, 0, 0, 2) + tiles, x+w*8, y, w, h);     
    VDP_fillTileMapRect(plan, TILE_ATTR_FULL(pal, 1, 0, 0, 1) + tiles, x+w*7, y, w, h);     

    VDP_fillTileMapRect(plan, TILE_ATTR_FULL(pal, 0, 0, 0, 4) + tiles, x+w*6, y, w, h);     
    VDP_fillTileMapRect(plan, TILE_ATTR_FULL(pal, 1, 0, 0, 2) + tiles, x+w*5, y, w, h);     

    VDP_fillTileMapRect(plan, TILE_ATTR_FULL(pal, 0, 0, 0, 6) + tiles, x+w*4, y, w, h);     
    VDP_fillTileMapRect(plan, TILE_ATTR_FULL(pal, 1, 0, 0, 3) + tiles, x+w*3, y, w, h);     
    
    VDP_fillTileMapRect(plan, TILE_ATTR_FULL(pal, 1, 0, 0, 4) + tiles, x+w*2, y, w, h);     
    VDP_fillTileMapRect(plan, TILE_ATTR_FULL(pal, 1, 0, 0, 5) + tiles, x+w*1, y, w, h);     
    VDP_fillTileMapRect(plan, TILE_ATTR_FULL(pal, 1, 0, 0, 6) + tiles, x+w*0, y, w, h);     
}
