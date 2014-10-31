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
 
#ifdef CDROM1

#define LOADER_OVERLAY 		1
#define MAIN_OVERLAY 		2
#define PATTERNS_OVERLAY 	3
#define TEST_EXT_OVERLAY 	4
#define GPHX_OVERLAY 		5
#define HELP_OVERLAY		6

#define TOOL_VSCROLL	1
#define TOOL_SOUND		2
#define TOOL_MANUAL		3
#define	TOOL_PASSIVE	4

#endif

#ifdef HELP_OVL

#define LOADER_OVERLAY 		1
#define MAIN_OVERLAY 		2
#define PATTERNS_OVERLAY 	3
#define TEST_EXT_OVERLAY 	4
#define GPHX_OVERLAY 		5
#define HELP_OVERLAY		6

#define TOOL_VSCROLL	1
#define TOOL_SOUND		2
#define TOOL_MANUAL		3
#define	TOOL_PASSIVE	4

#endif
 
#define XRES_BW 0x80

extern int xres_flags;
extern unsigned char Enabled240p;
extern unsigned char UseDefault;
extern unsigned char EnabledSoft;
extern unsigned char Enabled_C_BW;

extern int controller;   
extern int read; 

extern unsigned char redraw;
extern unsigned char refresh;

extern int row;
extern int x;
extern int x1;
extern int x2;
extern int x3;
extern int x4;
extern int y;
extern int y2;

extern int speed;
extern int color;
extern unsigned char text;

extern int clicks[];
extern unsigned char variation;
extern unsigned char change;
extern unsigned char audio;
extern unsigned char view;
extern int vary;

extern unsigned char draw;
extern int top;
extern int bottom;
extern int left;
extern int right;
extern unsigned char previous;
extern unsigned char screen;
 
void Set224p();
void Set240p();
void Set239p();

void SetNormalHMode();
void SetArcadeMode();

void spr_make(int spriteno, int spritex, int spritey, int spritepattern, int ctrl1, int ctrl2, int sprpal, int sprpri);