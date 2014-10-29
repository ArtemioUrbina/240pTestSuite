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

extern char my_font[];

void setupFont()
{	
    load_font(my_font, 96);
    set_font_pal(14);

	SetFontColors(14, RGB(3, 3, 3), RGB(7, 7, 7), 0);
	SetFontColors(15, RGB(3, 3, 3), RGB(7, 0, 0), 0);
}

void SetFontColors(int palette, int back, int text, int shade)
{
	set_color(palette*16+1, back);    
	set_color(palette*16+2, text);  
	set_color(palette*16+3, shade);  
}

int RGB(int R, int G, int B)
{
	return((G << 6) + (R << 3) + (B));
}