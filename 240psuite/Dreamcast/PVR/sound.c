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
	#include <fftw3.h>
#endif

#include "controller.h"
#include "sound.h"
#include "image.h"
#include "font.h"
#include "vmodes.h"
#include "vmu.h"

#include "help.h"
#include "menu.h"
#include <zlib/zlib.h>
#include <dc/flashrom.h>

int zlib_getlength(char *filename);

int isSIPPresent()
{
	maple_device_t	*sip = NULL;  
	
	sip = maple_enum_type(0, MAPLE_FUNC_MICROPHONE);
	if(!sip)
		return 0;
	return 1;
}

void SoundTest()
{
	int 				done = 0, sel = 1, play = 0, pan = 128;
	int					is_mono = 0, prevSel = -1, read = 0;
	uint16				pressed;		
	ImagePtr			back;
	sfxhnd_t			beep;
	controller			*st;
	char				*vmuMsg = "";
	flashrom_syscfg_t 	sysconf;

	back = LoadKMG("/rd/back.kmg.gz", 0);
	if(!back)
		return;

	beep = snd_sfx_load("/rd/beep.wav");
	if(beep == SFXHND_INVALID)
		return;
	
	read = flashrom_get_syscfg(&sysconf);
	if(read == 0)
	{
#ifdef DCLOAD
		dbglog(DBG_INFO,
			"Loaded Frashrom Sys Config:\n Audio: %d\n Language: %d\n Auto-Start: %d\n",
			sysconf.audio,
			sysconf.language, 
			sysconf.autostart
			);
#endif
		if(sysconf.audio == 0)
			is_mono = 1;
	}
#ifdef DCLOAD
	else
		dbglog(DBG_ERROR, "ERROR: flashrom_get_syscfg() returned %d\n", read); 
#endif
	
	while(!done && !EndProgram) 
	{
		StartScene();
		DrawImage(back);

		DrawStringSCentered(60, 0.0f, 1.0f, 0.0f, "Sound Test"); 
		if(!is_mono)
		{
			DrawStringS(80, 120, 1.0f, sel == 0 ? 0 : 1.0f,	sel == 0 ? 0 : 1.0f, "Left Channel"); 
			DrawStringSCentered(130, 1.0f, sel == 1 ? 0 : 1.0f, sel == 1 ? 0 : 1.0f, "Center Channel");
			DrawStringS(180, 120, 1.0f, sel == 2 ? 0 : 1.0f, sel == 2 ? 0 : 1.0f, "Right Channel");
		}
		else
			DrawStringS(120, 120, 1.0f, sel == 1 ? 0 : 1.0f, sel == 1 ? 0 : 1.0f, "Mono Channel");
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
			{
				ShowMenu(SOUNDHELP);
				prevSel = -1;
			}
		}
		
		if(is_mono)
			sel = 1;
		else
		{
			if(sel < 0)
				sel = 2;

			if(sel > 2)
				sel = 0;
		}

		if(sel != prevSel)
		{
			if(is_mono)
				vmuMsg = "  Mono  ";
			else
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

// These need to be global for the callback to work
// int is enough for our purposes, since it defaults to 32 bits
// (zlib_getlength made me check)
char	*stream_samples = NULL;
char	stream_buffer[SND_STREAM_BUFFER_MAX];
int		stream_samples_size = 0;
int		stream_pos = 0;
int		stream_samplerate = 44100;

void *sound_callback(__attribute__((unused))snd_stream_hnd_t hnd, int smp_req, int *smp_recv)
{
	int	bytes_to_copy = smp_req;
	
	memset(stream_buffer, 0, sizeof(char)*SND_STREAM_BUFFER_MAX);

	*smp_recv = smp_req;	
	if(!stream_samples || stream_pos >= stream_samples_size)
		return stream_buffer;

	if(stream_samples_size < stream_pos + smp_req)
		bytes_to_copy = stream_samples_size - stream_pos;
	
	memcpy(stream_buffer, stream_samples+stream_pos, sizeof(char)*bytes_to_copy);
	stream_pos += bytes_to_copy;

	return stream_buffer;
}

void CleanStreamSamples()
{
	if(stream_samples)
	{
		free(stream_samples);
		stream_samples = NULL;
	}
	stream_pos = stream_samples_size = 0;
}

inline void DisplayError(char *msg)
{
	DrawMessage(msg);
#ifdef DCLOAD
	dbglog(DBG_ERROR, msg);
#endif
}

// Takes around 10 seconds, around ten seconds faster than uncompressed from CD-R
int LoadGZMDFSamples()
{
	gzFile		file;
	int			deflate_size = 0, khz = 1;
	char		*filename = NULL;
	fmenudata 	resmenudata[] = { {1, "44100hz (best)"}, {2, "48000hz"} };
#ifdef BENCHMARK
	uint64 		start, end;
#endif
	
	VMURefresh("Select", "Frequency");
	khz = SelectMenu("Select Frequency", resmenudata, 2, khz);
	if(khz == MENU_CANCEL)
		return 0;
	
	if(khz == 1)
	{
		filename = "/cd/mdfourier-dac-44100.pcm.gz";
		stream_samplerate = 44100;
	}
	if(khz == 2)
	{
		filename = "/cd/mdfourier-dac-48000.pcm.gz";
		stream_samplerate = 48000;
	}
		
	CleanStreamSamples();
	deflate_size = zlib_getlength(filename);
	if(!deflate_size)
	{
		DisplayError("Could not find PCM samples file in drive\n");
		return 0;
	}
	
#ifdef BENCHMARK
	start = timer_us_gettime64();
#endif
	updateVMU_wait();
	DrawMessageOnce("Please wait while samples are loaded\n#G(about 10 seconds)#G");
	
	file = gzopen(filename, "r");
	if(!file)
	{
		DisplayError("No PCM samples file in drive\n");
		return 0;
	}
	stream_samples = (char*)malloc(sizeof(char)*deflate_size);
	if(!stream_samples) 
	{
		gzclose(file);        
		DisplayError("Out of memory for PCM samples\n");
		return 0;
	}
	if(gzread(file, stream_samples, sizeof(char)*deflate_size) != deflate_size)
	{
		gzclose(file);
		
		free(stream_samples);
		stream_samples = NULL;
		
		DisplayError("Error loading and decompressing samples file\n");
		return 0;
	}
	
	gzclose(file);
	
	stream_samples_size = deflate_size;
	
	if(cdrom_spin_down() != ERR_OK)
		dbglog(DBG_ERROR,"Could not stop CD-ROM from spinning\n");
	
#ifdef BENCHMARK
	end = timer_us_gettime64();

	dbglog(DBG_INFO, "PCM file loading took %g ms\n", (double)(end - start)/1000.0);
#endif
	return 1;
}

void MDFourier()
{
	float				delta = 0.01f;
	int 				done = 0, play = 0;
	char				vmsg[20];
	uint16				pressed;		
	ImagePtr			back, layer;
	controller			*st = NULL;
	snd_stream_hnd_t 	hnd = SND_STREAM_INVALID;

	back = LoadKMG("/rd/back.kmg.gz", 0);
	if(!back)
		return;
	layer = LoadKMG("/rd/mdfourier.kmg.gz", 0);
	if(!layer)
	{
		FreeImage(&back);
		return;
	}
	layer->alpha = 0.01f;
	
	if(!LoadGZMDFSamples())
	{
		if(cdrom_spin_down() != ERR_OK)
			dbglog(DBG_ERROR,"Could not stop CD-ROM from spinning\n");
		FreeImage(&back);
		FreeImage(&layer);
		return;
	}
	sprintf(vmsg, " %d hz", stream_samplerate);
	
	hnd = snd_stream_alloc(sound_callback, SND_STREAM_BUFFER_MAX);
	if(hnd == SND_STREAM_INVALID)
	{
		CleanStreamSamples();
		FreeImage(&back);
		FreeImage(&layer);
		DisplayError("Could not get SPU stream");
		return;
	}
	
	snd_stream_volume(hnd, 255);
	snd_stream_queue_enable(hnd);
	snd_stream_start(hnd, stream_samplerate, 1);
	
	while(!done && !EndProgram) 
	{
		char msg[250];
		
		StartScene();
		DrawImage(back);
		DrawImage(layer);

		DrawStringSCentered(60, 0.0f, 1.0f, 0.0f, "MDFourier"); 
		sprintf(msg, "Using %d hz Audio Equipment Test", stream_samplerate);
		DrawStringSCentered(70+1*fh, 1.0f, 1.0f, 1.0f, msg); 
		if(play)
		{
			if(stream_pos != stream_samples_size)
				sprintf(msg, "Please record signal");
			else
				sprintf(msg, "Press #YA#Y to play again");
		}
		else
			sprintf(msg, "Press #YA#Y to play signal");
		DrawStringSCentered(70+2*fh, 0.0f, 1.0f, 0.0f, msg);
		if(stream_samplerate == 48000)
			DrawStringSCentered(70+10*fh, 1.0f, 1.0f, 0.0f, "Yamaha AICA has aliasing at 48khz"); 
		DrawStringSCentered(70+13*fh, 1.0f, 1.0f, 1.0f, "Visit #Chttp://junkerhq.net/MDFourier#C for info"); 
		EndScene();
		
		snd_stream_poll(hnd);
		VMURefresh("MDFourier", vmsg);

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done =	1;

			if (pressed & CONT_A)
			{
				if(!play)
				{
					play = 1;
					snd_stream_queue_go(hnd);
				}
				else
					stream_pos = 0;
			}
			
			if (pressed & CONT_X)
			{
				snd_stream_stop(hnd);
				
				if(!LoadGZMDFSamples())
				{
					if(cdrom_spin_down() != ERR_OK)
						dbglog(DBG_ERROR,"Could not stop CD-ROM from spinning\n");
					done = 1;
				}
				else
				{
					stream_pos = play ? stream_samples_size : 0;

					snd_stream_volume(hnd, 255);
					snd_stream_queue_enable(hnd);
					snd_stream_start(hnd, stream_samplerate, 1);
					
					sprintf(vmsg, " %d hz", stream_samplerate);
					refreshVMU = 1;
				}
			}

			if (pressed & CONT_START)
			{
				if(play)
				{
					snd_stream_stop(hnd);
					stream_pos = stream_samples_size;
				}
				
				ShowMenu(MDFOURIERHELP);
				
				if(play)
					snd_stream_start(hnd, stream_samplerate, 1);
			}
		}
		
		if(play)
		{
			if(stream_pos != stream_samples_size)
			{
				layer->alpha += delta;
				if(layer->alpha > 0.6f)
				{
					layer->alpha = 0.6f;
					delta *= -1;
				}
				if(layer->alpha < 0.2f)
				{
					layer->alpha = 0.2f;
					delta *= -1;
				}
			}
			else
				layer->alpha = 0.01f;
		}
	}
	
	snd_stream_queue_disable(hnd);
	snd_stream_stop(hnd);
	snd_stream_destroy(hnd);
	
	CleanStreamSamples();
	FreeImage(&layer);
	FreeImage(&back);
}

void AudioSyncTest()
{
	int 		done = 0, paused = 0, oldvmode = -1, playtone = 0;
	int			y = 0, speed = -1, W = 320, H = 240;
	float		hstep = 0;
	uint16		pressed;
	sfxhnd_t	beep = SFXHND_INVALID;
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
	
	back->w = W;
	back->h = H;
	
	sprite->w = sprite->h = 8;
	sprite->x = W / 2 - 4;
	y = 180;
	hstep = -1 * W/120; // 60 steps times half screen
	
	lineB->w = W;
	lineB->h = 8;
	lineB->x = 0;
	lineB->y = 180 + sprite->h;
	
	squareL->w = W / 2;
	squareL->h = 16;
	squareL->x = 0;
	squareL->y = 80;
	
	squareR->w = W / 2;
	squareR->h = 16;
	squareR->x = W / 2;
	squareR->y = 80;
	
	cover->r = settings.PalBackR;
	cover->g = settings.PalBackG;
	cover->b = settings.PalBackB;
	
	cover->w = W;
	cover->h = H;
	cover->x = W;
	cover->y = 0;
	
	while(!done && !EndProgram) 
	{
		if(oldvmode != vmode)
		{
			int is50hz = 0;
			
			back->r = 0.0f;
			back->g = 0.0f;
			back->b = 0.0f;
			
			cover->r = settings.PalBackR;
			cover->g = settings.PalBackG;
			cover->b = settings.PalBackB;
				
			back->w = W;
			back->h = H;
			oldvmode = vmode;
			
			if(vmode == VIDEO_288P || vmode == VIDEO_576I_A264 || vmode == VIDEO_576I)
				is50hz = 1;
			
			if(beep != SFXHND_INVALID)
				snd_sfx_unload(beep);
			beep = SFXHND_INVALID;
			
			if(is50hz)	
				beep = snd_sfx_load("/rd/beep_PAL_delayed.wav");
			else
				beep = snd_sfx_load("/rd/beep_delayed.wav");
			if(beep == SFXHND_INVALID)
				return;
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
			}
			else
			{
				back->r = 0.0f;
				back->g = 0.0f;
				back->b = 0.0f;
				
				cover->r = settings.PalBackR;
				cover->g = settings.PalBackG;
				cover->b = settings.PalBackB;
			}
			
				
			// A frame is 16.80 ms, Since we have 1.8ms out of sync
			// we make a file with 15ms silence an play it back
			// 1 frame before in order to have sync
			// also adapted for PAL timing
			
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

sip_samples rec_buffer = { NULL, 0, 0, 0, 0 };

void CleanRecordBuffer()
{
	if(rec_buffer.size)
	{
		free(rec_buffer.buffer);
		rec_buffer.buffer = NULL;
	}
	rec_buffer.size = 0;
	rec_buffer.pos = 0;
	rec_buffer.overflow = 0;
	rec_buffer.recording = 0;
}

void DrawSIPScreen(ImagePtr back, ImagePtr wave, char *Status, int accuracy, double *Results, int ResCount, int showframes, double samplerate)
{
	int		i = 0;
	char	DPres[40];
	char	Header[40];
	char	Res[40];
	char	sr[40];

	StartScene();
	DrawImage(back);
	DrawImage(wave);


	if(accuracy == 1)
		sprintf(DPres, "Frame accuracy: 1 frame %gms", IsPAL ? PAL_FRAME_LEN : NTSC_FRAME_LEN);
	else
		sprintf(DPres, "Frame accuracy: 1/%d frame %0.3gms", accuracy, (IsPAL ? PAL_FRAME_LEN : NTSC_FRAME_LEN)/accuracy);

	DrawStringSCentered(60, 0.0f, 1.0f, 1.0f, "Lag Test via Microphone & Fast Fourier Transform"); 
	DrawStringS(40, 120, 1.0f, 1.0f, 1.0f, Status);
	if(showframes)
		DrawStringS(38, 180, 1.0f, 1.0f, 1.0f, "Press X to toggle to frames");
	else
		DrawStringS(38, 180, 1.0f, 1.0f, 1.0f, "Press X to toggle to milliseconds");
	DrawStringS(38, 190, 1.0f, 1.0f, 1.0f, "Press Y to toggle loop mode");
	DrawStringS(120, 200, 0.0f, 1.0f, 0.0f, DPres);
	sprintf(sr, "#CSR:#C%ghz", samplerate);
	DrawStringS(40, 200, 1.0f, 1.0f, 1.0f, sr);

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
					sprintf(Res, "#G%.2d#G Lag was #C%0.2f#C ms", i, Results[i-1]*(IsPAL ? PAL_FRAME_LEN : NTSC_FRAME_LEN));
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
	
	if(rec_buffer.overflow)
	{
		rec_buffer.overflow++;
#ifdef DEBUG_FFT
		dbglog(DBG_CRITICAL, "Prevented buffer overflow #%d by %d bytes\n", rec_buffer.overflow, len);
#endif
		return;
	}

	if(len > (rec_buffer.size - rec_buffer.pos))
	{
		rec_buffer.overflow = 1;
#ifdef DEBUG_FFT
		dbglog(DBG_CRITICAL, "Prevented Buffer overflow by %d bytes\n", len - (rec_buffer.size - rec_buffer.pos));
#endif
		return;
	}

	memcpy(rec_buffer.buffer + rec_buffer.pos, samples, len);
	rec_buffer.pos += len;
}

#define cleanSIPlag() \
	CleanRecordBuffer();\
	if(back) FreeImage(&back);\
	if(wave) FreeImage(&wave);\
	if(beep != SFXHND_INVALID)\
		snd_sfx_unload(beep);\
	fftw_cleanup();

void SIPLagTest()
{
	int				done = 0, state_machine = 0, frame_counter = 0;
	int				showframes = 0, accuracy = 1, cycle = 0, start_msg = 0;
	uint16			pressed;		
	ImagePtr		back = NULL, wave = NULL;
	sfxhnd_t		beep = SFXHND_INVALID;  
	controller		*st;
	maple_device_t	*sip = NULL;  
	double			Results[RESULTS_MAX], samplerate = 0;
	int				ResCount = 0, sampling = 0, tries = 0;
	char			DStatus[100];
	float			delta = 0.01f;
	char			*vmuMsg1 = "Lag v/Micr", *vmuMsg2 = "";

	DStatus[0] = 0x0;
	back = LoadKMG("/rd/back.kmg.gz", 0);
	if(!back)
	{
		cleanSIPlag();
		return;
	}

	wave = LoadKMG("/rd/1khz.kmg.gz", 0);
	if(!wave)
	{
		cleanSIPlag();
		return;
	}
	wave->alpha = 0.05f;

	beep = snd_sfx_load("/rd/Sample.wav");
	if(beep == SFXHND_INVALID)
	{
		cleanSIPlag();
		return;
	}

	memset(&rec_buffer, 0, sizeof(struct sip_recording_st));
	rec_buffer.buffer = malloc(sizeof(uint8)*SIP_BUFFER_SIZE);
	if(!rec_buffer.buffer)
	{
		cleanSIPlag();
		return;
	}
	memset(rec_buffer.buffer, 0, sizeof(uint8)*SIP_BUFFER_SIZE);
	rec_buffer.size = sizeof(uint8)*SIP_BUFFER_SIZE;
	rec_buffer.pos = 0;
	rec_buffer.overflow = 0;
	rec_buffer.recording = 0;

	sip = maple_enum_type(0, MAPLE_FUNC_MICROPHONE);
	if(!sip)
	{
		DrawMessage("No #YSIP Microphone#Y found");
		cleanSIPlag();
		return;
	}
	/*
		There are two types of SIP I've come across, the regular NTSC only and 
		the NTSC/PAL one included in the DreamEye.
		
		According to their extra info, sample rates differ, and they did.
		Select thd proper one based on function_data[0]
		
		========================================================================
		Official US Microphone                              HKT-7200 315-6182 S4
		========================================================================
		Functions     : 0x10000000  Function int 0: 0x0f000000
		Function int 1: 0x00000000  Function int 2: 0x00000000
		Region:         0x03        Connection:     0x01
		Product Name & License: SoundInputPeripheral (S.I.P.)
		  Produced By or Under License From SEGA ENTERPRISES,LTD.
		Standby power: 0x012c (300mW) Max: 0x012c (300mW)
		Extra data:
		000 | 56 65 72 73 69 6f 6e 20 31 2e 30 30 30 2c 31 39 | Version 1.000,19
		010 | 39 38 2f 30 35 2f 32 32 2c 33 31 35 2d 36 31 38 | 98/05/22,315-618
		020 | 32 20 20 20 20 20 20 20 53 34 28 53 65 67 61 53 | 2       S4(SegaS
		030 | 6f 75 6e 64 53 61 6d 70 6c 69 6e 67 53 79 73 74 | oundSamplingSyst
		040 | 65 6d 29 2f 53 61 6d 70 6c 69 6e 67 52 61 74 65 | em)/SamplingRate
		050 | 31 31 2e 30 32 35 37 6f 72 38 2e 30 38 35 6b 48 | 11.0257or8.085kH
		060 | 7a 2f 42 69 74 31 34 6f 72 38 62 69 74 2f 4e 54 | z/Bit14or8bit/NT
		070 | 53 43 20 6f 6e 6c 79 2f 43 6f 6e 73 75 6d 65 72 | SC only/Consumer
		080 | 20 44 65 76 65 72 6f 70 6d 65 6e 74 20 26 20 4d |  Deveropment & M
		090 | 61 6d 75 66 61 63 74 75 72 69 6e 67 20 44 49 56 | amufacturing DIV
		0a0 | 2e 43 53 50 44 32 2d 33 20 20 20 20 00 00 00 00 | .CSPD2-3
		0b0 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
		0c0 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
		0d0 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
		0e0 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
		End of Extra data
		
		========================================================================
		Official DreamEye Microphone                      HKT-9800 315-6182A S41
		========================================================================
		Functions     : 0x10000000  Function int 0: 0x3f000000
		Function int 1: 0x00000000  Function int 2: 0x00000000
		Region:         0x0f        Connection:     0x01
		Product Name & License: MicDevice for Dreameye
		  Produced By or Under License From SEGA ENTERPRISES,LTD.
		Standby power: 0x012c (300mW) Max: 0x012c (300mW)
		Extra data:
		000 | 56 65 72 73 69 6f 6e 20 31 2e 30 30 30 2c 32 30 | Version 1.000,20
		010 | 30 30 2f 30 32 2f 32 34 2c 33 31 35 2d 36 31 38 | 00/02/24,315-618
		020 | 32 41 20 20 20 20 20 20 53 34 31 28 53 65 67 61 | 2A      S41(Sega
		030 | 53 6f 75 6e 64 53 61 6d 70 6c 69 6e 67 53 79 73 | SoundSamplingSys
		040 | 74 65 6d 29 2f 53 61 6d 70 6c 69 6e 67 52 61 74 | tem)/SamplingRat
		050 | 65 31 30 2e 39 30 39 30 6f 72 38 2e 30 30 30 6b | e10.9090or8.000k
		060 | 48 7a 2f 42 69 74 31 34 6f 72 38 62 69 74 2f 4e | Hz/Bit14or8bit/N
		070 | 54 53 43 2f 50 41 4c 2f 43 6f 6e 73 75 6d 65 72 | TSC/PAL/Consumer
		080 | 20 44 65 76 65 72 6f 70 6d 65 6e 74 20 26 20 4d |  Deveropment & M
		090 | 61 6d 75 66 61 63 74 75 72 69 6e 67 20 44 49 56 | amufacturing DIV
		0a0 | 2e 43 53 52 44 20 20 20 20 20 20 20 00 00 00 00 | .CSRD
		0b0 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
		0c0 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
		0d0 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
		0e0 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
		End of Extra data
	*/

	if(sip->info.function_data[0] & (3<<28))		// Version 1.000, 2000/02/24,315-618
		samplerate = 10909;
	else
	{
		samplerate = 11025;
		/*
		// I tried it and it works....
		if(IsPAL)
		{
			if(!AskQuestion("Your #YSIP#Y is for #CNTSC#C regions only,\nbehaviour is unknown. Proceed with test?"))
			{
				cleanSIPlag();
				return;
			}
		}
		*/
	}

	if(sip_set_gain(sip, SIP_MAX_GAIN) != MAPLE_EOK) // SIP_DEFAULT_GAIN  
	{
		DrawMessage("Could not set Microphone gain");
		cleanSIPlag();
		return;
	}
	if(sip_set_sample_type(sip, SIP_SAMPLE_16BIT_SIGNED) != MAPLE_EOK)
	{
		DrawMessage("Could not set Microphone sample type");
		cleanSIPlag();
		return;
	}
	if(sip_set_frequency(sip, SIP_SAMPLE_11KHZ) != MAPLE_EOK) // 10909 or 11.025kHz
	{
		DrawMessage("Could not set Microphone sample frequency");
		cleanSIPlag();
		return;
	}

	do
	{
		int sipretval = 0;
		
		sipretval = sip_start_sampling(sip, sip_copy, 1);
		if(sipretval == MAPLE_EOK)
			sampling = 1;
#ifdef DC_LOAD
		if(sipretval == MAPLE_ETIMEOUT)
			dbglog(DBG_INFO, "Microphone timed out\n");
		if(sipretval == MAPLE_EAGAIN)
			dbglog(DBG_INFO, "Microphone asked for retry\n");
#endif
		if(sipretval == MAPLE_EFAIL)
		{
			DrawMessage("Microphone Recording Failed, already sampling\nTry reconnecting it");
			cleanSIPlag();
			return;
		}
		tries ++;
	}while(!sampling && tries < 10);
	
	if(!sampling)
	{
		DrawMessage("Microphone Recording Failed");
		cleanSIPlag();
		return;
	}

	sprintf(DStatus, "Press A");
	while(!done && !EndProgram) 
	{	
		if(state_machine)
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

		DrawSIPScreen(back, wave, DStatus, accuracy, Results, ResCount, showframes, samplerate);
		VMURefresh(vmuMsg1, vmuMsg2);

		sip = maple_enum_type(0, MAPLE_FUNC_MICROPHONE);
		if(!sip)
		{
			DrawMessage("#YSIP Microphone#Y disconnected");
			done = 1;
		}
	
		st = ReadController(0, &pressed);
		if(st) 
		{
			if(state_machine == 0) // these are not allowed while sampling
			{
				if (pressed & CONT_START)
					ShowMenu(FFTHELP);
	
				if (pressed & CONT_B)
					done = 1;

				if (pressed & CONT_A)
					state_machine = 1;

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

			if(cycle && !state_machine)
				state_machine = 1;
		}

		if(state_machine == 2 || state_machine == 4)
		{
			// frame accurate sampling
			if(state_machine == 2 && frame_counter == CUE_FRAMES && !rec_buffer.recording)
				rec_buffer.recording = 1;

			frame_counter --;
			if(!frame_counter)
				state_machine++;
			
			if(state_machine == 4)
			{
				sprintf(DStatus, "Recording Frame: %d", (int)(SECONDS_TO_RECORD*(IsPAL ? PAL_FRAME_RATE : NTSC_FRAME_RATE) - frame_counter));
				if(frame_counter == 1)
					rec_buffer.recording = 0;

				if(rec_buffer.pos == 0)
				{
					sprintf(DStatus, "#YPlease remove and reinsert Microphone#Y");
					cycle = 0;
					rec_buffer.recording = 0;
					state_machine = 0;
					
					vmuMsg1 = "Reinsert";
					vmuMsg2 = "micro";
					refreshVMU = 1;
					start_msg = 1;
				}
			}
		}

		if(state_machine == 1 && sip)
		{
			sprintf(DStatus, "Starting");
			state_machine = 2;
			frame_counter = CUE_FRAMES;
		}

		if(state_machine == 3 && beep != SFXHND_INVALID)
		{
			snd_sfx_play(beep, 255, 128);
			state_machine = 4;
			frame_counter = SECONDS_TO_RECORD*(IsPAL ? PAL_FRAME_RATE : NTSC_FRAME_RATE); // record N seconds + CUE frames
		}
		
		if(state_machine == 5)
		{
			sprintf(DStatus, "Stopped sampling");

#ifdef DC_LOAD
			dbglog(DBG_INFO, "Got %d bytes (%d 16 bit samples)\n", 
					(int)rec_buffer.pos, (int)(rec_buffer.pos/2));
#endif
			if(rec_buffer.buffer && rec_buffer.pos > 0)
			{
				double value;

				DrawSIPScreen(back, wave, "Analyzing...", accuracy, Results, ResCount, showframes, samplerate);
				if(start_msg)
				{
					vmuMsg1 = "Analyzing";
					vmuMsg2 = "";
					start_msg = 0;
					refreshVMU = 1;
				}
				
				// Mic sample rate can vary between models, see above comment
				value = ProcessSamples((short*)rec_buffer.buffer, (int)(rec_buffer.pos/2),
					samplerate, (IsPAL ? PAL_FRAME_RATE : NTSC_FRAME_RATE)*accuracy, SEARCH_1KHZ);
				if(value < 0 && value != FFT_NOT_FOUND && value != FFT_OM)
				{
					sprintf(DStatus, "#YNoise at 1khz#Y");
					vmuMsg1 = "Noise at";
					vmuMsg2 = "1Khz";
					refreshVMU = 1;
				}
				if(value == FFT_OM)
				{
					sprintf(DStatus, "#ROut of Memory#R");
					vmuMsg1 = "Out of";
					vmuMsg2 = "memory";
					refreshVMU = 1;
				}
				if(value == FFT_NOT_FOUND)
				{
					sprintf(DStatus, "#YCheck audio system#Y");
					vmuMsg1 = "Check";
					vmuMsg2 = "audio";
					refreshVMU = 1;
				}
				if(value >= 0)
				{
					char vmtext[10];

					sprintf(DStatus, "Lag is #C%g#C frames\n       #C%0.2f#C ms",
						value, value*(IsPAL ? PAL_FRAME_LEN : NTSC_FRAME_LEN));
					if(showframes)
						sprintf(vmtext, "%g f", value);
					else
						sprintf(vmtext, "%g ms", value*(IsPAL ? PAL_FRAME_LEN : NTSC_FRAME_LEN));
					vmuMsg1 = "Lag is:";
					vmuMsg2 = vmtext;
					refreshVMU = 1;
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
				refreshVMU = 1;
			}

			rec_buffer.pos = 0;
			state_machine = 0;
		}
	}

	sip = maple_enum_type(0, MAPLE_FUNC_MICROPHONE);
	if(sip && sampling)
	{
		int sipretval = 0, retries = 0;
		
		tries = 0;
		do
		{	
			sipretval = sip_stop_sampling(sip, 1);
			if(sipretval == MAPLE_EAGAIN)
			{
				retries	++;
				timer_spin_sleep(10);
			}
			else if(sipretval != MAPLE_EOK)
				dbglog(DBG_ERROR, "Got %d from sip_stop_sampling\n", sipretval);

			tries++;
		}while(sipretval == MAPLE_EAGAIN && tries < 100);
		
		if(retries)
			dbglog(DBG_ERROR, "Got MAPLE_EAGAIN %d times from sip_stop_sampling\n", retries);
		if(sipretval != MAPLE_EOK && done != 2)
		{
			dbglog(DBG_ERROR, "Microphone recording could not be stopped\n");
			done = 2;
		}
	}
	
	if(done == 2)
		DrawMessage("#YPlease remove and reinsert Microphone#Y");

	cleanSIPlag();
	return;
}

double ProcessSamples(short *samples, size_t size, double samplerate, double secondunits, double searchfreq)
{
	long		  	samplesize = 0, arraysize = 0;	
	long		  	i = 0, f = 0, framesizernd = 0; 
	double		  	*in = NULL, root = 0, framesize = 0;  
	double		  	*LoudFreqArray = NULL;
	fftw_complex  	*out = NULL;
	fftw_plan		p = NULL;
	double	  		mins, maxs;
	double			boxsize = 0;
	int 			casefrq = 0;  
	int 			found = 0;
	long 			pos = 0, count = 0;    
	long 			tpos = 0, tcount = 0;	 
	double			value = FFT_NOT_FOUND;
#ifdef DEBUG_FFT
	uint64			start, end, time;
#endif
	
	samplesize = (long)size;  

	framesize = samplerate/secondunits;
	framesizernd = (long)framesize;  

#ifdef DEBUG_FFT
	dbglog(DBG_INFO, "Samples are at %g Khz and %g seconds long. A Frame is %g (%ld) samples.\n",
					samplerate, (double)samplesize/samplerate, framesize, framesizernd);

	start = timer_us_gettime64();
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
	LoudFreqArray = malloc(sizeof(double)*(samplesize/framesize - 1));
	if(!LoudFreqArray)
	{
		 fftw_free(in); 
		 fftw_free(out);
		 return FFT_OM;
	}
	memset(LoudFreqArray, 0, sizeof(double)*(samplesize/framesize - 1));

	boxsize = (double)arraysize/samplerate;  
	for(f = 0; f < samplesize/framesize - 1; f++)
	{
		double loudest_freq = 0, loudest = 0;
		long   loudest_ind = -1;
		long   framestart = 0;

		framestart = framesize*f;
		for(i = 0; i < framesizernd; i++)
			in[i] = (double)samples[i+framestart];

#ifdef DEBUG_FFT
		//dbglog(DBG_INFO, "Estimating plan\n");
#endif
		p = fftw_plan_dft_r2c_1d(arraysize, in, out, FFTW_ESTIMATE);
		if(!p)
		{
			fftw_free(in); 
			fftw_free(out);
			return FFT_OM;
		}
#ifdef DEBUG_FFT
		//dbglog(DBG_INFO, "Executing FFTW [%ld-%ld]\n", framestart, framestart+framesizernd);
#endif
		fftw_execute(p); 
	
#ifdef DEBUG_FFT
		//dbglog(DBG_INFO, " Analyzing results from frame %ld\n", f);
#endif
		root = sqrt(arraysize);
		for(i = 0; i < arraysize/2+1; i++)
		{
			double r1 = creal(out[i]);
			double i1 = cimag(out[i]);
			double amplitude = 0;
	
			amplitude = sqrt(sqrt(r1 * r1 + i1 * i1) / root);
			if(amplitude > loudest)
			{
				loudest = amplitude;
				loudest_ind = i;
			}
		}
		
		if(loudest_ind != -1)
			loudest_freq = (double)((double)loudest_ind/boxsize);
		LoudFreqArray[f] = loudest_freq;
		
		fftw_destroy_plan(p);
	}

#ifdef DEBUG_FFT
	end = timer_us_gettime64();
	time = end - start;
	dbglog(DBG_INFO, "FFT for %d frames took %g ms\n", (int)(samplesize/framesize - 1), (double)time/1000.0);
	start = end;
#endif

	casefrq = (int)ceil(searchfreq/(1/boxsize));
	mins = (casefrq - 1) / boxsize;
	maxs = (casefrq + 1) / boxsize;
#ifdef DEBUG_FFT
	dbglog(DBG_INFO, "Searching for %g, due to samplerate, arraysize and accurancy %d it is %g, between %g and %g\n",
		searchfreq, (int)(secondunits/(IsPAL ? PAL_FRAME_RATE : NTSC_FRAME_RATE)), casefrq/boxsize, mins, maxs);
#endif
 
	for(f = 0; f < samplesize/framesize - 1; f++)
	{
		if(!found)
		{
#ifdef DEBUG_FFT
			dbglog(DBG_INFO, "Frame %ld: Main frequency %g Hz\n", f-CUE_FRAMES, LoudFreqArray[f]);
#endif
			if(count)
			{
				if(LoudFreqArray[f] < mins || LoudFreqArray[f] > maxs)
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
				
			if(!count && LoudFreqArray[f] >= mins && LoudFreqArray[f] <= maxs)
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
		dbglog(DBG_INFO, "Found (heuristic %ld) at %g frames -> %g sec\n", tcount, value, pos/secondunits);
#endif
	}

#ifdef DEBUG_FFT
	end = timer_us_gettime64();
	time = end - start;
	dbglog(DBG_INFO, "Processing frequencies took %g ms\n", (double)time/1000.0);
#endif

	if(LoudFreqArray)
		free(LoudFreqArray);
	LoudFreqArray = NULL;  
	
	if(in)
		fftw_free(in); 
	if(out)
		fftw_free(out);
	return(value);
}

#endif
