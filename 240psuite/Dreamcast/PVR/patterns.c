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
	int			matchIRE = settings.matchIRE, text_mV = 0;
	uint16		pressed;		
	ImagePtr	back = NULL, backFull = NULL;
	ImagePtr	backNTSC = NULL, black = NULL, highlight = NULL;
	controller	*st = NULL;
	char		msg[50], vmuMsg2[50];
	pallette	pNTSC, pFull;

	backNTSC = LoadKMG("/rd/plt888/pluge.dgz", 0);
	if(!backNTSC)
		return;
		
	if(!load_palette("/rd/plt888/pluge.pal", &pNTSC))
	{
		FreeImage(&backNTSC);
		return;
	}

	backFull = LoadKMG("/rd/plt888/plugepal.dgz", 0);
	if(!backFull)
	{
		release_palette(&pNTSC);
		FreeImage(&backNTSC);	
		return;
	}
	
	if(!load_palette("/rd/plt888/plugepal.pal", &pFull))
	{
		release_palette(&pNTSC);
		FreeImage(&backNTSC);	
		FreeImage(&backFull);	
		return;
	}
	
	black = LoadKMG("/rd/black.kmg.gz", 0);
	if(!black)
	{
		release_palette(&pNTSC);
		release_palette(&pFull);
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
	set_palette(&pFull);
	sprintf(vmuMsg2, matchIRE ? " 714.3 mV" : " 800.0 mV");
	while(!done && !EndProgram) 
	{
		double alpha = 0;
		
		if(oldvmode != vmode)
		{
			black->w = 320;
			black->h = IsPAL ? 264 : 240;
			oldvmode = vmode;
		}

		StartScene();

		DrawImage(black);
		if(matchIRE)
		{
			alpha = back->alpha;
			back->alpha *= MATCH_NTSC_IRE;  
		}
		DrawImage(back);
		if(matchIRE)
			back->alpha = alpha;
		
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
		
		if(text_mV)
		{
			DrawStringS(225, 210, 1.0f, 1.0f, 1.0f, vmuMsg2);
			text_mV--;
		}

		EndScene();
		VMURefresh(" Pluge ", vmuMsg2);

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
						set_palette(&pFull);
					}
					else
					{
						sprintf(msg, "NTSC 7.5 IRE");
						back = backNTSC;
						set_palette(&pNTSC);
					}
						
					text = 60;
				}
			}
			
			if (pressed & CONT_X)
				ShowHelp = 120;
				
			if (pressed & CONT_Y)
			{
				matchIRE = !matchIRE;
				sprintf(vmuMsg2, matchIRE ? " 714.3 mV IRE" : " 800.0 mV IRE");
				text_mV = 60;
				refreshVMU = 1;
			}
		}
	}
	FreeImage(&highlight);
	FreeImage(&black);
	FreeImage(&backNTSC);
	FreeImage(&backFull);
	release_palette(&pNTSC);
	release_palette(&pFull);
	return;
}

void DrawEBUColorBars()
{
	int 		done = 0, is75 = 0, text = 0, oldvmode = -1;
	int			matchIRE = settings.matchIRE, text_mV = 0;
	uint16		pressed;
	ImagePtr	back = NULL, black = NULL;
	controller	*st = NULL;
	char		msg[40], vmuMsg2[50];
	pallette	p75, p100, *pal = NULL;

	back = LoadKMG("/rd/plt888/EBUCB.dgz", 0);
	if(!back)
		return;

	if(!load_palette("/rd/plt888/EBUCB075.pal", &p75))
	{
		FreeImage(&back);
		return;
	}
	
	if(!load_palette("/rd/plt888/EBUCB100.pal", &p100))
	{
		release_palette(&p75);
		FreeImage(&back);
		return;
	}
	
	black = LoadKMG("/rd/black.kmg.gz", 0);
	if(!black)
	{
		release_palette(&p75);
		release_palette(&p100);
		FreeImage(&back);
		return;
	}	

	IgnoreOffset(back);
	
	sprintf(vmuMsg2, matchIRE ? " 714.3 mV" : " 800.0 mV");
	while(!done && !EndProgram) 
	{
		double alpha = 0;
		
		if(oldvmode != vmode)
		{
			black->w = 320;
			black->h = IsPAL ? 264 : 240;
			oldvmode = vmode;
		}
		
		if(!pal)
		{
			pal = is75 ? &p75 : &p100;
			set_palette(pal);
		}
		
		StartScene();

		if(matchIRE)
		{
			alpha = back->alpha;
			back->alpha *= MATCH_NTSC_IRE;  
		}
		DrawImage(black);
		DrawImage(back);
		if(matchIRE)
			back->alpha = alpha;

		if(text)
		{
			DrawStringB(260, 20, 0, 1.0f, 0, msg);
			text --;
		}
		
		if(text_mV)
		{
			DrawStringS(225, 210, 1.0f, 1.0f, 1.0f, vmuMsg2);
			text_mV--;
		}

		EndScene();
		VMURefresh(is75 ? " EBU  75%" : " EBU 100%", vmuMsg2);

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;

			if (pressed & CONT_A)
			{
				pal = NULL;
				is75 = !is75;
				text = 30;
				sprintf(msg, "%s%%", is75 ? "75" : "100");
				refreshVMU = 1;
			}
			
			if (pressed & CONT_Y)
			{
				matchIRE = !matchIRE;
				sprintf(vmuMsg2, matchIRE ? " 714.3 mV IRE" : " 800.0 mV IRE");
				text_mV = 60;
				refreshVMU = 1;
			}

			if (pressed & CONT_START)
				ShowMenu(EBUCOLOR);
		}
	}
	FreeImage(&back);
	release_palette(&p75);
	release_palette(&p100);
	FreeImage(&black);
	return;
}

void DrawSMPTEColorBars()
{
	int 		done = 0, is75 = 0, text = 0, oldvmode = -1;
	int			matchIRE = settings.matchIRE, text_mV = 0;
	uint16		pressed;
	ImagePtr	back = NULL, black = NULL;
	controller	*st = NULL;
	char		msg[40], vmuMsg2[50];
	pallette	p75, p100, *pal = NULL;

	back = LoadKMG("/rd/plt888/SMPTECB.dgz", 0);
	if(!back)
		return;

	if(!load_palette("/rd/plt888/SMPTECB075.pal", &p75))
	{
		FreeImage(&back);
		return;
	}
	
	if(!load_palette("/rd/plt888/SMPTECB100.pal", &p100))
	{
		release_palette(&p75);
		FreeImage(&back);
		return;
	}
	
	black = LoadKMG("/rd/black.kmg.gz", 0);
	if(!black)
	{
		release_palette(&p75);
		release_palette(&p100);
		FreeImage(&back);
		return;
	}	
	
	sprintf(vmuMsg2, matchIRE ? " 714.3 mV" : " 800.0 mV");
	while(!done && !EndProgram) 
	{
		double alpha = 0;
		
		if(oldvmode != vmode)
		{
			black->w = 320;
			black->h = IsPAL ? 264 : 240;
			oldvmode = vmode;
		}
		
		if(!pal)
		{
			pal = is75 ? &p75 : &p100;
			set_palette(pal);
		}
		
		StartScene();

		DrawImage(black);
		if(matchIRE)
		{
			alpha = back->alpha;
			back->alpha *= MATCH_NTSC_IRE;  
		}
		DrawImage(back);
		if(matchIRE)
			back->alpha = alpha;

		if(text)
		{
			DrawStringB(260, 20, 0, 1.0f, 0, msg);
			text --;
		}
		
		if(text_mV)
		{
			DrawStringS(225, 210, 1.0f, 1.0f, 1.0f, vmuMsg2);
			text_mV--;
		}

		EndScene();
		VMURefresh(is75 ? "SMPTE  75%" : "SMPTE 100%", vmuMsg2);

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;

			if (pressed & CONT_A)
			{
				pal = NULL;
				is75 = !is75;
				text = 30;
				sprintf(msg, "%s%%", is75 ? "75" : "100");
				refreshVMU = 1;
			}
			
			if (pressed & CONT_Y)
			{
				matchIRE = !matchIRE;
				sprintf(vmuMsg2, matchIRE ? " 714.3 mV IRE" : " 800.0 mV IRE");
				text_mV = 60;
				refreshVMU = 1;
			}

			if (pressed & CONT_START)
				ShowMenu(SMPTECOLOR);
		}
	}
	release_palette(&p75);
	release_palette(&p100);
	FreeImage(&back);
	FreeImage(&black);
	return;
}

void DrawGrayRamp()
{
	int 		done = 0, oldvmode = -1;
	int			matchIRE = settings.matchIRE, text_mV = 0;
	uint16		pressed;		
	ImagePtr	back = NULL, black = NULL;
	controller	*st = NULL;
	char		vmuMsg2[50];
	pallette	pgray;

	back = LoadKMG("/rd/plt888/grayramp.dgz", 0);
	if(!back)
		return;
		
	if(!load_palette("/rd/plt888/grayramp.pal", &pgray))
	{
		FreeImage(&back);
		return;
	}
		
	black = LoadKMG("/rd/black.kmg.gz", 0);
	if(!black)
	{
		FreeImage(&back);
		return;
	}	

	set_palette(&pgray);
	sprintf(vmuMsg2, matchIRE ? " 714.3 mV" : " 800.0 mV");
	while(!done && !EndProgram) 
	{
		double alpha = 0;
		
		if(oldvmode != vmode)
		{
			black->w = 320;
			black->h = IsPAL ? 264 : 240;
			oldvmode = vmode;
		}
		
		StartScene();
		if(matchIRE)
		{
			alpha = back->alpha;
			back->alpha *= MATCH_NTSC_IRE;  
		}
		DrawImage(black);
		DrawImage(back);
		if(matchIRE)
			back->alpha = alpha;

		if(text_mV)
		{
			DrawStringS(225, 210, 1.0f, 1.0f, 1.0f, vmuMsg2);
			text_mV--;
		}

		EndScene();
		
		VMURefresh("Gray Ramp", vmuMsg2);

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;								
				
			if (pressed & CONT_Y)
			{
				matchIRE = !matchIRE;
				sprintf(vmuMsg2, matchIRE ? " 714.3 mV IRE" : " 800.0 mV IRE");
				text_mV = 60;
				refreshVMU = 1;
			}

			if (pressed & CONT_START)
				ShowMenu(GRAYHELP);
		}
	}
	FreeImage(&back);
	FreeImage(&black);
	release_palette(&pgray);
	return;
}

void DrawWhiteScreen()
{
	int 		done = 0, color = 0, text = 0;
	int			sel = 1, editmode = 0, oldvmode = -1;
	int			matchIRE = settings.matchIRE, text_mV = 0;
	int			cr, cb, cg, BlackLevel = 0;
	uint16		pressed;		
	ImagePtr	back = NULL, black = NULL;
	controller	*st = NULL;
	char		*mode[5] = { "White", "Black", "Red", "Green", "Blue" };
	char		msg[100], *vmuMsg = mode[0], vmuMsg2[50], *edited = "  Edited";

	back = LoadKMG("/rd/white.kmg.gz", 1);
	if(!back)
		return;
	black = LoadKMG("/rd/black.kmg.gz", 0);
	if(!black)
	{
		FreeImage(&back);
		return;
	}	
		
	cr = cb = cg = 0xFF; // white
	sprintf(vmuMsg2, matchIRE ? " 714.3 mV" : " 800.0 mV");
	while(!done && !EndProgram) 
	{	
		double alpha = 0;
		
		if(vmode != oldvmode)
		{
			back->w = dW;
			back->h = dH;
			
			black->w = dW;
			black->h = dH;
			oldvmode = vmode;
		}
		
		StartScene();

		if(matchIRE)
		{
			alpha = back->alpha;
			back->alpha *= MATCH_NTSC_IRE;  
		}
		DrawImage(black);
		DrawImage(back);
		if(matchIRE)
			back->alpha = alpha;

		if(text)
		{						
			DrawStringB(dW - 25*fw, 20, 1.0f, 1.0f, 1.0f, msg);			
			text --;
		}
		
		if(text_mV)
		{
			DrawStringB(dW - 25*fw, 20+fh, 1.0f, 1.0f, 1.0f, vmuMsg2);
			text_mV--;
		}
		
		EndScene();
		VMURefresh(vmuMsg, vmuMsg2);

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
				if(color == 0 && cr + cb + cg != 3*0xff)
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
				if(color == 0 && cr + cb + cg != 3*0xff)
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
					BlackLevel = 0x13;  // 0.075 of 0xFF
					sprintf(msg, "#GBlack Level: 7.5 IRE#G");
				}
				else
				{
					BlackLevel = 0;
					sprintf(msg, "#GBlack Level: 0 IRE#G");
				}
				text = 140;
			}

			if (pressed & CONT_A && color == 0)
				editmode = !editmode;

			if(editmode)
			{
				int *current = NULL;

				sprintf(msg, "#%cR:0x%02x#W #%cG:0x%02x#W #%cB:0x%02x#W", 
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
						(*current) += 1;
						if(*current > 0xff)
							*current = 0xff;
					}
				}
				
				if ( pressed & CONT_DPAD_DOWN )
				{			
					if(current)
					{
						(*current) -= 1;
						if(*current < 0)
							*current = 0;
					}
				}	
	
				if ( pressed & CONT_X )
				{				
					if(current)			
						*current  = 0;					
				}
				
				if ( pressed & CONT_Y )
				{			
					if(current)				
						*current = 0xff;
				}
				
				if(cr + cb + cg != 3*0xff && vmuMsg != edited)
				{
					vmuMsg = edited;
					refreshVMU = 1;
				}
				
				if(cr + cb + cg == 3*0xff && vmuMsg == edited)
				{
					vmuMsg = mode[color];
					refreshVMU = 1;
				}
			}
			else
			{
				if (pressed & CONT_Y)
				{
					matchIRE = !matchIRE;
					sprintf(vmuMsg2, matchIRE ? " 714.3 mV IRE" : " 800.0 mV IRE");
					text_mV = 60;
					refreshVMU = 1;
				}
			}

			switch(color)
			{
				case 0:
					UseDirectColor(back, 0xff, cr, cg, cb);
					break;
				case 1:
					UseDirectColor(back, 0xff, BlackLevel, BlackLevel, BlackLevel);
					break;
				case 2:
					UseDirectColor(back, 0xff, 0xff, 0x00, 0x00);
					break;
				case 3:
					UseDirectColor(back, 0xff, 0x00, 0xff, 0x00);
					break;
				case 4:
					UseDirectColor(back, 0xff, 0x00, 0x00, 0xff);
					break;
			}
		}

	}
	FreeImage(&back);
	FreeImage(&black);
	return;
}

void DrawColorBars()
{
	int 		done = 0, type = 2, showborder = 0, oldvmode = -1;
	int			matchIRE = settings.matchIRE, text_mV = 0;
	uint16		pressed;		
	ImagePtr	back = NULL, backgrid = NULL, backcolor = NULL;
	ImagePtr	border = NULL, black = NULL;
	ImagePtr	color_low = NULL, color_high = NULL;
	controller	*st = NULL;
	char		vmuMsg2[50];
	pallette	pcolor, pcgrid, phigh, plow, *pal = NULL;

	backcolor = LoadKMG("/rd/plt888/color.dgz", 0);
	if(!backcolor)
		return;
	if(!load_palette("/rd/plt888/color.pal", &pcolor))
		return;
		
	backgrid = LoadKMG("/rd/plt888/color_grid.dgz", 0);
	if(!backgrid)
		return;
	if(!load_palette("/rd/plt888/color_grid.pal", &pcgrid))
		return;
		
	color_high = LoadKMG("/rd/plt888/colorhigh.dgz", 0);
	if(!backgrid)
		return;
	if(!load_palette("/rd/plt888/colorhigh.pal", &phigh))
		return;
		
	color_low = LoadKMG("/rd/plt888/colorlow.dgz", 0);
	if(!backgrid)
		return;
	if(!load_palette("/rd/plt888/colorlow.pal", &plow))
		return;

	border = LoadKMG("/rd/color_border.kmg.gz", 0);
	if(!border)
		return;
			
	black = LoadKMG("/rd/black.kmg.gz", 0);
	if(!black)
		return;
	
	sprintf(vmuMsg2, matchIRE ? " 714.3 mV" : " 800.0 mV");
	while(!done && !EndProgram) 
	{
		double alpha = 0;
		
		if(oldvmode != vmode)
		{
			black->w = 320;
			black->h = IsPAL ? 264 : 240;
			oldvmode = vmode;
		}
		
		if(!pal)
		{
			switch(type)
			{
				case 0:
					pal = &pcgrid;
					break;
				case 1:
					pal = &plow;
					break;
				case 2:
					pal = &pcolor;
					break;
				case 3:
					pal = &phigh;
					break;
				default:
					pal = &pcolor;
					type = 2;
					dbglog(DBG_ERROR, "Invalid input received for type and palette\n");
					break;
			}
			set_palette(pal);
		}
		
		StartScene();
		
		if(!back)
		{
			switch(type)
			{
				case 0:
					back = backgrid;
					break;
				case 1:
					back = color_low;
					break;
				case 2:
					back = backcolor;
					break;
				case 3:
					back = color_high;
					break;
				default:
					back = backcolor;
					type = 2;
					dbglog(DBG_ERROR, "Invalid input received for type and palette\n");
					break;
			}
		}
		
		if(matchIRE)
		{
			alpha = back->alpha;
			back->alpha *= MATCH_NTSC_IRE;  
		}
		DrawImage(black);
		DrawImage(back);
		if(matchIRE)
			back->alpha = alpha;

		if(text_mV)
		{
			DrawStringS(225, 210, 1.0f, 1.0f, 1.0f, vmuMsg2);
			text_mV--;
		}

		if(showborder)
		{
			border->r = 1.0f;
			border->g = 0.0f;
			border->b = 1.0f;
			if(!type)
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
		VMURefresh("Colorbars", vmuMsg2);

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;

			if (pressed & CONT_A)
			{
				if(!type)
					type = 2;
				else
					type = 0;
				
				back = NULL;
				pal = NULL;
				refreshVMU = 1;
			}
			
			if(type)
			{
				if (pressed & CONT_LTRIGGER)
				{
					type --;
					if(type < 1)
						type = 1;
					back = NULL;
					pal = NULL;
					refreshVMU = 1;
				}
				
				if (pressed & CONT_RTRIGGER)
				{
					type ++;
					if(type > 3)
						type = 3;
					back = NULL;
					pal = NULL;
					refreshVMU = 1;
				}
			}
			
			if (pressed & CONT_X)
				showborder = 120;
				
			if (pressed & CONT_Y)
			{
				matchIRE = !matchIRE;
				sprintf(vmuMsg2, matchIRE ? " 714.3 mV IRE" : " 800.0 mV IRE");
				text_mV = 60;
				refreshVMU = 1;
			}

			if (pressed & CONT_START)
				ShowMenu(COLORBARSHELP);
		}
	}
	FreeImage(&border);
	
	release_palette(&plow);
	FreeImage(&color_low);
	
	release_palette(&phigh);
	FreeImage(&color_high);
	
	release_palette(&pcgrid);
	FreeImage(&backgrid);
	
	release_palette(&pcolor);
	FreeImage(&backcolor);
	
	FreeImage(&black);
	
	return;
}

void Draw601ColorBars()
{
	int 		done = 0, oldvmode = -1;
	int			matchIRE = settings.matchIRE, text_mV = 0;
	uint16		pressed;		
	ImagePtr	back = NULL, black = NULL;
	controller	*st = NULL;
	char		vmuMsg2[50];
	pallette	p601;

	back = LoadKMG("/rd/plt888/601701cb.dgz", 0);
	if(!back)
		return;
		
	if(!load_palette("/rd/plt888/601701cb.pal", &p601))
	{
		FreeImage(&back);
		return;
	}
		
	black = LoadKMG("/rd/black.kmg.gz", 0);
	if(!black)
	{
		FreeImage(&back);
		return;
	}	

	set_palette(&p601);
	sprintf(vmuMsg2, matchIRE ? " 714.3 mV" : " 800.0 mV");
	while(!done && !EndProgram) 
	{
		double alpha = 0;
		
		if(oldvmode != vmode)
		{
			black->w = 320;
			black->h = IsPAL ? 264 : 240;
			oldvmode = vmode;
		}
		
		StartScene();
		if(matchIRE)
		{
			alpha = back->alpha;
			back->alpha *= MATCH_NTSC_IRE;  
		}
		DrawImage(black);
		DrawImage(back);
		if(matchIRE)
			back->alpha = alpha;
		if(text_mV)
		{
			DrawStringS(225, 210, 1.0f, 1.0f, 1.0f, vmuMsg2);
			text_mV--;
		}
		EndScene();
		VMURefresh("Gray CB", vmuMsg2);

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;
			
			if (pressed & CONT_Y)
			{
				matchIRE = !matchIRE;
				sprintf(vmuMsg2, matchIRE ? " 714.3 mV IRE" : " 800.0 mV IRE");
				text_mV = 60;
				refreshVMU = 1;
			}

			if (pressed & CONT_START)
				ShowMenu(COLOR601);
		}
	}
	FreeImage(&back);
	FreeImage(&black);
	release_palette(&p601);
	return;
}

void DrawColorBleed()
{
	int 		done = 0, type = 0, oldvmode = -1;
	int			matchIRE = settings.matchIRE, text_mV = 0;
	uint16		pressed;
	ImagePtr	back = NULL, backchk = NULL, backnrm = NULL, black = NULL;
	controller	*st = NULL;
	char		vmuMsg2[50];

	black = LoadKMG("/rd/black.kmg.gz", 0);
	if(!black)
		return;

	sprintf(vmuMsg2, matchIRE ? " 714.3 mV" : " 800.0 mV");
	while(!done && !EndProgram) 
	{
		double alpha = 0;
		
		if(oldvmode != vmode)
		{
			black->w = dW;
			black->h = dH;
			
			if(backnrm)
				FreeImage(&backnrm);		
			if(backchk)
				FreeImage(&backchk);
			oldvmode = vmode;
		}

		if(!back)
		{
			if(vmode >= HIGH_RES)
			{
				backnrm = LoadKMG("/rd/480/colorbleed-480.kmg.gz", 0);
				if(!backnrm)
					return;
				backnrm->scale = 0;
				backchk = LoadKMG("/rd/480/colorbleed-480-chk.kmg.gz", 0);
				if(!backchk)
					return;
				backchk->scale = 0;
			}
			else
			{
				backnrm = LoadKMG("/rd/colorbleed.kmg.gz", 0);
				if(!backnrm)
					return;
				backchk = LoadKMG("/rd/colorbleedchk.kmg.gz", 0);
				if(!backchk)
					return;
			}
		}
		StartScene();
		if(!type)
			back = backnrm;
		else
			back = backchk;
		if(matchIRE)
		{
			alpha = back->alpha;
			back->alpha *= MATCH_NTSC_IRE;  
		}
		DrawImage(black);
		DrawImage(back);
		if(matchIRE)
			back->alpha = alpha;
		if(text_mV)
		{
			DrawStringS(dW-20*fw, dH-5*fh, 1.0f, 1.0f, 1.0f, vmuMsg2);
			text_mV--;
		}
		EndScene();
	
		VMURefresh("Bleed CHK", vmuMsg2);

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;

			if (pressed & CONT_A)
				type = !type;
				
			if (pressed & CONT_Y)
			{
				matchIRE = !matchIRE;
				sprintf(vmuMsg2, matchIRE ? " 714.3 mV IRE" : " 800.0 mV IRE");
				text_mV = 60;
				refreshVMU = 1;
			}

			if (pressed & CONT_START)
				ShowMenu(COLORBLEEDHELP);
		}
	}
	FreeImage(&backnrm);
	FreeImage(&backchk);
	FreeImage(&black);
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
			if(vmode >= HIGH_RES) {
				VMURefresh("Grid 480", "");
			}
			else
			{
				if(full == 1) {
					VMURefresh("  Grid  ", "");
				}
				else {
					VMURefresh("Grid 224", "");
				}
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

#define	IRE_MAX 255.0f

void Draw100IRE()
{
	int 			done = 0, oldvmode = -1, vmuMsgSend = 0;
	int				changed_palette = 1, step_fraction = 0;
	float			ire_level = IRE_MAX, ire_step = 255.0f/10.0f;
	float			black_level = 0.0f, *change_level = NULL;
	int				matchIRE = settings.matchIRE, text_mV = 0;
	uint16			pressed, text = 0, invert = 0;	
	ImagePtr		back = NULL, white = NULL, black = NULL;
	controller		*st = NULL;
	char			msg[50], vmuMsg[50], vmuMsg2[50];

	black = LoadKMG("/rd/black.kmg.gz", 1);
	if(!black)
		return;
	back = LoadKMG("/rd/plt888/100IRE.dgz", 0);
	if(!back)
	{
		FreeImage(&black);
		return;
  	}
  	white = LoadKMG("/rd/white.kmg.gz", 0);
	if(!white)
  	{
		FreeImage(&black);
  		FreeImage(&back);
		return;
  	}

	sprintf(vmuMsg, " 100 IRE ");
	sprintf(vmuMsg2, matchIRE ? " 714.3 mV" : " 800.0 mV");
	change_level = &ire_level;
	while(!done && !EndProgram) 
	{
		double alpha = 0;
		
		if(changed_palette)
		{
			// set up an ARGB8888 palette:
			pvr_set_pal_format(PVR_PAL_ARGB8888);
			pvr_set_pal_entry(0, PACK_ARGB8888(255, 
					(int)black_level, 
					(int)black_level, 
					(int)black_level));
			pvr_set_pal_entry(1, PACK_ARGB8888(0, 0, 0, 0));	// transparent
			pvr_set_pal_entry(2, PACK_ARGB8888(255,				// solid selected color
					(int)ire_level, 
					(int)ire_level, 
					(int)ire_level));
			changed_palette = 0;
		}
		
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
		if(matchIRE)
		{
			alpha = back->alpha;
			back->alpha *= MATCH_NTSC_IRE;  
		}
		DrawImage(back);
		if(matchIRE)
			back->alpha = alpha;

		if(text)
		{
			if(!invert)
			{
				if(text > 30)
					sprintf(msg, "RANGE 0-100 IRE");
				else
					sprintf(msg, "%0.0f IRE", (double)((ire_level * 100.0f)/IRE_MAX));
				DrawStringS(225, 225, 1.0f, 1.0f, 1.0f, msg);
				text --;
			}
			else
			{
				if(text > 30)
					sprintf(msg, "RANGE 100-140 IRE");
				else
					sprintf(msg, "%0.0f IRE", (double)(100.0f + (black_level * 40.0f)/IRE_MAX));
				DrawStringS(225, 225, 1.0f, 1.0f, 1.0f, msg);
				text --;
			}
		}
		
		if(text_mV)
		{
			DrawStringS(225, 210, 1.0f, 1.0f, 1.0f, vmuMsg2);
			text_mV--;
		}

		EndScene();
		VMURefresh(vmuMsg, vmuMsg2);
		
		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_LTRIGGER)
			{
				*change_level -= ire_step;
				if(*change_level <= 0.0f)
					*change_level = 0.0f;
				changed_palette = 1;
				text = 30;
				vmuMsgSend = 1;
			}
		
			if (pressed & CONT_RTRIGGER)
			{
				*change_level += ire_step;
				if(*change_level > IRE_MAX)
					*change_level = IRE_MAX;
				changed_palette = 1;
				text = 30;
				vmuMsgSend = 1;
			}

			if (pressed & CONT_A)
			{
				invert = !invert;
				if(!invert)
					change_level = &ire_level;
				else
					change_level = &black_level;
				ire_level = IRE_MAX;
				black_level = 0.0f;
				text = 60;
				vmuMsgSend = 1;
				changed_palette = 1;
			}
			
			if(vmuMsgSend)
			{
				if(!invert)
					sprintf(vmuMsg, " %3.0f IRE", (double)((ire_level * 100.0f)/IRE_MAX));
				else
					sprintf(vmuMsg, " %3.0f IRE", (double)(100.0f + (black_level * 40.0f)/IRE_MAX));
				refreshVMU = 1;
				vmuMsgSend = 0;
			}
		
			if (pressed & CONT_B)
				done =	1;
			
			if (pressed & CONT_X)	
			{
				step_fraction++;
				if(step_fraction > 3)
					step_fraction = 0;
				switch(step_fraction)
				{
					case 0:	// 10 IRE
						ire_step = 255.0f/10.0f;
						break;
					case 1: // 05 IRE
						ire_step = 255.0f/20.0f;
						break;
					case 2: // 02 IRE
						ire_step = 255.0f/50.0f;
						break;
					case 3:	// 01 IRE
						ire_step = 255.0f/100.0f;
						break;
				}
			}
			
			if (pressed & CONT_Y)
			{
				matchIRE = !matchIRE;
				sprintf(vmuMsg2, matchIRE ? " 714.3 mV IRE" : " 800.0 mV IRE");
				text_mV = 60;
				refreshVMU = 1;
			}

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
			
			JoystickDirections(st, &pressed, &joycntx, &joycnty);
			
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
		
		VMURefresh("Conver-", "    gence");
		
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

hcfr_file *listCSVFiles(char *path, int *num_files)
{
	int			filecount = 0, maxlen = 0, i = 0;
	file_t		d;
	dirent_t 	*dir;
	hcfr_file	*filenames = NULL;
	
	if(!num_files)
		return NULL;

	*num_files = 0;
	d = fs_open(path, O_RDONLY  | O_DIR);
	if(d)
	{
		dir = fs_readdir(d);
		while(dir)
		{
			int len = 0;
			
			len = strlen(dir->name);
			if(len > 4)
			{
				if( dir->name[len - 3] == 'c' && 
					dir->name[len - 2] == 's' &&
					dir->name[len - 1] == 'v')
				{
					filecount++;
					if(len > maxlen)
						maxlen = len;
				}
			}
			dir = fs_readdir(d);
		}
	}
	fs_close(d);
	
	if(!filecount)
		return NULL;

	filenames = (hcfr_file*)malloc(sizeof(hcfr_file)*filecount);
	if(!filenames)
		return NULL;

	memset(filenames, 0, sizeof(hcfr_file)*filecount);
	maxlen += strlen(path) + 1;	// add path
	for(i = 0; i < filecount; i++)
	{
		filenames[i].dispname = (char*)malloc(sizeof(char)*maxlen);
		if(!filenames[i].dispname)
		{
			int pos = 0;
			
			for(pos = 0; pos < i; pos ++)
			{
				free(filenames[pos].dispname);
				free(filenames[pos].filename);
			}
			free(filenames);
			return NULL;
		}
		memset(filenames[i].dispname, 0, sizeof(char)*maxlen);
		
		filenames[i].filename = (char*)malloc(sizeof(char)*maxlen);
		if(!filenames[i].filename)
		{
			int pos = 0;
			
			free(filenames[i].dispname);
			for(pos = 0; pos < i; pos ++)
			{
				free(filenames[pos].dispname);
				free(filenames[pos].filename);
			}
			free(filenames);
			return NULL;
		}
		memset(filenames[i].filename, 0, sizeof(char)*maxlen);
	}
	
	i = 0;
	// store the values
	d = fs_open(path, O_RDONLY | O_DIR);
	if(d)
	{
		dir = fs_readdir(d);
		while(dir)
		{
			int len = 0;
			
			len = strlen(dir->name);
			if(len > 7)
			{
				if( dir->name[len - 3] == 'c' && 
					dir->name[len - 2] == 's' &&
					dir->name[len - 1] == 'v')
				{
					int l_len = 0, pos = 0;
					
					// remove leading sort number and extension
					memcpy(filenames[i].dispname, dir->name+3, sizeof(char)*(len - 7)); 
					l_len = strlen(filenames[i].dispname);
					for(pos = 0; pos < l_len; pos++)
					{
						if(filenames[i].dispname[pos] == '_')
							filenames[i].dispname[pos] = ' ';
					}
					sprintf(filenames[i].filename, "%s%s", path, dir->name);
					i++;
				}
			}
			dir = fs_readdir(d);
		}
	}
	fs_close(d);
	
	*num_files = filecount;
	return(filenames);
}

void releaseCSVList(hcfr_file **listCSVFiles, int num_files)
{
	int pos = 0;
			
	for(pos = 0; pos < num_files; pos ++)
	{
		free((*listCSVFiles)[pos].dispname);
		free((*listCSVFiles)[pos].filename);
		(*listCSVFiles)[pos].dispname = NULL;
		(*listCSVFiles)[pos].filename = NULL;
	}
	free(*listCSVFiles);
	*listCSVFiles = NULL;
}

hcfr_color *LoadHCFR(char *filename, int *hcfr_num)
{
	uint 		size = 0;
	FILE 		*fp = NULL;
	int			hcfr_count = 0;
	uint		i = 0, lines = 0, current_line = 0;
	uint		max_line_len = 0,line_len = 0;
	char		*buffer = NULL, *lineBuffer = NULL, *last_csv = NULL;
	hcfr_color 	*hcfr_colors = NULL;
	
	if(!hcfr_num)
		return NULL;

	*hcfr_num = 0;
	fp = fopen(filename, "r");
	if(!fp)
	{
		dbglog(DBG_ERROR, "Could not load %s HCFR file\n", filename);
		return NULL;
	}
	fseek(fp, 0L, SEEK_END);
	size = ftell(fp)+1;
	buffer = (char*)malloc(sizeof(char)*size);
	if(!buffer)
	{
		fclose(fp);
		dbglog(DBG_ERROR, "Could not reserve space for %s CSV HCFR in RAM\n", filename);
		return NULL;
	}
	fseek(fp, 0L, SEEK_SET);
	memset(buffer, 0x0, sizeof(char)*size);
	if(fread(buffer, sizeof(char), size-1, fp) != size-1)
	{
		free(buffer);
		fclose(fp);
		dbglog(DBG_ERROR, "Could not load %s CSV HCFR file to RAM\n", filename);
		return NULL;
	}
	fclose(fp);
	
	for(i = 0; i < size; i++)
	{
		line_len++;
		
		if(buffer[i] == '\n')
		{
			lines ++;
			if(line_len > max_line_len)
				max_line_len = line_len;
			line_len = 0;
		}
	}
	
	hcfr_count = lines - 1; // we remove the header
	if(hcfr_count <= 0)
	{
		dbglog(DBG_ERROR, "File %s has no HCFR color data\n", filename);
		free(buffer);
		buffer = NULL;
		return NULL;
	}
	hcfr_colors = (hcfr_color*)malloc(sizeof(hcfr_color)*hcfr_count);
	if(!hcfr_colors)
	{
		dbglog(DBG_ERROR, "Could not malloc array of hcfr_colors %s\n", filename);
		free(buffer);
		buffer = NULL;
		return NULL;
	}
	memset(hcfr_colors, 0, sizeof(hcfr_color)*hcfr_count);
	
	lineBuffer = (char*)malloc(sizeof(char)*max_line_len);
	if(!lineBuffer)
	{
		free(buffer);
		free(hcfr_colors);
		dbglog(DBG_ERROR, "Could not load %s CSV HCFR file to RAM\n", filename);
		return NULL;
	}
	
	// parse the CSV from RAM
	last_csv = buffer;
	for(i = 0; i < size; i++)
	{
		line_len++;
		if(buffer[i] == '\n')
		{
			if(current_line)	// we start at 1, due to header
			{
				uint	last_comma = 0, pos = 0,
						comma_count = 0, str_len = 0;
				
				for(pos = 0; pos < line_len; pos++)
				{
					if(comma_count == 0)
						str_len ++;
					if(last_csv[pos] == ',')
						comma_count ++;
					if(comma_count == 4)
					{
						last_comma = pos;
						break;
					}
				}
				memcpy(lineBuffer, last_csv, sizeof(char)*last_comma);
				lineBuffer[last_comma] = '\0';
				
				if(str_len > HCFR_COLOR_NAME_MAX)
					str_len = HCFR_COLOR_NAME_MAX;
				memcpy(hcfr_colors[current_line-1].name, last_csv, sizeof(char)*(str_len-1));
				if(sscanf(lineBuffer+str_len, "%d,%d,%d",
								&hcfr_colors[current_line-1].r,
								&hcfr_colors[current_line-1].g,
								&hcfr_colors[current_line-1].b) != 3)
					{
						dbglog(DBG_ERROR, "File: %s Invalid structure in line %d: \"%s\"\n", 
								filename, current_line, lineBuffer);
						free(buffer);
						free(hcfr_colors);
						free(lineBuffer);
						return NULL;
					}
			}
			
			current_line ++;
			line_len = 0;
			last_csv = buffer + i + 1;
		}
	}
	
	free(buffer);
	free(lineBuffer);
	*hcfr_num = hcfr_count;
	return hcfr_colors;
}

void DrawHCFR()
{
	int 		done = 0, oldvmode = -1, current_hcfr = 0;
	int			matchIRE = settings.matchIRE, text_mV = 0;
	uint16		pressed;		
	ImagePtr	back = NULL, black = NULL;
	controller	*st = NULL;
	char		screenmsg[100], vmuMsg2[50];
	int			hcfr_num = 0, changed_palette = 1, num_files = 0;
	int			i = 0, selected_hcfr = 0;
	hcfr_color 	*hcfr_colors = NULL;
	hcfr_file	*filenames = NULL;
	fmenudata 	*resmenudata = NULL;
	
	filenames = listCSVFiles("/rd/hcfr/", &num_files);
	if(!filenames)
	{
		DrawMessage("Could not find any HCFR CSV files in the hcfr folder");
		return;	
	}
	
	resmenudata = (fmenudata*)malloc(sizeof(fmenudata)*num_files);
	if(!resmenudata)
	{
		releaseCSVList(&filenames, num_files);
		return;
	}
	
	for(i = 0; i < num_files; i++)
	{
		resmenudata[i].option_value = i + 1;
		resmenudata[i].option_text = filenames[i].dispname;
	}
	
	selected_hcfr = SelectMenuEx("Select Standard", resmenudata, num_files, selected_hcfr + 1, HCFR_MENU_HELP);
	if(selected_hcfr == MENU_CANCEL)
	{
		releaseCSVList(&filenames, num_files);
		free(resmenudata);
		return;
	}
	selected_hcfr -= 1;
	
	hcfr_colors = LoadHCFR(filenames[selected_hcfr].filename, &hcfr_num);
	if(!hcfr_colors)
	{
		DrawMessage("Could not load HCFR CSV file");
		return;
	}

#ifdef DCLOAD
	dbglog(DBG_INFO, "File: \"%s\" %s\n", 
		filenames[selected_hcfr].dispname,
		filenames[selected_hcfr].filename);
	for(i = 0; i < hcfr_num; i++)
	{
		dbglog(DBG_INFO, "HCFR [%02d]: %s %d %d %d\n", 
				i,
				hcfr_colors[i].name,
				hcfr_colors[i].r,
				hcfr_colors[i].g,
				hcfr_colors[i].b);
	}
#endif
	
	back = LoadKMG("/rd/hcfr/00_hcfr_pal_base.dgz", 0);
	if(!back)
		return;
	
	black = LoadKMG("/rd/black.kmg.gz", 0);
	if(!black)
	{
		FreeImage(&back);
		return;
	}	

	sprintf(vmuMsg2, matchIRE ? " 714.3 mV" : " 800.0 mV");
	while(!done && !EndProgram) 
	{
		double alpha = 0;
		
		if(oldvmode != vmode)
		{
			black->w = 320;
			black->h = IsPAL ? 264 : 240;
			oldvmode = vmode;
		}
							
		if(changed_palette)
		{
			// set up an ARGB8888 palette:
			pvr_set_pal_format(PVR_PAL_ARGB8888);
			pvr_set_pal_entry(0, PACK_ARGB8888(255, 0, 0, 0));	// solid black
			pvr_set_pal_entry(1, PACK_ARGB8888(0, 0, 0, 0));	// transparent
			pvr_set_pal_entry(2, PACK_ARGB8888(255,				// solid selected color
					hcfr_colors[current_hcfr].r, 
					hcfr_colors[current_hcfr].g, 
					hcfr_colors[current_hcfr].b));
			sprintf(screenmsg, "%s %03d,%03d,%03d", 
					hcfr_colors[current_hcfr].name,
					hcfr_colors[current_hcfr].r, 
					hcfr_colors[current_hcfr].g, 
					hcfr_colors[current_hcfr].b);
			changed_palette = 0;
		}
		
		StartScene();
				
		if(matchIRE)
		{
			alpha = back->alpha;
			back->alpha *= MATCH_NTSC_IRE;  
		}
		DrawImage(black);
		DrawImage(back);
		if(matchIRE)
			back->alpha = alpha;
			
		DrawStringSCentered(180, 0.5f, 0.5f, 0.5f, screenmsg);

		if(text_mV)
		{
			DrawStringS(225, 210, 1.0f, 1.0f, 1.0f, vmuMsg2);
			text_mV--;
		}

		EndScene();
		
		VMURefresh(hcfr_colors[current_hcfr].name, vmuMsg2);

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;								
				
			if (pressed & CONT_Y)
			{
				matchIRE = !matchIRE;
				sprintf(vmuMsg2, matchIRE ? " 714.3 mV IRE" : " 800.0 mV IRE");
				text_mV = 60;
				refreshVMU = 1;
			}
			
			if (pressed & CONT_X)
			{
				int selected_hcfr_tmp = 0;
				
				selected_hcfr_tmp = SelectMenu("Select Standard", resmenudata, num_files, selected_hcfr + 1);
				if(selected_hcfr_tmp != MENU_CANCEL && selected_hcfr_tmp != selected_hcfr + 1)
				{
					selected_hcfr = selected_hcfr_tmp - 1;
					refreshVMU = 1;
					changed_palette = 1;
					current_hcfr = 0;
				}
			}
			
			if (pressed & CONT_LTRIGGER || 
				pressed & CONT_DPAD_LEFT)
			{
				current_hcfr --;
				if(current_hcfr < 0)
					current_hcfr = hcfr_num - 1;
				changed_palette = 1;
				refreshVMU = 1;
			}
			
			if (pressed & CONT_RTRIGGER || 
				pressed & CONT_DPAD_RIGHT)
			{
				current_hcfr++;
				if(current_hcfr > hcfr_num - 1)
					current_hcfr = 0;
				changed_palette = 1;
				refreshVMU = 1;
			}

			if (pressed & CONT_START)
			{
				ShowMenu(HCFRHELP);
				changed_palette = 1;
			}
		}
	}
	FreeImage(&back);
	FreeImage(&black);
	free(hcfr_colors);
	releaseCSVList(&filenames, num_files);
	free(resmenudata);
	return;
}