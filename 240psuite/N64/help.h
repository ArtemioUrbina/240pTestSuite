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

#ifndef HELP_H
#define HELP_H

extern char *helpData;

#define PLUGEHELP		"/help/pluge.txt"
#define GENERALHELP		"/help/general.txt"
#define GRAYHELP		"/help/gray.txt"
#define DROPSHADOW		"/help/dshadow.txt"
#define COLORBARSHELP	"/help/colors.txt"
#define SCROLLHELP		"/help/scroll.txt"
#define SMPTECOLOR		"/help/SMPTEColor.txt"
#define COLOR601		"/help/color601.txt"
#define COLORBLEEDHELP	"/help/bleed.txt"
#define SHARPNESSHELP	"/help/sharpness.txt"
#define WHITEHELP		"/help/white.txt"
#define CHECKHELP		"/help/check.txt"
#define STRIPESHELP		"/help/stripes.txt"
#define MDFOURIERHELP	"/help/mdfourier.txt"
#define MEMORYHELP		"/help/memory.txt"
#define PASSIVELAG		"/help/passivelag.txt"

#define ALTERNATE		"/help/alt240p.txt"
#define BACKLITHELP		"/help/backlit.txt"
#define GRIDHELP		"/help/grid.txt"
#define GRID224HELP		"/help/grid224.txt"
#define IREHELP  		"/help/ire100.txt"
#define SOUNDHELP		"/help/sound.txt"
#define STRIPED			"/help/striped.txt"
#define GRIDSCROLL		"/help/gridscroll.txt"
#define MANUALLAG		"/help/manuallag.txt"
#define OVERSCANHELP	"/help/overscan.txt"
#define OPTIONSHELP		"/help/options.txt"
#define VIDEOHELP		"/help/vmode.txt"
#define DIAGONALHELP	"/help/diagonal.txt"

#define OVERSCANWARNING	"/help/warningoverscan.txt"
#define GRIDWARNING		"/help/warninggrid.txt"

void helpWindow(char *filename);
char *loadHelpFile(char *filename, char ***pages, int *npages);

#endif

