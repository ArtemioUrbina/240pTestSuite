/* 
 * 240p Test Suite
 * Copyright (C)2014 Artemio Urbina (SNES)
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
 
#include "snes.h"

#define	HELP_GENERAL		1
#define	HELP_PLUGE			2
#define	HELP_COLORS			3
#define	HELP_GRID		    4
#define	HELP_BLEED			5
#define	HELP_IRE   			6
#define HELP_601CB      	7
#define HELP_SHARPNESS		8
#define HELP_OVERSCAN		9
#define HELP_SMPTE			10
#define HELP_MONOSCOPE		11
#define HELP_GRAY			12
#define HELP_WHITE			13
#define HELP_CHECK			14
#define HELP_STRIPES		15
#define HELP_SHADOW			16
#define HELP_STRIPED		17
#define HELP_MANUALLAG		18
#define HELP_HSCROLL		19
#define HELP_VSCROLL		20
#define HELP_SOUND			21
#define HELP_LED			23
#define HELP_LAG			24
#define	HELP_GRID_239	    25
#define	HELP_MODE7		    26
#define	HELP_VIDEO			27
#define	HELP_ALTERNATE		28
#define HELP_AUDIOSYNC		29
#define HELP_CONTRAST		30
#define HELP_CONVERGENCE	31
#define HELP_DISAPPEAR		32
#define HELP_PHASE			33
#define HELP_MDFOURIER		34

void DrawHelp(u16 help);
