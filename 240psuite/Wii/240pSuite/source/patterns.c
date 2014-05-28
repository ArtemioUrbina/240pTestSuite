/* 
 * r
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	02111-1307	USA
 */

#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "font.h"
#include "video.h"
#include "controller.h"

#include "patterns.h"

#include "help.h"
#include "menu.h"

void DrawPluge()
{
	int 		done = 0;
	u32			pressed;		
	ImagePtr	back;	
	
	back = LoadImage(PLUGEIMG, 0);
	if(!back)
		return;
			
	while(!done && !EndProgram) 
	{		
		StartScene();
		        
		DrawImage(back);
		
        EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
				
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = PLUGEHELP;
		}
			
		if (pressed & PAD_BUTTON_B)
			done =	1;										

	}
	FreeImage(&back);
	return;
}

void DrawGrayRamp()
{
	int 		done = 0;
	u32			pressed;		
	ImagePtr	back;	
	
	back = LoadImage(GRAYIMG, 0);
	if(!back)
		return;
			
	while(!done && !EndProgram) 
	{				
		StartScene();
		        
		DrawImage(back);
		
        EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
				
		if (pressed & PAD_BUTTON_B)
			done =	1;								
	
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = GRAYHELP;
		}		
	}
	FreeImage(&back);
	return;
}

void DrawWhiteScreen()
{
	int 		done = 0, color = 0;
	u32		pressed;		
	ImagePtr	back;
	
	back = LoadImage(WHITEIMG, 1);
	if(!back)
		return;
		
	if(vmode == VIDEO_480I || vmode == VIDEO_480P || vmode == VIDEO_576I)
	{
		back->w = 640;
		back->h = 480;
	}
		
	while(!done && !EndProgram) 
	{		
		StartScene();
		        
		DrawImage(back);
		
        EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
				
		if (pressed & PAD_BUTTON_B)
			done =	1;						
			
		if (pressed & PAD_TRIGGER_R)
			color ++;

		if (pressed & PAD_TRIGGER_L)
			color --;
			
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = WHITEHELP;
		}	

		if(color > 4)
			color = 0;
		if(color < 0)
			color = 4;

		switch(color)
		{
				case 0:
					back->r = 0xff;
					back->g = 0xff;
					back->b = 0xff;
					break;
				case 1:
					back->r = 0x00;
					back->g = 0x00;
					back->b = 0x00;
					break;
				case 2:
					back->r = 0xff;
					back->g = 0x00;
					back->b = 0x00;
					break;
				case 3:
					back->r = 0x00;
					back->g = 0xff;
					back->b = 0x00;
					break;
				case 4:
					back->r = 0x00;
					back->g = 0x00;
					back->b = 0xff;
					break;
		}
	}
	FreeImage(&back);
	return;
}

void DrawColorBars()
{
	int 		done = 0, type = 0;
	u32         pressed;		
	ImagePtr	back, backgrid;
	
	back = LoadImage(COLORIMG, 0);
	if(!back)
		return;
	backgrid = LoadImage(COLORGRIDIMG, 0);
	if(!backgrid)
	{
		FreeImage(&back);
		return;
	}
			
	while(!done && !EndProgram) 
	{				
		StartScene();
		        
		if(!type)
			DrawImage(back);
		else
			DrawImage(backgrid);
		
        EndScene();	

		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
		
		if (pressed & PAD_BUTTON_B)
			done =	1;								

		if (pressed & PAD_BUTTON_A)
			type = !type;

		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = COLORBARSHELP;
		}			
	}
	FreeImage(&backgrid);
	FreeImage(&back);
	return;
}

void Draw601ColorBars()
{
	int 		done = 0;
	u32			pressed;		
	ImagePtr	back;
	
	back = LoadImage(CB601701IMG, 0);
	if(!back)
		return;
			
	while(!done && !EndProgram) 
	{			
		StartScene();
		        
		DrawImage(back);
		
        EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
		
		if (pressed & PAD_BUTTON_B)
			done =	1;								
	
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = COLOR601HELP;
		}	
	}
	FreeImage(&back);
	return;
}

void DrawSMPTEColorBars()
{
	int 		done = 0;
	u32			pressed;		
	ImagePtr	back;
	
	back = LoadImage(SMPTECBIMG, 0);
	if(!back)
		return;
			
	while(!done && !EndProgram) 
	{		
		StartScene();
		        
		DrawImage(back);
		
        EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
		
		if (pressed & PAD_BUTTON_B)
			done =	1;								
	
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = SMPTECOLOR;
		}		

	}
	FreeImage(&back);
	return;
}

void DrawColorBleed()
{
	int 		done = 0, type = 0;
	u32			pressed;		
	ImagePtr	back, backchk;
	
	back = LoadImage(COLORBLEEDIMG, 0);
	if(!back)
		return;
	backchk = LoadImage(COLORBLEEDCHKIMG, 0);
	if(!backchk)
		return;
		
	while(!done && !EndProgram) 
	{		
		StartScene();
		        
		if(!type)
			DrawImage(back);
		else
			DrawImage(backchk);
		
        EndScene();		
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
		if (pressed & PAD_BUTTON_B)
			done =	1;								

		if (pressed & PAD_BUTTON_A)
			type = !type;

		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = COLORBLEEDHELP;
		}				
	}
	FreeImage(&back);
	FreeImage(&backchk);
	return;
}

void DrawGrid()
{
	int 		done = 0, oldvmode = vmode;
	u32			pressed;		
	ImagePtr	back = NULL;
	
	while(!done && !EndProgram) 
	{		
		if(oldvmode != vmode)
			FreeImage(&back);		
		
		if(!back)
		{
			if(vmode == VIDEO_480I || vmode == VIDEO_480P)
			{
				back = LoadImage(GRID480IMG, 0);
				if(!back)
					return;
				back->scale = 0;		
			}
			
			if(vmode == VIDEO_288P || vmode == VIDEO_576I_A264)
			{
				back = LoadImage(GRIDPALIMG, 0);
				if(!back)
					return;        	
			}
			
			if(vmode == VIDEO_576I)
			{
				back = LoadImage(GRIDPAL480IMG, 0);
				if(!back)
					return;        	
			}
			
			if(!back)
			{
				back = LoadImage(GRIDIMG, 0);
				if(!back)
					return;		
			}
		}
		
		StartScene();
		        
		DrawImage(back);
		
        EndScene();		
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
		
		if (pressed & PAD_BUTTON_B)
			done =	1;								
	
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = GRIDHELP;
			oldvmode = vmode;
		}						

	}

	FreeImage(&back);
	return;
}

void DrawLinearity()
{
	int 		done = 0, gridpattern = 0, showgrid = 0;
	u32			pressed;
	ImagePtr	circles, grid, gridd;
	
	circles = LoadImage(CIRCLESIMG, 0);
	if(!circles)
		return;
	grid = LoadImage(CIRCLESGRIDIMG, 1);
	if(!grid)
		return;
	gridd = LoadImage(CIRCLESGRIDDOTIMG, 1);
	if(!gridd)
		return;
	grid->w = 320;
	grid->h = Hertz ? 264 : 240;
	gridd->w = 320;
	gridd->h = Hertz ? 264 : 240;
			
	while(!done && !EndProgram) 
	{        
		StartScene();
		        
		if(showgrid)
		{
			if(gridpattern)
				DrawImage(gridd);
			else
				DrawImage(grid);
		}
		DrawImage(circles);
		
        EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
				
		if (pressed & PAD_BUTTON_B)
			done =	1;	

		if (pressed & PAD_TRIGGER_R)
			showgrid = !showgrid;
		
		if (pressed & PAD_TRIGGER_L)
			gridpattern = !gridpattern;
			
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = LINEARITYHELP;
		}	
		
		
	}

	FreeImage(&gridd);
	FreeImage(&grid);
	FreeImage(&circles);
	return;
}

void Draw100IRE()
{
	int 			done = 0;
	u8				step = 0x0a;
	u32				pressed, text = 0, invert = 0;	
	ImagePtr		back, white;	
	char			msg[50];
	
	back = LoadImage(IRE100IMG, 0);
	if(!back)
		return;
  	white = LoadImage(WHITEIMG, 0);
	if(!white)
  	{
  		FreeImage(&back);
		return;
  	}
	white->w = 320;
	white->h = Hertz ? 264 : 240;
	
	while(!done && !EndProgram) 
	{		
		StartScene();
    	if(invert)
      		DrawImage(white);	
		DrawImage(back);		

		if(text)
		{
    		if(!invert)
      		{
				sprintf(msg, "%0.0f IRE", (double)(back->alpha*100/0xff));
			  	DrawStringS(265, 225, 0xff, 0xff, 0xff, msg);
			  	text --;
      		}
      		else
      		{
			 	sprintf(msg, "%0.0f IRE", 100.0f + (double)abs(40 - (double)(back->alpha * 40/0xff)));
			  	DrawStringS(265, 225, 0xff, 0xff, 0xff, msg);
			  	text --;
      		}
		}
		EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
		if (pressed & PAD_TRIGGER_L)
		{
      		if(!invert)
        	{    			
	    		if((int)back->alpha - step <  0x00)
		    		back->alpha = 0x00;
				else
					back->alpha -= step;
        	}
        	else
        	{        		
			    if((int)back->alpha + step > 0xff)
				    back->alpha = 0xff;
				else
					back->alpha += step;
        	}

			text = 30;
		}
	
		if (pressed & PAD_TRIGGER_R)
		{
      		if(!invert)
        	{				
				if((int)back->alpha + step > 0xff)
					back->alpha = 0xff;
				else
					back->alpha += step;
        	}
        	else
        	{        		
				if((int)back->alpha - step < 0x00)
					back->alpha = 0x00;
				else
					back->alpha -= step;
        	}

			text = 30;
		}

     	if (pressed & PAD_BUTTON_X)
      	{
			invert = !invert;
        		back->alpha = 0xff;
			text = 30;			
      	}
	
		if (pressed & PAD_BUTTON_B)
			done =	1;				
							
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = IREHELP;
		}						
	}

	FreeImage(&back);
	FreeImage(&white);
	return;
}

void DrawSharpness()
{
	int 		done = 0;
	u32			pressed;		
	ImagePtr	back;	
	
	back = LoadImage(SHARPNESSIMG, 0);
	if(!back)
		return;
			
	while(!done && !EndProgram) 
	{		
		StartScene();
		        
		DrawImage(back);
		
        EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
				
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = SHARPNESSHELP;
		}
			
		if (pressed & PAD_BUTTON_B)
			done =	1;										

	}
	FreeImage(&back);
	return;
}


