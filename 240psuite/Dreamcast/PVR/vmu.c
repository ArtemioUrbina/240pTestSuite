/* 
 * 240p Test Suite
 * Copyright (C)2011-2022 Artemio Urbina
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

#include <kos.h>
#include <string.h>
#include <stdlib.h>
#include "vmu_print.h"
#include "vmodes.h"
#include "vmu.h"

static uint8 bitmap[192];

int isVMUPresent()
{
	maple_device_t	*vmu;

	vmu = maple_enum_type(0, MAPLE_FUNC_MEMCARD);
	if(!vmu)
		return 0;
	return 1;
}

void updateVMU(char *line1, char *line2, int force)
{
	maple_device_t *mvmu = NULL;

	if(!force && ovmode == vmode && ovcable == vcable)
		return;

	ovmode = vmode;
	ovcable = vcable;

	mvmu = maple_enum_type(0, MAPLE_FUNC_LCD);
	if (mvmu != 0)
	{
		vmu_clear_bitmap(bitmap);
		vmu_draw_str(bitmap, (unsigned char*)line1, 0, 0);
		vmu_draw_str(bitmap, (unsigned char*)line2, 0, 10);
		switch(vmode)
		{
			case VIDEO_240P:
				vmu_draw_str(bitmap, (unsigned char*)"     240p", 0, 20);
				break;
			case VIDEO_480I_A240:
				vmu_draw_str(bitmap, (unsigned char*)"240p->480i", 0, 20);
				break;
			case VIDEO_480I:
				vmu_draw_str(bitmap, (unsigned char*)"  480i/FS", 0, 20);
				break;
			case VIDEO_288P:
				vmu_draw_str(bitmap, (unsigned char*)"     288p", 0, 20);
				break;
			case VIDEO_576I_A264:
				vmu_draw_str(bitmap, (unsigned char*)"288p->576i", 0, 20);
				break;
			case VIDEO_576I:
				vmu_draw_str(bitmap, (unsigned char*)"  576i/FS", 0, 20);
				break;
			case VIDEO_480P:
				vmu_draw_str(bitmap, (unsigned char*)"  480p/FS", 0, 20);
				break;
			case VIDEO_480P_SL:
				vmu_draw_str(bitmap, (unsigned char*)"  480p/SL", 0, 20);
				break;
		}
		vmu_draw_lcd(mvmu, bitmap);
	}
}

void updateVMUFlash(char *line1, char *line2, int force)
{
	maple_device_t *mvmu = NULL;

	updateVMU(line1, line2, force);
	mvmu = maple_enum_type(0, MAPLE_FUNC_LCD);
	if(mvmu != 0)
	{
		vmu_invert_bitmap(bitmap);
		vmu_draw_lcd(mvmu, bitmap);
		vmu_invert_bitmap(bitmap);
		vmu_draw_lcd(mvmu, bitmap);
	}
}

char *sd_xpm[32] = {
"    ........................................    ",
"   ..........................................   ",
"   ........     ...................  ........   ",
"   ........                    ..... ........   ",
"   ........                     .... ........   ",
"   ........                     .... ........   ",
"   ........                    ....  ........   ",
"   .......                      ..    .......   ",
"   .......                            .......   ",
"   ......                             .......   ",
"    .....                              ......   ",
"    .....                              .....    ",
"      ...                              .....    ",
"      ...    ....              ....    ...      ",
"      ...   .. ....          .... ..   ...      ",
"      ...     ........    ........     ...      ",
"     . ...   .. ......    ...... ..    ...      ",
"     . ...   . ... .        . ... .   ... .     ",
"     . ...   . .....        ..... .   ... .     ",
"     . ...   . .....        ..... .   ... .     ",
"      .  .      ...          ...      ... .     ",
"      ....    .       .  .       .    .  .      ",
"          .      ..  .    .  ..      .....      ",
"          .           ....           .          ",
"          .            ..            .          ",
"        ...                          ...        ",
"        .  .                        .  .        ",
"       ... .         ......         .. ..       ",
"       .  ...                      ..  ..       ",
"    .....  ....        ..        .....  ....    ",
"   ..  .  . .....              ...... . .  ..   ",
"   . . . . . .......        ......... . ..  .   "};

char *sd_b1_xpm[32] = {
"    ........................................    ",
"   ..........................................   ",
"   ........     ...................  ........   ",
"   ........                    ..... ........   ",
"   ........                     .... ........   ",
"   ........                     .... ........   ",
"   ........                    ....  ........   ",
"   .......                      ..    .......   ",
"   .......                            .......   ",
"   ......                             .......   ",
"    .....                              ......   ",
"    .....                              .....    ",
"      ...                              .....    ",
"      ...                              ...      ",
"      ...    ....              ....    ...      ",
"      ...   .. ....          .... ..   ...      ",
"     . ...    ........    ........     ...      ",
"     . ...   .. ......    ...... ..   ... .     ",
"     . ...   . ... .        . ... .   ... .     ",
"     . ...   . .....        ..... .   ... .     ",
"      .  .    .                  .    ... .     ",
"      ....       ..   .  .   ..       .  .      ",
"          .          .    .          .....      ",
"          .           ....           .          ",
"          .            ..            .          ",
"        ...                          ...        ",
"        .  .                        .  .        ",
"       ... .         ......         .. ..       ",
"       .  ...                      ..  ..       ",
"    .....  ....        ..        .....  ....    ",
"   ..  .  . .....              ...... . .  ..   ",
"   . . . . . .......        ......... . ..  .   "};

char *sd_b2_xpm[32] = {
"    ........................................    ",
"   ..........................................   ",
"   ........     ...................  ........   ",
"   ........                    ..... ........   ",
"   ........                     .... ........   ",
"   ........                     .... ........   ",
"   ........                    ....  ........   ",
"   .......                      ..    .......   ",
"   .......                            .......   ",
"   ......                             .......   ",
"    .....                              ......   ",
"    .....                              .....    ",
"      ...                              .....    ",
"      ...                              ...      ",
"      ...                              ...      ",
"      ...    ....              ....    ...      ",
"     . ...  .. ....          .... ..   ...      ",
"     . ...     .......    .......     ... .     ",
"     . ...      ......    ......      ... .     ",
"     . ...                            ... .     ",
"      .  .                            ... .     ",
"      ....            .  .            .  .      ",
"          .          .    .          .....      ",
"          .           ....           .          ",
"          .            ..            .          ",
"        ...                          ...        ",
"        .  .                        .  .        ",
"       ... .         ......         .. ..       ",
"       .  ...                      ..  ..       ",
"    .....  ....        ..        .....  ....    ",
"   ..  .  . .....              ...... . .  ..   ",
"   . . . . . .......        ......... . ..  .   "};

void updateVMU_SD()
{
	updateVMUGraphic(sd_xpm);
}

#define MAX_FRAMES 12
char **frames[MAX_FRAMES] = { sd_b1_xpm, NULL, NULL, NULL, sd_b2_xpm, NULL, NULL, sd_b1_xpm, NULL, NULL, NULL, sd_xpm };
int blink_counter = 0;
int is_blinking = 0;
int blink_frame = 0;

// 15 to 20 blinks a minute
// a possible blink every 240 frames
// displaced by MAX_FRAMES of animation
void SD_blink_cycle()
{
	blink_counter++;
	if(!is_blinking)
	{
		if(blink_counter < 240)
			return;
		if(rand() % 100 < 98)	// 2% chance every frame after 240
			return;
		
		is_blinking = 1;
		blink_counter = 0;	
	}
	
	if(frames[blink_frame])
		updateVMUGraphic(frames[blink_frame]);	// takes between 10-25ms

	blink_frame++;
	if(blink_frame >= MAX_FRAMES)
	{
		blink_frame = 0;
		is_blinking = 0;
	}
}

char *donna_xpm[32] = {
"  ..............................................",
"   .............................................",
"    ............................................",
"   .............................................",
" ......................... ................... .",
"       ...................  ................... ",
".       ..................   .................. ",
"......   ..................   ..................",
".......   .................  ...................",
".  ......  ................ ....................",
"  ........ .....................................",
"  ...   ..... ..............                    ",
"     ........... ........ ..                    ",
"     ... .......  ........ ..       ......      ",
"    ..... .. .............  ..     ...     ..   ",
" .. ....  ..  ......................         .. ",
"  .  ...   .. ..........................       .",
"  .. ...   .  .............        ........     ",
"  ..          ...........         ...  .......  ",
"   .          ..     ...   ..   ...    .........",
"    .         .       ..    .. ...     .. .. ...",
".            ..       ..      ..         .... ..",
"             ..       ..                 .... ..",
"            ...       ..              .  ....  .",
"            ..        ..              .. ....  .",
"            .         ..              .  ...   .",
"           .          ..              ..        ",
"          ..           .               .      . ",
"          .  .         .               ..    .  ",
"         .  .          .                ..  .   ",
"        .  .           ..           .           ",
"      ..                .             ..        "};

void updateVMU_Donna()
{
	updateVMUGraphic(donna_xpm);
}

void updateVMUGraphic(char **xpm)
{
	int x = 0, y = 0;
	maple_device_t *mvmu = NULL;

	if(!xpm)
		return; 

	vmu_clear_bitmap(bitmap);
	for(y = 0; y < 32; y++)
	{
		for(x = 0; x < 48; x++)
		{
			char c = xpm[y][x];
			
			if(c == '.')
				vmu_set_bit(bitmap, x, y);
		}
	}
	
	mvmu = maple_enum_type(0, MAPLE_FUNC_LCD);
	if(!mvmu)
		return;
		
	vmu_draw_lcd(mvmu, bitmap);
}

