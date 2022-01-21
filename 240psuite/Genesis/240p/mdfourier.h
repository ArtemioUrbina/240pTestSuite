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
#ifdef SEGACD
enum SCD_Command{
		Op_Null = 0,			// Null Operation
		Op_LoadBootFile, 		//Load File (from ISO9660 filesystem)
		Op_GetWordRAM,			//Give WordRAM to Main CPU
        Op_InitCD,		    	//Init
        Op_SeekCDMDF,		    //Seek CD-DA Track 2 for MDFourier
		Op_UnPauseCD,		    //Play (unpause) CD-DA Track
		Op_StopCD,		    	//Stop CD-DA Track
		Op_RemovePauseLimit, 	//Removes the pause limit for MDF wait 
		Op_ResetPauseLimit,		//Resets the pause limit after MDF 
		Op_PlayCDMDF,		    //Play CD-DA Track 2 for the MDFourier Debug
		Op_PlayCD240,		    //Play CD-DA Track 3 for the Suite
		Op_PlayPCM,				//Play Full PCM Memory
		Op_StopPCM,				//Stop PCM Playback
		Op_SetPCMLeft, 			//Set PCM for Left Channel
		Op_SetPCMRight, 		//Set PCM for Right Channel
		Op_SetPCMCenter, 		//Set PCM for Both Channels
		Op_IncremetPCMFreq, 	//Increment the internal value by 1
		Op_DecrementPCMFreq, 	//Decrement the internal value by 1
		Op_TestPCM, 			//Test PCM Frequencies
		Op_SetSamplesSweep,		//Back to normal samples for sweep
		Op_SetSamplesTest,		//Sound Test PCM 
		Op_SetSamplesTest2,		//Sound Test PCM 2
		Op_SetSampSin32000,		//Use 32000hz 1khz sample
		Op_SetSampSin32552,		//Use 32552hz 1khz sample
		Op_SetSampSin32604		//Use 32604hz 1khz sample
	}; 
void SendSCDCommand(enum SCD_Command command);
void PlayCDTrack();
void PlayCDTrackTimed();
void PlayPCM(int barrier);
void TestPCM(int barrier);
void ChangePCM(int *type);
#endif
void ExececuteMDF(u16 framelen);
void MDFourier(u8 armedAlert);
