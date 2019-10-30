 /*
 * 240p Test Suite
 * Copyright (C)2014-2019 Artemio Urbina (PC Engine/TurboGrafx-16)
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
 
 
 This version of the suite is compiled with HuC from https://github.com/uli/huc
 
 */


const unsigned char *psg_ch = 0x800;
const unsigned char *psg_bal = 0x801;
const unsigned char *psg_freqlo = 0x802;
const unsigned char *psg_freqhi = 0x803;
const unsigned char *psg_ctrl = 0x804;
const unsigned char *psg_chbal = 0x805;
const unsigned char *psg_data = 0x806;
const unsigned char *psg_noise = 0x807;
const unsigned char *psg_lfofreq = 0x808;
const unsigned char *psg_lfoctrl = 0x809;

#define PULSE_TRAIN_FREQ 	13
#define PULSE_INTERNAL_FREQ	9
#define PULSE_SKIP_EMU		4


/*
http://ppmck.web.fc2.com/wavetable_js.html
*/

const unsigned char sine1x[32] = {	0x11, 0x14, 0x17, 0x1a, 0x1c, 0x1e, 0x1f, 0x1f,
									0x1f, 0x1f, 0x1e, 0x1c, 0x1a, 0x17, 0x14, 0x11,
									0x0e, 0x0b, 0x08, 0x05, 0x03, 0x01, 0x00, 0x00,
									0x00, 0x00, 0x01, 0x03, 0x05, 0x08, 0x0b, 0x0e };
/*									
const unsigned char sine2x[32] = {	0x13, 0x18, 0x1d, 0x1f, 0x1f, 0x1d, 0x18, 0x13, 
									0x0c, 0x07, 0x02, 0x00, 0x00, 0x02, 0x07, 0x0c, 
									0x13, 0x18,	0x1d, 0x1f, 0x1f, 0x1d, 0x18, 0x13,
									0x0c, 0x07, 0x02, 0x00, 0x00, 0x02, 0x07, 0x0c };
*/
	
const unsigned char sine4x[32] = {	0x16, 0x1e, 0x1e, 0x16, 0x09, 0x01, 0x01, 0x09, 
									0x16, 0x1e, 0x1e, 0x16, 0x09, 0x01, 0x01, 0x09,
									0x16, 0x1e, 0x1e, 0x16, 0x09, 0x01, 0x01, 0x09,
									0x16, 0x1e, 0x1e, 0x16, 0x09, 0x01, 0x01, 0x09 };
/*
const unsigned char sqstep[32] = { 	0x18, 0x1C, 0x1C, 0x1C,	0x00, 0x1C, 0x04, 0x1C,
									0x08, 0x1C, 0x0C, 0x1C,	0x10, 0x1C, 0x14, 0x1C,
									0x18, 0x1C, 0x1C, 0x1C,	0x00, 0x1C, 0x00, 0x1C,
									0x00, 0x1C, 0x00, 0x1C,	0x04, 0x1C, 0x08, 0x1C };				
			
const unsigned char square[32] = {	0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 
									0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
*/
/*			
const unsigned char tiangle[32] = {	0x01, 0x03, 0x05, 0x07, 0x09, 0x0b, 0x0d, 0x0f, 
									0x11, 0x13, 0x15, 0x17, 0x19, 0x1b, 0x1d, 0x1f,
									0x1f, 0x1d, 0x1b, 0x19, 0x17, 0x15, 0x13, 0x11,
									0x0f, 0x0d, 0x0b, 0x09, 0x07, 0x05, 0x03, 0x01 };
			
const unsigned char saw[32] = {		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
									0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
									0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
									0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };
			
const unsigned char sinesaw[32] = {	0x10, 0x13, 0x16, 0x18, 0x1B, 0x1D, 0x1E, 0x1F,
									0x1F, 0x1F, 0x1E, 0x1D, 0x1B, 0x18, 0x16, 0x13,
									0x01, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E,
									0x10, 0x12, 0x14, 0x16, 0x18, 0x1A, 0x1C, 0x1E };
*/

void LoadWave(unsigned char chan, unsigned char *wave)
{	
	__sei();
	*psg_ch = chan;
	*psg_ctrl = 0;
	for(i = 0; i < 32; i++)
	{
		*psg_data = *wave;
		wave++;
	}
	*psg_bal = 0xff;
	*psg_noise = 0;
	__cli();
}

void PlayLeft(unsigned char chan)
{
	__sei();
	*psg_ch = chan;
	*psg_chbal = 0xf0;
	*psg_ctrl = 0x9f;
	__cli();
}

void PlayRight(unsigned char chan)
{
	__sei();
	*psg_ch = chan;
	*psg_chbal = 0x0f;
	*psg_ctrl = 0x9f;
	__cli();
}


void PlayCenter(unsigned char chan)
{
	__sei();
	*psg_ch = chan;
	*psg_chbal = 0xff;
	*psg_ctrl = 0x9f;
	__cli();
}

void StopAudio(unsigned char chan)
{
	__sei();
	*psg_ch = chan;
	*psg_ctrl = 0;
	__cli();
}

void StopAllAudio()
{
	for(i = 0; i < 6; i++)
		StopAudio(i);
}


/*

                       3580000
 12 bit value = ---------------------
                 32 x frequency (Hz)
				 
12 as value -> 9322.91Hz
13 as value -> 8605.76Hz

2044 as value -> 54.73Hz
4    as value -> 27968.75Hz
*/


void SetWaveFreq(unsigned char chan, unsigned int freq)
{
	__sei();
	*psg_ch = chan;
	*psg_freqlo = freq & 0xff;
	*psg_freqhi = freq >> 8;
	__cli();
}

void SetNoiseFreq(unsigned int chan, unsigned int freq)
{
	__sei();
	*psg_ch = chan;
	*psg_noise = 0x80 | (freq & 0x1F)^0x1F;
	__cli();
}

void StopNoise(unsigned int chan)
{
	__sei();
	*psg_ch = chan;
	*psg_noise = 0;
	*psg_ctrl = 0;
	__cli();
}

void ExecutePulseTrain(unsigned int chann)
{
	//Sync
	
	SetWaveFreq(chann, PULSE_TRAIN_FREQ);
	for(i = 0; i < 10; i++)
	{
		PlayCenter(chann);
		vsync();
		StopAudio(chann);
		vsync();
	}
}

void ExecuteSilence()
{
	//Silence
	for(i = 0; i < 20; i++)
		vsync();
}

void PlayRampChannel(int chann)
{
	//54Hz to 22375Hz
	PlayCenter(chann);
	for(i = 2044; i > 4; i-=6)
	{
		SetWaveFreq(chann, i);
		vsync();
	}
	StopAudio(chann);
}

/*
void PlayBothRampChannel(int chann1, int chann2)
{
	//54Hz to 22375Hz
	PlayLeft(chann1);
	PlayRight(chann2);
	for(i = 2044; i > 4; i-=6)
	{
		SetWaveFreq(chann1, i);
		SetWaveFreq(chann2, i);
		vsync();
	}
	StopAudio(chann1);
	StopAudio(chann2);
}
*/