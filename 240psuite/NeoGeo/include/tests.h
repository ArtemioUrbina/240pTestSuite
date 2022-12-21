/* 
 * 240p Test Suite for the Neo Geo
 * by Artemio Urbina and Dustin Dembrosky (Dasutin)
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _TESTS_H_
#define _TESTS_H_

#include "types.h"

typedef struct timecode
{
	u16 hours;
	u16 minutes;
	u16 seconds;
	u16 frames;
	u16 type;
	u16 res;
} timecode;

void vt_drop_shadow_test(void);
void vt_striped_sprite_test(void);
void vt_lag_test(void);
void vt_reflex_test(void);
void vt_scroll_test(void);
void vt_gridscroll_test(void);
void vt_horizontal_stripes(void);
void vt_vertical_stripes(void);
void vt_checkerboard(void);
void vt_backlitzone_test(void);
void vt_disappear_logo(void);

#endif /* _TESTS_H_ */