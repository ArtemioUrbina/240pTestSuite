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

int ChangedVideoFormat = 0;
int ChangedResolution = 0;

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
	int end = 0, px, py, sel = 1, color32 = 0;
	sprite_t *menu = NULL;
	struct controller_data keys;
	
	if(current_bitdepth != DEPTH_32_BPP)
	{
		CopyScreen();
		WaitVsync();
		DarkenScreenBuffer(0x30);
	}
	
	menu = LoadImage("/menu.bin");
	if(!menu)
		return;
		
	px = (dW - menu->width) / 2;
	py = (dH - menu->height) / 2;
	
	if(current_bitdepth == DEPTH_32_BPP)
	{
		current_bitdepth = DEPTH_16_BPP;
		color32 = 1;
		set_video();
	}
		
    while(!end)
    {	
		int     r = 0xff;
		int     g = 0xff;
		int     b = 0xff;
		int   	c = 1;				    					   
		int     x = px + 10;
		int     y = py + 10;
		char	videomode[50];
				
		GetDisplay();
		
		if(!color32)
			drawScreenBufferDMA(0, 0);
			
		rdp_texture_start();
		rdp_DrawImage(px, py, menu);
		rdp_end();  

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
			
		if(keys.c[0].B || keys.c[0].start)
			end = 1;
			
		if(keys.c[0].A)
		{	
			switch(sel)
			{
				case 1:
					HelpWindow(HelpData, 1);
					break;
				case 2:
					if(ChangeVideoEnabled)
					{
						SelectVideoMode(1);
						px = (dW - menu->width) / 2;
						py = (dH - menu->height) / 2;
					}
					break;
				case 3:
					ShowOptions(1);
					break;
				case 4:
					DrawCredits(1);
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
	
	if(color32)
	{
		current_bitdepth = DEPTH_32_BPP;
		set_video();
	}
}

void SelectVideoMode(int usebuffer)
{
	int 		sel = 1, close = 0, mode = current_resolution, vformat = useNTSC;
	sprite_t 	*back;
	struct controller_data keys;
	
	if(!usebuffer)
	{
		CopyScreen();
		WaitVsync();
	}
	
	back = LoadImage("/help.bin");
	if(!back)
		return;
		
	ChangedVideoFormat = 0;
	ChangedResolution = 0;
		
	sel = mode + (!useNTSC ? 2 : 0) + 1;
	while(!close)
	{		
		int     r = 0xff;
		int     g = 0xff;
		int     b = 0xff;
		int   	c = 1;   
		int     x = 70;
		int     y = 58;
				
		GetDisplay();

		drawScreenBufferDMA(0, 0);
		rdp_texture_start();
		rdp_DrawImage(0, 37, back);
		rdp_end();  

		DrawStringS(x - 20, y, 0x00, 0xff, 0x00, "Please select the desired video mode"); y += 3*fh; 
		
		mode = current_resolution;	
			
		DrawStringS(x - 10, y + (mode * fh) + (!useNTSC ? 2*fh+fh/2 : 0), 0x00, 0xff, 0x00, ">"); 
		
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "240p"); y += fh; c++;
		//DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "480i scaled 240p assets (NTSC)"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "480i mixed 480/240 assets (1:1/NTSC)"); y += fh; c++;				
		
		y += fh/2;
		if(EnablePAL)
		{
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "288p"); y += fh; c++;
			//DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "576i scaled 264/240p assets (PAL)"); y += fh; c++;
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "576i 528/480/240 assets (PAL)"); c++;
		}
		else
		{
			DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "288p"); y += fh; c++;
			//DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "576i scaled 264/240p assets (stretched/PAL)"); y += fh; c++;
			DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "576i 528/480/240 assets (PAL)"); c++;			
		}			
		
		y += fh/2;
			
		DrawStringS(x, y + fh, r-0x40, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); 		
				
		DrawStringS(x+40, 200, r, g, b, "Press START for help");
	
		WaitVsync();

		controller_scan();
		keys = Controller_ButtonsDown();
		
		if(keys.c[0].up)
	    {
		    sel --;
		    if(sel < 1)
			    sel = c;		
	    }
	    
	    if(keys.c[0].down)
	    {
		    sel ++;
		    if(sel > c)
			    sel = 1;	
	    }	

		if(keys.c[0].start)
		{
			char *oldHelp = HelpData;
			
			HelpWindow(VIDEOHELP, 1);
			HelpData = oldHelp;
		}		
			
		if(keys.c[0].B)
			close = 1;	
	
		if(keys.c[0].A)
		{     
			switch(sel)
			{			
					case 1:						
						useNTSC = 1;
						current_resolution = RESOLUTION_320x240;
						set_video();
						break;
					case 2:
						useNTSC = 1;
						current_resolution = RESOLUTION_640x480;
						set_video();
						break;
					case 3:
						if(EnablePAL)
						{
							useNTSC = 0;
							current_resolution = RESOLUTION_320x240;
							set_video();
						}
						break;
					case 4:
						if(EnablePAL)
						{
							useNTSC = 0;
							current_resolution = RESOLUTION_640x480;
							set_video();
						}
						break;		
					case 5:
						close = 1;
						break;
					default:
						break;
			} 	
			if(mode != current_resolution)
				ChangedResolution = 1;
				
			if(vformat != useNTSC)
				ChangedVideoFormat = 1;
				
			mode = current_resolution;
			vformat = useNTSC;		
		}		
	}
	FreeImage(&back);
}

void DrawCredits(int usebuffer)
{
	int 	done = 0;	
    int     counter = 1;
	char	data[50];
	sprite_t 	*back;
	struct controller_data keys;
	
	back = LoadImage("/help.bin");
	if(!back)
		return;
		
	while(!done)  
	{
		int x = 35, y = 58;

		GetDisplay();

		if(usebuffer)
			drawScreenBufferDMA(0, 0);
		rdp_texture_start();
		rdp_DrawImage(0, 37, back);
		rdp_end();  
		
        DrawStringS(x, y, 0x00, 0xff, 0x00, "Code and Patterns:"); y += fh; 
        DrawStringS(x, y, 0xff, 0xff, 0xff, "Artemio Urbina"); y += fh; 

		if(counter == 1)
			sprintf(data, "aurbina@junkerhq.net");			
		if(counter == 60*4)
			sprintf(data, "@Artemio (twitter)");					
		if(counter == 60*8)
			counter = 0;

		DrawStringS(x, y, 0x00, 0xff, 0x00, "Support and suggestions:"); y += fh; 
		DrawStringS(x+5, y, 0xff, 0xff, 0xff, data); y += fh; 

		DrawStringS(x, y, 0x00, 0xff, 0x00, "SDK:"); y += fh; 
		DrawStringS(x+5, y, 0xff, 0xff, 0xff, "libdragon"); y += fh; 	
		DrawStringS(x, y, 0x00, 0xff, 0x00, "Menu Pixel Art:"); y += fh; 
		DrawStringS(x+5, y, 0xff, 0xff, 0xff, "Asher"); y += fh; 		
		DrawStringS(x, y, 0x00, 0xff, 0x00, "Advisor:"); y += fh; 
		DrawStringS(x+5, y, 0xff, 0xff, 0xff, "Fudoh"); y += fh; 
		DrawStringS(x, y, 0x00, 0xff, 0x00, "Collaboration:"); y += fh; 
		DrawStringS(x+5, y, 0xff, 0xff, 0xff, "shmups.system11.org regulars"); y += fh; 
	
		DrawStringS(x, y, 0x00, 0xff, 0x00, "Info on using this suite:"); y += fh; 
		DrawStringS(x+5, y, 0xff, 0xff, 0xff, "http://junkerhq.net/240p/"); y += 2*fh; 

		DrawStringS(x, y, 0x00, 0xba, 0xba, "This program is free Software.");  y += fh;
		DrawStringS(x, y, 0x00, 0xba, 0xba, "Source code is available under GPL.");  y += fh;
		
		DrawStringS(180, y, 0x00, 0xba, 0xba, "Dedicated to Elisa.");
		
		y = 58;
		
		DrawStringS(200, y, 0x0f, 0xff, 0xff, VERSION_NUMBER); y += fh;
		DrawStringS(200, y, 0x0f, 0xff, 0xff, VERSION_DATE); y += 2*fh;

		WaitVsync();

		controller_scan();
		keys = Controller_ButtonsDown();
		
		if(keys.c[0].Z)
		{
			int 		check = 0;
			sprite_t 	*nish;	
			
			nish = LoadImage("/nish.bin");
			if(nish)
			{					
				while(!check) 
				{		
					check = 1;
					GetDisplay();
					drawImageDMA(0, 0, nish);
					WaitVsync();
					controller_scan();
					keys = Controller_ButtonsHeld();
					if(keys.c[0].Z)
						check =	0;								
				}
				FreeImage(&nish);
			}
		}
		
		if(keys.c[0].B)
			done =	1;		
			
		counter ++;			
	}
	
	FreeImage(&back);
}

void ShowOptions(int usebuffer)
{
	int 		sel = 1, close = 0, last_frame = 0, warning = 0;
	sprite_t 	*back;
	struct controller_data keys;
	char		str[100];
	int			o_current_antialias, o_EnableDither, o_current_bitdepth, 
				o_EnableDivot, o_current_gamma;
	
	if(!usebuffer)
	{
		CopyScreen();
		WaitVsync();
	}
	
	back = LoadImage("/help.bin");
	if(!back)
		return;
	
	o_current_antialias = current_antialias;
	o_EnableDither = EnableDither;
	o_current_bitdepth = current_bitdepth;
	o_EnableDivot = EnableDivot;
	o_current_gamma = current_gamma;
	
	while(!close)
	{		
		int     r = 0xff;
		int     g = 0xff;
		int     b = 0xff;
		int   	c = 1;   
		int     x = 70;
		int     y = 58;
				
		GetDisplay();

		drawScreenBufferDMA(0, 0);
		rdp_texture_start();
		rdp_DrawImage(0, 37, back);
		rdp_end();  

		DrawStringS(x, y, 0x00, 0xff, 0x00, "Options Menu"); y += 3*fh; 

		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Enable PAL:");
		DrawStringS(x+100, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, EnablePAL ? "ON" : "OFF");
		y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Gamma:");
		switch(o_current_gamma)
		{
			case GAMMA_NONE:
				sprintf(str, "None");
				break;
			case GAMMA_CORRECT:
				sprintf(str, "Correct");
				break;
			case GAMMA_CORRECT_DITHER:
				sprintf(str, "Correct+Dither");
				break;
		}
		DrawStringS(x+100, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, str);
		y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Divot:");
		DrawStringS(x+100, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, o_EnableDivot ? "ON" : "OFF");
		y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Antialias:");
		switch(o_current_antialias)
		{
			case ANTIALIAS_OFF:
				sprintf(str, "OFF");
				break;
			case ANTIALIAS_RESAMPLE:
			case ANTIALIAS_RESAMPLE_NODIVOT:
				sprintf(str, "Resample");
				break;
			case ANTIALIAS_RESAMPLE_FETCH_NEEDED:
			case ANTIALIAS_RESAMPLE_FETCH_NEEDED_NODIVOT:
				sprintf(str, "Fetch Needed");
				break;
			case ANTIALIAS_RESAMPLE_FETCH_ALWAYS:
			case ANTIALIAS_RESAMPLE_FETCH_ALWAYS_NODIVOT:
				sprintf(str, "Fetch Always");
				break;
			default:
				sprintf(str, "Error");
				break;
		}
		DrawStringS(x+100, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, str);
		y += fh; c++;
		
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Dither Filter:");
		DrawStringS(x+100, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, o_EnableDither ? "ON" : "OFF");
		y += fh*2; c++;
		
		if(current_antialias != o_current_antialias || EnableDither != o_EnableDither || 
							current_bitdepth != o_current_bitdepth || EnableDivot != o_EnableDivot || 
							current_gamma != o_current_gamma)
			DrawStringS(x, y, r, sel == c ? 0 : g,	0, "Save Options");
		else
			DrawStringS(x, y, sel == c ? 0x66 : 0x99, sel == c ? 0x66 : 0x99, sel == c ? 0x66 : 0x99, "Save Options");
		y += fh; c++;
		
		y += fh/2;
		
		DrawStringS(x, y + fh, r-0x40, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); 		
		DrawStringS(x+40, 200, r, g, b, "Press START for help");
		
		if(warning)
		{
			DrawStringS(x, 160, 0, 0xff, 0xff, "Values have not been saved!");
			DrawStringS(x, 168, 0, 0xff, 0xff, "(B to exit without saving)");
			warning --;
		}
	
		WaitVsync();

		controller_scan();
		keys = Controller_ButtonsDown();
		
		if(keys.c[0].up)
	    {
		    sel --;
		    if(sel < 1)
			    sel = c;		
	    }
	    
	    if(keys.c[0].down)
	    {
		    sel ++;
		    if(sel > c)
			    sel = 1;	
	    }	

		if(keys.c[0].start)
		{
			char *oldHelp = HelpData;
			
			HelpWindow(OPTIONSHELP, 1);
			HelpData = oldHelp;
		}		
			
		if(keys.c[0].B)
			close = 1;	
	
		if(keys.c[0].A)
		{     
			switch(sel)
			{			
					case 1:						
						EnablePAL = !EnablePAL;
						break;
					case 2:
						o_current_gamma++;
						if(o_current_gamma > GAMMA_CORRECT_DITHER)
							o_current_gamma = GAMMA_NONE;
						break;
					case 3:
						o_EnableDivot = !o_EnableDivot;
						if(o_EnableDivot)
						{
							if(o_current_antialias >= ANTIALIAS_RESAMPLE_NODIVOT)
								o_current_antialias -= 3;
						}
						else
						{
							if(o_current_antialias < ANTIALIAS_RESAMPLE_NODIVOT)
								o_current_antialias += 3;
						}
						break;
					case 4:
						if(o_EnableDivot)
						{
							o_current_antialias++;
							if(o_current_antialias > ANTIALIAS_RESAMPLE_FETCH_ALWAYS)
								o_current_antialias = ANTIALIAS_RESAMPLE;
						}
						else
						{
							o_current_antialias++;
							if(o_current_antialias > ANTIALIAS_RESAMPLE_FETCH_ALWAYS_NODIVOT)
								o_current_antialias = ANTIALIAS_RESAMPLE_NODIVOT;
						}
						break;
					case 5:						
						o_EnableDither = !o_EnableDither;
						if(o_current_bitdepth != DEPTH_32_BPP)
						{
							if(o_EnableDither)
								o_current_bitdepth = DEPTH_16_BPP_DITHER;
							else
								o_current_bitdepth = DEPTH_16_BPP;
						}
						break;
					case 7:
						if(current_antialias != o_current_antialias || EnableDither != o_EnableDither || 
							current_bitdepth != o_current_bitdepth || EnableDivot != o_EnableDivot || 
							current_gamma != o_current_gamma)
							warning = 100;
						else
							close = 1;
						break;
			} 		
		}
		
		if(keys.c[0].A && last_frame + 120 < GetFrameCount())
		{     
			switch(sel)
			{		
				case 6:
					if(current_antialias != o_current_antialias || EnableDither != o_EnableDither || 
							current_bitdepth != o_current_bitdepth || EnableDivot != o_EnableDivot || 
							current_gamma != o_current_gamma)
					{
						warning = 0;
						
						current_antialias = o_current_antialias;
						EnableDither = o_EnableDither;
						current_bitdepth = o_current_bitdepth;
						EnableDivot = o_EnableDivot;
						current_gamma = o_current_gamma;
						
						set_video();
						last_frame = GetFrameCount();
					}
					break;
			} 	
		}		
	}
	FreeImage(&back);
}
