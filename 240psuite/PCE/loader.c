/* 
 * 240p Test Suite
 * Copyright (C)2014 Artemio Urbina (PC Engine/TurboGrafx-16)
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
 
 
 This version of the suite is compiled with HuC from https://github.com/uli/huc
 
 */
#include "huc.h"
#include "video.h"
#include "font.h"

#incchr(my_font, "graphics/font.pcx", 32, 3);

void DrawIntro()
{
	unsigned char frame;
	
	ResetVideo();
	setupFont();
	
	vsync();
	set_color(1, 0);
	SetFontColors(14, 0, 0, 0);
	put_string("KORDAMP PRESENTS", 12, 12);
	for(frame = 0; frame < 7; frame ++)
	{
		SetFontColors(14, 0, RGB(frame, frame, frame), 0);
		vsync(3);
	}
	
	frame = 5;
	while(frame)
		frame --;
	
	for(frame = 7; frame > 0; frame --)
	{
		SetFontColors(14, 0, RGB(frame, frame, frame), 0);
		vsync(3);
	}
}

void main()
{	
	DrawIntro();
	
	cls();
	vsync();
	
	xres_flags = XRES_SOFT;
	Enabled240p = 1;
	UseDefault = 0;
	EnabledSoft = 1;
	Enabled_C_BW = 0;
	
#ifndef CDROM1
	SetFontColors(14, 0, RGB(4, 4, 4), 0);
	put_string("Loading...", 28, 26);
	cd_execoverlay(2); // Super CD-ROM2
#else
	cd_execoverlay(2); // CD-ROM2
#endif
}

