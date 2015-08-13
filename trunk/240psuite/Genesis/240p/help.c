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

#include "genesis.h"
#include "res.h"
#include "help.h"
#include "myvdp.h"
#include "main.h"

u8  showhelp = 0;

void CleanOrShowHelp(int option)
{
	if(joytype != JOY_TYPE_PAD6)
	{
		if(showhelp)
  		DrawHelp(option);    	
  	else
  	{
  		VDP_fadeOutAll(FADE_TIME, 0);
			VDP_clearTileMapRect(BPLAN, 0, 0, 320/8, 224/8);
			VDP_clearTileMapRect(APLAN, 0, 0, 320/8, 224/8);
			VDP_resetScreen();
  	}
	}	    
	else
	{
		VDP_fadeOutAll(FADE_TIME, 0);
		VDP_clearTileMapRect(BPLAN, 0, 0, 320/8, 224/8);
		VDP_clearTileMapRect(APLAN, 0, 0, 320/8, 224/8);
		VDP_resetScreen();
	}
}

void DrawHelp(int option)
{
  u16 ind = 0, size = 0, exit = 0, redraw = 1, totalpages = 1, page = 1;
  u16 buttons, oldButtons = 0xffff, pressedButtons;
	
	FadeAndCleanUp();
	
  VDP_setPalette(PAL0, palette_grey);
  VDP_setPalette(PAL1, palette_green); 
  VDP_setPalette(PAL2, back_pal);
  VDP_setPalette(PAL3, bw_pal);    
  
  ind = TILE_USERINDEX; 
  size = sizeof(back_tiles) / 32; 
  VDP_loadTileData(back_tiles, ind, size, USE_DMA); 
    
  if(option == HELP_GENERAL)
  	showhelp = 1;
  	
  switch(option)
  {
  	case HELP_GENERAL:
  	case HELP_PLUGE:
  	case HELP_STRIPES:
  	case HELP_SHADOW:
  	case HELP_LED:
  	case HELP_LAG:
  		totalpages = 2;
  		break;
  	case HELP_MANUALLAG:
  		totalpages = 3;
  		break;
  }
  	
	VDP_setMyTileMapRect(BPLAN, back_map, TILE_USERINDEX, 0, 0, 320/8, 224/8);
  while(!exit)
  { 
  	if(redraw)   
  	{
  		int y = 7;
  		
  		VDP_clearTileMapRect(APLAN, 0, 0, 320/8, 224/8);
  	
		  switch(option)
		  {
		    case HELP_GENERAL: 
		    	switch(page)
		    	{     
		    		case 1:		    		
				      VDP_drawTextBG(APLAN, "          HELP (1/2)", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
				      VDP_drawTextBG(APLAN, "The 240p Test Suite was designed", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "with two goals in mind:", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      y++;
				      VDP_drawTextBG(APLAN, "1) Evaluate 240p signals on TV", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "sets and video procesors; and", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      y++;
				      VDP_drawTextBG(APLAN, "2) Provide calibration patterns", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "from a game console to help in", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "properly calibrating the display", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "black, white and color levels.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);	
				      y++;	      
				      VDP_drawTextBG(APLAN, "The Genesis/MegaDrive can output", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "224 active video lines. ", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "(cont...)", TILE_ATTR(PAL0, 0, 0, 0), 26, 22);		      
				      break;
				    case 2:
							VDP_drawTextBG(APLAN, "          HELP (2/2)", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);				    					      				      
				      VDP_drawTextBG(APLAN, "In PAL consoles, it can display", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "either 224 or 240 lines.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      y++;
				      if(joytype != JOY_TYPE_PAD6)
				      {
				      	VDP_drawTextBG(APLAN, "When enabled, help will be shown", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      	VDP_drawTextBG(APLAN, "after selecting a test/pattern.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      }
				      else
				      {
				      	VDP_drawTextBG(APLAN, "Help can be shown with 'Z' in", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      	VDP_drawTextBG(APLAN, "any test or pattern.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      }				      
				      y++;		      	
				      VDP_drawTextBG(APLAN, "The 240p suite is also available", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "in GameCube, Nintendo Wii,", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "PC Engine/TG-16, Super CD,", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "CD-ROM2, and Sega Dreamcast.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);				          
				      y++;
				      VDP_drawTextBG(APLAN, "Visit:", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
					  VDP_drawTextBG(APLAN, "http://junkerhq.net/240p", TILE_ATTR(PAL1, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "for more information.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				    	break;
		    	}
		      break;
		    case HELP_PLUGE:      
		    	switch(page)
		    	{
			    	case 1:
				      VDP_drawTextBG(APLAN, "         PLUGE (1/2)", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
					  VDP_drawTextBG(APLAN, "NTSC levels require black to be", TILE_ATTR(PAL1, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "at 7.5 IRE for video. This HW", TILE_ATTR(PAL1, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "lowest is 6 IRE (6%), so using", TILE_ATTR(PAL1, 0, 0, 0), 4, y++);      
				      VDP_drawTextBG(APLAN, "this value for general 240p use", TILE_ATTR(PAL1, 0, 0, 0), 4, y++);      				      
				      VDP_drawTextBG(APLAN, "is not recommended.", TILE_ATTR(PAL1, 0, 0, 0), 4, y++);
				      y++;
				      VDP_drawTextBG(APLAN, "Of course using it as reference", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "will work perfectly for games", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);				      
				      VDP_drawTextBG(APLAN, "in this platform.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      y++;
				      VDP_drawTextBG(APLAN, "In PAL - and console gaming in", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "general - it is adviced to use", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		
				      VDP_drawTextBG(APLAN, "a value of 2 IRE as black.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		
				      VDP_drawTextBG(APLAN, "(cont...)", TILE_ATTR(PAL0, 0, 0, 0), 26, 22);
				      break;				      
				    case 2:
				    	VDP_drawTextBG(APLAN, "         PLUGE (2/2)", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
				      VDP_drawTextBG(APLAN, "The PLUGE pattern is used to", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "help adjust the black level to", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "a correct value. ", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);      
				      y++;
				      VDP_drawTextBG(APLAN, "The inner bars on the sides are", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "black at 6%, the outer at 12%. ", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "If these bars are not visible, ", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "adjust the \"brightness\" control", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "until they are.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      y++;
				      VDP_drawTextBG(APLAN, "You should lower it until they", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "are not visible, and raise it", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "until they show.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      break;				      
				  }
		      break;
		    case HELP_COLORS:      
		      VDP_drawTextBG(APLAN, "           COLORBARS", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
		      VDP_drawTextBG(APLAN, "This pattern allows you to", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "calibrate each color: Red, Green", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "and Blue; as well as white.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);            
		    	y++;
		      VDP_drawTextBG(APLAN, "Adjust the white level first, ", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "using the \"Contrast\" control", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "on your TV set. Raise it until", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "you cannot distinguish between", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "the blocks under \"C\" and \"E\",", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "and lower it slowly until you", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "can clearly tell them appart.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		    	y++;
		      VDP_drawTextBG(APLAN, "Do the same for each color.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      break;
		    case HELP_GRID:      
		      VDP_drawTextBG(APLAN, "              GRID", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
		      if(pal_240)
		        VDP_drawTextBG(APLAN, "This grid uses the full 320x240", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      else
		        VDP_drawTextBG(APLAN, "This grid uses the full 320x224", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      if(pal_240)
		        VDP_drawTextBG(APLAN, "PAL resolution.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      else
		        VDP_drawTextBG(APLAN, "resolution.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      y++;
		      VDP_drawTextBG(APLAN, "You can use it to verify that ", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);            
		      VDP_drawTextBG(APLAN, "all the visible area is being ", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "displayed, and that there is no", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "distortion present.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      y++;
		      VDP_drawTextBG(APLAN, "The full active video signal can", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "be filled with gray by pressing", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "the 'A' button.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      break;
		    case HELP_BLEED:
		      VDP_drawTextBG(APLAN, "          COLOR BLEED", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
		      VDP_drawTextBG(APLAN, "This pattern helps diagnose", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "color bleed caused by", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "unneeded color upsampling. ", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);            
		      y++;
		      VDP_drawTextBG(APLAN, "You can toggle between", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "vertical bars and", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "checkerboard with 'A'.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);      
		      break;
		    case HELP_IRE:
		      VDP_drawTextBG(APLAN, "          100 IRE", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
		      VDP_drawTextBG(APLAN, "You can vary IRE intensity", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "with A and B. Values are:", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "13, 25, 41, 53, 66, 82, 94", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);                  
		      break;
		    case HELP_601CB:
		      VDP_drawTextBG(APLAN, "          601 COLORBARS", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
		      VDP_drawTextBG(APLAN, "You can use color filters or the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "blue only option in your display", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "-if available- to confirm color", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "balance.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
		      break;      
		    case HELP_SHARPNESS:
		      VDP_drawTextBG(APLAN, "            SHARPNESS", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
		      VDP_drawTextBG(APLAN, "You should set the sharpness of", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "your CRT to a value that shows", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "clean black and gray transitions", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "with no white ghosting between.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      y++;     
		      VDP_drawTextBG(APLAN, "On most modern displays, the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "sharpness control is an edge-", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "enhancement control, and most", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "probably should be set to zero,", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "or in the middle.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
		      break;      
		    case HELP_OVERSCAN:
		      VDP_drawTextBG(APLAN, "            OVERSCAN", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
		      VDP_drawTextBG(APLAN, "With this pattern you can", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "interactively find out the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "overscan in pixels of each edge", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "in a display.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      y++;     
		      VDP_drawTextBG(APLAN, "Use left and right to increment", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "the overscan until you see the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "white border, then go back one", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "pixel. The resulting number is", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "the amount of overscan in pixels", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "in each direction.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      break;      
		    case HELP_SMPTE:
		      VDP_drawTextBG(APLAN, "         SMPTE COLOR BARS", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
		      VDP_drawTextBG(APLAN, "This pattern can be used to", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "calibrate for NTSC levels", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "regarding contrast,", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "brightness and colors. ", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      y++;     
		      VDP_drawTextBG(APLAN, "You can toggle between 75% and", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "100% SMPTE color bars with A.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "Of course the percentages are", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "relative to the console output.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
		      y++;
		      VDP_drawTextBG(APLAN, "You can use color filters or the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "blue only option in your display", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "-if available- to confirm color", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "balance.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
			  y++;		      
			  VDP_drawTextBG(APLAN, "This HW lowest black is 6%.", TILE_ATTR(PAL1, 0, 0, 0), 4, y++);			  
		      break;      
		    case HELP_LINEARITY:
		      VDP_drawTextBG(APLAN, "          LINEARITY", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
		      VDP_drawTextBG(APLAN, "This pattern shows 5 circles,", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "that are matched to the 10:11", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "NTSC and the 59:54 PAL pixel", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "aspect ratios.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      y++;     
		      VDP_drawTextBG(APLAN, "The linearity of the display or", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "upscaler can be verified by", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "measuring the diameter of the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "circles.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
		      y++;
		      VDP_drawTextBG(APLAN, "Of course the linearity should", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "be kept in all directions.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
		      break;      
		    case HELP_GRAY:
		      VDP_drawTextBG(APLAN, "          GRAY RAMP", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
		      VDP_drawTextBG(APLAN, "This gray ramp pattern can be", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "used to check color balance.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
		      y++;
		      VDP_drawTextBG(APLAN, "You should make sure the bars", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
		      VDP_drawTextBG(APLAN, "are gray, with no red or blue", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "bias. This can be adjusted with", TILE_ATTR(PAL0, 0, 0, 0), 4, y++); 
		      VDP_drawTextBG(APLAN, "the individual color settings,", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "or the tint control in NTSC", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "displays.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      		      
		      break;      
		    case HELP_WHITE:
		      VDP_drawTextBG(APLAN, "          WHITE SCREEN", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
		      VDP_drawTextBG(APLAN, "This pattern can be changed", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "between white, black, red,", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "green and blue screens with the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "'A' and 'B' buttons.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      y++;     
		      VDP_drawTextBG(APLAN, "Some displays and scalers have", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "issues when changing between a", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "black 0 IRE and a white screen.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
		      y++;
		      VDP_drawTextBG(APLAN, "A custom color mode is", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "available by pressing 'C'.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
		      break; 
		    case HELP_CHECK:    
		    	VDP_drawTextBG(APLAN, "          CHECKERBOARD", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
		      VDP_drawTextBG(APLAN, "This pattern shows all the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "visible pixels in an", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "alternating white and black", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "grid array.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      y++;     
		      VDP_drawTextBG(APLAN, "You can toggle the pattern with", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "button 'B', or turn on", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "auto-toggle each frame with the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
		      VDP_drawTextBG(APLAN, "'A' button. A frame counter is", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
		      VDP_drawTextBG(APLAN, "also available with 'C'.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
		      break;
		    case HELP_STRIPES:
		    	switch(page)
		    	{
		    		case 1:
				    	VDP_drawTextBG(APLAN, "    HORIZONTAL STRIPES (1/2)", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
				      VDP_drawTextBG(APLAN, "You should see a pattern of", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "lines, each one pixel in height", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "starting with a white one.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
				      y++;     
				      VDP_drawTextBG(APLAN, "You can toggle the pattern with", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "button 'B', or turn on", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "auto-toggle each frame with the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
				      VDP_drawTextBG(APLAN, "'A' button. A frame counter is", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
				      VDP_drawTextBG(APLAN, "also available with 'C'.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		   
				      y++;   
				      VDP_drawTextBG(APLAN, "When auto-toggle is set, you", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "should see the lines", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "alternating rapidly. On some", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
				      VDP_drawTextBG(APLAN, "setups, the pattern doesn't", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
				      VDP_drawTextBG(APLAN, "change at all. This means that", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		
				      VDP_drawTextBG(APLAN, "(cont...)", TILE_ATTR(PAL0, 0, 0, 0), 26, 22);	
				      break;
				    case 2:
			      	VDP_drawTextBG(APLAN, "    HORIZONTAL STRIPES (2/2)", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
				      VDP_drawTextBG(APLAN, "the signal is being treated as", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "480i/576i and odd or even frames", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "are being discarded completely.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      				      				      
				      y++;   
				      VDP_drawTextBG(APLAN, "You can also display vertical", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "bars by pressing 'UP'. That", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "pattern will help you ", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
				      VDP_drawTextBG(APLAN, "evaluate if the signal is not", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
				      VDP_drawTextBG(APLAN, "distorted horizontaly, since", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		
				      VDP_drawTextBG(APLAN, "all lines should be one pixel", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);						      
				      VDP_drawTextBG(APLAN, "wide.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		
				      break;
			    }		         
		      break;
		    case HELP_SHADOW:
		    	switch(page)
		    	{
		    		case 1:
				    	VDP_drawTextBG(APLAN, "     DROP SHADOW TEST (1/2)", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
				      VDP_drawTextBG(APLAN, "This is a crucial test for 240p", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "upscan converters. It displays a", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "simple sprite shadow against a", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      				      
				      VDP_drawTextBG(APLAN, "background, but the shadow is", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "shown only on each other frame.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      y++;
				      VDP_drawTextBG(APLAN, "On a CRT this achieves a", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
				      VDP_drawTextBG(APLAN, "transparency effect, since you", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
				      VDP_drawTextBG(APLAN, "are watching a 30hz (25Hz)", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		   				         
				      VDP_drawTextBG(APLAN, "shadow on a 60hz (50hz) signal.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "No background detail should be", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "lost and the shadow should be", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
				      VDP_drawTextBG(APLAN, "visible.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      				      
				      VDP_drawTextBG(APLAN, "(cont...)", TILE_ATTR(PAL0, 0, 0, 0), 26, 22);	
				      break;
				    case 2:
			      	VDP_drawTextBG(APLAN, "     DROP SHADOW TEST (2/2)", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
				      VDP_drawTextBG(APLAN, "The user can toggle the frame", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "used to draw the shadow with", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "button 'A'. Backgrounds can be", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      				      				      				      
				      VDP_drawTextBG(APLAN, "switched with the 'B' button and", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "button 'C' toggles sprites. ", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);				      
				      break;
			    }		         
		      break;
		    case HELP_STRIPED:
		    	VDP_drawTextBG(APLAN, "      STRIPED SPRITE TEST", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
		      VDP_drawTextBG(APLAN, "There are deinterlacers out", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "there that can display the drop", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "shadows correctly and still", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "interpret 240p as 480i. With a", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
		      VDP_drawTextBG(APLAN, "striped sprite it should be easy", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "to tell if a processor tries to", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "deinterlace (plus interpolate).", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      y++;
		      VDP_drawTextBG(APLAN, "You can change backgrounds with", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "'A'.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      break;
		    case HELP_MANUALLAG:
		    	switch(page)
		    	{
		    		case 1:
				    	VDP_drawTextBG(APLAN, "     MANUAL LAG TEST (1/3)", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
				      VDP_drawTextBG(APLAN, "The main intention is to show a", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "changing pattern on the screen,", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "which can be complemented with", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      				      				      				      
				      VDP_drawTextBG(APLAN, "audio. This should show to some", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "degree any lag when processing", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);	
				      VDP_drawTextBG(APLAN, "the signal.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      y++;
				      VDP_drawTextBG(APLAN, "As an added feature, the user", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
				      VDP_drawTextBG(APLAN, "can click the 'A' button when", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
				      VDP_drawTextBG(APLAN, "the sprite is aligned with the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		   				         
				      VDP_drawTextBG(APLAN, "one on the background, and the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "offset in frames from the actual", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "intersection will be shown on", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "screen. This can be repeated ten", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "times and the software will", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "(cont...)", TILE_ATTR(PAL0, 0, 0, 0), 26, 22);	
				      break;
				    case 2:
			      	VDP_drawTextBG(APLAN, "     MANUAL LAG TEST (2/3)", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
				      VDP_drawTextBG(APLAN, "calculate the average. Whenever", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "the button was pressed before", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "the actual intersection frame,", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      				      				      				      
				      VDP_drawTextBG(APLAN, "the result will be ignored, but", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "still shown onscreen. Button 'B'", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "can be used to change the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "direction of the sprite from", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "vertical to horizontal, or both", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "at the same time.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      y++;
				      VDP_drawTextBG(APLAN, "Of course the evaluation is", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "dependent on reflexes and/or", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "rhythm more than anything. The", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "visual and audio cues are the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "more revealing aspects which", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "(cont...)", TILE_ATTR(PAL0, 0, 0, 0), 26, 22);					      
				      break;
				    case 3:
			      	VDP_drawTextBG(APLAN, "     MANUAL LAG TEST (3/3)", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
				      VDP_drawTextBG(APLAN, "the user should consider, of", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "course the interactive factor", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "can give an experienced player", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      				      				      				      
				      VDP_drawTextBG(APLAN, "the hang of the system when", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "testing via different", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "connections.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);				      				  				      
				      break;
			    }		         
		      break;
		    case HELP_HSCROLL:
		    	VDP_drawTextBG(APLAN, "          SCROLL TEST", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
		      VDP_drawTextBG(APLAN, "This test shows a two layer", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "background from Sonic the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "Hedgehog. The speed can be", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "varied with the d-pad, and", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
		      VDP_drawTextBG(APLAN, "change direction with the button", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "'B'. The 'A' button stops the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "scroll.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      y++;
		      VDP_drawTextBG(APLAN, "This can be used to notice any", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "drops in frameratewhen using a", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "video processor.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
		      y++;
		      VDP_drawTextBG(APLAN, "Sonic The Hedgehog is a", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "trademark of Sega Enterprises", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "Ltd.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
		      break;
		    case HELP_VSCROLL:
		    	VDP_drawTextBG(APLAN, "       GRID SCROLL TEST", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
		      VDP_drawTextBG(APLAN, "A grid is scrolled vertically or", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "horizontally, which can be used", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "to test linearity of the signal", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "and how well the display or", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
		      VDP_drawTextBG(APLAN, "video processor copes with", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "scrolling and framerate.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
		      y++;
		      VDP_drawTextBG(APLAN, "Button 'C' can be used to toggle", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "between horizontal and vertical,", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "while the d-pad regulates speed. ", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
		      y++;
		      VDP_drawTextBG(APLAN, "Button 'A' stops the scroll and", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "button 'B' changes direction.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
		      break;
		    case HELP_SOUND:
		    	VDP_drawTextBG(APLAN, "         SOUND TEST", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
		      VDP_drawTextBG(APLAN, "This simple test plays a beep", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "in either channel, or a synth", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "track by shiru in both.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      y++;
		      VDP_drawTextBG(APLAN, "It can be used to determine if", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
		      VDP_drawTextBG(APLAN, "the audio chain is working", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
		      VDP_drawTextBG(APLAN, "properly.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		 		 
		      break;     		      
		    case HELP_LED:
		    	switch(page)
		    	{
		    		case 1:
				    	VDP_drawTextBG(APLAN, "      BACKLIT TEST (1/2)", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
				      VDP_drawTextBG(APLAN, "This test allows you to check", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "how the display's backlit works", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "when only a small array of", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      				      
				      VDP_drawTextBG(APLAN, "pixels is shown. This can be", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "very revealing and can be used", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "to detect the zones in LED", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
				      VDP_drawTextBG(APLAN, "backlit displays, or to evaluate", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
				      VDP_drawTextBG(APLAN, "global and local dimming.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		   
				      y++;   
				      VDP_drawTextBG(APLAN, "The user can move around the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "white pixel arrays with the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "d-pad, and change the size of", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
				      VDP_drawTextBG(APLAN, "the pixel array with 'A' and 'B'", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
				      VDP_drawTextBG(APLAN, "The 'C' button allows the user", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		
				      VDP_drawTextBG(APLAN, "to hide the pixel array in", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		
				      VDP_drawTextBG(APLAN, "(cont...)", TILE_ATTR(PAL0, 0, 0, 0), 26, 22);	
				      break;
				    case 2:
			      	VDP_drawTextBG(APLAN, "      BACKLIT TEST (2/2)", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
				      VDP_drawTextBG(APLAN, "order to alternate a fully black", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "screen.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      y++;
				      VDP_drawTextBG(APLAN, "It might be necessary to have a", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      				      				      				         
				      VDP_drawTextBG(APLAN, "fully-off panel (if possible)", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "when displaying the black", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "background,  in order to", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
				      VDP_drawTextBG(APLAN, "evaluate full-backlit displays", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
				      VDP_drawTextBG(APLAN, "by adjusting brightness", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		
				      VDP_drawTextBG(APLAN, "accordingly for this test.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);						      				      
				      break;
			    }		         
		      break;
		    case HELP_LAG:		    
		    	switch(page)
		    	{
		    		case 1:
				    	VDP_drawTextBG(APLAN, "          LAG TEST (1/2)", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
				      VDP_drawTextBG(APLAN, "This test is designed to be used", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "with two displays conected at", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "the same time. One being a CRT,", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      				      
				      VDP_drawTextBG(APLAN, "or a display with a known lag as", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "reference, and the other the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "display to test.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
				      y++;
				      VDP_drawTextBG(APLAN, "Using a camera, a picture should", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
				      VDP_drawTextBG(APLAN, "be taken of both screens at the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		   				      
				      VDP_drawTextBG(APLAN, "same time. The picture will show", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "the frame discrepancy between", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "them. ", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
				      y++;
				      VDP_drawTextBG(APLAN, "The circles in the bottom help", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		
				      VDP_drawTextBG(APLAN, "determine the frame even when", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		
				      VDP_drawTextBG(APLAN, "(cont...)", TILE_ATTR(PAL0, 0, 0, 0), 26, 22);	
				      break;
				    case 2:
			      	VDP_drawTextBG(APLAN, "          LAG TEST (2/2)", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
				      VDP_drawTextBG(APLAN, "the numbers are blurry.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);				      
				      y++;
				      VDP_drawTextBG(APLAN, "This version of the suite can be", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      				      				      				         
				      VDP_drawTextBG(APLAN, "used with a Sega Nomad as the", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "reference display.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      y++;
				      VDP_drawTextBG(APLAN, "You can also split the video", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      
				      VDP_drawTextBG(APLAN, "signal and feed both displays.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		      				      
				      break;
			    }
			    break;
		    default:
		    	exit = 1;
		    	break; 
		  }		  		
  			
  		redraw = 0;
  	}     	
  	 
    buttons = JOY_readJoypad(JOY_1);
    pressedButtons = buttons & ~oldButtons;
    oldButtons = buttons;    

	if(joytype != JOY_TYPE_PAD6)
	{
		VDP_drawTextBG(APLAN, "Press START to exit help", TILE_ATTR(PAL0, 0, 0, 0), 9, 24);
		VDP_drawTextBG(APLAN, "Show HELP (A):", TILE_ATTR(PAL0, 0, 0, 0), 14, 26);
		VDP_drawTextBG(APLAN, showhelp == 0 ? "No " : "Yes" , TILE_ATTR(PAL1, 0, 0, 0), 29, 26);
			
		if (pressedButtons & BUTTON_START)      	
			exit = 1;
	
		if (pressedButtons & BUTTON_A)
		{
			showhelp = !showhelp;
			redraw = 1;
		}
    }
    else
    {
    	VDP_drawTextBG(APLAN, "Press Z to exit help", TILE_ATTR(PAL0, 0, 0, 0), 11, 24);
    	
    	if (pressedButtons & BUTTON_Z)
			exit = 1;      	      
		if (pressedButtons & BUTTON_START)      	
			exit = 1;
    }
    
    if (pressedButtons & BUTTON_LEFT)
  	{
  		if(page - 1 > 0)
  		{
  			page --;
  			redraw = 1;
  		}
  	}

    if (pressedButtons & BUTTON_RIGHT)
    {      
    	if(page + 1 <= totalpages)
    	{
    		page ++;
    		redraw = 1;
    	}
    }

    VDP_waitVSync();
  }
  VDP_fadeOutAll(FADE_TIME, 0);
  VDP_clearTileMapRect(APLAN, 0, 0, 320/8, 224/8);
  VDP_clearTileMapRect(BPLAN, 0, 0, 320/8, 224/8);
  VDP_resetScreen();
}

