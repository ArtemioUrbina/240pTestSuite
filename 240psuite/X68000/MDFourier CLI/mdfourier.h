/* 
 * 240p Test Suite
 * Copyright (C)2011-2014 Artemio Urbina
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

#include "types.h"

#define STEREO_RIGHT		0x80
#define STEREO_LEFT			0x40
#define STEREO_BOTH			0xC0

extern int video_count;

void ym2151_keyoff(u8 channel);
void ym2151_keyoffAll();
void yminit();
void ymloadchannel(u8 channel);
void ymPlay(u8 channel, u8 note, u8 octave, u8 pan);
void ExecutePulseTrain(u8 channel);
void ExecuteSilence();
int ExecuteFM(u16 framelen);
void ExecuteMDF(u16 framelen, u8 *adpcm, u16 adpcm_size);
void ExecuteADPCMOnly(u8 *adpcm, u16 adpcm_size);
