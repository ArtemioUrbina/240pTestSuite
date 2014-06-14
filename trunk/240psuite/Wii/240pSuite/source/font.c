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

#include "font.h"
#include "image.h"
#include "video.h"
#include "stdio.h"

float f_size = 8.0f;

float fw = 5.0f;
float fh = 9.0f;

ImagePtr 	black_t = NULL;
ImagePtr 	font_t = NULL;

float n_size = 40.0f;

float nw = 24.0f;
float nh = 40.0f;

ImagePtr  num_t = NULL;

void LoadFont()
{
	if(!font_t)
	{
		font_t = LoadImage(FONTIMG, 0);
		font_t->layer = 2.0f;
	}	

	if(!black_t)
	{				
		black_t = LoadImage(BLACKIMG, 0);
		black_t->alpha = 0xaa;
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

void DrawChar(u16 x, u16 y, char c) 
{
	int charx, chary;

	c -= 32;
	charx = (c % 16) * fw;
	chary = (c / 16) * f_size;
	CalculateUV(charx, chary, fw, f_size, font_t);
	font_t->x = x;
	font_t->y = y;	
	DrawImage(font_t);
}

void DrawString(u16 x, u16 y, u8 r, u8 g, u8 b, char *str) 
{	
	float orig_x = x;
	int highlight = 0;

	font_t->r = r;
	font_t->g = g;
	font_t->b = b;
	
	while (*str) 
	{		
		if(*str == '\n')
		{
			x = orig_x;
			y += fh;
			str++;
			continue;
		}
		
		if(*str == '#')
		{						
			highlight = !highlight;
			str++;
			
			if(highlight && r + g + b != 0x0)
			{
				font_t->r = 0x00;
				font_t->g = 0x00;
				font_t->b = 0x00;
				switch(*str)
				{
					case 'R':
						font_t->r = 0xFF;
						break;
					case 'G':
						font_t->g = 0xFF;
						break;
					case 'B':
						font_t->b = 0xFF;
						break;
					case 'Y':
						font_t->r = 0xFF;
						font_t->g = 0xFF;
						break;
					case 'C':
						font_t->g = 0xFF;
						font_t->b = 0xFF;
						break;
					case 'M':
						font_t->r = 0xFF;
						font_t->b = 0xFF;
						break;
					default:						
						font_t->g = 0xFF;
						break;
				}
			}			
			else
			{
				font_t->r = r;
				font_t->g = g;
				font_t->b = b;
			}
			str++;
			continue;
		}
				
		DrawChar(x, y, *str++);
		x += fw;
	}
}

void DrawStringS(u16 x, u16 y, u8 r, u8 g, u8 b, char *str) 
{
	font_t->layer = 1.0f;
	font_t->alpha = 0xaa;
	DrawString(x+1, y+1, 0x00, 0x00, 0x00, str);	
	font_t->layer = 2.0f;
	font_t->alpha = 0xff;
	DrawString(x, y, r, g, b, str);
}

void DrawStringB(u16 x, u16 y, u8 r, u8 g, u8 b, char *str) 
{	
	if(black_t)
	{
		float len;
		
		len = strlen(str);
		len *= fw;

		black_t->x = x - 1;
		black_t->y = y - 1;
		black_t->w = len + 2;
		black_t->h = f_size + 2;
		DrawImage(black_t);
	}
	
	DrawString(x, y, r, g, b, str);
}

/* Big Numbers */

void LoadNumbers()
{
	if(!num_t)
	{
		num_t = LoadImage(NUMBERSIMG, 0);
		num_t->layer = 2.0f;
	}		
}

void ReleaseNumbers()
{	
	if(num_t)
	{
		FreeImage(&num_t);
		num_t = NULL;
	}	
}


void DrawDigit(u16 x, u16 y, u8 r, u8 g, u8 b, int digit) 
{
	int numx, numy;
	
	if(!num_t)
		return;
		
	num_t->r = r;
	num_t->g = g;
	num_t->b = b;	

	numx = (digit % 5) * nw;
	numy = (digit / 5) * n_size;
	CalculateUV(numx, numy, nw, n_size, num_t);
	num_t->x = x;
	num_t->y = y;	
	DrawImage(num_t);
}

void DrawNumber(u16 x, u16 y, u8 r, u8 g, u8 b, int number) 
{
	char  string[10], *str;

	str = string;

	sprintf(str, "%d", number);
	while (*str) 
	{				
		DrawDigit(x, y, r, g, b, *str++);
		x += nw;
	}
}
