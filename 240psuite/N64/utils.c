/* 
 * 240p Test Suite for Nintendo 64
 * Copyright (C)2016 Artemio Urbina
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

#include "utils.h"

int fh = 8; // font height

void init_n64()
{
	init_interrupts();
	
	init_video();
	set_video();
	dfs_init(DFS_DEFAULT_LOCATION);
	controller_init();
	timer_init();
}

void DrawString(int x, int y, int r, int g, int b, char *str)
{
	uint32_t color = 0;

	color = graphics_make_color(r, g, b, 0xff);
	graphics_set_color(color, 0x00000000);
	graphics_draw_text(__dc, x, y, str);
}

void DrawStringS(int x, int y, int r, int g, int b, char *str)
{
	uint32_t color = 0;

	color = graphics_make_color(r, g, b, 0xff);
    graphics_set_color(0x00000000, 0x00000000);
	graphics_draw_text(__dc, x+1, y+1, str);
	graphics_set_color(color, 0x00000000);
	graphics_draw_text(__dc, x, y, str);
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

void DrawStringB(int x, int y, int r, int g, int b, char *str)
{
	int len = 0;
	uint32_t color = 0;
				
	len = MeasureString(str);
	len *= 5;
	
	color = graphics_make_color(r, g, b, 0xff);
	
    graphics_draw_box(__dc, x, y-1, len + 3, fh, graphics_make_color(0, 0, 0, 0));
	graphics_set_color(color, 0x00000000);
	graphics_draw_text(__dc, x, y, str);
}

int DetectRamSize()
{
	int available_memory_size = *(int *)(0x80000318);

	return (available_memory_size/0x100000);
}