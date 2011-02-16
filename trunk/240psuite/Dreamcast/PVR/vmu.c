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

#include <kos.h>
#include <string.h>
#include "vmu_print.h"
#include "vmodes.h"

static uint8 bitmap[192];

int ovcable = -1;
int ovmode = -1;

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
			case NATIVE_320:
				vmu_draw_str(bitmap, (unsigned char*)"     240p", 0, 20);
				break;
			case FAKE_640:
				vmu_draw_str(bitmap, (unsigned char*)"Fake 480i", 0, 20);
				break;
			case NATIVE_640:
				vmu_draw_str(bitmap, (unsigned char*)"     480i", 0, 20);
				break;
			case NATIVE_640_FS:
				vmu_draw_str(bitmap, (unsigned char*)"  480i/FS", 0, 20);
				break;
			case FAKE_640_SL:
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

