/* 
 * 240p Test Suite
 * Copyright (C)2014-2022 Artemio Urbina (PC Engine/TurboGrafx-16)
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
 
#include "huc.h"
#include "video.h"
#include "font.h"
#include "helpdata.h"


// Include them directly in order to reduce ovl and loading times.

#incchr(my_font, "graphics/font.pcx", 32, 3);
#incbin(fs_map, "graphics/fullscreen.fmp");
#incchr_ex(white_bg, "graphics/check.pcx", 8, 8, 1, 1, 0);

void CheckStart(int sync)
{
	vsync(sync);
	controller = joy(0);
	if(controller & JOY_RUN)
		runmdf = 1;
}

void DrawIntro()
{
	unsigned char frame;
	
	ResetVideo();
	setupFont();
	
	CheckStart(1);
	set_color(1, 0);
	SetFontColors(14, 0, 0, 0);
	put_string("KORDAMP PRESENTS", 12, 12);
	for(frame = 0; frame < 7; frame ++)
	{
		SetFontColors(14, 0, RGB(frame, frame, frame), 0);
		CheckStart(3);
	}
	
	frame = 5;
	while(frame)
		frame --;
	
	for(frame = 7; frame > 0; frame --)
	{
		SetFontColors(14, 0, RGB(frame, frame, frame), 0);
		CheckStart(3);
	}
}

void main()
{	
	xres_flags = XRES_SOFT;
	Enabled240p = 1;
	UseDefault = 0;
	EnabledSoft = 1;
	Enabled_C_BW = 0;
	
	DrawIntro();
	
	xres_flags_g = xres_flags;
	Enabled240p_g = Enabled240p;
	UseDefault_g = UseDefault;
	EnabledSoft_g = EnabledSoft;
	Enabled_C_BW_g = Enabled_C_BW;
	
	cls();
	vsync();
#ifndef SYSCARD1
	SetFontColors(14, 0, RGB(4, 4, 4), 0);
	put_string("Loading...", 28, 26);
#else
	x = cd_getver() >> 8;
	if(x == 3)
	{
		SetFontColors(14, 0, RGB(7, 7, 7), 0);
		put_string("There is a version of this software", 1, 10);
		put_string("for System Card 3 so you don't have", 1, 12);
		put_string("loading times", 10, 14);
		CheckStart(40);
	}
#endif
	if(runmdf)
	{
		x_g = 1;
		HelpItem = MDFOURIER_HELP;
	}
	cd_execoverlay(MAIN_OVERLAY); 
}


