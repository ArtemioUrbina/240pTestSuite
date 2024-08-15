/* 
 * 240p Test Suite for Nintendo 64
 * Copyright (C)2024 Artemio Urbina
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

// Measured with scope and get_ticks_us() 
#define N64_NTSC_FRAME_LEN 16.715f

#define SUITE_320x240	0
#define SUITE_640x480	1
#define SUITE_256x240	3
#define SUITE_512x480	4
#define SUITE_512x240	5

#define SUITE_NONE		-1

extern surface_t* __disp;

extern resolution_t		current_resolution;
extern bitdepth_t		current_bitdepth;
extern unsigned int		current_buffers;
extern unsigned int		current_gamma;
extern filter_options_t	current_antialias;
extern rdpq_antialias_t current_rdp_aa_filter;
extern unsigned int		enablePAL60;
extern unsigned int		vMode;

int is50Hz();

void getDisplay();
void waitVsync();
void waitVsyncWithAudio();
void drawNoVsyncWithAudio();

int getHardWidth();
int getHardHeight();

void initVideo();
void changeVMode(resolution_t newRes);

int isSameRes(resolution_t *res1, const resolution_t *res2);
int videoModeToInt(resolution_t *res);
void getVideoModeStr(char *res, int shortdesc);
void changeToH256onVBlank();
void changeToH320onVBlank();
void changeTo16BitDepthOnVBlank();
void changeTo32BitDepthOnVBlank();
int isVMode256();
int isVMode480();

void setVideoInternal(resolution_t newRes);

#ifdef DEBUG_BENCHMARK
#define	IDLE_WARN_MS	2.0f
void resetIdle();
#endif

#endif