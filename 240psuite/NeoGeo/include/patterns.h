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

#ifndef _PATTERNS_H_
#define _PATTERNS_H_

void tp_pluge(void);
void tp_colorchart(void);
void tp_colorbars(void);
void tp_smpte_color_bars(void);
void tp_ref_color_bars(void);
void tp_color_bleed_check(void);
void tp_grid(void);
void tp_monoscope(void);
void tp_gray_ramp(void);
void tp_white_rgb(void);
void tp_100_ire(void);
void tp_sharpness(void);
void tp_overscan(void);
void tp_convergence(void);

#endif /* _PATTERNS_H_ */