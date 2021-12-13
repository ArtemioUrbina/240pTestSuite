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
#include "adpcm_sweep.h"

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

void yminit()
{
	YM2151_reset();

//0x01 Test LFO Reset
	YM2151_writeReg(0x01, 0x02);
//0x0F Noise disable Freq: 0x000
	YM2151_writeReg(0x0F, 0x00);
//0x14 Clock Functions CSM OFF F-Reset: 00 IRQEN: 00 LOAD: 00
	YM2151_writeReg(0x14, 0x00);
//0x18 LOW OSCILLATION FREQUENCY 0x00BC
	YM2151_writeReg(0x18, 0x00);
//0x19 PHS OR AMP MODULATION DEPTH 0x0000
	YM2151_writeReg(0x19, 0x00);
//0x1B Disk State OFF CT: 00 (4mhz) Waveform: 01 (square)
	YM2151_writeReg(0x1B, 0x01);
//0x39 Channel: 1 Phase & amplitude modulation sensitivity PMS: 0x00 AMS: 0x00
	YM2151_writeReg(0x39, 0x00);
//0x41 Channel: 1 Detune & phase multiply Slot: 0000 Decay: 0x00 Multi: 0x00
	YM2151_writeReg(0x41, 0x04);
//0x49 Channel: 1 Detune & phase multiply Slot: 0001 Decay: 0x00 Multi: 0x00
	YM2151_writeReg(0x49, 0x04);
//0x51 Channel: 1 Detune & phase multiply Slot: 0010 Decay: 0x00 Multi: 0x00
	YM2151_writeReg(0x51, 0x04);
//0x59 Channel: 1 Detune & phase multiply Slot: 0011 Decay: 0x02 Multi: 0x00
	YM2151_writeReg(0x59, 0x04);
//0x61 Channel: 1 Total level Slot: 0000 Volume: 0x0020
	YM2151_writeReg(0x61, 0x00);
//0x69 Channel: 1 Total level Slot: 0001 Volume: 0x0020
	YM2151_writeReg(0x69, 0x00);
//0x71 Channel: 1 Total level Slot: 0010 Volume: 0x0020
	YM2151_writeReg(0x71, 0x00);
//0x79 Channel: 1 Total level Slot: 0011 Volume: 0x000D
	YM2151_writeReg(0x79, 0x00);
//0x81 Channel: 1 EG Attack Slot: 0000 Keyscale: 01 Attack rate: 0x01F
	YM2151_writeReg(0x81, 0x5F);
//0x89 Channel: 1 EG Attack Slot: 0001 Keyscale: 00 Attack rate: 0x01F
	YM2151_writeReg(0x89, 0x1F);
//0x91 Channel: 1 EG Attack Slot: 0010 Keyscale: 00 Attack rate: 0x01F
	YM2151_writeReg(0x91, 0x1F);
//0x99 Channel: 1 EG Attack Slot: 0011 Keyscale: 00 Attack rate: 0x01F
	YM2151_writeReg(0x99, 0x1F);
//0xA1 Channel: 1 EG Decay 1 Slot: 0000 Amplitude modulation Sensitivity: N First decay rate: 0x01F
	YM2151_writeReg(0xA1, 0x1F);
//0xA9 Channel: 1 EG Decay 1 Slot: 0001 Amplitude modulation Sensitivity: N First decay rate: 0x01F
	YM2151_writeReg(0xA9, 0x1F);
//0xB1 Channel: 1 EG Decay 1 Slot: 0010 Amplitude modulation Sensitivity: N First decay rate: 0x01F
	YM2151_writeReg(0xB1, 0x1F);
//0xB9 Channel: 1 EG Decay 1 Slot: 0011 Amplitude modulation Sensitivity: N First decay rate: 0x01F
	YM2151_writeReg(0xB9, 0x1F);
//0xC1 Channel: 1 EG Decay 2 Slot: 0000 Detune(2): 0x0 Second decay rate: 0x00
	YM2151_writeReg(0xC1, 0x00);
//0xC9 Channel: 1 EG Decay 2 Slot: 0001 Detune(2): 0x0 Second decay rate: 0x00
	YM2151_writeReg(0xC9, 0x00);
//0xD1 Channel: 1 EG Decay 2 Slot: 0010 Detune(2): 0x0 Second decay rate: 0x00
	YM2151_writeReg(0xD1, 0x00);
//0xD9 Channel: 1 EG Decay 2 Slot: 0011 Detune(2): 0x0 Second decay rate: 0x00
	YM2151_writeReg(0xD9, 0x00);
//0xE1 Channel: 1 EG Decay level, Release rate Slot: 0000 First Decay: 0x00 Release rate: 0x0F
	YM2151_writeReg(0xE1, 0x0F);
//0xE9 Channel: 1 EG Decay level, Release rate Slot: 0001 First Decay: 0x00 Release rate: 0x0F
	YM2151_writeReg(0xE9, 0x0F);
//0xF1 Channel: 1 EG Decay level, Release rate Slot: 0010 First Decay: 0x00 Release rate: 0x0F
	YM2151_writeReg(0xF1, 0x0F);
//0xF9 Channel: 1 EG Decay level, Release rate Slot: 0011 First Decay: 0x00 Release rate: 0x0F
	YM2151_writeReg(0xF9, 0x0F);
//0x21 Channel: 1 L: Y R: Y Feedback: 0100 Connection: 0100
	YM2151_writeReg(0x21, 0xC7);
//0x29 Channel: 1 Octave:0x07 Note: 0x0E
	YM2151_writeReg(0x29, 0x6C);

}

void ymPlay(u8 channel, u8 note, u8 octave, u8 pan)
{
	ym2151_keyoff(1);

	// set stereo balance
	//YM2151_writeReg(0x20+1, pan+0x07);

	//Register 0x28 - frequency
	YM2151_writeReg(0x28+1,(octave << 4) | note);
	
	// Register 0x08 - Key on
	YM2151_writeReg(0x08, 0x79);

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

void ExecutePulseTrain()
{
	u16 f = 0;
	//Sync
	
	for(f = 0; f < 10; f++)
	{
		YM2151_writeReg(0x08, 0x79);
		wait_vblank();
		ym2151_keyoff(1);
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

void ExecuteFM(u16 framelen)
{
	int octave, frame;
	
	// FM Test
	for(octave = 0; octave < 8; octave ++)
	{
		int chann = 0, note;
		
		for(note = 0; note < 16; note++)
		{
			ymPlay(chann, note, octave, STEREO_LEFT);
			//ymPlay(chann+3, note, octave, STEREO_RIGHT);
			
			for(frame = 0; frame < framelen; frame++)
			{					
				if(frame == framelen - framelen/5)
				{
					ym2151_keyoff(1);
					//ym2151_keyoff(chann+3);
				}
				wait_vblank();
			}
			chann ++;
			if(chann > 2)
				chann = 0;
		}
	}

	ym2151_keyoffAll();
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

void MDFSequence(u16 framelen)
{
	yminit();
	Prepare_ADPCM_DMA(adpcm_sweep, 39061);
	wait_vblank();	
	video_count = 0;
	adcpm_dma_frames = 0;
#ifdef COMPENSATE_FASTER_ADPCM
	adcpm_dma_frames_extra = 0;
#endif

	ExecutePulseTrain();
	ExecuteSilence();
	
	ExecuteFM(framelen);
	/* wait for four frames */
	for(int wait = 0; wait < 4; wait++)
		wait_vblank();
	ExecuteADPCM();

	/* reset frequency for tones */
	YM2151_writeReg(0x21, 0xC7);
	YM2151_writeReg(0x29, 0x6C);
	
	ExecuteSilence();
	ExecutePulseTrain();
	
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

void ExecuteMDF(u16 framelen)
{
	MDFSequence(framelen);
	SilenceMDF();
}

void ExecuteADPCMOnly()
{
	yminit();
	Prepare_ADPCM_DMA(adpcm_sweep, 39061);
	wait_vblank();
	
	video_count = 0;
	adcpm_dma_frames = 0;
#ifdef COMPENSATE_FASTER_ADPCM
	adcpm_dma_frames_extra = 0;
#endif

	ExecuteADPCM();

	printf("ADPCM DMA playback frames: %d\n", adcpm_dma_frames);
	
	if(adcpm_dma_frames == ADPCM_FRAME_LEN)
		printf("ADPCM DMA playback took the expected duration\n");
		
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

