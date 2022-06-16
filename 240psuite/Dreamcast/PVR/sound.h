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

#ifndef SOUND_H
#define SOUND_H

void SoundTest();
void AudioSyncTest();
void MDFourier();

int isSIPPresent();

#ifndef NO_FFTW

#define	SEARCH_1KHZ			1000
#define CUE_FRAMES			5
#define SECONDS_TO_RECORD	2
#define RESULTS_MAX			10
#define SIP_BUFFER_SIZE		50000	// we need around 44100 only for 2 seconds, or 45016 in PAL
#define	FFT_OM				-5000
#define	FFT_NOT_FOUND		-500
void SIPLagTest();
double ProcessSamples(short *samples, size_t size, double samplerate, double secondunits, double searchfreq);
#endif

typedef struct sip_recording_st {
	uint8	*buffer;
	size_t	size;
	size_t	pos;
	uint8	overflow;
	uint8	recording;
} sip_samples;

extern sip_samples rec_buffer;
void CleanRecordBuffer();

// These need to be global for the callback to work
extern char		*stream_samples;
extern char		stream_buffer[SND_STREAM_BUFFER_MAX];
extern int		stream_samples_size;
extern int		stream_pos;
extern int		stream_samplerate;

void *sound_callback(snd_stream_hnd_t hnd, int smp_req, int *smp_recv);
void CleanStreamSamples();
void sip_copy(maple_device_t *dev, uint8 *samples, size_t len);

#endif

