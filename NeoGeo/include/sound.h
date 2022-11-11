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

#ifndef _SOUND_H_
#define _SOUND_H_

// Sounds...
#define SOUNDCMD_PlayJingleA	0x02
#define SOUNDCMD_PlayCoinA		0x20
#define SOUNDCMD_PlayLeft  		0x21
#define SOUNDCMD_PlayRight		0x22
#define SOUNDCMD_PlayCenter 	0x23
#define SOUNDCMD_StopADPCMA		0x30
#define SOUNDCMD_ADPCMB_Left	0x31
#define SOUNDCMD_ADPCMB_Center	0x32
#define SOUNDCMD_ADPCMB_Right	0x33
#define SOUNDCMD_StopADPCMB		0x34
#define SOUNDCMD_LoopB			0x35
#define SOUNDCMD_NoLoopB		0x36
#define SOUNDCMD_ADPCMB_Sample0 0x37
#define SOUNDCMD_ADPCMB_Sample1 0x38

#define SOUNDCMD_SSGRampinit	0x40
#define SOUNDCMD_SSGRampcycle	0x41
#define SOUNDCMD_SSGPulseStart	0x42
#define SOUNDCMD_SSG1KHZStart	0x43
#define SOUNDCMD_SSG260HZStart	0x44
#define SOUNDCMD_SSGPulseStop	0x4f

#define SOUNDCMD_RateB_0		0x80
#define SOUNDCMD_RateB_1		0x81
#define SOUNDCMD_RateB_2		0x82
#define SOUNDCMD_RateB_3		0x83
#define SOUNDCMD_RateB_4		0x84
#define SOUNDCMD_RateB_5		0x85
#define SOUNDCMD_RateB_6		0x86
#define SOUNDCMD_RateB_7		0x87

#define	RAMTESTCMD				0x10

#ifdef __cd__
#define CDDA_MDFOURIER			0x02
#define CDDA_SNDTEST			0x03
#endif

#define Z80COMMAND_OK(cmd)		(cmd|0x80)
#define Z80COMMAND_FAIL(cmd)		(cmd)
#define Z80COMMAND_TIME(cmd)		(cmd == 0)

#endif /* _SOUND_H_ */