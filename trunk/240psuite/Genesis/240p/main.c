/* 
 * 240p Test Suite
 * Copyright (C)2011-2014 Artemio Urbina
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


#include "main.h"

u8	joytype = JOY_TYPE_UNKNOWN;
u8	IsPALVDP = 0;
u8	VDPChanged = 0;

void VBlankIntCallback()
{
		u8 PalCheck = 0;
		
	 	joytype = JOY_getJoypadType(JOY_1);
	 	PalCheck = Detect_VDP_PAL();
	 	if(PalCheck != IsPALVDP)
	 	{
	 		IsPALVDP = PalCheck;
	 		if(!IsPALVDP && pal_240)
	 		{
				VDP_setScreenHeight224(); 
    		pal_240 = 0;
	 		}
	 		VDPChanged = 1;
	 	}
	 	else
	 		VDPChanged = 0;
}

int main() 
{ 
  u16 cursel = 1, pos, reload = 1;
  u16 buttons, oldButtons = 0xffff, pressedButtons;
  u16 ind = 0, size = 0;
  char md_ver[30];
  
  VDP_init(); 
  JOY_init();
   
  SYS_setVIntCallback(VBlankIntCallback);   
  VDP_setScreenWidth320(); 
  if(Detect_VDP_PAL())
  {
    VDP_setScreenHeight240();
    pal_240 = 1;
  }
  else
  {
    VDP_setScreenHeight224(); 
    pal_240 = 0;
  }

  VDP_loadFontData(font_tiles, FONT_LEN, USE_DMA);
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
      VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL3, 0, 0, 0) + ind, 216/8, 80/8, 56/8, 104/8);      
      reload = 0;
    }   

    if(VDPChanged)
    {            
      if(cursel == 13)
        cursel = 14;     	
    }     
              
    pos = 6;
    VDP_drawTextBG(APLAN, "Test Patterns", TILE_ATTR(cursel == 1 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);        
    VDP_drawTextBG(APLAN, "Drop Shadow Test", TILE_ATTR(cursel == 2 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);        
    VDP_drawTextBG(APLAN, "Striped Sprite Test", TILE_ATTR(cursel == 3 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);        
    VDP_drawTextBG(APLAN, "Lag Test", TILE_ATTR(cursel == 4 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    VDP_drawTextBG(APLAN, "Manual Lag Test", TILE_ATTR(cursel == 5 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    VDP_drawTextBG(APLAN, "Scroll Test", TILE_ATTR(cursel == 6 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);    
    VDP_drawTextBG(APLAN, "Grid Scroll Test", TILE_ATTR(cursel == 7 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    VDP_drawTextBG(APLAN, "Horizontal Stripes", TILE_ATTR(cursel == 8 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    VDP_drawTextBG(APLAN, "Checkerboard", TILE_ATTR(cursel == 9 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    VDP_drawTextBG(APLAN, "Backlit Zone Test", TILE_ATTR(cursel == 10 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    VDP_drawTextBG(APLAN, "Sound Test", TILE_ATTR(cursel == 11 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);    
    VDP_drawTextBG(APLAN, "Help", TILE_ATTR(cursel == 12 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    if(IsPALVDP)
      VDP_drawTextBG(APLAN, pal_240 ? "PAL VDP 320x240p " : "PAL VDP 320x224p ", TILE_ATTR(cursel == 13 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    else
    {
        if(VDP_Detect_Interlace())
            VDP_drawTextBG(APLAN, "NTSC VDP 320x224p", TILE_ATTR(PAL0, 0, 0, 0), 5, pos++);
        else
            VDP_drawTextBG(APLAN, "NTSC VDP 320x224i", TILE_ATTR(PAL0, 0, 0, 0), 5, pos++);
    }
    VDP_drawTextBG(APLAN, "Credits", TILE_ATTR(cursel == 14 ? PAL1 : PAL0, 0, 0, 0), 5, ++pos); 
        	    
 		Detect_MD(md_ver);
 		VDP_drawTextBG(APLAN, md_ver, TILE_ATTR(PAL0, 0, 0, 0), 20, 26);   		
 		
    buttons = JOY_readJoypad(JOY_1);
    pressedButtons = buttons & ~oldButtons;
    oldButtons = buttons;
		
		if (pressedButtons & BUTTON_Z)
		{								
			DrawHelp(HELP_GENERAL);  			
     	reload = 1;   			
		}
		
    if (pressedButtons & BUTTON_DOWN)
    {
      cursel ++;
      if(cursel > pos - 6)
        cursel = 1;
      if(cursel == 13 && !IsPALVDP)
        cursel = 14;
    }

    if (pressedButtons & BUTTON_UP)
    {
      cursel --;
      if(cursel < 1)
        cursel = pos - 6;
      if(cursel == 13 && !IsPALVDP)
        cursel = 12;
    }

    if (pressedButtons & BUTTON_A)
    {       
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
          PassiveLagTest();             
          break;
        case 5:
          LagTest();   
          break;
        case 6:
          HScrollTest();   
          break;		
        case 7:
          VScrollTest();   
          break;
        case 8:
          DrawStripes();   
          break;
        case 9: 
          DrawCheckBoard();                                   
          break;  
        case 10: 
          LEDZoneTest();                                   
          break;  
        case 11: 
          SoundTest();                                   
          break;          
        case 12: 
          DrawHelp(HELP_GENERAL);                                   
          break;
        case 13:
          if(IsPALVDP)
          {
            if(!pal_240)
            {
              VDP_setScreenHeight240();
              pal_240 = 1;
            }
            else
            {
              VDP_setScreenHeight224(); 
              pal_240 = 0;
            }
          }
          break;
        case 14: 
          DrawCredits();                                   
          break;
      }      

			FadeAndCleanUp();
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
    
  FadeAndCleanUp();
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
      VDP_fillTileMapRectInc(APLAN, TILE_ATTR(PAL3, 0, 0, 0) + ind, 216/8, 80/8, 56/8, 104/8);

      reload = 0;
    }        
            
    pos = 6;
    VDP_drawTextBG(APLAN, "Pluge", TILE_ATTR(cursel == 1 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    VDP_drawTextBG(APLAN, "Color Bars", TILE_ATTR(cursel == 2 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    if(!IsPALVDP)
    	VDP_drawTextBG(APLAN, "SMPTE Color Bars", TILE_ATTR(cursel == 3 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    else
    	VDP_drawTextBG(APLAN, "EBU Color Bars", TILE_ATTR(cursel == 3 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    VDP_drawTextBG(APLAN, "Color Bars with Gray Reference", TILE_ATTR(cursel == 4 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    VDP_drawTextBG(APLAN, "Color Bleed Check", TILE_ATTR(cursel == 5 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    VDP_drawTextBG(APLAN, pal_240 ? "Grid 320x240" : "Grid 320x224", TILE_ATTR(cursel == 6 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    VDP_drawTextBG(APLAN, pal_240 ? "Grid 256x240" : "Grid 256x224", TILE_ATTR(cursel == 7 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    VDP_drawTextBG(APLAN, "Linearity", TILE_ATTR(cursel == 8 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    VDP_drawTextBG(APLAN, "Gray Ramp", TILE_ATTR(cursel == 9 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);        
    VDP_drawTextBG(APLAN, "White & RGB Screens", TILE_ATTR(cursel == 10 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);                
    VDP_drawTextBG(APLAN, "100 IRE", TILE_ATTR(cursel == 11 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);     
    VDP_drawTextBG(APLAN, "Sharpness", TILE_ATTR(cursel == 12 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);     
    VDP_drawTextBG(APLAN, "Overscan", TILE_ATTR(cursel == 13 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);     
    if(IsPALVDP)
      VDP_drawTextBG(APLAN, pal_240 ? "PAL VDP 320x240p " : "PAL VDP 320x224p ", TILE_ATTR(cursel == 14 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
    else
      VDP_drawTextBG(APLAN, "NTSC VDP 320x224p", TILE_ATTR(PAL0, 0, 0, 0), 5, pos++);           
    VDP_drawTextBG(APLAN, "Back to Main Menu", TILE_ATTR(cursel == 15 ? PAL1 : PAL0, 0, 0, 0), 5, ++pos);    
    
    buttons = JOY_readJoypad(JOY_1);
    pressedButtons = buttons & ~oldButtons;
    oldButtons = buttons;
    
     if(VDPChanged)
    {      
      if(cursel == 14)
        cursel = 15;
    }   
    
 		if(pal_240 && !IsPALVDP)
    {      
      if(cursel == 14)
        cursel = 15;
    }  
    
		if (pressedButtons & BUTTON_Z)
		{				
			DrawHelp(HELP_GENERAL);  			
     	reload = 1;   			
		}
		
    if (pressedButtons & BUTTON_DOWN)
    {
      cursel ++;
      if(cursel > 15)
        cursel = 1;
      if(cursel == 14 && !IsPALVDP)
        cursel = 15;
    }

    if (pressedButtons & BUTTON_UP)
    {
      cursel --;
      if(cursel < 1)
        cursel = 15;
      if(cursel == 14 && !IsPALVDP)
        cursel = 13;
    }

    if (pressedButtons & BUTTON_B || pressedButtons & BUTTON_START)
      done = 1;            
    
    if (pressedButtons & BUTTON_A)
    {                        
      switch(cursel)
      {
        case 1:
          DrawPluge();
          break;
        case 2:
          DrawColorBars();
          break;
        case 3:
          DrawSMPTE();
          break;
        case 4:
          Draw601ColorBars();
          break;
        case 5:
          DrawColorBleed();                    
          break;
        case 6:
          DrawGrid(GRID_320);                    
          break;
        case 7:
          DrawGrid(GRID_256);                    
          break;
        case 8:
          DrawLinearity();
          break;
        case 9:
          DrawGrayRamp();
          break;
        case 10:
          DrawWhiteScreen();
          break;
        case 11: 
          Draw100IRE();
          break;
        case 12: 
          DrawSharpness();
          break;
        case 13: 
          DrawOverscan();
          break;
        case 14:
          if(IsPALVDP)
          {
            if(!pal_240)
            {
              VDP_setScreenHeight240();
              pal_240 = 1;
            }
            else
            {
              VDP_setScreenHeight224(); 
              pal_240 = 0;
            }
          }
          break;
        case 15: 
          done = 1;
          break;
      }
      FadeAndCleanUp();
      
      buttons = JOY_readJoypad(JOY_1);
    	pressedButtons = buttons & ~oldButtons;
    	oldButtons = buttons;
    	
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

void Detect_MD(char *str)
{
  char *pointer = NULL;
  u16 IsPAL = 0, data = 0;
  u16	TMSS = 0, IsAsia = 0;

  pointer = (char *)0xA10001;
  
  data = *pointer;
  
  IsPAL = (data & 0x40);
  TMSS = (data & 0x0F);
  IsAsia = (data & 0xFF00) == 0x0;
    
  if(TMSS)
  	strcpy(str, "TMSS ");
  else
  	strcpy(str, "     ");
  
  if(!IsAsia)
  { 
  	if(IsPAL) 
  		strcat(str, "MegaDrive EU");  	
  	else
  		strcat(str, "Sega Genesis");  	
  }
  else
  	strcat(str, "MegaDrive JP");  	
}

void DrawCredits()
{
  u16 ind = 0, size = 0, exit = 0, pos = 8, counter = 1;
  u16 buttons, oldButtons = 0xffff, pressedButtons;

	FadeAndCleanUp();
  VDP_setPalette(PAL0, palette_grey);
  VDP_setPalette(PAL1, palette_green); 
  VDP_setPalette(PAL2, back_pal);
  VDP_setPalette(PAL3, bw_pal);    
  
  ind = TILE_USERINDEX; 
  size = sizeof(back_tiles) / 32; 
  VDP_loadTileData(back_tiles, ind, size, USE_DMA); 
  
  VDP_setMyTileMapRect(BPLAN, back_map, TILE_USERINDEX, 0, 0, 320/8, 224/8);      
  
  VDP_setVerticalScroll(PLAN_A, 4);

#ifdef SEGACD
	pos = 6;
#endif

  VDP_drawTextBG(APLAN, "Code and Patterns:", TILE_ATTR(PAL1, 0, 0, 0), 4, pos++); 
  pos++; 
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
  VDP_drawTextBG(APLAN, "Collaboration:", TILE_ATTR(PAL1, 0, 0, 0), 4, pos++);
  VDP_drawTextBG(APLAN, "Konsolkongen & shmups regulars", TILE_ATTR(PAL0, 0, 0, 0), 5, pos++);
  VDP_drawTextBG(APLAN, "Info on using this test suite:", TILE_ATTR(PAL1, 0, 0, 0), 4, pos++);
  VDP_drawTextBG(APLAN, "http://junkerhq.net/240p", TILE_ATTR(PAL0, 0, 0, 0), 5, pos++);

  VDP_drawTextBG(APLAN, "Ver. 1.14", TILE_ATTR(PAL0, 0, 0, 0), 26, 6);
  VDP_drawTextBG(APLAN, "27/06/2014", TILE_ATTR(PAL0, 0, 0, 0), 26, 7);
  
#ifdef SEGACD
	pos = 7;
#else
	pos = 9;
#endif
  while(!exit)
  {
    buttons = JOY_readJoypad(JOY_1);
    pressedButtons = buttons & ~oldButtons;
    oldButtons = buttons;
    
    if(counter == 1)
    	VDP_drawTextBG(APLAN, "Artemio Urbina      ", TILE_ATTR(PAL0, 0, 0, 0), 5, pos);  	  	
	  if(counter == 60*4)
			VDP_drawTextBG(APLAN, "@Artemio (twitter)  ", TILE_ATTR(PAL0, 0, 0, 0), 5, pos);  
	  if(counter == 60*8)
	  	VDP_drawTextBG(APLAN, "aurbina@junkerhq.net", TILE_ATTR(PAL0, 0, 0, 0), 5, pos);  
	  if(counter == 60*16)
			counter = 0;				      

    if (pressedButtons & BUTTON_START)
      exit = 1;         

    VDP_waitVSync();
    counter ++;
  }

	VDP_fadeOutAll(FADE_TIME, 0);
  VDP_setVerticalScroll(PLAN_A, 0);
}

void FadeAndCleanUp()
{
	VDP_fadeOutAll(FADE_TIME, 0);
	VDP_resetSprites();
  VDP_updateSprites();
  VDP_setHorizontalScroll(PLAN_B, 0);
  VDP_setHorizontalScroll(PLAN_A, 0);
  VDP_setVerticalScroll(PLAN_A, 0);
  VDP_setHilightShadow(0);
  VDP_setScreenWidth320(); 		
	VDP_resetScreen();
}
