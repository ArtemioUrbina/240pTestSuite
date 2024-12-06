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

typedef enum {
        VIDEO_SCANMODE_240I = 0,
        VIDEO_SCANMODE_240P = 1,
        VIDEO_SCANMODE_480I = 2,
        VIDEO_SCANMODE_480P = 3
} __packed video_scanmode_t;

typedef enum {
        VIDEO_X_RESOLUTION_320 = 0,
        VIDEO_X_RESOLUTION_352 = 1,
} __packed video_x_resolution_t;

typedef struct {
        video_scanmode_t scanmode;
        video_x_resolution_t x_res;
        vdp2_tvmd_vert_t y_res;
        bool x_res_doubled;
        vdp2_tvmd_tv_standard_t colorsystem;
} __packed video_screen_mode_t;

void video_init(video_screen_mode_t screen_mode, bool bmp_mode);
void video_deinit();
int video_is_inited();

char * scanmode_text_value(video_screen_mode_t screenmode);
void print_screen_mode(video_screen_mode_t screenmode);
double get_screen_square_pixel_ratio(video_screen_mode_t screenmode);
video_screen_mode_t next_screen_mode(video_screen_mode_t screenmode);
video_screen_mode_t prev_screen_mode(video_screen_mode_t screenmode);
int get_screenmode_number(video_screen_mode_t screenmode);
video_screen_mode_t create_screenmode_by_number(vdp2_tvmd_tv_standard_t colorsystem, int number);
int get_screenmode_resolution_x(video_screen_mode_t screenmode);
int get_screenmode_resolution_y(video_screen_mode_t screenmode);
bool is_screenmode_special(video_screen_mode_t screenmode);
void update_screen_mode(video_screen_mode_t screenmode, bool bmp_mode);

#endif /* !VIDEO_H */
