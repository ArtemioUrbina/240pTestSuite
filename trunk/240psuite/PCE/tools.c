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
#include "font.h"
#include "video.h"
#include "graphics.h"

#ifdef CDROM
extern int xres_flags;
extern int Enabled240p;
extern int UseDefault;
extern int EnabledSoft;
extern int Enabled_C_BW;
#endif
 
void DrawSP()
{
	int x = 210;
	int y = 84;
	int vram = 0x5000;
	int pos = 0;
	int row = 0;
	int count = 0;
	
	load_palette(16, SD_pal, 1);
	load_vram(0x5000, SD_sp, 0x700);

	for(row = 0; row < 7; row++)
	{
		for(pos = 0; pos < 4; pos++)
		{
			spr_make(count, pos*16+x, row*16+y, 0x40*count+vram, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_16x16, 0, 1);
			count ++;
		}
	}
}

// returns 1 if US system
char DetectTG16()
{
	char region;
	char *io = 0x1000;
	
	region = *io;
	
	if((region & 0x40) == 0)
		return 1;
	else
		return 0;
}

// returns 1 if CD detected
char DetectCDROM()
{
	char region;
	char *io = 0x1000;
	
	region = *io;
	if((region & 0x80) == 0)
		return 1;
	else
		return 0;
}

void DisplaySystemInfo()
{
	SetFontColors(13, RGB(2, 4, 7), RGB(7, 7, 7), RGB(1, 3, 7));
	set_font_pal(13);
	if(DetectTG16())
	{
		if(DetectCDROM())
			put_string("TG-16+CDROM", 26, 27);
		else
			put_string("TG-16", 32, 27);
	}
	else
	{
		if(DetectCDROM())
			put_string("PCE+CDROM2", 26, 27);
		else
			put_string("PCE", 34, 27);
	}
	if(Enabled240p)
	{
		if(UseDefault)
			put_string("240p", 33, 28);
		else
			put_string("Full 240p", 28, 28);
	}
	else
		put_string("224p", 33, 28);	
}