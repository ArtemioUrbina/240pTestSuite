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
#include <stdio.h>

#include "image.h"
#include "font.h"
#include "vmodes.h"
#include "vmu.h"
#include "tests.h"

#include "controller.h"

#include "help.h"
#include "settings.h"
#include "vmodes.h"

struct settings_st settings = {
	0,
	0
};

void Settings(ImagePtr back)
{
	int 				done = 0, sel = 1;
	uint16			oldbuttons, pressed;		
	controller	*st;

	oldbuttons = InitController(0);

	updateVMU("Settings", "", 1);
	while(!done) 
	{
		int     x = 30, y = 52;
		float   r = 1.0f;
		float   g = 1.0f;
		float   b = 1.0f;
		int     c = 1;
		char		str[100];

		StartScene();

		DrawImage(back);

		DrawStringS(x, y, 0.0, 1.0, 0.0, "SETTINGS:"); y += 2*fh; 
		if(vcable != CT_VGA)
		{
			r = g = b = 0.8f;
			DrawStringS(x+5, y, r, g, b, "VGA Settings"); y += fh; c++;
			DrawStringS(x+5, y, r, g, b, "Draw Scanlines"); y += fh; c++;
			DrawStringS(x+5, y, r, g, b, "Scanline Intensity"); y += fh; c++;
		}
		else
		{
			DrawStringS(x+5, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, "VGA Settings"); y += fh;  c++;
			DrawStringS(x+5, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, "Draw Scanlines"); y += fh;  c++;
			sprintf(str, "Scanline Intensity: %0.0f%%", GetScanlineIntensity());
			DrawStringS(x+5, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, str); y += fh; c++;
		}

		sprintf(str, "Draw Video Border:   %s ", settings.drawborder == 1 ? "yes" : "no");
		DrawStringS(x+5, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, str); y += fh; c++;
		sprintf(str, "Draw PVR Background: %s ", settings.drawpvrbg == 1 ? "yes" : "no");
		DrawStringS(x+5, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, str); y += fh; c++;

		EndScene();

		st = ReadController(0);
		if(st)
		{
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
			
			if (pressed & CONT_START)
				done =	1;				

			if (pressed & CONT_DPAD_UP)
			{
				sel --;
				if(sel < 1)
					sel = c - 1;

				if(vcable != CT_VGA)
				{
					if(sel <= 3)
						sel = c - 1;
				}
			}

			if (pressed & CONT_DPAD_DOWN)
			{
				sel ++;
				if(sel > c - 1)
					sel = 1;
				if(vcable != CT_VGA)
				{
					if(sel <= 3)
						sel = 4;
				}
			}

			if (pressed & CONT_A)
			{
				switch(sel)
				{
					case 1:
						TestVideoMode();
						break;
					case 2:
						break;
					case 3:
						break;
					case 4:
						settings.drawborder = !settings.drawborder;
						break;
					case 5:
						settings.drawpvrbg = !settings.drawpvrbg;
						break;
				}
			}

			if (pressed & CONT_B)
			{
				switch(sel)
				{
					case 1:
						break;
					case 2:
						break;
					case 3:
						break;
					case 4:
						settings.drawborder = !settings.drawborder;
						break;
					case 5:
						settings.drawpvrbg = !settings.drawpvrbg;
						break;
				}
			}
		}
	}
}




