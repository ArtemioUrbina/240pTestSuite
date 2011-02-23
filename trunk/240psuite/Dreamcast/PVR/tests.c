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

#include "controller.h"
#include "tests.h"
#include "image.h"
#include "font.h"
#include "vmodes.h"
#include "vmu.h"

#include "help.h"


extern ImagePtr		scanlines;

static inline void DrawScanlines()
{
	if(vmode == FAKE_640_SL && scanlines)
		DrawImage(scanlines);
}

void DropShadowTest()
{
	char			msg[50];
	int				done = 0, x = 0, y = 0, invert = 0, frame = 0, text = 0, selback = 0, sprite = 0;
	uint16		oldbuttons, pressed;		
	ImagePtr	back[4], ssprite, shadow, buzz, buzzshadow;
	controller *st;

	oldbuttons = InitController(0);

	if(vmode != NATIVE_640_FS)
	{
		back[0] = LoadImage("/rd/motoko.png", 1);
		back[1] = LoadImage("/rd/sonicbg-240.png", 1);
		back[2] = LoadImage("/rd/checkpos.png", 1);
		back[3] = LoadImage("/rd/stripespos.png", 1);
	}
	else
	{
		back[0] = LoadImage("/rd/480/motoko-480.png", 1);
		back[0]->scale = 0;
		back[1] = LoadImage("/rd/480/sonicbg-480.png", 1);
		back[1]->scale = 0;
		back[2] = LoadImage("/rd/480/checkpos-480.png", 1);
		back[2]->scale = 0;
		back[3] = LoadImage("/rd/480/stripespos-480.png", 1);
		back[3]->scale = 0;
	}
		
	ssprite = LoadImage("/rd/shadow.png", 0);	
	buzz = LoadImage("/rd/buzzbomber.png", 0);
	buzzshadow = LoadImage("/rd/buzzbomberShadow.png", 0);
	
	srand((int)(time(0) ^ getpid()));
	sprite = rand() % 2;
	if(sprite == 0)
		shadow = ssprite;
	else
		shadow = buzzshadow;
	updateVMU(" Shadow	", " 	even	", 1);
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
				x --;
		
			if (st->buttons & CONT_DPAD_RIGHT)
				x ++;

			// Joystick
			if(st->joyx != 0)
				x += st->joyx/20;
		
			if(st->joyy != 0)
				y += st->joyy/20;
		
			if (pressed & CONT_START)
				done =	1;				

			if (pressed & CONT_X)		
			{
				invert = !invert; 												
				if(invert)
				{
					sprintf(msg, "Shadow on odd frames");
					updateVMU(" Shadow	", "		odd	", 1);
				}
				else
				{
					sprintf(msg, "Shadow on even frames");
					updateVMU(" Shadow	", " 	even	", 1);
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

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);
		DrawImage(back[selback]);

		if(text)
		{
			if(vmode != NATIVE_640_FS)
				DrawStringB(140, 12, 0, 1.0, 0, msg);
			else
				DrawStringB(450, 20, 0, 1.0, 0, msg);
			text --;
		}

		if(x < 0)
			x = 0;
		if(y < 0)
			y = 0;
		if(x > back[selback]->w - shadow->w)
			x = back[selback]->w - shadow->w;
		if(y > back[selback]->h - shadow->h)
			y = back[selback]->h - shadow->h;

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
	FreeImage(&ssprite);
	FreeImage(&buzz);
	FreeImage(&buzzshadow);
	return;
}

void StripedSpriteTest()
{	
	int		done = 0, x = 0, y = 0, selback = 0;
	uint16		oldbuttons = 0xffff, pressed;		
	ImagePtr	back[4], striped;
	controller *st;

	oldbuttons = InitController(0);
	if(vmode != NATIVE_640_FS)
	{
		back[0] = LoadImage("/rd/motoko.png", 1);
		back[1] = LoadImage("/rd/sonicbg-240.png", 1);
		back[2] = LoadImage("/rd/checkpos.png", 1);
		back[3] = LoadImage("/rd/stripespos.png", 1);
	}
	else
	{
		back[0] = LoadImage("/rd/480/motoko-480.png", 1);
		back[0]->scale = 0;
		back[1] = LoadImage("/rd/480/sonicbg-480.png", 1);
		back[1]->scale = 0;
		back[2] = LoadImage("/rd/480/checkpos-480.png", 1);
		back[2]->scale = 0;
		back[3] = LoadImage("/rd/480/stripespos-480.png", 1);
		back[3]->scale = 0;
	}
	striped = LoadImage("/rd/striped.png", 0);
		
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
				x += st->joyx/20;
		
			if(st->joyy != 0)
				y += st->joyy/20;
		
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

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);
		DrawImage(back[selback]);

		if(x < 0)
			x = 0;
		if(y < 0)
			y = 0;
		if(x > back[selback]->w - striped->w)
			x = back[selback]->w - striped->w;
		if(y > back[selback]->h - striped->h)
			y = back[selback]->h - striped->h;
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
	FreeImage(&striped);
	return;
}

void LagTest()
{
	char											msg[60];
	int												clicks[10], done = 0, view = 0, speed = 1, change = 1;
	int												x, y, x2, y2, audio = 1, pos = 0, i = 0;
	uint16										oldbuttons, pressed;		
	ImagePtr									back, spriteA, spriteB, spriteneg;
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

	updateVMU("Lag Test ", "", 1);
	back = LoadImage("/rd/lag-per.png", 0);
	spriteA = LoadImage("/rd/lag-per.png", 0);
	spriteB = LoadImage("/rd/lag-per.png", 0);
	spriteneg = LoadImage("/rd/lag-full.png", 0);

	beep = snd_sfx_load("/rd/beep.wav");
	x = 144;
	y = 60;
	x2 = 108;
	y2 = 96;
		
	back->x = 144;
	back->y = 96;
	spriteneg->x = 144;
	spriteneg->y = 96;

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
					
			if (pressed & CONT_A)
			{
				if(change)
				{
					clicks[pos] = (y - 96) *speed;
					change = 0;
		
					sprintf(msg, " Off: %d", clicks[pos]);
					updateVMU("Lag Test ", msg, 1);
		
					if(clicks[pos] >= 0)
						pos ++;
			
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
		
			if (pressed & CONT_START)
				done =	1;				
		}
		
		if(y > 132)
		{
			speed = -1;
			change = 1;
		}

		if(y < 60)
		{
			speed = 1;
			change = 1;
		}

		y += speed;
		x2 += speed;

		spriteA->x = x;
		spriteA->y = y;
		spriteB->x = x2;
		spriteB->y = y2;

		pvr_scene_begin();

		pvr_list_begin(PVR_LIST_TR_POLY);

		if(y == 96)
		{
			DrawImage(spriteneg);
			if(audio && beep != SFXHND_INVALID)
				snd_sfx_play(beep, 255, speed*255);	// toggle pan to 0 and 255, l & r

			purupuru = maple_enum_type(0, MAPLE_FUNC_PURUPURU);
			if(purupuru)
				purupuru_rumble(purupuru, &effect);
		}
		else
			DrawImage(back);

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
				sprintf(msg, "%2d", clicks[i]);

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

		DrawStringS(20, 190, 1.0f, 1.0f, 1.0f, "Press \"A\" when the sprite is aligned with the background.");
		DrawStringS(20, 190+fh, 1.0f, 1.0f, 1.0f, "Negative values mean you pressed \"A\" before they intersected");
		DrawStringS(20, 190+2*fh, 1.0f, 1.0f, 1.0f, "\"B\" button toggles horizontal and vertical movement.");

		DrawScanlines();
		pvr_list_finish();				

		pvr_scene_finish();
	}

	if(beep != SFXHND_INVALID)
		snd_sfx_unload(beep);
	FreeImage(&back);
	FreeImage(&spriteA);
	FreeImage(&spriteB);
	FreeImage(&spriteneg);
	snd_shutdown();

	updateVMU("Lag Test ", "-Results-", 1);
	if(pos > 9)
	{
		int	total = 0;
		double	res = 0, ms = 0;
		ImagePtr wall;
		
		done = 0;
		wall = LoadImage("/rd/back.png", 1);

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
			DrawStringS(60, 70 + fh*12, 0.0f, 1.0f, 1.0f, "Keep in mind that a frame is around 16.67 ms");

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
	int 				done = 0, speed = 1, acc = -1, x = 0, pause = 0;
	uint16			oldbuttons, pressed;		
	ImagePtr		back;
	controller	*st;

	oldbuttons = InitController(0);
	back = LoadImage("/rd/MSBG-1.png", 0);
	if(!back)
		return;
	
	back->y = (dH - 240)/2;

	updateVMU(" Scroll	", "", 1);
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
				done =	1;								

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
		{
			if(!(x <= (back->tw - W) && x >= 0))
				acc *= -1;

			x += speed * acc;
		}

		CalculateUV(x, 0, dW, 240, back);
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
	if(vmode != NATIVE_640_FS)
	{
		stripespos = LoadImage("/rd/stripespos.png", 1);
		stripesneg = LoadImage("/rd/stripesneg.png", 1);
	}
	else
	{
		stripespos = LoadImage("/rd/480/stripespos-480.png", 1);
		stripespos->scale = 0;
		stripesneg = LoadImage("/rd/480/stripesneg-480.png", 1);
		stripesneg->scale = 0;
	}
	vstripespos = LoadImage("/rd/vertstripespos.png", 1);
	vstripesneg = LoadImage("/rd/vertstripesneg.png", 1);
		
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
					updateVMU(" Stripes", "	still	", 1);
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
	if(vmode != NATIVE_640_FS)
	{
		checkpos = LoadImage("/rd/checkpos.png", 1);
		checkneg = LoadImage("/rd/checkneg.png", 1);
	}
	else
	{
		checkpos = LoadImage("/rd/480/checkpos-480.png", 1);
		checkneg = LoadImage("/rd/480/checkneg-480.png", 1);
	}
		
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
					updateVMU("CHKB PTTN", "	still	", 1);
			}
		
			if (pressed & CONT_X)
			{
				dframe = !dframe;
				frame = 0;
			}
						
			if (pressed & CONT_B && !alternate)
				field = !field;
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
	snd_init();
	back = LoadImage("/rd/back.png", 1);
	beep = snd_sfx_load("/rd/beep.wav");
		
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
			{
				if(HelpWindow(SOUNDHELP, NULL, 0))
					done =	1;								
			}

			if (pressed & CONT_A)
				play =	1; 						

			if (pressed & CONT_DPAD_LEFT)
				sel --;

			if (pressed & CONT_DPAD_RIGHT)
				sel ++;
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

		DrawStringS(120, 60, 1.0f, 1.0f, 1.0f, "Sound Test"); 
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

