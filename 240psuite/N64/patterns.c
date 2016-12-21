/* 
 * 240p Test Suite for Nintendo 64
 * Copyright (C)2016 Artemio Urbina
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

#include "patterns.h"
#include "utils.h"
#include "menu.h"

void DrawPLUGE()
{
	int end = 0, ShowHelp = 0, text = 0;
	sprite_t *backNTSC = NULL, *backPAL = NULL;
	sprite_t *back = NULL, *highlight = NULL;
	struct controller_data keys;
	char		msg[50];
	
	backPAL = LoadImage("/plugePAL.bin");
	backNTSC = LoadImage("/pluge.bin");
	highlight = LoadImage("/PLUGEBorder.bin");
	
	back = backPAL;
    while(!end)
    {	
		GetDisplay();
		
		drawImageDMA(0, 0, back);
		
		if(ShowHelp)
		{
			int r = 0xff, g = 0x00, b = 0x00;
			
			DrawStringB(14, 205, r, g, b, back == backNTSC ? "11.5" : "3.5");	
			SoftDrawImage(14, 39, highlight);
			
			if(back == backNTSC)
			{
				r = 0x00;
				g = 0xff;
			}
			DrawStringB(44, 205, r, g, b, back == backNTSC ? "7.5" : "2");
			SoftDrawImage(44, 39, highlight);
			
			if(back != backNTSC)
			{
				r = 0x00;
				g = 0xff;
			}
			else
			{
				r = 0xff;
				g = 0x00;
			}
			DrawStringB(74, 205, r, g, b, back == backNTSC ? "3.5" : "1");	
			SoftDrawImage(74, 39, highlight);
			
			DrawStringB(228, 205, r, g, b, back == backNTSC ? "3.5" : "1");	
			SoftDrawImage(228, 39, highlight);
			
			if(back == backNTSC)
			{
				r = 0x00;
				g = 0xff;
			}		
			else
			{
				r = 0xff;
				g = 0x00;
			}
			DrawStringB(259, 205, r, g, b, back == backNTSC ? "7.5" : "2");
			SoftDrawImage(259, 39, highlight);
			
			r = 0xff;
			g = 0x00;
			DrawStringB(289, 205, r, g, b, back == backNTSC ? "11.5" : "3.5");	
			SoftDrawImage(289, 39, highlight);

			ShowHelp--;
		}
		
		if(text)
		{			
			DrawStringB(228, 20, 0, 0xff, 0, msg);
			text --;
		}
		
		CheckMenu(PLUGEHELP);
		WaitVsync();
		
		controller_scan();
		keys = Controller_ButtonsDown();
		
		if(keys.c[0].B)
			end = 1;
			
		if(keys.c[0].A)
		{
			if(useNTSC)
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
		
		if(keys.c[0].C_right)
			ShowHelp = 100;
		
		CheckStart(keys);
	}
	
	FreeImage(&backPAL);
	FreeImage(&backNTSC);
	FreeImage(&highlight);
}

void DrawColorBars()
{
	int end = 0, type, reload = 1;
	sprite_t *back[4];
	struct controller_data keys;
	
	for(type = 0; type < 4; type++)
		back[type] = NULL;
		
	type = 1;
	
	current_bitdepth = DEPTH_32_BPP;
	set_video();
	
    while(!end)
    {	
		if(current_bitdepth == DEPTH_16_BPP)
		{
			current_bitdepth = DEPTH_32_BPP;
			set_video();
		}
		
		if(reload)
		{
			back[0] = LoadImage("/colorlow.bin");			
			back[1] = LoadImage("/color.bin");					
			back[2] = LoadImage("/colorhigh.bin");		
			back[3] = LoadImage("/color_grid.bin");	
			reload = 0;
		}
		
		GetDisplay();
		
		drawImageDMA(0, 0, back[type]);
		
		if(ShowMenu)
		{
			for(int i = 0; i < 4; i++)
				FreeImage(&back[i]);
			reload = 1;
		}
		
		CheckMenu(COLORBARSHELP);
		WaitVsync();
		
		controller_scan();
		keys = Controller_ButtonsDown();
		
		if(keys.c[0].B)
			end = 1;
			
		CheckStart(keys);
			
		if(keys.c[0].left || keys.c[0].L)
			type --;
		if(keys.c[0].right || keys.c[0].R)
			type ++;
			
		if(type < 0)
			type = 0;
		if(type > 3)
			type = 3;
	}
	
	for(type = 0; type < 4; type++)
		FreeImage(&back[type]);
		
	current_bitdepth = DEPTH_16_BPP;
	set_video();
}

void DrawGrid()
{
	int end = 0, reload = 1;
	sprite_t *back = NULL;
	struct controller_data keys;
		
	HelpWindow(GRIDWARNING, 0);	
    while(!end)
    {	
		if(ChangedVideoFormat || ChangedResolution || reload)
		{
			if(back)
				FreeImage(&back);
				
			if(dH == 240)
				back = LoadImage("/grid.bin");
			if(dH == 480)
				back = LoadImage("/grid480.bin");
			reload = 0;
			ChangedVideoFormat = ChangedResolution = 0;
		}
		
		GetDisplay();
		
		drawImageDMA(0, 0, back);
		
		CheckMenu(GRIDHELP);
		WaitVsync();
		
		controller_scan();
		keys = Controller_ButtonsDown();
		if(keys.c[0].B)
			end = 1;
		CheckStart(keys);
	}
	
	FreeImage(&back);
}

void DrawSolidColor()
{
	int end = 0, color = 0, BlackLevel = 0x00, text = 0;
	int	cr, cb, cg, drawr, drawg, drawb, sel = 1, editmode = 0;
	struct controller_data keys;
	char		msg[100], *mode[5] = { "White", "Black", "Red", "Green", "Blue" };
	
	if(useNTSC)
		BlackLevel = 0x13; // 7.5 IRE
	else
		BlackLevel = 0x00; // 0 IRE
	
	current_bitdepth = DEPTH_32_BPP;
	set_video();
	
	cr = cb = cg = 0xff; // white	
	drawr = drawg = drawb = 0xff; // white	
	while(!end)
    {	
		if(!useNTSC)
			BlackLevel = 0x00;
			
		if(current_bitdepth == DEPTH_16_BPP)
		{
			current_bitdepth = DEPTH_32_BPP;
			set_video();
		}
			
		GetDisplay();
		
		rdp_fill_start();
		rdp_rectangle(0, 0, dW, dH, drawr, drawg, drawb);
		rdp_end();
		
		if(text)
		{						
			DrawStringB(200, 20, 0xff, 0xff, 0xff, msg);			
			text --;
		}	
		
		CheckMenu(WHITEHELP);
		WaitVsync();
		
		controller_scan();
		keys = Controller_ButtonsDown();
		if(keys.c[0].B)
			end = 1;
		CheckStart(keys);
		
		if(keys.c[0].A && color == 1 && useNTSC)
		{
			if(!BlackLevel)
			{
				BlackLevel = 0x13;
				sprintf(msg, "#GBlack Level: 7.5 IRE#G");
			}
			else
			{
				BlackLevel = 0x0;
				sprintf(msg, "#GBlack Level: 0 IRE#G");
			}
			text = 140;
		}
		
		if(keys.c[0].C_right || keys.c[0].R)
		{
			color ++;
			if(color > 4)
				color = 0;		
			
			editmode = 0;
			if(color == 0 && cr + cb + cg != 3*0xff)
				sprintf(msg, "%s [EDITED]", mode[color]);
			else
				sprintf(msg, "%s", mode[color]);
			text = 30;
		}

		if(keys.c[0].C_left || keys.c[0].L)
		{			
			color --;
			if(color < 0)
				color = 4;			
				
			editmode = 0;
			if(color == 0 && cr + cb + cg != 3*0xff)
				sprintf(msg, "%s [edited]", mode[color]);
			else
				sprintf(msg, "%s", mode[color]);
			text = 30;
		}
		
		if(keys.c[0].A && color == 0)
			editmode = !editmode;
			
		if(editmode)
		{
			int *current = NULL;
							
			sprintf(msg, "#%cR:0x%x#W #%cG:0x%x#W #%cB:0x%x#W", 
					sel == 1 ? 'G' : 'W', cr,
					sel == 2 ? 'G' : 'W', cg, 
					sel == 3 ? 'G' : 'W', cb);
			text = 1;

			if(keys.c[0].left)
			{
				sel --;
				if(sel < 1)
					sel = 3;
			}
			
			if(keys.c[0].right)
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
			
			if(keys.c[0].up)
			{				
				if(current)
				{
					(*current) ++;
					if(*current > 0xff)
						*current = 0xff;
				}
			}
			
			if(keys.c[0].down)
			{			
				if(current)
				{
					(*current) --;
					if(*current < 0)
						*current = 0;
				}
			}	

			if(keys.c[0].C_up)
			{				
				if(current)			
					*current  = 0;					
			}
			
			if(keys.c[0].C_down)
			{			
				if(current)				
					*current = 0xff;
			}	
		}
		
		switch(color)
		{
				case 0:
					drawr = cr;
					drawg = cg;
					drawb = cb;
					break;
				case 1:
					drawr = BlackLevel;
					drawg = BlackLevel;
					drawb = BlackLevel;
					break;
				case 2:
					drawr = 0xff;
					drawg = 0x00;
					drawb = 0x00;
					break;
				case 3:
					drawr = 0x00;
					drawg = 0xff;
					drawb = 0x00;
					break;
				case 4:
					drawr = 0x00;
					drawg = 0x00;
					drawb = 0xff;
					break;
		}
	}
	
	current_bitdepth = DEPTH_16_BPP;
	set_video();
}

void DrawOverscan()
{
	int end = 0, sel = 0, reset = 0;
	int oLeft = 0, oTop = 0, oRight = 0, oBottom = 0;
	int square_x = 0, square_y = 0, square_w = dW, square_h = dH;
	struct controller_data keys, held;
	char msg[50];
	
	HelpWindow(OVERSCANWARNING, 0);	
    while(!end)
    {	
		int x = 0, y = 0;
		
		if(ChangedResolution || reset)
		{
			oTop = oLeft = oBottom = oRight = 0;
			square_x = square_y = 0;
			square_w = dW;
			square_h = dH;
			reset = 0;
			ChangedResolution = 0;
		}
		
		GetDisplay();

		rdp_fill_start();
		rdp_rectangle(0, 0, dW, dH, 0xff, 0xff, 0xff);
		rdp_rectangle(square_x, square_y, square_w, square_h, 0x60, 0x60, 0x60);	
		rdp_end();
		
		x = dW/2;
		y = dH/2-2*fh;
		
		DrawStringS(x-110, y+(fh*sel), 0xff, 0x00, 0x00, ">");
				
		DrawStringS(x-100, y, 0xee, 0xee, 0xff, "Top Overscan:");
		sprintf(msg, "%d pixels (%g%%)", oTop, (oTop*100.0f)/(dH/2));
		DrawStringS(x+20, y, 0xee, 0xee, 0xff, msg);
		
		y+= fh;
		
		DrawStringS(x-100, y, 0xee, 0xee, 0xff, "Bottom Overscan:");
		sprintf(msg, "%d pixels (%g%%)", oBottom, (oBottom*100.0f)/(dH/2));
		DrawStringS(x+20, y, 0xee, 0xee, 0xff, msg);
		
		y+= fh;		
		
		DrawStringS(x-100, y, 0xee, 0xee, 0xff, "Left Overscan:");
		sprintf(msg, "%d pixels (%g%%)", oLeft, (oLeft*100.0f)/(dW/2));
		DrawStringS(x+20, y, 0xee, 0xee, 0xff, msg);
		
		y+= fh;
		
		DrawStringS(x-100, y, 0xee, 0xee, 0xff, "Right Overscan:");
		sprintf(msg, "%d pixels (%g%%)", oRight, (oRight*100.0f)/(dW/2));
		DrawStringS(x+20, y, 0xee, 0xee, 0xff, msg);
		
		CheckMenu(OVERSCANHELP);
		WaitVsync();
		
		controller_scan();
		keys = Controller_ButtonsDown();
		held = Controller_ButtonsHeld();
		
		if(keys.c[0].down)
			sel++;
		if(keys.c[0].up)
			sel--;
			
		if(sel < 0)
			sel = 3;
		if(sel > 3)
			sel = 0;
			
		// Top
		if(sel == 0 && (keys.c[0].R || held.c[0].C_right))
		{
			if(square_y + 1 <= dH/2 && oTop + 1 <= dH/2)
			{				
				square_y++;
				oTop++;
			}
		}
		
		if(sel == 0 && (keys.c[0].L || held.c[0].C_left))
		{
			if(square_y - 1 >= 0 && oTop - 1 >= 0)
			{				
				square_y--;
				oTop--;
			}
		}
		
		// Bottom
		if(sel == 1 && (keys.c[0].R || held.c[0].C_right))
		{
			if(square_h - 1 >= 0 && oBottom + 1 <= dH/2)
			{								
				square_h--;
				oBottom++;
			}
		}
		
		if(sel == 1 && (keys.c[0].L || held.c[0].C_left))
		{
			if(square_h + 1 <= dW && oBottom - 1 >=0 )
			{								
				square_h++;	
				oBottom--;
			}
		}
		
		// Left
		if(sel == 2 && (keys.c[0].R || held.c[0].C_right))
		{
			if(square_x + 1 <= dW/2 && oLeft + 1 <= dW/2)
			{				
				square_x++;
				oLeft++;
			}
		}
		
		if(sel == 2 && (keys.c[0].L || held.c[0].C_left))
		{
			if(square_x - 1 >= 0 && oLeft - 1 >= 0)
			{				
				square_x--;
				oLeft--;
			}
		}
		
		// Right
		if(sel == 3 && (keys.c[0].R || held.c[0].C_right))
		{
			if(square_w - 1 >= 0 && oRight + 1 <= dW/2)
			{								
				square_w--;
				oRight++;
			}
		}
		
		if(sel == 3 && (keys.c[0].L || held.c[0].C_left))
		{
			if(square_w + 1 <= dW && oRight - 1 >= 0)
			{								
				square_w++;	
				oRight--;
			}
		}
				
		if(keys.c[0].A)	
			reset = 1;
			
		if(keys.c[0].B)
			end = 1;
			
		CheckStart(keys);
	}
}

void DrawCB601()
{
	int end = 0;
	sprite_t *back = NULL;
	struct controller_data keys;
	
	back = LoadImage("/601701cb.bin");
	if(!back)
		return;
		
    while(!end)
    {	
		GetDisplay();
		
		drawImageDMA(0, 0, back);
		
		CheckMenu(COLOR601);
		WaitVsync();
		
		controller_scan();
		keys = Controller_ButtonsDown();
		
		if(keys.c[0].B)
			end = 1;
		
		CheckStart(keys);
	}
	
	FreeImage(&back);
}

void DrawSharpness()
{
	int end = 0;
	sprite_t *back = NULL;
	struct controller_data keys;
	
	back = LoadImage("/sharpness.bin");
	if(!back)
		return;
		
    while(!end)
    {	
		GetDisplay();
		
		drawImageDMA(0, 0, back);
		
		CheckMenu(SHARPNESSHELP);
		WaitVsync();
		
		controller_scan();
		keys = Controller_ButtonsDown();
		
		if(keys.c[0].B)
			end = 1;
		
		CheckStart(keys);
	}
	
	FreeImage(&back);
}

void DrawGrayRamp()
{
	int end = 0;
	sprite_t *back = NULL;
	struct controller_data keys;
	
	back = LoadImage("/grayramp.bin");
	if(!back)
		return;
		
    while(!end)
    {	
		GetDisplay();
		
		drawImageDMA(0, 0, back);
		
		CheckMenu(GRAYHELP);
		WaitVsync();
		
		controller_scan();
		keys = Controller_ButtonsDown();
		
		if(keys.c[0].B)
			end = 1;
		
		CheckStart(keys);
	}
	
	FreeImage(&back);
}

void DrawSMPTE()
{
	int 		end = 0, draw100 = 0, text = 0, load = 1;
	sprite_t 	*back = NULL, *back100 = NULL, *font = NULL;
	char		msg[40];
	struct controller_data keys;
	
	font = LoadImage("/font.bin");
    while(!end)
    {	
		GetDisplay();
		
		if(load || ChangedVideoFormat)
		{
			FreeImage(&back);
			FreeImage(&back100);
			
			if(useNTSC)
			{
				back = LoadImage("/SMPTECB75.bin");
				back100 = LoadImage("/SMPTECB100.bin");
			}
			else
			{
				back = LoadImage("/EBUColorBars75.bin");
				back100 = LoadImage("/EBUColorBars100.bin");
			}
			load = 0;
			ChangedVideoFormat = 0;
		}
		
		//drawImageDMA(0, 0, draw100 ? back100 : back); 
		rdp_texture_start();
		rdp_DrawImage(60, 60, font);		
		rdp_end();
		
		if(text)
		{
			DrawStringB(260, 20, 0, 0xff, 0, msg);
			text --;
		}
		
		CheckMenu(SMPTECOLOR);
		WaitVsync();
		
		controller_scan();
		keys = Controller_ButtonsDown();
		
		if(keys.c[0].B)
			end = 1;
			
		if(keys.c[0].A)
		{
			draw100 = !draw100;
			sprintf(msg, "%s%%", draw100 ? "100" : "75");
			text = 30;
		}
		
		CheckStart(keys);
	}
	
	FreeImage(&back);
	FreeImage(&back100);
}

void DrawColorBleed()
{
	int end = 0;
	sprite_t *back = NULL, *check = NULL, *normal = NULL;
	struct controller_data keys;
	
	normal = LoadImage("/colorbleed.bin");
	check = LoadImage("/colorbleedchk.bin");
	
	back = normal;	
    while(!end)
    {	
		GetDisplay();
		
		drawImageDMA(0, 0, back);
		
		CheckMenu(COLORBLEEDHELP);
		WaitVsync();
		
		controller_scan();
		keys = Controller_ButtonsDown();
		
		if(keys.c[0].B)
			end = 1;
		
		if(keys.c[0].A)
		{
			if(back == normal)
				back = check;
			else
				back = normal;
		}
		
		CheckStart(keys);
	}
	
	FreeImage(&check);
	FreeImage(&normal);
}

void DrawLinearity()
{
	int 		end = 0, load = 1;
	sprite_t 	*back = NULL;
	struct controller_data keys;
	
    while(!end)
    {	
		GetDisplay();
		
		if(load || ChangedVideoFormat)
		{
			FreeImage(&back);
			
			if(useNTSC)
				back = LoadImage("/circlesNTSC.bin");
			else
				back = LoadImage("/circlesPAL.bin");
			load = 0;
			ChangedVideoFormat = 0;
		}
		
		drawImageDMA(0, 0, back); 
		
		CheckMenu(LINEARITYHELP);
		WaitVsync();
		
		controller_scan();
		keys = Controller_ButtonsDown();
		
		if(keys.c[0].B)
			end = 1;
		
		CheckStart(keys);
	}
	
	FreeImage(&back);
}
