/* 
 * 240p Test Suite
 * Copyright (C)2014-2022 Artemio Urbina (Wii GX)
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "tests.h"
#include "image.h"
#include "font.h"
#include "video.h"

#include "help.h"
#include "menu.h"
#include "controller.h"
#include "options.h"
#include <asndlib.h>
#undef MAX_VOICES
#undef SND_BUFFERSIZE
#include <aesndlib.h>

#include "beep_snd.h"

typedef struct timecode{
	int hours;
	int minutes;
	int seconds;
	int frames;
	int type;
	int res;
} timecode;

void DropShadowTest()
{
	char		msg[50];
	int			done = 0, x = dW/2, y = dH/2, invert = 0, frame = 0, text = 0, selback = 0, sprite = 0;
	int			oldvmode = vmode, i = 0, currentsonic = 0, currentframe = 0;
	u32			pressed, held, reload = 0;		
	ImagePtr	back[4], sonicback[4], ssprite, shadow, buzz, buzzshadow, overlay;	

	sonicback[0] = LoadImage(SONICBACK1IMG, 0);
	if(!sonicback[0])
		return;
	sonicback[1] = LoadImage(SONICBACK2IMG, 0);
	if(!sonicback[1])
		return;
	sonicback[2] = LoadImage(SONICBACK3IMG, 0);
	if(!sonicback[2])
		return;
	sonicback[3] = LoadImage(SONICBACK4IMG, 0);
	if(!sonicback[3])
		return;

	for(i = 0; i < 4; i++)
		back[i] = NULL;
		
	back[1] = sonicback[0];
	if(!back[1])
		return;
	
	overlay = LoadImage(SONICFLOORIMG, 0);
	if(!overlay)
		return;
	
	reload = 1;
		
	ssprite = LoadImage(SHADOWIMG, 0);	
	if(!ssprite)
		return;
	buzz = LoadImage(BUZZBOMBERIMG, 0);
	if(!buzz)
		return;
	buzzshadow = LoadImage(BUZZBOMBERSHADOWIMG, 0);
	if(!buzzshadow)
		return;
	
	srand((int)(time(0)));
	sprite = rand() % 2;
	if(sprite == 0)
		shadow = ssprite;
	else
		shadow = buzzshadow;
	
	while(!done && !EndProgram) 
	{
		if(reload || oldvmode != vmode)
		{
			FreeImage(&back[0]);
			FreeImage(&back[2]);
			FreeImage(&back[3]);

			back[2] = LoadImage(CHECKPOSIMG, 1);
			if(!back[2])
				return;
			back[3] = LoadImage(STRIPESPOSIMG, 1);
			if(!back[3])
				return;
			
			if(vmode != VIDEO_480P && vmode != VIDEO_480I &&
				vmode != VIDEO_576I)
			{		
				back[0] = LoadImage(DONNAIMG, 0);
				if(!back[0])
					return;
				
				for(i = 0; i < 4; i++)
					sonicback[i]->scale = 1;
				back[0]->scale = 1;
				overlay->scale = 1;
				
				for(i = 0; i < 4; i++)
					sonicback[i]->y = 0;
				overlay->y = 0;
			}
			else
			{	
				back[0] = LoadImage(DONNA480IMG, 0);
				if(!back[0])
					return;
				
				back[0]->scale = 0;
				overlay->scale = 0;
				for(i = 0; i < 4; i++)
				{
					sonicback[i]->scale = 0;
					sonicback[i]->y = (dH - 240)/2;
				}

				overlay->y = (dH - 240)/2;
				if(offsetY)  // center in PAL modes
				{
					for(i = 0; i < 4; i++)
						sonicback[i]->y -= offsetY;
					overlay->y -= offsetY;
				}
			}
			reload = 0;
			oldvmode = vmode;
		}
		
		if(selback == 1)
		{
			CalculateUV(x, 0, dW, 240, back[selback]);
			CalculateUV(x*2, 0, dW, 240, overlay);
		}	
		
		StartScene();
		DrawImage(back[selback]);
		if(selback == 1)
			DrawImage(overlay);

		if(text)
		{
			if(vmode != VIDEO_480P)
				DrawStringB(140, 30, 0, 0xff, 0, msg);
			else
				DrawStringB(450, 40, 0, 0xff, 0, msg);
			text --;
		}

		if(frame == invert)
		{
			shadow->x = x;
			shadow->y = y;
			DrawImage(shadow);
		}
		frame = !frame;
				
		if(sprite == 1)
		{
			buzz->x = x - 20;
			buzz->y = y - 20;
			DrawImage(buzz);
		} 			
		EndScene();
		
		ControllerScan();
		
		held = Controller_ButtonsHeld(0);		
				
		if (held & PAD_BUTTON_UP)
			y --;
	
		if (held & PAD_BUTTON_DOWN)
			y ++;
	
		if (held & PAD_BUTTON_LEFT)
		{
			x --;
			FlipH(buzz, 0);
			FlipH(buzzshadow, 0);
		}
	
		if (held & PAD_BUTTON_RIGHT)
		{
			x ++;
			FlipH(buzz, 1);
			FlipH(buzzshadow, 1);
		}
		
		pressed = Controller_ButtonsDown(0);
		if (pressed & PAD_BUTTON_B)
			done =	1;

		if (pressed & PAD_BUTTON_X)
		{
			invert = !invert;
			if(invert)
				sprintf(msg, "Shadow on odd frames");				
			else
				sprintf(msg, "Shadow on even frames");
			text = 60;
		}
			
		if (pressed & PAD_BUTTON_A)
		{
			if(selback < 3)
				selback ++;
			else
				selback = 0;
		}
	
		if (pressed & PAD_BUTTON_Y)
		{
			if(sprite == 0)
			{
				shadow = buzzshadow;
				sprite = 1;
			}
			else
			{
				shadow = ssprite;
				sprite = 0;
			}
		}

		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = DROPSHADOW;
		}	

		if(x < back[selback]->x)
			x = back[selback]->x;
		if(y < back[selback]->y)
			y = back[selback]->y;
		if(x > back[selback]->w + back[selback]->x - shadow->w)
			x = back[selback]->w + back[selback]->x - shadow->w;
		if(y > back[selback]->h + back[selback]->y - shadow->h)
			y = back[selback]->h + back[selback]->y - shadow->h;

		currentframe ++;
		if(currentframe > 10)
		{
			currentsonic++;
			if(currentsonic > 3)
				currentsonic = 0;
			currentframe = 0;
			back[1] = sonicback[currentsonic];
		}
	}
	FreeImage(&back[0]);
	for(i = 0; i < 4; i++)
		FreeImage(&sonicback[i]);
	FreeImage(&back[2]);
	FreeImage(&back[3]);
	FreeImage(&overlay);
	FreeImage(&ssprite);
	FreeImage(&buzz);
	FreeImage(&buzzshadow);
}

void StripedSpriteTest()
{	
	int			done = 0, x = dW/2, y = dH/2, selback = 0;
	int			oldvmode = vmode, currentsonic = 0, currentframe = 0, i;
	u32			pressed, held, reload = 0;	
	ImagePtr	back[4], sonicback[4], striped, overlay;	

	sonicback[0] = LoadImage(SONICBACK1IMG, 0);
	if(!sonicback[0])
		return;
	sonicback[1] = LoadImage(SONICBACK2IMG, 0);
	if(!sonicback[1])
		return;
	sonicback[2] = LoadImage(SONICBACK3IMG, 0);
	if(!sonicback[2])
		return;
	sonicback[3] = LoadImage(SONICBACK4IMG, 0);
	if(!sonicback[3])
		return;
	
	for(i = 0; i < 4; i++)
		back[i] = NULL;
		
	back[1] = sonicback[0];
	if(!back[1])
		return;
	
	overlay = LoadImage(SONICFLOORIMG, 0);
	if(!overlay)
		return;
	
	reload = 1;
	striped = LoadImage(STRIPEDIMG, 0);
	if(!striped)
		return;
			
	while(!done && !EndProgram) 
	{	
		if(reload || oldvmode != vmode)
		{
			FreeImage(&back[0]);
			FreeImage(&back[2]);
			FreeImage(&back[3]);
				
			back[2] = LoadImage(CHECKPOSIMG, 1);
			if(!back[2])
				return;
			back[3] = LoadImage(STRIPESPOSIMG, 1);
			if(!back[3])
				return;
				
			if(vmode != VIDEO_480P && vmode != VIDEO_480I && 
				vmode != VIDEO_576I)
			{	
				back[0] = LoadImage(DONNAIMG, 0);
				if(!back[0])
					return;
					
				for(i = 0; i < 4; i++)
					sonicback[i]->scale = 1;
				back[0]->scale = 1;
				overlay->scale = 1;
				
				for(i = 0; i < 4; i++)
					sonicback[i]->y = 0;
				overlay->y = 0;
			}
			else
			{	
				back[0] = LoadImage(DONNA480IMG, 0);
				if(!back[0])
					return;
	
				back[0]->scale = 0;
				overlay->scale = 0;
				for(i = 0; i < 4; i++)
				{
					sonicback[i]->scale = 0;
					sonicback[i]->y = (dH - 240)/2;
				}

				overlay->y = (dH - 240)/2;
				if(offsetY)  // center in PAL modes
				{
					for(i = 0; i < 4; i++)
						sonicback[i]->y -= offsetY;
					overlay->y -= offsetY;
				}
			}
			reload = 0;
			oldvmode = vmode;
		}
	
		StartScene();
		if(selback == 1)
		{
			CalculateUV(x, 0, dW, 240, back[selback]);
			CalculateUV(x*2, 0, dW, 240, overlay);
		}
		DrawImage(back[selback]);
		if(selback == 1)
			DrawImage(overlay);

		striped->x = x;
		striped->y = y;
		DrawImage(striped);

		EndScene();
		
		ControllerScan();
		
		held = Controller_ButtonsHeld(0);		

		if (held & PAD_BUTTON_UP)
			y --;
	
		if (held & PAD_BUTTON_DOWN)
			y ++;

		if (held & PAD_BUTTON_LEFT)
			x --;

		if (held & PAD_BUTTON_RIGHT)
			x ++;
			

		pressed = Controller_ButtonsDown(0);
		if (pressed & PAD_BUTTON_B)
			done =	1;
					
		if (pressed & PAD_BUTTON_A)
		{
			if(selback > 0)
				selback --;
			else
				selback = 3;
		}
		
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = STRIPED;
		}			

		if(x < back[selback]->x)
			x = back[selback]->x;
		if(y < back[selback]->y)
			y = back[selback]->y;
		if(x > back[selback]->w + back[selback]->x - striped->w)
			x = back[selback]->w + back[selback]->x - striped->w;
		if(y > back[selback]->h + back[selback]->y - striped->h)
			y = back[selback]->h + back[selback]->y - striped->h;
			
		currentframe ++;
		if(currentframe > 10)
		{
			currentsonic++;
			if(currentsonic > 3)
				currentsonic = 0;
			currentframe = 0;
			back[1] = sonicback[currentsonic];
		}
	}
	FreeImage(&back[0]);
	for(i = 0; i < 4; i++)
		FreeImage(&sonicback[i]);
	FreeImage(&back[2]);
	FreeImage(&back[3]);
	FreeImage(&overlay);
	FreeImage(&striped);
}

void TimingReflexTest()
{
	char			msg[60], strings[100];
	int				clicks[10], done = 0, view = 0, speed = 1, change = 1;
	int				x, y, x2, y2, pos = 0, i = 0, vary = 0, variation = 1;
	u16				pressed, audio = 0, rumble = 0;
	ImagePtr		back, spriteA, spriteB, fixed;
	char 			**ControllerButtons = NULL;
	
	
	srand((int)(time(0)));	
	back = LoadImage(WHITEIMG, 1);
	if(!back)
		return;
	
	back->r = 0x00;
	back->g = 0x00;
	back->b = 0x00;
	
	fixed = LoadImage(LAGPERIMG, 0);
	if(!fixed)
		return;
	spriteA = LoadImage(LAGPERIMG, 0);
	if(!spriteA)
	{
		FreeImage(&fixed);
		return;
	}
	spriteB = LoadImage(LAGPERIMG, 0);
	if(!spriteB)
	{
		FreeImage(&spriteA);
		FreeImage(&fixed);
		return;
	}
		
	ASND_Init(); 
	ASND_Pause(0); 	

	x = 144;
	y = 60;
	x2 = 108;
	y2 = 96;
		
	fixed->x = 144;
	fixed->y = 96;	

	for(i = 0; i < 10; i++)
		clicks[i] = 0xFF;

	while(!done && !EndProgram) 
	{
		StartScene();

		DrawImage(back);
		DrawImage(fixed);

		if(y > 132 + vary)
		{
			speed = -1;
			change = 1;
			if(variation)
			{
				if(rand() % 2)
					vary = rand() % 7;
				else
					vary = -1 * rand() % 7;
			}
		}

		if(y < 60 + vary)
		{
			speed = 1;
			change = 1;
			if(variation)
			{
				if(rand() % 2)
					vary = rand() % 7;
				else
					vary = -1 * rand() % 7;
			}
		}

		y += speed;
		x2 += speed;

		spriteA->x = x;
		spriteA->y = y;
		spriteB->x = x2;
		spriteB->y = y2;
		
		if(y == 96)
		{			
			if(audio)
				ASND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_16BIT, 48000, 0, (void*)beep_snd, beep_snd_size, speed == -1 ? MIN_VOLUME : MAX_VOLUME, speed == -1 ? MAX_VOLUME : MIN_VOLUME, NULL);
				
			if(rumble)
				ControllerRumble(0, 1);
			
			spriteA->r = 0xff;
			spriteA->g = 0x00;
			spriteA->b = 0x00;

			spriteB->r = 0xff;
			spriteB->g = 0x00;
			spriteB->b = 0x00;
		}
		else
		{
			if(rumble)
				ControllerRumble(0, 0);
				
			if(y == 97 || y == 95) // one pixel off
			{
				spriteA->r = 0x00;
				spriteA->g = 0xff;
				spriteA->b = 0x00;

				spriteB->r = 0x00;
				spriteB->g = 0xff;
				spriteB->b = 0x00;
			}

			if(y == 98 || y == 94) // two pixels off
			{
				spriteA->r = 0xff;
				spriteA->g = 0xff;
				spriteA->b = 0xff;

				spriteB->r = 0xff;
				spriteB->g = 0xff;
				spriteB->b = 0xff;
			}
		}			

		if(view == 0 || view == 2)
			DrawImage(spriteA);

		if(view == 1 || view == 2)
			DrawImage(spriteB);

		for(i = 0; i < 10; i++)
		{
			if(clicks[i] != 0xFF)
			{
				int px, py;

				px = 10;
				py = i*fh + 20;
	
				sprintf(msg, "Offset %02d:", i + 1);
				DrawStringS(px, py, 0xff, 0xff, 0xff, msg);
			
				px += (strlen(msg) + 2)*fw;
				if(clicks[i] == 1)
					sprintf(msg, "%2d frame", clicks[i]);
				else
					sprintf(msg, "%2d frames", clicks[i]);

				if(clicks[i] >= 0)
				{
					if(clicks[i] == 0)
						DrawStringS(px, py, 0x00, 0xff, 0x00, msg);
					else
						DrawStringS(px, py, 0xff, 0xff, 0xff, msg);
				}
				else
					DrawStringS(px, py, 0xff, 0x00, 0x00, msg);
			}
		}

		sprintf(msg, "Audio: %s", audio ? "on" : "off");
		DrawStringS(200, 20, 0xff, 0xff, 0xff, msg);
		sprintf(msg, "Timing: %s", variation ? "random" : "rhythmic");
		DrawStringS(200, 20+fh, 0xff, 0xff, 0xff, msg);		
		sprintf(msg, "Rumble: %s", rumble ? "on" : "off");
		DrawStringS(200, 20+2*fh, 0xff, 0xff, 0xff, msg);		

#ifdef WII_VERSION
		if(ControllerType == ControllerWiimote)
			ControllerButtons = ControlNamesWii;
			
		if(ControllerType == ControllerWiiClassic)
		{
			if(Options.SFCClassicController)
				ControllerButtons = ControlNamesWiiSFC_CC;
			else
				ControllerButtons = ControlNamesWiiCC;
		}
			
		if(ControllerType == ControllerGC)
#endif
			ControllerButtons = ControlNamesGC;
			
		sprintf(strings, "Press \"%c\" when the sprite is aligned with the background.", ControllerButtons[ControlButtonA][0]);
		DrawStringS(20, 170, 0x00, 0xff, 0x00, strings);
		
		sprintf(strings, "Negative values mean you pressed \"%c\" before they intersected", ControllerButtons[ControlButtonA][0]);
		DrawStringS(20, 170+fh, 0x00, 0xff, 0x00, strings);
		
		sprintf(strings, "\"%c\" button toggles horizontal and vertical movement.", ControllerButtons[ControlButtonX][0]);
		DrawStringS(20, 170+2*fh, 0x00, 0xff, 0x00, strings);
		
		sprintf(strings, "\"%c\" trigger toggles rhythmic timing.", ControllerButtons[ControlButtonY][0]);
		DrawStringS(20, 170+3*fh, 0x00, 0xff, 0x00, strings);
		
		sprintf(strings, "\"%c\" button toggles audio feedback.", ControllerButtons[ControlButtonR][0]);
		DrawStringS(20, 170+4*fh, 0x00, 0xff, 0x00, strings);
		
		sprintf(strings, "\"%c\" button toggles rumble feedback.", ControllerButtons[ControlButtonL][0]);
		DrawStringS(20, 170+5*fh, 0x00, 0xff, 0x00, strings);

		EndScene();
		
		ControllerScan();
		pressed = Controller_ButtonsDown(0);
		if (pressed & PAD_BUTTON_B)
			done =	1;
			
		if (pressed & PAD_BUTTON_START)		
		{
			DrawMenu = 1;					
			HelpData = TIMINGLAG;
		}
			
		if (pressed & PAD_BUTTON_A)
		{
			if(change)
			{
				clicks[pos] = (y - 96) *speed;
	
				sprintf(msg, " Off: %d", clicks[pos]);					
	
				if(clicks[pos] >= 0)
				{
					change = 0;
					pos ++;
				}
		
				if(pos > 9)
					done = 1;
			}
		}

		if (pressed & PAD_TRIGGER_R)
			audio =	!audio;	
			
		if (pressed & PAD_TRIGGER_L)
		{
			rumble = !rumble;	
			if(!rumble)
				ControllerRumble(0, 0);
		}
			
		if (pressed & PAD_BUTTON_X)
		{
			view ++;
			if(view > 2)
				view = 0;
		}
					
						
		if(pressed & PAD_BUTTON_Y)
		{
			variation = !variation;
			if(!variation)
				vary = 0;
		}
	}
	
	ControllerRumble(0, 0);

	FreeImage(&back);
	FreeImage(&fixed);
	FreeImage(&spriteA);
	FreeImage(&spriteB);	
	
	if(pos > 9)
	{
		int	total = 0;
		double	res = 0, ms = 0;
		ImagePtr wall;
		
		done = 0;
		wall = LoadImage(BACKIMG, 0);
		if(!wall)
			return;
		
		while(!done && !EndProgram) 
		{			
			StartScene();

			DrawImage(wall);

			total = 0;
			for(i = 0; i < 10; i++)
			{
				int px, py;
	
				px = 70;
				py = i*fh + 70;
	
				sprintf(msg, "%2d", clicks[i]);
				if(clicks[i] >= 0)
				{
					if(clicks[i] == 0)
						DrawStringS(px, py, 0, 0xff, 0, msg);
					else
						DrawStringS(px, py, 0xff, 0xff, 0xff, msg);
				}
				else
					DrawStringS(px, py, 0xff, 0, 0, msg);
	
				if(clicks[i] >= 0)
					total += clicks[i];
	
			}

			if(!IsPAL)
			{
				res = (double)total / 10.0;
				ms = (double)(res*(1000.0/60.0));
				sprintf(msg, "%d/10 = %0.2f average frames ~= %0.2f ms", total, res, ms);
				DrawStringS(60, 110, 0xff, 0x00, 0x00, "+");
				DrawStringS(55, 70 + fh*10, 0xff, 0x00, 0x00, "_____");
				DrawStringS(60, 70 + fh*11, 0xff, 0xff, 0xff, msg);
				DrawStringS(30, 70 + fh*12, 0x00, 0xff, 0xff, "Keep in mind that an NTSC frame is around 16.67 ms");
			}
			else
			{
				res = (double)total / 10.0;
				ms = (double)(res*(1000.0/50.0));
				sprintf(msg, "%d/10 = %0.2f average frames ~= %0.2f ms", total, res, ms);
				DrawStringS(60, 110, 0xff, 0x00, 0xff, "+");
				DrawStringS(55, 70 + fh*10, 0xff, 0x00, 0x00, "_____");
				DrawStringS(60, 70 + fh*11, 0xff, 0xff, 0xff, msg);
				DrawStringS(40, 70 + fh*12, 0x00, 0xff, 0xff, "Keep in mind that a PAL frame is around 20 ms");
			}

			if(total && total < 5)
			{
				DrawStringS(100, 120, 0x00, 0xff, 0x00, "EXCELLENT REFLEXES!");
			}
			if(total == 0)
			{
				DrawStringS(100, 120, 0x00, 0xff, 0x00, "INCREDIBLE REFLEXES!!");
			}

			EndScene();
			
			ControllerScan();
		
			pressed = Controller_ButtonsDown(0);		
			
			if (pressed & PAD_BUTTON_B)
				done =	1;	
	
			if (pressed & PAD_BUTTON_START)		
			{
				DrawMenu = 1;					
				HelpData = TIMINGLAG;
			}
		}
		FreeImage(&wall);
	}
	SND_End();
	
	return;
}

void ScrollTest()
{
	int 	done = 0, speed = 1, acc = 1, x = 0, y = 0, pause = 0, vertical = 0;
	int		oldvmode = -1, i = 0, currentsonic = 0, currentframe = 0;
	u32		pressed;		
	ImagePtr	sonicback[4], overlay, kiki;	
	
	sonicback[0] = LoadImage(SONICBACK1IMG, 0);
	if(!sonicback[0])
		return;
	sonicback[1] = LoadImage(SONICBACK2IMG, 0);
	if(!sonicback[1])
		return;
	sonicback[2] = LoadImage(SONICBACK3IMG, 0);
	if(!sonicback[2])
		return;
	sonicback[3] = LoadImage(SONICBACK4IMG, 0);
	if(!sonicback[3])
		return;

	overlay = LoadImage(SONICFLOORIMG, 0);
	if(!overlay)
		return;
		
	kiki = LoadImage(KIKIBACKIMG, 0);
	if(!kiki)
		return;

	for(i = 0; i < 4; i++)
		IgnoreOffset(sonicback[i]);
	IgnoreOffset(overlay);
	IgnoreOffset(kiki);

	while(!done && !EndProgram) 
	{		
		if(oldvmode != vmode)
		{
			for(i = 0; i < 4; i++)	
				sonicback[i]->y = (dH - 240)/2;
			overlay->y = (dH - 240)/2;
			if(offsetY)  // center in PAL modes
			{
				for(i = 0; i < 4; i++)	
					sonicback[i]->y -= offsetY;
				overlay->y -= offsetY;
			}
			kiki->x = (dW-256)/2;
			oldvmode = vmode;
		}
		
		StartScene();

		if(speed > 15)
			speed = 15;

		if(speed < 1)
			speed = 1;

		if(!pause)
		{
			if(!vertical)
				x += speed * acc;
			else
				y -= speed * acc;
		}

		if(!vertical)
		{
			if(x > overlay->tw)
				x = 1;

			if(x < -1*overlay->tw)
				x = -1;
				
			CalculateUV(x, 0, dW, 240, sonicback[currentsonic]);
			CalculateUV(x*2, 0, dW, 240, overlay);
			DrawImage(sonicback[currentsonic]);
			DrawImage(overlay);
		}
		else
		{
			if(y > kiki->th)
				y = 1;

			if(y < -1*kiki->th)
				y = -1;
				
			CalculateUV(0, y, 256, dH, kiki);
			DrawImage(kiki);
		}
		
		EndScene();

		ControllerScan();

		pressed = Controller_ButtonsDown(0);
		
		if (pressed & PAD_BUTTON_UP)
			speed ++;

		if (pressed & PAD_BUTTON_DOWN)
			speed --;

		if (pressed & PAD_BUTTON_B)
			done = 1;

		if (pressed & PAD_BUTTON_A)
			pause = !pause;

		if (pressed & PAD_BUTTON_X)
			acc *= -1;
			
		if (pressed & PAD_BUTTON_Y)
			vertical = !vertical;

		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = SCROLL;
		}	

		if(!vertical)
		{
			currentframe ++;
			if(currentframe > 10)
			{
				currentsonic++;
				if(currentsonic > 3)
					currentsonic = 0;
				currentframe = 0;
			}
		}			
	}
	for(i = 0; i < 4; i++)
		FreeImage(&sonicback[i]);
	FreeImage(&overlay);
	FreeImage(&kiki);
}

void GridScrollTest()
{
	int 			done = 0, speed = 1, acc = 1, x = 0, y = 0, pause = 0, direction = 0;
	int				angle = 0, oldvmode = -1, subpixel = 0, subpixcnt = 0;
	u32				pressed;		
	ImagePtr		back, backsquares, backdiag;
		
	backdiag = LoadImage(DIAGONALIMG, 1);
	if(!backdiag)
		return; 
	backsquares = LoadImage(SMALLGRIDIMG, 1);
	if(!backsquares)
	{
		FreeImage(&backdiag);  
		return;  
	}

	backsquares->x = -0.5*dW;
	backsquares->y = -0.5*dH;
	back = backsquares;
	
	while(!done && !EndProgram) 
	{	
		Mtx m;
		
		if(oldvmode != vmode)
		{
			backsquares->w = dW;
			backsquares->h = dH;
			backdiag->w = dW;
			backdiag->h = dH;
	
			oldvmode = vmode;
		}
		
		StartSceneMtx(&m);

		if(speed > 5)
			speed = 5;

		if(speed < -5)
			speed = -5;

		if(!pause)
		{
			int advance = 0;
			
			if(!subpixel)
				advance = speed;
			else
			{
				subpixcnt ++;
				if(subpixcnt >= subpixel)
				{
					advance = 1;
					subpixcnt = 0;
				}
			}
			if(direction)
				x += advance * acc;
			else
				y += advance * acc;
		}	
				
		CalculateUV(x, y, dW*2, dH*2, back);	
		DrawImageRotate(back, angle, &m);
		
		/*
		{
			char str[100];
			
			sprintf(str, "speed: %d", speed);
			DrawStringS(20, 20, 0x00, 0xff, 0x00, str);
			sprintf(str, "subpixel: %d", subpixel);
			DrawStringS(20, 40, 0x00, 0xff, 0x00, str);
			sprintf(str, "subpixcnt: %d", subpixcnt);
			DrawStringS(20, 60, 0x00, 0xff, 0x00, str);
		}
		*/
	
		EndScene();

		ControllerScan();
				
		pressed = Controller_ButtonsDown(0);
		if (pressed & PAD_TRIGGER_R)
		{
			if(angle)
				angle = 0;
			else
			{
				angle = 45;
				back = backsquares;
			}
		}
		
		if (pressed & PAD_TRIGGER_L)
		{
			if(back == backsquares)
			{
				back = backdiag;
				if(angle)
					angle = 0;
			}
			else
				back = backsquares;
		}

		if (pressed & PAD_BUTTON_UP)
		{
			speed ++;
			if(speed == 1)
				subpixel = subpixcnt = 0;
			if(subpixel)
				subpixel -= 1;
		}

		if (pressed & PAD_BUTTON_DOWN)
		{
			speed --;
			if(speed == 0)
			{
				subpixel = 1;
				subpixcnt = 0;
			}
			if(subpixel && speed > -5)
				subpixel += 1;
		}

		if (pressed & PAD_BUTTON_B)
			done = 1;

		if (pressed & PAD_BUTTON_A)
			pause = !pause;

		if (pressed & PAD_BUTTON_X)
			acc *= -1;

		if (pressed & PAD_BUTTON_Y)
			direction = !direction;
		
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = GRIDSCROLL;
		}							
	}
	FreeImage(&backsquares); 
	FreeImage(&backdiag);	
	return;
}

void DrawStripes()
{
	int 				done = 0, field = 1, alternate = 0,
						frame = 0, dframe = 0, is_vertical = 0;
	u16					pressed;		
	ImagePtr			stripespos, stripesneg;
	ImagePtr			vstripespos, vstripesneg;	
	fmenudata 			resmenudata[] = { {0, "Horizontal"}, {1, "Vertical"} };
	
	stripespos = LoadImage(STRIPESPOSIMG, 1);
	if(!stripespos)
		return;
	stripesneg = LoadImage(STRIPESNEGIMG, 1);
	if(!stripesneg)
		return;

	vstripespos = LoadImage(VERTSTRIPESPOSIMG, 1);
	if(!vstripespos)
		return;
	vstripesneg = LoadImage(VERTSTRIPESNEGIMG, 1);
	if(!vstripesneg)
		return;

	is_vertical = SelectMenu("Select Type", resmenudata, 2, is_vertical+1);
	if(is_vertical == MENU_CANCEL)
	{
		is_vertical = 0;
		done = 1;
	}
	
	ChangeVideoEnabled = 0;
	while(!done && !EndProgram) 
	{	
		StartScene();

		if(!is_vertical)
		{
			if(field == 1)
				DrawImage(stripespos);
			else
				DrawImage(stripesneg);
		}
		else
		{
			if(field == 1)
				DrawImage(vstripespos);
			else
				DrawImage(vstripesneg);
		}

		if(alternate)
			field = !field;

		if(dframe)
		{
			char msg[20];

			sprintf(msg, "Frame: %02d", frame);
			if(vmode != VIDEO_480P)
				DrawStringB(20, 210, 0xff, 0xff, 0xff, msg);
			else
				DrawStringB(20, 460, 0xff, 0xff, 0xff, msg);
			frame ++;
			if(IsPAL)
			{
				if(frame > 49)
					frame = 0;
			}
			else
			{
				if(frame > 59)
					frame = 0;
			}
		}

		EndScene();
		
		ControllerScan();
		pressed = Controller_ButtonsDown(0);
		if (pressed & PAD_BUTTON_B)
			done =	1;				
					
		if (pressed & PAD_BUTTON_A)
		{
			alternate = !alternate;
		}
					
		if (pressed & PAD_TRIGGER_R)
		{
			int sel;
			
			sel = SelectMenu("Select Type", resmenudata, 2, is_vertical+1);
			if(sel != MENU_CANCEL)
				is_vertical = sel;
		}
					
		if (pressed & PAD_BUTTON_Y)
		{
			dframe = !dframe;
			frame = 0;
		}
					
		if (pressed & PAD_BUTTON_X && !alternate)
			field = !field;

		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = STRIPESHELP;
		}												
	}
	
	ChangeVideoEnabled = 1;

	FreeImage(&vstripespos);
	FreeImage(&vstripesneg);
	FreeImage(&stripespos);
	FreeImage(&stripesneg);
	return;
}

void DrawCheckBoard()
{
	int 				done = 0, field = 1, alternate = 0,
						frame = 0, dframe = 0;
	u16					pressed;		
	ImagePtr			checkpos, checkneg;
	
	checkpos = LoadImage(CHECKPOSIMG, 1);
	if(!checkpos)
		return;
	checkneg = LoadImage(CHECKNEGIMG, 1);
	if(!checkneg)
		return;	
	
	while(!done && !EndProgram) 
	{
		StartScene();

		if(field == 1)
			DrawImage(checkpos);
		else
			DrawImage(checkneg);

		if(alternate)
			field = !field;

		if(dframe)
		{
			char msg[20];

			sprintf(msg, "Frame: %02d", frame);
			if(vmode != VIDEO_480P)
				DrawStringB(20, 210, 0xff, 0xff, 0xff, msg);
			else
				DrawStringB(20, 460, 0xff, 0xff, 0xff, msg);
			frame ++;
			if(IsPAL)
			{
				if(frame > 49)
					frame = 0;
			}
			else
			{
				if(frame > 59)
					frame = 0;
			}
		}

		EndScene();

		ControllerScan();
		pressed = Controller_ButtonsDown(0);
		
		if (pressed & PAD_BUTTON_B)
			done =	1;				
					
		if (pressed & PAD_BUTTON_A)
		{
			alternate = !alternate;				
		}
	
		if (pressed & PAD_BUTTON_Y)
		{
			dframe = !dframe;
			frame = 0;
		}
					
		if (pressed & PAD_BUTTON_X && !alternate)
			field = !field;

		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = CHECKHELP;
		}																
	}
	FreeImage(&checkpos);
	FreeImage(&checkneg);
	return;
}

void AudioSyncTest()
{
	int 		done = 0, paused = 0, oldvmode = 0, playtone = 2;
	int			y = 0, speed = -1;
	float		hstep = 0;
	u32			pressed;
	s32 		voice = 0;
	ImagePtr	squareL, squareR, lineB, sprite, back;	
	
	ASND_Init(); 
	ASND_Pause(0);
	voice = ASND_GetFirstUnusedVoice();
	if(voice == SND_INVALID)
		return;
		
	back = LoadImage(WHITEIMG, 1);
	if(!back)
		return;
	squareL = LoadImage(WHITEIMG, 1);
	if(!squareL)
		return;
	squareR = LoadImage(WHITEIMG, 1);
	if(!squareR)
		return;
	lineB = LoadImage(WHITEIMG, 1);
	if(!lineB)
		return;
	sprite = LoadImage(WHITEIMG, 1);
	if(!sprite)
		return;
	
	SetTextureColor(back, 0x00, 0x00, 0x00);
	
	back->w = dW;
	back->h = dH;
	
	sprite->w = sprite->h = 8;
	sprite->x = dW / 2 - 4;
	y = 180;
	hstep = -1 * dW/120; // 60 steps times half screen
	
	lineB->w = dW;
	lineB->h = 8;
	lineB->x = 0;
	lineB->y = 180 + sprite->h;
	
	squareL->w = dW / 2;
	squareL->h = 16;
	squareL->x = 0;
	squareL->y = 80;
	
	squareR->w = dW / 2;
	squareR->h = 16;
	squareR->x = dW / 2;
	squareR->y = 80;
	
	while(!done && !EndProgram) 
	{
		if(oldvmode != vmode)
		{
			back->w = dW;
			back->h = dH;
			oldvmode = vmode;
		}
		
		if(!paused)
		{
			y += speed;
			sprite->y = y;
			squareL->x += hstep;
			squareR->x -= hstep;
			if(y == 180 || y == 120)
			{
				speed *= -1;
				hstep *= -1;
			}
			if(y == 180)
				SetTextureColor(back, 0xff, 0xff, 0xff);
			else
				SetTextureColor(back, 0x00, 0x00, 0x00);
			
			// play the tone 1 frame before it hits so they are in sync
			// it is off by 1.7ms to 2.5ms due to audio buffer
			// but centered around the fully white frame
			if(y == 179 && speed == 1)
				playtone = 1;
		}
		
		if(playtone == 2)
		{
			if(ASND_StatusVoice(voice) == SND_WORKING)
				ASND_StopVoice(voice);
			ASND_Pause(1);
			ASND_SetVoice(voice, VOICE_MONO_16BIT, 48000, 0, (void*)beep_snd, beep_snd_size, MAX_VOLUME, MAX_VOLUME, NULL);
			playtone = 0;
		}
		
		if(playtone == 1)
		{
			ASND_Pause(0);
			playtone = 2;
		}
			
		StartScene();
		DrawImage(back);
		DrawImage(squareL);
		DrawImage(squareR);
		DrawImage(lineB);
		DrawImage(sprite);
		EndScene();
			
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
				
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;			
			//HelpData = AUDIOSYNCHELP;			
		}
		
		if (pressed & PAD_BUTTON_B)
			done =	1;
			
		if (pressed & PAD_BUTTON_A)
			paused = !paused;

	}
	
	FreeImage(&back);
	FreeImage(&squareL);
	FreeImage(&squareR);
	FreeImage(&lineB);
	FreeImage(&sprite);
	
	if(ASND_StatusVoice(voice) == SND_WORKING)
		ASND_StopVoice(voice);
	ASND_End();
}

void DrawMessage(ImagePtr back, char *title, char *msg)
{
	char *data1 = "Using 48khz Audio Equipment Test";
	char *data2 = "Visit #Chttp://junkerhq.net/MDFourier#C for info";
	
	StartScene();
	DrawImage(back);

	DrawStringS((dW-fw*strlen(title))/2, 60, 0x00, 0xff, 0x00, title); 
	DrawStringS((dW-fw*strlen(data1))/2, 80, 0xee, 0xee, 0xee, data1);
	DrawStringS((dW-fw*strlen(msg))/2, 120, 0xff, 0xff, 0xff, msg);
	DrawStringS((dW-fw*strlen(data2))/2, 180, 0x00, 0xff, 0xff, data2);
	EndScene();
}

void aesnd_callback(AESNDPB *pb, unsigned int state)
{
	unsigned int *playback = AESND_GetVoiceUserData(pb);
	
	if (state == VOICE_STATE_RUNNING) 
		*playback = 1;
	if(state == VOICE_STATE_STOPPED)
		*playback = 0;
}

void AudioEquipmentTest(ImagePtr back)
{
	int 			done = 0, loaded = 0, counter = 2;
	u32				pressed;
	char			*msg = "Loading audio file...";
	char			*title = "MDFourier";
	u8				*aet_samples = NULL;
	ulong			aet_size = 0;
	AESNDPB			*voice = NULL;
	unsigned int	playback = 0;

	AESND_Init();
	
	voice = AESND_AllocateVoice(aesnd_callback);
	if(!voice)
		return;
	AESND_SetVoiceUserData(voice, &playback);

	while(counter --)
		DrawMessage(back, title, msg);
	
	while(!done && !EndProgram) 
	{
		DrawMessage(back, title, msg);
		
		if(!loaded)
		{
#ifdef WII_VERSION
			aet_samples = LoadFileToBuffer(EQUIPMENT_FILE, &aet_size);
			if(!aet_samples)
				return;
#else
			// Load into our uncompressed Texture Buffer, since malloc would fail due to audio file size
			DrawMessage(back, title, "Preparing memory");
			CloseTextures();
			memset(full_textures_tpl, 0, full_textures_tpl_size);
			DrawMessage(back, title, "Loading file...");
			if(!LoadFileToMemoryAddress(EQUIPMENT_FILE, &aet_size, full_textures_tpl, full_textures_tpl_size))
			{
				LoadTextures();
				return;
			}
			aet_samples = full_textures_tpl;
#endif
			loaded = 1;
			msg = "Press A to start";
		}
		
		if(counter)
		{
			counter --;
			if(counter == 0)
				msg = "Press A to start";
		}
		
		ControllerScan();
		pressed = Controller_ButtonsDown(0);
		if (pressed & PAD_BUTTON_B)
			done = 1;

#ifdef WII_VERSION
		// GC version has all textures removed
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = MDFOURIER_HELP;
		}
#endif

		if (pressed & PAD_BUTTON_A)
		{
			if(loaded)
			{	
				int cancel = 0;
				
				msg = "Playing Test Signal";

				AESND_PlayVoice(voice, VOICE_STEREO16, aet_samples, aet_size, lrintf(DSP_DEFAULT_FREQ), 0, false);
				
				while(playback != 1)
					DrawMessage(back, title, msg);
				
				while(playback && cancel != 2)
				{
					if(counter)
					{
						counter --;
						if(counter == 0)
						{
							msg = "Playing Test Signal";
							cancel = 0;
						}
					}
					DrawMessage(back, title, msg);
					ControllerScan();
					pressed = Controller_ButtonsDown(0);
					if(pressed & PAD_BUTTON_B && cancel == 0)
					{
						msg = "Press A to abort";
						cancel = 1;
						counter = 120;
					}
					if(pressed & PAD_BUTTON_A && cancel == 1)
					{
						msg = "Playback cancelled";
						cancel = 2;
						AESND_SetVoiceStop(voice, true);
						counter = 120;
					}
				}
				
				if(cancel != 2)
					msg = "Playback Finished, press A to replay";
				
				DrawMessage(back, title, msg);
			}
		}
	}

	AESND_SetVoiceStop(voice, true);
	AESND_FreeVoice(voice);
	AESND_Reset();

#ifdef WII_VERSION
	if(aet_samples)
	{
		free(aet_samples);
		aet_samples = NULL;
	}
#else
	if(loaded)
		LoadTextures();	
#endif
	return;
}


void SoundTest()
{
	int 			done = 0, sel = 1, play = 0, aleft = 0, aright = 0;
	u32				pressed;		
	ImagePtr		back;	
	
	back = LoadImage(BACKIMG, 0);
	if(!back)
		return;
		
	ASND_Init(); 
	ASND_Pause(0);
	while(!done && !EndProgram) 
	{
		StartScene();
		DrawImage(back);

		DrawStringS(130, 60, 0x00, 0xff, 0x00, "Sound Test"); 
		DrawStringS(80, 120, 0xff, sel == 0 ? 0 : 0xff, sel == 0 ? 0 : 0xff, "Left Channel"); 
		DrawStringS(120, 130, 0xff, sel == 1 ? 0 : 0xff, sel == 1 ? 0 : 0xff, "Center Channel");
		DrawStringS(160, 120, 0xff, sel == 2 ? 0 : 0xff, sel == 2 ? 0 : 0xff, "Right Channel");
		EndScene();

		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);		
		if (pressed & PAD_BUTTON_B)
			done =	1;								

		if (pressed & PAD_BUTTON_A)
			play =	1; 								

		if (pressed & PAD_BUTTON_LEFT)
			sel --;

		if (pressed & PAD_BUTTON_RIGHT)
			sel ++;

		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = SOUNDHELP;
		}																	
		
		if(sel < 0)
			sel = 2;

		if(sel > 2)
			sel = 0;
		
		switch(sel)
		{
			case 0:
				aleft = MAX_VOLUME;
				aright = MIN_VOLUME;
				break;
			case 1:
				aleft = MAX_VOLUME;
				aright = MAX_VOLUME;
				break;
			case 2:
				aleft = MIN_VOLUME;
				aright = MAX_VOLUME;
				break;
		}

		if(play)
		{
			ASND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_16BIT, 48000, 0, (void*)beep_snd, beep_snd_size, aleft, aright, NULL);
			play = 0;
		}

	}
	FreeImage(&back);
	ASND_End();
	return;
}

void LEDZoneTest()
{	
	int			done = 0, x = dW/2, y = dH/2, selsprite = 1, show = 1;
	u32			pressed, held;
	ImagePtr	back, sprite[5];
	
	back = LoadImage(WHITEIMG, 1);
	if(!back)
		return;

	SetTextureColor(back, 0, 0, 0);
			
	sprite[0] = LoadImage(SPRITE0LEDIMG, 0);
	if(!sprite[0])
		return;
	sprite[1] = LoadImage(SPRITE1LEDIMG, 0);
	if(!sprite[1])
		return;
	sprite[2] = LoadImage(SPRITE2LEDIMG, 0);
	if(!sprite[2])
		return;
	sprite[3] = LoadImage(SPRITE3LEDIMG, 0);
	if(!sprite[3])
		return;
	sprite[4] = LoadImage(SPRITE4LEDIMG, 0);
	if(!sprite[4])
		return;
		
	while(!done && !EndProgram) 
	{
		StartScene();	
		DrawImage(back);		

		if(show)
		{
			sprite[selsprite]->x = x;
			sprite[selsprite]->y = y;
			DrawImage(sprite[selsprite]);
		}

		EndScene();

		ControllerScan();
		
		held = Controller_ButtonsHeld(0);		

		if (held & PAD_BUTTON_UP)
			y --;
	
		if (held & PAD_BUTTON_DOWN)
			y ++;

		if (held & PAD_BUTTON_LEFT)
			x --;

		if (held & PAD_BUTTON_RIGHT)
			x ++;			
	
		pressed = Controller_ButtonsDown(0);

		if (pressed & PAD_BUTTON_B)
			done =	1;
					
		if (pressed & PAD_TRIGGER_L)
		{
			if(selsprite > 0)
				selsprite --;
			else
				selsprite = 4;
		}
	
		if (pressed & PAD_TRIGGER_R)
		{
			if(selsprite < 4)
				selsprite ++;
			else
				selsprite = 0;
		}

		if (pressed & PAD_BUTTON_A)
			show = !show;
		
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = BACKLITHELP;
		}																			
		
		if(x < 0)
			x = 0;
		if(y < 0)
			y = 0;
		if(x > back->w - 1)
			x = back->w - 1;
		if(y > back->h - 1)
			y = back->h - 1;

	}
	FreeImage(&back);
	FreeImage(&sprite[0]);
	FreeImage(&sprite[1]);
	FreeImage(&sprite[2]);
	FreeImage(&sprite[3]);
	FreeImage(&sprite[4]);
	return;
}

void changeCircleColor(ImagePtr circle, u16 color, u16 toggle)
{
	if(toggle)
	{
		if(color)
		{
			circle->r = 0xff;
			circle->g = 0;
			circle->b = 0;
		}
		else
		{
			circle->r = 0x2b;
			circle->g = 0x2b;
			circle->b = 0x2b;
		}
	}
	else
	{
		if(color)
		{
			circle->r = 0;
			circle->g = 0;
			circle->b = 0xff;
		}
		else
		{
			circle->r = 0x80;
			circle->g = 0x80;
			circle->b = 0x80;
		}
	}
}

void PassiveLagTest()
{
	u16 		frames = 0, seconds = 0, minutes = 0;
	u16			color = 1, hours = 0, done =  0, framecnt = 1;
	u16			pressed, lsd, msd, pause = 0, toggle = 0;
	ImagePtr	back, circle, barl, barr;	
	
	back = LoadImage(WHITEIMG, 0);
	if(!back)
		return;
	back->w = 320;
	back->h = 240;
	barl = LoadImage(WHITEIMG, 0);
	if(!barl)
		return;
	barr = LoadImage(WHITEIMG, 0);
	if(!barr)
		return;
	barr->x = 0;
	barl->w = 6;
	barl->h = 240;
	
	barr->x = 314;
	barr->w = 6;
	barr->h = 240;

	circle= LoadImage(CIRCLEIMG, 0);
	if(!circle)
		return;
	SetTextureColor(circle, 0x00, 0x00, 0xff);
	
	LoadNumbers();

	while(!done && !EndProgram) 
	{		
		if(!pause)
		{
			frames ++;
			framecnt ++;
			if(framecnt > 8)
				framecnt = 1;
			toggle = !toggle;
		}
		
		if(!toggle)
		{
			barl->r = 0;
			barl->g = 0;
			barl->b = 0;
			
			barr->r = 0;
			barr->g = 0;
			barr->b = 0;
		}
		else
		{
			barl->r = 0xff;
			barl->g = 0xff;
			barl->b = 0xff;
			
			barr->r = 0xff;
			barr->g = 0xff;
			barr->b = 0xff;
		}

		if(IsPAL)
		{
			if(frames > 49)
			{
				frames = 0;
				seconds ++;
			}

		}
		else
		{
			if(frames > 59)
			{
				frames = 0;
				seconds ++;
			}
		}

		if(seconds > 59)
		{
			seconds = 0;
			minutes ++;
		}

		if(minutes > 59)
		{
			minutes = 0;
			hours ++;
		}

		if(hours > 99)
			hours = 0;

		StartScene();

		DrawImage(back);
		DrawImage(barl);
		DrawImage(barr);
		DrawString(32, 8, 0, 0,	0, "hours");
		DrawString(104, 8, 0, 0, 0, "minutes");
		DrawString(176, 8, 0, 0, 0, "seconds");
		DrawString(248, 8, 0, 0, 0, "frames");

		// Counter Separators
		DrawDigit(80, 16, 0, 0, 0, 10);
		DrawDigit(152, 16, 0, 0, 0, 10);
		DrawDigit(224, 16, 0, 0, 0, 10);

		// Circles 1st row
		circle->x = 8;
		circle->y = 56;
		changeCircleColor(circle, color, framecnt == 1);
		DrawImage(circle);
		DrawDigit(28, 68, 0xff, 0xff, 0xff, 1);

		circle->x = 88;
		circle->y = 56;
		changeCircleColor(circle, color, framecnt == 2);
		DrawImage(circle);
		DrawDigit(108, 68, 0xff, 0xff, 0xff, 2);

		circle->x = 168;
		circle->y = 56;
		changeCircleColor(circle, color, framecnt == 3);
		DrawImage(circle);
		DrawDigit(188, 68, 0xff, 0xff, 0xff, 3);

		circle->x = 248;
		circle->y = 56;
		changeCircleColor(circle, color, framecnt == 4);
		DrawImage(circle);
		DrawDigit(268, 68, 0xff, 0xff, 0xff, 4);

		// Circles 2nd row
		circle->x = 8;
		circle->y = 136;
		changeCircleColor(circle, color, framecnt == 5);
		DrawImage(circle);
		DrawDigit(28, 148, 0xff, 0xff, 0xff, 5);

		circle->x = 88;
		circle->y = 136;
		changeCircleColor(circle, color, framecnt == 6);
		DrawImage(circle);
		DrawDigit(108, 148, 0xff, 0xff, 0xff, 6);

		circle->x = 168;
		circle->y = 136;
		changeCircleColor(circle, color, framecnt == 7);
		DrawImage(circle);
		DrawDigit(188, 148, 0xff, 0xff, 0xff, 7);

		circle->x = 248;
		circle->y = 136;
		changeCircleColor(circle, color, framecnt == 8);
		DrawImage(circle);
		DrawDigit(268, 148, 0xff, 0xff, 0xff, 8);

		// Draw Hours
		lsd = hours % 10;
		msd = hours / 10;
		DrawDigit(32, 16, 0, 0, 0, msd);
		DrawDigit(56, 16, 0, 0, 0, lsd);

		// Draw Minutes
		lsd = minutes % 10;
		msd = minutes / 10;
		DrawDigit(104, 16, 0, 0, 0, msd);
		DrawDigit(128, 16, 0, 0, 0, lsd);

		// Draw Seconds
		lsd = seconds % 10;
		msd = seconds / 10;
		DrawDigit(176, 16, 0, 0, 0, msd);
		DrawDigit(200, 16, 0, 0, 0, lsd);

		// Draw Frames
		lsd = frames % 10;
		msd = frames / 10;
		if(color)
		{
			DrawDigit(248, 16, toggle?0xff:0, 0, toggle?0:0xff, msd);
			DrawDigit(272, 16, toggle?0xff:0, 0, toggle?0:0xff, lsd);
		}
		else
		{
			DrawDigit(248, 16, 0, 0, 0, msd);
			DrawDigit(272, 16, 0, 0, 0, lsd);
		}

		EndScene();

		ControllerScan();
		pressed = Controller_ButtonsDown(0);
					
		if (pressed & PAD_BUTTON_B)
			done =	1;				
					
		if (pressed & PAD_BUTTON_X && pause)
		{
			frames = hours = minutes = seconds = 0;
			framecnt = 1;
		}
		
		if (pressed & PAD_BUTTON_Y)
			color = !color;

		if (pressed & PAD_BUTTON_A)
			pause = !pause;
		
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = PASSIVELAG;
		}										
	}
	FreeImage(&back);
	FreeImage(&barl);
	FreeImage(&barr);
	FreeImage(&circle);
	ReleaseNumbers();
}

u16 ConvertToFrames(timecode *time)
{
	u16	frames = 0;

	if(!time)
		return frames;

	frames = time->frames;
	if(IsPAL)
		frames += time->seconds*50;
	else
		frames += time->seconds*60;
	frames += time->minutes*3600;
	frames += time->hours*216000;
	return frames;
}

void ConvertFromFrames(timecode *value, u16 Frames)
{
	if(!value)
		return;
	value->hours = Frames / 216000;
	Frames = Frames % 216000;
	value->minutes = Frames / 3600;
	Frames = Frames % 3600;
	if(IsPAL)
	{
		value->seconds = Frames / 50;
		value->frames = Frames % 50;
	}
	else
	{
		value->seconds = Frames / 60;
		value->frames = Frames % 60;
	}
}

void Alternate240p480i()
{
	int 		frames = 0, seconds = 0, minutes = 0, hours = 0, done =  0, current = 0, res = 0, status = 0;
	timecode	times[20];
	u32		pressed, oldvmode = vmode;		
	char 		buffer[100];
	ImagePtr	back;
	
	if(IsPAL)
	{
		if(vmode != VIDEO_288P)
		{			
			SetVideoMode(VIDEO_288P);				
			SetupGX();
		}

	}
	else
	{
		if(vmode != VIDEO_240P)
		{		
			SetVideoMode(VIDEO_240P);				
			SetupGX();
		}
	}
	
	back = LoadImage(WHITEIMG, 1);
	if(!back)
		return;
		
	back->r = 0x00;
	back->g = 0x00;
	back->b = 0x00;
	CalculateUV(0, 0, dW, dH, back);

	ChangeVideoEnabled = 0;
	
	while(!done && !EndProgram) 
	{
		frames ++;

		if(IsPAL)
		{
			if(frames > 49)
			{
				frames = 0;
				seconds ++;
			}
		}
		else
		{
			if(frames > 59)
			{
				frames = 0;
				seconds ++;
			}
		}

		if(seconds > 59)
		{
			seconds = 0;
			minutes ++;
		}

		if(minutes > 59)
		{
			minutes = 0;
			hours ++;
		}

		if(hours > 99)
			hours = 0;

		StartScene();

		DrawImage(back);
		DrawString(32, 8, 0, 0xff, 0, "Current Resolution:");
		DrawString(140, 8, 0, 0xff, 0, res == 0 ? (IsPAL ? "288p" : "240p") : (IsPAL ? "576i" : "480i"));

		sprintf(buffer, "%02d:%02d:%02d:%02d", hours, minutes, seconds, frames);
		DrawString(32, 32, 0xff, 0xff, 0xff, "Elapsed Timer:");
		DrawString(140, 32, 0xff, 0xff, 0xff, buffer);

		if(current)
		{
			int i = 0;
			for(i = 0; i < current; i++)
			{
				if(times[i].type == 0)
				{
					DrawString(32,		40+i*8, 0xff, 0xff, 0.0, "Switched to");
					DrawString(32+12*5, 40+i*8, 0xff, 0xff, 0.0, 
							times[i].res == 0 ? (IsPAL ? "288p" : "240p") : (IsPAL ? "576i" : "480i"));
					DrawString(32+16*5, 40+i*8, 0xff, 0xff, 0.0, " at:");
				}
				else
					DrawString(32, 40+i*8, 0.0, 0xff, 0.0, "Viewed at:");
				sprintf(buffer, "%02d:%02d:%02d:%02d", times[i].hours, times[i].minutes, times[i].seconds, times[i].frames);
				DrawString(140, 40+i*8, 0xff, 0xff, 0xff, buffer);
				if(times[i].type != 0 && i >= 1 && i <= 19)
				{
					u16 		framesA = 0, framesB = 0, res = 0;
					timecode 	len;

					framesB = ConvertToFrames(&times[i]);
					framesA = ConvertToFrames(&times[i - 1]);
					res = framesB - framesA;
					ConvertFromFrames(&len, res);
					sprintf(buffer, "%02d:%02d:%02d:%02d", len.hours, len.minutes, len.seconds, len.frames);
					DrawString(200, 40+i*8, 0xff, 0.0, 0.0, buffer);
				}
			}
		}

		EndScene();


		ControllerScan();
		pressed = Controller_ButtonsDown(0);
					
		if ( pressed & PAD_BUTTON_START ) 		
		{			
			DrawMenu = 1;		
			HelpData = ALTERNATE;
		}													

		if (pressed & PAD_BUTTON_B)
			done =	1;				
					
		if (pressed & PAD_BUTTON_A)
		{
			if(current <= 19)
				current ++;
			else
				current = 1;

			times[current - 1].frames = frames;
			times[current - 1].minutes = minutes;
			times[current - 1].seconds = seconds;
			times[current - 1].hours = hours;

			status ++;
			if(status == 1)
			{
				times[current - 1].type = 0;
				res = !res;
				times[current - 1].res = res;	
				if(!res)
					SetVideoMode(IsPAL ? VIDEO_288P : VIDEO_240P);
				else
					SetVideoMode(IsPAL ? VIDEO_576I_A264 : VIDEO_480I_A240);
				CalculateUV(0, 0, dW, dH, back);
				SetupGX();
			}
			if(status == 2)
			{
				times[current - 1].type = 1;
				times[current - 1].res = res;
				status = 0;
			}
		}
	}	
	
	ChangeVideoEnabled = 1;
	
	SetVideoMode(oldvmode);				
	SetupGX();
	FreeImage(&back);
}

void FixSpriteSize(ImagePtr sprite, int full)
{
	if(!sprite)
		return;
		
	if(!full)
	{
		if(vmode != VIDEO_480P && vmode != VIDEO_480I &&
				vmode != VIDEO_576I)
		{
			sprite->x = 60;
			sprite->y = 100;
			
			sprite->w = 200;
			sprite->h = 40;
		}
		else
		{
			sprite->x = 120;
			sprite->y = 200;
			
			sprite->w = 400;
			sprite->h = 80;
		}
	}
	else
	{
		if(vmode != VIDEO_480P && vmode != VIDEO_480I &&
				vmode != VIDEO_576I)
		{
			sprite->x = -160;
			sprite->y = -120;
			
			sprite->w = 640;
			sprite->h = 480;
		}
		else
		{
			sprite->x = -320;
			sprite->y = -240;
			
			sprite->w = 1280;
			sprite->h = 960;
		}

	}
}

void DiagonalPatternTest()
{	
	int			done = 0, autorotate = 0, full = 0;
	float		angle = 45, speed = 1.0f;
	u32			pressed, oldvmode = vmode;
	ImagePtr	back, sprite;
	char		str[40];
	Mtx 		m;
	
	back = LoadImage(WHITEIMG, 1);
	if(!back)
		return;

	back->r = 0x00;
	back->g = 0x00;
	back->b = 0x00;
			
	sprite = LoadImage(LONGRECTANGLEIMG, 0);
	if(!sprite)
		return;
		
	FixSpriteSize(sprite, full);
	
	while(!done && !EndProgram) 
	{
		if(oldvmode != vmode)
		{
			FreeImage(&sprite);
			sprite = LoadImage(LONGRECTANGLEIMG, 0);
			if(!sprite)
				return;
			oldvmode = vmode;
			FixSpriteSize(sprite, full);
		}
			
		StartSceneMtx(&m);
		
		DrawImage(back);		

		sprintf(str, "Angle: %0.2f", angle);
		DrawStringS(10, 10, 0x00, 0xff, 0x00, str); 
		sprintf(str, "Angle increment: %s%d", speed != 1 ? "1/" : "", (int)speed);
		DrawStringS(10, 20, 0x00, 0xff, 0x00, str); 
	
		DrawImageRotate(sprite, -1*angle, &m);
		
		EndScene();

		ControllerScan();
			
		pressed = Controller_ButtonsDown(0);
		
		if (pressed & PAD_BUTTON_UP)
		{
			full = !full;			
			FixSpriteSize(sprite, full);			
		}

		if (pressed & PAD_BUTTON_X)
			speed+=1;
			
		if (pressed & PAD_BUTTON_Y)
			speed-=1;
			
		if(speed > 20)
			speed = 20;
			
		if(speed < 1)
			speed = 1;
			
		if (pressed & PAD_BUTTON_B)
			done =	1;
			
		if (pressed & PAD_BUTTON_A)
			autorotate = !autorotate;
					
		if(!autorotate)
		{
			if (pressed & PAD_TRIGGER_L)
			{
				if(angle > 0)
					angle -= 1/speed;
				else
					angle = 359;
			}
		
			if (pressed & PAD_TRIGGER_R)
			{
				if(angle < 359)
					angle += 1/speed;
				else
					angle = 0;
			}
		}
		
		if(autorotate)
		{
			angle += 1/speed;
			if(angle > 360)
				angle = 0;
		}

		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = DIAGONALHELP;
		}																			
		

	}
	FreeImage(&back);
	FreeImage(&sprite);
	return;
}

void SD_blink_cycle_phase(ImagePtr sd_b1, ImagePtr sd_b2, int index)
{
	static int blink_counter[5] = { 0, 0, 0, 0, 0 };
	static int is_blinking[5] = { 0, 0, 0, 0, 0 };
	
	blink_counter[index]++;	
	if(sd_b1 && sd_b2 && blink_counter[index] > 230)
	{
		if(!is_blinking[index])
		{
			if(rand() % 100 < 25)
			{
				is_blinking[index] = 1;
				blink_counter[index] = 230;
				DrawImage(sd_b1);
			}
		}
		else
		{
			if(blink_counter[index] >= 232 && blink_counter[index] < 234)
				DrawImage(sd_b2);
				
			if(blink_counter[index] >= 234 && blink_counter[index] < 238)
				DrawImage(sd_b1);
	
			if(blink_counter[index] >= 238)
			{	
				blink_counter[index] = 0;
				is_blinking[index] = 0;
			}
		}
	}
}

void DrawPhase()
{
	int 		done = 0, oldvmode = -1, type = 0, hpos = 0, i = 0;
	u32			pressed;
	ImagePtr	back = NULL, backcheck = NULL;
	ImagePtr	sd[5] = { NULL, NULL, NULL, NULL, NULL};
	ImagePtr	sd_b1[5] = { NULL, NULL, NULL, NULL, NULL};
	ImagePtr	sd_b2[5] = { NULL, NULL, NULL, NULL, NULL};
	
	back = LoadImage(PHASEIMG, 0);
	if(!back)
		return;
	
	backcheck = LoadImage(CHECKPOSIMG, 1);
	if(!backcheck)
		return;

	for(i = 0; i < 5; i++)
	{
		sd[i] = LoadImage(SDIMG, 0);
		if(!sd[i])
			return;
		sd[i]->x = 5+i*64;
		sd[i]->y = 70;
		sd_b1[i] = LoadImage(SD_B1_IMG, 0);
		if(sd_b1[i])
		{
			sd_b1[i]->x = sd[i]->x+16;
			sd_b1[i]->y = sd[i]->y+32;
		}
		
		sd_b2[i] = LoadImage(SD_B2_IMG, 0);
		if(sd_b2[i])
		{
			sd_b2[i]->x = sd[i]->x+16;
			sd_b2[i]->y = sd[i]->y+32;
		}
	}

	while(!done && !EndProgram) 
	{
		if(oldvmode != vmode)
		{
			CalculateUV(0, 0, dW, dH, backcheck);
			oldvmode = vmode;
		}
		
		StartScene();
		if(!type)
			DrawImage(back);
		else
			DrawImage(backcheck);
		for(i = 0; i < 5; i++)
		{
			sd[i]->x = 5+i*64+hpos;
			DrawImage(sd[i]);
			if(sd_b1[i] && sd_b2[i])
			{
				sd_b1[i]->x = sd[i]->x+16;
				sd_b2[i]->x = sd[i]->x+16;
				SD_blink_cycle_phase(sd_b1[i], sd_b2[i], i);
			}
		}
		EndScene();
		
		ControllerScan();
		pressed = Controller_ButtonsDown(0);
		if (pressed & PAD_BUTTON_START )
		{
			DrawMenu = 1;					
			HelpData = PHASEHELP;
		}

		if (pressed & PAD_BUTTON_X)
			type = !type;
		
		if (pressed & PAD_BUTTON_A)
			hpos = 0;
		
		if (pressed & PAD_BUTTON_LEFT)
		{
			hpos --;
			if(hpos < -5)
				hpos = -5;
		}
		
		if (pressed & PAD_BUTTON_RIGHT)
		{
			hpos ++;
			if(hpos > 5)
				hpos = 5;
		}
		
		if (pressed & PAD_BUTTON_B)
			done =	1;
	}
	FreeImage(&back);
	FreeImage(&backcheck);
	for(i = 0; i < 5; i++)
	{
		FreeImage(&sd[i]);
		FreeImage(&sd_b1[i]);
		FreeImage(&sd_b2[i]);
	}
	return;
}

void DrawDisappear()
{
	int 		frames = 0, seconds = 0, minutes = 0, hours = 0;
	int			toggle = 0, done = 0;
	u32			pressed;
	u16			lsd, msd, show = 1;		
	ImagePtr	back = NULL, sd = NULL;
	float		x = -8, y = 10;

	back = LoadImage(WHITEIMG, 1);
	if(!back)
		return;
		
	SetTextureColor(back, 0, 0, 0);
	back->w = 320;
	back->h = 240;
	
	sd = LoadImage(SDIMG, 0);
	if(!sd)
		return;
	sd->x = 128;
	sd->y = 85;
			
	LoadNumbers();
	while(!done && !EndProgram) 
	{
		StartScene();
		DrawImage(back);
		DrawString(x+32,  y+8, 0xff, 0xff, 0xff, "hours");
		DrawString(x+104, y+8, 0xff, 0xff, 0xff, "minutes");
		DrawString(x+176, y+8, 0xff, 0xff, 0xff, "seconds");
		DrawString(x+248, y+8, 0xff, 0xff, 0xff, "frames");

		// Counter Separators
		DrawDigit(x+80,  y+16, 0xff, 0xff, 0xff, 10);
		DrawDigit(x+152, y+16, 0xff, 0xff, 0xff, 10);
		DrawDigit(x+224, y+16, 0xff, 0xff, 0xff, 10);

		// Draw Hours
		lsd = hours % 10;
		msd = hours / 10;
		DrawDigit(x+32, y+16, 0xff, 0xff, 0xff, msd);
		DrawDigit(x+56, y+16, 0xff, 0xff, 0xff, lsd);

		// Draw Minutes
		lsd = minutes % 10;
		msd = minutes / 10;
		DrawDigit(x+104, y+16, 0xff, 0xff, 0xff, msd);
		DrawDigit(x+128, y+16, 0xff, 0xff, 0xff, lsd);

		// Draw Seconds
		lsd = seconds % 10;
		msd = seconds / 10;
		DrawDigit(x+176, y+16, 0xff, 0xff, 0xff, msd);
		DrawDigit(x+200, y+16, 0xff, 0xff, 0xff, lsd);

		// Draw Frames
		lsd = frames % 10;
		msd = frames / 10;
		DrawDigit(x+248, y+16, 0xff, 0xff, 0xff, msd);
		DrawDigit(x+272, y+16, 0xff, 0xff, 0xff, lsd);
		
		if(show)
		{
			DrawImage(sd);
			SD_blink_cycle(sd);
		}

		EndScene();
		
		if(toggle)
		{
			toggle --;
			if(toggle == 0)
				SetTextureColor(back, 0, 0, 0);
		}

		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);		
		
		if (pressed & PAD_BUTTON_B)
			done =	1;
		
		if (pressed & PAD_BUTTON_A)
			show = !show;
			
		if (pressed & PAD_BUTTON_X)
		{
			SetTextureColor(back, 0xff, 0xff, 0xff);
			toggle = 2;
		}

		if (pressed & PAD_BUTTON_START)
		{
			DrawMenu = 1;
			HelpData = DISAPHELP;
		}

		frames ++;

		if(frames > (IsPAL ? 49 : 59))
		{
			frames = 0;
			seconds ++;
		}

		if(seconds > 59)
		{
			seconds = 0;
			minutes ++;
		}

		if(minutes > 59)
		{
			minutes = 0;
			hours ++;
		}

		if(hours > 99)
			hours = 0;
	}
	FreeImage(&back);
	FreeImage(&sd);
	ReleaseNumbers();
}

