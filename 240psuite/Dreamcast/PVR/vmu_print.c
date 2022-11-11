/* 
 * To Eleven - Dreamcast Audio Player
 * Copyright (C)2001 Patrick Swieskowski
 *
 * This file is part of To Eleven.
 *
 * To Eleven is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * To Eleven is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with To Eleven; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	02111-1307	USA
 */

#include <kos.h>
#include "vmu_font.h"

void vmu_clear_bitmap(uint8 bitmap[192])
{
	int i;
	for (i = 0; i < 192; i++)
		bitmap[i] = 0x00;
}

inline void vmu_flip_bit(uint8 bitmap[192], uint8 x, uint8 y)
	/* (0, 0) is upper-left corner */
{
	if (x < 48 && y < 32)
		bitmap[6*(31-y)+(5-(x/8))] ^= (1 << (x%8));
}

void vmu_invert_bitmap(uint8 bitmap[192])
{
	int x, y;

	for (x = 0; x < 48; x++)
		for (y = 0; y < 32; y++)
			vmu_flip_bit(bitmap, x, y);
}

inline void vmu_set_bit(uint8 bitmap[192], uint8 x, uint8 y)
	/* (0, 0) is upper-left corner */
{
	if (x < 48 && y < 32)
		bitmap[6*(31-y)+(5-(x/8))] |= (1 << (x%8));
}

void vmu_draw_char(uint8 bitmap[192], unsigned char c, int x, int y)
	/* (x, y) is position for upper-left corner of character,
		(0, 0) is upper-left corner of screen */
{
	int i, j;

	if (x < -4 || x > 47 || y < -9 || y > 31)
		return;

	for (i = 0; i < 10; i++) {
		for (j = 0; j < 5; j++) {
			if (vmufont[(int)c][i] & (0x80 >> j))
				vmu_set_bit(bitmap, x+j, y+i);
		}
	}
}

void vmu_draw_str(uint8 bitmap[192], unsigned char *str, int x, int y)
	/* (x, y) is position for upper-left corner of string,
		(0, 0) is upper-left corner of screen */
{
	int i;

	if (y < -9 || y > 31)
		return;
	
	for (i = 0; str[i] != '\0'; i++) {
		if (x < -4) {
			x += 5;
			continue;
		}
		vmu_draw_char(bitmap, str[i], x, y);
		x += 5;
		if (x > 47)
			break;
	}
}
