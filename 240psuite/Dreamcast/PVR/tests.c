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
  #include <complex.h>
  #include <fftw3.h>
#endif

#include "controller.h"
#include "tests.h"
#include "image.h"
#include "font.h"
#include "vmodes.h"
#include "vmu.h"

#include "help.h"
#include "settings.h"

void DropShadowTest()
{
	char			msg[50];
	int				done = 0, x = 0, y = 0, invert = 0, frame = 0, text = 0, selback = 0, sprite = 0;
	uint16		oldbuttons, pressed;		
	ImagePtr	back[4], ssprite, shadow, buzz, buzzshadow, overlay;
	controller *st;

	oldbuttons = InitController(0);

	back[1] = LoadKMG("/rd/sonicback.kmg.gz", 1);
	if(!back[1])
		return;
	back[2] = LoadKMG("/rd/checkpos.kmg.gz", 1);
	if(!back[2])
		return;
	back[3] = LoadKMG("/rd/stripespos.kmg.gz", 1);
	if(!back[3])
		return;
	overlay = LoadKMG("/rd/sonicfloor.kmg.gz", 1);
	if(!overlay)
		return;

	if(vmode != NATIVE_640_FS)
	{		
		back[0] = LoadKMG("/rd/motoko.kmg.gz", 1);
		if(!back[0])
			return;
	}
	else
	{
		back[0] = LoadKMG("/rd/480/motoko-480.kmg.gz", 1);
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
	updateVMU("  Shadow	", "  even  ", 1);
	while(!done) 
	{
		pvr_wait_ready();

		st = ReadController(0);
		if(st)
		{
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
					
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
		
			if (pressed & CONT_START)
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
		
			if (pressed & CONT_B)
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
		}

		if(x < back[selback]->x)
			x = back[selback]->x;
		if(y < back[selback]->y)
			y = back[selback]->y;
		if(x > back[selback]->w + back[selback]->x - shadow->w)
			x = back[selback]->w + back[selback]->x - shadow->w;
		if(y > back[selback]->h + back[selback]->y - shadow->h)
			y = back[selback]->h + back[selback]->y - shadow->h;

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);
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
			if(vmode != NATIVE_640_FS)
				DrawStringB(140, 12, 0, 1.0, 0, msg);
			else
				DrawStringB(450, 20, 0, 1.0, 0, msg);
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
		DrawScanlines();
		pvr_list_finish();				

		pvr_scene_finish();
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
	int		done = 0, x = 0, y = 0, selback = 0;
	uint16		oldbuttons = 0xffff, pressed;
	ImagePtr	back[4], striped, overlay;
	controller *st;

	oldbuttons = InitController(0);
	if(vmode != NATIVE_640_FS)
	{		
		back[0] = LoadKMG("/rd/motoko.kmg.gz", 1);
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
		overlay = LoadKMG("/rd/sonicfloor.kmg.gz", 1);
		if(!overlay)
			return;
	}
	else
	{
		back[0] = LoadKMG("/rd/480/motoko-480.kmg.gz", 1);
		if(!back[0])
			return;
		back[0]->scale = 0;
		back[1] = LoadKMG("/rd/sonicback.kmg.gz", 1);
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
		overlay = LoadKMG("/rd/sonicfloor.kmg.gz", 1);
		if(!overlay)
			return;

		back[1]->y = (dH - 240)/2;
  	overlay->y = (dH - 240)/2;
	}
	striped = LoadKMG("/rd/striped.kmg.gz", 0);
	if(!striped)
		return;
		
	updateVMU(" Striped ", "", 1);
	while(!done) 
	{
		pvr_wait_ready();

		st = ReadController(0);
		if(st)
		{
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
					
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
		
			if (pressed & CONT_START)
				done =	1;
						
			if (pressed & CONT_A)
			{
				if(selback > 0)
					selback --;
				else
					selback = 3;
			}
		
			if (pressed & CONT_B)
			{
				if(selback < 3)
					selback ++;
				else
					selback = 0;
			}
		}

		if(x < back[selback]->x)
			x = back[selback]->x;
		if(y < back[selback]->y)
			y = back[selback]->y;
		if(x > back[selback]->w + back[selback]->x - striped->w)
			x = back[selback]->w + back[selback]->x - striped->w;
		if(y > back[selback]->h + back[selback]->y - striped->h)
			y = back[selback]->h + back[selback]->y - striped->h;

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);
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

		DrawScanlines();
		pvr_list_finish();

		pvr_scene_finish();
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
	char											msg[60];
	int												clicks[10], done = 0, view = 0, speed = 1, change = 1;
	int												x, y, x2, y2, audio = 0, pos = 0, i = 0, vibrate = 0, vary = 0, variation = 1;
	uint16										oldbuttons, pressed, ltrig = 0, oldltrig = 0;		
	ImagePtr									back, spriteA, spriteB;
	sfxhnd_t									beep;
	maple_device_t						*purupuru = NULL;
	static purupuru_effect_t	effect;
	controller 								*st;

	oldbuttons = InitController(0);
	effect.duration = 1;
	effect.effect2 = PURUPURU_EFFECT2_UINTENSITY(1);
	effect.effect1 = PURUPURU_EFFECT1_INTENSITY(1);
	effect.special = PURUPURU_SPECIAL_MOTOR1;

	snd_init();

	srand((int)(time(0) ^ getpid()));
	updateVMU("Lag Test ", "", 1);
	back = LoadKMG("/rd/lag-per.kmg.gz", 0);
	if(!back)
		return;
	spriteA = CloneImage(back, 0);
	if(!spriteA)
		return;
	spriteB = CloneImage(back, 0);
	if(!spriteB)
		return;	

	beep = snd_sfx_load("/rd/beep.wav");
	x = 144;
	y = 60;
	x2 = 108;
	y2 = 96;
		
	back->x = 144;
	back->y = 96;	

	for(i = 0; i < 10; i++)
		clicks[i] = 0xFF;

	while(!done) 
	{
		pvr_wait_ready();

		st = ReadController(0);
		if(st)
		{
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
			ltrig = st->ltrig > 5 && oldltrig < 5;
			oldltrig = st->ltrig;
					
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

			if (pressed & CONT_B)
			{
				view ++;
				if(view > 2)
					view = 0;
			}
						
			if (pressed & CONT_Y)
				audio =	!audio;				
		
			if (pressed & CONT_X)
				vibrate =	!vibrate;				
		
			if(ltrig)
			{
				variation = !variation;
				if(!variation)
					vary = 0;
			}

			if (pressed & CONT_START)
				done =	1;				
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

		spriteA->x = x;
		spriteA->y = y;
		spriteB->x = x2;
		spriteB->y = y2;

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);

    DrawImage(back);

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
		DrawStringS(20, 170+2*fh, 0.0f, 1.0f, 0.0f, "\"B\" button toggles horizontal and vertical movement.");
		DrawStringS(20, 170+3*fh, 0.0f, 1.0f, 0.0f, "\"L\" trigger toggles rhythmic timing.");
		DrawStringS(20, 170+4*fh, 0.0f, 1.0f, 0.0f, "\"Y\" button toggles audio feedback.");
		if(purupuru)
			DrawStringS(20, 170+5*fh, 0.0f, 1.0f, 0.0f, "\"X\" button toggles vibration feedback.");

		DrawScanlines();

		pvr_list_finish();				

		pvr_scene_finish();
	}

	if(beep != SFXHND_INVALID)
		snd_sfx_unload(beep);
	FreeImage(&back);
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
		wall = LoadKMG("/rd/back.kmg.gz", 1);
		if(!wall)
			return;

		oldbuttons = InitController(0);
		while(!done)
		{
			pvr_wait_ready();

			st = ReadController(0);	
			if(st)
			{
				if (st->buttons & CONT_START)
					done =	1;
			}
	
		
			pvr_scene_begin();
			pvr_list_begin(PVR_LIST_TR_POLY);

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
			ms = (double)(res*(1000.0/60.0));
			sprintf(msg, "%d/10 = %0.2f average frames ~= %0.2f ms", total, res, ms);
			DrawStringS(60, 110, 1.0f, 0, 0, "+");
			DrawStringS(55, 70 + fh*10, 1.0f, 0, 0, "_____");
			DrawStringS(60, 70 + fh*11, 1.0f, 1.0f, 1.0f, msg);
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

			DrawScanlines();
			pvr_list_finish();				

			pvr_scene_finish();
		}
		FreeImage(&wall);
	}
	return;
}

void ScrollTest()
{
	int 				done = 0, speed = 1, acc = 1, x = 0, pause = 0;
	uint16			oldbuttons, pressed;		
	ImagePtr		back, overlay;
	controller	*st;

	oldbuttons = InitController(0);
	back = LoadKMG("/rd/sonicback.kmg.gz", 0);
	if(!back)
		return;
  overlay = LoadKMG("/rd/sonicfloor.kmg.gz", 0);
	if(!overlay)
		return;
	
	back->y = (dH - 240)/2;
  overlay->y = (dH - 240)/2;

	updateVMU(" Scroll  ", "", 1);
	while(!done) 
	{
		pvr_wait_ready();

		st = ReadController(0);
		if(st)
		{
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
					
			if (pressed & CONT_DPAD_UP)
				speed ++;

			if (pressed & CONT_DPAD_DOWN)
				speed --;

			if (pressed & CONT_START)
				done = 1;

			if (pressed & CONT_A)
				pause = !pause;

			if (pressed & CONT_B)
				acc *= -1;
		}

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);

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
		DrawScanlines();
		pvr_list_finish();				

		pvr_scene_finish();
	}
	FreeImage(&back);
  FreeImage(&overlay);
	return;
}

void GridScrollTest()
{
	int 				done = 0, speed = 1, acc = 1, x = 0, y = 0, pause = 0, direction = 0;
	uint16			oldbuttons, pressed;		
	ImagePtr		back;
	controller	*st;

	oldbuttons = InitController(0);
	back = LoadKMG("/rd/circles_grid.kmg.gz", 0);
	if(!back)
		return;  
	
	//back->y = (dH - 240)/2;  

	updateVMU("G. Scroll", "", 1);
	while(!done) 
	{
		pvr_wait_ready();

		st = ReadController(0);
		if(st)
		{
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
					
			if (pressed & CONT_DPAD_UP)
				speed ++;

			if (pressed & CONT_DPAD_DOWN)
				speed --;

			if (pressed & CONT_START)
				done = 1;

			if (pressed & CONT_A)
				pause = !pause;

			if (pressed & CONT_B)
				acc *= -1;

			if (pressed & CONT_X)
				direction = !direction;

			// Joystick
			if(st->joyx != 0)
				x += st->joyx/30;
		
			if(st->joyy != 0)
				y += st->joyy/30;
		}

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);

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
  
		DrawScanlines();
		pvr_list_finish();				

		pvr_scene_finish();
	}
	FreeImage(&back);  
	return;
}

void DrawStripes()
{
	int 				done = 0, field = 1, alternate = 0,
						frame = 0, dframe = 0, vertical = 0;
	uint16			oldbuttons, pressed;		
	ImagePtr		stripespos, stripesneg;
	ImagePtr		vstripespos, vstripesneg;
	controller	*st;

	oldbuttons = InitController(0);
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
	while(!done) 
	{
		pvr_wait_ready();

		st = ReadController(0);
		if(st)
		{
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
					
			if (pressed & CONT_START)
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
						
			if (pressed & CONT_B && !alternate)
				field = !field;

			if(st->rtrig > 5)
				oldbuttons = HelpWindow(STRIPESHELP, stripespos);
		}

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);

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
			if(vmode != NATIVE_640_FS)
				DrawStringB(20, 210, 1.0f, 1.0f, 1.0f, msg);
			else
				DrawStringB(20, 460, 1.0f, 1.0f, 1.0f, msg);
			frame ++;
			if(frame > 59)
				frame = 0;
		}

		DrawScanlines();
		pvr_list_finish();

		pvr_scene_finish();
	}

	FreeImage(&stripespos);
	FreeImage(&vstripesneg);
	FreeImage(&stripespos);
	FreeImage(&stripesneg);
	return;
}

void DrawCheckBoard()
{
	int 				done = 0, field = 1, alternate = 0,
						frame = 0, dframe = 0;
	uint16			oldbuttons, pressed;		
	ImagePtr		checkpos, checkneg;
	controller	*st;

	oldbuttons = InitController(0);
	checkpos = LoadKMG("/rd/checkpos.kmg.gz", 1);
	if(!checkpos)
		return;
	checkneg = LoadKMG("/rd/checkneg.kmg.gz", 1);
	if(!checkneg)
		return;

	updateVMU("CHKB PTTN", "", 1);
	while(!done) 
	{
		pvr_wait_ready();

		st = ReadController(0);
		if(st)
		{
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
					
			if (pressed & CONT_START)
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
						
			if (pressed & CONT_B && !alternate)
				field = !field;

			if(st->rtrig > 5)
				oldbuttons = HelpWindow(CHECKHELP, checkpos);
		}

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);

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
			if(vmode != NATIVE_640_FS)
				DrawStringB(20, 210, 1.0f, 1.0f, 1.0f, msg);
			else
				DrawStringB(20, 460, 1.0f, 1.0f, 1.0f, msg);
			frame ++;
			if(frame > 59)
				frame = 0;
		}

		DrawScanlines();
		pvr_list_finish();				

		pvr_scene_finish();
	}
	FreeImage(&checkpos);
	FreeImage(&checkneg);
	return;
}

void SoundTest()
{
	int 				done = 0, sel = 1, play = 0, pan = 0;
	uint16			oldbuttons, pressed;		
	ImagePtr		back;
	sfxhnd_t		beep;
	controller	*st;

	oldbuttons = InitController(0);
	
	back = LoadKMG("/rd/back.kmg.gz", 1);
	if(!back)
		return;
	beep = snd_sfx_load("/rd/beep.wav");
	if(!beep)
		return;
	
  snd_init();	
	updateVMU("Sound Test", "", 1);
	while(!done) 
	{
		pvr_wait_ready();

		st = ReadController(0);
		if(st)
		{
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
					
			if (pressed & CONT_START)
				done =	1;								

			if (pressed & CONT_A)
				play =	1; 						

			if (pressed & CONT_DPAD_LEFT)
				sel --;

			if (pressed & CONT_DPAD_RIGHT)
				sel ++;

			//if(st->rtrig > 5)
				//oldbuttons = HelpWindow(SOUNDHELP, NULL);
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

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);
		DrawImage(back);

		DrawStringS(130, 60, 1.0f, 1.0f, 1.0f, "Sound Test"); 
		DrawStringS(80, 120, 1.0f, sel == 0 ? 0 : 1.0f,	sel == 0 ? 0 : 1.0f, "Left Channel"); 
		DrawStringS(120, 130, 1.0f, sel == 1 ? 0 : 1.0f,	sel == 1 ? 0 : 1.0f, "Center Channel");
		DrawStringS(160, 120, 1.0f, sel == 2 ? 0 : 1.0f,	sel == 2 ? 0 : 1.0f, "Right Channel");
		DrawScanlines();
		
		pvr_list_finish();				

		pvr_scene_finish();
	}
	if(beep != SFXHND_INVALID)
		snd_sfx_unload(beep);
	FreeImage(&back);
	return;
}

void LEDZoneTest()
{	
	int		    done = 0, x = 0, y = 0, selsprite = 1, show = 1;
	uint16		oldbuttons = 0xffff, pressed;
	ImagePtr	back, sprite[5];
	controller *st;

	oldbuttons = InitController(0);
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
	while(!done) 
	{
		pvr_wait_ready();

		st = ReadController(0);
		if(st)
		{
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;				
			
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
		
			if (pressed & CONT_START)
				done =	1;
						
			if (pressed & CONT_A)
			{
				if(selsprite > 0)
					selsprite --;
				else
					selsprite = 4;
			}
		
			if (pressed & CONT_B)
			{
				if(selsprite < 4)
					selsprite ++;
				else
					selsprite = 0;
			}

      if (pressed & CONT_Y)
			  show = !show;

			if(st->rtrig > 5)
				oldbuttons = HelpWindow(BACKLITHELP, NULL);
		}
		
		if(x < 0)
			x = 0;
		if(y < 0)
			y = 0;
		if(x > back->w - 1)
			x = back->w - 1;
		if(y > back->h - 1)
			y = back->h - 1;

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);		
		DrawImage(back);		

    if(show)
    {
		  sprite[selsprite]->x = x;
   		sprite[selsprite]->y = y;
		  DrawImage(sprite[selsprite]);
    }

		DrawScanlines();
		pvr_list_finish();

		pvr_scene_finish();
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
	int frames = 0, seconds = 0, minutes = 0, hours = 0, framecnt = 1, done =  0;
	uint16			oldbuttons, pressed, lsd, msd, pause = 0;		
	ImagePtr		back, circle;
	controller	*st;

	oldbuttons = InitController(0);

	LoadNumbers();
	back = LoadKMG("/rd/white.kmg.gz", 1);
	if(!back)
		return;

	circle= LoadKMG("/rd/circle.kmg.gz", 0);
	if(!circle)
		return;
	circle->r = 0.0f;
	circle->g = 0.0f;
	circle->b = 1.0f;

	updateVMU("LAG TEST", "", 1);
	while(!done) 
	{
		pvr_wait_ready();

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

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);

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

		DrawScanlines();
		pvr_list_finish();				

		pvr_scene_finish();

		st = ReadController(0);
		if(st)
		{
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
					
			if (pressed & CONT_START)
				done =	1;				
						
			if (pressed & CONT_B && pause)
			{
				frames = hours = minutes = seconds = 0;
				framecnt = 1;
			}

			if (pressed & CONT_A)
				pause = !pause;
		}
	}
	FreeImage(&back);
	FreeImage(&circle);
	ReleaseNumbers();
}

void ResetVideoValues(vid_mode_t *vga_mode)
{
	if(!vga_mode)
		return;

	/*
	vga_mode->generic = DM_640x480;
	vga_mode->width = 640;
	vga_mode->height = 480;
	vga_mode->flags = VID_INTERLACE;
	vga_mode->cable_type = CT_VGA;
	vga_mode->pm = PM_RGB565;
	vga_mode->scanlines = 526;
	vga_mode->clocks = 855;		
	vga_mode->bitmapx = 156;	// X & Y in Framebuffer
	vga_mode->bitmapy = 36;
	vga_mode->scanint1 = 21;	// scanline interrupt positions
	vga_mode->scanint2 = 260;
	vga_mode->borderx1 = 115; // X Start 
	vga_mode->borderx2 = 835; // X Stop  
	vga_mode->bordery1 = 36;  // Y start 
	vga_mode->bordery2 = 516; // Y Stop 
	vga_mode->fb_curr = 0; 
	vga_mode->fb_count = 1; 
	vga_mode->fb_base[0] = 0; 
	vga_mode->fb_base[1] = 0; 
	vga_mode->fb_base[2] = 0; 
	*/

	vga_mode->generic = DM_640x480;
	vga_mode->width = 640;
	vga_mode->height = 480;
	vga_mode->flags = VID_INTERLACE;
	vga_mode->cable_type = CT_VGA;
	vga_mode->pm = PM_RGB565;
	vga_mode->scanlines = 526;
	vga_mode->clocks = 855;		
	vga_mode->bitmapx = 175;	// X & Y in Framebuffer
	vga_mode->bitmapy = 36;
	vga_mode->scanint1 = 21;	// scanline interrupt positions
	vga_mode->scanint2 = 260;
	vga_mode->borderx1 = 135; // X Start 
	vga_mode->borderx2 = 855; // X Stop  
	vga_mode->bordery1 = 36;  // Y start 
	vga_mode->bordery2 = 516; // Y Stop 
	vga_mode->fb_curr = 0; 
	vga_mode->fb_count = 1; 
	vga_mode->fb_base[0] = 0; 
	vga_mode->fb_base[1] = 0; 
	vga_mode->fb_base[2] = 0; 
	vga_mode->fb_base[3] = 0; 
}

vid_mode_t  vga_mode;
int					initvga = 1;

void TestVideoMode()
{
	int 				done =  0;
	uint16			oldbuttons, pressed, update = 0, load = 0, sel = 1, showback = 1;		
	controller	*st;
	char				str[50];
	ImagePtr		back;

	if(vcable != CT_VGA)
		return;

	if(initvga)
	{
		ResetVideoValues(&vga_mode);
		initvga = 0;
	}

	back = LoadKMG("/rd/480/grid-480.kmg.gz", 1);
	if(!back)
		return;
	back->scale = 0;

	oldbuttons = InitController(0);

	updateVMU("VIDEO", "", 1);

	vid_set_mode_ex(&vga_mode);

	while(!done) 
	{
		float   r = 1.0f;
		float   g = 1.0f;
		float   b = 1.0f;
		int     c = 1;
		float   x = 40.0f;
		float   y = 55.0f;

		if(load)
		{
			LoadFont();
			LoadScanlines();
			load = 0;
		}

		pvr_wait_ready();
		pvr_scene_begin();
		pvr_list_begin(PVR_LIST_TR_POLY);

		if(showback)
			DrawImage(back);

		sprintf(str, " Width:     %d ", vga_mode.width);
		DrawStringB(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, str); y += fh; c++;
		sprintf(str, " Height:    %d ", vga_mode.height);
		DrawStringB(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, str); y += fh; c++;

		DrawStringB(x, y, 0, 0,  0 , "                "); y+= fh;

		sprintf(str, " Scanlines: %d ", vga_mode.scanlines);
		DrawStringB(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, str); y += fh; c++;
		sprintf(str, " Clocks:    %d ", vga_mode.clocks);
		DrawStringB(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, str); y += fh; c++;

		DrawStringB(x, y, 0, 0,  0 , "                "); y+= fh;

		sprintf(str, " Bitmap X:  %d ", vga_mode.bitmapx);
		DrawStringB(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, str); y += fh; c++;
		sprintf(str, " Bitmap Y:  %d ", vga_mode.bitmapy);
		DrawStringB(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, str); y += fh; c++;

		DrawStringB(x, y, 0, 0,  0 , "                "); y+= fh;

		sprintf(str, " Border X1: %d ", vga_mode.borderx1);
		DrawStringB(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, str); y += fh; c++;
		sprintf(str, " Border X2: %d ", vga_mode.borderx2);
		DrawStringB(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, str); y += fh; c++;
		sprintf(str, " Border Y1: %d ", vga_mode.bordery1);
		DrawStringB(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, str); y += fh; c++;
		sprintf(str, " Border Y2: %d ", vga_mode.bordery2);
		DrawStringB(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, str); y += fh; c++;

		DrawStringB(x, y, 0, 0,  0 , "                "); y+= fh;

		sprintf(str, " Draw Video Border:   %s ", settings.drawborder == 1 ? "yes" : " no");
		DrawStringB(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, str); y += fh; c++;
		sprintf(str, " Draw PVR Background: %s ", settings.drawpvrbg == 1 ? "yes" : " no");
		DrawStringB(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, str); y += fh; c++;

		sprintf(str, " Video Border Size:               %dx%d ", vga_mode.borderx2-vga_mode.borderx1, vga_mode.bordery2-vga_mode.bordery1);
		DrawStringB(x, y, 0, 1.0f, 0.0f, str); y += fh; 

		sprintf(str, " Active Area Margin Left:         %d", vga_mode.bitmapx - vga_mode.borderx1);
		DrawStringB(x, y, 0, 1.0f, 0.0f, str); y += fh; 
		sprintf(str, " Active Area Margin Right:        %d", vga_mode.clocks - vga_mode.bitmapx - vga_mode.width);
		DrawStringB(x, y, 0, 1.0f, 0.0f, str); y += fh; 
		sprintf(str, " Active Area Margin Top:          %d", vga_mode.bitmapy);
		DrawStringB(x, y, 0, 1.0f, 0.0f, str); y += fh; 
		sprintf(str, " Active Area Margin Bottom:       %d", vga_mode.scanlines - vga_mode.bitmapy - vga_mode.height);
		DrawStringB(x, y, 0, 1.0f, 0.0f, str); y += fh; 

		DrawScanlines();

		pvr_list_finish();				
		pvr_scene_finish();

		if(update)
		{
			ReleaseScanlines();
			ReleaseFont();

			vid_set_mode_ex(&vga_mode);

			if(settings.drawborder)
				vid_border_color(255, 255, 255);
			else
				vid_border_color(0, 0, 0);

			if(settings.drawpvrbg)
				pvr_set_bg_color(0.0f, 1.0f, 0.0f);
			else
				pvr_set_bg_color(0.0f, 0.0f, 0.0f);

			update = 0;
			load = 1;
		}

		st = ReadController(0);
		if(st)
		{
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
					
			if (pressed & CONT_START)
				done =	1;				

			if (pressed & CONT_X)
			{
				ResetVideoValues(&vga_mode);
				update = 1;
			}
						
			if (pressed & CONT_Y)
				update = 1;

			if (pressed & CONT_A)
				showback = !showback;

			if (pressed & CONT_DPAD_UP)
			{
				sel --;
				if(sel < 1)
					sel = c - 1;
			}

			if (pressed & CONT_DPAD_DOWN)
			{
				sel ++;
				if(sel > c - 1)
					sel = 1;
			}

			if (pressed & CONT_DPAD_LEFT)
			{
				switch(sel)
				{
					case 1:
						if (pressed & CONT_B)
							vga_mode.width --;
						break;
					case 2:
						if (pressed & CONT_B)
							vga_mode.height --;
						break;
					case 3:
						vga_mode.scanlines --;
						break;
					case 4:
						vga_mode.clocks --;
						break;
					case 5:
						vga_mode.bitmapx --;
						break;
					case 6:
						vga_mode.bitmapy --;
						break;
					case 7:
						vga_mode.borderx1 --;
						break;
					case 8:
						vga_mode.borderx2 --;
						break;
					case 9:
						vga_mode.bordery1 --;
						break;
					case 10:
						vga_mode.bordery2 --;
						break;
					case 11:
						settings.drawborder = !settings.drawborder;
						break;
					case 12:
						settings.drawpvrbg = !settings.drawpvrbg;
						break;
				}
			}

			if (pressed & CONT_DPAD_RIGHT)
			{
				switch(sel)
				{
					case 1:
						if (pressed & CONT_B)
							vga_mode.width ++;
						break;
					case 2:
						if (pressed & CONT_B)
							vga_mode.height ++;
						break;
					case 3:
						vga_mode.scanlines ++;
						break;
					case 4:
						vga_mode.clocks ++;
						break;
					case 5:
						vga_mode.bitmapx ++;
						break;
					case 6:
						vga_mode.bitmapy ++;
						break;
					case 7:
						vga_mode.borderx1 ++;
						break;
					case 8:
						vga_mode.borderx2 ++;
						break;
					case 9:
						vga_mode.bordery1 ++;
						break;
					case 10:
						vga_mode.bordery2 ++;
						break;
					case 11:
						settings.drawborder = !settings.drawborder;
						break;
					case 12:
						settings.drawpvrbg = !settings.drawpvrbg;
						break;
				}
			}
		}
	}

	FreeImage(&back);

	sprintf(str, " Width:     %d\n", vga_mode.width);
	dbglog(DBG_KDEBUG, str);
	sprintf(str, " Height:    %d\n", vga_mode.height);
	dbglog(DBG_KDEBUG, str);

	sprintf(str, " Scanlines: %d\n", vga_mode.scanlines);
	dbglog(DBG_KDEBUG, str);
	sprintf(str, " Clocks:    %d\n", vga_mode.clocks);
	dbglog(DBG_KDEBUG, str);

	sprintf(str, " Bitmap X:  %d\n", vga_mode.bitmapx);
	dbglog(DBG_KDEBUG, str);
	sprintf(str, " Bitmap Y:  %d\n", vga_mode.bitmapy);
	dbglog(DBG_KDEBUG, str);

	sprintf(str, " Border X1: %d\n", vga_mode.borderx1);
	dbglog(DBG_KDEBUG, str);
	sprintf(str, " Border X2: %d\n", vga_mode.borderx2);
	dbglog(DBG_KDEBUG, str);
	sprintf(str, " Border Y1: %d\n", vga_mode.bordery1);
	dbglog(DBG_KDEBUG, str);
	sprintf(str, " Border Y2: %d\n", vga_mode.bordery2);
	dbglog(DBG_KDEBUG, str);
}

#ifdef USE_FFTW

void SIPLagTest()
{
  int 				    done = 0, status = 0, counter = 0;
	uint16			    oldbuttons, pressed;		
	ImagePtr		    back;
	sfxhnd_t		    beep;  
	controller	    *st;
	maple_device_t  *sip = NULL;  
  size_t          size = 0;
  uint8           *buffer = NULL;

	oldbuttons = InitController(0);
	
	back = LoadKMG("/rd/back.kmg.gz", 1);
	if(!back)
		return;
	beep = snd_sfx_load("/rd/Sample.wav");
	if(!beep)
		return;

  snd_init();		
	updateVMU("Lag v/Micr", "", 1);
	while(!done) 
	{
		pvr_wait_ready();

    if(status == 0) // no input if we are sampling
    {
		  st = ReadController(0);
		  if(st)
		  {
			  pressed = st->buttons & ~oldbuttons;
			  oldbuttons = st->buttons;					      
       
        if (pressed & CONT_START)
				  done =	1;				
  
        if (pressed & CONT_A)
  			  status = 1;
		  }
    }

    if(status == 1 && sip)
    {
      printf("Status 1: Setting up mic and recording\n");
      sip_set_gain(sip, SIP_MAX_GAIN); // SIP_DEFAULT_GAIN  
      sip_set_sample_type(sip, SIP_SAMPLE_16BIT_SIGNED); 
      sip_set_frequency(sip, SIP_SAMPLE_11KHZ); // 11.025kHz
      
      if(sip_start_sampling(sip, 1) == MAPLE_EOK)
      {
        counter = 5; // wait 5 frames
        status = 2;
      }
      else
      {
        printf("Start Recording: Failed\n");
        status = 0;
      }
    }

    if(status == 2 || status == 4)
    {
      printf("Status %d: Waiting %d\n", status, counter);
      counter --;
      if(!counter)
        status++;
    }
    
		if(status == 3 && beep != SFXHND_INVALID)
		{
      printf("Status 3: Starting playback\n");
			snd_sfx_play(beep, 255, 128);
			status = 4;
      counter = 5*60; // record 5 seconds
		}    

    if(status == 5 && sip)
    {
      printf("Status 5: Stopping sampling\n");
      if(sip_stop_sampling(sip, 1) == MAPLE_EOK)
      {
        buffer = sip_get_samples(sip, &size);
        printf("Got %d bytes\n", (int)size);
        if(buffer && size)
        {
          FILE *fp = fopen("/pc/samples.raw", "wb");
          printf("Creating samples.raw file\n");
          if(fp)
          {
            fwrite(buffer, 1, size, fp);
            fclose(fp);
            printf("Wrote %d bytes to file\n", size);
  
            thd_sleep(1000);
          }          

          ProcessSamples(buffer);

          free(buffer);
          buffer = NULL;
          size = 0;
        }        
      }
      else
        printf("Stop sampling failed: aborting\n");        

      sip_clear_samples(sip);
      status = 0;
    }   

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);
		DrawImage(back);

		DrawStringS(130, 60, 1.0f, 1.0f, 1.0f, "Lag Test via Microphone"); 
		
		DrawStringS(160, 120, 1.0f, 1.0f,	1.0f, "Status goes here");
		DrawScanlines();
		
		pvr_list_finish();				

		pvr_scene_finish();

	  sip = maple_enum_type(0, MAPLE_FUNC_MICROPHONE);
    if(!sip)
      done = 1;
	}

	if(beep != SFXHND_INVALID)
		snd_sfx_unload(beep);
	FreeImage(&back);
	return;
}

void ProcessSamples(uint8 *samples, size_t size)
{
  long          samplesize = 0;
  unsigned long samplerate = 0;
  long          i = 0, f = 0, framesizernd = 0, time = 0;
  double        *in, root = 0, framesize = 0;  
  double        *MaxFreqArray = NULL;
  //short       *samples = NULL;
  fftw_complex  *out;
  fftw_plan     p;
  uint64        start, end;
  
  samplesize = (long) size;
  samplerate = 11025; 

  framesize = samplerate/60.0;
  framesizernd = (long)framesize;
  samplesize /= 2; // Make a 16 bit array

  printf("Samples are at %u Khz and %g seconds long. A Frame is %g samples.\n", samplerate, (double)samplesize/samplerate, framesize);    

  start = timer_ms_gettime64();
  in = (double*) fftw_malloc(sizeof(double) * framesizernd);
  out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (framesizernd/2+1));  
    
  long arraysize = framesizernd;
  MaxFreqArray = malloc(sizeof(double)*(long)(samplesize/framesize));
  for(f = 0; f < samplesize/framesize - 1; f++)
  {
    double mainfreq = 0, max = 0;
    long   maxind = -1;
    long   framestart = 0;

    framestart = framesize*f;
    for(i = 0; i < framesizernd; i++)
      in[i] = (double)samples[i+framestart];

    //printf("Estimating plan\n");
    p = fftw_plan_dft_r2c_1d(arraysize, in, out, FFTW_ESTIMATE);      

    //printf("Executing FFTW\n");
    fftw_execute(p); 
  
    //printf("Analyzing results\n");
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
    
    mainfreq = (double)((double)maxind/((double)arraysize/(double)samplerate));
    MaxFreqArray[f] = mainfreq;
    
    //printf("Frame %ld: Main frequency %g Hz\n", f, mainfreq, less, plus);
  }

  end = timer_ms_gettime64();
  time = end - start;
  printf("FFT for %ld frames took %ld\n", (long)samplesize/framesize, time);
  start = end;

  // 60 hz array boxes. since 60hz sample chunks
  {
    long pos = 0, count = 0;    

    for(f = 0; f < samplesize/framesize; f++)
    {
      if(count)
      {
        if(MaxFreqArray[pos] != MaxFreqArray[f])
        {
          count = 0;          
          pos = 0;
        }
        else
        {
          count++;
          if(count == 60)
          {
            printf("Found at %d frames -> %g sec (we waited 5 frames to start playback)\n", pos, pos/60.0);
            pos = 0;
            count = 0;
          }

          // this is just for debugging
          if(count > 50)
            printf("Found (%d) at %d frames -> %g sec (we waited 5 frames to start playback)\n", count, pos, pos/60.0);
        }
      }
  
      // 930 and 1030 since our sampling is 11025 and we have 60 hz "boxes", our error is also 60
      if(!count && MaxFreqArray[f] > 970 && MaxFreqArray[f] < 1030)        
      {
        pos = f;        
        count = 1;
      }      
    }
  }

  end = timer_ms_gettime64();
  time = end - start;
  printf("Processing frequencies took %ld\n", time);

  free(MaxFreqArray);
  MaxFreqArray = NULL;  
  
  fftw_destroy_plan(p);
  fftw_free(in); 
  fftw_free(out);
  fftw_cleanup();  
}

#endif
