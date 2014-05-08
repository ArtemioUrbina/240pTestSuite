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
	int video = NATIVE_320, x = 0, y = 0;	
	ImagePtr GridImage = NULL, Image = NULL, Check = NULL;
		
	VIDEO_Init();
		
	WPAD_Init();

	SetVideoMode(video);
	InitGX();
	SetupGX();

    LoadFont();
	GridImage = LoadImage(grid, 0);
	if(!GridImage)
    {
        ReleaseFont();
		exit(0);
    }
	Check = LoadImage(checkpos, 0);
	if(!Check)
    {
        ReleaseFont();
		exit(0);
    }
		
	Image = GridImage;
	while(1) 
	{
		WPAD_ScanPads();
		
		u32 pressed = WPAD_ButtonsDown(0);

		// We return to the launcher application via exit
		if ( pressed & WPAD_BUTTON_HOME ) 		
		{	
			Image = NULL;
			FreeImage(&GridImage);
			FreeImage(&Check);
            ReleaseFont();
			exit(0);
		}
		
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
		
		if( pressed & WPAD_BUTTON_1 )
		{		
			Image = GridImage;
		}
		
		if ( pressed & WPAD_BUTTON_2 )
		{			
			Image = Check;
		}
		
		StartScene();
		        
		DrawImage(Image);
		DrawStringS(x, y, 0xff, 0xff, 0xff, "Test Patterns");
		
		EndScene();

		VIDEO_Flush();
		VIDEO_WaitVSync();		
	}

	return 0;
}
