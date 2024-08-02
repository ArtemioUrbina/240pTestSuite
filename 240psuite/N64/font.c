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
#include "image.h" 
 
sprite_t *font240 = NULL;
int __reducedWidthSpace = 1;
 
int fw = 8; // font width
int fh = 10; // font height
int fhR = 8; // font height real

int loadFont() {
	/* Read in the custom fonts */
	
	if(!font240){
		font240 = sprite_load("rom:/240pSuite-font.sprite");
		if(!font240)
			return 0;
	}
	graphics_set_font_sprite(font240);
	fw = 8;
	fh = 10;
	fhR = 8;

	return 1;
}
 
void releaseFont() {
	if(font240)	{
		free(font240);
		font240 = NULL;
	}
}

void useReducedWidthSpace(int use) {
	__reducedWidthSpace = use;
}

#define SPACE_WIDTH			7
#define FULL_SPACE_WIDTH	8
#define LINE_HEIGHT			8

inline unsigned int getSpaceWidth() {
	return(__reducedWidthSpace ? SPACE_WIDTH : FULL_SPACE_WIDTH);
}
 
unsigned int measureString(char *str) {
	unsigned int len = 0;
	
	while(*str) {
		switch( *str ) {
			case '\r':
			case '\n':
				break;
			case '#':
				str++;
				break;
			case '\t':
				len += fw*5;
				break;
			case '.':
			case ',':
			case ';':
			case '\'':
				len = len - fw/2 + 1;
				len += fw/2 + 1;
				break;
			case ' ':
				len += getSpaceWidth();
				break;
			default:
				len += fw;
				break;
		}
		str++;
	}
	return(len);
}

uint32_t f_color = 0xFFFFFFFF;

void graphics_draw_text_suite( surface_t* disp, int x, int y, const char * const msg ) {
	int highlight = 0, lh = LINE_HEIGHT, sw = getSpaceWidth();
	uint32_t old_f_color = 0xFFFFFFFF;
	
	if( disp == 0 ) { return; }
	if( msg == 0 ) { return; }
	
	int tx = x;
	int ty = y;
	const char *text = (const char *)msg;

	while( *text ) {
		switch( *text ) {
			case '\r':
				break;
			case '\n':
				tx = x;
				ty += lh;
				break;
			case ' ':
				tx += sw;
				break;
			case '\t':
				tx += fw * 5;
				break;
			case '#':
				highlight = !highlight;

				if(highlight) {
					old_f_color = f_color;
					text++;
					if(f_color != 0x00000000) {
						switch(*text)
						{
							case 'R':
								f_color = graphics_make_color(0xff, 0x00, 0x00, 0xff);
								break;
							case 'G':
								f_color = graphics_make_color(0x00, 0xff, 0x00, 0xff);
								break;
							case 'B':
								f_color = graphics_make_color(0x00, 0x00, 0xff, 0xff);
								break;
							case 'Y':
								f_color = graphics_make_color(0xff, 0xff, 0x00, 0xff);
								break;
							case 'C':
								f_color = graphics_make_color(0x00, 0xff, 0xff, 0xff);
								break;
							case 'M':
								f_color = graphics_make_color(0xff, 0xff, 0x00, 0xff);
								break;
							case 'W':
							default:
								f_color = graphics_make_color(0xff, 0xff, 0xff, 0xff);
								break;
						}
					}
				}			
				else {
					f_color = old_f_color;
					text++;
				}
				graphics_set_color(f_color, 0x00000000);
				break;
			default: {
					int shown = 0;
					
					// show ... as three dots with less spacing
					if(*text == '.' && *(text + 1) != '\0') {
						if(*(text + 1) == '.' && *(text + 1) == *(text + 2)) {
							int stride = fw;
							
							tx -= fw/4;
							for(int pos = 0; pos < 3; pos++) {
								graphics_draw_character(disp, tx, ty, '.');
								tx += stride;
							}
							tx += fw/4;
							shown = 1;
							text += 2;
						}
					}
					
					// show some characters with less spacing
					if(!shown) {
						if(*text == '.' || *text == ',' || *text == ';' || 
							*text == '\'') {
							tx = tx - fw/2 + 1;
							graphics_draw_character(disp, tx, ty, *text);
							tx += fw/2 + 1;
						} else {  // any regular character
							graphics_draw_character(disp, tx, ty, *text);
							tx += fw;
						}
					}
				}
				break;
		}

		text++;
	}
}
 
void drawString(int x, int y, int r, int g, int b, char *text) {
	f_color = graphics_make_color(r, g, b, 0xff);
	graphics_set_color(f_color, 0x00000000);
	graphics_draw_text_suite(__disp, x, y, text);
}

void drawStringS(int x, int y, int r, int g, int b, char *text) {
	f_color = 0x00000000;
	graphics_set_color(f_color, 0x00000000);
	graphics_draw_text_suite(__disp, x+1, y+1, text);

	f_color = graphics_make_color(r, g, b, 0xff);
	graphics_set_color(f_color, 0x00000000);
	graphics_draw_text_suite(__disp, x, y, text);
}

void drawStringB(int x, int y, int r, int g, int b, char *text) {
	unsigned int boxWidth = 0, boxHeight = fh;

	boxWidth = measureString(text) + 2;
	f_color = graphics_make_color(r, g, b, 0xff);

	graphics_draw_box(__disp, x-1, y-1, boxWidth, boxHeight, 0x00000000);
	graphics_set_color(f_color, 0x00000000);
	graphics_draw_text_suite(__disp, x, y, text);
}

void drawStringC(int y, int r, int g, int b, char *text) {
	unsigned int x = 0, strWidth = 0;
	
	strWidth = measureString(text);
	x = (getDispWidth() - strWidth)/2;
	f_color = 0x00000000;
	graphics_set_color(f_color, 0x00000000);
	graphics_draw_text_suite(__disp, x+1, y+1, text);

	f_color = graphics_make_color(r, g, b, 0xff);
	graphics_set_color(f_color, 0x00000000);
	graphics_draw_text_suite(__disp, x, y, text);
}

 
 