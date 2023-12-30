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

#ifndef VIDEO_H
#define VIDEO_H

#include "svin.h"

void print_screen_mode(_svin_screen_mode_t screenmode);
double get_screen_square_pixel_ratio(_svin_screen_mode_t screenmode);
_svin_screen_mode_t next_screen_mode(_svin_screen_mode_t screenmode);
_svin_screen_mode_t prev_screen_mode(_svin_screen_mode_t screenmode);
int get_screenmode_number(_svin_screen_mode_t screenmode);
_svin_screen_mode_t create_screenmode_by_number(vdp2_tvmd_tv_standard_t colorsystem, int number);

#endif /* !VIDEO_H */
