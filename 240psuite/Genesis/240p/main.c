/* 
 * 240p Test Suite
 * Copyright (C)2011 Artemio Urbina
 *
 * This file is part of the 240p Test Suite
 *
 * To Eleven is free software; you can redistribute it and/or modify
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
#include "tests.h"
#include "help.h"
#include "myvdp.h"

void TestPatternMenu();
void DrawCredits();
void DrawIntro();

int main() 
{ 
  u16 cursel = 1, pos, reload = 1;
  u16 buttons, oldButtons = 0xffff, pressedButtons;
  u16 ind = 0, size = 0;
  
  VDP_init(); 
  JOY_init();
  
  VDP_setScreenWidth320(); 
  VDP_setScreenHeight224(); 
  
  VDP_loadFont(font_tiles, USE_DMA);
  DrawIntro();
  while(1)
  {    
    if(reload)
    {
      VDP_setPalette(PAL2, back_pal); 
      VDP_setPalette(PAL3, gillian_pal); 

      ind = TILE_USERINDEX; 
      size = sizeof(back_tiles) / 32; 
      VDP_loadTileData(back_tiles, ind, size, USE_DMA); 
      ind += size;
      size = sizeof(gillian_tiles) / 32; 
      VDP_loadTileData(gillian_tiles, ind, size, USE_DMA); 

      VDP_setMyTileMapRect(BPLAN, back_map, TILE_USERINDEX, 0, 0, 320/8, 224/8);    
      VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL3, 0, 0, 0) + ind, 200/8, 80/8, 72/8, 112/8);
      reload = 0;
    }        
              
    pos = 6;
    VDP_drawTextBG(APLAN, "Test Patterns", TILE_ATTR(cursel == 1 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);        
    VDP_drawTextBG(APLAN, "Drop Shadow Test", TILE_ATTR(cursel == 2 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);        
    VDP_drawTextBG(APLAN, "Striped Sprite Test", TILE_ATTR(cursel == 3 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);        
    VDP_drawTextBG(APLAN, "Lag Test", TILE_ATTR(cursel == 4 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    VDP_drawTextBG(APLAN, "Scroll Test", TILE_ATTR(cursel == 5 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
		VDP_drawTextBG(APLAN, "Grid Scroll Test", TILE_ATTR(cursel == 6 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    VDP_drawTextBG(APLAN, "Horizontal Stripes", TILE_ATTR(cursel == 7 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    VDP_drawTextBG(APLAN, "Checkerboard", TILE_ATTR(cursel == 8 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    VDP_drawTextBG(APLAN, "Sound Test", TILE_ATTR(cursel == 9 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    VDP_drawTextBG(APLAN, "Help", TILE_ATTR(cursel == 10 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    VDP_drawTextBG(APLAN, "Credits", TILE_ATTR(cursel == 11 ? PAL1 : PAL0, 0, 0, 0), 5, ++pos);
    
    buttons = JOY_readJoypad(JOY_1);
    pressedButtons = buttons & ~oldButtons;
    oldButtons = buttons;
		
    if (pressedButtons & BUTTON_DOWN)
    {
      cursel ++;
      if(cursel > pos - 6)
        cursel = 1;
    }

    if (pressedButtons & BUTTON_UP)
    {
      cursel --;
      if(cursel < 1)
        cursel = pos - 6;
    }

    if (pressedButtons & BUTTON_A)
    { 
      VDP_clearTileMapRect(APLAN, 0, 0, 320/8, 224/8);
      VDP_clearTileMapRect(BPLAN, 0, 0, 320/8, 224/8);      
      switch(cursel)
      {
        case 1:
          TestPatternMenu();
          break;                
        case 2:
          DropShadowTest();
          break;
        case 3:
          StripedSpriteTest();
          break;
        case 4:
          LagTest();   
          break;
        case 5:
          HScrollTest();   
          break;
				case 6:
          VScrollTest();   
          break;
        case 7:
          DrawStripes();   
          break;
        case 8: 
          DrawCheckBoard();                                   
          break;  
        case 9: 
          SoundTest();                                   
          break;  
        case 10: 
          DrawHelp(HELP_GENERAL);                                   
          break;
        case 11: 
          DrawCredits();                                   
          break;
      }
      VDP_clearTileMapRect(BPLAN, 0, 0, 320/8, 224/8);
      VDP_clearTileMapRect(APLAN, 0, 0, 320/8, 224/8);

      VDP_resetScreen();
      reload = 1;
    }
    VDP_waitVSync();
  }

  return 0;
} 

void TestPatternMenu() 
{ 
  u16 cursel = 1, pos, reload = 1;
  u16 buttons, oldButtons = 0xffff, pressedButtons;
  u16 ind = 0, size = 0, done = 0;
    
  while(!done)
  {
    if(reload)
    {
      VDP_setPalette(PAL2, back_pal); 
      VDP_setPalette(PAL3, gillian_pal); 

      ind = TILE_USERINDEX; 
      size = sizeof(back_tiles) / 32; 
      VDP_loadTileData(back_tiles, ind, size, USE_DMA); 
      ind += size;
      size = sizeof(gillian_tiles) / 32; 
      VDP_loadTileData(gillian_tiles, ind, size, USE_DMA); 

      VDP_setMyTileMapRect(BPLAN, back_map, TILE_USERINDEX, 0, 0, 320/8, 224/8);        
      VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL3, 0, 0, 0) + ind, 200/8, 80/8, 72/8, 112/8);

      reload = 0;
    }        
            
    pos = 6;
    VDP_drawTextBG(APLAN, "Pluge", TILE_ATTR(cursel == 1 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    VDP_drawTextBG(APLAN, "Color Bars", TILE_ATTR(cursel == 2 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    VDP_drawTextBG(APLAN, "Color Bars with Gray Reference", TILE_ATTR(cursel == 3 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    VDP_drawTextBG(APLAN, "Grid", TILE_ATTR(cursel == 4 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    VDP_drawTextBG(APLAN, "Linearity", TILE_ATTR(cursel == 5 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    VDP_drawTextBG(APLAN, "Gray Ramp", TILE_ATTR(cursel == 6 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);        
    VDP_drawTextBG(APLAN, "White Screen", TILE_ATTR(cursel == 7 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);                
    VDP_drawTextBG(APLAN, "Back to Main Menu", TILE_ATTR(cursel == 8 ? PAL1 : PAL0, 0, 0, 0), 5, ++pos);
    
    buttons = JOY_readJoypad(JOY_1);
    pressedButtons = buttons & ~oldButtons;
    oldButtons = buttons;

    if (pressedButtons & BUTTON_DOWN)
    {
      cursel ++;
      if(cursel > 8)
        cursel = 1;
    }

    if (pressedButtons & BUTTON_UP)
    {
      cursel --;
      if(cursel < 1)
        cursel = 8;
    }

    if (pressedButtons & BUTTON_B)
      done = 1;            
    
    if (pressedButtons & BUTTON_A)
    {            
      VDP_clearTileMapRect(APLAN, 0, 0, 320/8, 224/8);
      VDP_clearTileMapRect(BPLAN, 0, 0, 320/8, 224/8);      
      switch(cursel)
      {
        case 1:
          DrawPluge();
          break;
        case 2:
          DrawColorBars();
          break;
        case 3:
          Draw601ColorBars();
          break;
        case 4:
          DrawGrid();                    
          break;
        case 5:
          DrawLinearity();
          break;
        case 6:
          DrawGrayRamp();
          break;
        case 7:
          DrawWhiteScreen();
          break;
        case 8: 
          done = 1;
          break;
      }
      VDP_clearTileMapRect(BPLAN, 0, 0, 320/8, 224/8);
      VDP_clearTileMapRect(APLAN, 0, 0, 320/8, 224/8);

      VDP_resetScreen();
      reload = 1;
    }

    VDP_waitVSync();
  }

  return;
} 

void DrawIntro()
{ 
  VDP_setPalette(PAL0, palette_black);
  VDP_drawTextBG(APLAN, "KORDAMP PRESENTS", TILE_ATTR(PAL0, 0, 0, 0), 12, 12); 
  VDP_fadeIn(0, 15, palette_grey, 60, 1);  
  VDP_waitFadeCompletion();
  VDP_fadeOutAll(40, 0);
  VDP_resetScreen();
}

void DrawCredits()
{
  u16 ind = 0, size = 0, exit = 0, pos = 6;
  u16 buttons, oldButtons = 0xffff, pressedButtons;

  VDP_setPalette(PAL0, palette_grey);
  VDP_setPalette(PAL1, palette_green); 
  VDP_setPalette(PAL2, back_pal);
  VDP_setPalette(PAL3, bw_pal);    
  
  ind = TILE_USERINDEX; 
  size = sizeof(back_tiles) / 32; 
  VDP_loadTileData(back_tiles, ind, size, USE_DMA); 
  
  VDP_setMyTileMapRect(BPLAN, back_map, TILE_USERINDEX, 0, 0, 320/8, 224/8);      
  
  VDP_drawTextBG(APLAN, "Code and Patterns:", TILE_ATTR(PAL1, 0, 0, 0), 4, pos++);
  VDP_drawTextBG(APLAN, "Artemio Urbina", TILE_ATTR(PAL0, 0, 0, 0), 5, pos++);  
  VDP_drawTextBG(APLAN, "Menu Pixel Art:", TILE_ATTR(PAL1, 0, 0, 0), 4, pos++);
  VDP_drawTextBG(APLAN, "Asher", TILE_ATTR(PAL0, 0, 0, 0), 5, pos++);
  VDP_drawTextBG(APLAN, "SDK:", TILE_ATTR(PAL1, 0, 0, 0), 4, pos++);
  VDP_drawTextBG(APLAN, "http://code.google.com/p/sgdk/", TILE_ATTR(PAL0, 0, 0, 0), 5, pos++);
  VDP_drawTextBG(APLAN, "SDK Consultor:", TILE_ATTR(PAL1, 0, 0, 0), 4, pos++);
  VDP_drawTextBG(APLAN, "Stef", TILE_ATTR(PAL0, 0, 0, 0), 5, pos++);
#ifdef SEGACD
	VDP_drawTextBG(APLAN, "SEGA CD Loader by:", TILE_ATTR(PAL1, 0, 0, 0), 4, pos++);
  VDP_drawTextBG(APLAN, "Luke Usher/SoullessSentinel", TILE_ATTR(PAL0, 0, 0, 0), 5, pos++);
	VDP_drawTextBG(APLAN, "SEGA CD Consultors:", TILE_ATTR(PAL1, 0, 0, 0), 4, pos++);
  VDP_drawTextBG(APLAN, "Chilly Willy & TascoDLX", TILE_ATTR(PAL0, 0, 0, 0), 5, pos++);
#endif
	VDP_drawTextBG(APLAN, "Advisor:", TILE_ATTR(PAL1, 0, 0, 0), 4, pos++);
  VDP_drawTextBG(APLAN, "Fudoh", TILE_ATTR(PAL0, 0, 0, 0), 5, pos++);
  VDP_drawTextBG(APLAN, "Info on using this test suite:", TILE_ATTR(PAL1, 0, 0, 0), 4, pos++);
  VDP_drawTextBG(APLAN, "http://junkerhq.net/xrgb", TILE_ATTR(PAL0, 0, 0, 0), 5, pos++);

  VDP_drawTextBG(APLAN, "Ver. 1.07", TILE_ATTR(PAL0, 0, 0, 0), 26, 22);
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


