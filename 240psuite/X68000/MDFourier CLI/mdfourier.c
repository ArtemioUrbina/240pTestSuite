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

#include <stdio.h>
#include "mdfourier.h"
#include "ym2151.h"
#include "MSM6258.h"
#include "video.h"
#include "key.h"

int video_count = 0;
int adcpm_dma_frames = 0;
#ifdef COMPENSATE_FASTER_ADPCM
int adcpm_dma_frames_extra = 0;
#endif

#define ADPCM_FRAME_LEN	278
#define MDF_FRAME_LEN	2924

void SilenceMDF()
{
	ym2151_keyoffAll();
}

void ym2151_keyoff(u8 channel)
{	
	YM2151_writeReg(0x08, channel &0x07);
}

void ym2151_keyoffAll()
{
	u8 c = 0;

	for(c = 0; c < 8; c ++)
		ym2151_keyoff(c);
}

void ymloadchannel(u8 channel)
{
//0x38 Phase & amplitude modulation sensitivity PMS: 0x00 AMS: 0x00
	YM2151_writeReg(0x38+channel, 0x00);
//0x40 Detune & phase multiply Slot: 0000 Decay: 0x00 Multi: 0x04
	YM2151_writeReg(0x40+channel, 0x04);
//0x48 Detune & phase multiply Slot: 0001 Decay: 0x00 Multi: 0x04
	YM2151_writeReg(0x48+channel, 0x04);
//0x50 Detune & phase multiply Slot: 0010 Decay: 0x00 Multi: 0x04
	YM2151_writeReg(0x50+channel, 0x04);
//0x58 Detune & phase multiply Slot: 0011 Decay: 0x00 Multi: 0x04
	YM2151_writeReg(0x58+channel, 0x04);
//0x60 Total level Slot: 0000 Volume: 0x0000
	YM2151_writeReg(0x60+channel, 0x00);
//0x68 Total level Slot: 0001 Volume: 0x0000
	YM2151_writeReg(0x68+channel, 0x00);
//0x70 Total level Slot: 0010 Volume: 0x0000
	YM2151_writeReg(0x70+channel, 0x00);
//0x78 Total level Slot: 0011 Volume: 0x0000
	YM2151_writeReg(0x78+channel, 0x00);
//0x80 EG Attack Slot: 0000 Keyscale: 01 Attack rate: 0x01F
	YM2151_writeReg(0x80+channel, 0x5F);
//0x88 EG Attack Slot: 0001 Keyscale: 00 Attack rate: 0x01F
	YM2151_writeReg(0x88+channel, 0x1F);
//0x90 EG Attack Slot: 0010 Keyscale: 00 Attack rate: 0x01F
	YM2151_writeReg(0x90+channel, 0x1F);
//0x98 EG Attack Slot: 0011 Keyscale: 00 Attack rate: 0x01F
	YM2151_writeReg(0x98+channel, 0x1F);
//0xA0 EG Decay 1 Slot: 0000 Amplitude modulation Sensitivity: N First decay rate: 0x01F
	YM2151_writeReg(0xA0+channel, 0x1F);
//0xA8 EG Decay 1 Slot: 0001 Amplitude modulation Sensitivity: N First decay rate: 0x01F
	YM2151_writeReg(0xA8+channel, 0x1F);
//0xB0 EG Decay 1 Slot: 0010 Amplitude modulation Sensitivity: N First decay rate: 0x01F
	YM2151_writeReg(0xB0+channel, 0x1F);
//0xB8 EG Decay 1 Slot: 0011 Amplitude modulation Sensitivity: N First decay rate: 0x01F
	YM2151_writeReg(0xB8+channel, 0x1F);
//0xC0 EG Decay 2 Slot: 0000 Detune(2): 0x0 Second decay rate: 0x00
	YM2151_writeReg(0xC0+channel, 0x00);
//0xC8 EG Decay 2 Slot: 0001 Detune(2): 0x0 Second decay rate: 0x00
	YM2151_writeReg(0xC8+channel, 0x00);
//0xD0 EG Decay 2 Slot: 0010 Detune(2): 0x0 Second decay rate: 0x00
	YM2151_writeReg(0xD0+channel, 0x00);
//0xD8 EG Decay 2 Slot: 0011 Detune(2): 0x0 Second decay rate: 0x00
	YM2151_writeReg(0xD8+channel, 0x00);
//0xE0 EG Decay level, Release rate Slot: 0000 First Decay: 0x00 Release rate: 0x0F
	YM2151_writeReg(0xE0+channel, 0x0F);
//0xE8 EG Decay level, Release rate Slot: 0001 First Decay: 0x00 Release rate: 0x0F
	YM2151_writeReg(0xE8+channel, 0x0F);
//0xF0 EG Decay level, Release rate Slot: 0010 First Decay: 0x00 Release rate: 0x0F
	YM2151_writeReg(0xF0+channel, 0x0F);
//0xF8 EG Decay level, Release rate Slot: 0011 First Decay: 0x00 Release rate: 0x0F
	YM2151_writeReg(0xF8+channel, 0x0F);
//0x20 L: Y R: Y Feedback: 000 Connection: 111
	YM2151_writeReg(0x20+channel, 0xC7);
//0x28 Octave:0x06 Note: 0x0C
	YM2151_writeReg(0x28+channel, 0x6C);
}

void yminit()
{
	u8 c = 0;
	
	YM2151_reset();

//0x01 Test LFO Reset
	YM2151_writeReg(0x01, 0x02);
//0x0F Noise disable Freq: 0x000
	YM2151_writeReg(0x0F, 0x00);
//0x14 Clock Functions CSM OFF F-Reset: 00 IRQEN: 00 LOAD: 00
	YM2151_writeReg(0x14, 0x00);
//0x18 LOW OSCILLATION FREQUENCY 0x0000
	YM2151_writeReg(0x18, 0x00);
//0x19 PHS OR AMP MODULATION DEPTH 0x0000
	YM2151_writeReg(0x19, 0x00);
//0x1B Disk State OFF CT: 00 (4mhz) Waveform: 01 (square)
	YM2151_writeReg(0x1B, 0x01);

	for(c = 0; c < 8; c++)
		ymloadchannel(c);
}


void ymPlay(u8 channel, u8 note, u8 octave, u8 pan)
{
	ym2151_keyoff(channel);

	// set stereo balance Feedback: 000 Connection: 111
	YM2151_writeReg(0x20+channel, pan|0x07);

	//Register 0x28 - frequency
	YM2151_writeReg(0x28+channel,(octave << 4) | note);
	
	// Register 0x08 - Key on
	YM2151_writeReg(0x08, 0x78+channel);

}

void adpcm_dma_wait_complete_vblank()
{
	u8						count = 0;
	vu8						*adpcm_status;
	volatile struct DMAREG 	*dma;

	dma = (struct DMAREG *)0xe840c0;
	adpcm_status = (u8*)0xe92001;

	adcpm_dma_frames = 0;
	while(!(dma->csr & 0x90) && ! (*adpcm_status & 0x80))
	{
		wait_vblank();
		adcpm_dma_frames++;
	}
	
#ifdef COMPENSATE_FASTER_ADPCM
	if(adcpm_dma_frames < ADPCM_FRAME_LEN)
	{
		for(count = 0; count < ADPCM_FRAME_LEN - adcpm_dma_frames; count++)
		{
			wait_vblank();
			adcpm_dma_frames_extra++;
		}
	}
#endif
}

void ExecutePulseTrain(u8 channel)
{
	u16 f = 0;
	//Sync
	
	for(f = 0; f < 10; f++)
	{
		YM2151_writeReg(0x08, 0x78+channel);
		wait_vblank();
		ym2151_keyoff(channel);
		wait_vblank();
	}
}

void ExecuteSilence()
{
	u16 frame = 0;
	
	//Silence
	for(frame = 0; frame < 20; frame++)
		wait_vblank();
}

int ExecuteFM(u16 framelen)
{
	int octave, frame;
	
	// FM Test
	for(octave = 0; octave < 8; octave ++)
	{
		int chann = 0, note;
		
		for(note = 0; note < 16; note++)
		{
			ymPlay(chann, note, octave, STEREO_LEFT);
			ymPlay(chann+4, note, octave, STEREO_RIGHT);
			
			for(frame = 0; frame < framelen; frame++)
			{					
				if(frame == framelen - framelen/5)
				{
					ym2151_keyoff(chann);
					ym2151_keyoff(chann+4);
				}
				if(read_input() == -1)
					return 0;
				wait_vblank();
			}
			chann ++;
			if(chann > 3)
				chann = 0;
		}
	}

	ym2151_keyoffAll();
	return 1;
}

void ExecuteADPCM()
{
	wait_vblank();

	adpcm_dma_start();
	adpcm_start();
	adpcm_dma_wait_complete_vblank();
	adpcm_stop();
	adpcm_dma_clear_flag();

	wait_vblank();
}

void MDFSequence(u16 framelen, u8 *adpcm, u16 adpcm_size)
{
	yminit();
	Prepare_ADPCM_DMA(adpcm, adpcm_size);
	wait_vblank();	
	video_count = 0;
	adcpm_dma_frames = 0;
#ifdef COMPENSATE_FASTER_ADPCM
	adcpm_dma_frames_extra = 0;
#endif

	ExecutePulseTrain(0);
	ExecuteSilence();
	
	if(!ExecuteFM(framelen))
		return;
	/* wait for four frames */
	for(int wait = 0; wait < 4; wait++)
		wait_vblank();
	ExecuteADPCM();

	/* reset frequency for tones channel 0*/
	YM2151_writeReg(0x20, 0xC7);
	YM2151_writeReg(0x28, 0x6C);
	
	ExecuteSilence();
	ExecutePulseTrain(0);
	
	printf("ADPCM DMA playback frames: %d\n", adcpm_dma_frames);
	printf("Total test vsync count %d\n", video_count);
	
	if(adcpm_dma_frames < ADPCM_FRAME_LEN)
	{
		printf("WARNING: ADPCM DMA playback was faster than the expected %d frames\n", ADPCM_FRAME_LEN);
#ifdef COMPENSATE_FASTER_ADPCM
		printf("  Compensated with %d extra silent frames, they won't align properly\n", adcpm_dma_frames_extra);
#endif
	}
	
	if(adcpm_dma_frames > ADPCM_FRAME_LEN)
		printf("WARNING: ADPCM DMA playback took longer than the expected %d frames\n", ADPCM_FRAME_LEN);
		
	if(video_count != MDF_FRAME_LEN)
		printf("WARNING: Vsync count didn't match expected %d frames\n", MDF_FRAME_LEN);
}

void ExecuteMDF(u16 framelen, u8 *adpcm, u16 adpcm_size)
{
	MDFSequence(framelen, adpcm, adpcm_size);
	SilenceMDF();
}

void ExecuteADPCMOnly(u8 *adpcm, u16 adpcm_size)
{
	yminit();
	Prepare_ADPCM_DMA(adpcm, adpcm_size);
	wait_vblank();
	
	video_count = 0;
	adcpm_dma_frames = 0;
#ifdef COMPENSATE_FASTER_ADPCM
	adcpm_dma_frames_extra = 0;
#endif

	ExecuteADPCM();

	printf("ADPCM DMA playback frames: %d\n", adcpm_dma_frames);
	
	if(adcpm_dma_frames == ADPCM_FRAME_LEN)
		printf("ADPCM DMA playback lasted the expected frame duration\n");
		
	if(adcpm_dma_frames < ADPCM_FRAME_LEN)
	{
		printf("WARNING: ADPCM DMA playback was faster than the expected %d frames\n", ADPCM_FRAME_LEN);
#ifdef COMPENSATE_FASTER_ADPCM
		printf("  Compensated with %d extra silent frames, they won't align properly\n", adcpm_dma_frames_extra);
#endif
	}
	
	if(adcpm_dma_frames > ADPCM_FRAME_LEN)
		printf("WARNING: ADPCM DMA playback took longer than the expected %d frames\n", ADPCM_FRAME_LEN);
}

