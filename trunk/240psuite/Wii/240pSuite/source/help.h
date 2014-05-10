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

#ifndef HELP_H
#define HELP_H

#include "image.h"

#define COLORBLEEDHELP	0  // "/rd/help/bleed.txt"
#define CHECKHELP		1  // "/rd/help/check.txt"
#define COLORBARSHELP	2  // "/rd/help/colors.txt"
#define GENERALHELP		3  // "/rd/help/general.txt"
#define GRAYHELP		4  // "/rd/help/gray.txt"
#define GRIDHELP    	5  // "/rd/help/grid.txt"
#define PLUGEHELP		6  // "/rd/help/pluge.txt"
#define STRIPESHELP		7  // "/rd/help/stripes.txt"
#define BACKLITHELP		8  // "/rd/help/backlit.txt"
#define IREHELP  		9  // "/rd/help/ire100.txt"
#define ALTERNATE		10 // "/rd/help/alt240p.txt"
#define SOUNDHELP		11 // "/rd/help/sound.txt"
#define DROPSHADOW		12 // "/rd/help/dshadow.txt"
#define STRIPED			13 // "/rd/help/striped.txt"
#define GRIDSCROLL		14 // "/rd/help/gridscroll.txt"
#define SCROLL			15 // "/rd/help/scroll.txt"
#define MANUALLAG		16 // "/rd/help/manuallag.txt"
#define PASSIVELAG		17 // "/rd/help/passivelag.txt"

u32 HelpWindow(char *filename, ImagePtr screen);
char *LoadHelpFile(char *filename, char ***pages, int *npages);

#endif

