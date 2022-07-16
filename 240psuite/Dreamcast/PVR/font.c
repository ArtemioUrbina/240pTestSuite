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
#include "font.h"
#include "image.h"
#include "vmodes.h"

float f_size = 8.0f;

float fw = 5.0f;
float fh = 9.0f;

float half_fw = 2.2f;

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
		font_t = LoadKMG("/rd/font.kmg.gz", 0);
		if(!font_t)
			return;
		font_t->layer = 2.0f;
	}	

	if(!black_t)
	{				
		black_t = LoadKMG("/rd/black.kmg.gz", 0);
		if(!black_t)
			return;
		black_t->layer = 2.0f;
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

	// Don't draw outside of visible screen
	if(x < -2*fw ||x > dW+fw || y < -2*fh || y > dH+fh)
		return;
	
	if(c < 0x20)
		return;
	c -= 0x20;
	if(c > 0x5F)		// save some polygons, skip space char (0x20) and all out of range characters
		return;

	charx = (c % 16) * fw;
	chary = (c / 16) * f_size;
	CalculateUV(charx, chary, fw, f_size, font_t);
	font_t->x = x;
	font_t->y = y;	
	DrawImage(font_t);
}

void DrawString(float x, float y, float r, float g, float b, char *str) 
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

		if(*str == '$')
		{
			x += half_fw;
			str++;
			continue;
		}
		
		if(*str == '#')
		{						
			highlight = !highlight;
			str++;
			
			if(highlight && r + g + b != 0.0f)
			{
				font_t->r = 0.0f;
				font_t->g = 0.0f;
				font_t->b = 0.0f;
				switch(*str)
				{
					case 'R':
						font_t->r = 1.0f;
						break;
					case 'G':
						font_t->g = 1.0f;
						break;
					case 'B':
						font_t->b = 1.0f;
						break;
					case 'Y':
						font_t->r = 1.0f;
						font_t->g = 1.0f;
						break;
					case 'C':
						font_t->g = 1.0f;
						font_t->b = 1.0f;
						break;
					case 'M':
						font_t->r = 1.0f;
						font_t->b = 1.0f;
						break;
					case 'O':
						font_t->r = 0.812f;
						font_t->g = 0.200f;
						font_t->b = 0.067f;
						break;
					case 'W':
					default:											
						font_t->r = 1.0f;
						font_t->g = 1.0f;
						font_t->b = 1.0f;
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

void DrawStringNH(float x, float y, float r, float g, float b, char *str) 
{	
	float orig_x = x;

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
		
		DrawChar(x, y, *str++);
		x += fw;
	}
}

void DrawStringS(float x, float y, float r, float g, float b, char *str) 
{
	font_t->layer = 2.0f;
	font_t->alpha = 0.75f;
	DrawString(x+1, y+1, 0.0f, 0.0f, 0.0f, str);	
	font_t->layer = 3.0f;
	font_t->alpha = 1.0f;
	DrawString(x, y, r, g, b, str);
}

void DrawStringSCenteredInternal(float y, float r, float g, float b, char *str, int full_screen, int useback) 
{
	int			currlen = 0;
	char 		*startstr = NULL;
	float		xpos = 34.0f, len = 0, screen_width = 0;
	
	startstr = str;
	while (*str) 
	{		
		if(*str == '\n')
		{
			*str = '\0';
			len = MeasureString(startstr)*fw;
			if(len)
			{
				screen_width = full_screen ? dW : 320;
				xpos = (int)((screen_width-len)/2.0f);
				if(!useback)
					DrawStringS(xpos, y, r, g, b, startstr);
				else
					DrawStringB(xpos, y, r, g, b, startstr);
			}
			*str = '\n';
			startstr = str + 1;
			currlen = 0;
			y += fh;
		}
		str++;
		currlen++;
	}
	
	if(currlen)
	{
		len = MeasureString(startstr)*fw;
		if(len)
		{
			screen_width = full_screen ? dW : 320;
			xpos = (int)((screen_width-len)/2.0f);
			if(!useback)
				DrawStringS(xpos, y, r, g, b, startstr);
			else
				DrawStringB(xpos, y, r, g, b, startstr);
		}
	}
}

void DrawStringSCentered(float y, float r, float g, float b, char *str)
{
	DrawStringSCenteredInternal(y, r, g, b, str, 0, 0);
}

void DrawStringSCenteredFull(float y, float r, float g, float b, char *str)
{
	DrawStringSCenteredInternal(y, r, g, b, str, vmode < HIGH_RES ? 0 : 1, 0);
}

void DrawStringSCenteredXY(float r, float g, float b, char *str) 
{
	int			currlen = 0;
	char 		*startstr = NULL;
	float		xpos = 34.0f, ypos = 0, len = 0, height = 0;
	
	height = countLineFeeds(str)*fh;
	ypos = 120 - (int)(height/2.0f) -fh;
	startstr = str;
	
	while (*str) 
	{		
		if(*str == '\n')
		{
			*str = '\0';
			len = MeasureString(startstr)*fw;
			if(len)
			{
				xpos = (int)((320-len)/2.0f);
				DrawStringS(xpos, ypos, r, g, b, startstr);
			}
			*str = '\n';
			startstr = str + 1;
			currlen = 0;
			ypos += fh;
		}
		str++;
		currlen++;
	}
	
	if(currlen)
	{
		len = MeasureString(startstr)*fw;
		if(len)
		{
			xpos = (int)((320-len)/2.0f);
			DrawStringS(xpos, ypos, r, g, b, startstr);
		}
	}
}

int MeasureString(char *str)
{
	int len = 0, count = 0;
	char *pos = NULL, *check = str;
	
	if(!str)
		return len;
		
	len = strlen(str);
	
	do
	{
		pos = strchr(check, '#');
		if(pos)
		{
			check = pos + 1;
			count ++;
		}
	}while(pos);
	
	len = len - 2*count;
	
	if(len < 0)
		len = 0;
		
	return len;
}


void DrawStringB(float x, float y, float r, float g, float b, char *str) 
{	
	if(black_t)
	{
		float len;
		
		len = MeasureString(str);
		len *= fw;

		black_t->x = x - 1;
		black_t->y = y - 1;
		black_t->w = len + 2;
		black_t->h = f_size + 2;
		DrawImage(black_t);
	}
	
	DrawString(x, y, r, g, b, str);
}

void DrawStringBCentered(float y, float r, float g, float b, char *str)
{
	DrawStringSCenteredInternal(y, r, g, b, str, 0, 1);
}

void DrawStringBCenteredFull(float y, float r, float g, float b, char *str)
{
	DrawStringSCenteredInternal(y, r, g, b, str, vmode < HIGH_RES ? 0 : 1, 1);
}

int countLineFeeds(char *str) 
{	
	int lfeed = 0;
	
	while (*str) 
	{		
		if(*str == '\n')
			lfeed ++;
		str++;
	}
	return lfeed;
}

/* Big Numbers */

void LoadNumbers()
{
	if(!num_t)
	{
		num_t = LoadKMG("/rd/numbers.kmg.gz", 0);
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


void DrawDigit(float x, float y, float r, float g, float b, int digit) 
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

void DrawNumber(float x, float y, float r, float g, float b, int number) 
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
