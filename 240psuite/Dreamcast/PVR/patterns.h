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

#ifndef PATTERNS_H
#define PATTERNS_H

void DrawPluge();
void DrawWhiteScreen();
void DrawGrayRamp();
void DrawColorBars();
void DrawColorBleed();
void Draw601ColorBars();
void DrawGrid();
void DrawMonoscope();
void Draw100IRE();
void DrawEBUColorBars();
void DrawSMPTEColorBars();
void DrawSharpness();
void DrawOverscan();
void DrawConvergence();
void DrawHCFR();


#define HCFR_COLOR_NAME_MAX		64

typedef struct hcfr_color_st {
	char	name[HCFR_COLOR_NAME_MAX];
	int		r;
	int		g;
	int		b;
} hcfr_color;

typedef struct hcfr_files_st {
	char	*dispname;
	char	*filename;
} hcfr_file;

#endif

