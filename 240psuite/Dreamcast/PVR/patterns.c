/* 
 * 240p Test Suite
 * Copyright (C)2011-2022 Artemio Urbina
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
	int 		done = 0, text = 0, ShowHelp = 0, oldvmode = -1;
	uint16		pressed;		
	ImagePtr	back = NULL, backFull = NULL;
	ImagePtr	backNTSC = NULL, black = NULL, highlight = NULL;
	controller	*st = NULL;
	char		msg[50];

	backNTSC = LoadKMG("/rd/pluge.kmg.gz", 0);
	if(!backNTSC)
		return;

	backFull = LoadKMG("/rd/plugepal.kmg.gz", 0);
	if(!backFull)
	{
		FreeImage(&backNTSC);	
		return;
	}
	black = LoadKMG("/rd/black.kmg.gz", 0);
	if(!black)
	{
		FreeImage(&backFull);
		FreeImage(&backNTSC);
		return;
	}	

	highlight = LoadKMG("/rd/PLUGEBorder.kmg.gz", 0);
	if(!highlight)
	{
		FreeImage(&backFull);
		FreeImage(&black);
		FreeImage(&backNTSC);
		return;
	}	

	back = backFull;

	while(!done && !EndProgram) 
	{
		if(oldvmode != vmode)
		{
			black->w = 320;
			black->h = IsPAL ? 264 : 240;
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
						
			DrawStringB(14, 205, 1.0f, 0.0f, 0, back == backNTSC ? "10" : "7");
			highlight->x = 14;
			DrawImage(highlight);
						
			highlight->r = 0.0f;
			highlight->g = 1.0f;
			DrawStringB(44, 205, 1.0f, 0.0f, 0, back == backNTSC ? "7.5" : "4");
			highlight->x = 44;
			DrawImage(highlight);
						
			highlight->r = 1.0f;
			highlight->g = 0.0f;
			DrawStringB(74, 205, 1.0f, 0.0f, 0, back == backNTSC ? "4" : "0");
			highlight->x = 74;
			DrawImage(highlight);
			
			
			highlight->r = 1.0f;
			highlight->g = 0.0f;							
			DrawStringB(228, 205, 1.0f, 0.0f, 0, back == backNTSC ? "4" : "0");
			highlight->x = 228;
			DrawImage(highlight);
						
			highlight->r = 0.0f;
			highlight->g = 1.0f;
			DrawStringB(259, 205, 1.0f, 0.0f, 0, back == backNTSC ? "7.5" : "4");
			highlight->x = 259;
			DrawImage(highlight);
			
			highlight->r = 1.0f;
			highlight->g = 0.0f;
			highlight->b = 0.0f;	
			DrawStringB(289, 205, 1.0f, 0.0f, 0, back == backNTSC ? "10" : "7");
			highlight->x = 289;
			DrawImage(highlight);

			ShowHelp--;
		}
		
		
		if(text)
		{			
			DrawStringB(228, 20, 0, 1.0f, 0, msg);
			text --;
		}		

		EndScene();
		VMURefresh(" Pluge ", "");

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
						back = backFull;
					}
					else
					{
						sprintf(msg, "NTSC 7.5 IRE");
						back = backNTSC;
					}
						
					text = 60;
				}
			}
			
			if (pressed & CONT_X)
				ShowHelp = 120;
		}
	}
	FreeImage(&highlight);
	FreeImage(&backNTSC);
	FreeImage(&backFull);
	FreeImage(&black);
	return;
}

void DrawEBUColorBars()
{
	int 		done = 0, is75 = 0, text = 0;
	uint16		pressed;
	ImagePtr	backEBU75 = NULL, backEBU100 = NULL;
	controller	*st = NULL;
	char		msg[40];

	backEBU75 = LoadKMG("/rd/EBUColorBars75.kmg.gz", 0);
	if(!backEBU75)
		return;

	backEBU100 = LoadKMG("/rd/EBUColorBars100.kmg.gz", 0);
	if(!backEBU100)
	{
		FreeImage(&backEBU75);
		return;
	}

	IgnoreOffset(backEBU75);
	IgnoreOffset(backEBU100);
		
	while(!done && !EndProgram) 
	{
		StartScene();

		if(is75)
			DrawImage(backEBU75);
		else
			DrawImage(backEBU100);

		if(text)
		{
			DrawStringB(260, 20, 0, 1.0f, 0, msg);
			text --;
		}

		EndScene();
		VMURefresh(" EBU ", is75 ? "  75%" : " 100%");

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;

			if (pressed & CONT_A)
			{
				is75 = !is75;
				text = 30;
				sprintf(msg, "%s%%", is75 ? "75" : "100");
				refreshVMU = 1;
			}

			if (pressed & CONT_START)
				ShowMenu(EBUCOLOR);
		}
	}
	FreeImage(&backEBU75);
	FreeImage(&backEBU100);
	return;
}

void DrawSMPTEColorBars()
{
	int 		done = 0, is75 = 0, text = 0;
	uint16		pressed;
	ImagePtr	backNTSC75 = NULL, backNTSC100 = NULL;
	controller	*st = NULL;
	char		msg[40];

	backNTSC75 = LoadKMG("/rd/SMPTEColorBars.kmg.gz", 0);
	if(!backNTSC75)
		return;

	backNTSC100 = LoadKMG("/rd/SMPTEColorBars100.kmg.gz", 0);
	if(!backNTSC100)
	{
		FreeImage(&backNTSC75);
		return;
	}
	
	while(!done && !EndProgram) 
	{
		StartScene();

		if(is75)
			DrawImage(backNTSC75);
		else
			DrawImage(backNTSC100);

		if(text)
		{
			DrawStringB(260, 20, 0, 1.0f, 0, msg);
			text --;
		}

		EndScene();
		VMURefresh("SMPTE", is75 ? "  75%" : " 100%");

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;

			if (pressed & CONT_A)
			{
				is75 = !is75;
				text = 30;
				sprintf(msg, "%s%%", is75 ? "75" : "100");
				refreshVMU = 1;
			}

			if (pressed & CONT_START)
				ShowMenu(SMPTECOLOR);
		}
	}
	FreeImage(&backNTSC75);
	FreeImage(&backNTSC100);
	return;
}

void DrawGrayRamp()
{
	int 		done = 0;
	uint16		pressed;		
	ImagePtr	back = NULL;
	controller	*st = NULL;

	back = LoadKMG("/rd/grayramp.kmg.gz", 0);
	if(!back)
		return;

	while(!done && !EndProgram) 
	{
		StartScene();
		DrawImage(back);
		EndScene();
		
		VMURefresh("Gray Ramp", "");

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
	int			sel = 1, editmode = 0, oldvmode = -1;
	float		BlackLevel = 0.0f, cr, cb, cg;
	uint16		pressed;		
	ImagePtr	back = NULL;
	controller	*st = NULL;
	char		*mode[5] = { "White", "Black", "Red", "Green", "Blue" };
	char		msg[100], *vmuMsg = mode[0], *edited = "Edited";

	back = LoadKMG("/rd/white.kmg.gz", 1);
	if(!back)
		return;
		
	cr = cb = cg = 1.0f; // white
	while(!done && !EndProgram) 
	{	
		if(vmode != oldvmode)
		{
			back->w = dW;
			back->h = dH;
		}
		
		StartScene();

		DrawImage(back);

		if(text)
		{						
			DrawStringB(200, 20, 1.0f, 1.0f, 1.0f, msg);			
			text --;
		}		
		
		EndScene();
		VMURefresh("Color scr", vmuMsg);

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
				{
					sprintf(msg, "%s [EDITED]", mode[color]);
					vmuMsg = edited;
				}
				else
				{
					sprintf(msg, "%s", mode[color]);
					vmuMsg = mode[color];
				}

				refreshVMU = 1;
				text = 30;
			}

			if (pressed & CONT_LTRIGGER)
			{			
				color --;
				if(color < 0)
					color = 4;

				editmode = 0;
				if(color == 0 && cr + cb + cg != 3*1.0f)
				{
					sprintf(msg, "%s [edited]", mode[color]);
					vmuMsg = edited;
				}
				else
				{
					sprintf(msg, "%s", mode[color]);
					vmuMsg = mode[color];
				}
				refreshVMU = 1;
				text = 30;
			}

			if (pressed & CONT_START)
				ShowMenu(WHITEHELP);

			if (pressed & CONT_A && color == 1)
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
				float *current = NULL;

				sprintf(msg, "#%cR:%0.2f#W #%cG:%0.2f#W #%cB:%0.2f#W", 
					sel == 1 ? 'G' : 'W', (double)cr,
					sel == 2 ? 'G' : 'W', (double)cg, 
					sel == 3 ? 'G' : 'W', (double)cb);
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
						(*current) += .01;
						if(*current > 1.0f)
							*current = 1.0f;
					}
				}
				
				if ( pressed & CONT_DPAD_DOWN )
				{			
					if(current)
					{
						(*current) -= .01;
						if(*current < 0)
							*current = 0;
					}
				}	
	
				if ( pressed & CONT_X )
				{				
					if(current)			
						*current  = 0.0f;					
				}
				
				if ( pressed & CONT_Y )
				{			
					if(current)				
						*current = 1.0f;
				}
				
				if(cr + cb + cg != 3*1.0f && vmuMsg != edited)
				{
					vmuMsg = edited;
					refreshVMU = 1;
				}
				
				if(cr + cb + cg == 3*1.0f && vmuMsg == edited)
				{
					vmuMsg = mode[color];
					refreshVMU = 1;
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

	}
	FreeImage(&back);
	return;
}

void DrawColorBars()
{
	int 		done = 0, type = 0, showborder = 0;
	uint16		pressed;		
	ImagePtr	back = NULL, backgrid = NULL, border = NULL;
	controller	*st = NULL;

	back = LoadKMG("/rd/color.kmg.gz", 0);
	if(!back)
		return;
	backgrid = LoadKMG("/rd/color_grid.kmg.gz", 0);
	if(!back)
		return;
	border = LoadKMG("/rd/color_border.kmg.gz", 0);
	if(!border)
		return;
		
	while(!done && !EndProgram) 
	{
		StartScene();
		if(!type)
			DrawImage(back);
		else
			DrawImage(backgrid);
		if(showborder)
		{
			border->r = 1.0f;
			border->g = 0.0f;
			border->b = 1.0f;
			if(type)
			{
				border->x = -21.0f;
				border->y =   3.0f;
			}
			else
			{
				border->x = 0.0f;
				border->y = 0.0f;
			}
			DrawImage(border);
			showborder--;
		}
		EndScene();
		VMURefresh("Colorbars", !type ? "Normal" : "Grid");

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;

			if (pressed & CONT_A)
			{
				type = !type;
				refreshVMU = 1;
			}
			
			if (pressed & CONT_X)
				showborder = 120;

			if (pressed & CONT_START)
				ShowMenu(COLORBARSHELP);
		}
	}
	FreeImage(&border);
	FreeImage(&backgrid);
	FreeImage(&back);
	return;
}

void Draw601ColorBars()
{
	int 		done = 0;
	uint16		pressed;		
	ImagePtr	back = NULL;
	controller	*st = NULL;

	back = LoadKMG("/rd/601701cb.kmg.gz", 0);
	if(!back)
		return;

	while(!done && !EndProgram) 
	{
		StartScene();
		DrawImage(back);
		EndScene();
		VMURefresh("Colorbars", "with gray");

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
	int 		done = 0, type = 0, oldvmode = -1;
	uint16		pressed;
	ImagePtr	back = NULL, backchk = NULL;
	controller	*st = NULL;

	while(!done && !EndProgram) 
	{
		if(oldvmode != vmode)
		{
			if(back)
				FreeImage(&back);		
			if(backchk)
				FreeImage(&backchk);
			oldvmode = vmode;
		}

		if(!back)
		{
			if(vmode >= HIGH_RES)
			{
				back = LoadKMG("/rd/480/colorbleed-480.kmg.gz", 0);
				if(!back)
					return;
				back->scale = 0;
				backchk = LoadKMG("/rd/480/colorbleed-480-chk.kmg.gz", 0);
				if(!backchk)
					return;
				backchk->scale = 0;
			}
			else
			{
				back = LoadKMG("/rd/colorbleed.kmg.gz", 0);
				if(!back)
					return;
				backchk = LoadKMG("/rd/colorbleedchk.kmg.gz", 0);
				if(!backchk)
					return;
			}
		}
		StartScene();
		if(!type)
			DrawImage(back);
		else
			DrawImage(backchk);
		EndScene();
	
		VMURefresh("Bleed CHK", "");

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
	int 		done = 0, oldvmode = -1, border = 0, full = 1;
	uint16		pressed;		
	ImagePtr	back = NULL;
	controller	*st = NULL;
	fmenudata 	resmenudata[] = { {1, "Full 240p"}, {2, "224 sized"} };
	
	if(vmode < HIGH_RES && vmode == VIDEO_240P)
	{
		VMURefresh("Grid", "");
		full = SelectMenu("Select Grid", resmenudata, 2, full);
		if(full == MENU_CANCEL)
			return;
	}
	while(!done && !EndProgram) 
	{
		if(oldvmode != vmode)
		{
			if(back)
				FreeImage(&back);		
			oldvmode = vmode;
		}
		
		if(!back)
		{
			if(vmode >= HIGH_RES)
			{
				back = LoadKMG("/rd/480/grid-480.kmg.gz", 0);
				if(!back)
					return;
				back->scale = 0;
			}
		
			if(vmode == VIDEO_288P)
			{
				back = LoadKMG("/rd/gridPAL.kmg.gz", 0);
				if(!back)
					return;
			}
			
			// Use 240p Grid
			if(!back)
			{
				if(full == 1)
					back = LoadKMG("/rd/grid.kmg.gz", 0);
				else
				{
					back = LoadKMG("/rd/grid224.kmg.gz", 0);
					back->y = 8;
				}
				if(!back)
					return;
			}

			if(back)
				IgnoreOffset(back);
		}
		
		StartScene();
		DrawImage(back);
		EndScene();
		
		if(refreshVMU)
		{
			if(vmode >= HIGH_RES)
				updateVMU("Grid 480", "", 1);
			else
			{
				if(full == 1)
					updateVMU("  Grid  ", "", 1);
				else
					updateVMU("Grid 224", "", 1);
			}
			refreshVMU = 0;
		}

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_A)
			{
				border = !border;
				if(!border)
					vid_border_color(0, 0, 0);
				else
					vid_border_color(255, 255, 255);
			}
			if (pressed & CONT_B)
				done =	1;
			if (pressed & CONT_X)
			{
				if(vmode < HIGH_RES && vmode == VIDEO_240P)
				{
					int type = 0;
					
					type = SelectMenu("Select Grid", resmenudata, 2, full);
					if(type != MENU_CANCEL)
						full = type;
				}
			}
			if (pressed & CONT_START)
			{
				if(border)
				{
					vid_border_color(0, 0, 0);
					border = 0;
				}
				if(full == 0 && vmode == VIDEO_240P)
					ShowMenu(GRID224HELP);
				else
					ShowMenu(GRIDHELP);
			}
		}
	}

	if(border)
		 vid_border_color(0, 0, 0);
	FreeImage(&back);
	return;
}

void DrawMonoscope()
{
	int 		done = 0, oldvmode = -1;
	uint16		pressed;		
	ImagePtr	back = NULL, rlines = NULL, black = NULL;
	controller	*st = NULL;
	char		vmumsg[5];

	black = LoadKMG("/rd/black.kmg.gz", 1);
	if(!black)
		return;
	while(!done && !EndProgram) 
	{
		if(oldvmode != vmode)
		{
			if(back)
				FreeImage(&back);
			if(rlines)
				FreeImage(&rlines);
			black->w = dW;
			black->h = dH;
			oldvmode = vmode;
		}
		
		if(!back)
		{
			if(vmode == VIDEO_240P || vmode == VIDEO_480I_A240 ||
				vmode == VIDEO_480P_SL)
			{
				// Use 240p Monoscope
				if(!back)
				{
					back = LoadKMG("/rd/monoscope.kmg.gz", 0);
					if(!back)
						return;
					rlines = LoadKMG("/rd/monoscope_lin.kmg.gz", 0);
					if(!rlines)
					{
						FreeImage(&back);
						return;
					}
				}
			}
			
			if(vmode == VIDEO_480I || vmode == VIDEO_480P)
			{
				back = LoadKMG("/rd/480/monoscope-480.kmg.gz", 0);
				if(!back)
					return;
				rlines = LoadKMG("/rd/480/monoscope-480_lin.kmg.gz", 0);
				if(!rlines)
				{
					FreeImage(&back);
					return;
				}
				back->scale = 0;
				rlines->scale = 0;
			}
			
			if(vmode == VIDEO_288P || vmode == VIDEO_576I_A264)
			{
				// Use PAL Monoscope
				if(!back)
				{
					back = LoadKMG("/rd/monoscopePAL.kmg.gz", 0);
					if(!back)
						return;
					rlines = LoadKMG("/rd/monoscopePAL_lin.kmg.gz", 0);
					if(!rlines)
					{
						FreeImage(&back);
						return;
					}
				}
			}
			
			if(vmode == VIDEO_576I)
			{
				back = LoadKMG("/rd/480/monoscopePAL576.kmg.gz", 0);
				if(!back)
					return;
				rlines = LoadKMG("/rd/480/monoscopePAL576_lin.kmg.gz", 0);
				if(!rlines)
				{
					FreeImage(&back);
					return;
				}
				back->scale = 0;
				rlines->scale = 0;
			}

			if(back)
			{
				IgnoreOffset(back);
				sprintf(vmumsg, "  %3d%%", FLOAT_TO_INT(back->alpha*100.0f));
				refreshVMU = 1;
			}
			if(rlines)
				IgnoreOffset(rlines);
		}
		
		StartScene();
		DrawImage(black);
		DrawImage(back);
		DrawImage(rlines);
		EndScene();
		
		VMURefresh("Monoscope", vmumsg);

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;

			if (pressed & CONT_START)
				ShowMenu(MONOSCOPEHELP);

			if (pressed & CONT_LTRIGGER)
			{
				back->alpha -= 0.1f;
				if(back->alpha < 0.0f)
					back->alpha = 1.0f;
				sprintf(vmumsg, "  %3d%%", FLOAT_TO_INT(back->alpha*100.0f));
				refreshVMU = 1;
			}
		
			if (pressed & CONT_RTRIGGER)
			{
				back->alpha += 0.1f;
				if(back->alpha > 1.0f)
					back->alpha = 0.0f;
				sprintf(vmumsg, "  %3d%%", FLOAT_TO_INT(back->alpha*100.0f));
				
				refreshVMU = 1;
			}
		}
	}
	FreeImage(&black);
	FreeImage(&back);
	FreeImage(&rlines);
	return;
}

void Draw100IRE()
{
	int 			done = 0, oldvmode = -1;
	uint16			pressed, text = 0, invert = 0;	
	ImagePtr		back = NULL, white = NULL, black = NULL;
	controller		*st = NULL;
	char			msg[50], *vmuMsg = " 100 IRE ";

	black = LoadKMG("/rd/black.kmg.gz", 1);
	if(!black)
		return;
	back = LoadKMG("/rd/100IRE.kmg.gz", 0);
	if(!back)
		return;
  	white = LoadKMG("/rd/white.kmg.gz", 0);
	if(!white)
  	{
  		FreeImage(&back);
		return;
  	}

	while(!done && !EndProgram) 
	{
		if(oldvmode != vmode)
		{
			black->w = 320;
			black->h = 240;
			
			white->w = 320;
			white->h = 240;
			
			oldvmode = vmode;
		}
		
		StartScene();
		DrawImage(black);
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
		VMURefresh(vmuMsg, "");
		
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
					vmuMsg = " 100 IRE ";
				else
					vmuMsg = " 140 IRE ";
				refreshVMU = 0;
			}
		
			if (pressed & CONT_B)
				done =	1;

			if (pressed & CONT_START)
				ShowMenu(IREHELP);
		}
	}

	FreeImage(&back);
	FreeImage(&white);
	FreeImage(&black);
	return;
}

void DrawSharpness()
{
	int 		done = 0, oldvmode = -1, type = 0;
	uint16		pressed;
	ImagePtr	back = NULL, backbricks = NULL;
	controller	*st = NULL;
	
	back = LoadKMG("/rd/sharpness.kmg.gz", 0);
	if(!back)
		return;
	backbricks = LoadKMG("/rd/bricks.kmg.gz", 1);
	if(!backbricks)
		return;

	while(!done && !EndProgram) 
	{
		if(oldvmode != vmode)
		{
			CalculateUV(0, 0, dW, dH, backbricks);
			oldvmode = vmode;
		}
		
		StartScene();
		if(!type)
			DrawImage(back);
		else
			DrawImage(backbricks);
		EndScene();
		
		VMURefresh("Sharpness", !type ? "Normal": "Bricks");
		
		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_START )
				ShowMenu(SHARPNESSHELP);

			if (pressed & CONT_A)
			{
				type = !type;
				refreshVMU = 1;
			}
			if (pressed & CONT_B)
				done =	1;
		}

	}
	FreeImage(&back);
	FreeImage(&backbricks);
	return;
}

void DrawOverscan()
{
	int 		done = 0, oLeft = 0, oTop = 0, 
				oRight = 0, oBottom = 0, 
				sel = 0, oldvmode = -1, reset = 0,
				joycntx = 0, joycnty = 0;
	uint16		pressed;
	ImagePtr	square = NULL, border = NULL;	
	char		msg[50], vmumsg[10];;
	controller	*st = NULL;
	
	square = LoadKMG("/rd/white.kmg.gz", 1);
	if(!square)
		return;
		
	border = LoadKMG("/rd/white.kmg.gz", 1);
	if(!border)
	{
		FreeImage(&square);
		return;		
	}
	
	border->r = 0.7;
	border->g = 0.7;
	border->b = 0.7;
	
	square->r = 0.3;
	square->g = 0.3;
	square->b = 0.3;
	
	refreshVMU = 1;
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
			refreshVMU = 1;
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
		
		if(refreshVMU)
			sprintf(vmumsg, "%dx%d", dW-oRight-oLeft, dH-oTop-oBottom);

		VMURefresh("Overscan", vmumsg);
		
		st = ReadController(0, &pressed);
		if(st)
		{
			if(pressed & CONT_START ) 		
				ShowMenu(OVERSCANHELP);
			
			JoystickDirectios(st, &pressed, &joycntx, &joycnty);
			
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
				(pressed & CONT_DPAD_RIGHT && sel == 0))
			{
				if(square->y + 1 <= dH/2 && oTop + 1 <= dH/2)
				{				
					square->y++;
					square->h--;
					oTop++;
					refreshVMU = 1;
				}
			}
		
			if((pressed & CONT_LTRIGGER && sel == 0) ||
				(pressed & CONT_DPAD_LEFT && sel == 0))
			{
				if(square->y - 1 >= 0 && oTop - 1 >= 0)
				{				
					square->y--;
					square->h++;	
					oTop--;
					refreshVMU = 1;
				}
			}
		
			// Bottom
			if((pressed & CONT_RTRIGGER && sel == 1) ||
				(pressed & CONT_DPAD_RIGHT && sel == 1))
			{
				if(square->h - 1 >= 0 && oBottom + 1 <= dH/2)
				{								
					square->h--;
					oBottom++;
					refreshVMU = 1;
				}
			}
			
			if((pressed & CONT_LTRIGGER && sel == 1) ||
				(pressed & CONT_DPAD_LEFT && sel == 1))
			{
				if(square->h + 1 <= dW && oBottom - 1 >=0 )
				{								
					square->h++;	
					oBottom--;
					refreshVMU = 1;
				}
			}
			
			// Left
			if((pressed & CONT_RTRIGGER && sel == 2) ||
				(pressed & CONT_DPAD_RIGHT && sel == 2))
			{
				if(square->x + 1 <= dW/2 && oLeft + 1 <= dW/2)
				{				
					square->x++;
					square->w--;
					oLeft++;
					refreshVMU = 1;
				}
			}
		
			if((pressed & CONT_LTRIGGER && sel == 2) ||
				(pressed & CONT_DPAD_LEFT && sel == 2))
			{
				if(square->x - 1 >= 0 && oLeft - 1 >= 0)
				{				
					square->x--;
					square->w++;
					oLeft--;
					refreshVMU = 1;
				}
			}
		
			// Right
			if((pressed & CONT_RTRIGGER && sel == 3) ||
				(pressed & CONT_DPAD_RIGHT && sel == 3))
			{
				if(square->w - 1 >= 0 && oRight + 1 <= dW/2)
				{								
					square->w--;
					oRight++;
					refreshVMU = 1;
				}
			}
		
			if((pressed & CONT_LTRIGGER && sel == 3) ||
				(pressed & CONT_DPAD_LEFT && sel == 3))
			{
				if(square->w + 1 <= dW && oRight - 1 >= 0)
				{								
					square->w++;	
					oRight--;
					refreshVMU = 1;
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

#define	NUM_CONV	5
void DrawConvergence()
{
    int 		done = 0, i = 0, current = 0, oldvmode = -1;
	uint16		pressed;	
	ImagePtr	back[NUM_CONV] = { NULL };	
	
	back[0] = LoadKMG("/rd/Convergence-01-grid.kmg.gz", 1);
	if(!back[0])
		return;
	back[1] = LoadKMG("/rd/Convergence-02-cross.kmg.gz", 1);
	if(!back[1])
		return;
	back[2] = LoadKMG("/rd/Convergence-03-dots.kmg.gz", 1);
	if(!back[2])
		return;
	back[3] = LoadKMG("/rd/Convergence-04-colors.kmg.gz", 1);
	if(!back[3])
		return;
	back[4] = LoadKMG("/rd/Convergence-05-colorsbl.kmg.gz", 1);
	if(!back[4])
		return;
		
	current = 0;
	while(!done && !EndProgram) 
	{				
		if(oldvmode != vmode)
		{
			for(i = 0; i < NUM_CONV; i++)
				CalculateUV(0, 0, dW, dH, back[i]);
			oldvmode = vmode;
		}

		StartScene();
		DrawImage(back[current]);
        EndScene();
		
		ReadController(0, &pressed);
				
		if (pressed & CONT_B)
			done =	1;								
	
		if (pressed & CONT_START)
			ShowMenu(DROPSHADOW);
		
		if (pressed & CONT_A || pressed & CONT_RTRIGGER)
		{
			current ++;
			if(current >= NUM_CONV)
				current = 0;
		}
		
		if (pressed & CONT_LTRIGGER)
		{
			current --;
			if(current < 0)
				current = NUM_CONV - 1;
		}
	}
	for(i = 0; i < NUM_CONV; i++)
		if(back[i]) FreeImage(&back[i]);
	return;
}
