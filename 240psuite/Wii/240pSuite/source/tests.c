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
#include "tests.h"
#include "image.h"
#include "font.h"
#include "video.h"

#include "help.h"
#include "menu.h"
#include "controller.h"
#include "options.h"
#include <asndlib.h>

extern u8 beep_snd_end[];
extern u8 beep_snd[];
extern u32 beep_snd_size;

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
	int		    done = 0, x = 0, y = 0, invert = 0, frame = 0, text = 0, selback = 0, sprite = 0;
	u32		    pressed, held;		
	ImagePtr	back[4], ssprite, shadow, buzz, buzzshadow, overlay;	

	if(vmode != VIDEO_480P && vmode != VIDEO_480I && vmode != VIDEO_576I)
	{		
		back[0] = LoadImage(MOTOKOIMG, 0);
		if(!back[0])
			return;
		back[1] = LoadImage(SONICBACKIMG, 0);
		if(!back[1])
			return;
		back[2] = LoadImage(CHECKPOSIMG, 1);
		if(!back[2])
			return;
		back[3] = LoadImage(STRIPESPOSIMG, 1);
		if(!back[3])
			return;
		overlay = LoadImage(SONICFLOORIMG, 0);
		if(!overlay)
			return;
	}
	else
	{
		back[0] = LoadImage(MOTOKO480IMG, 0);
		if(!back[0])
			return;
		back[1] = LoadImage(SONICBACKIMG, 0);
		if(!back[1])
			return;
		back[2] = LoadImage(CHECKPOSIMG, 1);
		if(!back[2])
			return;
		back[3] = LoadImage(STRIPESPOSIMG, 1);
		if(!back[3])
			return;
		overlay = LoadImage(SONICFLOORIMG, 0);
		if(!overlay)
			return;

		back[0]->scale = 0;
		back[1]->scale = 0;
		back[2]->scale = 0;
		back[3]->scale = 0;
		overlay->scale = 0;

		back[1]->y = (dH - 240)/2;
		overlay->y = (dH - 240)/2;
	}
		
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
				DrawStringB(140, 12, 0, 0xff, 0, msg);
			else
				DrawStringB(450, 20, 0, 0xff, 0, msg);
			text --;
		}

		if(frame == invert)
		{
			shadow->x = x;
			shadow->y = y;
			DrawImage(shadow);
			frame = !frame;
		}
		else
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
	}
	FreeImage(&back[0]);
	FreeImage(&back[1]);
	FreeImage(&back[2]);
	FreeImage(&back[3]);
	FreeImage(&overlay);
	FreeImage(&ssprite);
	FreeImage(&buzz);
	FreeImage(&buzzshadow);
	return;
}

void StripedSpriteTest()
{	
	int		    done = 0, x = 0, y = 0, selback = 0;
	u32		    pressed, held;	
	ImagePtr	back[4], striped, overlay;	

	if(vmode != VIDEO_480P && vmode != VIDEO_480I && vmode != VIDEO_576I)
	{		
		back[0] = LoadImage(MOTOKOIMG, 0);
		if(!back[0])
			return;
		back[1] = LoadImage(SONICBACKIMG, 0);
		if(!back[1])
			return;
		back[2] = LoadImage(CHECKPOSIMG, 1);
		if(!back[2])
			return;
		back[3] = LoadImage(STRIPESPOSIMG, 1);
		if(!back[3])
			return;
		overlay = LoadImage(SONICFLOORIMG, 0);
		if(!overlay)
			return;
	}
	else
	{
		back[0] = LoadImage(MOTOKO480IMG, 0);
		if(!back[0])
			return;
		back[0]->scale = 0;
		back[1] = LoadImage(SONICBACKIMG, 0);
		if(!back[1])
			return;
		back[1]->scale = 0;
		back[2] = LoadImage(CHECKPOSIMG, 1);
		if(!back[2])
			return;
		back[2]->scale = 0;
		back[3] = LoadImage(STRIPESPOSIMG, 1);
		if(!back[3])
			return;
		back[3]->scale = 0;
		overlay = LoadImage(SONICFLOORIMG, 0);
		if(!overlay)
			return;

		back[1]->y = (dH - 240)/2;
		overlay->y = (dH - 240)/2;
	}
	striped = LoadImage(STRIPEDIMG, 0);
	if(!striped)
		return;
			
	while(!done && !EndProgram) 
	{		
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
	}
	FreeImage(&back[0]);
	FreeImage(&back[1]);
	FreeImage(&back[2]);
	FreeImage(&back[3]);
	FreeImage(&overlay);
	FreeImage(&striped);
	return;
}

void LagTest()
{
	char			msg[60], strings[100];
	int				clicks[10], done = 0, view = 0, speed = 1, change = 1;
	int				x, y, x2, y2, pos = 0, i = 0, vary = 0, variation = 1;
	u16				pressed, audio = 0, rumble = 0;
	ImagePtr		back, spriteA, spriteB;
	char 			**ControllerButtons = NULL;
	
	
	srand((int)(time(0)));	
	back = LoadImage(LAGPERIMG, 0);
	if(!back)
		return;
	spriteA = CloneImage(back, 0);
	if(!spriteA)
		return;
	spriteB = CloneImage(back, 0);
	if(!spriteB)
		return;	
		
	SND_Init(INIT_RATE_48000); 
    SND_Pause(0); 	

	x = 144;
	y = 60;
	x2 = 108;
	y2 = 96;
		
	back->x = 144;
	back->y = 96;	

	for(i = 0; i < 10; i++)
		clicks[i] = 0xFF;

	while(!done && !EndProgram) 
	{
		StartScene();

		DrawImage(back);

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
				SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_STEREO_16BIT, 44100, 0, beep_snd, beep_snd_size, speed == -1 ? 0 : 255, speed == -1 ? 255 : 0, NULL);				
				
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
			HelpData = MANUALLAG;
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
				DrawStringS(60, 110, 0xff, 0x00, 0xff, "+");
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
				HelpData = MANUALLAG;
			}
		}
		FreeImage(&wall);
	}
	SND_End();
	
	return;
}

void ScrollTest()
{
	int 	done = 0, speed = 1, acc = 1, x = 0, pause = 0;
	u32		pressed;		
	ImagePtr	back, overlay;	
	
	back = LoadImage(SONICBACKIMG, 0);
	if(!back)
		return;
	overlay = LoadImage(SONICFLOORIMG, 0);
	if(!overlay)
		return;
	
	back->y = (dH - 240)/2;
	overlay->y = (dH - 240)/2;

	while(!done && !EndProgram) 
	{			
		StartScene();

		if(speed > 15)
			speed = 15;

		if(speed < 1)
			speed = 1;

		if(!pause)
			x += speed * acc;
		
		if(x > overlay->tw)
			x = 1;

		if(x < -1*overlay->tw)
			x = -1;

		CalculateUV(x, 0, dW, 240, back);
		CalculateUV(x*2, 0, dW, 240, overlay);
		DrawImage(back);
		DrawImage(overlay);
		
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

		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = SCROLL;
		}				
	}
	FreeImage(&back);
	FreeImage(&overlay);
	return;
}

void GridScrollTest()
{
	int 			done = 0, speed = 1, acc = 1, x = 0, y = 0, pause = 0, direction = 0;
	u32			    pressed;		
	ImagePtr		back;
		
	back = LoadImage(CIRCLESGRIDIMG, 0);
	if(!back)
		return;  

	while(!done && !EndProgram) 
	{	
		StartScene();

		if(speed > 5)
			speed = 5;

		if(speed < 1)
			speed = 1;

		if(!pause)
		{
			if(direction)
				x += speed * acc;
			else
				y += speed * acc;
		}	
				
		CalculateUV(x, y, dW, dH, back);    
		DrawImage(back);
	
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
			direction = !direction;
		
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = GRIDSCROLL;
		}							
	}
	FreeImage(&back);  
	return;
}

void DrawStripes()
{
	int 				done = 0, field = 1, alternate = 0,
						frame = 0, dframe = 0, vertical = 0;
	u16			        pressed;		
	ImagePtr		    stripespos, stripesneg;
	ImagePtr		    vstripespos, vstripesneg;	
	
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

	while(!done && !EndProgram) 
	{   
		StartScene();

		if(!vertical)
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
			vertical = !vertical;
					
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
	u16			        pressed;		
	ImagePtr		    checkpos, checkneg;
	
	checkpos = LoadImage(CHECKPOSIMG, 1);
	if(!checkpos)
		return;
	checkneg = LoadImage(CHECKNEGIMG, 1);
	if(!checkneg)
		return;	
		
#ifdef WII_VERSION		
	if(vmode == VIDEO_480P)
	{				
		HelpData = CHECK480PWII;
		HelpWindow(checkpos);					
	}
#endif
	
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

void SoundTest()
{
	int 			done = 0, sel = 1, play = 0, aleft = 0, aright = 0;
	u32			    pressed;		
	ImagePtr		back;	
	
	back = LoadImage(BACKIMG, 0);
	if(!back)
		return;
		
    SND_Init(INIT_RATE_48000); 
    SND_Pause(0);
	while(!done && !EndProgram) 
	{
		StartScene();
		DrawImage(back);

		DrawStringS(130, 60, 0xff, 0xff, 0xff, "Sound Test"); 
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
				aleft = 0xff;
				aright = 0x00;
				break;
			case 1:
				aleft = 0xff;
				aright = 0xff;
				break;
			case 2:
				aleft = 0x00;
				aright = 0xff;
				break;
		}

		if(play)
		{
			SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_STEREO_16BIT, 44100, 0, beep_snd, beep_snd_size, aleft, aright, NULL);				
			play = 0;
		}        

	}    
	FreeImage(&back);
	SND_End();
	return;
}

void LEDZoneTest()
{	
	int		    done = 0, x = 0, y = 0, selsprite = 1, show = 1;
	u32		    pressed, held;
	ImagePtr	back, sprite[5];
	
	back = LoadImage(WHITEIMG, 1);
	if(!back)
		return;

	back->r = 0x00;
	back->g = 0x00;
	back->b = 0x00;
			
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

void PassiveLagTest()
{
	int 		frames = 0, seconds = 0, minutes = 0, hours = 0, framecnt = 1, done =  0;
	u16		    pressed, lsd, msd, pause = 0;		
	ImagePtr	back, circle;	

	LoadNumbers();
	back = LoadImage(WHITEIMG, 1);
	if(!back)
		return;

	circle= LoadImage(CIRCLEIMG, 0);
	if(!circle)
		return;
	circle->r = 0x00;
	circle->g = 0x00;
	circle->b = 0xff;

	while(!done && !EndProgram) 
	{		
		if(!pause)
		{
			frames ++;
			framecnt ++;
			if(framecnt > 8)
				framecnt = 1;
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
		DrawString(32, 8, 0, 0,	0, "hours");
		DrawString(104, 8, 0, 0, 0, "minutes");
		DrawString(176, 8, 0, 0, 0, "seconds");
		DrawString(248, 8, 0, 0, 0, "frames");

		// Counter Separators
		DrawDigit(80, 16, 0, 0, 0, 10);
		DrawDigit(152, 16, 0, 0, 0, 10);
		DrawDigit(224, 16, 0, 0, 0, 10);

		// Circles 1st row
		circle->x = 16;
		circle->y = 56;
		if(framecnt == 1)
		{
			circle->b = 0;
			circle->r = 0xff;
		}
		else
		{
			circle->b = 0xff;
			circle->r = 0;
		}
		DrawImage(circle);
		DrawDigit(36, 68, 0xff, 0xff, 0xff, 1);

		circle->x = 96;
		circle->y = 56;
		if(framecnt == 2)
		{
			circle->b = 0;
			circle->r = 0xff;
		}
		else
		{
			circle->b = 0xff;
			circle->r = 0;
		}
		DrawImage(circle);
		DrawDigit(116, 68, 0xff, 0xff, 0xff, 2);

		circle->x = 176;
		circle->y = 56;
		if(framecnt == 3)
		{
			circle->b = 0;
			circle->r = 0xff;
		}
		else
		{
			circle->b = 0xff;
			circle->r = 0;
		}
		DrawImage(circle);
		DrawDigit(196, 68, 0xff, 0xff, 0xff, 3);

		circle->x = 256;
		circle->y = 56;
		if(framecnt == 4)
		{
			circle->b = 0;
			circle->r = 0xff;
		}
		else
		{
			circle->b = 0xff;
			circle->r = 0;
		}
		DrawImage(circle);
		DrawDigit(276, 68, 0xff, 0xff, 0xff, 4);

		// Circles 2nd row
		circle->x = 16;
		circle->y = 136;
		if(framecnt == 5)
		{
			circle->b = 0;
			circle->r = 0xff;
		}
		else
		{
			circle->b = 0xff;
			circle->r = 0;
		}
		DrawImage(circle);
		DrawDigit(36, 148, 0xff, 0xff, 0xff, 5);

		circle->x = 96;
		circle->y = 136;
		if(framecnt == 6)
		{
			circle->b = 0;
			circle->r = 0xff;
		}
		else
		{
			circle->b = 0xff;
			circle->r = 0;
		}
		DrawImage(circle);
		DrawDigit(116, 148, 0xff, 0xff, 0xff, 6);

		circle->x = 176;
		circle->y = 136;
		if(framecnt == 7)
		{
			circle->b = 0;
			circle->r = 0xff;
		}
		else
		{
			circle->b = 0xff;
			circle->r = 0;
		}
		DrawImage(circle);
		DrawDigit(196, 148, 0xff, 0xff, 0xff, 7);

		circle->x = 256;
		circle->y = 136;
		if(framecnt == 8)
		{
			circle->b = 0;
			circle->r = 0xff;
		}
		else
		{
			circle->b = 0xff;
			circle->r = 0;
		}
		DrawImage(circle);
		DrawDigit(276, 148, 0xff, 0xff, 0xff, 8);

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
		DrawDigit(248, 16, 0, 0, 0, msd);
		DrawDigit(272, 16, 0, 0, 0, lsd);

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

		if (pressed & PAD_BUTTON_A)
			pause = !pause;
		
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = PASSIVELAG;
		}										
	}
	FreeImage(&back);
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
	u32		    pressed, oldvmode = vmode;		
	char 		buffer[20];
	
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

		DrawString(32, 8, 0, 0xff, 0, "Current Resolution:");
		DrawString(140, 8, 0, 0xff, 0, res == 0 ? (IsPAL ? "288p" : "240p") : (IsPAL ? "576i" : "480i"));
#ifdef WII_VERSION
		DrawString(180, 8, 0xff, 0xff, 0xff, "Press HOME button for help");
#else
		DrawString(180, 8, 0xff, 0xff, 0xff, "Press Start button for help");
#endif

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
					DrawString(32,      40+i*8, 0xff, 0xff, 0.0, "Switched to");
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
			HelpData = ALTERNATE;
			DrawHelpWindow();
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
	SetVideoMode(oldvmode);				
	SetupGX();
}

