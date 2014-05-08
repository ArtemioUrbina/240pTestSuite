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

#include "patterns.h"

#ifdef WII_VERSION
#include "CheckRegion.h"
#endif

void TestPatternsMenu(ImagePtr title, ImagePtr sd);
void DrawCredits(ImagePtr Back);
void DrawIntro();

#ifdef WII_VERSION
char wiiregion[100];
void GetWiiRegion();
#endif

int main(int argc, char **argv) 
{
	int 		video = NATIVE_320, close = 0;	
	ImagePtr 	Back = NULL, sd = NULL;
	u8   		sel = 1;	
		
	VIDEO_Init();
		
	WPAD_Init();

	SetVideoMode(video);
	InitGX();
	SetupGX();

    LoadFont();
	Back = LoadImage(BACKIMG, 0);
	if(!Back)
    {
        ReleaseFont();
		exit(0);
    }	
    sd = LoadImage(SDIMG, 0);
    if(!sd)
    {
        ReleaseFont();
		exit(0);
    }
    
    sd->x = 221;
    sd->y = 94;
			
	DrawIntro();
	
#ifdef WII_VERSION
	GetWiiRegion();
#endif
	
	while(!close) 
	{        
        char	res[40];
		u8      r = 0xff;
		u8      g = 0xff;
		u8      b = 0xff;
		u8   	c = 1;				    					   
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
        DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Alternating 240p/480i Test"); y += fh; c++;
        DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Sound Test"); y += fh; c++;
        switch(vmode)
		{
			case NATIVE_320:
				sprintf(res, "Video: 240p");				
				break;			
			case FAKE_640:
				sprintf(res, "Video: Fake 480i");
				break;
			case NATIVE_640_FS:
				sprintf(res, "Video: 480p");
				break;				
		}
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, res); y += fh; c++;
        DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Help"); y += fh; c++;
		DrawStringS(x, y + fh, r, sel == c ? 0 : g, sel == c ? 0 : b, "Credits"); y += fh; 
		
		if(VIDEO_HaveComponentCable())		
			DrawStringS(215, 225, r, g,	 b, "Component");
		else
			DrawStringS(215, 225, r, g,	 b, "Composite");		

#ifdef WII_VERSION
		DrawStringS(215, 215.0f, r, g, b, wiiregion);
#endif				
		
		EndScene();

		VIDEO_Flush();
		VIDEO_WaitVSync();

        WPAD_ScanPads();
		
		pressed = WPAD_ButtonsDown(0);

		// We return to the launcher application via exit
		if ( pressed & WPAD_BUTTON_HOME ) 		
			close = 1;
		
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

        if (pressed & WPAD_BUTTON_A)
		{		
			switch(sel)
			{
				case 1:
					TestPatternsMenu(Back, sd);
					break;
				/*
				case 2:					
					DropShadowTest();
					break;
				case 3:
					StripedSpriteTest();
					break;
				case 4:
					PassiveLagTest();
					break;
				case 5:
					LagTest();
					break;
				case 6:
					ScrollTest();
					break;
				case 7:
					GridScrollTest();
					break;
				case 8:
					DrawStripes();
					break;
				case 9:
					DrawCheckBoard();
					break;
				case 10:
					LEDZoneTest();
					break;
				case 11:
					Alternate240p480i();					
					break;
				case 12:
					SoundTest();
					break;
				*/
				case 13:
					video ++;
					
					if(!VIDEO_HaveComponentCable() && video >= NATIVE_640_FS)
						video ++;
						
			        if(video >= TOTAL_VMODES)
				        video = 0;							
	        
			        SetVideoMode(video);
			        SetupGX();
					break;
				/*
				case 14:
					HelpWindow(GENERALHELP, title);
					break;
				*/
				case 15:
					DrawCredits(Back);
					break;
			} 									
		}
	}
		
	FreeImage(&Back);
	ReleaseFont();	

	return 0;
}

void TestPatternsMenu(ImagePtr title, ImagePtr sd)
{
	int 			sel = 1, close = 0;		
	
	while(!close) 
	{		
		u8      r = 0xff;
		u8      g = 0xff;
		u8      b = 0xff;
		u8   	c = 1;				    					   
		u16     x = 40;
		u16     y = 55;
        u32     pressed = 0;
				
		StartScene();
		        
		DrawImage(title);
        DrawImage(sd);

		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Pluge"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Color Bars"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Color Bars with Gray Scale"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Color Bleed Check"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Grid"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Linearity"); y += fh; c++;		
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Gray Ramp"); y += fh; c++;		
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "White Screen"); y += fh; c++;				
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "100 IRE"); y += fh; c++;				
		DrawStringS(x, y + fh, r, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); y += fh; 

		if(VIDEO_HaveComponentCable())		
			DrawStringS(215, 225, r, g,	 b, "Component");
		else
			DrawStringS(215, 225, r, g,	 b, "Composite");				
		
#ifdef WII_VERSION
		DrawStringS(215, 215.0f, r, g, b, wiiregion);
#endif		
		
		//DrawScanlines();
				
		EndScene();		
        
        WPAD_ScanPads();

		pressed = WPAD_ButtonsDown(0);
		
        /*
		pressed = st->buttons & ~oldbuttons;
		oldbuttons = st->buttons;

		if (pressed & CONT_DPAD_RIGHT && st->buttons & CONT_Y)
			RaiseScanlineIntensity();

		if (pressed & CONT_DPAD_LEFT && st->buttons & CONT_Y)
			LowerScanlineIntensity();
        */

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
					DrawPluge();
					break;
				/*
				case 2:
					DrawColorBars();
					break;
				case 3:
					Draw601ColorBars();
					break;
				case 4:
					DrawColorBleed();
					break;
				case 5:
					DrawGrid();
					break;
				case 6:
					DrawLinearity();
					break;					
					*/
				case 7:
					DrawGrayRamp();
					break;
				case 8:
					DrawWhiteScreen();
					break;
					/*
				case 9:
					Draw100IRE();
					break;
				*/
				case 10:
					close = 1;
					break;
			} 			            										
		}
		
		VIDEO_Flush();
		VIDEO_WaitVSync();			
	}

	return;
}

void DrawCredits(ImagePtr Back)
{
	int 	done = 0;	
    u32     pressed = 0;		
#ifdef WII_VERSION
	u8 		shopcode = 0;
	char	shop[100];	
	
	if (CONF_GetShopCode(&shopcode) >= 0) {
		sprintf(shop, "Shop: (%u) %s\n", shopcode, (strlen(CONF_CountryCodes[shopcode]) ? CONF_CountryCodes[shopcode] : "Unknown Country"));
	} else {		
		sprintf(shop, "Error getting shop code!\n");			
	}
#endif
	
	while(!done) 
	{
		int x = 30, y = 52;

		WPAD_ScanPads();
		pressed = WPAD_ButtonsDown(0);
		
        if (pressed & WPAD_BUTTON_PLUS)
			DrawIntro();
		if (pressed & WPAD_BUTTON_B)
			done =	1;						

		StartScene();
		        
		DrawImage(Back);

		DrawStringS(x, y, 0x00, 0xff, 0x00, "Code and Patterns:"); y += fh; 
		DrawStringS(x+5, y, 0xff, 0xff, 0xff, "Artemio Urbina"); y += fh; 
		DrawStringS(x, y, 0x00, 0xff, 0x00, "SDK:"); y += fh; 
		DrawStringS(x+5, y, 0xff, 0xff, 0xff, "devkitPPC"); y += fh; 		
		DrawStringS(x, y, 0x00, 0xff, 0x00, "Menu Pixel Art:"); y += fh; 
		DrawStringS(x+5, y, 0xff, 0xff, 0xff, "Asher"); y += fh; 		
		DrawStringS(x, y, 0x00, 0xff, 0x00, "Advisor:"); y += fh; 
		DrawStringS(x+5, y, 0xff, 0xff, 0xff, "Fudoh"); y += fh; 
		DrawStringS(x, y, 0x00, 0xff, 0x00, "Collaboration:"); y += fh; 
		DrawStringS(x+5, y, 0xff, 0xff, 0xff, "Konsolkongen & shmups regulars"); y += fh; 
		DrawStringS(x, y, 0x00, 0xff, 0x00, "Info on using this suite:"); y += fh; 
		DrawStringS(x+5, y, 0xff, 0xff, 0xff, "http://junkerhq.net/xrgb/"); y += 2*fh; 
		
		DrawStringS(x, y, 0x00, 0xaa, 0xaa, "This program is free Software");  y += fh;
		DrawStringS(x, y, 0x00, 0xaa, 0xaa, "Source code is available under GPL.");  y += fh;
		DrawStringS(x, y, 0x00, 0xaa, 0xaa, "Includes libcheckregion.");

		y = 58;
		
		DrawStringS(200, y, 0xff, 0xff, 0xff, "Wii GX Ver. 1.18"); y += fh;
		DrawStringS(200, y, 0xff, 0xff, 0xff, "08/05/2014"); y += 2*fh;

#ifdef WII_VERSION		
		DrawStringS(200, y, 0xee, 0xee, 0xee, shop); y += fh;
		DrawStringS(200, y, 0xee, 0xee, 0xee, wiiregion); y += fh;
#endif

		EndScene();

		VIDEO_Flush();
		VIDEO_WaitVSync();
	}
}

void DrawIntro()
{    
	u32				counter, frames = 60;
	int				delta;
	ImagePtr		black;

	black = LoadImage(BLACKIMG, 1);
	if(!black)
		return;

	black->alpha = 0xff;
	delta = 1;
	black->w = dW;
	black->h = dH;
	black->layer = 5.0f;
	for(counter = 0; counter < frames*2; counter ++)
	{
		black->alpha -= delta;
		if(black->alpha < 0x00)
			black->alpha = 0x00;

		StartScene();
	
		DrawStringS(120, 115, 0xff, 0xff, 0xff, "KORDAMP PRESENTS");
		DrawImage(black);
		
		EndScene();

		VIDEO_Flush();
		VIDEO_WaitVSync();

		if(counter == frames)
			delta *= -1;
	}
	FreeImage(&black);
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