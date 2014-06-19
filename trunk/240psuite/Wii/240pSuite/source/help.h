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

#define COLORBLEEDHELP	bleed_txt
#define CHECKHELP		check_txt
#define COLORBARSHELP	colors_txt
#define GENERALHELP		general_txt
#define GRAYHELP		gray_txt
#define GRIDHELP    	grid_txt
#define PLUGEHELP		pluge_txt
#define STRIPESHELP		stripes_txt
#define BACKLITHELP		backlit_txt
#define IREHELP  		ire100_txt
#define ALTERNATE		alt240p_txt
#define SOUNDHELP		sound_txt
#define DROPSHADOW		dshadow_txt
#define STRIPED			striped_txt
#define GRIDSCROLL		gridscroll_txt
#define SCROLL			scroll_txt
#define MANUALLAG		manuallag_txt
#define PASSIVELAG		passivelag_txt
#define WHITEHELP 		white_txt
#define COLOR601HELP	color601_txt
#define SMPTECOLOR 		SMPTEColor_txt
#define LINEARITYHELP	linearity_txt
#define CHECK480PWII	check480pWii_txt
#define SHARPNESSHELP	sharpness_txt
#define OVERSCANHELP	overscan_txt
#define VIDEOMODEHELP	vmode_txt
#define OPTIONSHELP		options_txt
#define OPTIONSGCHELP	optionsgc_txt

extern char *SMPTEColor_txt[];
extern char *alt240p_txt[];
extern char *backlit_txt[];
extern char *bleed_txt[];
extern char *check_txt[];
extern char *check480pWii_txt[];
extern char *color601_txt[];
extern char *colors_txt[];
extern char *dshadow_txt[];
extern char *general_txt[];
extern char *gray_txt[];
extern char *grid_txt[];
extern char *gridscroll_txt[];
extern char *ire100_txt[];
extern char *linearity_txt[];
extern char *manuallag_txt[];
extern char *options_txt[];
extern char *optionsgc_txt[];
extern char *overscan_txt[];
extern char *passivelag_txt[];
extern char *pluge_txt[];
extern char *scroll_txt[];
extern char *sharpness_txt[];
extern char *sound_txt[];
extern char *striped_txt[];
extern char *stripes_txt[];
extern char *vmode_txt[];
extern char *white_txt[];

void HelpWindow(ImagePtr screen);
void DrawHelpWindow();
void ShowAllHelp();

#endif

