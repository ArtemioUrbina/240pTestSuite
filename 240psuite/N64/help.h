/* 
 * 240p Test Suite
 * Copyright (C)2016 Artemio Urbina (N64)
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


#ifndef HELP_H
#define HELP_H

#include "utils.h"

extern char *HelpData;

#define ALTERNATE		"/help/alt240p.txt"
#define BACKLITHELP		"/help/backlit.txt"
#define COLORBLEEDHELP	"/help/bleed.txt"
#define CHECKHELP		"/help/check.txt"
#define COLOR601		"/help/color601.txt"
#define COLORBARSHELP	"/help/colors.txt"
#define GENERALHELP		"/help/general.txt"
#define GRAYHELP		"/help/gray.txt"
#define GRIDHELP		"/help/grid.txt"
#define GRID224HELP		"/help/grid224.txt"
#define PLUGEHELP		"/help/pluge.txt"
#define STRIPESHELP		"/help/stripes.txt"
#define IREHELP  		"/help/ire100.txt"
#define SOUNDHELP		"/help/sound.txt"
#define DROPSHADOW		"/help/dshadow.txt"
#define STRIPED			"/help/striped.txt"
#define GRIDSCROLL		"/help/gridscroll.txt"
#define SCROLL			"/help/scroll.txt"
#define MANUALLAG		"/help/manuallag.txt"
#define PASSIVELAG		"/help/passivelag.txt"
#define SMPTECOLOR		"/help/SMPTEColor.txt"
#define WHITEHELP		"/help/white.txt"
#define LINEARITYHELP	"/help/linearity.txt"
#define LIN224HELP		"/help/linearity224.txt"
#define SHARPNESSHELP	"/help/sharpness.txt"
#define OVERSCANHELP	"/help/overscan.txt"
#define OPTIONSHELP		"/help/options.txt"
#define VIDEOHELP		"/help/vmode.txt"
#define DIAGONALHELP	"/help/diagonal.txt"

#define OVERSCANWARNING	"/help/warningoverscan.txt"
#define GRIDWARNING		"/help/warninggrid.txt"

void HelpWindow(char *filename, int usebuffer);
char *LoadHelpFile(char *filename, char ***pages, int *npages);

#endif

