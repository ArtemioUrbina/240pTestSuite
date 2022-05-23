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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	02111-1307	USA
 */

#include <kos.h>
#include <stdlib.h>
#include <dc/sound/sound.h>
#include <dc/sound/sfxmgr.h>

#ifndef NO_FFTW
	#include <math.h>
	#include <complex.h>
	#include <fftw/fftw3.h>
#endif

#include "controller.h"
#include "sound.h"
#include "image.h"
#include "font.h"
#include "vmodes.h"
#include "vmu.h"

#include "help.h"
#include "menu.h"

void SoundTest()
{
	int 		done = 0, sel = 1, play = 0, pan = 128, prevSel = -1;
	uint16		pressed;		
	ImagePtr	back;
	sfxhnd_t	beep;
	controller	*st;
	char		*vmuMsg = "";

	back = LoadKMG("/rd/back.kmg.gz", 0);
	if(!back)
		return;

	beep = snd_sfx_load("/rd/beep.wav");
	if(!beep)
		return;
	
	while(!done && !EndProgram) 
	{
		StartScene();
		DrawImage(back);

		DrawStringS(130, 60, 1.0f, 1.0f, 1.0f, "Sound Test"); 
		DrawStringS(80, 120, 1.0f, sel == 0 ? 0 : 1.0f,	sel == 0 ? 0 : 1.0f, "Left Channel"); 
		DrawStringS(120, 130, 1.0f, sel == 1 ? 0 : 1.0f,	sel == 1 ? 0 : 1.0f, "Center Channel");
		DrawStringS(160, 120, 1.0f, sel == 2 ? 0 : 1.0f,	sel == 2 ? 0 : 1.0f, "Right Channel");
		EndScene();
		
		VMURefresh("Sound Test", vmuMsg);

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;

			if (pressed & CONT_A)
				play =	1;

			if (pressed & CONT_DPAD_LEFT)
				sel --;

			if (pressed & CONT_DPAD_RIGHT)
				sel ++;

			if (pressed & CONT_START)
				ShowMenu(SOUNDHELP);
		}
		
		if(sel < 0)
			sel = 2;

		if(sel > 2)
			sel = 0;

		if(sel != prevSel)
		{
			switch(sel)
			{
				case 0:
					pan = 0;
					vmuMsg = "Left      ";
					break;
				case 1:
					pan = 128;
					vmuMsg = "  Center  ";
					break;
				case 2:
					pan = 255;
					vmuMsg = "     Right";
					break;
			}
			prevSel = sel;
			refreshVMU = 1;
		}

		if(play && beep != SFXHND_INVALID)
		{
			snd_sfx_play(beep, 255, pan);
			play = 0;
		}
	}
	if(beep != SFXHND_INVALID)
		snd_sfx_unload(beep);
	FreeImage(&back);
	return;
}

void AudioSyncTest()
{
	int 		done = 0, paused = 0, oldvmode = 0, playtone = 0;
	int			y = 0, speed = -1;
	float		hstep = 0;
	uint16		pressed;
	sfxhnd_t	beep;
	ImagePtr	squareL, squareR, lineB, sprite, back, cover;	
	
	back = LoadKMG("/rd/white.kmg.gz", 1);
	if(!back)
		return;
	squareL = LoadKMG("/rd/white.kmg.gz", 1);
	if(!squareL)
		return;
	squareR = LoadKMG("/rd/white.kmg.gz", 1);
	if(!squareR)
		return;
	lineB = LoadKMG("/rd/white.kmg.gz", 1);
	if(!lineB)
		return;
	sprite = LoadKMG("/rd/white.kmg.gz", 1);
	if(!sprite)
		return;
	cover = LoadKMG("/rd/white.kmg.gz", 1);
	if(!cover)
		return;
	
	back->r = 0.0f;
	back->g = 0.0f;
	back->b = 0.0f;
	
	back->w = dW;
	back->h = dH;
	
	sprite->w = sprite->h = 8;
	sprite->x = dW / 2 - 4;
	y = 180;
	hstep = -1 * dW/120; // 60 steps times half screen
	
	lineB->w = dW;
	lineB->h = 8;
	lineB->x = 0;
	lineB->y = 180 + sprite->h;
	
	squareL->w = dW / 2;
	squareL->h = 16;
	squareL->x = 0;
	squareL->y = 80;
	
	squareR->w = dW / 2;
	squareR->h = 16;
	squareR->x = dW / 2;
	squareR->y = 80;
	
	cover->r = 0.0f;
	cover->g = 0.0f;
	cover->b = 0.0f;
	
	cover->w = dW;
	cover->h = dH;
	cover->x = dW;
	cover->y = 0;
	
	beep = snd_sfx_load("/rd/beep_delayed.wav");
	if(beep == SFXHND_INVALID)
		return;
	
	while(!done && !EndProgram) 
	{
		if(oldvmode != vmode)
		{
			back->w = dW;
			back->h = dH;
			oldvmode = vmode;
		}
		
		if(!paused)
		{
			y += speed;
			sprite->y = y;
			squareL->x += hstep;
			squareR->x -= hstep;
			if(y == 180 || y == 120)
			{
				speed *= -1;
				hstep *= -1;
			}
			if(y == 180)
			{
				back->r = 1.0f;
				back->g = 1.0f;
				back->b = 1.0f;
				
				cover->r = 1.0f;
				cover->g = 1.0f;
				cover->b = 1.0f;
			}
			else
			{
				back->r = 0.0f;
				back->g = 0.0f;
				back->b = 0.0f;
				
				cover->r = 0.0f;
				cover->g = 0.0f;
				cover->b = 0.0f;
			}
			
				
			// A frame is 16.80 ms, Since we have 1.8ms out of sync
			// we make a file with 15ms silence an play it back
			// 1 frame before in order to have sync
			
			if(y == 180 && speed == -1)
				playtone = 1;
		}
			
		StartScene();
		DrawImage(back);
		DrawImage(squareL);
		DrawImage(squareR);
		DrawImage(lineB);
		DrawImage(sprite);
		DrawImage(cover);
		EndScene();
		
		if(playtone == 1)
		{
			snd_sfx_play(beep, 255, 128); // Centered
			playtone = 0;
		}
			
		VMURefresh("A. Sync", "");
		
		ReadController(0, &pressed);
		
		if (pressed & CONT_B)
			done =	1;
			
		if (pressed & CONT_A)
			paused = !paused;
			
		if (pressed & CONT_START)
			ShowMenu(AUDIOSYNCHELP);

	}
	
	FreeImage(&back);
	FreeImage(&squareL);
	FreeImage(&squareR);
	FreeImage(&lineB);
	FreeImage(&sprite);
	FreeImage(&cover);
	
	if(beep != SFXHND_INVALID)
		snd_sfx_unload(beep);
}

#ifndef NO_FFTW

#define CUE_FRAMES		5
#define SECONDS_TO_RECORD	2
#define RESULTS_MAX		10
#define BUFFER_SIZE 		50000	// we need around 45k only
#define	FFT_OM			-5000
#define	FFT_NOT_FOUND		-500


typedef struct recording {
	uint8	*buffer;
	size_t	size;
	int		pos;
	uint8	overflow;
	uint8	recording;
} recording;

recording rec_buffer;

void DrawSIPScreen(ImagePtr back, ImagePtr wave, char *Status, int accuracy, double *Results, int ResCount, int showframes)
{
	int	i = 0;
	char	DPres[40];
	char	Header[40];
	char	Res[40];

	StartScene();
	DrawImage(back);
	DrawImage(wave);

	if(IsPAL)
	{
		if(accuracy == 1)
			sprintf(DPres, "Frame accuracy: 1 frame 20ms");
		else
			sprintf(DPres, "Frame accuracy: 1/%d frame %0.3gms", accuracy, 20.0/accuracy);
	}
	else
	{
		if(accuracy == 1)
			sprintf(DPres, "Frame accuracy: 1 frame 16.67ms");
		else
			sprintf(DPres, "Frame accuracy: 1/%d frame %0.3gms", accuracy, 16.6667/accuracy);
	}

	DrawStringS(40, 60, 0.0f, 1.0f, 1.0f, "Lag Test via Microphone & Fast Fourier Transform"); 
	DrawStringS(40, 120, 1.0f, 1.0f, 1.0f, Status);
	DrawStringS(38, 180, 1.0f, 1.0f, 1.0f, "Press X to toggle between milliseconds and frames");
	DrawStringS(38, 190, 1.0f, 1.0f, 1.0f, "Press Y to toggle loop mode");
	DrawStringS(120, 200, 0.0f, 1.0f, 0.0f, DPres);

	if(ResCount)
	{
		if(ResCount == 1)
			sprintf(Header, "Last result:");
		else
			sprintf(Header, "Last %d results:", ResCount);
		DrawStringS(174, 80, 0.0f, 1.0f, 0.0f, Header);
		for(i = 1; i <= ResCount; i++)
		{
			if(Results[i-1] < 0 && Results[i-1] != FFT_OM && Results[i-1] != FFT_NOT_FOUND)
				sprintf(Res, "#G%.2d#G #YNoise at 1khz#Y", i);
			if(Results[i-1] == FFT_OM)
				sprintf(Res, "#G%.2d#G #ROut of Memory#R", i);
			if(Results[i-1] == FFT_NOT_FOUND)
				sprintf(Res, "#G%.2d#G #RNo tone detected#R", i);
			if(Results[i-1] >= 0)
			{
				if(showframes)
					sprintf(Res, "#G%.2d#G Lag was #C%g#C frames", i, Results[i-1]);
				else
					sprintf(Res, "#G%.2d#G Lag was #C%0.2f#C ms", i, Results[i-1]*16.666);
			}
			DrawStringS(164, 80+i*fh, 1.0f, 1.0f, 1.0f, Res);
		}
	}
	EndScene();
}

void sip_copy(maple_device_t *dev, uint8 *samples, size_t len)
{
	if(!rec_buffer.recording)
		return;

	if(!rec_buffer.buffer || !rec_buffer.size)
	{
#ifdef DEBUG_FFT
		dbglog(DBG_CRITICAL, "Invalid buffer or size\n");
#endif
		return;	
	}
	if(!dev || !samples || !len)
	{
#ifdef DEBUG_FFT
		dbglog(DBG_CRITICAL, "Invalid dev, samples or len\n");
#endif
		return;
	}
	
	if(len > (rec_buffer.size - (size_t)rec_buffer.pos))
	{
		rec_buffer.overflow = 1;
#ifdef DEBUG_FFT
		dbglog(DBG_CRITICAL, "Buffer overflow\n");
#endif
		return;
	}

	memcpy(rec_buffer.buffer + rec_buffer.pos, samples, len);
	rec_buffer.pos += len;
}

void SIPLagTest()
{
	int				done = 0, status = 0, counter = 0;
	int				showframes = 0, accuracy = 1, cycle = 0;
	uint16			pressed;		
	ImagePtr		back, wave;
	sfxhnd_t		beep;  
	controller		*st;
	maple_device_t	*sip = NULL;  
	double			Results[RESULTS_MAX];
	int				ResCount = 0;
	char			DStatus[100];
	float			delta = 0.01f;
	char			*vmuMsg1 = "Lag v/Micr", *vmuMsg2 = "";

	DStatus[0] = 0x0;
	back = LoadKMG("/rd/back.kmg.gz", 0);
	if(!back)
		return;

	wave = LoadKMG("/rd/1khz.kmg.gz", 0);
	if(!wave)
	{
		FreeImage(&back);
		return;
	}
	wave->alpha = 0.05f;

	srand((int)(time(0) ^ getpid()));

	beep = snd_sfx_load("/rd/Sample.wav");
	if(!beep)
	{
		FreeImage(&back);
		FreeImage(&wave);
		return;
	}

	rec_buffer.buffer = malloc(sizeof(uint8)*BUFFER_SIZE);
	if(!rec_buffer.buffer)
	{
		if(beep != SFXHND_INVALID)
			snd_sfx_unload(beep);
		FreeImage(&back);
		return;
	}
	rec_buffer.size = sizeof(uint8)*BUFFER_SIZE;
	rec_buffer.pos = 0;
	rec_buffer.overflow = 0;
	rec_buffer.recording = 0;

	sip = maple_enum_type(0, MAPLE_FUNC_MICROPHONE);
	if(!sip)
		return;

	sip_set_gain(sip, SIP_MAX_GAIN); // SIP_DEFAULT_GAIN  
	sip_set_sample_type(sip, SIP_SAMPLE_16BIT_SIGNED); 
	sip_set_frequency(sip, SIP_SAMPLE_11KHZ); // 11.025kHz
		
	if(sip_start_sampling(sip, sip_copy, 1) == MAPLE_EOK)
	{
#ifdef DEBUG_FFT
		dbglog(DBG_INFO, "Mic ok\n");
#endif
	}
	else
	{
		dbglog(DBG_CRITICAL, "Mic Recording: Failed\n");
		done = 1;
	}

	sprintf(DStatus, "Press A");
	while(!done && !EndProgram) 
	{		
		if(status)
		{
			wave->alpha += delta;
			if(wave->alpha > 0.6f)
			{
				wave->alpha = 0.6f;
				delta *= -1;
			}
			if(wave->alpha < 0.2f)
			{
				wave->alpha = 0.2f;
				delta *= -1;
			}
		}

		DrawSIPScreen(back, wave, DStatus, accuracy, Results, ResCount, showframes);
		VMURefresh(vmuMsg1, vmuMsg2);

		sip = maple_enum_type(0, MAPLE_FUNC_MICROPHONE);
		if(!sip)
			done = 1;
	
		st = ReadController(0, &pressed);
		if(st) 
		{
			if(status == 0) // these are not allowed while sampling
			{
				if (pressed & CONT_START)
					ShowMenu(FFTHELP);
	
				if (pressed & CONT_B)
					done = 1;

				if (pressed & CONT_A)
					status = 1;

				if (pressed & CONT_RTRIGGER)
					accuracy *= 2;

				if (pressed & CONT_LTRIGGER)
					accuracy /= 2;

				if(accuracy > 4)
					accuracy = 4;

				if(accuracy < 1)
					accuracy = 1;
			}

			if (pressed & CONT_Y)
				cycle = !cycle;

			if (pressed & CONT_X)
				showframes = !showframes;

			if(cycle && !status)
				status = 1;
		}

		if(status == 2 || status == 4)
		{
			// frame accurate sampling
			if(status == 2 && counter == CUE_FRAMES && !rec_buffer.recording)
				rec_buffer.recording = 1;

			counter --;
			if(!counter)
				status++;
			
			if(status == 4)
			{
				sprintf(DStatus, "Recording Frame: %d", SECONDS_TO_RECORD*(IsPAL ? 50 : 60) - counter);
				if(counter == 1)
					rec_buffer.recording = 0;

				if(rec_buffer.pos == 0)
				{
					sprintf(DStatus, "#YPlease remove and reinsert Microphone#Y");
					cycle = 0;
					rec_buffer.recording = 0;
					status = 0;
					
					vmuMsg1 = "Reinsert";
					vmuMsg2 = "micro";
					refreshVMU = 1;
				}
			}
		}

		if(status == 1 && sip)
		{
			sprintf(DStatus, "Starting");
			status = 2;
			counter = CUE_FRAMES;
		}

		if(status == 3 && beep != SFXHND_INVALID)
		{
			snd_sfx_play(beep, 255, 128);
			status = 4;
			counter = SECONDS_TO_RECORD*(IsPAL ? 50 : 60); // record N seconds + CUE frames
		}
		
		if(status == 5)
		{
			sprintf(DStatus, "Stopped sampling");

#ifdef DEBUG_FFT
			dbglog(DBG_INFO, "Got %d bytes\n", (int)rec_buffer.pos);
#endif
			if(rec_buffer.buffer && rec_buffer.pos > 0)
			{
				double value;

				DrawSIPScreen(back, wave, "Analyzing...", accuracy, Results, ResCount, showframes);
				vmuMsg1 = "Analyzing";
				vmuMsg2 = "";
				updateVMU(vmuMsg1, vmuMsg2, 1);
				value = ProcessSamples((short*)rec_buffer.buffer, rec_buffer.pos/2,
					11025, (IsPAL ? 50.0 : 60.0)*accuracy, 1000);
				if(value < 0 && value != FFT_NOT_FOUND && value != FFT_OM)
				{
					sprintf(DStatus, "#YNoise at 1khz#Y");
					vmuMsg1 = "Noise at";
					vmuMsg2 = "1Khz";
					updateVMU(vmuMsg1, vmuMsg2, 1);
				}
				if(value == FFT_OM)
				{
					sprintf(DStatus, "#ROut of Memory#R");
					vmuMsg1 = "Out of";
					vmuMsg2 = "memory";
					updateVMU(vmuMsg1, vmuMsg2, 1);
				}
				if(value == FFT_NOT_FOUND)
				{
					sprintf(DStatus, "#YCheck audio system#Y");
					vmuMsg1 = "Check";
					vmuMsg2 = "audio";
					updateVMU(vmuMsg1, vmuMsg2, 1);
				}
				if(value >= 0)
				{
					char vmtext[10];

					sprintf(DStatus, "Lag is #C%g#C frames\n       #C%0.2f#C ms",
						value, value*16.66);
					if(showframes)
						sprintf(vmtext, "%g f", value);
					else
						sprintf(vmtext, "%g ms", value*16.66);
					vmuMsg1 = "Lag is:";
					vmuMsg2 = vmtext;
					updateVMU(vmuMsg1, vmuMsg2, 1);
				}

				if(ResCount == RESULTS_MAX)
				{
					int i = 0;

					for(i = 0; i < RESULTS_MAX - 1; i++)
						Results[i] = Results[i+1];
					ResCount --;
				}
				Results[ResCount++] = value;
			}
			else
			{
				sprintf(DStatus, "#YRecording failed#Y");
				vmuMsg1 = "Record";
				vmuMsg2 = "failed";
				updateVMU(vmuMsg1, vmuMsg2, 1);
			}

			rec_buffer.pos = 0;
			status = 0;
		}
	}

	if(sip)
		sip_stop_sampling(sip, 0);

	if(rec_buffer.size)
	{
		free(rec_buffer.buffer);
		rec_buffer.size = 0;
		rec_buffer.pos = 0;
	}

	if(beep != SFXHND_INVALID)
		snd_sfx_unload(beep);
	FreeImage(&wave);
	FreeImage(&back);

	fftw_cleanup();  
	return;
}

double ProcessSamples(short *samples, size_t size, long samplerate, double secondunits, double searchfreq)
{
	long		  	samplesize = 0, arraysize = 0;	
	long		  	i = 0, f = 0, framesizernd = 0; 
	double		  	*in, root = 0, framesize = 0;  
	double		  	*MaxFreqArray = NULL;
	fftw_complex  	*out;
	fftw_plan	p = NULL;
#ifdef DEBUG_FFT
	long		time = 0;
	uint64		start, end;
#endif
	double	  	mins, maxs;
	double		 	boxsize = 0;
	int 		 	casefrq = 0;  
	int 		found = 0;
	long 		pos = 0, count = 0;    
	long 		tpos = 0, tcount = 0;	 
	double		value = FFT_NOT_FOUND;
	
	samplesize = (long) size;  

	framesize = samplerate/secondunits;
	framesizernd = (long)framesize;  

#ifdef DEBUG_FFT
	dbglog(DBG_INFO, "Samples are at %lu Khz and %g seconds long. A Frame is %g samples.\n",
					samplerate, (double)samplesize/samplerate, framesize);

	start = timer_ms_gettime64();
#endif
	in = (double*) fftw_malloc(sizeof(double) * framesizernd);
	if(!in)
		return FFT_OM;
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (framesizernd/2+1));  
	if(!out)
	{
		 fftw_free(in); 
		 return FFT_OM;
	}
		
	arraysize = framesizernd;
	MaxFreqArray = malloc(sizeof(double)*(samplesize/framesize - 1));
	if(!MaxFreqArray)
	{
		 fftw_free(in); 
		 fftw_free(out);
		 return FFT_OM;
	}

	boxsize = (double)arraysize/(double)samplerate;  
	for(f = 0; f < samplesize/framesize - 1; f++)
	{
		double mainfreq = 0, max = 0;
		long   maxind = -1;
		long   framestart = 0;

		framestart = framesize*f;
		for(i = 0; i < framesizernd; i++)
			in[i] = (double)samples[i+framestart];

#ifdef DEBUG_FFT
		dbglog(DBG_INFO, "Estimating plan\n");
#endif
		p = fftw_plan_dft_r2c_1d(arraysize, in, out, FFTW_ESTIMATE);

#ifdef DEBUG_FFT
		dbglog(DBG_INFO, "Executing FFTW\n");
#endif
		fftw_execute(p); 
	
#ifdef DEBUG_FFT
		dbglog(DBG_INFO, "Analyzing results\n");
#endif
		root = sqrt(arraysize);
		for(i = 0; i < arraysize/2+1; i++)
		{
			double r1 = creal(out[i]);
			double i1 = cimag(out[i]);
			double val = 0;
	
			val = sqrt(sqrt(r1 * r1 + i1 * i1) / root);
			if(val > max)
			{
				max = val;
				maxind = i;
			}
		}
		
		mainfreq = (double)((double)maxind/boxsize);
		MaxFreqArray[f] = mainfreq;
		
		fftw_destroy_plan(p);
	}

#ifdef DEBUG_FFT
	end = timer_ms_gettime64();
	time = end - start;
	dbglog(DBG_INFO, "FFT for %g frames took %ld\n", samplesize/framesize - 1, time);
	start = end;
#endif

	casefrq = (int)ceil(searchfreq/(1/boxsize));
	mins = (casefrq - 1) / boxsize;
	maxs = (casefrq + 1) / boxsize;
#ifdef DEBUG_FFT
	dbglog(DBG_INFO, "Searching for %g, due to samplerate, arraysize and accurancy %d it is %g, between %g and %g\n",
		searchfreq, (int)secondunits/(IsPAL ? 50 : 60), casefrq/boxsize, mins, maxs);
#endif
 
	for(f = 0; f < samplesize/framesize - 1; f++)
	{
		if(!found)
		{
#ifdef DEBUG_FFT
			dbglog(DBG_INFO, "Frame %ld: Main frequency %g Hz\n", f-CUE_FRAMES, MaxFreqArray[f]);
#endif
			if(count)
			{
				if(MaxFreqArray[f] < mins || MaxFreqArray[f] > maxs)
				{
					// tentative result if all fails
					if(!tpos) // only the first result
					{
						tpos = pos;
						tcount = count;
					}
	
					count = 0;
					pos = 0;
				}
				else
				{
					count++;
					if(count == (int)secondunits)
					{
						pos -= CUE_FRAMES*(secondunits/(IsPAL ? 50.0 : 60.0));
						value = pos/(secondunits/(IsPAL ? 50.0 : 60.0));
#ifdef DEBUG_FFT
						dbglog(DBG_INFO, "Found at %g frames -> %g sec\n", value, pos/secondunits);
#endif
						found = 1;
					}
				}
			}
				
			if(!count && MaxFreqArray[f] >= mins && MaxFreqArray[f] <= maxs)
			{
				pos = f;
				count = 1;
			}
		}
	}
	
	if(!found && tpos != 0 && tcount > secondunits/2) // Did we find one at least 1/2 second long?
	{
		pos = tpos - CUE_FRAMES*(secondunits/(IsPAL ? 50.0 : 60.0));
		value = pos/(secondunits/(IsPAL ? 50.0 : 60.0));
#ifdef DEBUG_FFT
		dbglog(DBG_INFO, "Found (heur %ld) at %g frames -> %g sec\n", tcount, value, pos/secondunits);
#endif
	}

#ifdef DEBUG_FFT
	end = timer_ms_gettime64();
	time = end - start;
	dbglog(DBG_INFO, "Processing frequencies took %ld\n", time);
#endif

	free(MaxFreqArray);
	MaxFreqArray = NULL;  
	
	fftw_free(in); 
	fftw_free(out);
	return(value);
}

#endif
