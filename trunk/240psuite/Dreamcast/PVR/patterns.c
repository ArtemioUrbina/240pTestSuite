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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	02111-1307	USA
 */

#include <kos.h>
#include <stdlib.h>
#include <dc/sound/sound.h>
#include <dc/sound/sfxmgr.h>

#include "controller.h"
#include "image.h"
#include "font.h"
#include "vmodes.h"
#include "vmu.h"

#include "patterns.h"

#include "help.h"
#include "menu.h"

void DrawPluge()
{
	int 		done = 0, text = 0, oldvmode = vmode, ShowHelp = 0;
	uint16		pressed;		
	ImagePtr	back = NULL, backPAL, backNTSC, black, highlight;
	controller	*st;
    char		msg[50];

	backNTSC = LoadKMG("/rd/pluge.kmg.gz", 0);
	if(!backNTSC)
		return;

    backPAL = LoadKMG("/rd/plugepal.kmg.gz", 0);
	if(!backPAL)
    {
        FreeImage(&backNTSC);	
		return;
    }
    black = LoadKMG("/rd/white.kmg.gz", 0);
	if(!black)
	{
		FreeImage(&backPAL);
		FreeImage(&backNTSC);
		return;
	}	

    highlight = LoadKMG("/rd/plugehigh.kmg.gz", 0);
	if(!highlight)
	{
		FreeImage(&backPAL);
		FreeImage(&black);
		FreeImage(&backNTSC);
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
		
	updateVMU(" Pluge ", "", 1);
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
			highlight->r = 1.0f;
			highlight->g = 0.0f;
			highlight->b = 0.0f;
			
			highlight->y = 39;
						
			DrawStringB(14, 205, 1.0f, 0.0f, 0, back == backNTSC ? "11.5" : "3.5");
			highlight->x = 14;			
			DrawImage(highlight);
						
			highlight->r = 0.0f;
			highlight->g = 1.0f;							
			DrawStringB(44, 205, 1.0f, 0.0f, 0, back == backNTSC ? "7.5" : "2");
			highlight->x = 44;			
			DrawImage(highlight);
						
			highlight->r = 1.0f;
			highlight->g = 0.0f;							
			DrawStringB(74, 205, 1.0f, 0.0f, 0, back == backNTSC ? "3.5" : "1");
			highlight->x = 74;			
			DrawImage(highlight);
			
			
			highlight->r = 1.0f;
			highlight->g = 0.0f;							
			DrawStringB(228, 205, 1.0f, 0.0f, 0, back == backNTSC ? "3.5" : "1");
			highlight->x = 228;			
			DrawImage(highlight);
						
			highlight->r = 0.0f;
			highlight->g = 1.0f;							
			DrawStringB(259, 205, 1.0f, 0.0f, 0, back == backNTSC ? "7.5" : "2");
			highlight->x = 259;			
			DrawImage(highlight);
			
			highlight->r = 1.0f;
			highlight->g = 0.0f;
			highlight->b = 0.0f;	
			DrawStringB(289, 205, 1.0f, 0.0f, 0, back == backNTSC ? "11.5" : "3.5");
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

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;								

			if (pressed & CONT_START)
				ShowMenu(PLUGEHELP);

            if (pressed & CONT_A)
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
	}
	FreeImage(&highlight);
	FreeImage(&backNTSC);
	FreeImage(&backPAL);
	FreeImage(&black);
	return;
}

void DrawSMPTEColorBars()
{
	int 		done = 0;
	uint16		pressed;		
	ImagePtr	back;
	controller	*st;

	back = LoadKMG("/rd/SMPTEColorBars.kmg.gz", 0);
	if(!back)
		return;
		
	updateVMU(" SMPTE ", "", 1);
	while(!done && !EndProgram) 
	{
		StartScene();
		DrawImage(back);
		EndScene();

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;								

			if (pressed & CONT_START)
				ShowMenu(SMPTECOLOR);
		}
	}
	FreeImage(&back);
	return;
}

void DrawGrayRamp()
{
	int 		done = 0;
	uint16		pressed;		
	ImagePtr	back;
	controller	*st;

	back = LoadKMG("/rd/grayramp.kmg.gz", 0);
	if(!back)
		return;
		
	updateVMU("Gray Ramp", "", 1);
	while(!done && !EndProgram) 
	{
		StartScene();
		DrawImage(back);
		EndScene();

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;								

			if (pressed & CONT_START)
				ShowMenu(GRAYHELP);
		}
	}
	FreeImage(&back);
	return;
}

void DrawWhiteScreen()
{
	int 		done = 0, color = 0, text = 0;
    int			sel = 1, editmode = 0;
    float       BlackLevel = 0.0f, cr, cb, cg;
	uint16		pressed;		
	ImagePtr	back;
	controller	*st;
    char		msg[100], *mode[5] = { "White", "Black", "Red", "Green", "Blue" };

	back = LoadKMG("/rd/white.kmg.gz", 1);
	if(!back)
		return;

    back->w = dW;
	back->h = dH;

    if(!IsPAL)
		BlackLevel = 0.075f; // 7.5 IRE
	else
		BlackLevel = 0.0f; // 0 IRE
		
    cr = cb = cg = 1.0f; // white
	updateVMU("White scr", "", 1);
	while(!done && !EndProgram) 
	{		
        if(IsPAL)
			BlackLevel = 0.0f;

		StartScene();

        DrawImage(back);

        if(text)
		{						
			DrawStringB(200, 20, 1.0f, 1.0f, 1.0f, msg);			
			text --;
		}		
		
		EndScene();

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;								

			if (pressed & CONT_RTRIGGER)
			{
			    color ++;
			    if(color > 4)
				    color = 0;		
			    
			    editmode = 0;
			    if(color == 0 && cr + cb + cg != 3*1.0f)
				    sprintf(msg, "%s [EDITED]", mode[color]);
			    else
				    sprintf(msg, "%s", mode[color]);

                updateVMU("White scr", mode[color], 1);
			    text = 30;
		    }

			if (pressed & CONT_LTRIGGER)
			{			
			    color --;
			    if(color < 0)
				    color = 4;			
				    
			    editmode = 0;
			    if(color == 0 && cr + cb + cg != 3*1.0f)
				    sprintf(msg, "%s [edited]", mode[color]);
			    else
				    sprintf(msg, "%s", mode[color]);
                updateVMU("White scr", mode[color], 1);
			    text = 30;
		    }

			if (pressed & CONT_START)
				ShowMenu(WHITEHELP);

            if (pressed CONT_A && color == 1 && !IsPAL)
		    {
			    if(!BlackLevel)
			    {
				    BlackLevel = 0.075f;
				    sprintf(msg, "#GBlack Level: 7.5 IRE#G");
			    }
			    else
			    {
				    BlackLevel = 0.0f;
				    sprintf(msg, "#GBlack Level: 0 IRE#G");
			    }
			    text = 140;
		    }				
		    
		    if (pressed & CONT_A && color == 0)
			    editmode = !editmode;

            if(editmode)
		    {
			    int *current = NULL;
							    
			    sprintf(msg, "#%cR:%0.2f#W #%cG:%0.2f#W #%cB:x#W", 
					    sel == 1 ? 'G' : 'W', cr,
					    sel == 2 ? 'G' : 'W', cg, 
					    sel == 3 ? 'G' : 'W', cb);
			    text = 1;
    
			    if ( pressed & CONT_DPAD_LEFT )
			    {
				    sel --;
				    if(sel < 1)
					    sel = 3;
			    }
			    
			    if ( pressed & CONT_DPAD_RIGHT )
			    {
				    sel ++;
				    if(sel > 3)
					    sel = 1;
			    }
			    
			    switch(sel)
			    {
				    case 1:
					    current = &cr;
					    break;
				    case 2:
					    current = &cg;
					    break;
				    case 3:
					    current = &cb;
					    break;
			    }
			    
			    if ( pressed & CONT_DPAD_UP )
			    {				
				    if(current)
				    {
					    (*current) + = .05;
					    if(*current > 0xff)
						    *current = 0xff;
				    }
			    }
			    
			    if ( pressed & CONT_DPAD_DOWN )
			    {			
				    if(current)
				    {
					    (*current) -= .05;
					    if(*current < 0)
						    *current = 0;
				    }
			    }	
    
			    if ( pressed & CONT_Y )
			    {				
				    if(current)			
					    *current  = 0.0f;					
			    }
			    
			    if ( pressed &CONT_X )
			    {			
				    if(current)				
					    *current = 1.0f;
			    }	
		    }
		}

		switch(color)
		{
				case 0:
					back->r = cr;
					back->g = cg;
					back->b = cb;
					break;
				case 1:
					back->r = BlackLevel;
					back->g = BlackLevel;
					back->b = BlackLevel;
					break;
				case 2:
					back->r = 1.0f;
					back->g = 0.0f;
					back->b = 0.0f;
					break;
				case 3:
					back->r = 0.0f;
					back->g = 1.0f;
					back->b = 0.0f;
					break;
				case 4:
					back->r = 0.0f;
					back->g = 0.0f;
					back->b = 1.0f;
					break;
		}

	}
	FreeImage(&back);
	return;
}

void DrawColorBars()
{
	int 		done = 0, type = 0;
	uint16		pressed;		
	ImagePtr	back, backgrid;
	controller	*st;

	back = LoadKMG("/rd/color.kmg.gz", 0);
	if(!back)
		return;
	backgrid = LoadKMG("/rd/color_grid.kmg.gz", 0);
	if(!back)
		return;
		
	updateVMU("Colorbars", "", 1);
	while(!done && !EndProgram) 
	{
		StartScene();
		if(!type)
			DrawImage(back);
		else
			DrawImage(backgrid);
		EndScene();

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;								

			if (pressed & CONT_A)
				type = !type;

			if (pressed & CONT_START)
				ShowMenu(COLORBARSHELP);
		}
	}
	FreeImage(&backgrid);
	FreeImage(&back);
	return;
}

void Draw601ColorBars()
{
	int 		done = 0;
	uint16		pressed;		
	ImagePtr	back;
	controller	*st;

	back = LoadKMG("/rd/601701cb.kmg.gz", 0);
	if(!back)
		return;
		
	updateVMU("Colorbars", "with gray", 1);
	while(!done && !EndProgram) 
	{
		StartScene();
		DrawImage(back);
		EndScene();

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;								

			if (pressed & CONT_START)
				ShowMenu(COLOR601);
		}
	}
	FreeImage(&back);
	return;
}

void DrawColorBleed()
{
	int 		done = 0, type = 0;
	uint16		pressed;		
	ImagePtr	back, backchk;
	controller	*st;

	back = LoadKMG("/rd/colorbleed.kmg.gz", 0);
	if(!back)
		return;
	backchk = LoadKMG("/rd/colorbleedchk.kmg.gz", 0);
	if(!backchk)
		return;
		
	updateVMU("Bleed CHK", "", 1);
	while(!done && !EndProgram) 
	{
		StartScene();
		if(!type)
			DrawImage(back);
		else
			DrawImage(backchk);
		EndScene();

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;								

			if (pressed & CONT_A)
				type = !type;

			if (pressed & CONT_START)
				ShowMenu(COLORBLEEDHELP);
		}
	}
	FreeImage(&back);
	FreeImage(&backchk);
	return;
}

void DrawGrid()
{
	int 		done = 0, oldvmode = vmode;
	uint16		pressed;		
	ImagePtr	back = NULL;
	controller	*st;

	updateVMU(" 	Grid	", "", 1);
	while(!done && !EndProgram) 
	{
		if(oldvmode != vmode)
		{
			FreeImage(&back);		
			oldvmode = vmode;
		}
		
		if(!back)
		{
			if(vmode == VIDEO_480I || vmode == VIDEO_480P || vmode == VIDEO_576I)
			{
				back = LoadKMG("/rd/480/grid-480.kmg.gz", 0);
				if(!back)
					return;
				back->scale = 0;		
			}
			
			if(vmode == VIDEO_288P /*|| vmode == VIDEO_576I_A264 */)
			{
				back = LoadKMG("/rd/gridPAL.kmg.gz", 0);
				if(!back)
					return;        	
			}
			
			/*
			if(vmode == VIDEO_576I)
			{
				back = LoadKMG("/rd/480/gridPAL480.kmg.gz", 0);
				if(!back)
					return;        	
			}
			*/
			
			// Use 240p Grid
			if(!back)
			{
				back = LoadKMG("/rd/grid.kmg.gz", 0);
				if(!back)
					return;		
			}

			if(back)
				IgnoreOffset(back);

		}
		
		StartScene();
		DrawImage(back);		
		EndScene();

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;											
			if (pressed & CONT_START)
				ShowMenu(GRIDHELP);
		}
	}

	FreeImage(&back);
	return;
}

void DrawLinearity()
{
	int 		done = 0, oldvmode = vmode, gridpattern = 0, showgrid = 0;
	uint16		pressed;
	ImagePtr	circles = NULL, grid, gridd;
	controller	*st;

	grid = LoadKMG("/rd/circles_grid.kmg.gz", 1);
	if(!grid)
		return;
	gridd = LoadKMG("/rd/circles_griddot.kmg.gz", 1);
	if(!gridd)
		return;
		
	updateVMU("Linearity", "", 1);
	while(!done && !EndProgram) 
	{
		if(oldvmode != vmode)
		{
			FreeImage(&circles);		
			oldvmode = vmode;
		}    
		
		if(!circles)
		{
			if(!IsPAL)
			{
				circles = LoadKMG("/rd/circles.kmg.gz", 0);
				if(!circles)
					return;
			}
			else
			{
				circles = LoadKMG("/rd/circlesPAL.kmg.gz", 0);
				if(!circles)
					return;
				IgnoreOffset(circles);
			}

			CalculateUV(0, 0, dW, dH, grid);
			CalculateUV(0, 0, dW, dH, gridd);
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

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_X)
				showgrid = !showgrid;
		
			if (pressed & CONT_Y)
				gridpattern = !gridpattern;
		
			if (pressed & CONT_B)
				done =	1;				

			if (pressed & CONT_START)
				ShowMenu(LINEARITYHELP);
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
	uint16			pressed, text = 0, invert = 0;	
	ImagePtr		back, white;
	controller		*st;
	char			msg[50];

	back = LoadKMG("/rd/100IRE.kmg.gz", 0);
	if(!back)
		return;
  	white = LoadKMG("/rd/white.kmg.gz", 0);
	if(!white)
  	{
  		FreeImage(&back);
		return;
  	}
	white->w = 320;
	white->h = 240;

	updateVMU(" 100 IRE ", "", 1);
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
					sprintf(msg, "%0.0f IRE", (double)(back->alpha * 100));
			  	DrawStringS(225, 225, 1.0f, 1.0f, 1.0f, msg);
			  	text --;
      			}
      			else
      			{
				if(text > 30)
					sprintf(msg, "RANGE 100-140 IRE");
				else
			 		sprintf(msg, "%0.0f IRE", 100.0f + (double)abs(40 - (double)(back->alpha * 40)));
			  	DrawStringS(225, 225, 1.0f, 1.0f, 1.0f, msg);
			  	text --;
      			}
		}

		EndScene();
		st = ReadController(0, &pressed);
		if(st)
		{
     			if (pressed & CONT_LTRIGGER)
			{
      				if(!invert)
        			{
					back->alpha -= 0.1f;
				  	if(back->alpha < 0.0f)
						back->alpha = 0.0f;
        			}
        			else
        			{
        				back->alpha += 0.125;
				  	if(back->alpha > 1.0f)
						back->alpha = 1.0f;
        			}

				text = 30;
			}
		
     			if (pressed & CONT_RTRIGGER)
			{
      				if(!invert)
        			{
					back->alpha += 0.1f;
				  	if(back->alpha > 1.0f)
						back->alpha = 1.0f;
        			}
        			else
        			{
        				back->alpha -= 0.125f;
				  	if(back->alpha < 0.0f)
						back->alpha = 0.0f;
        			}

				text = 30;
			}

     			if (pressed & CONT_A)
      			{
				invert = !invert;
        			back->alpha = 1.0f;
				text = 60;
				if(!invert)
					updateVMU(" 100 IRE ", "", 1);
				else
					updateVMU(" 140 IRE ", "", 1);
      			}
		
			if (pressed & CONT_B)
				done =	1;											

			if (pressed & CONT_START)
				ShowMenu(IREHELP);
		}
	}

	FreeImage(&back);
	FreeImage(&white);
	return;
}

void DrawSharpness()
{
	int 		done = 0;
	uint16		pressed;		
	ImagePtr	back;	
	controller	*st;
	
	back = LoadKMG("/rd/sharpness.kmg.gz", 0);
	if(!back)
		return;
			
	updateVMU("Sharpness", "", 1);
	while(!done && !EndProgram) 
	{		
		StartScene();
		DrawImage(back);
        	EndScene();
		
		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_START ) 		
				ShowMenu(SHARPNESSHELP);
			
			if (pressed & CONT_B)
				done =	1;										
		}

	}
	FreeImage(&back);
	return;
}

void DrawOverscan()
{
	int 		done = 0, oLeft = 0, oTop = 0, 
			oRight = 0, oBottom = 0, 
			sel = 0, oldvmode = vmode, reset = 0;
	uint16		pressed;		
	ImagePtr	square, border;	
	char		msg[50];
	controller	*st;
	
	square = LoadKMG("/rd/white.kmg.gz", 1);
	if(!square)
		return;
		
	border = CloneImage(square, 1);
	if(!border)
		return;		
	
	border->r = 1.0;
	border->g = 1.0;
	border->b = 1.0;	
	
	square->r = 0.4;
	square->g = 0.4;
	square->b = 0.4;	
			
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
		
		DrawStringS(x-110, y+(fh*sel), 1.0, 1.0, 1.0, ">");
				
		DrawStringS(x-100, y, 1.0, 1.0, 1.0, "Top Overscan:");
		sprintf(msg, "%d pixels (%g%%)", oTop, (double)(oTop*100.0f)/(dH/2));
		DrawStringS(x+20, y, 1.0, 1.0, 1.0, msg);
		
		y+= fh;
		
		DrawStringS(x-100, y, 1.0, 1.0, 1.0, "Bottom Overscan:");
		sprintf(msg, "%d pixels (%g%%)", oBottom, (double)(oBottom*100.0f)/(dH/2));
		DrawStringS(x+20, y, 1.0, 1.0, 1.0, msg);
		
		y+= fh;		
		
		DrawStringS(x-100, y, 1.0, 1.0, 1.0, "Left Overscan:");
		sprintf(msg, "%d pixels (%g%%)", oLeft, (double)(oLeft*100.0f)/(dW/2));
		DrawStringS(x+20, y, 1.0, 1.0, 1.0, msg);
		
		y+= fh;
		
		DrawStringS(x-100, y, 1.0, 1.0, 1.0, "Right Overscan:");
		sprintf(msg, "%d pixels (%g%%)", oRight, (double)(oRight*100.0f)/(dW/2));
		DrawStringS(x+20, y, 1.0, 1.0, 1.0, msg);			
				
		/*
		if(IsPAL && Options.PALScale576)
			DrawStringS(50, 50, 1.0, 1.0, 0x00,
			"When using PAL stretched modes, the pixel\ncount is inaccurate due to vertical scaling.\nRead the #MHELP#M for details");
		*/
			
        	EndScene();
		
		st = ReadController(0, &pressed);
		if(st)
		{
			if(pressed & CONT_START ) 		
				ShowMenu(OVERSCANHELP);
			
			if ( pressed & CONT_DPAD_UP ) 
				sel--;
		
			if ( pressed & CONT_DPAD_DOWN ) 		
				sel++;
			
			if(sel < 0)
				sel = 3;
			if(sel > 3)
				sel = 0;
			
			// Top
			if((pressed & CONT_RTRIGGER && sel == 0) ||
				(st->buttons & CONT_X && sel == 0))
			{
				if(square->y + 1 <= dH/2 && oTop + 1 <= dH/2)
				{				
					square->y++;
					square->h--;
					oTop++;
				}
			}
		
			if((pressed & CONT_LTRIGGER && sel == 0) ||
				(st->buttons & CONT_Y && sel == 0))
			{
				if(square->y - 1 >= 0 && oTop - 1 >= 0)
				{				
					square->y--;
					square->h++;	
					oTop--;
				}
			}
		
			// Bottom
			if((pressed & CONT_RTRIGGER && sel == 1) ||
				(st->buttons & CONT_X && sel == 1))
			{
				if(square->h - 1 >= 0 && oBottom + 1 <= dH/2)
				{								
					square->h--;
					oBottom++;
				}
			}
			
			if((pressed & CONT_LTRIGGER && sel == 1) ||
				(st->buttons & CONT_Y && sel == 1))
			{
				if(square->h + 1 <= dW && oBottom - 1 >=0 )
				{								
					square->h++;	
					oBottom--;
				}
			}
			
			// Left
			if((pressed & CONT_RTRIGGER && sel == 2) ||
				(st->buttons & CONT_X && sel == 2))
			{
				if(square->x + 1 <= dW/2 && oLeft + 1 <= dW/2)
				{				
					square->x++;
					square->w--;
					oLeft++;
				}
			}
		
			if((pressed & CONT_LTRIGGER && sel == 2) ||
				(st->buttons & CONT_Y && sel == 2))
			{
				if(square->x - 1 >= 0 && oLeft - 1 >= 0)
				{				
					square->x--;
					square->w++;
					oLeft--;
				}
			}
		
			// Right
			if((pressed & CONT_RTRIGGER && sel == 3) ||
				(st->buttons & CONT_X && sel == 3))
			{
				if(square->w - 1 >= 0 && oRight + 1 <= dW/2)
				{								
					square->w--;
					oRight++;
				}
			}
		
			if((pressed & CONT_LTRIGGER && sel == 3) ||
				(st->buttons & CONT_Y && sel == 3))
			{
				if(square->w + 1 <= dW && oRight - 1 >= 0)
				{								
					square->w++;	
					oRight--;
				}
			}
					
			if (pressed & CONT_A)	
				reset = 1;
			
			if (pressed & CONT_B)
				done =	1;										
		}
	}
	FreeImage(&border);
	FreeImage(&square);
	return;
}

