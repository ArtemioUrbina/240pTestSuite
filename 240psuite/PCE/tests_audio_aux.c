 /*
 * 240p Test Suite
 * Copyright (C)2014-2022 Artemio Urbina (PC Engine/TurboGrafx-16)
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

#define PSG_CH_ON	0x80
#define PSG_V_MAX	0x1f

const unsigned char *psg_ch 		= 0x800;
const unsigned char *psg_bal 		= 0x801;
const unsigned char *psg_freqlo 	= 0x802;
const unsigned char *psg_freqhi 	= 0x803;
const unsigned char *psg_ctrl 		= 0x804;
const unsigned char *psg_chbal 		= 0x805;
const unsigned char *psg_data 		= 0x806;
const unsigned char *psg_noise 		= 0x807;
const unsigned char *psg_lfofreq 	= 0x808;
const unsigned char *psg_lfoctrl 	= 0x809;

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

void PSG_LoadWave(unsigned char chan, unsigned char *wave)
{
	static unsigned char byte;
	
	__sei();
	*psg_ch = chan;
	*psg_ctrl = 0;
	for(byte = 0; byte < 32; byte++)
	{
		*psg_data = *wave;
		wave++;
	}
	*psg_bal = 0xff;
	*psg_noise = 0;
	__cli();
}

void PSG_PlayLeft(unsigned char chan)
{
	__sei();
	*psg_ch = chan;
	*psg_chbal = 0xf0;
	*psg_ctrl = 0x9f;	// *psg_ctrl = PSG_CH_ON|PSG_V_MAX;
	__cli();
}

void PSG_PlayRight(unsigned char chan)
{
	__sei();
	*psg_ch = chan;
	*psg_chbal = 0x0f;
	*psg_ctrl = 0x9f;	//*psg_ctrl = PSG_CH_ON|PSG_V_MAX;
	__cli();
}


void PSG_PlayCenter(unsigned char chan)
{
	__sei();
	*psg_ch = chan;
	*psg_chbal = 0xff;
	*psg_ctrl = 0x9f; 	//*psg_ctrl = PSG_CH_ON|PSG_V_MAX;
	__cli();
}


void PSG_StopAudio(unsigned char chan)
{
	__sei();
	*psg_ch = chan;
	*psg_chbal = 0;
	*psg_ctrl = 0;
	__cli();
}

void PSG_StopAllAudio()
{
	static unsigned char chan;
	
	__sei();
	for(chan = 0; chan < 6; chan++)
	{
		*psg_ch = chan;
		*psg_chbal = 0;
		*psg_ctrl = 0;
	}
	__cli();
}

void PSG_SetBalance(unsigned char chan, unsigned char left, unsigned char right, unsigned char vol)
{
	__sei();
	*psg_ch = chan;
	*psg_chbal = ((left&0x0f)<<4)|(right&0x0f);
	*psg_ctrl = PSG_CH_ON|(PSG_V_MAX&vol);
	__cli();
}

void PSG_SetGlobalBalance(unsigned char chan, unsigned char left, unsigned char right, unsigned char vol)
{
	__sei();
	*psg_ch = chan;
	*psg_bal = ((left&0x0f)<<4)|(right&0x0f);
	*psg_ctrl = PSG_CH_ON|(PSG_V_MAX&vol);
	__cli();
}

void PSG_SetVolume(unsigned char chan, unsigned char vol)
{
	__sei();
	*psg_ch = chan;
	*psg_ctrl = PSG_CH_ON|vol;
	__cli();
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


void PSG_SetWaveFreq(unsigned char chan, unsigned int freq)
{
	__sei();
	*psg_ch = chan;
	*psg_freqlo = freq & 0xff;
	*psg_freqhi = freq >> 8;
	__cli();
}

void PSG_SetNoiseFreq(unsigned int chan, unsigned int freq)
{
	__sei();
	*psg_ch = chan;
	*psg_noise = 0x80 | (freq & 0x1F)^0x1F;
	__cli();
}

void PSG_StopNoise(unsigned int chan)
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
	
	PSG_SetWaveFreq(chann, PULSE_TRAIN_FREQ);
	for(i = 0; i < 10; i++)
	{
		PSG_PlayCenter(chann);
		vsync();
		PSG_StopAudio(chann);
		vsync();
	}
}

void ExecuteSilence()
{
	//Silence
	//for(i = 0; i < 48; i++)   // we need this for decay....
	for(i = 0; i < 20; i++)
		vsync();
}

void PlayRampChannel(int chann)
{
	//54Hz to 22375Hz
	PSG_PlayCenter(chann);
	for(i = 2044; i > 4; i-=6)
	{
		PSG_SetWaveFreq(chann, i);
		vsync();
	}
	PSG_StopAudio(chann);
}

/* 
 these were used for the detailed test
void PlayRampChannel(int chann)
{
	//54Hz to 22375Hz
	for(i = 2044; i > 4; i-=4)
	{
		PSG_PlayCenter(chann);
		PSG_SetWaveFreq(chann, i);
		vsync();
	}
	PSG_StopAudio(chann);
	
	// wait 2 frames to align sweep to 16 frames even
	// 512 frames total (1020/2 + 4)
	// to keep same energy in FFT at 16 frames for all blocks
	for(i = 0; i < 2; i++)
		vsync();
}

void PlayRampWithVolume()
{
	x1 = 0;  // will be used as channel
	x2 = 0;  // our subsweep iterator
	
	//54Hz to 22375Hz
	for(i = 2044; i > 4; i-=24)
	{
		PSG_PlayCenter(x1);
		PSG_SetWaveFreq(x1, i);
		vsync();
		
		// Do subsweep
		x2 = PSG_V_MAX-1;
		do
		{
			PSG_SetBalance(x1, 0xf, 0xf, x2);
			vsync();
			
			x2--;
		}while(x2 >= 0);
		
		PSG_StopAudio(x1);
		
		// alternate between all channels
		x1++;
		if(x1 > 5)
			x1 = 0;
	}
}

void PlayRampWithPan()
{
	x1 = 0;  // will be used as channel
	x2 = 0;
	y  = 0;
	y2 = 1;
	
	//54Hz to 22375Hz
	for(i = 2044; i > 4; i-=2)
	{	
		PSG_SetWaveFreq(x1, i);
		if(y <= 0xf)
			PSG_SetBalance(x1, 0xf  , y  , PSG_V_MAX);
		else
			PSG_SetBalance(x1, 0xf-y, 0xf, PSG_V_MAX);
		vsync();
		
		y += y2;
		if(y == 0x1f)
		{
			y2 = 0;
			x2++;
			if(x2 == 10)
			{
				y2 = -1;
				x2 = 0;
			}
		}
		if(y == 0)
		{
			y2 = 0;
			x2++;
			if(x2 == 10)
			{
				y2 = 1;
				x2 = 0;
			}
		}
	}
	PSG_StopAudio(x1);
	
	for(i = 0; i < 4; i++)
		vsync();
}

void PlayRampWithGlobalPan()
{
	x1 = 0;  // will be used as channel
	x2 = 0;
	y  = 0;
	y2 = 1;
	
	// Init all channels
	__sei();
	for(i = 0; i < 5; i++)
	{
		*psg_ch = i;
		*psg_chbal = 0xff;
		*psg_ctrl = 0;
	}
	__cli();
	
	//54Hz to 22375Hz
	for(i = 2044; i > 4; i-=2)
	{	
		PSG_SetWaveFreq(x1, i);
		if(y <= 0xf)
			PSG_SetGlobalBalance(x1, 0xf  , y  , PSG_V_MAX);
		else
			PSG_SetGlobalBalance(x1, 0xf-y, 0xf, PSG_V_MAX);
		vsync();
		
		y += y2;
		if(y == 0x1f)
		{
			y2 = 0;
			x2++;
			if(x2 == 10)
			{
				y2 = -1;
				x2 = 0;
			}
		}
		if(y == 0)
		{
			y2 = 0;
			x2++;
			if(x2 == 10)
			{
				y2 = 1;
				x2 = 0;
			}
		}
	}
	PSG_StopAudio(x1);
	
	// Restore Global Balance.. as if...
	PSG_SetGlobalBalance(0, 0xf, 0xf, PSG_V_MAX);
	for(i = 0; i < 4; i++)
		vsync();
}

void PlayRampCombined()
{
	x1 = 0;  // will be used as channel
	x2 = 0;  // our subsweep iterator
	
	//54Hz to 22375Hz
	for(i = 2044; i > 4; i-=24)
	{
		PSG_PlayCenter(x1);
		PSG_SetWaveFreq(x1, i);
		vsync();
		
		PSG_SetBalance(x1, 0xf, 0xf, PSG_V_MAX-1);
		vsync();
		
		// Do subsweep
		x4 = PSG_V_MAX-2;
		x2 = 0xe;
		do
		{
			PSG_SetBalance(x1, x2, x2, x4);
			vsync();
			
			x4--;
			PSG_SetBalance(x1, x2, x2, x4);
			vsync();
			
			x2--;
			x4--;
		}while(x2 >= 0);
		
		PSG_StopAudio(x1);
		
		// alternate between all channels
		x1++;
		if(x1 > 5)
			x1 = 0;
	}
}

void PlayRampAllChannels()
{
	x1 = 0;  // will be used as channel
	x2 = 0;  // our subsweep iterator
	y2 = 0;  // our subsweep type

	//54Hz to 22375Hz
	for(i = 2044; i > 4; i-=6)
	{
		PSG_PlayCenter(x1);
		PSG_SetWaveFreq(x1, i);
		vsync();
		
		// Do subsweep
		x2 = 0xe;
		do
		{
			// alternate between PAN and volume sweeps
			if(y2 == 0)
				PSG_SetBalance(x1, 0xf, 0xf, x2);
			if(y2 == 1)
				PSG_SetBalance(x1, x2, 0xf, 0xf);
			if(y2 == 2)
				PSG_SetBalance(x1, 0xf, x2, 0xf);
			
			vsync();
			
			x2--;
		}while(x2 >= 0);
		
		PSG_StopAudio(x1);
		
		// alternate between all channels
		x1++;
		if(x1 > 5)
			x1 = 0;
		// alternate between pan and volume
		y2++;
		if(y2 > 2)
			y2 = 0;
	}
}
*/

/*
void PlayBothRampChannel(int chann1, int chann2)
{
	//54Hz to 22375Hz
	PSG_PlayLeft(chann1);
	PSG_PlayRight(chann2);
	for(i = 2044; i > 4; i-=6)
	{
		PSG_SetWaveFreq(chann1, i);
		PSG_SetWaveFreq(chann2, i);
		vsync();
	}
	PSG_StopAudio(chann1);
	PSG_StopAudio(chann2);
}
*/