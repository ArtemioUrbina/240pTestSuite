/* 
 * 240p Test Suite
 * Copyright (C)2016 Artemio Urbina (N64)
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

#include "menu.h"
#include "help.h"

int ChangeVideoEnabled = 1;
int ShowMenu = 0;

inline void CheckMenu(char *help)
{
	if(ShowMenu)
	{
		HelpData = help;
		showMenu();
		ShowMenu = 0;
		ClearScreen();
	}
}

inline void CheckStart(struct controller_data keys)
{
	if(keys.c[0].start)
		ShowMenu = 1;
}
 
 
void showMenu()
{
	int end = 0, px, py, sel = 1;
	sprite_t *menu = NULL;
	struct controller_data keys;
	
	CopyScreen();
	WaitVsync();
	DarkenScreenBuffer(0x30);
	
	menu = LoadImage("/menu.bin");
	if(!menu)
		return;
		
	px = (dW - menu->width) / 2;
	py = (dH - menu->height) / 2;
		
    while(!end)
    {	
		int     r = 0xff;
		int     g = 0xff;
		int     b = 0xff;
		int   	c = 1;				    					   
		int     x = px + 20;
		int     y = py + 10;
		char	videomode[50];
				
		GetDisplay();
		
		drawScreenBufferDMA(0, 0);
		SoftDrawImage(px, py, menu);   

		DrawStringS(x, y, 0x00, 0xff, 0x00, VERSION_NUMBER); y += 2*fh; 		
		
		GetVideoModeStr(videomode, 1);
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Help"); y += fh; c++;				
		if(ChangeVideoEnabled)
		{
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Video");
			DrawStringS(x+6*5, y, r, sel == c ? 0 : g, sel == c ? 0 : b, videomode); y += fh; c++;
		}
		else
		{
			DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA,	sel == c ? 0x77 : 0xAA, "Video");
			DrawStringS(x+6*5, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, videomode); y += fh; c++;		
		}
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Options"); y += fh; c++;		
        DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Credits"); y += 2* fh; c++;
		DrawStringS(x, y, r-0x40, sel == c ? 0 : g,	sel == c ? 0 : b, "Close Menu"); 
		
		WaitVsync();
		
		controller_scan();
		keys = Controller_ButtonsDown();
		
		if(keys.c[0].B)
			end = 1;
			
		if(keys.c[0].up)
			sel--;

		if(keys.c[0].down)
			sel++;
		
		if(sel > c)
			sel = 1;
		if(sel < 1)
			sel = c;
			
		if(keys.c[0].A)
		{	
			switch(sel)
			{
				case 1:
					HelpWindow(HelpData, 1);
					break;
				case 5:
					end = 1;
					break;
				default:
					break;
			}
		}
	}
	
	FreeImage(&menu);
}