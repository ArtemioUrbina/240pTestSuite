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
#include "options.h"
#include "controller.h"

#include "patterns.h"

#include "help.h"
#include "menu.h"

void DrawPluge()
{
	int 		done = 0, text = 0, oldvmode = vmode, ShowHelp = 0;
	u32			pressed;		
	ImagePtr	back = NULL, backPAL, backNTSC, black, highlight;	
	char		msg[50];
	
	backNTSC = LoadImage(PLUGEIMG, 0);
	if(!backNTSC)
		return;
	backPAL = LoadImage(PLUGEPALIMG, 0);
	if(!backPAL)
	{
		FreeImage(&back);	
		return;
	}	
	black = LoadImage(WHITEIMG, 1);
	if(!black)
	{
		FreeImage(&backPAL);
		FreeImage(&back);
		return;
	}	
	highlight = LoadImage(PLUGEBORDERIMG, 0);
	if(!highlight)
	{
		FreeImage(&backPAL);
		FreeImage(&black);
		FreeImage(&back);
		return;
	}	
	
	black->r = 0x0;
	black->g = 0x0;
	black->b = 0x0;	
	
	if(!IsPAL)
	{
		back = backNTSC;
		black->h = 240;
	}
	else
	{
		back = backPAL;
		black->h = 264;	
	}
			
	while(!done && !EndProgram) 
	{
		if(oldvmode != vmode)
		{
			if(!IsPAL)
			{
				back = backNTSC;
				black->h = 240;
			}
			else
			{
				back = backPAL;
				black->h = 264;
			}
			oldvmode = vmode;
		}
				
		StartScene();
		
		DrawImage(black);  
		DrawImage(back);
		
		if(ShowHelp)
		{
			highlight->r = 0xff;
			highlight->g = 0x0;
			highlight->b = 0x0;
			
			highlight->y = 39;
						
			DrawStringB(14, 205, 0xff, 0x00, 0, back == backNTSC ? "11.5" : "3.5");
			highlight->x = 14;			
			DrawImage(highlight);
						
			highlight->r = 0x0;
			highlight->g = 0xff;							
			DrawStringB(44, 205, 0xff, 0x00, 0, back == backNTSC ? "7.5" : "2");
			highlight->x = 44;			
			DrawImage(highlight);
						
			highlight->r = 0xff;
			highlight->g = 0x0;							
			DrawStringB(74, 205, 0xff, 0x00, 0, back == backNTSC ? "3.5" : "1");
			highlight->x = 74;			
			DrawImage(highlight);
			
			
			highlight->r = 0xff;
			highlight->g = 0x0;							
			DrawStringB(228, 205, 0xff, 0x00, 0, back == backNTSC ? "3.5" : "1");
			highlight->x = 228;			
			DrawImage(highlight);
						
			highlight->r = 0x0;
			highlight->g = 0xff;							
			DrawStringB(259, 205, 0xff, 0x00, 0, back == backNTSC ? "7.5" : "2");
			highlight->x = 259;			
			DrawImage(highlight);
			
			highlight->r = 0xff;
			highlight->g = 0x0;
			highlight->b = 0x0;	
			DrawStringB(289, 205, 0xff, 0x00, 0, back == backNTSC ? "11.5" : "3.5");
			highlight->x = 289;			
			DrawImage(highlight);

			ShowHelp--;
		}
		
		
		if(text)
		{			
			DrawStringB(228, 20, 0, 0xff, 0, msg);
			text --;
		}		
		
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
			
		if (pressed & PAD_BUTTON_A)
		{
			if(!IsPAL)
			{
				if(back == backNTSC)
				{
					sprintf(msg, "RGB Full Range");
					back = backPAL;
				}
				else
				{
					sprintf(msg, "NTSC 7.5 IRE");
					back = backNTSC;
				}
					
				text = 60;				
			}
		}
		
		if (pressed & PAD_BUTTON_X)
			ShowHelp = 100;

	}
	FreeImage(&highlight);
	FreeImage(&backNTSC);
	FreeImage(&backPAL);
	FreeImage(&black);
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
	int 		done = 0, color = 0, BlackLevel = 0x00, text = 0;
	u32			pressed;		
	ImagePtr	back;	
	
	back = LoadImage(WHITEIMG, 1);
	if(!back)
		return;
		
	
	back->w = dW;
	back->h = dH;
	
	if(!IsPAL)
		BlackLevel = 0x13; // 7.5 IRE
	else
		BlackLevel = 0x00; // 0 IRE
		
	while(!done && !EndProgram) 
	{		
		if(IsPAL)
			BlackLevel = 0x00;
			
		StartScene();
		        
		DrawImage(back);
		
		if(text && color == 1)
		{			
			if(BlackLevel)
				DrawStringB(200, 20, 0, 0xff, 0, "Black Level: 7.5 IRE");
			else
				DrawStringB(200, 20, 0, 0xff, 0, "Black Level: 0 IRE");
			text --;
		}			
		
        EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
				
		if (pressed & PAD_BUTTON_B)
			done =	1;						
			
		if (pressed & PAD_BUTTON_A && color == 1 && !IsPAL)
		{
			if(!BlackLevel)
				BlackLevel = 0x13;
			else
				BlackLevel = 0x0;
			text = 140;
		}				
			
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
					back->r = BlackLevel;
					back->g = BlackLevel;
					back->b = BlackLevel;
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
	int 		done = 0, is75 = 1, text = 0;
	u32			pressed;		
	ImagePtr	backNTSC75, backNTSC100, backPAL75, backPAL100;
	char		msg[40];
	
	backNTSC75 = LoadImage(SMPTECB75IMG, 0);
	if(!backNTSC75)
		return;
		
	backNTSC100 = LoadImage(SMPTECB100IMG, 0);
	if(!backNTSC75)
	{
		FreeImage(&backNTSC75);
		return;
	}
		
	backPAL75 = LoadImage(EBUCB75IMG, 0);
	if(!backNTSC75)
	{
		FreeImage(&backNTSC75);
		FreeImage(&backNTSC100);
		return;
	}
		
	backPAL100 = LoadImage(EBUCB100IMG, 0);
	if(!backNTSC75)
	{
		FreeImage(&backNTSC75);
		FreeImage(&backNTSC100);
		FreeImage(&backNTSC75);
		return;
	}
	
	IgnoreOffset(backPAL75);
	IgnoreOffset(backPAL100);
			
	while(!done && !EndProgram) 
	{		
		StartScene();
		
		if(!IsPAL)
		{
			if(is75)
				DrawImage(backNTSC75);
			else
				DrawImage(backNTSC100);
		}
		else
		{
			if(is75)
				DrawImage(backPAL75);
			else
				DrawImage(backPAL100);
		}

		if(text)
		{			
			DrawStringB(260, 20, 0, 0xff, 0, msg);
			text --;
		}		
		
        EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
		
		if (pressed & PAD_BUTTON_B)
			done =	1;								
			
		if (pressed & PAD_BUTTON_A)
		{
			is75 = !is75;
			text = 30;
			sprintf(msg, "%s%%", is75 ? "75" : "100");
		}
	
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = SMPTECOLOR;
		}		

	}
	FreeImage(&backNTSC75);
	FreeImage(&backNTSC100);
	FreeImage(&backPAL75);	
	FreeImage(&backPAL100);	
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
	int 		done = 0, oldvmode = vmode, text = 0;
	u32			pressed;		
	ImagePtr	back = NULL;
	char		msg[60];
	
	while(!done && !EndProgram) 
	{		
		if(oldvmode != vmode)
		{
			FreeImage(&back);		
			oldvmode = vmode;
		}
		
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
			
			IgnoreOffset(back);
		}
		
		StartScene();
		        
		DrawImage(back);
		
		if(text)
		{			
			DrawStringB(60, 40, 0, 0xff, 0, msg);
			text --;
		}
		
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
		
		if ( pressed & PAD_BUTTON_UP ) 
		{
			text = 30;
			back->y --;
		}
		if ( pressed & PAD_BUTTON_DOWN ) 		
		{
			text = 30;
			back->y ++;
		}	
		if ( pressed & PAD_BUTTON_LEFT ) 		
		{
			text = 30;
			back->x --;
		}
		if ( pressed & PAD_BUTTON_RIGHT ) 		
		{
			text = 30;
			back->x ++;
		}
		
		if ( pressed & PAD_BUTTON_A ) 		
		{
			text = 30;
			back->x = 0;
			back->y = 0;
		}
		
		if(text)
			sprintf(msg, "Grid origin in video signal: [%d,%d]", (int)back->x, (int)back->y);
	}

	FreeImage(&back);
	return;
}

void DrawLinearity()
{
	int 		done = 0, gridpattern = 0, oldvmode = vmode, showgrid = 0;
	u32			pressed;
	ImagePtr	circles = NULL, grid, gridd;
	

	grid = LoadImage(CIRCLESGRIDIMG, 1);
	if(!grid)
		return;
	gridd = LoadImage(CIRCLESGRIDDOTIMG, 1);
	if(!gridd)
		return;
		
	grid->w = 320;
	grid->h = IsPAL ? 264 : 240;
	gridd->w = 320;
	gridd->h = IsPAL ? 264 : 240;
			
	while(!done && !EndProgram) 
	{    
		if(oldvmode != vmode)
		{
			FreeImage(&circles);		
			oldvmode = vmode;
		}    
		
		if(!circles)
		{
			if(IsPAL)
			{	
				circles = LoadImage(CIRCLESPALIMG, 0);
				if(!circles)
					return;
				IgnoreOffset(circles);
			}
			else
			{
				circles = LoadImage(CIRCLESIMG, 0);
				if(!circles)
					return;
			}
		}
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
	u8				step = 0x01;
	u32				pressed, held, text = 0, invert = 0;	
	ImagePtr		back, white;	
	char			msg[50];
	
	back = LoadImage(IRE100IMG, 0);
	if(!back)
		return;
  	white = LoadImage(WHITEIMG, 1);
	if(!white)
  	{
  		FreeImage(&back);
		return;
  	}	
	
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
				if(text > 30)
					sprintf(msg, "RANGE 0-100 IRE");
				else
					sprintf(msg, "%0.1f IRE", (back->alpha*100.0/255.0));
			  	DrawStringS(225, 225, 0xff, 0xff, 0xff, msg);
			  	text --;
      		}
      		else
      		{
				if(text > 30)
					sprintf(msg, "RANGE 100-140 IRE");
				else
					sprintf(msg, "%0.0f IRE", 100.0f + abs(40 - (back->alpha * 40.0/255.0)));
			  	DrawStringS(225, 225, 0xff, 0xff, 0xff, msg);
			  	text --;
      		}
		}
		EndScene();
		
		ControllerScan();
		
		held = Controller_ButtonsHeld(0);
		pressed = Controller_ButtonsDown(0);
		if (pressed & PAD_TRIGGER_L || held & PAD_BUTTON_Y)
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
	
		if (pressed & PAD_TRIGGER_R || held & PAD_BUTTON_X) 
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
		
     	if (pressed & PAD_BUTTON_A)
      	{
			invert = !invert;
        	back->alpha = 0xff;
			text = 60;			
			if(invert)
				step = 0x04;
			else
				step = 0x01;
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


void DrawOverscan()
{
	int 		done = 0, oLeft = 0, oTop = 0, 
				oRight = 0, oBottom = 0, 
				sel = 0, oldvmode = vmode, reset = 0;
	u32			pressed, held;		
	ImagePtr	square, border;	
	char		msg[50];
	
	square = LoadImage(WHITEIMG, 1);
	if(!square)
		return;
		
	border = CloneImage(square, 1);
	if(!border)
		return;		
	
	border->r = 0xff;
	border->g = 0xff;
	border->b = 0xff;	
	
	square->r = 0x60;
	square->g = 0x60;
	square->b = 0x60;	
			
	while(!done && !EndProgram) 
	{			
		int x = 0, y = 0;
		
		if(oldvmode != vmode || reset)
		{
			oTop = oLeft = oBottom = oRight = 0;
			CalculateUV(0, 0, dW, dH, square);
			CalculateUV(0, 0, dW, dH, border);
			square->x = square->y = 0;			
			oldvmode = vmode;
			reset = 0;
		}		
		
		StartScene();
		        
		DrawImage(border);
		DrawImage(square);	

		x = dW/2;
		y = dH/2-2*fh;
		
		DrawStringS(x-110, y+(fh*sel), 0xff, 0xff, 0xff, ">");
				
		DrawStringS(x-100, y, 0xff, 0xff, 0xff, "Top Overscan:");
		sprintf(msg, "%d pixels (%g%%)", oTop, (oTop*100.0f)/(dH/2));
		DrawStringS(x+20, y, 0xff, 0xff, 0xff, msg);
		
		y+= fh;
		
		DrawStringS(x-100, y, 0xff, 0xff, 0xff, "Bottom Overscan:");
		sprintf(msg, "%d pixels (%g%%)", oBottom, (oBottom*100.0f)/(dH/2));
		DrawStringS(x+20, y, 0xff, 0xff, 0xff, msg);
		
		y+= fh;		
		
		DrawStringS(x-100, y, 0xff, 0xff, 0xff, "Left Overscan:");
		sprintf(msg, "%d pixels (%g%%)", oLeft, (oLeft*100.0f)/(dW/2));
		DrawStringS(x+20, y, 0xff, 0xff, 0xff, msg);
		
		y+= fh;
		
		DrawStringS(x-100, y, 0xff, 0xff, 0xff, "Right Overscan:");
		sprintf(msg, "%d pixels (%g%%)", oRight, (oRight*100.0f)/(dW/2));
		DrawStringS(x+20, y, 0xff, 0xff, 0xff, msg);			
				
		if(IsPAL && Options.PALScale576)
			DrawStringS(50, 50, 0xff, 0xff, 0x00, "When using PAL stretched modes, the pixel\ncount is inaccurate due to vertical scaling.\nRead the #MHELP#M for details");
			
        EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
		held = Controller_ButtonsHeld(0);
				
		if(pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = OVERSCANHELP;
		}
		
		if ( pressed & PAD_BUTTON_UP ) 
			sel--;
		
		if ( pressed & PAD_BUTTON_DOWN ) 		
			sel++;
			
		if(sel < 0)
			sel = 3;
		if(sel > 3)
			sel = 0;
			
		// Top
		if((pressed & PAD_TRIGGER_R && sel == 0) ||
			(held & PAD_BUTTON_X && sel == 0))
		{
			if(square->y + 1 <= dH/2 && oTop + 1 <= dH/2)
			{				
				square->y++;
				square->h--;
				oTop++;
			}
		}
		
		if((pressed & PAD_TRIGGER_L && sel == 0) ||
			(held & PAD_BUTTON_Y && sel == 0))
		{
			if(square->y - 1 >= 0 && oTop - 1 >= 0)
			{				
				square->y--;
				square->h++;	
				oTop--;
			}
		}
		
		// Bottom
		if((pressed & PAD_TRIGGER_R && sel == 1) ||
			(held & PAD_BUTTON_X && sel == 1))
		{
			if(square->h - 1 >= 0 && oBottom + 1 <= dH/2)
			{								
				square->h--;
				oBottom++;
			}
		}
		
		if((pressed & PAD_TRIGGER_L && sel == 1) ||
			(held & PAD_BUTTON_Y && sel == 1))
		{
			if(square->h + 1 <= dW && oBottom - 1 >=0 )
			{								
				square->h++;	
				oBottom--;
			}
		}
		
		// Left
		if((pressed & PAD_TRIGGER_R && sel == 2) ||
			(held & PAD_BUTTON_X && sel == 2))
		{
			if(square->x + 1 <= dW/2 && oLeft + 1 <= dW/2)
			{				
				square->x++;
				square->w--;
				oLeft++;
			}
		}
		
		if((pressed & PAD_TRIGGER_L && sel == 2) ||
			(held & PAD_BUTTON_Y && sel == 2))
		{
			if(square->x - 1 >= 0 && oLeft - 1 >= 0)
			{				
				square->x--;
				square->w++;
				oLeft--;
			}
		}
		
		// Right
		if((pressed & PAD_TRIGGER_R && sel == 3) ||
			(held & PAD_BUTTON_X && sel == 3))
		{
			if(square->w - 1 >= 0 && oRight + 1 <= dW/2)
			{								
				square->w--;
				oRight++;
			}
		}
		
		if((pressed & PAD_TRIGGER_L && sel == 3) ||
			(held & PAD_BUTTON_Y && sel == 3))
		{
			if(square->w + 1 <= dW && oRight - 1 >= 0)
			{								
				square->w++;	
				oRight--;
			}
		}
				
		if (pressed & PAD_BUTTON_A)	
			reset = 1;
		
		if (pressed & PAD_BUTTON_B)
			done =	1;										

	}
	FreeImage(&border);
	FreeImage(&square);
	return;
}


