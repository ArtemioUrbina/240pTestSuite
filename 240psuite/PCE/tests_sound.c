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

#ifdef SYSCARD1 

#include "huc.h"
#include "res.h"
#include "font.h"
#include "video.h"
#include "tests.h"
#include "help.h"
#include "tools.h"
#include "float.h"

char palCD[512];

extern int xres_flags;
extern unsigned char Enabled240p;
extern unsigned char UseDefault;
extern unsigned char EnabledSoft;
extern unsigned char Enabled_C_BW;
#endif

#ifdef SYSCARD1
void main()
{
	RestoreGlobals();
	
	switch(ToolItem)
	{
		case TOOL_SOUND:
			SoundTest();
			break;
		case TOOL_AUDIOSYNC:
			AudioSyncTest();
			break;
		case TOOL_MDFOURIER:
			MDFourier(x_g);
			break;
		case TOOL_MDFOURIER:
			MDFourier(x_g);
			break;
		case TOOL_CON_INTER:
			ConstrInterf();
			break;
		case HARDWARETOOL:
			HardwareTests();
			break;
		case TOOL_REFLEX:
			ReflexTest();
			break;
	}
	if(ToolItem == HARDWARETOOL)
		ToolItem = 0;
	cd_execoverlay(MAIN_OVERLAY);
}
#endif

#include "tests_audio_aux.c"

// This enables the ADPCM 32, 16, 8, 4 and 2 khz tests
//#define ADPCMDEBUG
#define SetupToneCommand(command) PSG_SetWaveFreq(0, PULSE_INTERNAL_FREQ); PSG_PlayCenter(0); vsync(); PSG_StopAudio(0); command;
#define WaitFrameCommand(command) vsync(); PSG_StopAudio(0); command;

void ChangeFilterMDF(int setValue)
{
	ChangeCompFilter(setValue, setValue);
	Set320H();
}

#ifdef CDROM
#ifdef ADPCMDEBUG
const unsigned char adpcm_val[5] = { 15, 14, 12, 8,  0 }; // ad_play values for 32, 16, 8, 4, 2 khz
const unsigned char adpcm_len[5] = { 1,   2,  4, 8, 16 }; // lenths multiples of 280 frames

void adpcmcycle()
{
	x3 = 0;
	
	do
	{
		WaitFrameCommand(ad_play(0, 64000, adpcm_val[x3], 0));
		
		//Wait for ADPCM to end
		WaitNFrames(280*adpcm_len[x3]);
		vsync();
		x3 ++;
	}while(x3 < 5);
}
#endif
#endif

void MDFourierExecute()
{
	x3 = 0; // check composite filter flag
	
	PSG_StopAllAudio();

	// Use XRES_SHARP @ 16.7145 ms per frame
#ifdef ALLOW_MDF_NOFILTER
	if(!EnabledSoft)
	{
		if(type == FLOAT_YES)
			ChangeFilterMDF(1);
	}
#else
	if(!EnabledSoft)
		ChangeFilterMDF(1);
#endif

#ifdef CDROM
	if(cd_status(0) != 0)
	  cd_pause();
	  
	if(ad_stat())
		ad_stop();
#endif

	vsync();
	ExecutePulseTrain(0);
	ExecuteSilence();
	
	
	PlayRampChannel(0);  // sine 1x
	PlayRampChannel(1);  // sine 4x

	/*
		Extra detailed test

	PlayRampWithVolume();
	PlayRampWithPan();
	PlayRampWithGlobalPan();
	PlayRampCombined();
	*/
	
	PSG_PlayCenter(4);
	PSG_SetNoiseFreq(4, 0);
	WaitNFrames(200);
	PSG_StopNoise(4);
	
	ExecuteSilence();
	ExecutePulseTrain(0);
	
#ifdef CDROM
	
	// Wait PAUSE
	WaitNFrames(4);

// ADPCM

#ifndef ADPCMDEBUG		
	WaitFrameCommand(ad_play(0, 64000, 15, 0));
	
	//Wait for ADPCM to end
	WaitNFrames(280);
	SetupToneCommand(vsync());
#else
	adpcmcycle();
#endif
	
	SetupToneCommand(vsync());

	clock_reset();
	// CD-DA	
	x2 = clock_tt();
	SetupToneCommand(cd_playtrk(4, 5, CDPLAY_NORMAL));
	x4 = clock_tt();
	y = x4-x2;
	
	set_font_pal(14);
	put_string("Play command delay", 4, 14);
	put_number(y, 4, 4, 15);
	
	//Wait for the cd audio track to end
	WaitNFrames(640);

	x2 = clock_tt();
	SetupToneCommand(cd_pause());
	x4 = clock_tt();
	y = x4-x2;
	
	put_string("Pause frames", 4, 16);
	put_number(y, 4, 4, 17);
	
#endif

	if(x3) // restore it to disabled
		ChangeFilterMDF(0);
}

void RedrawMDF()
{
	RedrawBG();

	put_string("MDFourier", 16, 4);
	refresh = 1;
	redraw = 0;
	disp_sync_on();
}

void MDFourier(int armed)
{
 	end = 0;
	redraw = 1;
	refresh = 1;
#ifdef ALLOW_MDF_NOFILTER
	type = FLOAT_NO;
#endif
	
#ifdef SCDROM
	put_string("MDFourier", 16, 4);
	armed = runmdf;
#endif

#ifdef CDROM
	ad_reset();
	
	ad_trans(ADPCM_SWEEP_OVL, 0, 32, 0);
#endif
	PSG_LoadWave(0, sine1x);
	PSG_LoadWave(1, sine4x);
	/*
		Extra detailed test 

	for(y = 0; y < 6; y+=2)
	{
		PSG_LoadWave(y  , sine1x);
		PSG_LoadWave(y+1, sine4x);
	}
	*/

	/* 	Some emulators and FPGA implementations
		have issues with the first frame of the
		first sound that is played back
		Make a decoy tone when entering the menu, before the tests.
	*/
	
	PSG_SetWaveFreq(0, PULSE_SKIP_EMU);
	PSG_PlayCenter(0);
	vsync();
	PSG_StopAudio(0);
	
	if(armed == 2)
		MDFourierExecute();
	
	if(armed == 1)
		ExecutePulseTrain(0);
	
	while(!end)
	{
		vsync();
		
		if(redraw)
			RedrawMDF();
		
		if(refresh)
		{
			set_font_pal(14);
			put_string("Start recording and press I", 6, 12);
			set_font_pal(13);
			put_string("Press START for HELP", 10, 26);
		}

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(MDFOURIER_HELP);
			redraw = 1;
		}
		
		if (controller & JOY_II)
			end = 1;
		
		if (controller & JOY_I)
		{
#ifdef ALLOW_MDF_NOFILTER
			if(!EnabledSoft)
			{
				type = SetFMYesNo("Enable Filter?", "No composite filter affects framerate");
				RedrawMDF();
			}	
			else
				type = FLOAT_NO;
#endif
			set_font_pal(15);
			put_string("Please wait while recording", 6, 12);
#ifdef ALLOW_MDF_NOFILTER
			if(!EnabledSoft && type != FLOAT_YES)
			{
				set_font_pal(13);
				put_string("Composite Filter disabled", 6, 15);
				put_string("Will be around 16.6518ms", 6, 16);
			}
#else
			if(!EnabledSoft)
			{
				set_font_pal(13);
				put_string("Composite Filter enabled", 6, 15);
				put_string("Will turn off after test", 6, 16);
			}
#endif

			MDFourierExecute();
			set_font_pal(13);
			put_string("Stop recording and press I ", 6, 12);
			
			do {
				controller = joytrg(0);
				vsync();
			} while(!(controller & JOY_I));
			redraw = 1;
		}
    }
	if(armed)
		redraw = 1;
}

void ConstrInterf()
{
 	end = 0;
	redraw = 1;
	refresh = 1;

#ifdef CDROM
	ad_reset();
	
	ad_trans(ADPCM_8khz_OVL, 0, 32, 0);
#endif
	PSG_LoadWave(0, sine1x);
	PSG_LoadWave(1, sine1x);
	PSG_LoadWave(2, sine1x);
	PSG_LoadWave(3, sine4x);
	PSG_LoadWave(4, sine4x);
	  
	/* 	Some emulators and FPGA implementations
		have issues with the first frame of the
		first sound that is played back
		Do so before the tests.
	*/
	
	PSG_SetWaveFreq(0, PULSE_SKIP_EMU);
	PSG_PlayCenter(0);
	vsync();
	PSG_StopAudio(0);
	
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			RedrawBG();
         
            put_string("Audio Clipping", 12, 4);
			refresh = 1;
            redraw = 0;
			disp_sync_on();
        }
		
		if(refresh)
		{
			set_font_pal(14);
            put_string("Start recording and press I", 6, 12);
			set_font_pal(13);
			put_string("Press START for HELP", 10, 26);
		}

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(CONSINTER_HELP);
			redraw = 1;
		}
		
		if (controller & JOY_II)
			end = 1;
		
		if (controller & JOY_I)
		{
			set_font_pal(15);
            put_string("Please wait while recording", 6, 12);
			ExecuteConstrInterf();
			set_font_pal(13);
			put_string("Stop recording and press I ", 6, 12);
			
			
			do {
				controller = joytrg(0);
				vsync();
			} while(!(controller & JOY_I));
			redraw = 1;
		}
    }
}

void WaitNFrames(int frames)
{
	for(i = 0; i < frames; i++)	
		vsync();
}

void RandomOnChannel(int channel)
{
	if(random(2))
		PSG_PlayCenter(channel);
	else
		PSG_StopAudio(channel);
}

void RandomizeChannels()
{
	for(i = 0; i < 5; i++)
		RandomOnChannel(i);
}

#ifdef CDROM
void RandomizeADPCM()
{
	if(random(2))
		ad_play(0, 64000, 15, 0x80);
	else
		ad_stop();
}
#endif

void PlayAndWait(int channel)
{
	PSG_PlayCenter(channel);
	WaitNFrames(30);
}

void SetContInterFreq()
{
	// PSG
	PSG_SetWaveFreq(0, 224);  // at around 500khz
	PSG_SetWaveFreq(1, 112);  // at around 1khz
	PSG_SetWaveFreq(2, 56);  // at around 2khz
	PSG_SetWaveFreq(3, 112);  // at around 4khz, was 28 at 1x
	PSG_SetWaveFreq(4, 56);  // at around 8khz, was 14 at 1x
	
	PSG_SetNoiseFreq(5, 0);
}

void ExecuteConstrInterf()
{
	srand(clock_tt());
	PSG_StopAllAudio();

	// HuCard
	vsync();
	ExecutePulseTrain(0);
	ExecuteSilence();

	SetContInterFreq();

	// PSG and Noise
	for(x = 0; x < 6; x++)
		PlayAndWait(x);
	
	for(x = 0; x < 200; x++)
	{
		RandomizeChannels();
		WaitNFrames(3);
	}
	
	PSG_StopAllAudio();
	PSG_StopNoise(5);
	
	ExecuteSilence();
	ExecutePulseTrain(0);

#ifdef CDROM
	// CD-ROM ROM
	// test starts now after framerate
	ExecuteSilence();
	SetContInterFreq();  // reset Pulse tone freq
	
	// PSG and Noise
	for(x = 0; x < 6; x++)
		PlayAndWait(x);
	
	// ADPCM at 8khz
	ad_play(0, 64000, 15, 0x80);
	WaitNFrames(30);

	clock_reset();
	// CD-DA at 16khz
	x2 = clock_tt();
	SetupToneCommand(cd_playtrk(5, 6, CDPLAY_NORMAL));	
	x3 = clock_tt();
	y = x3-x2;  // holds frame length for play
	
	//Wait for the cd audio track for a bit
	WaitNFrames(120);  
	
	for(x = 0; x < 200; x++)
	{
		RandomizeChannels();
		RandomizeADPCM();
		WaitNFrames(3);
	}
	
	ad_stop();
	PSG_StopAllAudio();
	PSG_StopNoise(5);
	
	/*
	set_font_pal(14);
	put_string("Play frames", 4, 14);
	put_number(y, 4, 4, 15);
	
	WaitNFrames(30);  
	*/
#endif
}

void SoundTest()
{
	option = 1;

#ifdef CDROM
	// type is x2 for CDROM
	x2 = 0;
	// l,r,c is X3 fof CCDA
	x3 = 1;
#endif

	end = 0;
	redraw = 1;
	refresh = 0;
	i = 0;
	
#ifdef CDROM
	ad_reset();
	
	ad_trans(ADPCM_VOICE_OVL, 0, 5, 0);
#endif

	PSG_LoadWave(0, sine1x);
	PSG_SetWaveFreq(0, 112);
	
    while(!end)
    {   
		vsync();
        if(redraw)
		{
			RedrawBG();
			put_string("Sound Test", 15, 4);
			
            redraw = 0;
			refresh = 1;
			disp_sync_on();
		}
		
		if(refresh)
		{
#ifdef CDROM
			if(x2 == 0)
				set_font_pal(12);
			else
				set_font_pal(14);
			put_string("PSG", 18, 8);
			if(x2 == 1)
				set_font_pal(12);
			else
				set_font_pal(14);
			put_string("CDDA", 18, 14);
			if(x2 == 2)
				set_font_pal(12);
			else
				set_font_pal(14);
			put_string("ADPCM", 18, 20);
			
			// PSG
			if(x2 == 0)
			{
				set_font_pal(option == 0 ? 15 : 14);
				put_string("Left", 9, 10);
				set_font_pal(option == 1 ? 15 : 14);
				put_string("Center", 17, 12);
				set_font_pal(option == 2 ? 15 : 14);
				put_string("Right", 26, 10);
			}
			
			if(x2 != 0)
			{
				set_font_pal(14);
				put_string("Left", 9, 10);
				put_string("Center", 17, 12);
				put_string("Right", 26, 10);
			}
			
			// CDDA
			if(x2 != 1)
			{
				set_font_pal(14);
				put_string("Left", 9, 16);
				put_string("Center", 17, 18);
				put_string("Right", 26, 16);
			}
			
			if(x2 == 1)
			{
				set_font_pal(x3 == 0 ? 15 : 14);
				put_string("Left", 9, 16);
				set_font_pal(x3 == 1 ? 15 : 14);
				put_string("Center", 17, 18);
				set_font_pal(x3 == 2 ? 15 : 14);
				put_string("Right", 26, 16);
			}
#else
			set_font_pal(option == 0 ? 15 : 14);
            put_string("Left", 9, 14);
            set_font_pal(option == 1 ? 15 : 14);
            put_string("Center", 17, 16);
            set_font_pal(option == 2 ? 15 : 14);
            put_string("Right", 26, 14);
#endif
			refresh = 0;
		}

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			PSG_StopAudio(0);
			showHelp(SOUND_HELP);
			redraw = 1;
		}
        
		if (controller & JOY_II)
			end = 1;
			
		if (controller & JOY_I)
		{
#ifdef CDROM
			if(x2 == 0)
			{
#endif
				switch(option)
				{
					case 0:
						PSG_PlayLeft(0);
						break;
					case 1:
						PSG_PlayCenter(0);
						break;
					case 2:
						PSG_PlayRight(0);
						break;
				}
				i = 20;
#ifdef CDROM
			}
	
			if(x2 == 1)
			{
				if(cd_status(0) == 0)
				{
					switch(x3)
					{
						case 0:
							cd_playtrk(6, 7, CDPLAY_NORMAL);
							break;
						case 1:
							cd_playtrk(3, 4, CDPLAY_NORMAL);
							break;
						case 2:
							cd_playtrk(7, 8, CDPLAY_NORMAL);
							break;
						default:
							break;
					}
				}
				else
				  cd_pause();
			}
			
			if(x2 == 2)
			{
				if(ad_stat())
					ad_stop();
				ad_play(0, 9595, 14, 0);
			}
#endif
		}
		
#ifdef CDROM
		if(x2 == 0)
		{
#endif
			if (controller & JOY_LEFT)
			{
				option --;
				refresh = 1;
			}
				
			if (controller & JOY_RIGHT)
			{
				option ++;
				refresh = 1;
			}
		
			if(option < 0)
				option = 0;
			if(option > 2)
				option = 2;		
#ifdef CDROM
		}
		
		if(x2 == 1)
		{
			if (controller & JOY_LEFT)
			{
				x3 --;
				refresh = 1;
			}
				
			if (controller & JOY_RIGHT)
			{
				x3 ++;
				refresh = 1;
			}
		
			if(x3 < 0)
				x3 = 0;
			if(option > 2)
				x3 = 2;		
		}
#endif
	
#ifdef CDROM		
		if (controller & JOY_UP)
		{
			x2 --;
			refresh = 1;
		}
			
		if (controller & JOY_DOWN)
		{
			x2 ++;
			refresh = 1;
		}
	
		if(x2 < 0)
			x2 = 2;
		if(x2 > 2)
			x2 = 0;
#endif	
		if(i)
			i--;
			
		if(i == 1)
			PSG_StopAudio(0);
    }
	PSG_StopAllAudio(0);
	
#ifdef CDROM
	if(cd_status(0) != 0)
		cd_pause();
	if(ad_stat())
		ad_stop();
#endif
}


void AudioSyncTest()
{
	option = -1;
	speed = -1;
	
	y = 160;
	end = 0;
	redraw = 1;
	refresh = 0;
	
	PSG_LoadWave(0, sine1x);
	PSG_SetWaveFreq(0, 112);
    while(!end)
    {   
		vsync();
        if(redraw)
        {
			disp_off();
			ResetVideo();
			setupFont();

			SetFontColors(13, RGB(2, 2, 2), RGB(0, 6, 0), 0);
			
			for(x = 0; x < 16; x++)
				set_color(x, 0);
				
			set_color(2, RGB(7, 7, 7));
				
#ifndef SYSCARD1
			set_map_data(audiosync_map, 40, 32);
			set_tile_data(audiosync_bg);
			load_tile(0x1000);
			load_map(0, 0, 0, 0, 40, 32);
#else
			set_screen_size(SCR_SIZE_64x32); 
			cd_loadvram(GPHX_OVERLAY, OFS_audiosync_DATA_bin, 0x1000, SIZE_audiosync_DATA_bin);
			cd_loadvram(GPHX_OVERLAY, OFS_audiosync_BAT_bin, 0x0000, SIZE_audiosync_BAT_bin);
#endif

			init_satb();
			set_color_rgb(256, 0, 0, 0); 
			set_color_rgb(257, 7, 7, 7); 
#ifndef SYSCARD1
			load_vram(0x5000, LED_sp, 0x100);
#else
			cd_loadvram(GPHX_OVERLAY, OFS_LEDsprites_tile_bin, 0x5000, SIZE_LEDsprites_tile_bin);
#endif
			spr_make(0, 160, y, 0x5000+0x40, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_16x16, 0, 1);
			satb_update();
			
			Center224in240();
			
            redraw = 0;
			disp_sync_on();
		}

        controller = joytrg(0);
        
		if (controller & JOY_II)
			end = 1;
			
		if (controller & JOY_I)
		{
			refresh = !refresh;
			if(!refresh)
				option = 121;
			else
				y = 160;
		}
		
		if(refresh && option == -1)
		{
			option = 0;
			speed = -1;
		}
		
		if(option > -1)
		{
			option++;
			if(option <= 120)
			{
				y += speed;
				spr_set(0);				
				spr_x(160);
				spr_y(y);			
				satb_update();
			}
		}

		if(option >= 20 && option <= 120)
		{
			switch (option)
			{
			case 20:
				break;
			case 40:
				set_color(3, RGB(7, 7, 7));
				break;
			case 60:
				speed = 1;
				set_color(4, RGB(7, 7, 7));
				break;
			case 80:
				set_color(5, RGB(7, 7, 7));
				break;
			case 100:
				set_color(6, RGB(7, 7, 7));
				break;
			case 120:
				set_color(7, RGB(7, 7, 7));
				break;
			}
		}
		
		if(option == 120)
		{
			PSG_PlayCenter(0);
			set_color(0, RGB(7, 7, 7));
		}

		if(option == 122)
		{
			set_color(0, 0);
			
			for(x = 3; x < 8; x++)
				set_color(x, 0);

			PSG_StopAudio(0);
			option = -1;
		}
		
		if (controller & JOY_RUN)
		{
			PSG_StopAudio(0);
			showHelp(AUDIOSYNC_HELP);
			redraw = 1;
			refresh = 0;
			option = 121;
			y = 160;
		}
		
    }
	PSG_StopAudio(0);
}

#ifndef SCDROM
#include "tests_reflex.c"
#else
void ReflexTest()
{
	cd_execoverlay(REFLEXSCD_OVERLAY);
}
#endif

/*
 *
 *		Hardware Menu
 *
 *
 */
 

#define HPOS 5

void RefreshHardwareTests()
{
	//set_font_pal(12);
	//put_string("Hardware Tests", 14, 6);
	
	row = 14;

	drawmenutext(0, "Controller Test");
	drawmenutext(1, "Turbo Pad Counter");
	drawmenutext(2, "Memory Viewer");
	
	row = 22;
	DrawMenuBottom(3, 0);
}

void HardwareTests()
{
	redraw = 1;
	refresh = 1;

	sel = 0;
	end = 0;
	controller = 0;
	
	disp_off();
	while(!end)
	{
		checkblink();
		vsync();
		
#ifdef SYSCARD1
		if(!HelpItem)
		{
#endif
		if(redraw)
		{
			RedrawMain();

			refresh = 1;
			redraw = 0;
			disp_sync_on();
		}
		
		if(refresh)
		{	
			RefreshHardwareTests();
			refresh = 0;
		}

		controller = joytrg(0);
		
		if (controller & JOY_II)
			end = 1;
		
		if (controller & JOY_SEL)
		{
#ifdef SYSCARD1
			x_g = OPTIONS_AUD_HELP;
#endif
			Options();
			redraw = 1;
		}
		
		if (controller & JOY_DOWN) 
		{
			sel++;
			if(sel > 4)
				sel = 0;
			refresh = 1;
		}

		if (controller & JOY_UP) 
		{
			sel--;
			if(sel < 0)
				sel = 4;
			refresh = 1;
		}
		
		if (controller & JOY_RUN)
		{
			showHelp(GENERAL_HW_HELP);
			redraw = 1;
		}
#ifdef SYSCARD1
		}
		else
		{
			if(HelpItem)
			{
				sel = HelpItem - HARDWAREHELP;
				if(HelpItem != GENERAL_HW_HELP)
					controller = JOY_I;
				HelpItem = 0;
			}
		}
#endif
		
		if (controller & JOY_I)
		{
			disp_off();
			ResetVideo();
#ifdef SYSCARD1
			prev_select = sel;
#endif
			switch(sel)
			{
				case 0:
					ControllerTest();
					break;
				case 1:
					TurboPadCount();
					break;
				case 2:
					MemViewer(0x2000);
					break;
				case 3:
#ifdef SYSCARD1
					x_g = OPTIONS_HW_HELP;
#endif
					Options();
					break;
				case 4:
					showHelp(GENERAL_HW_HELP);
					break;
				case 5:
					end = 1;
					break;
			}
			if(sel != 5)
				end = 0;
				
			redraw = 1;	
			disp_off();
		}
	}
	end = 0;
#ifdef SYSCARD1
	xres_flags_g = xres_flags;
	Enabled240p_g = Enabled240p;
	UseDefault_g = UseDefault;
	EnabledSoft_g = EnabledSoft;
	Enabled_C_BW_g = Enabled_C_BW;
	
	prev_select = 3;
	ResetVideo();
#else
	sel = 3;
#endif
}


void MemViewer(unsigned int address)
{			
	char buffer[3];
	
	end = 0;
	redraw = 1;
	refresh = 0;
	
	option = 0;		// ascii
	
	mem = NULL;
	while(!end)
	{
		vsync();
		
		if(redraw)
		{
			x = 0;
			y = 0;

			mem = (unsigned char*)address;
				
			ResetVideo();
			load_default_font();
			set_color(0, 0);
			
			SetFontColors(14, RGB(7, 7, 7), 0, 0);	
			SetFontColors(15, RGB(7, 0, 0), 0, 0);	
			
			disp_sync_on();

			set_font_pal(15);
			put_hex(address    , 4, 36, 0);
			put_hex(address+448, 4, 36, 27);
			
			set_font_pal(14);
			for(y = 0; y < 28; y++)
			{
				if(!option)
				{
					for(x = 0; x < 16; x++)
						put_hex(mem[y*16+x], 2, x*2, y);
				}
				else
				{
					for(x = 0; x < 16; x++)
					{
						buffer[0] = 32;				// Space
						buffer[1] = 0;
						buffer[2] = 0;
						
						x1 = mem[y*16+x];
						if(x1 >= 32 && x1 <= 126)	// ASCII range
							buffer[0] = x1;
						put_string(buffer, x*2, y);
					}
				}
			}
			
			redraw = 0;
		}

		controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(MEMVIEW_HELP);
			redraw = 1;
		}
		
		if (controller & JOY_I)
		{
			option = !option;
			redraw = 1;
		}
		
		if (controller & JOY_II)
			end = 1;
		
		if (controller & JOY_LEFT)
		{
			if(address >= 0x1C0)
				address -= 0x1C0;
			else
				address = 0;
				
			redraw = 1;
		}
		
		if (controller & JOY_RIGHT)
		{
			if(address >= 0xFE3F)
				address = 0xFE3F;
			else
				address += 0x1C0;
			
			redraw = 1;
		}
		
		if (controller & JOY_UP)
		{
			if(address >= 0x2000)
				address -= 0x2000;
			else
				address = 0;
		
			redraw = 1;
		}
		
		if (controller & JOY_DOWN)
		{
			if(address >= 0xDFFF)
				address = 0xFE3F;
			else
				address += 0x2000;

			redraw = 1;
		}
	}
}

void contollerstr(int direction, char *str, int posx, int posy)
{
	set_font_pal(controller & direction ? 12 : 14);
	put_string(str, posx, posy);
}

void CheckController(int joypad, int y)
{
	x = 5;
	
	controller = joy(joypad);
	
	// Bug in HUC, can't tell if there are more real controllers connected
	if(joypad && controller && controller == joy(0))	
		return;
		
	contollerstr(JOY_UP, "Up", x+4, y);
	contollerstr(JOY_LEFT, "Left", x, y+1);
	contollerstr(JOY_RIGHT, "Right", x+6, y+1);
	contollerstr(JOY_DOWN, "Down", x+3, y+2);
	
	contollerstr(JOY_SEL, "SEL", x+12, y+1);
	contollerstr(JOY_RUN, "RUN", x+16, y+1);
	
	if(controller & JOY_TYPE6)
	{
		contollerstr(JOY_IV, "IV", x+20, y);
		contollerstr(JOY_V, "V", x+24, y);
		contollerstr(JOY_VI, "VI", x+27, y);
		contollerstr(JOY_III, "III", x+20, y+2);
	}
	else
	{
		contollerstr(0, "  ", x+20, y);
		contollerstr(0, " ", x+24, y);
		contollerstr(0, "  ", x+27, y);
		contollerstr(0, "   ", x+20, y+2);
	}

	contollerstr(JOY_II, "II", x+24, y+2);
	contollerstr(JOY_I, "I", x+27, y+2);
}

void ControllerTest()
{
	redraw = 1;
	
	disp_off();
	while(!end)
	{	
		vsync();
		
		if(redraw)
		{
			RedrawBG();
			SetFontColors(12, RGB(3, 3, 3), RGB(7, 0, 0), 0);
			SetFontColors(13, RGB(2, 2, 2), RGB(0, 6, 0), 0);
			SetFontColors(15, RGB(2, 2, 2), RGB(7, 7, 7), 0);

			refresh = 1;
			redraw = 0;
			disp_sync_on();
		}
		if(refresh)
		{
			set_font_pal(13);
			put_string("Controller Test", 12, 4);
			set_font_pal(15);
			put_string("Use RUN+LEFT to exit", 11, 26);
		}
		
		CheckController(0, 6);
		
		if(controller & JOY_LEFT && controller & JOY_RUN)
			end = 1;
			
		CheckController(1, 10);
		CheckController(2, 14);
		CheckController(3, 18);
		CheckController(4, 22);
	}
}

void showFrames(int xpos, int ypos)
{
	if(x4)
		put_number(600, 3, xpos, ypos);
	else
		put_number(60, 3, xpos, ypos);
}

void TurboPadCount()
{
	redraw = 1;
	end = 0;
	x4 = 0;  // 10 seconds
	
	disp_off();
	while(!end)
	{	
		vsync();
		
		if(redraw)
		{
			RedrawBG();
			SetFontColors(12, RGB(3, 3, 3), RGB(7, 0, 0), 0);
			SetFontColors(13, RGB(2, 2, 2), RGB(0, 6, 0), 0);
			SetFontColors(15, RGB(2, 2, 2), RGB(7, 7, 7), 0);

			refresh = 1;
			redraw = 0;
			disp_sync_on();
		}
		
		if(refresh)
		{
			set_font_pal(13);
			put_string("Turbo Pad Counter", 12, 4);
			set_font_pal(14);
			put_string("Hold \"I\" to measure", 8, 14);
			showFrames(16, 16);
		}
		
		controller = joytrg(0);
		
		if (controller & JOY_I)
		{
			x1 = 0;	// presses
			if(x4)
				x5 = 600; // frame counter
			else
				x5 = 60;
			x2 = 0;
			x3 = 0; // last frame pressed
			
			put_string("Measuring...        ", 8, 14);
			put_string("Count:", 8, 15);
			put_string("Frames:", 8, 16);
			vsync();
			
			do
			{	
				controller = joy(0);
				if (controller & JOY_I)
				{
					if(!x3)
					{
						x1 ++;
						x3 = 1;
					}
				}
				else
					x3 = 0;
	
				x2 ++;
				put_number(x1, 3, 16, 15);
				put_number(x2, 3, 16, 16);
				vsync();
			} while(x2 < x5);
			
			put_string("Results:    ", 8, 14);
			
			do
			{
				vsync();
				
				controller = joytrg(0);
			} while(!(controller & JOY_II));
			
			controller = 0;
			redraw = 1;
		}
		
		if (controller & JOY_UP)
		{
			x4 = !x4;
			refresh = 1;
		}
			
		if (controller & JOY_II)
			end = 1;
	}
}

#ifdef CDPLAYER
void CDDAPlayer()
{
	int	tracks[20];
	
	redraw = 1;
	
	x3 = 0;			//selected track
	end = 0;
	option = 1;  // read cd
	x = 0;		 // Number of tracks
	
	controller = 0;
	
	disp_off();
	while(!end)
	{		
		vsync();
		if(redraw)
		{
			RedrawBG();

			refresh = 1;
			redraw = 0;
			disp_sync_on();
		}
		if(refresh)
		{
			x2 = 4;
			y = 8;
			
			SetFontColors(15, RGB(3, 3, 3), RGB(0, 7, 0), 0);
			
			set_font_pal(13);
			put_string("CD Audio Player", 12, 4);
			put_string("No Context CD-DA", 12, 26);
			
			for(x1 = 1; x1 <= x; x1++)
			{
				if(tracks[x1-1] == CDTRK_AUDIO)
				{
					set_font_pal(x3 + 1 == x1 ? 15 : 14);
					put_number(x1, 2, x2, y);
				}
				
				x2 += 3;
				if(x2 >= 33)
				{
					x2 = 4;
					y++;
				}				
			}
			set_font_pal(x3 + 1 == x1 ? 15 : 14);
			y += 2;
			put_string("Play 3 to", 24, y);
			put_number(x, 2, 33, y);
			refresh = 0;
		}
		controller = joytrg(0);
		
		if (controller & JOY_II)
			end = 1;
		if(option) // read cd
		{
			x3 = 0;
			refresh = 1;
			option = 0;
			x = cd_numtrk();
			for(x1 = 1; x1 <= x; x1++)
				tracks[x1-1] = cd_trktype(x1);
		}
		if (controller & JOY_LEFT)
		{
			x3 --;
			
			if(tracks[x3] != CDTRK_AUDIO)
				x3 --;

			if(x3 < 0)
				x3 = 0;
			
			refresh = 1;
		}
		if (controller & JOY_RIGHT)
		{
			x3 ++;
			if(tracks[x3] != CDTRK_AUDIO)
				x3 ++;
			if(x3 > x)
				x3 = x;
				
			refresh = 1;
		}
		if (controller & JOY_UP)
		{
			x3 = x3 - 10;
			if(tracks[x3] != CDTRK_AUDIO)
				x3 --;
			if(x3 < 0)
				x3 = 0;
				
			refresh = 1;
		}
		if (controller & JOY_DOWN)
		{
			x3 = x3 + 10;
			if(tracks[x3] != CDTRK_AUDIO)
				x3 ++;
			if(x3 > x)
				x3 = x;
				
			refresh = 1;
		}
		
		if (controller & JOY_I)
		{
			if(x3 != x)
				cd_playtrk(x3+1, x3+2, CDPLAY_NORMAL);
			else
				cd_playtrk(3, CDPLAY_ENDOFDISC, CDPLAY_NORMAL);
		}
			
		if (controller & JOY_SEL)
			cd_reset();
	}
}
#endif
