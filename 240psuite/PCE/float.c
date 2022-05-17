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
#include "float.h"
#include "font.h"
#include "video.h"
#include "graphics.h"

int type, ntype;
int fmx, fmy, tfmx, tfmy, lfm, fsel;
fmenudata resmenudata[6];

#ifdef SYSCARD1
#ifndef FLOAT_GFX
#define FLOAT_GFX
#incbin(float_map, "graphics/float.fmp");
#incchr_ex(float_bg, "graphics/float.pcx", 0, 0, 5, 3, 0);
#incpal(float_pal, "graphics/float.pcx", 0, 1);
#endif
#endif

#define CHANGE_VERT "Change vertical res under options"

#ifdef ALLOW_MDF_NOFILTER
int SetFMYesNo(char *title, char *bottom)
{
	resmenudata[0].id = FLOAT_NO;
	resmenudata[0].name = title;
	resmenudata[1].id = FLOAT_YES;
	resmenudata[1].name = "Yes";
	resmenudata[2].id = FLOAT_NO;
	resmenudata[2].name = "No";
	
	return(FloatMenu(1, 3, bottom));
}
#endif

void ChangeResType()
{
	if(type == RES_256)
		Set256H();
	else if(type == RES_320)
		Set320H();
	else if(type == RES_512)
		Set512H();
	else if(type == RES_352)
		Set352H();
}

int FloatMenuRes(int def)
{
	return(FloatMenuResExtra(def, NULL));
}

void SetFMRes(int pos, int id, char *res240, char *res224)
{
	resmenudata[pos].id = id;
	if(Enabled240p)
		resmenudata[pos].name = res240;
	else
		resmenudata[pos].name = res224;
}

void SetFMResTitle()
{
	resmenudata[0].id = FLOAT_CANCEL;
	resmenudata[0].name = "Display Mode";
}

void SetFMRes320(int pos)
{
	SetFMRes(pos, RES_320, "320x240", "320x224");
}

void SetFMRes256(int pos)
{
	SetFMRes(pos, RES_256, "256x240", "256x224");
}

void SetFMRes512(int pos)
{
	SetFMRes(pos, RES_512, "512x240", "512x224");
}

void SetFMRes352(int pos)
{
	SetFMRes(pos, RES_352, "352x240", "352x224");
}

int FloatMenuResExtra(int def, char *option)
{
	SetFMResTitle();
	
	SetFMRes320(1);
	SetFMRes256(2);
	SetFMRes512(3);
	SetFMRes352(4);
	
	if(option)
	{
		resmenudata[5].id = 5;
		resmenudata[5].name = option;
	}
	
	return(FloatMenu(def, option != NULL ? 6 : 5, CHANGE_VERT));
}

int FloatMenuRes320n256(int def)
{
	SetFMResTitle();
	
	SetFMRes320(1);
	SetFMRes256(2);
	
	return(FloatMenu(def, 3, CHANGE_VERT));
}

int FloatMenuRes320n256_224(int def)
{
	SetFMResTitle();
	
	SetFMRes320(1);
	
	resmenudata[2].id = RES_256;
	resmenudata[2].name = "256x224";

	
	return(FloatMenu(def, 3, CHANGE_VERT));
}

int FloatMenuResMinus352(int def)
{
	SetFMResTitle();
	
	SetFMRes320(1);
	SetFMRes256(2);
	SetFMRes512(3);
	
	return(FloatMenu(def, 4, NULL));
}

void RedrawFM(char *bottom)
{
	ResetVideo();
	setupFont();

	set_map_data(fs_map, 64, 32);
	set_tile_data(white_bg);
	load_tile(0x1000);
	load_map(0, 0, 0, 0, 64, 32);
	
	set_map_data(float_map, 16, 10);
	set_tile_data(float_bg);
	load_tile(0x1100);
	load_map(12, fmy, 0, 0, 16, 10);
	load_palette(0, float_pal, 1);  

	Center224in240();
	
	SetFontColors(13, RGB(2, 2, 2), RGB(6, 6, 6), 0);
	set_color_rgb(1, 0, 0, 0);

	// Title at index 0
	lfm = strlen(resmenudata[0].name);	
	tfmx = (16 - lfm)/2 + fmx;
	tfmy = fmy;

	set_font_pal(13);
	put_string(resmenudata[0].name, tfmx, tfmy);
	
	if(bottom)
	{
		lfm = strlen(bottom);	
		tfmx = (16 - lfm)/2 + fmx;
		tfmy = fmy+12;

		SetFontColors(12, 0, RGB(3, 3, 3), RGB(2, 2, 2));
		set_font_pal(12);
		put_string(bottom, tfmx, tfmy);
	}
}

int FloatMenu(int def, int size, char *bottom)
{
	fmx = 12;
	fmy = 10; 
	fsel = def;
	
	end = 0;
	redraw = 1;
	
	if(!Enabled240p)
		fmy = 9;

	while(!end)
	{
		vsync();
		
		if(redraw)
		{
			RedrawFM(bottom);
			
			redraw = 0;
			refresh = 1;
			disp_sync_on();
		}
			
		if(refresh)
		{	
			for(i = 1; i < size; i++)
			{	
				lfm = strlen(resmenudata[i].name);
				
				tfmx = (16 - lfm)/2 + fmx;
				tfmy = fmy+2+i+(6-size)/2;
				
				set_font_pal(fsel == i ? 15 : 14);
				put_string(resmenudata[i].name, tfmx, tfmy);
			}
			
			refresh = 0;
		}
		
		controller = joytrg(0);
		
		if (controller & JOY_UP) 
		{
			fsel --;
			if(fsel < 1)
				fsel = size - 1;
			refresh = 1;
		}

		if (controller & JOY_DOWN) 
		{
			fsel ++;
			if(fsel > size - 1)
				fsel = 1;
			refresh = 1;
		}
		
		if (controller & JOY_I || controller & JOY_RUN)
			end = 1;
			
		if (controller & JOY_II)
		{
			fsel = FLOAT_CANCEL;
			end = 1;
		}
	}
	
	end = 0;
	controller = 0;
	disp_off();
	return resmenudata[fsel].id;
}
