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
  VDP_loadTileData(check_tile, ind, size, 1); 
  ind += size;
  size = sizeof(check_tile_inv) / 32; 
  VDP_loadTileData(check_tile_inv, ind, size, 1); 

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
  u16 ind, size, count = 0, docounter = 0;
  u16 field = 1, alternate = 0, exit = 0;
  u16 buttons, oldButtons = 0xffff, pressedButtons;

  VDP_setPalette(PAL1, bw_pal);

  ind = TILE_USERINDEX; 
  size = sizeof(bw_tile) / 32; 
  VDP_loadTileData(bw_tile, ind, size, 1); 
  ind += size;
  size = sizeof(wb_tile) / 32; 
  VDP_loadTileData(wb_tile, ind, size, 1); 

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

void DropShadowTest()
{
  u16 size, ind, back = 0, changeback = 0, invert = 0, sprite = 0;
  u16 field = 1, x = 0, y = 0, exit = 0, text = 0, shadowpos = 0, buzzpos = 0, buzzshadowpos = 0;
  u16 buttons, pressedButtons, oldButtons = 0xffff;

  VDP_setPalette(PAL1, motoko_pal);
  VDP_setPalette(PAL2, wb_pal);    
  VDP_setPalette(PAL3, buzz_pal);    

  ind = TILE_USERINDEX; 
  size = sizeof(motoko_tiles) / 32; 
  VDP_loadTileData(motoko_tiles, ind, size, 1); 
  VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320/8, 224/8); 
  
  shadowpos = ind + size;
  size = sizeof(shadow_tiles) / 32; 
  VDP_loadTileData(shadow_tiles, shadowpos, size, 1);            

  buzzpos = shadowpos + size;
  size = sizeof(buzzShadow_tiles) / 32; 
  VDP_loadTileData(buzz_tiles, buzzpos, size, 1); 

  buzzshadowpos = buzzpos + size;
  size = sizeof(buzzShadow_tiles) / 32; 
  VDP_loadTileData(buzzShadow_tiles, buzzshadowpos, size, 1); 
  
  sprite = random() % 2;
       
  if(sprite == 0)
  {   
    x = 32;
    y = 32;

    VDP_setSprite(0, x - 20, y - 20, SPRITE_SIZE(4, 1), TILE_ATTR(PAL3, 0, 0, 0) + buzzpos, 1);       
    VDP_setSprite(1, x - 20, y - 12, SPRITE_SIZE(4, 1), TILE_ATTR(PAL3, 0, 0, 0) + buzzpos + 4, 2);            
    VDP_setSprite(2, x - 20, y - 4, SPRITE_SIZE(4, 1), TILE_ATTR(PAL3, 0, 0, 0) + buzzpos + 8, 3);            
    VDP_setSprite(3, x - 20, y + 4, SPRITE_SIZE(4, 1), TILE_ATTR(PAL3, 0, 0, 0) + buzzpos + 12, 4);                                

    VDP_setSprite(4, x, y, SPRITE_SIZE(4, 1), TILE_ATTR(PAL2, 0, 0, 0) + buzzshadowpos, 5);       
    VDP_setSprite(5, x, y+8, SPRITE_SIZE(4, 1), TILE_ATTR(PAL2, 0, 0, 0) + buzzshadowpos + 4, 6);            
    VDP_setSprite(6, x, y+16, SPRITE_SIZE(4, 1), TILE_ATTR(PAL2, 0, 0, 0) + buzzshadowpos + 8, 7);            
    VDP_setSprite(7, x, y+24, SPRITE_SIZE(4, 1), TILE_ATTR(PAL2, 0, 0, 0) + buzzshadowpos + 12, 0);             
                      
    sprite = 1;
  }
  else
  {                
    VDP_setSprite(0, x, y, SPRITE_SIZE(4, 1), TILE_ATTR(PAL2, 0, 0, 0) + shadowpos, 1);       
    VDP_setSprite(1, x, y+8, SPRITE_SIZE(4, 1), TILE_ATTR(PAL2, 0, 0, 0) + shadowpos + 4, 2);            
    VDP_setSprite(2, x, y+16, SPRITE_SIZE(4, 1), TILE_ATTR(PAL2, 0, 0, 0) + shadowpos + 8, 3);            
    VDP_setSprite(3, x, y+24, SPRITE_SIZE(4, 1), TILE_ATTR(PAL2, 0, 0, 0) + shadowpos + 12, 0);             
    sprite = 0;
  }           
    
  while(!exit)
  {        
    if(text)
    {
        text --;
        if(text == 0)
            changeback = 1;
    }       

    if(changeback)
    {           
      u16 sonic_water;                         
      changeback = 0;            
      VDP_clearTileMapRect(APLAN, 0, 0, 320/8, 224/8);
      switch(back)
      {
        case 0:
          size = sizeof(motoko_tiles) / 32; 
          VDP_loadTileData(motoko_tiles, ind, size, 1); 
          VDP_setPalette(PAL1, motoko_pal);
          VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + ind, 0, 0, 320/8, 224/8); 
          break;
        case 1:                    
          VDP_setPalette(PAL0, sonicback_pal);
          VDP_setPalette(PAL1, sonicwater_pal);                
          size = sizeof(sonicback_tiles) / 32; 
          VDP_loadTileData(sonicback_tiles, ind, size, 1);     
          sonic_water = ind + size;
          size = sizeof(sonicwater_tiles) / 32;           
          VDP_loadTileData(sonicwater_tiles, sonic_water, size, 1);             
          VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL0, 0, 0, 0) + ind, 0, 0, 256/8, 152/8); 
          VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL0, 0, 0, 0) + ind, 256/8, 0, 256/8, 152/8);         
          VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + sonic_water, 0, 152/8, 256/8, 48/8); 
          VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + sonic_water, 256/8, 152/8, 256/8, 48/8);                   
          break;
        case 2:
          VDP_setPalette(PAL0, palette_grey);
          size = sizeof(check_tile) / 32; 
          VDP_loadTileData(check_tile, ind, size, 1); 
          VDP_setPalette(PAL1, bw_pal);
          VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + ind, 0, 0, 320/8, 224/8); 
          break;
        case 3:
          size = sizeof(bw_tile) / 32; 
          VDP_loadTileData(bw_tile, ind, size, 1); 
          VDP_setPalette(PAL1, bw_pal);
          VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + ind, 0, 0, 320/8, 224/8); 
          break;
      }
    }

    if(sprite == 1)
    {
      VDP_setSpritePosition(0, x - 20, y - 20);
      VDP_setSpritePosition(1, x - 20, y - 12);
      VDP_setSpritePosition(2, x - 20, y - 4);
      VDP_setSpritePosition(3, x - 20, y + 4);
    }

    if(field == invert)
    {
      int index = 0;

      if(sprite == 1)
        index = 4;
      VDP_setSpritePosition(index++, x, y);
      VDP_setSpritePosition(index++, x, y+8);
      VDP_setSpritePosition(index++, x, y+16);
      VDP_setSpritePosition(index++, x, y+24);
      
      field = !field;
    }
    else
    {
      int index = 0;

      if(sprite == 1)
        index = 4;
      VDP_setSpritePosition(index++, 320, 224);
      VDP_setSpritePosition(index++, 320, 224);
      VDP_setSpritePosition(index++, 320, 224);
      VDP_setSpritePosition(index++, 320, 224);

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
    }   

    if (buttons & BUTTON_RIGHT)
    {
      if(x < 288)
        x ++;   
    }   

    if (pressedButtons & BUTTON_START)
      exit = 1;

    if (pressedButtons & BUTTON_A)
    {
      invert = !invert;
      if(invert)
        VDP_drawTextBG(APLAN, "Shadow on odd frames ", TILE_ATTR(PAL0, 0, 0, 0), 19, 0);
      else
        VDP_drawTextBG(APLAN, "Shadow on even frames", TILE_ATTR(PAL0, 0, 0, 0), 19, 0);
      
      text = 60;            
    }

    if (pressedButtons & BUTTON_B)
    {
      if(back < 3)
        back ++;
      else
        back = 0;
      
      VDP_clearTileMapRect(APLAN, 0, 0, 320/8, 224/8);
      changeback = 1;
    }

    if (pressedButtons & BUTTON_C)
    {
      if(sprite == 0)
      {                                                                
        VDP_setSprite(0, x - 20, y - 20, SPRITE_SIZE(4, 1), TILE_ATTR(PAL3, 0, 0, 0) + buzzpos, 1);       
        VDP_setSprite(1, x - 20, y - 12, SPRITE_SIZE(4, 1), TILE_ATTR(PAL3, 0, 0, 0) + buzzpos + 4, 2);            
        VDP_setSprite(2, x - 20, y - 4, SPRITE_SIZE(4, 1), TILE_ATTR(PAL3, 0, 0, 0) + buzzpos + 8, 3);            
        VDP_setSprite(3, x - 20, y + 4, SPRITE_SIZE(4, 1), TILE_ATTR(PAL3, 0, 0, 0) + buzzpos + 12, 4);                                
    
        VDP_setSprite(4, x, y, SPRITE_SIZE(4, 1), TILE_ATTR(PAL2, 0, 0, 0) + buzzshadowpos, 5);       
        VDP_setSprite(5, x, y+8, SPRITE_SIZE(4, 1), TILE_ATTR(PAL2, 0, 0, 0) + buzzshadowpos + 4, 6);            
        VDP_setSprite(6, x, y+16, SPRITE_SIZE(4, 1), TILE_ATTR(PAL2, 0, 0, 0) + buzzshadowpos + 8, 7);            
        VDP_setSprite(7, x, y+24, SPRITE_SIZE(4, 1), TILE_ATTR(PAL2, 0, 0, 0) + buzzshadowpos + 12, 0);               
        sprite = 1;
      }
      else
      {                
        VDP_setSprite(0, x, y, SPRITE_SIZE(4, 1), TILE_ATTR(PAL2, 0, 0, 0) + shadowpos, 1);       
        VDP_setSprite(1, x, y+8, SPRITE_SIZE(4, 1), TILE_ATTR(PAL2, 0, 0, 0) + shadowpos + 4, 2);            
        VDP_setSprite(2, x, y+16, SPRITE_SIZE(4, 1), TILE_ATTR(PAL2, 0, 0, 0) + shadowpos + 8, 3);            
        VDP_setSprite(3, x, y+24, SPRITE_SIZE(4, 1), TILE_ATTR(PAL2, 0, 0, 0) + shadowpos + 12, 0);             
        sprite = 0;
      }
    }

    VDP_updateSprites();
    VDP_waitVSync();
  }

  VDP_resetSprites();
  VDP_updateSprites();
}

void StripedSpriteTest()
{
  u16 size, ind, back = 0, changeback = 0;
  u16 x = 0, y = 0, exit = 0;
  u16 buttons, pressedButtons, oldButtons = 0xffff;

  VDP_setPalette(PAL1, motoko_pal);
  VDP_setPalette(PAL2, wb_pal);

  ind = TILE_USERINDEX; 
  size = sizeof(motoko_tiles) / 32; 
  VDP_loadTileData(motoko_tiles, ind, size, 1); 
  ind += size;
  size = sizeof(striped_tiles) / 32; 
  VDP_loadTileData(striped_tiles, ind, size, 1); 

  VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320/8, 224/8); 
  VDP_setSprite(0, x, y, SPRITE_SIZE(4, 1), TILE_ATTR(PAL2, 0, 0, 0) + ind, 1);       
  VDP_setSprite(1, x, y+8, SPRITE_SIZE(4, 1), TILE_ATTR(PAL2, 0, 0, 0) + ind + 4, 2);            
  VDP_setSprite(2, x, y+16, SPRITE_SIZE(4, 1), TILE_ATTR(PAL2, 0, 0, 0) + ind + 8, 3);            
  VDP_setSprite(3, x, y+24, SPRITE_SIZE(4, 1), TILE_ATTR(PAL2, 0, 0, 0) + ind + 12, 0);             
  
  while(!exit)
  {                
    if(changeback)
    {
      u16 sonic_water;                         
      changeback = 0;   
      VDP_clearTileMapRect(APLAN, 0, 0, 320/8, 224/8);        
      switch(back)
      {
        case 0:
          size = sizeof(motoko_tiles) / 32; 
          VDP_loadTileData(motoko_tiles, TILE_USERINDEX, size, 1); 
          VDP_setPalette(PAL1, motoko_pal);
          VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320/8, 224/8); 
          break;
        case 1:                    
          VDP_setPalette(PAL1, sonicback_pal);
          VDP_setPalette(PAL3, sonicwater_pal);                
          size = sizeof(sonicback_tiles) / 32; 
          VDP_loadTileData(sonicback_tiles, TILE_USERINDEX, size, 1);     
          sonic_water = TILE_USERINDEX + size;
          size = sizeof(sonicwater_tiles) / 32;           
          VDP_loadTileData(sonicwater_tiles, sonic_water, size, 1);             
          VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + TILE_USERINDEX, 0, 0, 256/8, 152/8); 
          VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + TILE_USERINDEX, 256/8, 0, 256/8, 152/8);         
          VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL3, 0, 0, 0) + sonic_water, 0, 152/8, 256/8, 48/8); 
          VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL3, 0, 0, 0) + sonic_water, 256/8, 152/8, 256/8, 48/8);                   
          break;
        case 2:          
          size = sizeof(check_tile) / 32; 
          VDP_loadTileData(check_tile, TILE_USERINDEX, size, 1); 
          VDP_setPalette(PAL1, bw_pal);
          VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320/8, 224/8); 
          break;
        case 3:
          size = sizeof(bw_tile) / 32; 
          VDP_loadTileData(bw_tile, TILE_USERINDEX, size, 1); 
          VDP_setPalette(PAL1, bw_pal);
          VDP_fillTileMapRect(APLAN, TILE_ATTR(PAL1, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320/8, 224/8); 
          break;
      }
    }
      
    VDP_setSpritePosition(0, x, y);
    VDP_setSpritePosition(1, x, y+8);
    VDP_setSpritePosition(2, x, y+16);
    VDP_setSpritePosition(3, x, y+24);
        
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

    if (pressedButtons & BUTTON_START)
      exit = 1;

    if (pressedButtons & BUTTON_A)
    {
      if(back > 0)
        back --;
      else
        back = 2;
      
      changeback = 1;
    }

    if (pressedButtons & BUTTON_B)
    {
      if(back < 3)
        back ++;
      else
        back = 0;
      
      changeback = 1;
      VDP_clearTileMapRect(APLAN, 0, 0, 320/8, 224/8);           
    }        

    VDP_updateSprites();
    VDP_waitVSync();
  }

  VDP_resetSprites();
  VDP_updateSprites();
}



void LagTest()
{
  char str[10];
  u16 pal = PAL0, change = 1;
  s16 speed = 1;
  u16 size, ind;
  u16 x = 0, y = 0, x2 = 0, y2 = 0, exit = 0;
  u16 buttons, pressedButtons, oldButtons = 0xffff;
  u16 pos = 0, view = 0, audio  = 1, drawoffset = 0;
  u32 len = 0;    

  s16 clicks[10];

  VDP_setPalette(PAL0, palette_grey);
  VDP_setPalette(PAL1, palette_red);
  VDP_setPalette(PAL2, palette_green);
  VDP_setPalette(PAL3, bw_pal);

  ind = TILE_USERINDEX; 
  size = sizeof(shadow_tiles) / 32; 
  VDP_loadTileData(shadow_tiles, ind, size, 1); 
  ind += size;
  size = sizeof(lag_tiles) / 32; 
  VDP_loadTileData(lag_tiles, ind, size, 1); 

  x = 144;
  y = 60;

  x2 = 108;
  y2 = 96;

  VDP_setSprite(0, x, y, SPRITE_SIZE(4, 1), TILE_ATTR(PAL3, 0, 0, 0) + ind, 1);       
  VDP_setSprite(1, x, y+8, SPRITE_SIZE(4, 1), TILE_ATTR(PAL3, 0, 0, 0) + ind + 4, 2);            
  VDP_setSprite(2, x, y+16, SPRITE_SIZE(4, 1), TILE_ATTR(PAL3, 0, 0, 0) + ind + 8, 3);            
  VDP_setSprite(3, x, y+24, SPRITE_SIZE(4, 1), TILE_ATTR(PAL3, 0, 0, 0) + ind + 12, 4);             

  VDP_setSprite(4, x2, y2, SPRITE_SIZE(4, 1), TILE_ATTR(PAL3, 0, 0, 0) + ind, 5);       
  VDP_setSprite(5, x2, y2+8, SPRITE_SIZE(4, 1), TILE_ATTR(PAL3, 0, 0, 0) + ind + 4, 6);            
  VDP_setSprite(6, x2, y2+16, SPRITE_SIZE(4, 1), TILE_ATTR(PAL3, 0, 0, 0) + ind + 8, 7);            
  VDP_setSprite(7, x2, y2+24, SPRITE_SIZE(4, 1), TILE_ATTR(PAL3, 0, 0, 0) + ind + 12, 0);             
  
  VDP_drawTextBG(APLAN, "Press the \"A\" button when the sprite", TILE_ATTR(PAL0, 0, 0, 0), 2, 22);
  VDP_drawTextBG(APLAN, "is aligned. A negative value means", TILE_ATTR(PAL0, 0, 0, 0), 2, 23);
  VDP_drawTextBG(APLAN, "you pressed \"A\" before they intersect.", TILE_ATTR(PAL0, 0, 0, 0), 2, 24);
  VDP_drawTextBG(APLAN, "\"B\" button toggles horz/vert", TILE_ATTR(PAL0, 0, 0, 0), 2, 25);

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
        change = 0;
      }           
    }

    //play audio 1 frame before
    //if((y == 95 && speed == 1 ) || (y == 97 && speed == -1))
    //    startPlay_PCM(beep, len, 44100, AUDIO_PAN_CENTER);            

    if(y == 96) // half the screen?        
    {
      if(audio)                
      {
        if(speed == 1)
          startPlay_PCM(beep, len, 44100, AUDIO_PAN_LEFT);            
        else
          startPlay_PCM(beep, len, 44100, AUDIO_PAN_RIGHT);            
      }  
      VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL3, 0, 0, 0) + TILE_USERINDEX, 144/8, 96/8, 32/8, 32/8);       
    }
    else
      VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL3, 0, 0, 0) + ind, 144/8, 96/8, 32/8, 32/8); 

    if (pressedButtons & BUTTON_B)
    {
      view ++;
      if(view > 2)
        view = 0;            
    }
  
    if (pressedButtons & BUTTON_C)        
      audio = !audio;    

    if (pressedButtons & BUTTON_START)
      exit = 1;

    if(drawoffset)
    {
      u16 ppos = 0;

      pal = PAL0;
      intToStr(clicks[pos], str, 1);

      if(clicks[pos] == 0)
        pal = PAL2;
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
      VDP_drawTextBG(APLAN, "   ", TILE_ATTR(pal, 0, 0, 0), pos == 9 ? 13 : 12 + ppos, pos);

      if(clicks[pos] >= 0)
        pos++;

      if(pos > 9)
        exit = 1;
      drawoffset = 0;
    }       

    if(y > 132)
    {
      speed = -1;
      change = 1;
    }
    
    if(y < 60)
    {
      speed = 1;
      change = 1;
    }

    y += speed;
    x2 += speed;

    if(view == 0 || view == 2)
    {
      VDP_setSpritePosition(0, x, y);
      VDP_setSpritePosition(1, x, y+8);
      VDP_setSpritePosition(2, x, y+16);
      VDP_setSpritePosition(3, x, y+24);
    }
    else
    {
      VDP_setSpritePosition(0, 320, 224);
      VDP_setSpritePosition(1, 320, 224);
      VDP_setSpritePosition(2, 320, 224);
      VDP_setSpritePosition(3, 320, 224);
    }

    if(view == 1 || view == 2)
    {
      VDP_setSpritePosition(4, x2, y2);
      VDP_setSpritePosition(5, x2, y2+8);
      VDP_setSpritePosition(6, x2, y2+16);
      VDP_setSpritePosition(7, x2, y2+24);
    }
    else
    {
      VDP_setSpritePosition(4, 320, 224);
      VDP_setSpritePosition(5, 320, 224);
      VDP_setSpritePosition(6, 320, 224);
      VDP_setSpritePosition(7, 320, 224);
    }
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

    VDP_setPalette(PAL3, back_pal); 

    size = sizeof(back_tiles) / 32; 
    VDP_loadTileData(back_tiles,  TILE_USERINDEX, size, 1); 
    
    VDP_fillTileMapRectInc(BPLAN, TILE_ATTR(PAL3, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320/8, 224/8);            
    exit = 0;

    for(c = 0; c < 10; c++)
    {
      if(clicks[c] !=  0xFF)
      {                
        intToStr(clicks[c], str, 1);

        pal = PAL0;
        if(clicks[c] == 0)
          pal = PAL2;
        if(clicks[c] < 0)
          pal = PAL1;
        if(clicks[c] >= 0)
        {
          total += clicks[c];
          count ++;
        }

        VDP_drawTextBG(APLAN, str, TILE_ATTR(pal, 0, 0, 0), 10, c + 10);
      }
    }   

    VDP_drawTextBG(APLAN, "+", TILE_ATTR(PAL1, 0, 0, 0), 8, 14);

    if(count > 0)
    {
      u16 h = 10;
      u16 v = 20;            

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
      VDP_drawTextBG(APLAN, str, TILE_ATTR(PAL2, 0, 0, 0), h, v);
      h += strlen(str);
      h -= 2;
      VDP_drawTextBG(APLAN, " frames", TILE_ATTR(PAL0, 0, 0, 0), h, v++);

      h = 14;
      v = 12;
      tot = FIX32(16.6834);
      cnt = fix32Mul(totald, tot);
      fix32ToStr(cnt, str);
      VDP_drawTextBG(APLAN, str, TILE_ATTR(PAL2, 0, 0, 0), h, v);
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


void ScrollTest()
{    
  u16 size, sonic_floor, sonic_water;
  u16 exit = 0, frame = 1;
  u16 buttons, oldButtons = 0xffff, pressedButtons;
  int x = 0, speed = 1, acc = -1, pause = 0;
  
  VDP_setPalette(PAL0, sonicback_pal);  
  VDP_setPalette(PAL1, sonicwater_pal);
  VDP_setPalette(PAL2, sonicfloor_pal);

  size = sizeof(sonicback_tiles) / 32; 
  VDP_loadTileData(sonicback_tiles, TILE_USERINDEX, size, 1);     

  sonic_water = TILE_USERINDEX + size;
  size = sizeof(sonicwater_tiles) / 32; 
  VDP_loadTileData(sonicwater_tiles, sonic_water, size, 1);     

  sonic_floor = sonic_water + size;
  size = sizeof(sonicfloor_tiles) / 32; 
  VDP_loadTileData(sonicfloor_tiles, sonic_floor, size, 1);     

  VDP_fillTileMapRectInc(BPLAN, TILE_ATTR(PAL0, 0, 0, 0) + TILE_USERINDEX, 0, 0, 256/8, 152/8); 
  VDP_fillTileMapRectInc(BPLAN, TILE_ATTR(PAL0, 0, 0, 0) + TILE_USERINDEX, 256/8, 0, 256/8, 152/8); 

  VDP_fillTileMapRectInc(BPLAN, TILE_ATTR(PAL1, 0, 0, 0) + sonic_water, 0, 152/8, 256/8, 48/8); 
  VDP_fillTileMapRectInc(BPLAN, TILE_ATTR(PAL1, 0, 0, 0) + sonic_water, 256/8, 152/8, 256/8, 48/8); 

  VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL2, 0, 0, 0) + sonic_floor, 0, 96/8, 256/8, 128/8); 
  VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL2, 0, 0, 0) + sonic_floor, 256/8, 96/8, 256/8, 128/8); 
  while(!exit)
  {
    switch(frame)
    {
      case 30:
        VDP_setPalette(PAL1, sonicwater_pal);
        break;
      case 60:
        VDP_setPalette(PAL1, sonicwater2_pal);
        break;
      case 90:
        VDP_setPalette(PAL1, sonicwater3_pal);  
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

    VDP_setHorizontalScroll(APLAN, 0, x);        
    VDP_setHorizontalScroll(BPLAN, 0, x/2);        
    VDP_waitVSync();
  }
  VDP_setHorizontalScroll(APLAN, 0, 0);
  VDP_setHorizontalScroll(BPLAN, 0, 0);
  VDP_loadFont(font_tiles, 1);
}

void SoundTest()
{
  int sel = 0;
   u16 ind = 0, size = 0, exit = 0;
  u16 buttons, oldButtons = 0xffff, pressedButtons;
  u16  len = 0;

  len = sizeof(beep);
  VDP_setPalette(PAL0, palette_grey);
  VDP_setPalette(PAL1, back_pal); 
  VDP_setPalette(PAL2, palette_green);
  VDP_setPalette(PAL3, bw_pal);    

  ind = TILE_USERINDEX; 
  size = sizeof(back_tiles) / 32; 
  VDP_loadTileData(back_tiles, ind, size, 1); 
  
  VDP_fillTileMapRectInc(BPLAN, TILE_ATTR(PAL1, 0, 0, 0) + TILE_USERINDEX, 0, 0, 320/8, 224/8);    

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
        startPlay_PCM(beep, len, 44100, AUDIO_PAN_LEFT);                  
      if(sel == 1)
        startPlay_PCM(beep, len, 44100, AUDIO_PAN_CENTER);            
      if(sel == 2)
        startPlay_PCM(beep, len, 44100, AUDIO_PAN_RIGHT);                       
    }
          
    VDP_drawTextBG(APLAN, "Sound Test", TILE_ATTR(PAL0, 0, 0, 0), 14, 6);
    VDP_drawTextBG(APLAN, "Left Channel", TILE_ATTR(sel == 0 ? PAL2 : PAL0, 0, 0, 0), 5, 12);
    VDP_drawTextBG(APLAN, "Center Channel", TILE_ATTR(sel == 1 ? PAL2 : PAL0, 0, 0, 0), 14, 14);
    VDP_drawTextBG(APLAN, "Right Channel", TILE_ATTR(sel == 2 ? PAL2 : PAL0, 0, 0, 0), 22, 12);    

    VDP_waitVSync();
  }
}
