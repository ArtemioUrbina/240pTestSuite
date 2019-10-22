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
#include "float.h"
#include "font.h"
#include "video.h"
#include "graphics.h"

int type;

#ifdef CDROM1
#ifndef FLOAT_GFX
#define FLOAT_GFX
#incbin(float_map, "graphics/float.fmp");
#incchr_ex(float_bg, "graphics/float.pcx", 0, 0, 5, 3, 0);
#incpal(float_pal, "graphics/float.pcx", 0, 1);
#endif
#endif


int DrawFloatMenuRes(int def)
{
	/*
	if(!enable_256)
		return RES_320;
	*/
		
	return(DrawFloatMenuResExtra(def, NULL));
}

int DrawFloatMenuResExtra(int def, char *option)
{
	fmenudata resmenudata[6];

	resmenudata[0].id = 0;
	resmenudata[0].name = "Display Mode";
	resmenudata[1].id = RES_320;
	resmenudata[1].name = "320x240";
	resmenudata[2].id = RES_256;
	resmenudata[2].name = "256x240";
	resmenudata[3].id = RES_512;
	resmenudata[3].name = "512x240";
	resmenudata[4].id = RES_352;
	resmenudata[4].name = "352x240";
	resmenudata[5].id = 5;
	resmenudata[5].name = NULL;
	
/*
	if(!enable_256 && option)
		return 3;
		*/
		
	if(option)
		resmenudata[5].name = option;
	
	return(DrawFloatMenu(def, resmenudata, option != NULL ? 6 : 5));
}

int DrawFloatMenu(int def, fmenudata *data, int size)
{
	int	sel = 1;
	int len = 0, ix, iy;
	
	x = 100;
	y = 72; 
	
	end = 0;
	sel = def;
	redraw = 1;
	
	/*
	x = (VDP_getScreenWidth() - 128)/ 2;
	y = (VDP_getScreenHeight() - 80)/ 2;
	*/
	
	while(!end)
	{
		vsync();
		
		if(redraw)
		{
			ResetVideo();
			setupFont();
	
			set_map_data(float_map, 16, 10);
			set_tile_data(float_bg);
			load_tile(0x1000);
			load_map(12, 9, 0, 0, 16, 10);
			load_palette(0, float_pal, 1);  

			Center224in240();
			
			SetFontColors(13, RGB(2, 2, 2), RGB(6, 6, 6), 0);

			// Title at index 0
			len = strlen(data[0].name);	
			ix = (128/8 - len)/2 + x/8;
			iy = y/8;

			set_font_pal(13);
			put_string(data[0].name, ix, iy);
			
			redraw = 0;
			refresh = 1;
			disp_on();
		}
			
		if(refresh)
		{	
			for(i = 1; i < size; i++)
			{	
				len = strlen(data[i].name);
				
				ix = (128/8 - len)/2 + x/8;
				iy = y/8+2+i+(6-size)/2;
				
				set_font_pal(sel == i ? 15 : 14);
				put_string(data[i].name, ix, iy);
			}
			
			refresh = 0;
		}
		
		controller = joytrg(0);
		
		if (controller & JOY_UP) 
		{
			sel --;
			if(sel < 1)
				sel = size - 1;
			refresh = 1;
		}

		if (controller & JOY_DOWN) 
		{
			sel ++;
			if(sel > size - 1)
				sel = 1;
			refresh = 1;
		}
		
		if (controller & JOY_I)
			end = 1;
	}
	
	disp_off();
	return data[sel].id;
}
