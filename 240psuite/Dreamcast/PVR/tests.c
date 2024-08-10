/* 
 * 240p Test Suite
 * Copyright (C)2011-2023 Artemio Urbina
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
	double internal_latency;
} timecode;

void DropShadowTest()
{
	char		msg[50];
	int			oldvmode = vmode, frame = 0, text = 0, reload = 0;
	int			selback = 0, sprite = 0, d_shown = 0;
	int			done = 0, x = dW/2-50, y = dH/2, invert = 0;
	uint16		pressed, currentsonic = 0, currentframe = 0, i = 0;
	ImagePtr	back[4], ssprite, shadow, buzz, buzzshadow, overlay, sonicback[4];
	controller 	*st;

	reload = 1;

	for(i = 0; i < 4; i++)
		back[i] = NULL;
	for(i = 0; i < 4; i++)
		sonicback[i] = NULL;
	overlay = NULL;
		
	ssprite = LoadIMG("/rd/shadow.kmg.gz", 0);	
	if(!ssprite)
		return;
	buzz = LoadIMG("/rd/buzzbomber.kmg.gz", 0);
	if(!buzz)
		return;
	buzzshadow = LoadIMG("/rd/buzzbomberShadow.kmg.gz", 0);
	if(!buzzshadow)
		return;
	
	sprite = rand() % 2;
	if(sprite == 0)
		shadow = ssprite;
	else
		shadow = buzzshadow;
	while(!done && !EndProgram) 
	{
		if(reload || oldvmode != vmode)
		{
			if(back[0]) FreeImage(&back[0]);
			for(i = 0; i < 4; i++)
				if(sonicback[i]) FreeImage(&sonicback[i]);
			if(back[2]) FreeImage(&back[2]);
			if(back[3]) FreeImage(&back[3]);
			if(overlay) FreeImage(&overlay);

			sonicback[0] = LoadIMG("/rd/sonicback1.kmg.gz", 0);
			if(!sonicback[0])
				return;
			sonicback[1] = LoadIMG("/rd/sonicback2.kmg.gz", 0);
			if(!sonicback[1])
				return;
			sonicback[2] = LoadIMG("/rd/sonicback3.kmg.gz", 0);
			if(!sonicback[2])
				return;
			sonicback[3] = LoadIMG("/rd/sonicback4.kmg.gz", 0);
			if(!sonicback[3])
				return;
		
			back[1] = sonicback[0];
			if(!back[1])
				return;
			back[2] = LoadIMG("/rd/checkpos.kmg.gz", 1);
			if(!back[2])
				return;
			back[3] = LoadIMG("/rd/stripespos.kmg.gz", 1);
			if(!back[3])
				return;
			overlay = LoadIMG("/rd/sonicfloor.kmg.gz", 0);
			if(!overlay)
				return;
		
			if(vmode < HIGH_RES)
			{		
				back[0] = LoadIMG("/rd/donna.kmg.gz", 0);
				if(!back[0])
					return;
			}
			else
			{
				back[0] = LoadIMG("/rd/480/donna-480.kmg.gz", 0);
				if(!back[0])
					return;
		
				back[0]->scale = 0;
				for(i = 0; i < 4; i++)
					sonicback[i]->scale = 0;
				back[2]->scale = 0;
				back[3]->scale = 0;
				overlay->scale = 0;
		
				for(i = 0; i < 4; i++)
					sonicback[i]->y = (dH - 240)/2;
				overlay->y = (dH - 240)/2;
			}
			CalculateUV(0, 0, dW, dH, back[0]);
			oldvmode = vmode;
			reload = 0;
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

		if(text)
		{
			if(vmode < HIGH_RES)
				DrawStringB(10, 20, 0.80, 0.60, 0.60, msg);
			else
				DrawStringB(20, 40, 0.80, 0.60, 0.60, msg);
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
		
		if(refreshVMU)
		{
			if(d_shown)
				d_shown = 0;
			updateVMU(" Shadow  ", invert ? "  even " : "  odd  ", 1);
			refreshVMU = 0;
		}

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
					sprintf(msg, "Shadow on odd frames");
				else
					sprintf(msg, "Shadow on even frames");
				refreshVMU = 1;
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

		currentframe ++;
		if(currentframe > 10)
		{
			currentsonic++;
			if(currentsonic > 3)
				currentsonic = 0;
			currentframe = 0;
			back[1] = sonicback[currentsonic];
		}
		
		if(selback == 0)
		{
			int factor = 1;
			
			if(vmode >= HIGH_RES)
				factor = 2;
			
			if(x > factor*125 && y > factor*80 && x < factor*220 && y < factor*150)
			{
				if(!d_shown)
				{
					updateVMU_Donna();
					d_shown = 1;
				}
			}
			else
			{
				if(d_shown)
					refreshVMU = 1;
			}
		}
		else
		{
			if(d_shown)
				refreshVMU = 1;
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
	return;
}

void StripedSpriteTest()
{	
	int			done = 0, x = dW/2-50, y = dH/2, selback = 0, d_shown = 0;
	int			oldvmode = vmode, reload = 0;
	uint16		pressed, currentsonic = 0, i = 0, currentframe = 0;
	ImagePtr	back[4], striped, overlay, sonicback[4];
	controller	*st;

	reload = 1;

	for(i = 0; i < 4; i++)
		back[i] = NULL;
	for(i = 0; i < 4; i++)
		sonicback[i] = NULL;
	overlay = NULL;

	striped = LoadIMG("/rd/striped.kmg.gz", 0);
	if(!striped)
		return;

	while(!done && !EndProgram) 
	{
		if(reload || oldvmode != vmode)
		{
			if(back[0]) FreeImage(&back[0]);
			for(i = 0; i < 4; i++)
				if(sonicback[i]) FreeImage(&sonicback[i]);
			if(back[2]) FreeImage(&back[2]);
			if(back[3]) FreeImage(&back[3]);
			if(overlay) FreeImage(&overlay);

			sonicback[0] = LoadIMG("/rd/sonicback1.kmg.gz", 0);
			if(!sonicback[0])
				return;
			sonicback[1] = LoadIMG("/rd/sonicback2.kmg.gz", 0);
			if(!sonicback[1])
				return;
			sonicback[2] = LoadIMG("/rd/sonicback3.kmg.gz", 0);
			if(!sonicback[2])
				return;
			sonicback[3] = LoadIMG("/rd/sonicback4.kmg.gz", 0);
			if(!sonicback[3])
				return;
		
			if(vmode < HIGH_RES)
			{		
				back[0] = LoadIMG("/rd/donna.kmg.gz", 0);
				if(!back[0])
					return;
				back[1] = sonicback[0];
				if(!back[1])
					return;
				back[2] = LoadIMG("/rd/checkpos.kmg.gz", 1);
				if(!back[2])
					return;
				back[3] = LoadIMG("/rd/stripespos.kmg.gz", 1);
				if(!back[3])
					return;
				overlay = LoadIMG("/rd/sonicfloor.kmg.gz", 0);
				if(!overlay)
					return;
			}
			else
			{
				back[0] = LoadIMG("/rd/480/donna-480.kmg.gz", 0);
				if(!back[0])
					return;
				back[0]->scale = 0;
				back[1] = sonicback[0];
				if(!back[1])
					return;
				for(i = 0; i < 4; i++)
					sonicback[i]->scale = 0;
				back[2] = LoadIMG("/rd/checkpos.kmg.gz", 1);
				if(!back[2])
					return;
				back[2]->scale = 0;
				back[3] = LoadIMG("/rd/stripespos.kmg.gz", 1);
				if(!back[3])
					return;
				back[3]->scale = 0;
				overlay = LoadIMG("/rd/sonicfloor.kmg.gz", 0);
				if(!overlay)
					return;
		
				for(i = 0; i < 4; i++)
					sonicback[i]->y = (dH - 240)/2;
				overlay->y = (dH - 240)/2;
			}
			CalculateUV(0, 0, dW, dH, back[0]);
			oldvmode = vmode;
			reload = 0;
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
		
		if(refreshVMU)
		{
			if(d_shown)
				d_shown = 0;
			updateVMU(" Striped ", "", 1);
			refreshVMU = 0;
		}

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

		currentframe ++;
		if(currentframe > 10)
		{
			currentsonic++;
			if(currentsonic > 3)
				currentsonic = 0;
			currentframe = 0;
			back[1] = sonicback[currentsonic];
		}
		
				
		if(selback == 0)
		{
			int factor = 1;
			
			if(vmode >= HIGH_RES)
				factor = 2;
			
			if(x > factor*125 && y > factor*80 && x < factor*220 && y < factor*150)
			{
				if(!d_shown)
				{
					updateVMU_Donna();
					d_shown = 1;
				}
			}
			else
			{
				if(d_shown)
					refreshVMU = 1;
			}
		}
		else
		{
			if(d_shown)
				refreshVMU = 1;
		}
	}
	FreeImage(&back[0]);
	for(i = 0; i < 4; i++)
		FreeImage(&sonicback[i]);
	FreeImage(&back[2]);
	FreeImage(&back[3]);
	FreeImage(&overlay);
	FreeImage(&striped);
	return;
}

void ReflexNTimming()
{
	char			msg[60], vmuMsg[60];
	int				clicks[10], done = 0, view = 0, time_out_puru = 0;
	int				speed = 1, change = 1, oldvmode = -1;
	int				x, y, x2, y2, audio = 0, pos = 0;
	int				i = 0, vibrate = 1, vary = 0, variation = 1;
	uint16			pressed;		
	ImagePtr		back, spriteA, spriteB, fixed;
	sfxhnd_t		beep;
	maple_device_t	*purupuru = NULL;
	controller 		*st;

	back = LoadIMG("/rd/black.kmg.gz", 1);
	if(!back)
		return;
			
	fixed = LoadIMG("/rd/lag-per.kmg.gz", 0);
	if(!fixed)
		return;
	spriteA = LoadIMG("/rd/lag-per.kmg.gz", 0);
	if(!spriteA)
		return;
	spriteB = LoadIMG("/rd/lag-per.kmg.gz", 0);
	if(!spriteB)
		return;	

	// the wav file has delay as in audio sync
	beep = snd_sfx_load("/rd/beep_delayed.wav");
	if(beep == SFXHND_INVALID)
		return;
	x = 144;
	y = 60;
	x2 = 108;
	y2 = 96;
		
	fixed->x = 144;
	fixed->y = 96;	

	for(i = 0; i < 10; i++)
		clicks[i] = 0xFF;

	memset(vmuMsg, 0, sizeof(char)*60);
	
	while(!done && !EndProgram) 
	{
		if(oldvmode != vmode)
		{
			back->w = 320;
			back->h = 240;
			
			oldvmode = vmode;
		}
		
		spriteA->x = x;
		spriteA->y = y;
		spriteB->x = x2;
		spriteB->y = y2;
		
		StartScene();

		DrawImage(back);
		DrawImage(fixed);

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

		purupuru = maple_enum_type(0, MAPLE_FUNC_PURUPURU);
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
		DrawStringS(20, 170+fh, 0.0f, 1.0f, 0.0f, "Negative values mean you pressed \"A\" prematurely");
		DrawStringS(20, 170+2*fh, 0.0f, 1.0f, 0.0f, "\"X\" button toggles horizontal and vertical movement.");
		DrawStringS(20, 170+3*fh, 0.0f, 1.0f, 0.0f, "\"Y\" button toggles rhythmic timing.");
		DrawStringS(20, 170+4*fh, 0.0f, 1.0f, 0.0f, "\"R\" trigger toggles audio feedback.");
		if(purupuru)
			DrawStringS(20, 170+5*fh, 0.0f, 1.0f, 0.0f, "\"L\" trigger toggles vibration feedback.");

		EndScene();
		
		// we have to sync audio to the frame buffer
		// so we activate audio one frame later, up and down
		if((y == 97 && speed == 1) || 
			(y == 95 && speed == -1))
		{
			if(audio)
				snd_sfx_play(beep, 255, 128);

			purupuru = maple_enum_type(0, MAPLE_FUNC_PURUPURU);
			if(vibrate)
			{
				if(purupuru)
				{
					rumble_puru(purupuru, 1);
					time_out_puru = 2;
				}
			}
		}
		
		// we are two frames ahead for puru
		if((y == 98 && speed == 1) || 
			(y == 94 && speed == -1))
		{
			if(audio)
				snd_sfx_play(beep, 255, 128);

			purupuru = maple_enum_type(0, MAPLE_FUNC_PURUPURU);
			if(vibrate)
			{
				if(purupuru)
				{
					rumble_puru(purupuru, 1);
					time_out_puru = 4;
				}
			}
		}
		
		// One frame after stop the rumble
		if(time_out_puru)
		{
			time_out_puru --;
			if(!time_out_puru)
			{
				purupuru = maple_enum_type(0, MAPLE_FUNC_PURUPURU);
				if(purupuru)
					stop_puru(purupuru);
			}
		}
		
		if(y == 96)
		{
			spriteA->r = 1.0;
			spriteA->g = 0.0;
			spriteA->b = 0.0;

			spriteB->r = 1.0;
			spriteB->g = 0.0;
			spriteB->b = 0.0;
#ifdef BENCHMARK		
			back->r = 1.0;
			back->g = 1.0;
			back->b = 1.0;
#endif
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
#ifdef BENCHMARK				
				back->r = 0.0;
				back->g = 0.0;
				back->b = 0.0;
#endif
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
		
		VMURefresh("Reflex   ", vmuMsg);

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_A)
			{
				if(change)
				{
					clicks[pos] = (y - 96) *speed;
		
					sprintf(vmuMsg, " Off: %d", clicks[pos]);
					refreshVMU = 1;
		
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
			{
				vibrate = !vibrate;				
				if(!vibrate)
				{
					purupuru = maple_enum_type(0, MAPLE_FUNC_PURUPURU);
					if(purupuru)
						stop_puru(purupuru);
				}
				
			}
		
			if (pressed & CONT_Y)
			{
				variation = !variation;
				if(!variation)
					vary = 0;
			}

			if (pressed & CONT_B)
				done =	1;

			if (pressed & CONT_START)
				ShowMenu(TIMEREFLEX);
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
	
	purupuru = maple_enum_type(0, MAPLE_FUNC_PURUPURU);
	if(purupuru)
		stop_puru(purupuru);

	if(beep != SFXHND_INVALID)
		snd_sfx_unload(beep);
	FreeImage(&back);
	FreeImage(&fixed);
	FreeImage(&spriteA);
	FreeImage(&spriteB);	

	sprintf(vmuMsg, "-Results-");
	refreshVMU = 1;
	
	if(pos > 9)
	{
		int	total = 0;
		double	res = 0, ms = 0;
		ImagePtr wall;
		
		done = 0;
		wall = LoadIMG("/rd/back.kmg.gz", 0);
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
			ms = (double)(res*(1000.0/(IsPAL ? PAL_FRAME_LEN : NTSC_FRAME_LEN)));
			sprintf(msg, "%d/10 = %0.2f average frames ~= %0.2f ms", total, res, ms);
			DrawStringS(60, 110, 1.0f, 0, 0, "+");
			DrawStringS(55, 70 + fh*10, 1.0f, 0, 0, "_____");
			DrawStringS(60, 70 + fh*11, 1.0f, 1.0f, 1.0f, msg);
			
			DrawStringS(60, 70 + fh*12, 0.0f, 1.0f, 1.0f, "Keep in mind that this is not a lag test");
			if(IsPAL)
				DrawStringS(60, 70 + fh*13, 0.0f, 1.0f, 1.0f, "A frame is around 20 ms");
			else
				DrawStringS(60, 70 + fh*13, 0.0f, 1.0f, 1.0f, "A frame is around 16.68 ms");

			if(total && total < 5)
			{
				DrawStringS(100, 120, 0.0f, 1.0f, 0.0f, "EXCELLENT REFLEXES!");
				sprintf(vmuMsg, " AWESOME");
				refreshVMU = 1;
			}
			if(total == 0)
			{
				DrawStringS(100, 120, 0.0f, 1.0f, 0.0f, "INCREDIBLE REFLEXES!!");
				sprintf(vmuMsg, "-PERFECT-");
				refreshVMU = 1;
			}

			EndScene();
			VMURefresh("Reflex   ", vmuMsg);
			
			st = ReadController(0, &pressed);	
			if(st)
			{
				if (pressed & CONT_B)
					done =	1;
				if (pressed & CONT_START)
					ShowMenu(TIMEREFLEX);
			}
		}
		FreeImage(&wall);
	}
	return;
}

void ScrollTest()
{
	char		*vmuMsg = " H.Scroll";
	int 		done = 0, speed = 1, acc = 1, x = 0, y = 0, pause = 0;
	int 		oldvmode = -1, i, currentback = 0, frame = 0;
	int			vertical = 0;
	uint16		pressed;
	ImagePtr	back[4], overlay = NULL, kiki = NULL;
	fmenudata 	resmenudata[] = { {1, "Horizontal"}, {2, "Vertical"} };
	controller	*st;

	vertical = SelectMenu("Select Scroll", resmenudata, 2, vertical + 1);
	if(vertical == MENU_CANCEL)
		return;
	vertical -= 1;
	
	for(i = 0; i < 4; i++)
		back[i] = NULL;
		
	back[0] = LoadIMG("/rd/sonicback1.kmg.gz", 0);
	if(!back[0])
		return;
	back[1] = LoadIMG("/rd/sonicback2.kmg.gz", 0);
	if(!back[1])
		return;
	back[2] = LoadIMG("/rd/sonicback3.kmg.gz", 0);
	if(!back[2])
		return;
	back[3] = LoadIMG("/rd/sonicback4.kmg.gz", 0);
	if(!back[3])
		return;

	overlay = LoadIMG("/rd/sonicfloor.kmg.gz", 0);
	if(!overlay)
		return;
	
	kiki = LoadIMG("/rd/kiki.kmg.gz", 0);
	if(!kiki)
		return;

	while(!done && !EndProgram) 
	{
		if(oldvmode != vmode)
		{
			for(i = 0; i < 4; i++)
				back[i]->y = (dH - 240)/2;
			overlay->y = (dH - 240)/2;
			
			if(offsetY)  // center in PAL modes
			{
				for(i = 0; i < 4; i++)
					back[i]->y -= offsetY;
				overlay->y -= offsetY;
			}
			kiki->x = (dW - 320)/2;
			
			for(i = 0; i < 4; i++)
				IgnoreOffset(back[i]);
			IgnoreOffset(overlay);
			
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
			CalculateUV(x, 0, dW, 240, back[currentback]);
			CalculateUV(x*2, 0, dW, 240, overlay);
			DrawImage(back[currentback]);
			DrawImage(overlay);
		}
		else
		{
			if(y > kiki->th)
				y = 1;
			if(y < -1*kiki->th)
				y = -1;
			CalculateUV(0, y, kiki->tw, dH, kiki);
			DrawImage(kiki);
		}

		EndScene();
		VMURefresh(vmuMsg, "");
		
		frame ++;
		if(frame > 10)
		{
			currentback++;
			if(currentback > 3)
				currentback = 0;
			frame = 0;
		}

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

			if (pressed & CONT_Y)
			{
				int nvertical = 0;
				
				nvertical = SelectMenu("Select Scroll", resmenudata, 2, vertical + 1);
				if(nvertical != MENU_CANCEL)
					vertical = nvertical - 1;
				if(!vertical)
					vmuMsg = " H.Scroll";
				else
					vmuMsg = " V.Scroll";
				refreshVMU = 1;
			}

			if (pressed & CONT_START)
				ShowMenu(SCROLL);
		}
	}
	for(i = 0; i < 4; i++)
		FreeImage(&back[i]);
	FreeImage(&overlay);
	FreeImage(&kiki);
	return;
}

void GridScrollTest()
{
	int 			done = 0, speed = 1, acc = 1, x = 0, y = 0;
	int				angle = 0, pause = 0, direction = 0;
	uint16			pressed;		
	ImagePtr		back = NULL, square = NULL, diag = NULL;
	controller		*st = NULL;

	square = LoadIMG("/rd/small_grid.kmg.gz", 1);
	if(!square)
		return;
	diag = LoadIMG("/rd/diagonal.kmg.gz", 1);
	if(!diag)
		return; 
	
	square->x = -0.5*dW;
	square->y = -0.5*dH;
	back = square;
	
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
				
		CalculateUV(x, y, 2*dW, 2*dH, back);
		DrawImageRotate(back, angle);
		EndScene();
		
		VMURefresh("G. Scroll", "");
	
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
				
			if(pressed & CONT_RTRIGGER)
			{
				if(angle)
					angle = 0;
				else
				{
					angle = 45;
					back = square;
				}
			}
			
			if (pressed & CONT_LTRIGGER)
			{
				if(back == square)
				{
					back = diag;
					if(angle)
						angle = 0;
				}
				else
					back = square;
			}

			// Joystick
			if(st->joyx != 0)
				x += st->joyx/30;
		
			if(st->joyy != 0)
				y += st->joyy/30;

			if (pressed & CONT_START)
				ShowMenu(GRIDSCROLL);
		}

	}
	FreeImage(&square);  
	FreeImage(&diag); 
	return;
}

void DrawStripes()
{
	int 			done = 0, field = 1, alternate = 0,
					frame = 0, dframe = 0, vertical = 0;
	uint16			pressed;		
	ImagePtr		stripespos, stripesneg;
	ImagePtr		vstripespos, vstripesneg;
	controller		*st;
	char			*vmuMsg1 = "H.Stripes", *vmuMsg2 = "still";
	fmenudata 		resmenudata[] = { {1, "Horiz. Stripes"}, {2, "Vert. Stripes"} };

	stripespos = LoadIMG("/rd/stripespos.kmg.gz", 1);
	if(!stripespos)
		return;
	stripesneg = LoadIMG("/rd/stripesneg.kmg.gz", 1);
	if(!stripesneg)
		return;

	vstripespos = LoadIMG("/rd/vertstripespos.kmg.gz", 1);
	if(!vstripespos)
		return;
	vstripesneg = LoadIMG("/rd/vertstripesneg.kmg.gz", 1);
	if(!vstripesneg)
		return;

	VMURefresh("Stripes", "");
	vertical = SelectMenu("Select Mode", resmenudata, 2, vertical + 1) - 1;
	if(vertical == MENU_CANCEL)
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
			if(vmode < HIGH_RES)
				DrawStringB(20, 210, 1.0f, 1.0f, 1.0f, msg);
			else
				DrawStringB(20, 460, 1.0f, 1.0f, 1.0f, msg);
			frame ++;
			if(frame > (IsPAL ? 49 : 59))
				frame = 0;
		}
		EndScene();
		VMURefresh(vmuMsg1, vmuMsg2);

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;
						
			if (pressed & CONT_A)
			{
				alternate = !alternate;
				if(alternate)
					vmuMsg2 = "alternate";
				else
					vmuMsg2 = "  still  ";
				refreshVMU = 1;
			}
						
			if (pressed & CONT_Y)
			{
				int nvertical = 0;
				
				nvertical = SelectMenu("Select Mode", resmenudata, 2, vertical + 1);
				if(nvertical != MENU_CANCEL)
					vertical = nvertical - 1;
				if(vertical)
					vmuMsg1 = "V.Stripes";
				else
					vmuMsg1 = "H.Stripes";
				refreshVMU = 1;
			}
						
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
	char			*vmuMsg = "";
	int 			done = 0, field = 1, alternate = 0,
					frame = 0, dframe = 0;
	uint16			pressed;		
	ImagePtr		checkpos, checkneg;
	controller		*st;

	checkpos = LoadIMG("/rd/checkpos.kmg.gz", 1);
	if(!checkpos)
		return;
	checkneg = LoadIMG("/rd/checkneg.kmg.gz", 1);
	if(!checkneg)
	{
		FreeImage(&checkpos);
		return;
	}

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
			if(vmode < HIGH_RES)
				DrawStringB(20, 210, 1.0f, 1.0f, 1.0f, msg);
			else
				DrawStringB(20, 460, 1.0f, 1.0f, 1.0f, msg);
			frame ++;
			if(frame > (IsPAL ? 49 : 59))
				frame = 0;
		}
		EndScene();
		VMURefresh("CHKB PTTN", vmuMsg);

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;
						
			if (pressed & CONT_A)
			{
				alternate = !alternate;				
				if(alternate)
					vmuMsg = "alternate";
				else
					vmuMsg = " still ";
				refreshVMU = 1;
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

void LEDZoneTest()
{	
	float		x = 160, y = 120;
	int 		sprStep[] = { 1, 2, 4, 6, 8 };
	int			done = 0, fast = 0, selsprite = 1, show = 1, oldvmode = -1;
	uint16		pressed;
	ImagePtr	back, sprite[5];
	controller 	*st;

	back = LoadIMG("/rd/black.kmg.gz", 1);
	if(!back)
		return;
			
	sprite[0] = LoadIMG("/rd/sprite0led.kmg.gz", 0);
	if(!sprite[0])
		return;
	sprite[1] = LoadIMG("/rd/sprite1led.kmg.gz", 0);
	if(!sprite[1])
		return;
	sprite[2] = LoadIMG("/rd/sprite2led.kmg.gz", 0);
	if(!sprite[2])
		return;
	sprite[3] = LoadIMG("/rd/sprite3led.kmg.gz", 0);
	if(!sprite[3])
		return;
	sprite[4] = LoadIMG("/rd/sprite4led.kmg.gz", 0);
	if(!sprite[4])
		return;

	while(!done && !EndProgram) 
	{
		if(oldvmode != vmode)
		{
			back->w = dW;
			back->h = dH;
			oldvmode = vmode;
		}
	
		sprite[selsprite]->x = x - sprStep[selsprite]/2;
		sprite[selsprite]->y = y - sprStep[selsprite]/2;
	
		StartScene();
		DrawImage(back);		
	
		if(show)
			DrawImage(sprite[selsprite]);
		
		EndScene();
		VMURefresh("Backlit", "");

		st = ReadController(0, &pressed);
		if(st)
		{
			if (st->buttons & CONT_DPAD_UP)
			{
				if(!fast)
					y --;
				else
					y -= sprStep[selsprite];
			}
		
			if (st->buttons & CONT_DPAD_DOWN)
			{
				if(!fast)
					y ++;
				else
					y += sprStep[selsprite];
			}

			if (st->buttons & CONT_DPAD_LEFT)
			{
				if(!fast)
					x --;
				else
					x -= sprStep[selsprite];
			}

			if (st->buttons & CONT_DPAD_RIGHT)
			{
				if(!fast)
					x ++;
				else
					x += sprStep[selsprite];
			}
					
			// Joystick
			if(st->joyx != 0)
				x += st->joyx/40;
		
			if(st->joyy != 0)
				y += st->joyy/40;
		
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

			if (pressed & CONT_X)
				show = !show;
			
			if (pressed & CONT_A)
				fast = !fast;

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

void FixSpriteSize(ImagePtr sprite, int full)
{
	if(!sprite)
		return;
		
	if(!full)
	{
		if(vmode < HIGH_RES)
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
		if(vmode < HIGH_RES)
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
	CalculateUV(0, 0, sprite->w, sprite->h, sprite);
}

void DiagonalPatternTest()
{	
	int			done = 0, autorotate = 0, full = 0;
	int			oldvmode = -1;
	float		angle = 45.0, step = 1;
	uint16		pressed;
	ImagePtr	back, spritesmall, spritebig, sprite;
	char		buffer[32];

	back = LoadIMG("/rd/black.kmg.gz", 1);
	if(!back)
		return;
			
	spritesmall = LoadIMG("/rd/longrectangle.kmg.gz", 1);
	if(!spritesmall)
	{
		FreeImage(&back);
		return;
	}
	spritebig = LoadIMG("/rd/longrectanglefull.kmg.gz", 1);
	if(!spritebig)
	{
		FreeImage(&back);
		FreeImage(&spritesmall);
		return;
	}
	
	sprite = spritesmall;
	
	while(!done && !EndProgram) 
	{
		if(oldvmode != vmode)
		{
			back->w = dW;
			back->h = dH;
			
			if(vmode < HIGH_RES)
				sprite = spritesmall;
			else
				sprite = spritebig;
			oldvmode = vmode;
			FixSpriteSize(sprite, full);
		}

		StartScene();
		DrawImage(back);		

		sprintf(buffer, "Angle: %0.2f", (double)angle);
		DrawString(32, 32, 0.0, 1.0, 0.0, buffer);
		sprintf(buffer, "Angle increment: %s%d", step != 1 ? "1/" : "", (int)step);
		DrawString(32, 42, 0.0, 1.0, 0.0, buffer);
		
		DrawImageRotate(sprite, angle);
		EndScene();
		VMURefresh("Diagonal", "");

		ReadController(0, &pressed);
		if (pressed & CONT_B)
			done =	1;
						
		if(!autorotate)
		{
			if (pressed & CONT_LTRIGGER)
				angle += 1/step;
		
			if (pressed & CONT_RTRIGGER)
				angle -= 1/step;
		}

		if (pressed & CONT_A)
			autorotate = !autorotate;

		if (pressed & CONT_X)
			step -= 1;

		if (pressed & CONT_Y)
			step += 1;

		if(step > 20)
			step = 20;
		if(step < 1)
			step = 1;

		if (pressed & CONT_START)
			ShowMenu(DIAGONALHELP);
		
		if(autorotate)
			angle += 1/step;

		if(angle <= 0)
			angle += 360;
		if(angle >= 360)
			angle -= 360;
	}
	FreeImage(&back);
	FreeImage(&spritesmall);	
	FreeImage(&spritebig);	
	return;
}

void changeCircleColor(ImagePtr circle, int color, int toggle)
{
	if(toggle)
	{
		if(color)
		{
			circle->r = 1.0;
			circle->g = 0;
			circle->b = 0;
		}
		else
		{
			circle->r = 0.30;
			circle->g = 0.30;
			circle->b = 0.30;
		}
	}
	else
	{
		if(color)
		{
			circle->r = 0;
			circle->g = 0;
			circle->b = 1.0;
		}
		else
		{
			circle->r = 0.5;
			circle->g = 0.5;
			circle->b = 0.5;
		}
	}
}

void PassiveLagTest()
{
	uint16 		frames = 0, seconds = 0, minutes = 0, hours = 0;
	int			toggle = 0, framecnt = 1, done = 0;
	uint16		pressed, lsd, msd, pause = 0, color = 1;
	ImagePtr	back, circle, barl, barr;
	controller	*st;
	float		x = -8, y = 10;
#ifdef BENCHMARK
	uint64 		last, now;
	
	last = timer_us_gettime64();
#endif
	
	back = LoadIMG("/rd/white.kmg.gz", 0);
	if(!back)
		return;
	back->w = 320;
	back->h = 240;
		
	barl = LoadIMG("/rd/white.kmg.gz", 0);
	if(!barl)
		return;
	barr = LoadIMG("/rd/white.kmg.gz", 0);
	if(!barr)
		return;
	barr->x = 0;
	barl->w = 6;
	barl->h = 240;
	
	barr->x = 314;
	barr->w = 6;
	barr->h = 240;

	circle= LoadIMG("/rd/circle.kmg.gz", 0);
	if(!circle)
		return;
	circle->r = 0.0f;
	circle->g = 0.0f;
	circle->b = 1.0f;

	LoadNumbers();
	while(!done && !EndProgram) 
	{
		StartScene();
		DrawImage(back);
		DrawImage(barl);
		DrawImage(barr);
		DrawString(x+32,  y+8, 0, 0, 0, "hours");
		DrawString(x+104, y+8, 0, 0, 0, "minutes");
		DrawString(x+176, y+8, 0, 0, 0, "seconds");
		DrawString(x+248, y+8, 0, 0, 0, "frames");

		// Counter Separators
		DrawDigit(x+80,  y+16, 0, 0, 0, 10);
		DrawDigit(x+152, y+16, 0, 0, 0, 10);
		DrawDigit(x+224, y+16, 0, 0, 0, 10);

		// Circles 1st row
		circle->x = x+16;
		circle->y = y+56;
		changeCircleColor(circle, color, framecnt == 1);
		DrawImage(circle);
		DrawDigit(x+36, y+68, 1.0f, 1.0f, 1.0f, 1);

		circle->x = x+96;
		circle->y = y+56;
		changeCircleColor(circle, color, framecnt == 2);
		DrawImage(circle);
		DrawDigit(x+116, y+68, 1.0f, 1.0f, 1.0f, 2);

		circle->x = x+176;
		circle->y = y+56;
		changeCircleColor(circle, color, framecnt == 3);
		DrawImage(circle);
		DrawDigit(x+196, y+68, 1.0f, 1.0f, 1.0f, 3);

		circle->x = x+256;
		circle->y = y+56;
		changeCircleColor(circle, color, framecnt == 4);
		DrawImage(circle);
		DrawDigit(x+276, y+68, 1.0f, 1.0f, 1.0f, 4);

		// Circles 2nd row
		circle->x = x+16;
		circle->y = y+136;
		changeCircleColor(circle, color, framecnt == 5);
		DrawImage(circle);
		DrawDigit(x+36, y+148, 1.0f, 1.0f, 1.0f, 5);

		circle->x = x+96;
		circle->y = y+136;
		changeCircleColor(circle, color, framecnt == 6);
		DrawImage(circle);
		DrawDigit(x+116, y+148, 1.0f, 1.0f, 1.0f, 6);

		circle->x = x+176;
		circle->y = y+136;
		changeCircleColor(circle, color, framecnt == 7);
		DrawImage(circle);
		DrawDigit(x+196, y+148, 1.0f, 1.0f, 1.0f, 7);

		circle->x = x+256;
		circle->y = y+136;
		changeCircleColor(circle, color, framecnt == 8);
		DrawImage(circle);
		DrawDigit(x+276, y+148, 1.0f, 1.0f, 1.0f, 8);

		// Draw Hours
		lsd = hours % 10;
		msd = hours / 10;
		DrawDigit(x+32, y+16, 0, 0, 0, msd);
		DrawDigit(x+56, y+16, 0, 0, 0, lsd);

		// Draw Minutes
		lsd = minutes % 10;
		msd = minutes / 10;
		DrawDigit(x+104, y+16, 0, 0, 0, msd);
		DrawDigit(x+128, y+16, 0, 0, 0, lsd);

		// Draw Seconds
		lsd = seconds % 10;
		msd = seconds / 10;
		DrawDigit(x+176, y+16, 0, 0, 0, msd);
		DrawDigit(x+200, y+16, 0, 0, 0, lsd);

		// Draw Frames
		lsd = frames % 10;
		msd = frames / 10;
		if(color)
		{
			DrawDigit(x+248, y+16, toggle? 1.0 : 0, 0, toggle ? 0 : 1.0, msd);
			DrawDigit(x+272, y+16, toggle? 1.0 : 0, 0, toggle ? 0 : 1.0, lsd);
		}
		else
		{
			DrawDigit(x+248, y+16, 0, 0, 0, msd);
			DrawDigit(x+272, y+16, 0, 0, 0, lsd);
		}

		EndScene();
		
#ifdef BENCHMARK
		now = timer_us_gettime64();
		dbglog(DBG_INFO, "Frame took %g ms\n", (double)(now - last)/1000.0);
		last = now;
#endif
		VMURefresh("LAG TEST", !pause ? "running" : "paused");

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
			
			if (pressed & CONT_Y)
				color = !color;

			if (pressed & CONT_A)
			{
				pause = !pause;
				refreshVMU = 1;
			}

			if (pressed & CONT_START)
				ShowMenu(PASSIVELAG);
		}


		if(!pause)
		{
			frames ++;
			framecnt ++;
			if(framecnt > 8)
				framecnt = 1;
			toggle = !toggle;
		}

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
			barl->r = 1.0f;
			barl->g = 1.0f;
			barl->b = 1.0f;
			
			barr->r = 1.0f;
			barr->g = 1.0f;
			barr->b = 1.0f;
		}
	}
	FreeImage(&back);
	FreeImage(&barl);
	FreeImage(&barr);
	FreeImage(&circle);
	ReleaseNumbers();
}

uint16 ConvertToFrames(timecode *time)
{
	uint16	frames = 0;

	if(!time)
		return frames;

	frames = time->frames;
	frames += time->seconds*(IsPAL ? PAL_FRAME_RATE : NTSC_FRAME_RATE);
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
	value->seconds = Frames / (IsPAL ? PAL_FRAME_RATE : NTSC_FRAME_RATE);
	value->frames = Frames % (IsPAL ? 50 : 60);
}

double ConvertMStoFrames(double milliseconds)
{
	double frames;
	
	frames = milliseconds / (IsPAL ? PAL_FRAME_RATE : NTSC_FRAME_RATE);
	return frames;
}

void Alternate240p480i()
{
	char		*vmuMsg = NULL;
	int 		frames = 0, seconds = 0, minutes = 0, hours = 0, done = 0;
	int			current = 0, res = 0, status = 0, oldvmode = -1;
	int			originalvmode = -1, delayVMU = 0;
	timecode	times[20];
	uint16		pressed;		
	char 		buffer[20];
	controller	*st;
	ImagePtr	back;

	back = LoadIMG("/rd/black.kmg.gz", 1);
	if(!back)
		return;
	
	originalvmode = vmode;
			
	while(!done && !EndProgram) 
	{
		if(oldvmode != vmode)
		{
			if(!IsPAL)
			{
				vmuMsg = "240p/480i";
				if(vmode != VIDEO_240P)
					Toggle240p480i(res);
			}
			else
			{
				vmuMsg = "288p/576i";
				if(vmode != VIDEO_288P)
					Toggle240p480i(res);
			}
			oldvmode = vmode;
		}
		StartScene();

		DrawImage(back);

		DrawString(32, 8, 0, 1.0f, 0, "Current Resolution:");
		DrawString(140, 8, 0, 1.0f, 0, res == 0 ?
			(IsPAL ? "288" : "240p") : (IsPAL ? "576i" : "480i"));
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
					DrawString(32+12*5, 40+i*8, 1.0, 1.0, 0.0, times[i].res == 0 ?
						(IsPAL ? "288" : "240p") : (IsPAL ? "576i" : "480i"));
					DrawString(32+16*5, 40+i*8, 1.0, 1.0, 0.0, " at:");
					sprintf(buffer, "took %0.2f frames", ConvertMStoFrames(times[i].internal_latency));
					DrawString(200, 40+i*8, 1.0, 0.0, 0.0, buffer);
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
		
		if(delayVMU)
		{
			delayVMU--;
			if(!delayVMU)
				refreshVMU = 1;
		}
		
		VMURefresh(vmuMsg, "");

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_START)
			{
				ShowHelpWindow(ALTERNATE);
				refreshVMU = 1;
			}

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
					
					times[current - 1].internal_latency = Toggle240p480i(res);
					
					delayVMU = 4;
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
	
	if(originalvmode != vmode)
		ChangeResolution(originalvmode);
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
	uint16		pressed;
	ImagePtr	back = NULL, backcheck = NULL;
	ImagePtr	sd[5] = { NULL, NULL, NULL, NULL, NULL};
	ImagePtr	sd_b1[5] = { NULL, NULL, NULL, NULL, NULL};
	ImagePtr	sd_b2[5] = { NULL, NULL, NULL, NULL, NULL};
	controller	*st = NULL;
	
	back = LoadIMG("/rd/phase.kmg.gz", 0);
	if(!back)
		return;
	
	backcheck = LoadIMG("/rd/checkpos.kmg.gz", 1);
	if(!backcheck)
		return;

	for(i = 0; i < 5; i++)
	{
		sd[i] = LoadIMG("/rd/SD.kmg.gz", 0);
		if(!sd[i])
			return;
		sd[i]->x = 5+i*64;
		sd[i]->y = 70;
		sd_b1[i] = LoadIMG("/rd/SD_b1.kmg.gz", 0);
		if(sd_b1[i])
		{
			sd_b1[i]->x = sd[i]->x+16;
			sd_b1[i]->y = sd[i]->y+32;
		}
		
		sd_b2[i] = LoadIMG("/rd/SD_b2.kmg.gz", 0);
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
		
		VMURefresh("Phase &SR", !type ? "Normal": "Check");
		
		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_START )
				ShowMenu(PHASEHELP);

			if (pressed & CONT_X)
			{
				type = !type;
				refreshVMU = 1;
			}
			
			if (pressed & CONT_A)
				hpos = 0;
			
			if (pressed & CONT_DPAD_LEFT)
			{
				hpos --;
				if(hpos < -5)
					hpos = -5;
			}
			
			if (pressed & CONT_DPAD_RIGHT)
			{
				hpos ++;
				if(hpos > 5)
					hpos = 5;
			}
			
			if (pressed & CONT_B)
				done =	1;
		}

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
	uint16		pressed, lsd, msd, show = 1;		
	ImagePtr	back = NULL, sd = NULL;
	controller	*st;
	float		x = -8, y = 10;
#ifdef BENCHMARK
	uint64 		last, now;
	
	last = timer_us_gettime64();
#endif
	
	back = LoadIMG("/rd/white.kmg.gz", 0);
	if(!back)
		return;
	back->w = 320;
	back->h = 240;
	
	back->r = 0.0f;
	back->g = 0.0f;
	back->b = 0.0f;
	
	sd = LoadIMG("/rd/SD.kmg.gz", 0);
	if(!sd)
		return;
	sd->x = 128;
	sd->y = 85;
			
	LoadNumbers();
	while(!done && !EndProgram) 
	{
		StartScene();
		DrawImage(back);
		DrawString(x+32,  y+8, 1.0f, 1.0f, 1.0f, "hours");
		DrawString(x+104, y+8, 1.0f, 1.0f, 1.0f, "minutes");
		DrawString(x+176, y+8, 1.0f, 1.0f, 1.0f, "seconds");
		DrawString(x+248, y+8, 1.0f, 1.0f, 1.0f, "frames");

		// Counter Separators
		DrawDigit(x+80,  y+16, 1.0f, 1.0f, 1.0f, 10);
		DrawDigit(x+152, y+16, 1.0f, 1.0f, 1.0f, 10);
		DrawDigit(x+224, y+16, 1.0f, 1.0f, 1.0f, 10);

		// Draw Hours
		lsd = hours % 10;
		msd = hours / 10;
		DrawDigit(x+32, y+16, 1.0f, 1.0f, 1.0f, msd);
		DrawDigit(x+56, y+16, 1.0f, 1.0f, 1.0f, lsd);

		// Draw Minutes
		lsd = minutes % 10;
		msd = minutes / 10;
		DrawDigit(x+104, y+16, 1.0f, 1.0f, 1.0f, msd);
		DrawDigit(x+128, y+16, 1.0f, 1.0f, 1.0f, lsd);

		// Draw Seconds
		lsd = seconds % 10;
		msd = seconds / 10;
		DrawDigit(x+176, y+16, 1.0f, 1.0f, 1.0f, msd);
		DrawDigit(x+200, y+16, 1.0f, 1.0f, 1.0f, lsd);

		// Draw Frames
		lsd = frames % 10;
		msd = frames / 10;
		DrawDigit(x+248, y+16, 1.0f, 1.0f, 1.0f, msd);
		DrawDigit(x+272, y+16, 1.0f, 1.0f, 1.0f, lsd);
		
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
			{
				back->r = 0.0f;
				back->g = 0.0f;
				back->b = 0.0f;
			}
		}
		
#ifdef BENCHMARK
		now = timer_us_gettime64();
		dbglog(DBG_INFO, "Frame took %g ms\n", (double)(now - last)/1000.0);
		last = now;
#endif
		VMURefresh("DISAPP. ", "LOGO   ");

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;
			
			if (pressed & CONT_A)
				show = !show;
				
			if (pressed & CONT_X)
			{
				back->r = 1.0f;
				back->g = 1.0f;
				back->b = 1.0f;
				toggle = 2;
			}

			if (pressed & CONT_START)
				ShowMenu(DISAPHELP);
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