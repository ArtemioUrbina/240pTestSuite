/* 
 * 240p Test Suite
 * Copyright (C)2014 Artemio Urbina (Wii GX)
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
 
#include <stdlib.h>
#include <wiiuse/wpad.h>
#include "video.h"
#include "image.h"
#include "font.h"

int main(int argc, char **argv) 
{
	int video = NATIVE_320, close = 0;	
	ImagePtr Back = NULL, sd = NULL;
		
	VIDEO_Init();
		
	WPAD_Init();

	SetVideoMode(video);
	InitGX();
	SetupGX();

    LoadFont();
	Back = LoadImage(back, 0);
	if(!Back)
    {
        ReleaseFont();
		exit(0);
    }	
    sd = LoadImage(supdef, 0);
    if(!sd)
    {
        ReleaseFont();
		exit(0);
    }
    
    sd->x = 221;
    sd->y = 94;
			
	while(!close) 
	{        
		u8      r = 0xff;
		u8      g = 0xff;
		u8      b = 0xff;
		u8   	c = 1;				    
		u8   	sel = 1;				    
		u16     x = 40;
		u16     y = 55;
        u32     pressed = 0;
		
		StartScene();
		        
		DrawImage(Back);
        DrawImage(sd);
		
        DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Test Patterns"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Drop Shadow Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Striped Sprite Test"); y += fh; c++;    
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Lag Test"); y += fh; c++;
        DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Manual Lag Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Scroll Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Grid Scroll Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Horizontal Stripes"); y += fh; c++;    
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Checkerboard"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Backlit Zone Test"); y += fh; c++;
        DrawStringS(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, "Alternating 240p/480i Test"); y += fh; c++;
        DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Sound Test"); y += fh; c++;
        DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Help"); y += fh; c++;
		DrawStringS(x, y + fh, r, sel == c ? 0 : g, sel == c ? 0 : b, "Credits"); y += fh; 
		
		EndScene();

		VIDEO_Flush();
		VIDEO_WaitVSync();

        WPAD_ScanPads();
		
		pressed = WPAD_ButtonsDown(0);

		// We return to the launcher application via exit
		if ( pressed & WPAD_BUTTON_HOME ) 		
			close = 1;
		
		// Change resolution
		if ( pressed & WPAD_BUTTON_A )
		{
			video ++;
			if(video >= TOTAL_VMODES)
				video = 0;			
	
			SetVideoMode(video);
			SetupGX();
		}
		
		if ( pressed & WPAD_BUTTON_UP )
		{
			y+=1;
		}
		
		if ( pressed & WPAD_BUTTON_DOWN )
		{
			y-=1;
		}
		
		if ( pressed & WPAD_BUTTON_RIGHT )
		{
			x+=1;
		}
		
		if ( pressed & WPAD_BUTTON_LEFT )
		{
			x-=1;
		}
		
		if( pressed & WPAD_BUTTON_B )
		{			
			x = y = 0;
		}		
	}
		
	FreeImage(&Back);
	ReleaseFont();	

	return 0;
}
