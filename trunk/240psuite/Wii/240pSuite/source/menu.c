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
 
#include <stdio.h>
#include <stdlib.h>
#include <wiiuse/wpad.h>
#include "video.h"
#include "image.h"
#include "font.h"
#include "options.h"
#include "help.h"
#include "menu.h"



#ifdef WII_VERSION
#include "CheckRegion.h"
char wiiregion[100];
#endif

u8 DrawMenu = 0;
u8 HelpData = 0;
u8 EndProgram = 0;

void ShowMenu()
{
	int 		sel = 1, close = 0;		
	ImagePtr	Back, cFB;	
	
	cFB = CopyFrameBufferToImage();	
	if(cFB)
	{
		cFB->alpha = 0x77;
	}
	Back = LoadImage(FLOATMENUIMG, 0);
	if(!Back)
		return;	
	
	Back->x = (dW - Back->w) / 2;
	Back->y = (dH - Back->h) / 2;
	
	Back->alpha = 0xaa;
	   
	while(!close) 
	{		
		u8      r = 0xff;
		u8      g = 0xff;
		u8      b = 0xff;
		u8   	c = 1;				    					   
		u16     x = 100;
		u16     y = 38;
        u32     pressed = 0;
				
		StartScene();
		
		DrawImage(cFB);         
		DrawImage(Back);       

		DrawStringS(x, y, 0x00, 0xff, 0x00, VERSION_NUMBER); y += 2*fh; 		
		
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Help"); y += fh; c++;		
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Options"); y += fh; c++;		
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Video"); y += fh; c++;		
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Close Menu"); y += 2* fh; c++;			
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Exit 240p Suite"); y += fh; 		
						
		EndScene();		
		
		WPAD_ScanPads();

		pressed = WPAD_ButtonsDown(0);
		
		if ( pressed & WPAD_BUTTON_UP )
	    {
		    sel --;
		    if(sel < 1)
			    sel = c;		
	    }
	    
	    if ( pressed & WPAD_BUTTON_DOWN )
	    {
		    sel ++;
		    if(sel > c)
			    sel = 1;	
	    }			
			
		if ( pressed & WPAD_BUTTON_B ) 		
			close = 1;	
	
		if (pressed & WPAD_BUTTON_A)
		{     
			switch(sel)
			{			
					case 1:						
						break;					
					case 2:		
						ChangeOptions(cFB);
						break;
					case 3:		
						SelectVideoMode(cFB, NULL);
						break;
					case 4:
						close = 1;
						break;
					case 5:
						EndProgram = 1;
						close = 1;
						break;
					default:
						break;
			} 			            										
		}		
	}
	
	FreeImage(&Back);
	FreeImage(&cFB);

	return;
}


void ChangeOptions(ImagePtr title)
{	
	int 		sel = 1, close = 0;	
	ImagePtr	back;
	
	back = LoadImage(HELPIMG, 1);
	if(!back)
		return;
		
	back->alpha = 0xaa;
		
	while(!close) 
	{		
		u8      r = 0xff;
		u8      g = 0xff;
		u8      b = 0xff;
		u8   	c = 1;				    					   
		u16     x = 80;
		u16     y = 55;
        u32     pressed = 0;
		char	intensity[80];
				
		StartScene();
		        
		DrawImage(title);        
		DrawImage(back);

		DrawStringS(x - 20, y, 0x00, 0xff, 0x00, "General Options"); y += 2*fh; 

#ifdef WII_VERSION				
		DrawStringS(x + 100, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, Options.ShowWiiRegion ? "ON" : "OFF");
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Show WII region"); y += fh; c++;		
#else
		DrawStringS(x + 100, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, Options.ShowWiiRegion ? "ON" : "OFF");
		DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "Show WII region"); y += fh; c++;		
#endif
				
		if(VIDEO_HaveComponentCable())
		{			
			DrawStringS(x + 100, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, Options.Activate480p ? "ON" : "OFF"); 					
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Allow 480p"); y += fh; c++;			
		}
		else
		{			
			DrawStringS(x + 100, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, Options.Activate480p ? "ON" : "OFF"); 					
			DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "Allow 480p"); y += fh; c++;			
		}	
		
		sprintf(intensity, "Scanline intensity: %d", GetScanlineIntensity()	);
		if(vmode == VIDEO_480P_SL)
		{
			DrawStringS(x, y + fh, r, sel == c ? 0 : g, sel == c ? 0 : b, intensity); y += fh; c++;			
			
			DrawStringS(x + 100, y + fh, r, sel == c ? 0 : g,	sel == c ? 0 : b, ScanlinesEven() ? "EVEN" : "ODD"); 					
			DrawStringS(x, y + fh, r, sel == c ? 0 : g, sel == c ? 0 : b, "Scanlines"); y += fh; c++;	
		}				
		else
		{
			DrawStringS(x, y + fh, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, intensity); y += fh; c++;			
			
			DrawStringS(x + 100, y + fh, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, ScanlinesEven() ? "EVEN" : "ODD"); 					
			DrawStringS(x, y + fh,sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "Scanlines"); y += fh; c++;	
		}
			
		DrawStringS(x, y + 2* fh, r, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); 		
				
		if(vmode != VIDEO_480P_SL && (sel == 3 || sel == 4))
			DrawStringS(x-40, y + 4*fh, r, g, b, "Scanlines are only available in\n480 Line Doubled mode"); 		
#ifdef WII_VERSION
		if(Options.ShowWiiRegion)
			DrawStringS(215, 215, r, g, b, wiiregion);
#endif				
				
		EndScene();		
        
        WPAD_ScanPads();

		pressed = WPAD_ButtonsDown(0);
		

		if ( pressed & WPAD_BUTTON_UP )
	    {
		    sel --;
		    if(sel < 1)
			    sel = c;		
	    }
	    
	    if ( pressed & WPAD_BUTTON_DOWN )
	    {
		    sel ++;
		    if(sel > c)
			    sel = 1;	
	    }			
		
		if ( pressed & WPAD_BUTTON_PLUS && sel == 3)
	    {
			if(vmode == VIDEO_480P_SL)
				RaiseScanlineIntensity();
	    }
	    
	    if ( pressed & WPAD_BUTTON_MINUS && sel == 3)
	    {
			if(vmode == VIDEO_480P_SL)
				LowerScanlineIntensity();
	    }			
			
		if ( pressed & WPAD_BUTTON_B ) 		
			close = 1;	
	
		if (pressed & WPAD_BUTTON_A)
		{     
			switch(sel)
			{			
					case 1:
#ifdef WII_VERSION		
						Options.ShowWiiRegion = !Options.ShowWiiRegion;
						if(Options.ShowWiiRegion)
							GetWiiRegion();
#endif
						break;
					case 2:
						if(!(Options.Activate480p && vmode >= VIDEO_480P))
							Options.Activate480p = !Options.Activate480p;
						break;
					case 3:						
						break;				
					case 4:
						if(vmode == VIDEO_480P_SL)
							ToggleScanlineEvenOdd();
						break;
					case 5:
						close = 1;
						break;
					default:
						break;
			} 			            										
		}		
	}
	
	FreeImage(&back);

	return;
}

void SelectVideoMode(ImagePtr title, ImagePtr sd)
{
	int 		sel = 1, close = 0;		
	ImagePtr	back;
	
	back = LoadImage(HELPIMG, 1);
	if(!back)
		return;
		
	back->alpha = 0xaa;
	while(!close) 
	{		
		u8      r = 0xff;
		u8      g = 0xff;
		u8      b = 0xff;
		u8   	c = 1;				    					   
		u16     x = 80;
		u16     y = 55;
        u32     pressed = 0;
				
		StartScene();
		        
		DrawImage(title);		
		DrawImage(back);
        DrawImage(sd);

		DrawStringS(x - 20, y, 0x00, 0xff, 0x00, "Please select the desired video mode"); y += 2*fh; 
		
		DrawStringS(x - 10, y + (vmode * fh), 0x00, 0xff, 0x00, ">"); 
		
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "240p"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "480i with 240p resources (Fake 480i)"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "480i with mixed 480p/240p resources"); y += fh; c++;
		if(Options.Activate480p && VIDEO_HaveComponentCable())
		{
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "480p with mixed 480p/240p resources"); y += fh; c++;
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "480p with 240p resources & scanlines"); y += fh; c++;
		}
		else
		{
			DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "480p with mixed 480p/240p resources"); y += fh; c++;
			DrawStringS(x, y, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, sel == c ? 0x77 : 0xAA, "480p with 240p resources & scanlines"); y += fh; c++;
		}	
			
		DrawStringS(x, y + fh, r, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); 		
		
		if(VIDEO_HaveComponentCable())		
		{
			DrawStringS(215, 225, r, g,	 b, "Component");
			if(sel >= 4 && sel != 6 && !Options.Activate480p)
				DrawStringS(x-40, y + 4*fh, r, g, b, "You can activate 480p from Options"); 		
		}
		else
		{
			DrawStringS(215, 225, r, g,	 b, "Composite");				
			if(sel >= 4 && sel != 6)
				DrawStringS(x-40, y + 4*fh, r, g, b, "You need a component cable for 480p"); 		
		}
		
#ifdef WII_VERSION
		if(Options.ShowWiiRegion)
			DrawStringS(215, 215, r, g, b, wiiregion);
#endif				
				
		EndScene();		
        
        WPAD_ScanPads();

		pressed = WPAD_ButtonsDown(0);
		
		if ( pressed & WPAD_BUTTON_UP )
	    {
		    sel --;
		    if(sel < 1)
			    sel = c;		
	    }
	    
	    if ( pressed & WPAD_BUTTON_DOWN )
	    {
		    sel ++;
		    if(sel > c)
			    sel = 1;	
	    }			
			
		if ( pressed & WPAD_BUTTON_B ) 		
			close = 1;	
	
		if (pressed & WPAD_BUTTON_A)
		{     
			switch(sel)
			{			
					case 1:
						SetVideoMode(VIDEO_240P);
						SetupGX();
						break;
					case 2:
						SetVideoMode(VIDEO_480I_A240);
						SetupGX();
						break;
					case 3:
						SetVideoMode(VIDEO_480I);
						SetupGX();
						break;
					case 4:
						if(Options.Activate480p)
						{
							SetVideoMode(VIDEO_480P);
							SetupGX();
						}
						break;
					case 5:
						if(Options.Activate480p)
						{
							SetVideoMode(VIDEO_480P_SL);
							SetupGX();
						}
						break;					
					case 6:
						close = 1;
						break;
					default:
						break;
			} 			            										
		}		
	}
	FreeImage(&back);

	return;
}

#ifdef WII_VERSION

void GetWiiRegion()
{
	s32 ret;
	char serialcode[SERIALCODE_SIZE] = "???\0";
	char model[MODEL_SIZE] = {0};	
	
	ret = CONF_GetSerialCode(serialcode);	
	switch (ret) {
		case CONF_CODE_JPN:
			// LJF - Confirmed
			sprintf(wiiregion, "Japanese Wii");
			break;
		case CONF_CODE_USA:
			// LU - Confirmed
			sprintf(wiiregion, "USA Wii");
			break;
		case CONF_CODE_EURH:
			// LEH - Confirmed
		case CONF_CODE_EURM:
			// LEM - Confirmed
		case CONF_CODE_EURF:
			// LEF - Confirmed
			sprintf(wiiregion, "European Wii");
			break;
		case CONF_CODE_KOR:
			// LKM - Confirmed
			sprintf(wiiregion, "Korean Wii");
			break;
		case CONF_CODE_AUS:
			// LAH - Confirmed
			sprintf(wiiregion, "n Australian Wii");
			break;
		case CONF_CODE_USAK:
			// KU - Confirmed (thanks LiNkZoR)
			sprintf(wiiregion, "new USA Wii");
			break;
		case CONF_CODE_EURHK:
			// KEH - Confirmed (thanks JohnyNyga)
		case CONF_CODE_EURMK:
			// KEM - Confirmed (thanks VampireLordAlucard)
		case CONF_CODE_EURFK:
			// KEF - Confirmed (thanks sureiya)
			sprintf(wiiregion, "new European Wii");
			break;
		case CONF_CODE_AUSK:
			// KAM - Confirmed (thanks Drexyl)
			sprintf(wiiregion, "new Australian Wii");
			break;
		case CONF_CODE_DLPH:
			// LE - Confirmed by myself
			sprintf(wiiregion, "Running Dolphin");
			break;
		case CONF_CODE_VJPNI:
			sprintf(wiiregion, "n 8GB Japanese Wii U");
			break;
		case CONF_CODE_VJPNO:
			sprintf(wiiregion, "32GB Wii U");
			break;
		case CONF_CODE_VUSAI:
			sprintf(wiiregion, "8GB USA Wii U");
			break;
		case CONF_CODE_VUSAO:
			sprintf(wiiregion, "32GB USA Wii U");
			break;
		case CONF_CODE_VEURHI:
		case CONF_CODE_VEURMI:
		case CONF_CODE_VEURFI:
			sprintf(wiiregion, "n 8GB European Wii U");
			break;
		case CONF_CODE_VEURHO:
		case CONF_CODE_VEURMO:
		case CONF_CODE_VEURFO:
			sprintf(wiiregion, "32GB European Wii U");
			break;
		case CONF_CODE_VKORI:
			sprintf(wiiregion, "n 8GB Korean Wii U");
			break;
		case CONF_CODE_VKORO:
			sprintf(wiiregion, "32GB Korean Wii U");
			break;
		case CONF_CODE_VAUSI:
			sprintf(wiiregion, "n 8GB Australian Wii U");
			break;
		case CONF_CODE_VAUSO:
			sprintf(wiiregion, "32GB Australian Wii U");
			break;
		case CONF_EBADVALUE:			
			if (!CONF_GetModel(model)) 
				sprintf(wiiregion, "Unknown SC %s model %s", serialcode, model);
			else
				sprintf(wiiregion, "Unknown SC %s", serialcode);			
			break;
		default:
			sprintf(wiiregion, "Error getting region");			
			break;
	}
}

#endif