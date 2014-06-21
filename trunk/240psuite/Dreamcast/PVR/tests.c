/* 
 * 240p Test Suite
 * Copyright (C)2011 Artemio Urbina
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

#include <kos.h>
#include <stdlib.h>
#include <dc/sound/sound.h>
#include <dc/sound/sfxmgr.h>

#ifdef USE_FFTW
	#include <math.h>
	#include <complex.h>
	#include <fftw/fftw3.h>
#endif

#include "controller.h"
#include "tests.h"
#include "image.h"
#include "font.h"
#include "vmodes.h"
#include "vmu.h"

#include "help.h"
#include "menu.h"

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
	int		done = 0, x = dW/2, y = dH/2, invert = 0, frame = 0, text = 0, selback = 0, sprite = 0;
	uint16		pressed;		
	ImagePtr	back[4], ssprite, shadow, buzz, buzzshadow, overlay;
	controller 	*st;

	back[1] = LoadKMG("/rd/sonicback.kmg.gz", 0);
	if(!back[1])
		return;
	back[2] = LoadKMG("/rd/checkpos.kmg.gz", 1);
	if(!back[2])
		return;
	back[3] = LoadKMG("/rd/stripespos.kmg.gz", 1);
	if(!back[3])
		return;
	overlay = LoadKMG("/rd/sonicfloor.kmg.gz", 0);
	if(!overlay)
		return;

	if(vmode != VIDEO_480P && vmode != VIDEO_480I && vmode != VIDEO_576I)
	{		
		back[0] = LoadKMG("/rd/motoko.kmg.gz", 0);
		if(!back[0])
			return;
	}
	else
	{
		back[0] = LoadKMG("/rd/480/motoko-480.kmg.gz", 0);
		if(!back[0])
			return;

		back[0]->scale = 0;
		back[1]->scale = 0;
		back[2]->scale = 0;
		back[3]->scale = 0;
		overlay->scale = 0;

		back[1]->y = (dH - 240)/2;
		overlay->y = (dH - 240)/2;
	}
		
	ssprite = LoadKMG("/rd/shadow.kmg.gz", 0);	
	if(!ssprite)
		return;
	buzz = LoadKMG("/rd/buzzbomber.kmg.gz", 0);
	if(!buzz)
		return;
	buzzshadow = LoadKMG("/rd/buzzbomberShadow.kmg.gz", 0);
	if(!buzzshadow)
		return;
	
	srand((int)(time(0) ^ getpid()));
	sprite = rand() % 2;
	if(sprite == 0)
		shadow = ssprite;
	else
		shadow = buzzshadow;
	updateVMU(" Shadow  ", "  even ", 1);
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

		if(text)
		{
			if(vmode != VIDEO_480I && vmode != VIDEO_480P)
				DrawStringB(140, 30, 0, 1.0, 0, msg);
			else
				DrawStringB(450, 40, 0, 1.0, 0, msg);
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

		st = ReadController(0, &pressed);
		if(st)
		{
			if (st->buttons & CONT_DPAD_UP)
				y --;
		
			if (st->buttons & CONT_DPAD_DOWN)
				y ++;
		
			if (st->buttons & CONT_DPAD_LEFT)
			{
				x --;
				FlipH(buzz, 0);
				FlipH(buzzshadow, 0);
			}
		
			if (st->buttons & CONT_DPAD_RIGHT)
			{
				x ++;
				FlipH(buzz, 1);
				FlipH(buzzshadow, 1);
			}

			// Joystick
			if(st->joyx != 0)
			{
				x += st->joyx/40;
				FlipH(buzz, st->joyx > 0 ? 1 : 0);
				FlipH(buzzshadow, st->joyx > 0 ? 1 : 0);
			}
		
			if(st->joyy != 0)
				y += st->joyy/40;
		
			if (pressed & CONT_B)
				done =	1;

			if (pressed & CONT_X)
			{
				invert = !invert;
				if(invert)
				{
					sprintf(msg, "Shadow on odd frames");
					updateVMU(" Shadow  ", "  odd ", 1);
				}
				else
				{
					sprintf(msg, "Shadow on even frames");
					updateVMU(" Shadow  ", "  even ", 1);
				}
				text = 60;
			}
				
			if (pressed & CONT_A)
			{
				if(selback < 3)
					selback ++;
				else
					selback = 0;
			}
		
			if (pressed & CONT_Y)
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

			if (pressed & CONT_START)
				ShowMenu(DROPSHADOW);
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
	int		done = 0, x = dW/2, y = dH/2, selback = 0;
	uint16		pressed;
	ImagePtr	back[4], striped, overlay;
	controller *st;

	if(vmode != VIDEO_480P && vmode != VIDEO_480I && vmode != VIDEO_576I)
	{		
		back[0] = LoadKMG("/rd/motoko.kmg.gz", 0);
		if(!back[0])
			return;
		back[1] = LoadKMG("/rd/sonicback.kmg.gz", 1);
		if(!back[1])
			return;
		back[2] = LoadKMG("/rd/checkpos.kmg.gz", 1);
		if(!back[2])
			return;
		back[3] = LoadKMG("/rd/stripespos.kmg.gz", 1);
		if(!back[3])
			return;
		overlay = LoadKMG("/rd/sonicfloor.kmg.gz", 0);
		if(!overlay)
			return;
	}
	else
	{
		back[0] = LoadKMG("/rd/480/motoko-480.kmg.gz", 0);
		if(!back[0])
			return;
		back[0]->scale = 0;
		back[1] = LoadKMG("/rd/sonicback.kmg.gz", 0);
		if(!back[1])
			return;
		back[1]->scale = 0;
		back[2] = LoadKMG("/rd/checkpos.kmg.gz", 1);
		if(!back[2])
			return;
		back[2]->scale = 0;
		back[3] = LoadKMG("/rd/stripespos.kmg.gz", 1);
		if(!back[3])
			return;
		back[3]->scale = 0;
		overlay = LoadKMG("/rd/sonicfloor.kmg.gz", 0);
		if(!overlay)
			return;

		back[1]->y = (dH - 240)/2;
		overlay->y = (dH - 240)/2;
	}
	striped = LoadKMG("/rd/striped.kmg.gz", 0);
	if(!striped)
		return;
		
	updateVMU(" Striped ", "", 1);
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

		st = ReadController(0, &pressed);
		if(st)
		{
			if(st->buttons & CONT_Y)
			{
				if (pressed & CONT_DPAD_UP)
					y --;
			
				if (pressed & CONT_DPAD_DOWN)
					y ++;
	
				if (pressed & CONT_DPAD_LEFT)
					x --;

				if (pressed & CONT_DPAD_RIGHT)
					x ++;
			}
			else
			{
				if (st->buttons & CONT_DPAD_UP)
					y --;
			
				if (st->buttons & CONT_DPAD_DOWN)
					y ++;
	
				if (st->buttons & CONT_DPAD_LEFT)
					x --;

				if (st->buttons & CONT_DPAD_RIGHT)
					x ++;

			}

			// Joystick
			if(st->joyx != 0)
				x += st->joyx/40;
		
			if(st->joyy != 0)
				y += st->joyy/40;
		
			if (pressed & CONT_B)
				done =	1;
						
			if (pressed & CONT_A)
			{
				if(selback > 0)
					selback --;
				else
					selback = 3;
			}
		
			if (pressed & CONT_START)
				ShowMenu(STRIPED);
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
	char				msg[60];
	int				clicks[10], done = 0, view = 0, speed = 1, change = 1;
	int				x, y, x2, y2, audio = 0, pos = 0, i = 0, vibrate = 1, vary = 0, variation = 1;
	uint16				pressed;		
	ImagePtr			back, spriteA, spriteB, fixed;
	sfxhnd_t			beep;
	maple_device_t			*purupuru = NULL;
	static purupuru_effect_t	effect;
	controller 			*st;

	effect.duration = 1;
	effect.effect2 = PURUPURU_EFFECT2_UINTENSITY(1);
	effect.effect1 = PURUPURU_EFFECT1_INTENSITY(1);
	effect.special = PURUPURU_SPECIAL_MOTOR1;

	snd_init();

	back = LoadKMG("/rd/white.kmg.gz", 1);
	if(!back)
		return;

	back->r = 0.0f;
	back->g = 0.0f;
	back->b = 0.0f;
			
	srand((int)(time(0) ^ getpid()));
	updateVMU("Lag Test ", "", 1);
	fixed = LoadKMG("/rd/lag-per.kmg.gz", 0);
	if(!fixed)
		return;
	spriteA = LoadKMG("/rd/lag-per.kmg.gz", 0);
	if(!spriteA)
		return;
	spriteB = LoadKMG("/rd/lag-per.kmg.gz", 0);
	if(!spriteB)
		return;	

	beep = snd_sfx_load("/rd/beep.wav");
	x = 144;
	y = 60;
	x2 = 108;
	y2 = 96;
		
	fixed->x = 144;
	fixed->y = 96;	

	for(i = 0; i < 10; i++)
		clicks[i] = 0xFF;

	purupuru = maple_enum_type(0, MAPLE_FUNC_PURUPURU);
	while(!done && !EndProgram) 
	{
		StartScene();

		DrawImage(back);
		DrawImage(fixed);

		spriteA->x = x;
		spriteA->y = y;
		spriteB->x = x2;
		spriteB->y = y2;

		if(y == 96)
		{			
			if(audio && beep != SFXHND_INVALID)
				snd_sfx_play(beep, 255, speed*255);	// toggle pan to 0 and 255, l & r

			purupuru = maple_enum_type(0, MAPLE_FUNC_PURUPURU);
			if(vibrate)
			{
				if(purupuru)
					purupuru_rumble(purupuru, &effect);
			}

			spriteA->r = 1.0;
			spriteA->g = 0.0;
			spriteA->b = 0.0;

			spriteB->r = 1.0;
			spriteB->g = 0.0;
			spriteB->b = 0.0;
		}
		else
		{
			if(y == 97 || y == 95) // one pixel off
			{
				spriteA->r = 0.0;
				spriteA->g = 1.0;
				spriteA->b = 0.0;

				spriteB->r = 0.0;
				spriteB->g = 1.0;
				spriteB->b = 0.0;
			}

			if(y == 98 || y == 94) // two pixels off
			{
				spriteA->r = 1.0;
				spriteA->g = 1.0;
				spriteA->b = 1.0;

				spriteB->r = 1.0;
				spriteB->g = 1.0;
				spriteB->b = 1.0;
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
				DrawStringS(px, py, 1.0f, 1.0f, 1.0f, msg);
			
				px += (strlen(msg) + 2)*fw;
				if(clicks[i] == 1)
					sprintf(msg, "%2d frame", clicks[i]);
				else
					sprintf(msg, "%2d frames", clicks[i]);

				if(clicks[i] >= 0)
				{
					if(clicks[i] == 0)
						DrawStringS(px, py, 0.0f, 1.0f, 0.0f, msg);
					else
						DrawStringS(px, py, 1.0f, 1.0f, 1.0f, msg);
				}
				else
					DrawStringS(px, py, 1.0f, 0.0f, 0.0f, msg);
			}
		}

		sprintf(msg, "Audio: %s", audio ? "on" : "off");
		DrawStringS(200, 20, 1.0f, 1.0f, 1.0f, msg);
		sprintf(msg, "Timing: %s", variation ? "random" : "rhythmic");
		DrawStringS(200, 20+fh, 1.0f, 1.0f, 1.0f, msg);
		if(purupuru)
		{
			sprintf(msg, "Vibration: %s", vibrate ? "on" : "off");
			DrawStringS(200, 20+2*fh, 1.0f, 1.0f, 1.0f, msg);
		}
		else
			DrawStringS(200, 20+2*fh, 1.0f, 1.0f, 1.0f, "Vibration: n/a");

		DrawStringS(20, 170, 0.0f, 1.0f, 0.0f, "Press \"A\" when the sprite is aligned with the background.");
		DrawStringS(20, 170+fh, 0.0f, 1.0f, 0.0f, "Negative values mean you pressed \"A\" before they intersected");
		DrawStringS(20, 170+2*fh, 0.0f, 1.0f, 0.0f, "\"X\" button toggles horizontal and vertical movement.");
		DrawStringS(20, 170+3*fh, 0.0f, 1.0f, 0.0f, "\"Y\" button toggles rhythmic timing.");
		DrawStringS(20, 170+4*fh, 0.0f, 1.0f, 0.0f, "\"R\" trigger toggles audio feedback.");
		if(purupuru)
			DrawStringS(20, 170+5*fh, 0.0f, 1.0f, 0.0f, "\"L\" trigger toggles vibration feedback.");

		EndScene();

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_A)
			{
				if(change)
				{
					clicks[pos] = (y - 96) *speed;
		
					sprintf(msg, " Off: %d", clicks[pos]);
					updateVMU("Lag Test ", msg, 1);
		
					if(clicks[pos] >= 0)
					{
						change = 0;
						pos ++;
					}
			
					if(pos > 9)
						done = 1;
				}
			}

			if (pressed & CONT_X)
			{
				view ++;
				if(view > 2)
					view = 0;
			}
						
			if (pressed & CONT_RTRIGGER)
				audio =	!audio;				
		
			if (pressed & CONT_LTRIGGER)
				vibrate = !vibrate;				
		
			if (pressed & CONT_Y)
			{
				variation = !variation;
				if(!variation)
					vary = 0;
			}

			if (pressed & CONT_B)
				done =	1;				

			if (pressed & CONT_START)
				ShowMenu(MANUALLAG);
		}
		
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


	}

	if(beep != SFXHND_INVALID)
		snd_sfx_unload(beep);
	FreeImage(&back);
	FreeImage(&fixed);
	FreeImage(&spriteA);
	FreeImage(&spriteB);	
	snd_shutdown();

	updateVMU("Lag Test ", "-Results-", 1);
	if(pos > 9)
	{
		int	total = 0;
		double	res = 0, ms = 0;
		ImagePtr wall;
		
		done = 0;
		wall = LoadKMG("/rd/back.kmg.gz", 0);
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
						DrawStringS(px, py, 0, 1.0f, 0, msg);
					else
						DrawStringS(px, py, 1.0f, 1.0f, 1.0f, msg);
				}
				else
					DrawStringS(px, py, 1.0f, 0, 0, msg);
	
				if(clicks[i] >= 0)
					total += clicks[i];
	
			}

			res = (double)total / 10.0;
			ms = (double)(res*(1000.0/(IsPAL ? 50.0 : 60.0)));
			sprintf(msg, "%d/10 = %0.2f average frames ~= %0.2f ms", total, res, ms);
			DrawStringS(60, 110, 1.0f, 0, 0, "+");
			DrawStringS(55, 70 + fh*10, 1.0f, 0, 0, "_____");
			DrawStringS(60, 70 + fh*11, 1.0f, 1.0f, 1.0f, msg);
			if(IsPAL)
				DrawStringS(60, 70 + fh*12, 0.0f, 1.0f, 1.0f, "Keep in mind that a frame is around 20 ms");
			else
				DrawStringS(60, 70 + fh*12, 0.0f, 1.0f, 1.0f, "Keep in mind that a frame is around 16.68 ms");

			if(total && total < 5)
			{
				DrawStringS(100, 120, 0.0f, 1.0f, 0.0f, "EXCELLENT REFLEXES!");
				updateVMUFlash("Lag Test ", " AWESOME", 1);
			}
			if(total == 0)
			{
				DrawStringS(100, 120, 0.0f, 1.0f, 0.0f, "INCREDIBLE REFLEXES!!");
				updateVMUFlash("Lag Test ", "-PERFECT-", 1);
			}

			EndScene();

			st = ReadController(0, &pressed);	
			if(st)
			{
				if (pressed & CONT_B)
					done =	1;
				if (pressed & CONT_START)
					ShowMenu(MANUALLAG);
			}
		}
		FreeImage(&wall);
	}
	return;
}

void ScrollTest()
{
	int 		done = 0, speed = 1, acc = 1, x = 0, pause = 0;
	int 		oldvmode = vmode;
	uint16		pressed;		
	ImagePtr	back, overlay;
	controller	*st;

	back = LoadKMG("/rd/sonicback.kmg.gz", 0);
	if(!back)
		return;
	overlay = LoadKMG("/rd/sonicfloor.kmg.gz", 0);
	if(!overlay)
		return;
	
	back->y = (dH - 240)/2;
	overlay->y = (dH - 240)/2;

	IgnoreOffset(back);
	IgnoreOffset(overlay);

	updateVMU(" Scroll  ", "", 1);
	while(!done && !EndProgram) 
	{
		if(oldvmode != vmode)
		{
			back->y = (dH - 240)/2;
			overlay->y = (dH - 240)/2;
			if(offsetY)  // center in PAL modes
			{
				back->y -= offsetY;
				overlay->y -= offsetY;
			}
			oldvmode = vmode;
		}

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

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_DPAD_UP)
				speed ++;

			if (pressed & CONT_DPAD_DOWN)
				speed --;

			if (pressed & CONT_B)
				done = 1;

			if (pressed & CONT_A)
				pause = !pause;

			if (pressed & CONT_X)
				acc *= -1;

			if (pressed & CONT_START)
				ShowMenu(SCROLL);
		}
	}
	FreeImage(&back);
	FreeImage(&overlay);
	return;
}

void GridScrollTest()
{
	int 			done = 0, speed = 1, acc = 1, x = 0, y = 0, pause = 0, direction = 0;
	uint16			pressed;		
	ImagePtr		back;
	controller		*st;

	back = LoadKMG("/rd/circles_grid.kmg.gz", 0);
	if(!back)
		return;  
	
	//back->y = (dH - 240)/2;  

	updateVMU("G. Scroll", "", 1);
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
	
		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_DPAD_UP)
				speed ++;

			if (pressed & CONT_DPAD_DOWN)
				speed --;

			if (pressed & CONT_B)
				done = 1;

			if (pressed & CONT_A)
				pause = !pause;

			if (pressed & CONT_Y)
				acc *= -1;

			if (pressed & CONT_X)
				direction = !direction;

			// Joystick
			if(st->joyx != 0)
				x += st->joyx/30;
		
			if(st->joyy != 0)
				y += st->joyy/30;

			if (pressed & CONT_START)
				ShowMenu(GRIDSCROLL);
		}

	}
	FreeImage(&back);  
	return;
}

void DrawStripes()
{
	int 			done = 0, field = 1, alternate = 0,
				frame = 0, dframe = 0, vertical = 0;
	uint16			pressed;		
	ImagePtr		stripespos, stripesneg;
	ImagePtr		vstripespos, vstripesneg;
	controller	*st;

	stripespos = LoadKMG("/rd/stripespos.kmg.gz", 1);
	if(!stripespos)
		return;
	stripesneg = LoadKMG("/rd/stripesneg.kmg.gz", 1);
	if(!stripesneg)
		return;

	vstripespos = LoadKMG("/rd/vertstripespos.kmg.gz", 1);
	if(!vstripespos)
		return;
	vstripesneg = LoadKMG("/rd/vertstripesneg.kmg.gz", 1);
	if(!vstripesneg)
		return;

	updateVMU(" Stripes", "", 1);
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
			if(vmode != VIDEO_480P &&  vmode != VIDEO_480I)
				DrawStringB(20, 210, 1.0f, 1.0f, 1.0f, msg);
			else
				DrawStringB(20, 460, 1.0f, 1.0f, 1.0f, msg);
			frame ++;
			if(frame > 59)
				frame = 0;
		}
		EndScene();

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;				
						
			if (pressed & CONT_A)
			{
				alternate = !alternate;
				if(alternate)
					updateVMU(" Stripes", "alternate", 1);
				else
					updateVMU(" Stripes", " still ", 1);
			}
						
			if (pressed & CONT_Y)
				vertical = !vertical;
						
			if (pressed & CONT_X)
			{
				dframe = !dframe;
				frame = 0;
			}
						
			if (pressed & CONT_RTRIGGER && !alternate)
				field = !field;

			if (pressed & CONT_START)
				ShowMenu(STRIPESHELP);
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
	int 			done = 0, field = 1, alternate = 0,
				frame = 0, dframe = 0;
	uint16			pressed;		
	ImagePtr		checkpos, checkneg;
	controller	*st;

	checkpos = LoadKMG("/rd/checkpos.kmg.gz", 1);
	if(!checkpos)
		return;
	checkneg = LoadKMG("/rd/checkneg.kmg.gz", 1);
	if(!checkneg)
	{
		FreeImage(&checkpos);
		return;
	}

	updateVMU("CHKB PTTN", "", 1);
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
			if(vmode != VIDEO_480I && vmode != VIDEO_480P)
				DrawStringB(20, 210, 1.0f, 1.0f, 1.0f, msg);
			else
				DrawStringB(20, 460, 1.0f, 1.0f, 1.0f, msg);
			frame ++;
			if(frame > 59)
				frame = 0;
		}
		EndScene();

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;				
						
			if (pressed & CONT_A)
			{
				alternate = !alternate;				
				if(alternate)
					updateVMU("CHKB PTTN", "alternate", 1);
				else
					updateVMU("CHKB PTTN", " still ", 1);
			}
		
			if (pressed & CONT_X)
			{
				dframe = !dframe;
				frame = 0;
			}
						
			if (pressed & CONT_RTRIGGER && !alternate)
				field = !field;

			if (pressed & CONT_START)
				ShowMenu(CHECKHELP);
		}
	}
	FreeImage(&checkpos);
	FreeImage(&checkneg);
	return;
}

void SoundTest()
{
	int 		done = 0, sel = 1, play = 0, pan = 0;
	uint16		pressed;		
	ImagePtr	back;
	sfxhnd_t	beep;
	controller	*st;

	back = LoadKMG("/rd/back.kmg.gz", 0);
	if(!back)
		return;
	snd_init();	
	beep = snd_sfx_load("/rd/beep.wav");
	if(!beep)
		return;
	
	updateVMU("Sound Test", "", 1);
	while(!done && !EndProgram) 
	{
		StartScene();
		DrawImage(back);

		DrawStringS(130, 60, 1.0f, 1.0f, 1.0f, "Sound Test"); 
		DrawStringS(80, 120, 1.0f, sel == 0 ? 0 : 1.0f,	sel == 0 ? 0 : 1.0f, "Left Channel"); 
		DrawStringS(120, 130, 1.0f, sel == 1 ? 0 : 1.0f,	sel == 1 ? 0 : 1.0f, "Center Channel");
		DrawStringS(160, 120, 1.0f, sel == 2 ? 0 : 1.0f,	sel == 2 ? 0 : 1.0f, "Right Channel");
		EndScene();

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;								

			if (pressed & CONT_A)
				play =	1; 						

			if (pressed & CONT_DPAD_LEFT)
				sel --;

			if (pressed & CONT_DPAD_RIGHT)
				sel ++;

			if (pressed & CONT_START)
				ShowMenu(SOUNDHELP);
		}
		
		if(sel < 0)
			sel = 2;

		if(sel > 2)
			sel = 0;

		switch(sel)
		{
			case 0:
				pan = 0;
				break;
			case 1:
				pan = 128;
				break;
			case 2:
				pan = 255;
				break;
		}

		if(play && beep != SFXHND_INVALID)
		{
			snd_sfx_play(beep, 255, pan);
			play = 0;
		}
	}
	if(beep != SFXHND_INVALID)
		snd_sfx_unload(beep);
	FreeImage(&back);
	snd_shutdown();
	return;
}

void LEDZoneTest()
{	
	int		done = 0, x = 0, y = 0, selsprite = 1, show = 1;
	uint16		pressed;
	ImagePtr	back, sprite[5];
	controller 	*st;

	back = LoadKMG("/rd/white.kmg.gz", 1);
	if(!back)
		return;

	back->r = 0.0f;
	back->g = 0.0f;
	back->b = 0.0f;
			
	sprite[0] = LoadKMG("/rd/sprite0led.kmg.gz", 0);
	if(!sprite[0])
		return;
	sprite[1] = LoadKMG("/rd/sprite1led.kmg.gz", 0);
	if(!sprite[1])
		return;
	sprite[2] = LoadKMG("/rd/sprite2led.kmg.gz", 0);
	if(!sprite[2])
		return;
	sprite[3] = LoadKMG("/rd/sprite3led.kmg.gz", 0);
	if(!sprite[3])
		return;
	sprite[4] = LoadKMG("/rd/sprite4led.kmg.gz", 0);
	if(!sprite[4])
		return;
		
	updateVMU("Backlit", "", 1);
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

		st = ReadController(0, &pressed);
		if(st)
		{
			if(show)
			{
				if (st->buttons & CONT_DPAD_UP)
					y --;
			
				if (st->buttons & CONT_DPAD_DOWN)
					y ++;
	
				if (st->buttons & CONT_DPAD_LEFT)
					x --;
	
				if (st->buttons & CONT_DPAD_RIGHT)
					x ++;
				
	
				// Joystick
				if(st->joyx != 0)
					x += st->joyx/40;
			
				if(st->joyy != 0)
					y += st->joyy/40;
			}
		
			if (pressed & CONT_B)
				done =	1;
						
			if (pressed & CONT_LTRIGGER)
			{
				if(selsprite > 0)
					selsprite --;
				else
					selsprite = 4;
			}
		
			if (pressed & CONT_RTRIGGER)
			{
				if(selsprite < 4)
					selsprite ++;
				else
					selsprite = 0;
			}

			if (pressed & CONT_A)
				show = !show;

			if (pressed & CONT_START)
				ShowMenu(BACKLITHELP);
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
	uint16		pressed, lsd, msd, pause = 0;		
	ImagePtr	back, circle;
	controller	*st;
	
	back = LoadKMG("/rd/white.kmg.gz", 1);
	if(!back)
		return;

	circle= LoadKMG("/rd/circle.kmg.gz", 0);
	if(!circle)
		return;
	circle->r = 0.0f;
	circle->g = 0.0f;
	circle->b = 1.0f;

	LoadNumbers();
	updateVMU("LAG TEST", "", 1);
	while(!done && !EndProgram) 
	{
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
			circle->r = 1.0f;
		}
		else
		{
			circle->b = 1.0f;
			circle->r = 0;
		}
		DrawImage(circle);
		DrawDigit(36, 68, 1.0f, 1.0f, 1.0f, 1);

		circle->x = 96;
		circle->y = 56;
		if(framecnt == 2)
		{
			circle->b = 0;
			circle->r = 1.0f;
		}
		else
		{
			circle->b = 1.0f;
			circle->r = 0;
		}
		DrawImage(circle);
		DrawDigit(116, 68, 1.0f, 1.0f, 1.0f, 2);

		circle->x = 176;
		circle->y = 56;
		if(framecnt == 3)
		{
			circle->b = 0;
			circle->r = 1.0f;
		}
		else
		{
			circle->b = 1.0f;
			circle->r = 0;
		}
		DrawImage(circle);
		DrawDigit(196, 68, 1.0f, 1.0f, 1.0f, 3);

		circle->x = 256;
		circle->y = 56;
		if(framecnt == 4)
		{
			circle->b = 0;
			circle->r = 1.0f;
		}
		else
		{
			circle->b = 1.0f;
			circle->r = 0;
		}
		DrawImage(circle);
		DrawDigit(276, 68, 1.0f, 1.0f, 1.0f, 4);

		// Circles 2nd row
		circle->x = 16;
		circle->y = 136;
		if(framecnt == 5)
		{
			circle->b = 0;
			circle->r = 1.0f;
		}
		else
		{
			circle->b = 1.0f;
			circle->r = 0;
		}
		DrawImage(circle);
		DrawDigit(36, 148, 1.0f, 1.0f, 1.0f, 5);

		circle->x = 96;
		circle->y = 136;
		if(framecnt == 6)
		{
			circle->b = 0;
			circle->r = 1.0f;
		}
		else
		{
			circle->b = 1.0f;
			circle->r = 0;
		}
		DrawImage(circle);
		DrawDigit(116, 148, 1.0f, 1.0f, 1.0f, 6);

		circle->x = 176;
		circle->y = 136;
		if(framecnt == 7)
		{
			circle->b = 0;
			circle->r = 1.0f;
		}
		else
		{
			circle->b = 1.0f;
			circle->r = 0;
		}
		DrawImage(circle);
		DrawDigit(196, 148, 1.0f, 1.0f, 1.0f, 7);

		circle->x = 256;
		circle->y = 136;
		if(framecnt == 8)
		{
			circle->b = 0;
			circle->r = 1.0f;
		}
		else
		{
			circle->b = 1.0f;
			circle->r = 0;
		}
		DrawImage(circle);
		DrawDigit(276, 148, 1.0f, 1.0f, 1.0f, 8);

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

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;				
						
			if (pressed & CONT_X && pause)
			{
				frames = hours = minutes = seconds = 0;
				framecnt = 1;
			}

			if (pressed & CONT_A)
				pause = !pause;

			if (pressed & CONT_START)
				ShowMenu(PASSIVELAG);
		}


		if(!pause)
		{
			frames ++;
			framecnt ++;
			if(framecnt > 8)
				framecnt = 1;
		}

		if(frames > 59)
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
	FreeImage(&circle);
	ReleaseNumbers();
}

uint16 ConvertToFrames(timecode *time)
{
	uint16	frames = 0;

	if(!time)
		return frames;

	frames = time->frames;
	frames += time->seconds*(IsPAL ? 50 : 60);
	frames += time->minutes*3600;
	frames += time->hours*216000;
	return frames;
}

void ConvertFromFrames(timecode *value, uint16 Frames)
{
	if(!value)
		return;
	value->hours = Frames / 216000;
	Frames = Frames % 216000;
	value->minutes = Frames / 3600;
	Frames = Frames % 3600;
	value->seconds = Frames / (IsPAL ? 50 : 60);
	value->frames = Frames % (IsPAL ? 50 : 60);
}

void Alternate240p480i()
{
	int 		frames = 0, seconds = 0, minutes = 0, hours = 0, done =  0, current = 0, res = 0, status = 0;
	timecode	times[20];
	uint16		pressed;		
	char 		buffer[20];
	controller	*st;
	ImagePtr	back;

	updateVMU("240p/480i", "", 1);
	if(vmode != VIDEO_240P)
	{
		ReleaseScanlines();
		ReleaseFont();
		Toggle240p480i(res);
		LoadFont();
		LoadScanlines();
	}

	back = LoadKMG("/rd/white.kmg.gz", 1);
	if(!back)
		return;

	back->r = 0.0f;
	back->g = 0.0f;
	back->b = 0.0f;
			
	while(!done && !EndProgram) 
	{
		StartScene();

		DrawImage(back);

		DrawString(32, 8, 0, 1.0f, 0, "Current Resolution:");
		DrawString(140, 8, 0, 1.0f, 0, res == 0 ? "240p" : "480i");
		DrawString(180, 8, 1.0, 1.0f, 1.0, "Press 'Start' for help");

		sprintf(buffer, "%02d:%02d:%02d:%02d", hours, minutes, seconds, frames);
		DrawString(32, 32, 1.0, 1.0, 1.0, "Elapsed Timer:");
		DrawString(140, 32, 1.0, 1.0, 1.0, buffer);

		if(current)
		{
			int i = 0;
			for(i = 0; i < current; i++)
			{
				if(times[i].type == 0)
				{
					DrawString(32,      40+i*8, 1.0, 1.0, 0.0, "Switched to");
					DrawString(32+12*5, 40+i*8, 1.0, 1.0, 0.0, times[i].res == 0 ? "240p" : "480i");
					DrawString(32+16*5, 40+i*8, 1.0, 1.0, 0.0, " at:");
				}
				else
					DrawString(32, 40+i*8, 0.0, 1.0, 0.0, "Viewed at:");
				sprintf(buffer, "%02d:%02d:%02d:%02d",
					times[i].hours, times[i].minutes, times[i].seconds, times[i].frames);
				DrawString(140, 40+i*8, 1.0, 1.0, 1.0, buffer);
				if(times[i].type != 0 && i >= 1 && i <= 19)
				{
					uint16 		framesA = 0, framesB = 0, res = 0;
					timecode 	len;

					framesB = ConvertToFrames(&times[i]);
					framesA = ConvertToFrames(&times[i - 1]);
					res = framesB - framesA;
					ConvertFromFrames(&len, res);
					sprintf(buffer, "%02d:%02d:%02d:%02d",
						len.hours, len.minutes, len.seconds, len.frames);
					DrawString(200, 40+i*8, 1.0, 0.0, 0.0, buffer);
				}
			}
		}

		EndScene();

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_START)
				ShowHelpWindow(ALTERNATE);

			if (pressed & CONT_B)
				done =	1;				
						
			if (pressed & CONT_A)
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
					ReleaseScanlines();
					ReleaseFont();
					Toggle240p480i(res);
					LoadFont();
					LoadScanlines();
				}
				if(status == 2)
				{
					times[current - 1].type = 1;
					times[current - 1].res = res;
					status = 0;
				}
			}
		}

		frames ++;

		if(frames > 59)
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
}

#ifdef USE_FFTW

#define CUE_FRAMES		5
#define SECONDS_TO_RECORD	2
#define RESULTS_MAX		10
#define BUFFER_SIZE 		50000	// we need around 45k only
#define	FFT_OM			-5000
#define	FFT_NOT_FOUND		-500

//#define DEBUG_FFT

typedef struct recording {
	uint8	*buffer;
	size_t	size;
	int	pos;
	uint8	overflow;
	uint8	recording;
} recording;

recording rec_buffer;

void DrawSIPScreen(ImagePtr back, ImagePtr wave, char *Status, int accuracy, double *Results, int ResCount)
{
	int	i = 0;
	char	DPres[40];
	char	Header[40];
	char	Res[40];

	StartScene();
	DrawImage(back);
	DrawImage(wave);

	if(IsPAL)
	{
		if(accuracy == 1)
			sprintf(DPres, "Frame accuracy: 1 frame 20ms");
		else
			sprintf(DPres, "Frame accuracy: 1/%d frame %0.3gms", accuracy, 20/accuracy);
	}
	else
	{
		if(accuracy == 1)
			sprintf(DPres, "Frame accuracy: 1 frame 16.67ms");
		else
			sprintf(DPres, "Frame accuracy: 1/%d frame %0.3gms", accuracy, 16.6667/accuracy);
	}

	DrawStringS(40, 60, 0.0f, 1.0f, 1.0f, "Lag Test via Microphone & Fast Fourier Transform"); 
	DrawStringS(50, 120, 1.0f, 1.0f, 1.0f, Status);
	DrawStringS(120, 200, 0.0f, 1.0f, 0.0f, DPres);

	if(ResCount)
	{
		if(ResCount == 1)
			sprintf(Header, "Last result:");
		else
			sprintf(Header, "Last %d results:", ResCount);
		DrawStringS(170, 90, 0.0f, 1.0f,	0.0f, Header);
		for(i = 1; i <= ResCount; i++)
		{
			if(Results[i-1] < 0 && Results[i-1] != FFT_OM && Results[i-1] != FFT_NOT_FOUND)
				sprintf(Res, "#G%.2d#G #YNoise at 1khz#Y", i);
			if(Results[i-1] == FFT_OM)
				sprintf(Res, "#G%.2d#G #ROut of Memory#R", i);
			if(Results[i-1] == FFT_NOT_FOUND)
				sprintf(Res, "#G%.2d#G #RNo tone detected#R", i);
			if(Results[i-1] >= 0)
				sprintf(Res, "#G%.2d#G Lag was #C%g#C frames", i, Results[i-1]);
			DrawStringS(160, 90+i*fh, 1.0f, 1.0f,	1.0f, Res);
		}
	}
	EndScene();
}

void sip_copy(maple_device_t *dev, uint8 *samples, size_t len)
{
	if(!rec_buffer.recording)
		return;

	if(!rec_buffer.buffer || !rec_buffer.size)
	{
#ifdef DEBUG_FFT
		printf("Invalid buffer or size\n");
#endif
		return;	
	}
	if(!dev || !samples || !len)
	{
#ifdef DEBUG_FFT
		printf("Invalid dev, samples or len\n");
#endif
		return;
	}
	
	if(len > (rec_buffer.size - (size_t)rec_buffer.pos))
	{
		rec_buffer.overflow = 1;
#ifdef DEBUG_FFT
		printf("Buffer overflow\n");
#endif
		return;
	}

	memcpy(rec_buffer.buffer + rec_buffer.pos, samples, len);
	rec_buffer.pos += len;
}

void SIPLagTest()
{
	int 		done = 0, status = 0, counter = 0, accuracy = 1;
	uint16		pressed;		
	ImagePtr	back, wave;
	sfxhnd_t	beep;  
	controller	*st;
	maple_device_t  *sip = NULL;  
	double          Results[RESULTS_MAX];
	int             ResCount = 0;
	char		DStatus[100];
	float		delta = 0.01f;

	DStatus[0] = 0x0;
	back = LoadKMG("/rd/back.kmg.gz", 0);
	if(!back)
		return;

	wave = LoadKMG("/rd/1khz.kmg.gz", 0);
	if(!wave)
	{
		FreeImage(&back);
		return;
	}
	wave->alpha = 0.05f;

	srand((int)(time(0) ^ getpid()));
	snd_init();		
	beep = snd_sfx_load("/rd/Sample.wav");
	if(!beep)
	{
		FreeImage(&back);
		FreeImage(&wave);
		return;
	}

	rec_buffer.buffer = malloc(sizeof(uint8)*BUFFER_SIZE);
	if(!rec_buffer.buffer)
	{
		if(beep != SFXHND_INVALID)
			snd_sfx_unload(beep);
		FreeImage(&back);
		return;
	}
	rec_buffer.size = sizeof(uint8)*BUFFER_SIZE;
	rec_buffer.pos = 0;
	rec_buffer.overflow = 0;
	rec_buffer.recording = 0;

	sip = maple_enum_type(0, MAPLE_FUNC_MICROPHONE);
	if(!sip)
		return;

	sip_set_gain(sip, SIP_MAX_GAIN); // SIP_DEFAULT_GAIN  
	sip_set_sample_type(sip, SIP_SAMPLE_16BIT_SIGNED); 
	sip_set_frequency(sip, SIP_SAMPLE_11KHZ); // 11.025kHz
		
	if(sip_start_sampling(sip, sip_copy, 1) == MAPLE_EOK)
	{
#ifdef DEBUG_FFT
		printf("Mic ok\n");
#endif
	}
	else
	{
		printf("Mic Recording: Failed\n");
		done = 1;
	}

	updateVMU("Lag v/Micr", "", 1);
	sprintf(DStatus, "Press A");
	while(!done && !EndProgram) 
	{		
		if(status)
		{
			wave->alpha += delta;
			if(wave->alpha > 0.6f)
			{
				wave->alpha = 0.6f;
				delta *= -1;
			}
			if(wave->alpha < 0.2f)
			{
				wave->alpha = 0.2f;
				delta *= -1;
			}
		}

		DrawSIPScreen(back, wave, DStatus, accuracy, Results, ResCount);

		sip = maple_enum_type(0, MAPLE_FUNC_MICROPHONE);
		if(!sip)
			done = 1;
	
		if(status == 0) // no input if we are sampling
		{
			st = ReadController(0, &pressed);
			if(st)
			{
				if (pressed & CONT_START)
					ShowMenu(FFTHELP);

				if (pressed & CONT_B)
					done =1;				
	
				if (pressed & CONT_A)
					status = 1;

				if (pressed & CONT_RTRIGGER)
					accuracy *= 2;

				if (pressed & CONT_LTRIGGER)
					accuracy /= 2;

				if(accuracy > 4)
					accuracy = 4;

				if(accuracy < 1)
					accuracy = 1;
			}
		}

		if(status == 2 || status == 4)
		{
			// frame accurate sampling
			if(status == 2 && counter == CUE_FRAMES && !rec_buffer.recording)
				rec_buffer.recording = 1;

			counter --;
			if(!counter)
				status++;
			
			if(status == 4)
			{
				sprintf(DStatus, "Recording Frame: %d", SECONDS_TO_RECORD*(IsPAL ? 50 : 60) - counter);
				if(counter == 1)
					rec_buffer.recording = 0;

				if(rec_buffer.pos == 0)
				{
					sprintf(DStatus, "#YPlease remove and reinsert Microphone#Y");
					rec_buffer.recording = 0;
					status = 0;
				}
			}
		}

		if(status == 1 && sip)
		{
			sprintf(DStatus, "Starting");
			status = 2;
			counter = CUE_FRAMES;
		}

		if(status == 3 && beep != SFXHND_INVALID)
		{
			snd_sfx_play(beep, 255, 128);
			status = 4;
			counter = SECONDS_TO_RECORD*(IsPAL ? 50 : 60); // record N seconds + CUE frames
		}    
		
		if(status == 5)
		{
			sprintf(DStatus, "Stopped sampling");

#ifdef DEBUG_FFT
			printf("Got %d bytes\n", (int)rec_buffer.pos);
#endif
			if(rec_buffer.buffer && rec_buffer.pos > 0)
			{
				double value;

				DrawSIPScreen(back, wave, "Analyzing...", accuracy, Results, ResCount);
				value = ProcessSamples((short*)rec_buffer.buffer, rec_buffer.pos/2,
					11025, (IsPAL ? 50.0 : 60.0)*accuracy, 1000);          
				if(value < 0 && value != FFT_NOT_FOUND && value != FFT_OM)
					sprintf(DStatus, "#YNoise at 1khz#Y");
				if(value == FFT_OM)
					sprintf(DStatus, "#ROut of Memory#R");
				if(value == FFT_NOT_FOUND)
					sprintf(DStatus, "#YCheck audio system#Y");
				if(value >= 0)
					sprintf(DStatus, "Lag is #C%g#C frames", value);

				if(ResCount == RESULTS_MAX)
				{
					int i = 0;

					for(i = 0; i < RESULTS_MAX - 1; i++)
						Results[i] = Results[i+1];
					ResCount --;
				}
				Results[ResCount++] = value;
			}        
			else
				sprintf(DStatus, "#YRecording failed#Y");

			rec_buffer.pos = 0;
			status = 0;
		}   
	}

	if(sip)
		sip_stop_sampling(sip, 0);

	if(rec_buffer.size)
	{
		free(rec_buffer.buffer);
		rec_buffer.size = 0;
		rec_buffer.pos = 0;
	}

	if(beep != SFXHND_INVALID)
		snd_sfx_unload(beep);
	FreeImage(&wave);
	FreeImage(&back);

	fftw_cleanup();  
	snd_shutdown();
	return;
}

double ProcessSamples(short *samples, size_t size, long samplerate, double secondunits, double searchfreq)
{
	long          	samplesize = 0, arraysize = 0;  
	long          	i = 0, f = 0, framesizernd = 0; 
	double        	*in, root = 0, framesize = 0;  
	double        	*MaxFreqArray = NULL;
	fftw_complex  	*out;
	fftw_plan	p = NULL;
#ifdef DEBUG_FFT
	long		time = 0;
	uint64		start, end;
#endif
	double    	mins, maxs;
	double       	boxsize = 0;
	int          	casefrq = 0;  
	int 		found = 0;
	long 		pos = 0, count = 0;    
	long 		tpos = 0, tcount = 0;    
	double		value = FFT_NOT_FOUND;
	
	samplesize = (long) size;  

	framesize = samplerate/secondunits;
	framesizernd = (long)framesize;  

#ifdef DEBUG_FFT
	printf("Samples are at %lu Khz and %g seconds long. A Frame is %g samples.\n",
			samplerate, (double)samplesize/samplerate, framesize);    

	start = timer_ms_gettime64();
#endif
	in = (double*) fftw_malloc(sizeof(double) * framesizernd);
	if(!in)
		return FFT_OM;
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (framesizernd/2+1));  
	if(!out)
	{     
		 fftw_free(in); 
		 return FFT_OM;
	}
		
	arraysize = framesizernd;
	MaxFreqArray = malloc(sizeof(double)*(samplesize/framesize - 1));
	if(!MaxFreqArray)
	{
		 fftw_free(in); 
		 fftw_free(out);
		 return FFT_OM;
	}

	boxsize = (double)arraysize/(double)samplerate;  
	for(f = 0; f < samplesize/framesize - 1; f++)
	{
		double mainfreq = 0, max = 0;
		long   maxind = -1;
		long   framestart = 0;

		framestart = framesize*f;
		for(i = 0; i < framesizernd; i++)
			in[i] = (double)samples[i+framestart];

#ifdef DEBUG_FFT
		//printf("Estimating plan\n");
#endif
		p = fftw_plan_dft_r2c_1d(arraysize, in, out, FFTW_ESTIMATE);      

#ifdef DEBUG_FFT
		//printf("Executing FFTW\n");
#endif
		fftw_execute(p); 
	
#ifdef DEBUG_FFT
		//printf("Analyzing results\n");
#endif
		root = sqrt(arraysize);
		for(i = 0; i < arraysize/2+1; i++)
		{
			double r1 = creal(out[i]);
			double i1 = cimag(out[i]);
			double val = 0;
	
			val = sqrt(sqrt(r1 * r1 + i1 * i1) / root);
			if(val > max)
			{
				max = val;
				maxind = i;
			}    
		}
		
		mainfreq = (double)((double)maxind/boxsize);
		MaxFreqArray[f] = mainfreq;
		
		fftw_destroy_plan(p);
	}

#ifdef DEBUG_FFT
	end = timer_ms_gettime64();
	time = end - start;
	printf("FFT for %g frames took %ld\n", samplesize/framesize - 1, time);
	start = end;
#endif

	casefrq = (int)ceil(searchfreq/(1/boxsize));
	mins = (casefrq - 1) / boxsize;
	maxs = (casefrq + 1) / boxsize;
#ifdef DEBUG_FFT
	printf("Searching for %g, due to samplerate, arraysize and accurancy %d it is %g, between %g and %g\n",
		searchfreq, (int)secondunits/(IsPAL ? 50 : 60), casefrq/boxsize, mins, maxs);
#endif
 
	for(f = 0; f < samplesize/framesize - 1; f++)
	{
		if(!found)
		{
#ifdef DEBUG_FFT
			printf("Frame %ld: Main frequency %g Hz\n", f-CUE_FRAMES, MaxFreqArray[f]);
#endif
			if(count)
			{       	 	
				if(MaxFreqArray[f] < mins || MaxFreqArray[f] > maxs)        
				{
					// tentative result if all fails
					if(!tpos) // only the first result
					{
						tpos = pos;
						tcount = count;
					}
	
					count = 0;          
					pos = 0;
				}
				else
				{
					count++;
					if(count == (int)secondunits)
					{
						pos -= CUE_FRAMES*(secondunits/(IsPAL ? 50.0 : 60.0));
						value = pos/(secondunits/(IsPAL ? 50.0 : 60.0));
#ifdef DEBUG_FFT
						printf("Found at %g frames -> %g sec\n", value, pos/secondunits);
#endif
						found = 1;
					}
				}
			}
				
			if(!count && MaxFreqArray[f] >= mins && MaxFreqArray[f] <= maxs)              
			{
				pos = f;        
				count = 1;
			}      
		}
	}
	
	if(!found && tpos != 0 && tcount > secondunits/2) // Did we find one at least 1/2 second long?
	{
		pos = tpos - CUE_FRAMES*(secondunits/(IsPAL ? 50.0 : 60.0));
		value = pos/(secondunits/(IsPAL ? 50.0 : 60.0));
#ifdef DEBUG_FFT
		printf("Found (heur %ld) at %g frames -> %g sec\n", tcount, value, pos/secondunits);
#endif
	}

#ifdef DEBUG_FFT
	end = timer_ms_gettime64();
	time = end - start;
	printf("Processing frequencies took %ld\n", time);
#endif

	free(MaxFreqArray);
	MaxFreqArray = NULL;  
	
	fftw_free(in); 
	fftw_free(out);
	return(value);
}

#endif
