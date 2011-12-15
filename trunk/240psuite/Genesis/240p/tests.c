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
#include "myvdp.h"
#include "res.h"
#include "tests.h"

void DrawCheckBoard()
{
  char cntstr[4];
  u16 ind, size, count = 0, docounter = 0;
  u16 field = 1, alternate = 0, exit = 0;
  u16 buttons, oldButtons = 0xffff, pressedButtons;

  VDP_setPalette(PAL1, bw_pal);

  ind = TILE_USERINDEX; 
  size = sizeof(check_tile) / 32; 
  VDP_loadTileData(check_tile, ind, size, USE_DMA); 
  ind += size;
  size = sizeof(check_tile_inv) / 32; 
  VDP_loadTileData(check_tile_inv, ind, size, USE_DMA); 

  VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320/8, 224/8); 
  while(!exit)
  {
    if(alternate)
    {
      if(field == 0)
      {
        VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320/8, 224/8);  
        field = 1;
      }
      else
      {
        VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + ind, 0, 0, 320/8, 224/8); 
        field = 0;
      }
    }
      
    if(docounter)
    {
      count ++;
      if(count > 59)
        count = 0;

      intToStr(count, cntstr, 2);
      VDP_drawText("Frame:", 2, 25);
      VDP_drawText(cntstr, 8, 25);
    }

    buttons = JOY_readJoypad(JOY_1);
    pressedButtons = buttons & ~oldButtons;
    oldButtons = buttons;

     if (pressedButtons & BUTTON_A)
      alternate = ~alternate;
  
    if (pressedButtons & BUTTON_B && !alternate)
    {
      if(field == 0)
      {
        VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320/8, 224/8); 
        field = 1;
      }
      else
      {
        VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + ind, 0, 0, 320/8, 224/8); 
        field = 0;
      }
    }   

    if (pressedButtons & BUTTON_C)
      docounter = ~docounter;
  
    if (pressedButtons & BUTTON_START)
      exit = 1;

    VDP_waitVSync();
  }
}


void DrawStripes()
{
  char cntstr[4];
  u16 hor1, hor2, ver1, ver2, size, count = 0, docounter = 0;
  u16 field = 1, alternate = 0, exit = 0, vertical = 0, redraw = 0;
  u16 buttons, oldButtons = 0xffff, pressedButtons;

  VDP_setPalette(PAL1, bw_pal);

  hor1 = TILE_USERINDEX; 
  size = sizeof(bw_tile) / 32; 
  VDP_loadTileData(bw_tile, hor1, size, USE_DMA); 
  hor2 = hor1 + size;
  size = sizeof(wb_tile) / 32; 
  VDP_loadTileData(wb_tile, hor2, size, USE_DMA); 
  ver1 = hor2 + size;
  size = sizeof(vstripes_tiles) / 32; 
  VDP_loadTileData(vstripes_tiles, ver1, size, USE_DMA); 
  ver2 = ver1 + size;
  size = sizeof(vstripesneg_tiles) / 32; 
  VDP_loadTileData(vstripesneg_tiles, ver2, size, USE_DMA); 

  VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320/8, 224/8); 
  while(!exit)
  {
    if(alternate || redraw)
    {
      if(field == 0)
      {
        if(vertical)
          VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + ver1, 0, 0, 320/8, 224/8);  
        else
          VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + hor1, 0, 0, 320/8, 224/8);  
        field = 1;
      }
      else
      {
        if(vertical)
          VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + ver2, 0, 0, 320/8, 224/8); 
        else
          VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + hor2, 0, 0, 320/8, 224/8); 
        field = 0;
      }
      redraw = 0;
    }
  
    if(docounter)
    {
      count ++;
      if(count > 59)
        count = 0;

      intToStr(count, cntstr, 2);
      VDP_drawText("Frame:", 2, 25);
      VDP_drawText(cntstr, 8, 25);
    }

    buttons = JOY_readJoypad(JOY_1);
    pressedButtons = buttons & ~oldButtons;
    oldButtons = buttons;

    if (pressedButtons & BUTTON_A)
      alternate = ~alternate;

    if (pressedButtons & BUTTON_B && !alternate)
    {
      if(field == 0)
      {
        if(vertical)
          VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + ver1, 0, 0, 320/8, 224/8); 
        else
          VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + hor1, 0, 0, 320/8, 224/8); 
        field = 1;
      }
      else
      {
        if(vertical)
          VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + ver2, 0, 0, 320/8, 224/8); 
        else
          VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + hor2, 0, 0, 320/8, 224/8);  
        field = 0;
      }
    }   

    if (pressedButtons & BUTTON_UP)
    {
      vertical = ~vertical;
      redraw = 1;
    }

    if (pressedButtons & BUTTON_C)
      docounter = ~docounter;

    if (pressedButtons & BUTTON_START)
      exit = 1;

    VDP_waitVSync();
  }
}

void DropShadowTest()
{
  u16 sonic_water = 0, sonic_floor = 0, frame = 0, direction = 0;                         
  u16 size, ind, back = 0, changeback = 1, invert = 0, sprite = 0, redraw = 0;
  u16 field = 1, x = 0, y = 0, exit = 0, text = 0, shadowpos = 0, buzzpos = 0, buzzshadowpos = 0, waterfall = 0;
  u16 buttons, pressedButtons, oldButtons = 0xffff;

  VDP_setPalette(PAL0, palette_green);
  VDP_setPalette(PAL1, motoko_pal);
  VDP_setPalette(PAL2, wb_pal);    
  VDP_setPalette(PAL3, buzz_pal);    

  shadowpos = TILE_USERINDEX;
  size = sizeof(shadow_tiles) / 32; 
  VDP_loadTileData(shadow_tiles, shadowpos, size, USE_DMA);

  buzzpos = shadowpos + size;
  size = sizeof(buzzShadow_tiles) / 32; 
  VDP_loadTileData(buzz_tiles, buzzpos, size, USE_DMA); 

  buzzshadowpos = buzzpos + size;
  size = sizeof(buzzShadow_tiles) / 32; 
  VDP_loadTileData(buzzShadow_tiles, buzzshadowpos, size, USE_DMA); 

  waterfall = buzzshadowpos + size;
  size = sizeof(waterfall_tiles) / 32; 
  VDP_loadTileData(waterfall_tiles, waterfall, size, USE_DMA);

  ind = waterfall + size;  
  
  sprite = random() % 2;         
  
  VDP_setSprite(0, 12, 12, SPRITE_SIZE(4, 4), TILE_ATTR(PAL3, 0, 0, 0) + buzzpos, 1);                               
  VDP_setSprite(1, 32, 32, SPRITE_SIZE(4, 4), TILE_ATTR(PAL2, 0, 0, 0) + buzzshadowpos, 2);
  VDP_setSprite(2, 0, 0, SPRITE_SIZE(4, 4), TILE_ATTR(PAL2, 0, 0, 0) + shadowpos, 3);                     
  VDP_setSprite(3, 320, 224, SPRITE_SIZE(4, 4), TILE_ATTR(PAL1, 0, 0, 0) + waterfall, 4);
  VDP_setSprite(4, 320, 224, SPRITE_SIZE(4, 4), TILE_ATTR(PAL1, 0, 0, 0) + waterfall, 0);                                

  if(sprite == 1)
  {   
    x = 32;
    y = 32;

    VDP_setSpritePosition(2, 320, 224);    
  }
  else
  {                
    VDP_setSpritePosition(0, 320, 224);
    VDP_setSpritePosition(1, 320, 224);
  }           
    
  while(!exit)
  {        
    if(text)
    {
        text --;
        if(text == 0)
            redraw = 1;
    }       

    if(changeback || redraw)
    {                             
      if(redraw)
        VDP_clearTileMapRect(APLAN, 0, 0, 320/8, 224/8);

      if(back != 1)
        VDP_setPalette(PAL0, palette_green);

      if(back == 2)
      {
        VDP_setHorizontalScroll(BPLAN, 0, 0);
        VDP_setHorizontalScroll(APLAN, 0, 0);
        VDP_setSpritePosition(3, 320, 224);
        VDP_setSpritePosition(4, 320, 224);
        VDP_updateSprites();
      } 

      switch(back)
      {
        case 0:
          if(!redraw)
          {
            size = sizeof(motoko_tiles) / 32; 
            VDP_loadTileData(motoko_tiles, ind, size, USE_DMA); 
            VDP_setPalette(PAL1, motoko_pal);
          }
          VDP_setMyTileMapRect(BPLAN, motoko_map, ind, 0, 0, 320/8, 224/8);                            
          break;
        case 1:       
          if(!redraw)
          {             
            VDP_setPalette(PAL0, sonicback_pal);
            VDP_setPalette(PAL1, sonicwater_pal);   
               
            size = sizeof(sonicback_tiles) / 32; 
            VDP_loadTileData(sonicback_tiles, ind, size, USE_DMA);
  
            sonic_water = ind + size;
            size = sizeof(sonicwater_tiles) / 32;           
            VDP_loadTileData(sonicwater_tiles, sonic_water, size, USE_DMA);
  
            sonic_floor = sonic_water + size;
            size = sizeof(sonicfloor_tiles) / 32; 
            VDP_loadTileData(sonicfloor_tiles, sonic_floor, size, USE_DMA);
          }
            
          if(!redraw)
          {
            VDP_setMyTileMapRect(BPLAN, sonicback_map, ind, 0, 0, 256/8, 152/8);                            
            VDP_setMyTileMapRect(BPLAN, sonicback_map, ind, 256/8, 0, 256/8, 152/8);                                   
            VDP_setMyTileMapRect(BPLAN, sonicwater_map, sonic_water, 0, 152/8, 256/8, 48/8);
            VDP_setMyTileMapRect(BPLAN, sonicwater_map, sonic_water, 256/8, 152/8, 256/8, 48/8);
          }
          VDP_setMyTileMapRect(APLAN, sonicfloor_map, sonic_floor, 0, 96/8, 256/8, 128/8);                            
          VDP_setMyTileMapRect(APLAN, sonicfloor_map, sonic_floor, 256/8, 96/8, 256/8, 128/8);                                      
          break;
        case 2:
          if(!redraw)
          {            
            size = sizeof(check_tile) / 32; 
            VDP_loadTileData(check_tile, ind, size, USE_DMA);
            VDP_setPalette(PAL1, bw_pal);
          }
          VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + ind, 0, 0, 320/8, 224/8); 
          break;
        case 3:
          if(!redraw)
          {
            size = sizeof(bw_tile) / 32; 
            VDP_loadTileData(bw_tile, ind, size, USE_DMA);
            VDP_setPalette(PAL1, bw_pal);
          }
          VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + ind, 0, 0, 320/8, 224/8); 
          break;
      }
      changeback = 0;            
      redraw = 0;
    }

    if(sprite == 1)
      VDP_setSpritePosition(0, x - 20, y - 20);

    if(field == invert)
    {      
      VDP_setSpritePosition(sprite == 1 ? 1 : 2, x, y);      

      field = !field;
    }
    else
    {            
      VDP_setSpritePosition(sprite == 1 ? 1 : 2, 320, 224);

      field = !field;
    }
   
    buttons = JOY_readJoypad(JOY_1);
    pressedButtons = buttons & ~oldButtons;
    oldButtons = buttons;

    if (buttons & BUTTON_UP)
    {
      if(y > 0)
        y --;   
    }   

    if (buttons & BUTTON_DOWN)
    {
      if(y < 192)
        y ++;   
    }   

    if (buttons & BUTTON_LEFT)
    {
      if(x > 0)
        x --;   
      if(direction == 1)
      {
        direction = 0;        
        VDP_setSpriteAttr(0, TILE_ATTR(PAL3, 0, 0, direction) + buzzpos);
        VDP_setSpriteAttr(1, TILE_ATTR(PAL2, 0, 0, direction) + buzzshadowpos);          
      }
    }   

    if (buttons & BUTTON_RIGHT)
    {
      if(x < 288)
        x ++;   
      if(direction == 0)
      {
        direction = 1;
        VDP_setSpriteAttr(0, TILE_ATTR(PAL3, 0, 0, direction) + buzzpos);
        VDP_setSpriteAttr(1, TILE_ATTR(PAL2, 0, 0, direction) + buzzshadowpos);          
      }
    }   

    if (pressedButtons & BUTTON_START)
      exit = 1;

    if (pressedButtons & BUTTON_A)
    {
      invert = !invert;
      
      if(invert)
        VDP_drawText("Shadow on odd frames ", 19, 0);
      else
        VDP_drawText("Shadow on even frames", 19, 0);
      
      text = 60;            
    }

    if(back == 1)
    {
      VDP_setSpritePosition(3, x*-2 + 79, 120);
      VDP_setSpritePosition(4, x*-2 + 335, 120);
      VDP_setHorizontalScroll(BPLAN, 0, x*-2);
      VDP_setHorizontalScroll(APLAN, 0, x*-4);      

      switch(frame)
      {
        case 30:
          VDP_setPalette(PAL1, sonicwater3_pal);
          break;
        case 60:
          VDP_setPalette(PAL1, sonicwater2_pal);
          break;
        case 90:
          VDP_setPalette(PAL1, sonicwater_pal);  
          break;        
      }
  
      frame ++;
      if(frame > 90)
        frame = 1;        
    }

    if (pressedButtons & BUTTON_B)
    {
      if(back < 3)
        back ++;
      else
        back = 0;
            
      changeback = 1;
      VDP_clearTileMapRect(BPLAN, 0, 0, 320/8, 224/8);        
      VDP_clearTileMapRect(APLAN, 0, 0, 320/8, 224/8);        
    }

    if (pressedButtons & BUTTON_C)    
    {
      if(!sprite)      
        VDP_setSpritePosition(2, 320, 224);    
      else
      {                
        VDP_setSpritePosition(0, 320, 224);
        VDP_setSpritePosition(1, 320, 224);
      }
      
      sprite = !sprite;
    }

    VDP_updateSprites();
    VDP_waitVSync();
  }

  VDP_resetSprites();
  VDP_updateSprites();
  VDP_setHorizontalScroll(BPLAN, 0, 0);
  VDP_setHorizontalScroll(APLAN, 0, 0);
}

void StripedSpriteTest()
{
  u16 sonic_water = 0, sonic_floor = 0, sprite, frame = 0, waterfall = 0;     
  u16 size, ind, back = 0, changeback = 1;
  u16 x = 0, y = 0, exit = 0;
  u16 buttons, pressedButtons, oldButtons = 0xffff;

  VDP_setPalette(PAL1, motoko_pal);
  VDP_setPalette(PAL3, wb_pal);

  sprite = TILE_USERINDEX; 
  size = sizeof(striped_tiles) / 32; 
  VDP_loadTileData(striped_tiles, sprite, size, USE_DMA);
  VDP_setSprite(0, x, y, SPRITE_SIZE(4, 4), TILE_ATTR(PAL3, 0, 0, 0) + sprite, 1);    

  waterfall = sprite + size;
  size = sizeof(waterfall_tiles) / 32; 
  VDP_loadTileData(waterfall_tiles, waterfall, size, USE_DMA);
    
  VDP_setSprite(1, 320, 224, SPRITE_SIZE(4, 4), TILE_ATTR(PAL1, 0, 0, 0) + waterfall, 2);
  VDP_setSprite(2, 320, 224, SPRITE_SIZE(4, 4), TILE_ATTR(PAL1, 0, 0, 0) + waterfall, 0);      

  ind = waterfall + size;  
    
  while(!exit)
  {                
    if(changeback)
    {      
      changeback = 0;   
      VDP_clearTileMapRect(APLAN, 0, 0, 320/8, 224/8);        
      VDP_clearTileMapRect(BPLAN, 0, 0, 320/8, 224/8); 
      if(back != 1)
      {
        VDP_setHorizontalScroll(BPLAN, 0, 0);
        VDP_setHorizontalScroll(APLAN, 0, 0);
        VDP_setSpritePosition(1, 320, 224);
        VDP_setSpritePosition(2, 320, 224);
        VDP_updateSprites();
      } 
      switch(back)
      {
        case 0:
          size = sizeof(motoko_tiles) / 32; 
          VDP_loadTileData(motoko_tiles, ind, size, USE_DMA);
          VDP_setPalette(PAL1, motoko_pal);
          VDP_setMyTileMapRect(BPLAN, motoko_map, ind, 0, 0, 320/8, 224/8);                  
          break;
        case 1:                    
          VDP_setPalette(PAL0, sonicback_pal);
          VDP_setPalette(PAL1, sonicwater_pal);                
          size = sizeof(sonicback_tiles) / 32; 
          VDP_loadTileData(sonicback_tiles, ind, size, USE_DMA);
          sonic_water = ind + size;
          size = sizeof(sonicwater_tiles) / 32;           
          VDP_loadTileData(sonicwater_tiles, sonic_water, size, USE_DMA);  
          sonic_floor = sonic_water + size;
          size = sizeof(sonicfloor_tiles) / 32; 
          VDP_loadTileData(sonicfloor_tiles, sonic_floor, size, USE_DMA);
          VDP_setMyTileMapRect(BPLAN, sonicback_map, ind, 0, 0, 256/8, 152/8);                            
          VDP_setMyTileMapRect(BPLAN, sonicback_map, ind, 256/8, 0, 256/8, 152/8);                        
          VDP_setMyTileMapRect(BPLAN, sonicwater_map, sonic_water, 0, 152/8, 256/8, 48/8);
          VDP_setMyTileMapRect(BPLAN, sonicwater_map, sonic_water, 256/8, 152/8, 256/8, 48/8);
          VDP_setMyTileMapRect(APLAN, sonicfloor_map, sonic_floor, 0, 96/8, 256/8, 128/8);                            
          VDP_setMyTileMapRect(APLAN, sonicfloor_map, sonic_floor, 256/8, 96/8, 256/8, 128/8);                                  
          break;
        case 2:          
          size = sizeof(check_tile) / 32; 
          VDP_loadTileData(check_tile, ind, size, USE_DMA);
          VDP_setPalette(PAL1, bw_pal);
          VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + ind, 0, 0, 320/8, 224/8); 
          break;
        case 3:
          size = sizeof(bw_tile) / 32; 
          VDP_loadTileData(bw_tile, ind, size, USE_DMA); 
          VDP_setPalette(PAL1, bw_pal);
          VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + ind, 0, 0, 320/8, 224/8); 
          break;
      }
    }
      
    VDP_setSpritePosition(0, x, y);
    
    buttons = JOY_readJoypad(JOY_1);
    pressedButtons = buttons & ~oldButtons;
    oldButtons = buttons;

    if (buttons & BUTTON_UP)
    {
      if(y > 0)
        y --;   
    }   

    if (buttons & BUTTON_DOWN)
    {
      if(y < 192)
        y ++;   
    }   

    if (buttons & BUTTON_LEFT)
    {
      if(x > 0)
        x --;   
    }   

    if (buttons & BUTTON_RIGHT)
    {
      if(x < 288)
        x ++;   
    }   

    if(back == 1)
    {
      VDP_setSpritePosition(1, x*-2 + 79, 120);
      VDP_setSpritePosition(2, x*-2 + 335, 120);
      VDP_setHorizontalScroll(BPLAN, 0, x*-2);
      VDP_setHorizontalScroll(APLAN, 0, x*-4);  
    
      switch(frame)
      {
        case 30:
          VDP_setPalette(PAL1, sonicwater3_pal);
          break;
        case 60:
          VDP_setPalette(PAL1, sonicwater2_pal);
          break;
        case 90:
          VDP_setPalette(PAL1, sonicwater_pal);  
          break;
      }
  
      frame ++;
      if(frame > 90)
        frame = 1;    
    }

    if (pressedButtons & BUTTON_START)
      exit = 1;

    if (pressedButtons & BUTTON_A)
    {
      if(back > 0)
        back --;
      else
        back = 3;
      
      changeback = 1;
    }

    if (pressedButtons & BUTTON_B)
    {
      if(back < 3)
        back ++;
      else
        back = 0;
      
      changeback = 1;      
    }        

    VDP_updateSprites();
    VDP_waitVSync();
  }

  VDP_resetSprites();
  VDP_updateSprites();
  VDP_setHorizontalScroll(BPLAN, 0, 0);
  VDP_setHorizontalScroll(APLAN, 0, 0);
}



void LagTest()
{
  char str[10];
  u16 pal = PAL0, change = 1;
  s16 speed = 1, vary = 0;
  u16 size, ind;
  u16 x = 0, y = 0, x2 = 0, y2 = 0, exit = 0, variation = 1, draw = 1;
  u16 buttons, pressedButtons, oldButtons = 0xffff;
  u16 pos = 0, view = 0, audio  = 0, drawoffset = 0;
  u32 len = 0;    

  s16 clicks[10];

  VDP_setPalette(PAL0, palette_grey);
  VDP_setPalette(PAL1, palette_red);
  VDP_setPalette(PAL2, bw_pal);
  VDP_setPalette(PAL3, palette_green);

  ind = TILE_USERINDEX; 
  size = sizeof(shadow_tiles) / 32; 
  VDP_loadTileData(shadow_tiles, ind, size, USE_DMA); 
  ind += size;
  size = sizeof(lag_tiles) / 32; 
  VDP_loadTileData(lag_tiles, ind, size, USE_DMA); 

  x = 144;
  y = 60;

  x2 = 108;
  y2 = 96;

  VDP_setSprite(0, x, 96, SPRITE_SIZE(4, 4), TILE_ATTR(PAL2, 0, 0, 0) + ind, 1);         
  VDP_setSprite(1, x, y, SPRITE_SIZE(4, 4), TILE_ATTR(PAL2, 0, 0, 0) + ind, 2);         
  VDP_setSprite(2, x2, y2, SPRITE_SIZE(4, 4), TILE_ATTR(PAL2, 0, 0, 0) + ind, 0);           
  
  VDP_drawTextBG(APLAN, "Press the \"A\" button when the sprite", TILE_ATTR(PAL3, 0, 0, 0), 2, 21);
  VDP_drawTextBG(APLAN, "is aligned. A negative value means", TILE_ATTR(PAL3, 0, 0, 0), 2, 22);
  VDP_drawTextBG(APLAN, "you pressed \"A\" before they intersect.", TILE_ATTR(PAL3, 0, 0, 0), 2, 23);
  VDP_drawTextBG(APLAN, "\"B\" button toggles horz/vert", TILE_ATTR(PAL3, 0, 0, 0), 2, 24);
  VDP_drawTextBG(APLAN, "\"C\" button toggles audio", TILE_ATTR(PAL3, 0, 0, 0), 2, 25);
  VDP_drawTextBG(APLAN, "DOWN toggles random/rhythmic", TILE_ATTR(PAL3, 0, 0, 0), 2, 26);

  len = sizeof(beep); 
  while(!exit)    
  {                        
    buttons = JOY_readJoypad(JOY_1);
    pressedButtons = buttons & ~oldButtons;
    oldButtons = buttons;
    
    if (pressedButtons & BUTTON_A)
    {            
      if(change)
      {                
        clicks[pos] = (y - 96) * speed;
        drawoffset = 1;
        if(clicks[pos] >= 0)
          change = 0;
      }           
    }

    //play audio 1 frame before
    //if((y == 95 && speed == 1 ) || (y == 97 && speed == -1))
    //    startPlay_PCM(beep, len, 16000, AUDIO_PAN_CENTER);            

    if(y == 96) // half the screen?        
    {
      if(audio)                
      {
        if(speed == 1)
          startPlay_PCM(beep, len, (u8)16000, AUDIO_PAN_LEFT, 0);            
        else
          startPlay_PCM(beep, len, (u8)16000, AUDIO_PAN_RIGHT, 0);            
      }        
    }    

    if (pressedButtons & BUTTON_B)
    {
      view ++;
      if(view > 2)
        view = 0;            
    }    
  
    if (pressedButtons & BUTTON_C)        
    {
      audio = !audio;    
      draw = 1;
    }

    if (pressedButtons & BUTTON_DOWN)
    {
      variation = !variation;
      if(!variation)
        vary = 0;
      draw = 1;
    }

    if (pressedButtons & BUTTON_START)
      exit = 1;

    if(drawoffset)
    {
      u16 ppos = 0;

      pal = PAL0;
      intToStr(clicks[pos], str, 1);

      if(clicks[pos] == 0)
        pal = PAL3;
      if(clicks[pos] < 0)
        pal = PAL1;

      ppos = pos + 1;                    
      intToStr(ppos, str, 1);
      VDP_drawTextBG(APLAN, "Offset", TILE_ATTR(PAL0, 0, 0, 0), 2, pos);
      VDP_drawTextBG(APLAN, str, TILE_ATTR(PAL0, 0, 0, 0), 9, pos);
      VDP_drawTextBG(APLAN, ":", TILE_ATTR(PAL0, 0, 0, 0), pos == 9 ? 11 : 10, pos);
      intToStr(clicks[pos], str, 1);
      VDP_drawTextBG(APLAN, str, TILE_ATTR(pal, 0, 0, 0), pos == 9 ? 13 : 12, pos);
      ppos = strlen(str);
      if(clicks[pos] == 1)
        VDP_drawTextBG(APLAN, " frame    ", TILE_ATTR(pal, 0, 0, 0), pos == 9 ? 13 : 12 + ppos, pos);
      else
        VDP_drawTextBG(APLAN, " frames   ", TILE_ATTR(pal, 0, 0, 0), pos == 9 ? 13 : 12 + ppos, pos);

      if(clicks[pos] >= 0)
        pos++;

      if(pos > 9)
        exit = 1;
      drawoffset = 0;
    }       

    if(draw)
    {      
      VDP_drawTextBG(APLAN, "Audio:", TILE_ATTR(PAL0, 0, 0, 0), 24, 0);
      if(audio)
        VDP_drawTextBG(APLAN, "on ", TILE_ATTR(PAL0, 0, 0, 0), 31, 0);
      else
        VDP_drawTextBG(APLAN, "off", TILE_ATTR(PAL0, 0, 0, 0), 31, 0);
      VDP_drawTextBG(APLAN, "Timing:", TILE_ATTR(PAL0, 0, 0, 0), 24, 1);
      if(variation)
        VDP_drawTextBG(APLAN, "random  ", TILE_ATTR(PAL0, 0, 0, 0), 32, 1);
      else
        VDP_drawTextBG(APLAN, "rhythmic", TILE_ATTR(PAL0, 0, 0, 0), 32, 1);
      draw = 0;
    }
    
    if(y > 132 + vary)
    {
      speed = -1;
      change = 1;
      if(variation)
			{
				if(random() % 2)
					vary = random() % 7;
				else
					vary = -1 * random() % 7;
			}
    }
    
    if(y < 60 + vary)
    {
      speed = 1;
      change = 1;
      if(variation)
			{
				if(random() % 2)
					vary = random() % 7;
				else
					vary = -1 * random() % 7;
			}
    }

    y += speed;
    x2 += speed;

    if(y == 96) // Red on the spot
      VDP_setSpriteAttr(0, TILE_ATTR(PAL1, 0, 0, 0) + ind);
    if(y == 95 || y == 97) //Green one pixel before or after
      VDP_setSpriteAttr(0, TILE_ATTR(PAL3, 0, 0, 0) + ind);
    if(y == 98 || y == 94) //Back to white two pixels before or after
      VDP_setSpriteAttr(0, TILE_ATTR(PAL2, 0, 0, 0) + ind);    

    if(view == 0 || view == 2)
      VDP_setSpritePosition(1, x, y);      
    else
      VDP_setSpritePosition(1, 320, 224);      

    if(view == 1 || view == 2)
      VDP_setSpritePosition(2, x2, y2);
    else
      VDP_setSpritePosition(2, 320, 224);      

    VDP_updateSprites();        
    VDP_waitVSync();
  }   

  VDP_resetSprites();
  VDP_updateSprites();

  VDP_clearTileMapRect(APLAN, 0, 0, 320/8, 224/8);
 
  if(pos > 9)
  {
    fix32 totald = 0, cnt, tot;
    u16 total = 0, count = 0, c = 0;       

    VDP_setPalette(PAL2, back_pal); 

    size = sizeof(back_tiles) / 32; 
    VDP_loadTileData(back_tiles,  TILE_USERINDEX, size, USE_DMA); 
    
    VDP_setMyTileMapRect(BPLAN, back_map, TILE_USERINDEX, 0, 0, 320/8, 224/8);      
    exit = 0;

    for(c = 0; c < 10; c++)
    {
      if(clicks[c] !=  0xFF)
      {                
        intToStr(clicks[c], str, 1);

        pal = PAL0;
        if(clicks[c] == 0)
          pal = PAL3;
        if(clicks[c] < 0)
          pal = PAL1;
        if(clicks[c] >= 0)
        {
          total += clicks[c];
          count ++;
        }

        VDP_drawTextBG(APLAN, str, TILE_ATTR(pal, 0, 0, 0), 10, c + 7);
      }
    }   

    VDP_drawTextBG(APLAN, "+", TILE_ATTR(PAL1, 0, 0, 0), 8, 11);

    if(count > 0)
    {
      u16 h = 10;
      u16 v = 18;            

      VDP_drawTextBG(APLAN, "----", TILE_ATTR(PAL0, 0, 0, 0), h - 2, v++);

      cnt = intToFix32(count);
      tot = intToFix32(total);

      intToStr(total, str, 1);            
      VDP_drawTextBG(APLAN, str, TILE_ATTR(PAL0, 0, 0, 0), h, v);
      h += strlen(str);
      VDP_drawTextBG(APLAN, "/", TILE_ATTR(PAL1, 0, 0, 0), h++, v);

      intToStr(count, str, 1);
      VDP_drawTextBG(APLAN, str, TILE_ATTR(PAL0, 0, 0, 0), h, v);
      h += strlen(str);
      VDP_drawTextBG(APLAN, "=", TILE_ATTR(PAL1, 0, 0, 0), h++, v);
      
      totald = fix32Div(tot, cnt);
      fix32ToStr(totald, str);
      VDP_drawTextBG(APLAN, str, TILE_ATTR(PAL3, 0, 0, 0), h, v);
      h += strlen(str);
      h -= 2;
      VDP_drawTextBG(APLAN, " frames", TILE_ATTR(PAL0, 0, 0, 0), h, v++);
      VDP_drawTextBG(APLAN, "Keep in mind that a frame is", TILE_ATTR(PAL0, 0, 0, 0), 6, ++v);
      VDP_drawTextBG(APLAN, "around 16.68 milliseconds.", TILE_ATTR(PAL0, 0, 0, 0), 6, ++v);

      h = 14;
      v = 12;
      tot = FIX32(16.6834);
      cnt = fix32Mul(totald, tot);
      fix32ToStr(cnt, str);
      VDP_drawTextBG(APLAN, str, TILE_ATTR(PAL3, 0, 0, 0), h, v);
      h += strlen(str);
      //h -= 2;
      VDP_drawTextBG(APLAN, " milliseconds", TILE_ATTR(PAL0, 0, 0, 0), h, v);      

      if(total < 5)
        VDP_drawTextBG(APLAN, "EXCELLENT REFLEXES!", TILE_ATTR(PAL1, 0, 0, 0), 14, 15);
      if(total == 0)
        VDP_drawTextBG(APLAN, "INCREDIBLE REFLEXES!!", TILE_ATTR(PAL1, 0, 0, 0), 14, 15);
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
}


void HScrollTest()
{    
  u16 size, sonic_floor, sonic_water, waterfall;
  u16 exit = 0, frame = 1;
  u16 buttons, oldButtons = 0xffff, pressedButtons;
  int x = 0, speed = 1, acc = -1, pause = 0;
  
  VDP_setPalette(PAL0, sonicback_pal);  
  VDP_setPalette(PAL1, sonicwater_pal);  

  size = sizeof(sonicback_tiles) / 32; 
  VDP_loadTileData(sonicback_tiles, TILE_USERINDEX, size, USE_DMA);     

  sonic_water = TILE_USERINDEX + size;
  size = sizeof(sonicwater_tiles) / 32; 
  VDP_loadTileData(sonicwater_tiles, sonic_water, size, USE_DMA);     

  sonic_floor = sonic_water + size;
  size = sizeof(sonicfloor_tiles) / 32; 
  VDP_loadTileData(sonicfloor_tiles, sonic_floor, size, USE_DMA);     

  waterfall = sonic_floor + size;
  size = sizeof(waterfall_tiles) / 32; 
  VDP_loadTileData(waterfall_tiles, waterfall, size, USE_DMA);     

  VDP_setMyTileMapRect(BPLAN, sonicback_map, TILE_USERINDEX, 0, 0, 256/8, 152/8);                            
  VDP_setMyTileMapRect(BPLAN, sonicback_map, TILE_USERINDEX, 256/8, 0, 256/8, 152/8);                        

  VDP_setMyTileMapRect(BPLAN, sonicwater_map, sonic_water, 0, 152/8, 256/8, 48/8);
  VDP_setMyTileMapRect(BPLAN, sonicwater_map, sonic_water, 256/8, 152/8, 256/8, 48/8);  

  VDP_setMyTileMapRect(APLAN, sonicfloor_map, sonic_floor, 0, 96/8, 256/8, 128/8);
  VDP_setMyTileMapRect(APLAN, sonicfloor_map, sonic_floor, 256/8, 96/8, 256/8, 128/8);

  VDP_setSprite(0, 72, 120, SPRITE_SIZE(4, 4), TILE_ATTR(PAL1, 0, 0, 0) + waterfall, 1);
  VDP_setSprite(1, 328, 120, SPRITE_SIZE(4, 4), TILE_ATTR(PAL1, 0, 0, 0) + waterfall, 0);  
  while(!exit)
  {
    switch(frame)
    {
      case 30:
        VDP_setPalette(PAL1, sonicwater3_pal);
        break;
      case 60:
        VDP_setPalette(PAL1, sonicwater2_pal);
        break;
      case 90:
        VDP_setPalette(PAL1, sonicwater_pal);  
        break;
    }

    frame ++;
    if(frame > 90)
      frame = 1;
    
    buttons = JOY_readJoypad(JOY_1);
    pressedButtons = buttons & ~oldButtons;
    oldButtons = buttons;
        
    if (pressedButtons & BUTTON_START)
      exit = 1;

    if (pressedButtons & BUTTON_UP)
      speed++;

    if (pressedButtons & BUTTON_DOWN)
      speed--;

    if(speed > 20)        
      speed = 20;          
    
    if(speed < 0)        
      speed = 0;          

    if (pressedButtons & BUTTON_A)
      pause = !pause;

    if (pressedButtons & BUTTON_B)
      acc *= -1;

    if(!pause)
      x += acc*speed;

    if(x >= 512)
      x = x % 512;

    if(x <= -512)
      x = x % -512;

    VDP_setSpritePosition(0, x/2 + 79, 120);
    VDP_setSpritePosition(1, x/2 + 335, 120);
    VDP_updateSprites();
    VDP_setHorizontalScroll(APLAN, 0, x);        
    VDP_setHorizontalScroll(BPLAN, 0, x/2);        
    VDP_waitVSync();
  }
  VDP_setHorizontalScroll(APLAN, 0, 0);
  VDP_setHorizontalScroll(BPLAN, 0, 0);
  VDP_resetSprites();
  VDP_updateSprites();  
}

void VScrollTest()
{    
  u16 size;
  u16 exit = 0;
  u16 buttons, oldButtons = 0xffff, pressedButtons;
  int pos = 0, speed = 1, acc = -1, pause = 0, direction = 0;
  
  VDP_setPalette(PAL0, bw_pal);    

	size = sizeof(circles_grid_tiles) / 32; 
  VDP_loadTileData(circles_grid_tiles, TILE_USERINDEX, size, USE_DMA);   

  VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL0, 0, 0, 0) + TILE_USERINDEX, 0, 0, 512/8, 512/8); 
  
  while(!exit)
  {
    buttons = JOY_readJoypad(JOY_1);
    pressedButtons = buttons & ~oldButtons;
    oldButtons = buttons;
        
    if (pressedButtons & BUTTON_START)
      exit = 1;

    if (pressedButtons & BUTTON_UP)
      speed++;

    if (pressedButtons & BUTTON_DOWN)
      speed--;

    if(speed > 5)        
      speed = 5;          
    
    if(speed < 0)        
      speed = 0;          

    if (pressedButtons & BUTTON_A)
      pause = !pause;

    if (pressedButtons & BUTTON_B)
      acc *= -1;

		if (pressedButtons & BUTTON_C)
      direction = !direction;

    if(!pause)
      pos += acc*speed;

		if(pos >= 512)
      pos = pos % 512;

    if(pos <= -512)
      pos = pos % -512;
    
		if(direction)
    	VDP_setHorizontalScroll(APLAN, 0, pos);            
		else
			VDP_setVerticalScroll(APLAN, 0, pos);            
    VDP_waitVSync();
  }  
  VDP_setHorizontalScroll(APLAN, 0, 0);              
	VDP_setVerticalScroll(APLAN, 0, 0);    
}

void SoundTest()
{
  int sel = 1;
   u16 ind = 0, size = 0, exit = 0;
  u16 buttons, oldButtons = 0xffff, pressedButtons;
  u16  len = 0;

  len = sizeof(beep);
  VDP_setPalette(PAL0, palette_grey);
  VDP_setPalette(PAL1, palette_green); 
  VDP_setPalette(PAL2, back_pal);
  VDP_setPalette(PAL3, palette_red);  

  ind = TILE_USERINDEX; 
  size = sizeof(back_tiles) / 32; 
  VDP_loadTileData(back_tiles, ind, size, USE_DMA); 
  
  VDP_setMyTileMapRect(BPLAN, back_map, TILE_USERINDEX, 0, 0, 320/8, 224/8);        

  while(!exit)
  {
    buttons = JOY_readJoypad(JOY_1);
    pressedButtons = buttons & ~oldButtons;
    oldButtons = buttons;

    if (pressedButtons & BUTTON_START)
      exit = 1;

    if (pressedButtons & BUTTON_LEFT)
      sel --;

    if (pressedButtons & BUTTON_RIGHT)
      sel ++;

    if(sel > 2)
      sel = 0;

    if(sel < 0)
      sel = 2;

    if (pressedButtons & BUTTON_A)
    {
      if(sel == 0)
      {        
        stopPlay_PCM();
        startPlay_TFM(left);                
	      //YM2612_writeRegSafe(0, 0xb4, 0x80);
      }
      if(sel == 1)
      {
        stopPlay_PCM();
        startPlay_TFM(center);
      }
      if(sel == 2)
      {
        stopPlay_PCM();
        startPlay_TFM(right);        
	      //YM2612_writeRegSafe(0, 0xb4, 0x40);
      }
    }
          
    VDP_drawTextBG(APLAN, "Sound Test", TILE_ATTR(PAL0, 0, 0, 0), 14, 6);
    VDP_drawTextBG(APLAN, "Left Channel", TILE_ATTR(sel == 0 ? PAL3 : PAL0, 0, 0, 0), 5, 12);
    VDP_drawTextBG(APLAN, "Center Channel", TILE_ATTR(sel == 1 ? PAL3 : PAL0, 0, 0, 0), 14, 14);
    VDP_drawTextBG(APLAN, "Right Channel", TILE_ATTR(sel == 2 ? PAL3 : PAL0, 0, 0, 0), 22, 12);        

    VDP_drawTextBG(APLAN, "Space Standart track by Shiru", TILE_ATTR(PAL1, 0, 0, 0), 5, 22);                

    VDP_waitVSync();
  }  
  stopPlay_PCM();
}

void LEDZoneTest()
{
  u16 size, sprite0, sprite1, sprite2, sprite3, sprite4;
  u16 x = 160, y = 112, exit = 0, sprite = 0, change = 0;
  u16 buttons, pressedButtons, oldButtons = 0xffff;

  VDP_setPalette(PAL1, bw_pal);  

  size = sizeof(size0led_t) / 32; 

  sprite0 = TILE_USERINDEX;   
  VDP_loadTileData(size0led_t, sprite0, size, USE_DMA);
  sprite1 = sprite0 + size;  
  VDP_loadTileData(size1led_t, sprite1, size, USE_DMA);  
  sprite2 = sprite1 + size;  
  VDP_loadTileData(size2led_t, sprite2, size, USE_DMA);
  sprite3 = sprite2 + size;  
  VDP_loadTileData(size3led_t, sprite3, size, USE_DMA);
  sprite4 = sprite3 + size;  
  VDP_loadTileData(size4led_t, sprite4, size, USE_DMA);  

  VDP_setSprite(0, x, y, SPRITE_SIZE(1, 1), TILE_ATTR(PAL1, 0, 0, 0) + sprite0, 1);    

  VDP_clearTileMapRect(APLAN, 0, 0, 320/8, 224/8);            
  while(!exit)
  {                          
    VDP_setSpritePosition(0, x, y);
    
    buttons = JOY_readJoypad(JOY_1);
    pressedButtons = buttons & ~oldButtons;
    oldButtons = buttons;

    if (buttons & BUTTON_UP)
    {
      if(y > 0)
        y --;   
    }   

    if (buttons & BUTTON_DOWN)
    {
      if(y < 224 - size)
        y ++;   
    }   

    if (buttons & BUTTON_LEFT)
    {
      if(x > 0)
        x --;   
    }   

    if (buttons & BUTTON_RIGHT)
    {
      if(x < 320 - size)
        x ++;   
    }   

    if (pressedButtons & BUTTON_START)
      exit = 1;

    if (pressedButtons & BUTTON_A)
    {          
      if(sprite == 0)
        sprite = 4;
      else
        sprite --;
      change = 1;
    }

    if (pressedButtons & BUTTON_B)
    {     
      if(sprite == 4)
        sprite = 0;
      else
        sprite ++;
      change = 1;
    }        

    if(change)
    {
      switch(sprite)
      {
        case 0:
          VDP_setSprite(0, x, y, SPRITE_SIZE(1, 1), TILE_ATTR(PAL1, 0, 0, 0) + sprite0, 1);    
          size = 1;
          break;
        case 1:
          VDP_setSprite(0, x, y, SPRITE_SIZE(1, 1), TILE_ATTR(PAL1, 0, 0, 0) + sprite1, 1);    
          size = 2;
          break;
        case 2:
          VDP_setSprite(0, x, y, SPRITE_SIZE(1, 1), TILE_ATTR(PAL1, 0, 0, 0) + sprite2, 1);    
          size = 4;
          break;
        case 3:
          VDP_setSprite(0, x, y, SPRITE_SIZE(1, 1), TILE_ATTR(PAL1, 0, 0, 0) + sprite3, 1);    
          size = 6;
          break;
        case 4:
          VDP_setSprite(0, x, y, SPRITE_SIZE(1, 1), TILE_ATTR(PAL1, 0, 0, 0) + sprite4, 1);    
          size = 8;
          break;
      }
    }

    VDP_updateSprites();
    VDP_waitVSync();
  }

  VDP_resetSprites();
  VDP_updateSprites();    
}

