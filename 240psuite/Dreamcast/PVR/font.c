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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <kos.h>
#include "font.h"
#include "image.h"
#include "vmodes.h"

float f_size = 8.0f;

float fw = 8.0f;
float fh = 9.0f;

ImagePtr   black_t = NULL;
ImagePtr   font_t = NULL;

void LoadFont()
{
	if(!font_t)
	{
		font_t = LoadImage("/rd/font.png", 0);
		font_t->layer = 2.0f;
	}	

	if(!black_t)
	{
		black_t = LoadImage("/rd/black.png", 0);
		black_t->alpha = 0.75f;
	}
}

void ReleaseFont()
{	
	if(font_t)
	{
		FreeImage(&font_t);
		font_t = NULL;
	}
	if(black_t)
	{
		FreeImage(&black_t);
		black_t = NULL;
	}
}

void DrawChar(float x, float y, char c) 
{
	int charx, chary;

	c -= 32;
	charx = (c % 16) * 8;
	chary = (c / 16) * 8;
	CalculateUV(charx, chary, fw, f_size, font_t);
	font_t->x = x;
	font_t->y = y;	
	DrawImage(font_t);
}

void DrawString(float x, float y, float r, float g, float b, char *str) 
{
	font_t->r = r;
	font_t->g = g;
	font_t->b = b;

	while (*str) 
	{    
    DrawChar(x, y, *str++);
		x += fw;
  }
}

void DrawStringS(float x, float y, float r, float g, float b, char *str) 
{
	font_t->layer = 1.0f;
	font_t->alpha = 0.75f;
	DrawString(x+1, y+1, 0.0f, 0.0f, 0.0f, str);	
	font_t->layer = 2.0f;
	font_t->alpha = 1.0f;
	DrawString(x, y, r, g, b, str);
}

void DrawStringB(float x, float y, float r, float g, float b, char *str) 
{	
	if(black_t)
	{
		int len;
		
		len = strlen(str);
		len *= 8;

		black_t->x = x - 1;
		black_t->y = y - 1;
		black_t->w = len + 2;
		black_t->h = f_size + 2;
		DrawImage(black_t);
	}
	
	DrawString(x, y, r, g, b, str);
}
