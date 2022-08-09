/* 
 * 240p Test Suite for Nintendo 64
 * Copyright (C)2018 Artemio Urbina
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

#include <yaul.h>

#include "fs.h"

extern uint8_t root_romdisk[];

static void *romdisk = NULL;
/*
bool fs_init(void)
{
	if (romdisk)
		return false;
	
	romdisk_init();
	romdisk = romdisk_mount("/", root_romdisk);
	if(!romdisk)
		return false;
	return true;
}

void *fs_open(const char *path)
{
	void *fh = NULL;

	if(!romdisk) return fh;
	
	fh = romdisk_open(romdisk, path);
	return fh;
}

bool fs_load(void *fh, void *dst)
{
	size_t len;

	if(!romdisk) return false;
	if(!fh) return false;

	len = romdisk_read(fh, dst, romdisk_total(fh));
	if(len == romdisk_total(fh))
		return true;
	return false;
}

size_t fs_size(void *fh)
{
	if(!romdisk) return 0;
	if(!fh) return 0;
	
	return romdisk_total(fh);
}
*/


