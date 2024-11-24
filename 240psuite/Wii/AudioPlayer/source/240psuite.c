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
#include "controller.h"
#include "video.h"
#include "image.h"
#include "font.h"
#include "options.h"
#include "tests.h"


// Default options
struct options_st Options = DEFAULT_OPTIONS;
struct options_st DefaultOptions = DEFAULT_OPTIONS;

int EndProgram = 0;

ImagePtr 	sd_b1 = NULL, sd_b2 = NULL;
void SD_blink_cycle();

s8 HWButton = -1;
#ifdef WII_VERSION
void WiiResetPressed();
void WiiPowerPressed();
void WiimotePowerPressed(s32 chan);
#else
void GCResetPressed();
#endif

#include "dirent.h"
#include "ctype.h"

#define MAX_FILE_NAME_SIZE	256
#define MAX_FILE_COUNT		20

int FindAudioFiles(char *folder, char **names, unsigned int max_size){
	int count = 0;
	DIR *dirp = NULL;
	struct dirent *entry = NULL;
	
	if(!InitFS())
		return 0;
	
	dirp = opendir(folder);
	if(!dirp) {
		CloseFS();
		return 0;
	}
	while((entry = readdir(dirp)) != NULL) {
		if(max_size > count) {
			int len = 0;
			
			len = strlen(entry->d_name);
			if(len > 5) {
				if( toupper(entry->d_name[len-3]) == 'W' && 
					toupper(entry->d_name[len-2]) == 'A' && 
					toupper(entry->d_name[len-1]) == 'V') {
					strncpy(names[count], entry->d_name, MAX_FILE_NAME_SIZE);
					count ++;
				}
			}
		}
	}
	CloseFS();
	return count;
}

int main(int argc, char **argv) 
{
	int			close = 0;	
	ImagePtr	Back = NULL, sd = NULL;
	int			sel = 0, searchFiles = 1;
	int			filecount = 0, i = 0;
	char		*filenames[MAX_FILE_COUNT];

	for(i = 0; i < MAX_FILE_COUNT; i++) {
		filenames[i] = NULL;
	}
	
	for(i = 0; i < MAX_FILE_COUNT; i++) {
		filenames[i] = (char*)malloc(sizeof(char)*MAX_FILE_NAME_SIZE);
		if(!filenames[i])
			EndProgram = 1;
	}
	
#ifdef WII_VERSION
	SYS_SetResetCallback(WiiResetPressed);
	SYS_SetPowerCallback(WiiPowerPressed);
	WPAD_SetPowerButtonCallback(WiimotePowerPressed);
#else
	SYS_SetResetCallback(GCResetPressed);
#endif

	ControllerInit();

	LoadOptions();
	
	InitVideo();	

	if(!InitGX())
		return 0;
		
	SetupGX();

	LoadFont();
	
	Back = LoadImageMemCpy(BACKIMG, 0);
	if(!Back)
	{
		ReleaseFont();
		EndProgram = 1;
	}	
	sd = LoadImageMemCpy(SDIMG, 0);
	if(!sd)
	{
		ReleaseFont();
		EndProgram = 1;
	}	
	
	// load blinking graphics if appropiate
	if(sd)
	{
		sd->x = 230;
		sd->y = 90;		
		
		sd_b1 = LoadImageMemCpy(SD_B1_IMG, 0);
		if(sd_b1)
		{
			sd_b1->x = sd->x+16;
			sd_b1->y = sd->y+32;
		}
		
		sd_b2 = LoadImageMemCpy(SD_B2_IMG, 0);
		if(sd_b2)
		{
			sd_b2->x = sd->x+16;
			sd_b2->y = sd->y+32;
		}
	}
	
	while(!close && !EndProgram) 
	{
		u8		r = 0xff;
		u8		g = 0xff;
		u8		b = 0xff;									   
		u16 	x = 40;
		u16 	y = 60;
		u32 	pressed = 0;
	
		if(searchFiles) {
			i = 4;
			while(i--)
				DrawMessage(Back, "Searching for files", "Please wait...");
			filecount = FindAudioFiles(".", filenames, MAX_FILE_COUNT);
			searchFiles = 0;
		}

		StartScene();
				
		DrawImage(Back);
		DrawImage(sd);
		SD_blink_cycle();
		
		DrawStringS(34, 42, 0xff, 0xff, 0xff, "WAV file player"); 
		if(!filecount) {
			DrawStringS(x, y, r, 0,	0, "No wav files found");
			DrawStringS(x, y+fh, r, 0,	0, "Press X to scan for files");
		}
		else {
			for(i = 0; i < filecount; i++) {
				DrawStringS(x, y, r, sel == i ? 0 : g, sel == i ? 0 : b, filenames[i]);
				y += fh;
			}
		}
		
		EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);

		if(HWButton != -1)
			EndProgram = 1;
			
		if ( pressed & PAD_BUTTON_UP )
		{
			sel --;
			if(sel < 0)
				sel = filecount - 1;		
		}
		
		if ( pressed & PAD_BUTTON_DOWN )
		{
			sel ++;
			if(sel > filecount - 1)
				sel = 0;	
		}

		if (filecount && pressed & PAD_BUTTON_A)
		{		
			PlayAudioFile(Back, filenames[sel]);						
		}
		
		if (pressed & PAD_BUTTON_X)
			searchFiles	= 1;
	}
	
	for(i = 0; i < MAX_FILE_COUNT; i++) {
		free(filenames[i]);
		filenames[i] = NULL;
	}
			
	FreeImage(&Back);
	FreeImage(&sd);
	FreeImage(&sd_b1);
	FreeImage(&sd_b2);
	ReleaseFont();	
	
	EndGX();
	RestoreVideo();
	
#ifdef WII_VERSION
	if(HWButton != -1)
		SYS_ResetSystem(HWButton, 0, 0);
#endif

	return EXIT_SUCCESS;
}

void SD_blink_cycle()
{
	static int blink_counter = 0;
	static int is_blinking = 0;
	
	blink_counter++;	
	if(sd_b1 && sd_b2 && blink_counter > 230)
	{
		if(!is_blinking)
		{
			if(rand() % 100 < 98)	// 2% chance every frame after 240
			{
				is_blinking = 1;
				blink_counter = 230;
				DrawImage(sd_b1);
			}
		}
		else
		{
			if(blink_counter >= 232 && blink_counter < 234)
				DrawImage(sd_b2);
				
			if(blink_counter >= 234 && blink_counter < 238)
				DrawImage(sd_b1);
	
			if(blink_counter >= 238)
			{	
				blink_counter = 0;
				is_blinking = 0;
			}
		}
	}
}

#ifdef WII_VERSION
/**
 * Callback for the reset button on the Wii.
 */
void WiiResetPressed()
{
	HWButton = SYS_RETURNTOMENU;
}
 
/**
 * Callback for the power button on the Wii.
 */
void WiiPowerPressed()
{
	HWButton = SYS_POWEROFF;
}
 
/**
 * Callback for the power button on the Wiimote.
 * @param[in] chan The Wiimote that pressed the button
 */
void WiimotePowerPressed(s32 chan)
{
	HWButton = SYS_POWEROFF;
}

#else
/**
 * Callback for the reset button on the GameCube.
 */
void GCResetPressed()
{
	HWButton = SYS_HOTRESET;
}

#endif
