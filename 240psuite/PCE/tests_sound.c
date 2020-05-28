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

#ifdef CDROM1 

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

#ifdef CDROM1
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
		case TOOL_MANUAL:
			ManualLagTest();
			break;
		case HARDWARETOOL:
			HardwareTests();
			break;
	}
	if(ToolItem == HARDWARETOOL)
		ToolItem = 0;
	cd_execoverlay(MAIN_OVERLAY);
}
#endif

#include "tests_audio_aux.c"

#define SetupToneCommand(command) SetWaveFreq(0, PULSE_INTERNAL_FREQ); PlayCenter(0); vsync(); StopAudio(0); command;

void MDFourierExecute()
{
	StopAllAudio();
	
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
	
	PlayCenter(4);
	SetNoiseFreq(4, 0);
	for(i = 0; i < 200; i++)	
		vsync();
	StopNoise(4);
	
	ExecuteSilence();
	ExecutePulseTrain(0);
	
#ifdef CDROM
	
	// Wait PAUSE
	for(i = 0; i < 4; i++)	
		vsync();
		
	// ADPCM
	//
	SetupToneCommand(ad_play(0, 64000, 15, 0));
	
	//Wait for ADPCM to end
	for(i = 0; i < 280; i++)	
		vsync();
	SetupToneCommand(vsync());

	clock_reset();
	// CD-DA	
	x2 = clock_tt();
	SetupToneCommand(cd_playtrk(4, 5, CDPLAY_NORMAL));
	//cd_playtrk(4, 5, CDPLAY_NORMAL);
	x3 = clock_tt();
	
	put_string("Play", 4, 14);
	put_number(x3-x2, 4, 4, 15);
	
	//Wait for the cd audio track to end
	for(i = 0; i < 640; i++)	
		vsync();

	SetupToneCommand(cd_pause());
	
#endif

}

void MDFourier(int armed)
{
 	end = 0;
	redraw = 1;
	refresh = 1;
	
#ifdef SCDROM
	armed = runmdf;
#endif

#ifdef CDROM
	ad_reset();
	
	ad_trans(ADPCM_SWEEP_OVL, 0, 32, 0);
#endif
	LoadWave(0, sine1x);
	LoadWave(1, sine4x);
	  
	/* 	Some emulators and FPGA implementations
		have issues with the first frame of the
		first sound that is played back
		Do so before the tests.
	*/
	
	SetWaveFreq(0, PULSE_SKIP_EMU);
	PlayCenter(0);
	vsync();
	StopAudio(0);
	
	if(armed)
		ExecutePulseTrain(0);
	
    while(!end)
    {   
		vsync();
		
        if(redraw)
        {
			RedrawBG();
         
            put_string("MDFourier", 16, 4);
			refresh = 1;
            redraw = 0;
			disp_on();
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
			showHelp(MDFOURIER_HELP);
			redraw = 1;
		}
		
		if (controller & JOY_II)
			end = 1;
		
		if (controller & JOY_I)
		{
			set_font_pal(15);
            put_string("Please wait while recording", 6, 12);
			MDFourierExecute();
			set_font_pal(13);
			put_string("Stop recording and press I ", 6, 12);
			
			
			do {
				controller = joytrg(0);
				vsync();
			} while(!(controller & JOY_I));
			refresh = 1;
		}
		
		if(controller & JOY_SEL)
			PlayAllChannelsAtOnce(3);
    }
	if(armed)
		redraw = 1;
}

void PlayAllChannelsAtOnce()
{

	StopAllAudio();
	
#ifdef CDROM
	if(cd_status(0) != 0)
	  cd_pause();
	  
	if(ad_stat())
		ad_stop();
#endif

	vsync();
	// PSG
	SetWaveFreq(1, 112);  // at around 1khz
	PlayCenter(1);
	
	// Noise
	SetNoiseFreq(4, 0);
	PlayCenter(4);
	
#ifdef CDROM
	// ADPCM
	ad_play(0, 64000, 15, 0);
	// CD-DA	
	cd_playtrk(4, 5, CDPLAY_NORMAL);
	//Wait for the cd audio track to end
	for(i = 0; i < 400; i++)	
		vsync();
		
	cd_pause();
#else
	// Wait
	for(i = 0; i < 200; i++)	
		vsync();
#endif
	StopAudio(1);
	StopNoise(4);
}

void SoundTest()
{
	option = 1;

#ifdef CDROM
	// type is x2 for CDROM
	x2 = 0;
#endif

	end = 0;
	redraw = 1;
	refresh = 0;
	i = 0;
	
#ifdef CDROM
	ad_reset();
	
	ad_trans(ADPCM_VOICE_OVL, 0, 5, 0);
#endif

	LoadWave(0, sine1x);
	SetWaveFreq(0, 112);
	
    while(!end)
    {   
		vsync();
        if(redraw)
		{
			RedrawBG();
			put_string("Sound Test", 15, 4);
			
            redraw = 0;
			refresh = 1;
			disp_on();
		}
		
		if(refresh)
		{
#ifndef CDROM
			set_font_pal(option == 0 ? 15 : 14);
            put_string("Left", 9, 14);
            set_font_pal(option == 1 ? 15 : 14);
            put_string("Center", 17, 16);
            set_font_pal(option == 2 ? 15 : 14);
            put_string("Right", 26, 14);
#else

			if(x2 == 0)
				set_font_pal(12);
			else
				set_font_pal(14);
			put_string("PSG", 18, 8);
			if(x2 == 1)
				set_font_pal(12);
			else
				set_font_pal(14);
			put_string("CDDA", 18, 18);
			if(x2 == 2)
				set_font_pal(12);
			else
				set_font_pal(14);
			put_string("ADPCM", 18, 20);
			
			if(x2 == 0)
			{
				set_font_pal(option == 0 ? 15 : 14);
				put_string("Left", 9, 10);
				set_font_pal(option == 1 ? 15 : 14);
				put_string("Center", 17, 12);
				set_font_pal(option == 2 ? 15 : 14);
				put_string("Right", 26, 10);
			}
			else
			{
				set_font_pal(14);
				put_string("Left", 9, 10);
				put_string("Center", 17, 12);
				put_string("Right", 26, 10);
			}
#endif
			refresh = 0;
		}

        controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			StopAudio(0);
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
						PlayLeft(0);
						break;
					case 1:
						PlayCenter(0);
						break;
					case 2:
						PlayRight(0);
						break;
				}
				i = 20;
#ifdef CDROM
			}
	
			if(x2 == 1)
			{
				if(cd_status(0) == 0)
					cd_playtrk(3, 4, CDPLAY_NORMAL);
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
			StopAudio(0);
    }
	StopAllAudio(0);
	
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
	
	LoadWave(0, sine1x);
	SetWaveFreq(0, 112);
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
				
#ifndef CDROM1
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
#ifndef CDROM1		
			load_vram(0x5000, LED_sp, 0x100);
#else
			cd_loadvram(GPHX_OVERLAY, OFS_LEDsprites_tile_bin, 0x5000, SIZE_LEDsprites_tile_bin);
#endif
			spr_make(0, 160, y, 0x5000+0x40, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_16x16, 0, 1);
			satb_update();
			
			Center224in240();
			
            redraw = 0;
			disp_on();
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
			PlayCenter(0);
			set_color(0, RGB(7, 7, 7));
		}

		if(option == 122)
		{
			set_color(0, 0);
			
			for(x = 3; x < 8; x++)
				set_color(x, 0);

			StopAudio(0);
			option = -1;
		}
		
		if (controller & JOY_RUN)
		{
			StopAudio(0);
			showHelp(AUDIOSYNC_HELP);
			redraw = 1;
			refresh = 0;
			option = 121;
			y = 160;
		}
		
    }
	StopAudio(0);
}

#ifndef SCDROM2
#include "tests_manual.c"
#else

void ManualLagTest()
{
	cd_execoverlay(MANUALLAGSCD_OVERLAY);
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
	
	row = 12;

	drawmenutext(0, "Controller Test");
	drawmenutext(1, "Memory Viewer");
	
	row++;
	DrawMenuBottom(2, 0);
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
		vsync();
		
#ifdef CDROM1
		if(!HelpItem)
		{
#endif
		if(redraw)
		{
			RedrawMain();

			refresh = 1;
			redraw = 0;
			disp_on();
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
#ifdef CDROM1
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
#ifdef CDROM1
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
#ifdef CDROM1
			prev_select = sel;
#endif
			switch(sel)
			{
				case 0:
					ControllerTest();
					break;
				case 1:
					MemViewer(0x2000);
					break;
				case 2:
#ifdef CDROM1
					x_g = OPTIONS_HW_HELP;
#endif
					Options();
					break;
				case 3:
					showHelp(GENERAL_HW_HELP);
					break;
				case 4:
					end = 1;
					break;
			}
			if(sel != 4)
				end = 0;
				
			redraw = 1;	
			disp_off();
		}
	}
	end = 0;
#ifdef CDROM1
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
	end = 0;
	redraw = 1;
	refresh = 0;
	
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
			
			disp_on();

			set_font_pal(15);
			put_hex(address    , 4, 36, 0);
			put_hex(address+448, 4, 36, 27);
			
			set_font_pal(14);
			for(y = 0; y < 28; y++)
			{
				for(x = 0; x < 16; x++)
					put_hex(mem[y*16+x], 2, x*2, y);
			}
			
			redraw = 0;
		}

		controller = joytrg(0);
		
		if (controller & JOY_RUN)
		{
			showHelp(MEMVIEW_HELP);
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
	clicks[joypad] = 1;
}

void ControllerTest()
{
	redraw = 1;
		
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
			disp_on();
		}
		if(refresh)
		{
			set_font_pal(13);
			put_string("Controller Test", 12, 4);
			set_font_pal(15);
			put_string("Use START+LEFT to exit", 9, 26);
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

