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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with To Eleven; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef VMU_PRINT_H
#define VMU_PRINT_H

void vmu_clear_bitmap(uint8 bitmap[192]);
void vmu_invert_bitmap(uint8 bitmap[192]);

inline void vmu_flip_bit(uint8 bitmap[192], uint8 x, uint8 y);
	/* (0, 0) is upper-left corner */

inline void vmu_set_bit(uint8 bitmap[192], uint8 x, uint8 y);
	/* (0, 0) is upper-left corner */

void vmu_draw_char(uint8 bitmap[192], unsigned char c, int x, int y);
        /* (x, y) is position for upper-left corner of character,
	   (0, 0) is upper-left corner of screen */

void vmu_draw_str(uint8 bitmap[192], unsigned char *str, int x, int y);
	/* (x, y) is position for upper-left corner of string,
           (0, 0) is upper-left corner of screen */

#endif
