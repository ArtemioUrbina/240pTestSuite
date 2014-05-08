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

#include <stdlib.h>
#include "image.h"
#include "font.h"
#include "video.h"
#include <wiiuse/wpad.h>

#include "patterns.h"

#include "help.h"

void DrawPluge()
{
	int 		done = 0;
	u32			pressed;		
	ImagePtr	back;	
	
	back = LoadImage(PLUGEIMG, 1);
	if(!back)
		return;
			
	while(!done) 
	{		
		WPAD_ScanPads();
		
		pressed = WPAD_ButtonsDown(0);
				
		if (pressed & WPAD_BUTTON_B)
			done =	1;								
	
	//	if (pressed & WPAD_BUTTON_PLUS)
		//	HelpWindow(PLUGEHELP, back);

		StartScene();
		        
		DrawImage(back);
		
        EndScene();

		VIDEO_Flush();
		VIDEO_WaitVSync();
	}
	FreeImage(&back);
	return;
}

void DrawGrayRamp()
{
	int 		done = 0;
	u32			pressed;		
	ImagePtr	back;	
	
	back = LoadImage(GRAYIMG, 1);
	if(!back)
		return;
			
	while(!done) 
	{		
		WPAD_ScanPads();
		
		pressed = WPAD_ButtonsDown(0);
				
		if (pressed & WPAD_BUTTON_B)
			done =	1;								
	
	//	if (pressed & WPAD_BUTTON_PLUS)
		//	HelpWindow(PLUGEHELP, back);

		StartScene();
		        
		DrawImage(back);
		
        EndScene();

		VIDEO_Flush();
		VIDEO_WaitVSync();
	}
	FreeImage(&back);
	return;
}

void DrawWhiteScreen()
{
	int 		done = 0, color = 0;
	u32			pressed;		
	ImagePtr	back;
	
	back = LoadImage(WHITEIMG, 1);
	if(!back)
		return;
		
	while(!done) 
	{
		WPAD_ScanPads();
		
		pressed = WPAD_ButtonsDown(0);
				
		if (pressed & WPAD_BUTTON_B)
			done =	1;						
			
		if (pressed & WPAD_BUTTON_PLUS)
			color ++;

		if (pressed & WPAD_BUTTON_MINUS)
			color --;

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

		StartScene();
		        
		DrawImage(back);
		
        EndScene();

		VIDEO_Flush();
		VIDEO_WaitVSync();
	}
	FreeImage(&back);
	return;
}

/*
void DrawColorBars()
{
	int 				done = 0, type = 0;
	u32			oldbuttons, pressed;		
	ImagePtr		back, backgrid;
	controller	*st;

	oldbuttons = InitController(0);
	back = LoadImage("/rd/color.kmg.gz", 1);
	if(!back)
		return;
	backgrid = LoadImage("/rd/color_grid.kmg.gz", 1);
	if(!back)
		return;
		
	updateVMU("Colorbars", "", 1);
	while(!done) 
	{
		pvr_wait_ready();

		st = ReadController(0);
		if(st)
		{
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
					
			if (pressed & CONT_START)
				done =	1;								

			if (pressed & CONT_A)
				type = !type;

			if(st->rtrig > 5)
				oldbuttons = HelpWindow(COLORBARSHELP, back);
		}

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);
		if(!type)
			DrawImage(back);
		else
			DrawImage(backgrid);
		DrawScanlines();
		pvr_list_finish();				

		pvr_scene_finish();
	}
	FreeImage(&back);
	return;
}

void Draw601ColorBars()
{
	int 				done = 0;
	u32			oldbuttons, pressed;		
	ImagePtr		back;
	controller	*st;

	oldbuttons = InitController(0);
	back = LoadImage("/rd/601701cb.kmg.gz", 1);
	if(!back)
		return;
		
	updateVMU("Colorbars", "with gray", 1);
	while(!done) 
	{
		pvr_wait_ready();

		st = ReadController(0);
		if(st)
		{
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
					
			if (pressed & CONT_START)
				done =	1;								

			if(st->rtrig > 5)
				oldbuttons = HelpWindow(COLORBARSHELP, back);
		}

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);
		DrawImage(back);
		DrawScanlines();
		pvr_list_finish();				

		pvr_scene_finish();
	}
	FreeImage(&back);
	return;
}

void DrawColorBleed()
{
	int 				done = 0, type = 0;
	u32			oldbuttons, pressed;		
	ImagePtr		back, backchk;
	controller	*st;

	oldbuttons = InitController(0);
	back = LoadImage("/rd/colorbleed.kmg.gz", 1);
	if(!back)
		return;
	backchk = LoadImage("/rd/colorbleedchk.kmg.gz", 1);
	if(!backchk)
		return;
		
	updateVMU("Bleed CHK", "", 1);
	while(!done) 
	{
		pvr_wait_ready();

		st = ReadController(0);
		if(st)
		{
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
					
			if (pressed & CONT_START)
				done =	1;								

			if (pressed & CONT_A)
				type = !type;

			if(st->rtrig > 5)
				oldbuttons = HelpWindow(COLORBLEEDHELP, back);
		}

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);
		if(!type)
			DrawImage(back);
		else
			DrawImage(backchk);
		DrawScanlines();
		pvr_list_finish();				

		pvr_scene_finish();
	}
	FreeImage(&back);
	FreeImage(&backchk);
	return;
}

void DrawGrid()
{
	int 			done = 0;
	u32			oldbuttons, pressed;		
	ImagePtr		back;
	controller	*st;

	oldbuttons = InitController(0);
	if(vmode != NATIVE_640_FS)
	{
		back = LoadImage("/rd/grid.kmg.gz", 1);
		if(!back)
			return;
	}
	else
	{
		back = LoadImage("/rd/480/grid-480.kmg.gz", 1);
		if(!back)
			return;
		back->scale = 0;
	}

	updateVMU(" 	Grid	", "", 1);
	while(!done) 
	{
		pvr_wait_ready();

		st = ReadController(0);
		if(st)
		{
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
					
			if (pressed & CONT_START)
				done =	1;											
			if (st->rtrig > 5)
				oldbuttons = HelpWindow(GRIDHELP, back);
		}

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);
		DrawImage(back);		
		DrawScanlines();
		pvr_list_finish();				

		pvr_scene_finish();
	}

	FreeImage(&back);
	return;
}

void DrawLinearity()
{
	int 			done = 0, gridpattern = 0, showgrid = 0;
	u32			oldbuttons, pressed;
	ImagePtr		circles, grid, gridd;
	controller	*st;

	oldbuttons = InitController(0);
	circles = LoadImage("/rd/circles.kmg.gz", 1);
	if(!circles)
		return;
	grid = LoadImage("/rd/circles_grid.kmg.gz", 1);
	if(!grid)
		return;
	gridd = LoadImage("/rd/circles_griddot.kmg.gz", 1);
	if(!gridd)
		return;
	grid->w = 320;
	grid->h = 240;
	gridd->w = 320;
	gridd->h = 240;
		
	updateVMU("Linearity", "", 1);
	while(!done) 
	{
		pvr_wait_ready();

		st = ReadController(0);
		if(st)
		{
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
					
			if (pressed & CONT_A)
				showgrid = !showgrid;
		
			if (pressed & CONT_B)
				gridpattern = !gridpattern;
		
			if (pressed & CONT_START)
				done =	1;				
		}
						
		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);
		if(showgrid)
		{
			if(gridpattern)
				DrawImage(gridd);
			else
				DrawImage(grid);
		}
		DrawImage(circles);
		DrawScanlines();
		pvr_list_finish();				

		pvr_scene_finish();
	}

	FreeImage(&gridd);
	FreeImage(&grid);
	FreeImage(&circles);
	return;
}

void Draw100IRE()
{
	int 			done = 0;
	u32				oldbuttons, pressed, text = 0, invert = 0;	
	ImagePtr		back, white;
	controller		*st;
	char			msg[50];

	oldbuttons = InitController(0);
	back = LoadImage("/rd/100IRE.kmg.gz", 1);
	if(!back)
		return;
  	white = LoadImage("/rd/white.kmg.gz", 1);
	if(!white)
  	{
  		FreeImage(&back);
		return;
  	}
	white->w = 320;
	white->h = 240;

	updateVMU(" 100 IRE ", "", 1);
	while(!done) 
	{
		pvr_wait_ready();

		st = ReadController(0);
		if(st)
		{
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
					
			if (pressed & CONT_A)
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
		
			if (pressed & CONT_B)
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

     			if (pressed & CONT_Y)
      			{
				invert = !invert;
        			back->alpha = 1.0f;
				text = 30;
				if(!invert)
					updateVMU(" 100 IRE ", "", 1);
				else
					updateVMU(" 140 IRE ", "", 1);
      			}
		
			if (pressed & CONT_START)
				done =	1;											
			if (st->rtrig > 5)
				oldbuttons = HelpWindow(IREHELP, back);
		}

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);
    		if(invert)
      			DrawImage(white);	
		DrawImage(back);		

		if(text)
		{
    			if(!invert)
      			{
				sprintf(msg, "%0.0f IRE", (double)(back->alpha * 100));
			  	DrawStringS(265, 225, 1.0f, 1.0f, 1.0f, msg);
			  	text --;
      			}
      			else
      			{
			 	sprintf(msg, "%0.0f IRE", 100.0f + (double)abs(40 - (double)(back->alpha * 40)));
			  	DrawStringS(265, 225, 1.0f, 1.0f, 1.0f, msg);
			  	text --;
      			}
		}

		DrawScanlines();
		pvr_list_finish();				

		pvr_scene_finish();
	}

	FreeImage(&back);
	FreeImage(&white);
	return;
}

*/

