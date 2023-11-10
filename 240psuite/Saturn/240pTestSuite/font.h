/* 
 * 240p Test Suite for Nintendo 64
 * Copyright (C)2018 Artemio Urbina
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


#ifndef FONT_H
#define FONT_H

#define FONT_WHITE	16
#define FONT_RED	15
#define FONT_GREEN	14
#define FONT_BLUE	13
#define FONT_CYAN	12
#define FONT_MAGENTA	11
#define FONT_YELLOW	10
#define FONT_BLACK	9

#define FONT_QUAD_WIDTH	320
#define FONT_QUAD_HEIGHT 224

extern int _fh;
extern int _fw;

extern unsigned char SuiteFont[];
extern int SuiteFont_len;

void LoadFont();
void DrawString(char *str, unsigned int x, unsigned int y, unsigned int palette);
void ClearText(int left, int top, int width, int height);
void ClearTextLayer();

void DrawChar(unsigned int x, unsigned int y, char c, unsigned int palette, bool transparent);

#endif /* !FONT_H */