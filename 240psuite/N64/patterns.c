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

void DrawPLUGE()
{
	int end = 0;
	sprite_t *back = NULL;
	struct controller_data keys;
	
	back = LoadImage("/pluge.bin");
	if(!back)
		return;
		
    while(!end)
    {	
		GetDisplay();
		
		drawImageDMA(0, 0, back);
		
		WaitVsync();
		
		controller_scan();
		keys = Controller_ButtonsDown();
		if(keys.c[0].B)
			end = 1;
	}
	
	FreeImage(&back);
}

void DrawColorBars()
{
	int end = 0, type, redraw = 0;
	sprite_t *back[4];
	struct controller_data keys;
	
	for(type = 0; type < 4; type++)
		back[type] = NULL;
		
	type = 1;
	
	current_bitdepth = DEPTH_32_BPP;
	set_video();
	
	back[0] = LoadImage("/colorlow.bin");			
	back[1] = LoadImage("/color.bin");					
	back[2] = LoadImage("/colorhigh.bin");		
	back[3] = LoadImage("/color_grid.bin");
	
    while(!end)
    {	
		GetDisplay();
		
		drawImageDMA(0, 0, back[type]);
		
		WaitVsync();
		
		controller_scan();
		keys = Controller_ButtonsDown();
		if(keys.c[0].B)
			end = 1;
			
		if(keys.c[0].left)
		{
			type --;
			redraw = 0;
		}
		if(keys.c[0].right)
		{
			type ++;
			redraw = 0;
		}
			
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
	int end = 0;
	sprite_t *back = NULL;
	struct controller_data keys;
	
	if(dH == 240)
		back = LoadImage("/grid.bin");
	if(dH == 480)
		back = LoadImage("/grid480.bin");
	if(!back)
		return;
		
    while(!end)
    {	
		GetDisplay();
		
		SoftDrawImage(0, 0, back);
		
		WaitVsync();
		
		controller_scan();
		keys = Controller_ButtonsDown();
		if(keys.c[0].B)
			end = 1;
	}
	
	FreeImage(&back);
}

void DrawSolidColor()
{
	int end = 0;
	struct controller_data keys;
	
	while(!end)
    {	
		GetDisplay();
		
		rdp_fill_start();
		rdp_rectangle(0, 0, dW, dH, 0xff, 0xff, 0xff);
		rdp_end();
		
		WaitVsync();
		
		controller_scan();
		keys = Controller_ButtonsDown();
		if(keys.c[0].B)
			end = 1;
	}
}

void DrawOverscan()
{
	int end = 0, sel = 0, reset = 0;
	int oLeft = 0, oTop = 0, oRight = 0, oBottom = 0;
	int square_x = 0, square_y = 0, square_w = dW, square_h = dH;
	struct controller_data keys, held;
	char msg[50];
	
    while(!end)
    {	
		int x = 0, y = 0;
		
		if(reset)
		{
			oTop = oLeft = oBottom = oRight = 0;
			square_x = square_y = 0;
			square_w = dW;
			square_h = dH;
			reset = 0;
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
	}
}
