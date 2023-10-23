/* 
 * 240p Test Suite
 * Copyright (C)2014-2022 Artemio Urbina (PC Engine/TurboGrafx-16)
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

#ifdef SCDROM
#define LOADER_OVERLAY 			1
#define MAIN_OVERLAY 			2
#define REFLEXSCD_OVERLAY		3
#define ADPCM_VOICE_OVL			4
#define ADPCM_SWEEP_OVL			5
#define ADPCM_8khz_OVL			6
#endif

#ifdef SYSCARD1
#define OVL_DEFINES
#endif

#ifdef HELP_OVL
#define OVL_DEFINES
#endif

#ifdef OVL_DEFINES
#define LOADER_OVERLAY 		1
#define MAIN_OVERLAY 		2
#define PATTERNS_OVERLAY 	3
#define PATT_EXT_OVERLAY 	4
#define TEST_EXT_OVERLAY 	5
#define TEST_SND_OVERLAY 	6
#define GPHX_OVERLAY 		7
#define HELP_OVERLAY		8
#define ADPCM_VOICE_OVL		9
#define ADPCM_SWEEP_OVL		10
#define ADPCM_8khz_OVL		11
#endif

#ifdef CDROM
#define PATTERNTOOL		10
#define	TOOL_WHITE		19
#define TOOL_IRE		20
#define	TOOL_OVERSCAN	22
#define TOOL_CONVERG	23

#define VIDEOTOOL		30
#define	TOOL_PASSIVE	32
#define TOOL_REFLEX		33
#define TOOL_VSCROLL	35
#define	TOOL_V_STRIPES	36
#define	TOOL_H_STRIPES	37
#define TOOL_CHECK		38
#define TOOL_PHASE		39
#define TOOL_LEDZONE	40

#define SOUNDTOOL		50
#define TOOL_SOUND		50
#define TOOL_AUDIOSYNC	51
#define TOOL_MDFOURIER	52
#define TOOL_CON_INTER	53

#define HARDWARETOOL	60
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
extern int x5;
extern int y;
extern int y2;

extern int i;
extern int sel;
extern int option;
extern unsigned char end;

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
extern int hres;

extern unsigned char back;
extern int colswap;

extern unsigned char *mem;
extern int runmdf;

void Set256H();
void Set320H();
void Set352H();
void Set512H();

void Set224p();
void Set240p();
void Set239p();

