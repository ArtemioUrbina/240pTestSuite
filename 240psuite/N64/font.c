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
 
 #include "font.h"
 #include "video.h"
 
 sprite_t *custom_font = NULL;
 
 int fw = 8; // font width
 int fh = 8; // font height
 
 int loadFont()
 {
	if(custom_font)
		return 0;
	
	/* Read in the custom font */
	int fp = dfs_open("/240pSuite-font.sprite");
	if(fp == DFS_ESUCCESS)
		return 0;

	custom_font = malloc(dfs_size(fp));
	if(!custom_font)
	{
		dfs_close( fp );
		return 0;
	}
	
	if(dfs_read(custom_font, 1, dfs_size(fp), fp) < 0)
	{
		free(custom_font);
		custom_font = NULL;
		return 0;
	}
	
	dfs_close(fp);
	
	graphics_set_font_sprite(custom_font);
	return 1;
 }
 
 void releaseFont()
 {
	if(custom_font)
	{
		free(custom_font);
		custom_font = NULL;
	}
 }
 
 int measureString(char *str)
{
	return(strlen(str));
}
 
void drawString(int x, int y, int r, int g, int b, char *text)
{
	uint32_t color = 0;

	color = graphics_make_color(r, g, b, 0xff);
	graphics_set_color(color, 0x00000000);
	graphics_draw_text(__dc, x, y, text);
}

void drawStringS(int x, int y, int r, int g, int b, char *text)
{
	uint32_t color = 0;

	color = graphics_make_color(r, g, b, 0xff);
	
	graphics_set_color(0x00000000, 0x00000000);
	graphics_draw_text(__dc, x+1, y+1, text);

	graphics_set_color(color, 0x00000000);
	graphics_draw_text(__dc, x, y, text);
}

void drawStringB(int x, int y, int r, int g, int b, char *text)
{
	int width = 0;
	uint32_t color = 0;
				
	width = measureString(text)*fw;
	color = graphics_make_color(r, g, b, 0xff);
	
    graphics_draw_box(__dc, x-1, y-1, width, fh+1, 0x00000000);
	graphics_set_color(color, 0x00000000);
	graphics_draw_text(__dc, x, y, text);
}

 
 