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

u16  showhelp = 0;

void DrawHelp(int option)
{
  u16 ind = 0, size = 0, exit = 0, redraw = 1, totalpages = 1, page = 1;
  u16 buttons, oldButtons = 0xffff, pressedButtons;

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
  		totalpages = 2;
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
				      VDP_drawTextBG(APLAN, "When enabled, help will be shown", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "after selecting a test/pattern.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      y++;		      	
				      VDP_drawTextBG(APLAN, "The 240p suite is also available", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "in Nintendo GameCube, Wii and", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "Sega Dreamcast.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);				          
				      y++;
				      VDP_drawTextBG(APLAN, "Visit: http://junkerhq.net/240p", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "for more information.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				    	break;
		    	}
		      break;
		    case HELP_PLUGE:      
		    	switch(page)
		    	{
			    	case 1:
				      VDP_drawTextBG(APLAN, "         PLUGE (1/2)", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
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
				      VDP_drawTextBG(APLAN, "(cont...)", TILE_ATTR(PAL0, 0, 0, 0), 26, 22);
				      break;				      
				    case 2:
				    	VDP_drawTextBG(APLAN, "         PLUGE (2/2)", TILE_ATTR(PAL1, 0, 0, 0), 4, 4);
				      VDP_drawTextBG(APLAN, "NTSC levels require black to be", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "at 7.5 IRE for video. This HW", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "can produce 6 IRE (6%), so using", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);      
				      VDP_drawTextBG(APLAN, "this value for general 240p use", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);      				      
				      VDP_drawTextBG(APLAN, "is not recommended.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      y++;
				      VDP_drawTextBG(APLAN, "Of course using it as reference", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "will work perfectly for games", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);				      
				      VDP_drawTextBG(APLAN, "in this platform.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      y++;
				      VDP_drawTextBG(APLAN, "In PAL - and console gaming in", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);
				      VDP_drawTextBG(APLAN, "general - it is adviced to use", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		
				      VDP_drawTextBG(APLAN, "a value of 2 IRE as black.", TILE_ATTR(PAL0, 0, 0, 0), 4, y++);		
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
		      break; 
		    case HELP_CHECK:    
		    case HELP_STRIPES:
		    case HELP_SHADOW:
		    case HELP_STRIPED:
		    case HELP_MANUALLAG:
		    case HELP_HSCROLL:
		    case HELP_VSCROLL:
		    case HELP_SOUND:
		    case HELP_LED:
		    case HELP_LAG:		    
		    default:
		    	exit = 1;
		    	break; 
		  }
		  
		  VDP_drawTextBG(APLAN, "Press START to exit help", TILE_ATTR(PAL0, 0, 0, 0), 9, 24);
  		VDP_drawTextBG(APLAN, "Show HELP (A):", TILE_ATTR(PAL0, 0, 0, 0), 14, 26);
  		VDP_drawTextBG(APLAN, showhelp == 0 ? "No " : "Yes" , TILE_ATTR(PAL1, 0, 0, 0), 29, 26);
  		redraw = 0;
  	}    
    buttons = JOY_readJoypad(JOY_1);
    pressedButtons = buttons & ~oldButtons;
    oldButtons = buttons;

    if (pressedButtons & BUTTON_START)
      exit = 1;

    if (pressedButtons & BUTTON_A)
    {
      showhelp = !showhelp;
      redraw = 1;
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
  VDP_clearTileMapRect(APLAN, 0, 0, 320/8, 224/8);
  VDP_clearTileMapRect(BPLAN, 0, 0, 320/8, 224/8);
}

