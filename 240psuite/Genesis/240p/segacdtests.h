/* 
 * 240p Test Suite
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

#include "genesis.h"
#include "types.h"
#include "main.h"

#define MEMORY_OK	0xFFFFFFFF
#define	IS_8BIT		0
#define	IS_16BIT	1

enum SCD_Command{
		Op_Null = 0,			// Null Operation
#ifdef SEGACD
		Op_LoadBootFile, 		//Load File (from ISO9660 filesystem)
#else
		Op_InitISOFS, 			//Init ISO9660 filesystem
#endif
		Op_GetWordRAM,			//Give WordRAM to Main CPU
        Op_InitCD,		    	//Init
        Op_SeekCDMDF,		    //Seek CD-DA Track 2 for MDFourier
		Op_UnPauseCD,		    //Play (unpause) CD-DA Track
		Op_StopCD,		    	//Stop CD-DA Track
		Op_RemovePauseLimit, 	//Removes the pause limit for MDF wait 
		Op_ResetPauseLimit,		//Resets the pause limit after MDF 
		Op_PlayCDMDF,		    //Play CD-DA Track 2 for the MDFourier Debug
		Op_PlayCD240,		    //Play CD-DA Track 3 for the Suite
		Op_PlayCDDA,    		//Play CD-DA Track from param
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
		Op_SetSampSin32604,		//Use 32604hz 1khz sample
		Op_CheckPCMRAM,			//Check full PCM RAM
		Op_LoadPCMRAM,			//Load sample into PCM ram from cartridge
		Op_CDTracks,			//Query Number of Tracks
		Op_DriveVersion,		//Query Drive Version
		Op_DummyTest			//Dummy Test Command
	}; 


int DetectSCDviaExpansion();
void SegaCDMenu();
void Z80RamTest();

void PCMRAMCheck();
void CRC32_reset();
void CRC32_update(uint8_t data);
uint32_t CRC32_finalize();
void MemViewer(uint32_t address);

// These change addressed with the SEGACD define
void SendSCDCommand(enum SCD_Command command);
u16 SendSCDCommandRetVal(enum SCD_Command command, u16 param, u16 *extraData);

#ifdef SEGACD
void PlayCDTrack();
void PlayCDTrackTimed();
void PlayPCM(int barrier);
void TestPCM(int barrier);
void ChangePCM(int *type);
#else
void PCMCartPlay();
#endif

// These are defined in kos.s
short set_sr(short new_sr);
void Kos_Decomp(volatile uint8_t *src, uint8_t *dst);
uint8_t segacd_init();
void resetSegaCD();
void vblank_scd();
extern void write_byte(unsigned int dst, unsigned char val);
extern void write_word(unsigned int dst, unsigned short val);
extern void write_long(unsigned int dst, unsigned int val);
extern unsigned char read_byte(unsigned int src);
extern unsigned short read_word(unsigned int src);
extern unsigned int read_long(unsigned int src);


