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
 
 #include "stddef.h"
 
extern int type;	// float menu
extern int ntype;	// float menu

#ifndef MENUDATA
#define MENUDATA
struct menu_data {
    int		id;
	char	*name;
};

typedef struct menu_data fmenudata;

extern fmenudata resmenudata[];

extern int fmx, fmy, tfmx, tfmy, lfm;
#endif

#define	FLOAT_CANCEL	0
#define RES_320 		1
#define RES_256 		2
#define RES_512 		3
#define RES_352 		4

extern char float_map[];
extern int float_bg[];
extern int float_pal[];

void ChangeResType();
int FloatMenuRes(int def);
int FloatMenuResExtra(int def, char *option);
int FloatMenuRes320n256(int def);
int FloatMenuRes320n256_224(int def);
int FloatMenuResMinus352(int def);
int FloatMenu(int def, int size, char *bottomMsg);