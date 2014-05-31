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
	int 		done = 0;
	uint16		pressed;		
	ImagePtr	back;
	controller	*st;

	back = LoadKMG("/rd/pluge.kmg.gz", 0);
	if(!back)
		return;
		
	updateVMU(" Pluge ", "", 1);
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
				ShowMenu(PLUGEHELP);
		}
	}
	FreeImage(&back);
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
	int 		done = 0, color = 0;
	uint16		pressed;		
	ImagePtr	back;
	controller	*st;

	back = LoadKMG("/rd/white.kmg.gz", 1);
	if(!back)
		return;
		
	updateVMU("White scr", "", 1);
	while(!done && !EndProgram) 
	{
		if(color > 4)
			color = 0;
		if(color < 0)
			color = 4;

		switch(color)
		{
				case 0:
					back->r = 1.0f;
					back->g = 1.0f;
					back->b = 1.0f;
					break;
				case 1:
					back->r = 0.0f;
					back->g = 0.0f;
					back->b = 0.0f;
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

		StartScene();
		DrawImage(back);
		EndScene();

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;								

			if (pressed & CONT_RTRIGGER)
				color ++;

			if (pressed & CONT_LTRIGGER)
				color --;

			if (pressed & CONT_START)
				ShowMenu(WHITEHELP);
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
			if(vmode == VIDEO_480I || vmode == VIDEO_480P)
			{
				back = LoadKMG("/rd/480/grid-480.kmg.gz", 0);
				if(!back)
					return;
				back->scale = 0;		
			}
			
			if(vmode == VIDEO_288P || vmode == VIDEO_576I_A264)
			{
				back = LoadKMG("/rd/gridPAL.kmg.gz", 0);
				if(!back)
					return;        	
			}
			
			if(vmode == VIDEO_576I)
			{
				back = LoadKMG("/rd/480/gridPAL480.kmg.gz", 0);
				if(!back)
					return;        	
			}
			
			if(!back)
			{
				back = LoadKMG("/rd/grid.kmg.gz", 0);
				if(!back)
					return;		
			}
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


