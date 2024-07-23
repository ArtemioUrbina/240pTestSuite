/* 
 * 240p Test Suite
 * Copyright (C)2014-2022 Artemio Urbina (Wii GX)
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
#include <stdlib.h>
#include "tests.h"
#include "image.h"
#include "font.h"
#include "video.h"

#include "controller.h"
#include "options.h"

void DrawMessage(ImagePtr back, char *title, char *msg)
{
	StartScene();
	DrawImage(back);

	DrawStringS((dW-fw*strlen(title))/2, 60, 0x00, 0xff, 0x00, title); 
	DrawStringS((dW-fw*strlen(msg))/2, 120, 0xff, 0xff, 0xff, msg);
	EndScene();
}

#include <aesndlib.h>

extern int EndProgram;

void aesnd_callback(AESNDPB *pb, unsigned int state, void *cb_arg)
{
	unsigned int *playback = (unsigned int *)cb_arg;
	
	if (state == VOICE_STATE_RUNNING) 
		*playback = 1;
	if(state == VOICE_STATE_STOPPED)
		*playback = 0;
}

void PlayAudioFile(ImagePtr back, char *filename)
{
	int 			done = 0, loaded = 0, counter = 2;
	u32				pressed;
	char			*msg = "Loading file...";
	char			*title = filename;
	u8				*aet_samples = NULL;
	ulong			aet_size = 0;
	AESNDPB			*voice = NULL;
	unsigned int	playback = 0;

	AESND_Init();
	
	voice = AESND_AllocateVoiceWithArg(aesnd_callback, &playback);
	if(!voice)
		return;
	AESND_SetVoiceVolume(voice, 0xff, 0xff);

	while(counter --)
		DrawMessage(back, title, msg);
	
	while(!done && !EndProgram) 
	{
		DrawMessage(back, title, msg);
		
		if(!loaded)
		{
			int errType = 0;
			
			aet_samples = LoadPCMToBuffer(filename, &aet_size, &errType);
			if(!aet_samples)
			{
				do{
					DrawMessage(back, title, errors[errType]);
					ControllerScan();
					
					pressed = Controller_ButtonsDown(0);
					if(pressed & PAD_BUTTON_A || pressed & PAD_BUTTON_B)
						done = 1;
				}while(!done);
				
				return;
			}

			loaded = 1;
			msg = "Press A to start";
		}
		
		if(counter)
		{
			counter --;
			if(counter == 0)
				msg = "Press A to start";
		}
		
		ControllerScan();
		pressed = Controller_ButtonsDown(0);
		if (pressed & PAD_BUTTON_B)
			done = 1;
			
		if (pressed & PAD_BUTTON_A)
		{
			if(loaded)
			{	
				int cancel = 0;
				
				msg = "Playing Audio File";

				AESND_PlayVoice(voice, VOICE_STEREO16, aet_samples, aet_size, DSP_DEFAULT_FREQ, 0, 0);
				
				while(playback != 1)
					DrawMessage(back, title, msg);
				
				while(playback && cancel != 2)
				{
					if(counter)
					{
						counter --;
						if(counter == 0)
						{
							msg = "Playing Audio File";
							cancel = 0;
						}
					}
					DrawMessage(back, title, msg);
					ControllerScan();
					pressed = Controller_ButtonsDown(0);
					if(pressed & PAD_BUTTON_B && cancel == 0)
					{
						msg = "Press A to abort";
						cancel = 1;
						counter = 120;
					}
					if(pressed & PAD_BUTTON_A && cancel == 1)
					{
						msg = "Playback cancelled";
						cancel = 2;
						AESND_SetVoiceStop(voice, true);
						counter = 120;
					}
				}
				
				if(cancel != 2)
					msg = "Playback Finished";
				DrawMessage(back, title, msg);
			}
		}
	}

	AESND_SetVoiceStop(voice, true);
	AESND_FreeVoice(voice);
	AESND_Reset();
	
	if(aet_samples) {
		free(aet_samples);
		aet_samples = NULL;
	}

	return;
}
