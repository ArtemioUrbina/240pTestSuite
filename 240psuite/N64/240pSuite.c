/* 
 * 240p Test Suite for Nintendo 64
 * Copyright (C)2024 Artemio Urbina
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
 
#include "system.h"
#include "video.h"
#include "font.h"
#include "image.h"
 
int main(void)
{
	int alternate = 1, sel = 0;
	
	initN64();
	loadFont();

	sprite_t *bg = NULL, *sd = NULL;
	
	bg = sprite_load("rom:/mainbg.sprite");
	sd = sprite_load("rom:/sd.sprite");

	while(1) {
		
		getDisplay();
		

		if(!alternate)
		{
			graphics_fill_screen(__dc, graphics_make_color(0xff, 0, 0xff, 0xff));
			
			graphics_draw_sprite(__dc, 0, 0, bg);
		}
		else
		{
			rdpqStart();
			rdpqDrawImage(bg, 0, 0);
			rdpqDrawImage(sd, 221, 86);
			rdpqEnd();
		}
		
		
		int fh = 12;
		int c = 1, x = 20, y = 40+fh;
		int r = 0xFF, g = 0xFF, b = 0xFF;
		
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Pluge"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Color Bars"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "EBU Color Bars"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "SMPTE Color Bars"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Color Bars with Gray Scale"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Color Bleed Check"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Gray Ramp"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "White & RGB Screens"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "100 IRE"); y += fh; c++;
		drawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Sharpness"); y += fh; c++;
		
		sel ++;
		if(sel > 10)
			sel = 0;
				
		waitVsync();
	}
}
