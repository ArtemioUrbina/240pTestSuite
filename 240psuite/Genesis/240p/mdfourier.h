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

#include "genesis.h"
#include "myvdp.h"
#include "res.h"
#include "tests.h"
#include "help.h"
#include "main.h"

#define NUMPITCHES 			12
#define PULSE_TRAIN_FREQ 	8820

#define STEREO_RIGHT 0x40
#define STEREO_LEFT 0x80
#define STEREO_BOTH 0xC0

extern const u16 PITCHES[NUMPITCHES];

void Z80Lock();
void Z80Release();
void ymGrandPianoLoad(u8 channel);
void ymInstrumentLoad(u8 channel);
void ym2612_keyoff(u8 channel);
void ym2612_keyoffAll();
void ym2612_disableDAC();
void yminit();
void ymPlay(u8 channel, u8 note, u8 octave, u8 pan);
void ExecutePulseTrain();
void ExecuteSilence();
void ExecuteFM(u16 framelen);
void ExecuteFMRamp();
void ExecutePSG(u16 framelen);
void ExecutePSGRamp();
void ExecuteNoise(u16 framelen);
void ExececuteMDF(u16 framelen);
void MDFourier(u8 armedAlert);
