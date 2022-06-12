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
#include <string.h>
#include <stdlib.h>
#include "vmu_print.h"
#include "vmodes.h"
#include "vmu.h"

static 	uint8 bitmap[192];
uint64 	lcd_vmu_update_time = 0;
uint8	disableVMU_LCD_val = 0;
uint8	is_sleeping = 0;
uint8	sleepEnabled = 1;
extern	uint8 refreshVMU;

int isLCDPresent()
{
	maple_device_t	*lcd = NULL;

	lcd = maple_enum_type(0, MAPLE_FUNC_LCD);
	if(!lcd)
		return 0;
	return 1;
}

/* Return the Nth device of the requested type (where N is zero-indexed) */
maple_device_t * maple_enum_mixed_type(int n, uint32 func, uint32 extrafunc) {
    int p, u;
    maple_device_t *dev;

    for(p = 0; p < MAPLE_PORT_COUNT; p++) {
        for(u = 0; u < MAPLE_UNIT_COUNT; u++) {
            dev = maple_enum_dev(p, u);

            if(dev != NULL && dev->info.functions & func && dev->info.functions & extrafunc)
			{
                if(!n) 
					return dev;

                n--;
            }
        }
    }

    return NULL;
}

/* Returns the first VMU that can save and has LCD */
int isVMUPresent()
{
	maple_device_t	*vmu = NULL;

	vmu = maple_enum_mixed_type(0, MAPLE_FUNC_LCD, MAPLE_FUNC_MEMCARD);
	if(!vmu)
		return 0;

	return 1;
}

void internal_vmu_draw_lcd(maple_device_t *dev,	void *bitmap)
{
	int retval = 0;
	
	// takes around 20-30ms
	retval = vmu_draw_lcd(dev, bitmap);
	lcd_vmu_update_time = timer_ms_gettime64();
	if(retval == MAPLE_EOK)
		return;

	switch(retval)
	{
		case MAPLE_EAGAIN:
			dbglog(DBG_ERROR, "VMU LCD could not be updated, got MAPLE_EAGAIN\n");
		case MAPLE_ETIMEOUT:
			dbglog(DBG_ERROR, "VMU LCD could not be updated, got MAPLE_ETIMEOUT\n");
		default:
			disableVMU_LCD_val = 1;
			break;
	}
}

void updateVMU(char *line1, char *line2, int force)
{
	maple_device_t *mvmu = NULL;

	if(disableVMU_LCD_val)
		return;

	if(!force && ovmode == vmode && ovcable == vcable)
		return;

	ovmode = vmode;
	ovcable = vcable;

	mvmu = maple_enum_type(0, MAPLE_FUNC_LCD);
	if (mvmu != 0)
	{
		vmu_clear_bitmap(bitmap);
		vmu_draw_str(bitmap, (unsigned char*)line1, 0, 0);
		vmu_draw_str(bitmap, (unsigned char*)line2, 0, 10);
		switch(vmode)
		{
			case VIDEO_240P:
				vmu_draw_str(bitmap, (unsigned char*)"     240p", 0, 20);
				break;
			case VIDEO_480I_A240:
				vmu_draw_str(bitmap, (unsigned char*)"240p->480i", 0, 20);
				break;
			case VIDEO_480I:
				vmu_draw_str(bitmap, (unsigned char*)"  480i/FS", 0, 20);
				break;
			case VIDEO_288P:
				vmu_draw_str(bitmap, (unsigned char*)"     288p", 0, 20);
				break;
			case VIDEO_576I_A264:
				vmu_draw_str(bitmap, (unsigned char*)"288p->576i", 0, 20);
				break;
			case VIDEO_576I:
				vmu_draw_str(bitmap, (unsigned char*)"  576i/FS", 0, 20);
				break;
			case VIDEO_480P:
				vmu_draw_str(bitmap, (unsigned char*)"  480p/FS", 0, 20);
				break;
			case VIDEO_480P_SL:
				vmu_draw_str(bitmap, (unsigned char*)"  480p/SL", 0, 20);
				break;
		}
		internal_vmu_draw_lcd(mvmu, bitmap);
	}
}

void updateVMUFlash(char *line1, char *line2, int force)
{
	maple_device_t *mvmu = NULL;

	if(disableVMU_LCD_val)
		return;

	updateVMU(line1, line2, force);
	mvmu = maple_enum_type(0, MAPLE_FUNC_LCD);
	if(mvmu != 0)
	{
		vmu_invert_bitmap(bitmap);
		internal_vmu_draw_lcd(mvmu, bitmap);
		vmu_invert_bitmap(bitmap);
		internal_vmu_draw_lcd(mvmu, bitmap);
	}
}

char *sd_xpm[32] = {
"    ........................................    ",
"   ..........................................   ",
"   ........     ...................  ........   ",
"   ........                    ..... ........   ",
"   ........                     .... ........   ",
"   ........                     .... ........   ",
"   ........                    ....  ........   ",
"   .......                      ..    .......   ",
"   .......                            .......   ",
"   ......                             .......   ",
"    .....                              ......   ",
"    .....                              .....    ",
"      ...                              .....    ",
"      ...    ....              ....    ...      ",
"      ...   .. ....          .... ..   ...      ",
"      ...     ........    ........     ...      ",
"     . ...   .. ......    ...... ..    ...      ",
"     . ...   . ... .        . ... .   ... .     ",
"     . ...   . .....        ..... .   ... .     ",
"     . ...   . .....        ..... .   ... .     ",
"      .  .      ...          ...      ... .     ",
"      ....    .       .  .       .    .  .      ",
"          .      ..  .    .  ..      .....      ",
"          .           ....           .          ",
"          .            ..            .          ",
"        ...                          ...        ",
"        .  .                        .  .        ",
"       ... .         ......         .. ..       ",
"       .  ...                      ..  ..       ",
"    .....  ....        ..        .....  ....    ",
"   ..  .  . .....              ...... . .  ..   ",
"   . . . . . .......        ......... . ..  .   "};

char *sd_b1_xpm[32] = {
"    ........................................    ",
"   ..........................................   ",
"   ........     ...................  ........   ",
"   ........                    ..... ........   ",
"   ........                     .... ........   ",
"   ........                     .... ........   ",
"   ........                    ....  ........   ",
"   .......                      ..    .......   ",
"   .......                            .......   ",
"   ......                             .......   ",
"    .....                              ......   ",
"    .....                              .....    ",
"      ...                              .....    ",
"      ...                              ...      ",
"      ...    ....              ....    ...      ",
"      ...   .. ....          .... ..   ...      ",
"     . ...    ........    ........     ...      ",
"     . ...   .. ......    ...... ..   ... .     ",
"     . ...   . ... .        . ... .   ... .     ",
"     . ...   . .....        ..... .   ... .     ",
"      .  .    .                  .    ... .     ",
"      ....       ..   .  .   ..       .  .      ",
"          .          .    .          .....      ",
"          .           ....           .          ",
"          .            ..            .          ",
"        ...                          ...        ",
"        .  .                        .  .        ",
"       ... .         ......         .. ..       ",
"       .  ...                      ..  ..       ",
"    .....  ....        ..        .....  ....    ",
"   ..  .  . .....              ...... . .  ..   ",
"   . . . . . .......        ......... . ..  .   "};

char *sd_b2_xpm[32] = {
"    ........................................    ",
"   ..........................................   ",
"   ........     ...................  ........   ",
"   ........                    ..... ........   ",
"   ........                     .... ........   ",
"   ........                     .... ........   ",
"   ........                    ....  ........   ",
"   .......                      ..    .......   ",
"   .......                            .......   ",
"   ......                             .......   ",
"    .....                              ......   ",
"    .....                              .....    ",
"      ...                              .....    ",
"      ...                              ...      ",
"      ...                              ...      ",
"      ...    ....              ....    ...      ",
"     . ...  .. ....          .... ..   ...      ",
"     . ...     .......    .......     ... .     ",
"     . ...      ......    ......      ... .     ",
"     . ...                            ... .     ",
"      .  .                            ... .     ",
"      ....            .  .            .  .      ",
"          .          .    .          .....      ",
"          .           ....           .          ",
"          .            ..            .          ",
"        ...                          ...        ",
"        .  .                        .  .        ",
"       ... .         ......         .. ..       ",
"       .  ...                      ..  ..       ",
"    .....  ....        ..        .....  ....    ",
"   ..  .  . .....              ...... . .  ..   ",
"   . . . . . .......        ......... . ..  .   "};

char *sd_b1_f_xpm[32] = {
"    ........................................    ",
"   ..........................................   ",
"   ........     ...................  ........   ",
"   ........                    ..... ........   ",
"   ........                     .... ........   ",
"   ........                     .... ........   ",
"   ........                    ....  ........   ",
"   .......                      ..    .......   ",
"   .......                            .......   ",
"   ......                             .......   ",
"    .....                              ......   ",
"    .....                              .....    ",
"      ...                              .....    ",
"      ...    ....              ....    ...      ",
"      ...   .. ....          .... ..   ...      ",
"      ...       ......    ......       ...      ",
"     . ...        ....    ....         ...      ",
"     . ...                            ... .     ",
"     . ...   .......        .......   ... .     ",
"     . ...   . .....        ..... .   ... .     ",
"      .  .      ...          ...      ... .     ",
"      ....    .       .  .       .    .  .      ",
"          .      ..  .    .  ..      .....      ",
"          .           ....           .          ",
"          .            ..            .          ",
"        ...                          ...        ",
"        .  .                        .  .        ",
"       ... .         ......         .. ..       ",
"       .  ...                      ..  ..       ",
"    .....  ....        ..        .....  ....    ",
"   ..  .  . .....              ...... . .  ..   ",
"   . . . . . .......        ......... . ..  .   "};

char *sd_b2_f_xpm[32] = {
"    ........................................    ",
"   ..........................................   ",
"   ........     ...................  ........   ",
"   ........                    ..... ........   ",
"   ........                     .... ........   ",
"   ........                     .... ........   ",
"   ........                    ....  ........   ",
"   .......                      ..    .......   ",
"   .......                            .......   ",
"   ......                             .......   ",
"    .....                              ......   ",
"    .....                              .....    ",
"      ...                              .....    ",
"      ...    ....              ....    ...      ",
"      ...   .. ....          .... ..   ...      ",
"      ...       ......    ......       ...      ",
"     . ...        ....    ....         ...      ",
"     . ...                            ... .     ",
"     . ...                            ... .     ",
"     . ...   ..                  ..   ... .     ",
"      .  .    .......      .......    ... .     ",
"      ....     .....  .  .  .....     .  .      ",
"          .          .    .          .....      ",
"          .           ....           .          ",
"          .            ..            .          ",
"        ...                          ...        ",
"        .  .                        .  .        ",
"       ... .         ......         .. ..       ",
"       .  ...                      ..  ..       ",
"    .....  ....        ..        .....  ....    ",
"   ..  .  . .....              ...... . .  ..   ",
"   . . . . . .......        ......... . ..  .   "};

void updateVMU_SD()
{
	updateVMUGraphic(sd_xpm);
}

#define MAX_FRAMES 12
char **frames_k[MAX_FRAMES] = { sd_b1_xpm  , NULL, NULL, NULL, sd_b2_xpm  , NULL, NULL, sd_b1_xpm  , NULL, NULL, NULL, sd_xpm };
char **frames_f[MAX_FRAMES] = { sd_b1_f_xpm, NULL, NULL, NULL, sd_b2_f_xpm, NULL, NULL, sd_b1_f_xpm, NULL, NULL, NULL, sd_xpm };

// 15 to 20 blinks a minute
// a possible blink every 240 frames
// displaced by MAX_FRAMES of animation
void SD_blink_cycle()
{
	static char ***frames = NULL;
	static int blink_counter = 0;
	static int is_blinking = 0;
	static int blink_frame = 0;

	if(is_sleeping)
		return;
		
	if(disableVMU_LCD_val)
		return;

	blink_counter++;
	if(!is_blinking)
	{
		if(blink_counter < 240)
			return;
		if(rand() % 100 < 98)	// 2% chance every frame after 240
			return;
			
		if(rand() % 2)			// 50% chance of either animation
			frames = frames_k;
		else
			frames = frames_f;
		
		is_blinking = 1;
		blink_counter = 0;	
	}
	
	if(frames[blink_frame])
		updateVMUGraphic(frames[blink_frame]);	// takes between 10-25ms

	blink_frame++;
	if(blink_frame >= MAX_FRAMES)
	{
		blink_frame = 0;
		is_blinking = 0;
	}
}

char *sleep0_xpm[] = {
"........................... ....................",
"................................... .....   ....",
"................ ...... ................     ...",
".......................................       ..",
"............................. .........       ..",
".................. ...... ....... .....       ..",
"........................................     ...",
".................................... ....   ....",
"..  ..........                      ............",
"...  ......                            ...... ..",
".........                               ........",
"........            ...                   ......",
"....   ....      ...  .                    .....",
".... .               .                      ....",
".... ..            . .                       ...",
"....                .             ..         ...",
" . .                 .          ...           . ",
". ..                 .  .      ..             . ",
" ...                 .  .      .              ..",
".. ..                .  .      .               .",
".   .     ...    .. .. ..      .          .    .",
".   ..              .  .        .        ..    .",
".......       ...  .  .  ..........      .     .",
"...   .....    . ..   . .               ..    ..",
"...       ........   ....          ......     ..",
". .......................   ........         .. ",
" ............................                ...",
". . . . . ......... . ......               .... ",
" . . . . . . . . . . . . ...             ..... .",
". . . . . . . . . . . . . ................... . ",
" . . . . . . . . . . . . . ............... . . .",
". . . . . . . . . . . . . . . . . . . . . . . . "};

char *sleep1_xpm[] = {
"........................... ....................",
"................................... .....   ....",
"................ ...... ................     ...",
".......................................       ..",
"............................. .........       ..",
".................. ...... ....... .....       ..",
"....  ..................................     ...",
".....  ........                   .......   ....",
"..  ........                         ...........",
"...  ......                            ...... ..",
".........                               ........",
"........            ...                   ......",
"....   ....      ...  .                    .....",
".... .               .                      ....",
".... ..            . .                       ...",
"....                .             ..         ...",
" . .                 .          ...           . ",
". ..                 .  .      ..             . ",
" ...                 .  .      .              ..",
".. ..     ...    ..  .  .      .               .",
".   .               .. ..      .          .    .",
".   ..        ...   .  .        .        ..    .",
".......        .   .  .  ..........      .     .",
"...   .....      ..   . .               ..    ..",
"...       ........   ....          ......     ..",
". .......................   ........         .. ",
" ............................                ...",
". . . . . ......... . ......               .... ",
" . . . . . . . . . . . . ...             ..... .",
". . . . . . . . . . . . . ................... . ",
" . . . . . . . . . . . . . ............... . . .",
". . . . . . . . . . . . . . . . . . . . . . . . "};

char *sleep2_xpm[] = {
"........................... ....................",
"................................... .....   ....",
"................ ...... ................     ...",
".......................................       ..",
"......  ..................... .........       ..",
".......  ........................ .....       ..",
"....  ..........               .........     ...",
".....  ......                       .....   ....",
"..  .......                           ..........",
"...  .....                             ...... ..",
".........                                .......",
"........            ...                   ......",
"....   ....      ...  .                    .....",
".... .               .                      ....",
".... ..            . .                       ...",
"....                .             ..         ...",
" . .                 .          ...           . ",
". ..                 .  .      ..             . ",
" ...      ...    ..  .  .      .              ..",
".. ..                .  .      .               .",
".   .         ...   .. ..      .          .    .",
".   ..         .    .  .        .        ..    .",
".......            .  .  ..........      .     .",
"...   .....      ..   . .               ..    ..",
"...       ........   ....          ......     ..",
". .......................   ........         .. ",
" ............................                ...",
". . . . . ......... . ......               .... ",
" . . . . . . . . . . . . ...             ..... .",
". . . . . . . . . . . . . ................... . ",
" . . . . . . . . . . . . . ............... . . .",
". . . . . . . . . . . . . . . . . . . . . . . . "};
	
#define MAX_FRAMES_SLEEP 6
char **frames_sleep[MAX_FRAMES_SLEEP] = { sleep0_xpm, NULL, sleep1_xpm, sleep2_xpm, NULL, sleep1_xpm };

// ferigne animated this as a 900ms/450ms/900ms/450ms cycle
// this roughly translates to 52 frames/ 26 frames
// our base will be change frame every 26
void sleep_VMU_cycle()
{
	static int sleep_counter = 0;
	static int sleep_frame = 0;
	static int frame_interval = 27;

	if(disableVMU_LCD_val || !sleepEnabled)
		return;

	sleep_counter++;
	if(!is_sleeping)
	{
		if(sleep_counter < 2700)  /// ~2700 (59.97 fps*50secs)
			return;
		if(rand() % 100 < 70)	// 30% chance every frame 
			return;
		
		is_sleeping = 1;
		sleep_counter = 0;	
		sleep_frame = 0;
		frame_interval = 0;
	}

	frame_interval++;
	if(frame_interval > 26)
	{
		frame_interval = 0;
		
		if(frames_sleep[sleep_frame])
			updateVMUGraphic(frames_sleep[sleep_frame]);	// takes between 10-25ms
		
		sleep_frame++;
		if(sleep_frame >= MAX_FRAMES_SLEEP)
			sleep_frame = 0;
	}
}

void resetSleep()
{
	is_sleeping = 0;
	refreshVMU = 1;
}

void disableSleep()
{
	sleepEnabled = 0;
}

void enableSleep()
{
	sleepEnabled = 1;
}

char *donna_xpm[32] = {
"  ..............................................",
"   .............................................",
"    ............................................",
"   .............................................",
" ......................... ................... .",
"       ...................  ................... ",
".       ..................   .................. ",
"......   ..................   ..................",
".......   .................  ...................",
".  ......  ................ ....................",
"  ........ .....................................",
"  ...   ..... ..............                    ",
"     ........... ........ ..                    ",
"     ... .......  ........ ..       ......      ",
"    ..... .. .............  ..     ...     ..   ",
" .. ....  ..  ......................         .. ",
"  .  ...   .. ..........................       .",
"  .. ...   .  .............        ........     ",
"  ..          ...........         ...  .......  ",
"   .          ..     ...   ..   ...    .........",
"    .         .       ..    .. ...     .. .. ...",
".            ..       ..      ..         .... ..",
"             ..       ..                 .... ..",
"            ...       ..              .  ....  .",
"            ..        ..              .. ....  .",
"            .         ..              .  ...   .",
"           .          ..              ..        ",
"          ..           .               .      . ",
"          .  .         .               ..    .  ",
"         .  .          .                ..  .   ",
"        .  .           ..           .           ",
"      ..                .             ..        "};

void updateVMU_Donna()
{
	updateVMUGraphic(donna_xpm);
}

void updateVMUGraphic(char **xpm)
{
	int x = 0, y = 0;
	maple_device_t *mvmu = NULL;
	
	if(disableVMU_LCD_val)
		return;

	if(!xpm)
		return;

	mvmu = maple_enum_type(0, MAPLE_FUNC_LCD);
	if(!mvmu)
		return;

	vmu_clear_bitmap(bitmap);
	for(y = 0; y < 32; y++)
	{
		for(x = 0; x < 48; x++)
		{
			char c = xpm[y][x];
			
			if(c == '.')
				vmu_set_bit(bitmap, x, y);
		}
	}
		
	internal_vmu_draw_lcd(mvmu, bitmap);
}

void disableVMU_LCD()
{
	disableVMU_LCD_val = 1;
}

void enableVMU_LCD()
{
	disableVMU_LCD_val = 0;
}

/*
 *
 * We have two methods to detect a VMU that reports as having an LCD 
 * but doesn't. That wouldn't be an issue, but when graphics are sent 
 * to reported the "LCD", it resets the controller, slowing everything
 * down and in some cases, mangling controller input.
 *
 * This method checks for the time it takes between controller swaps.
 * The user is free to swap controllers at any time, however if the "swap"
 * takes between 0 and 150 ms, this wasn't a human swapping controllers,
 * it was the misbehaved VMU resetting the controller when receving the
 * vmu_draw_lcd() commands. 
 *
 * It takes arund 50 ms in a regular DC controller and 130 in Arcade Stick.
 *
 * If this happens, all LCD commands are blocked for the current session.
 *
 * The other method is checked at boot time, in hardware.c
 *
 */

void vmu_report_controller_swap(int no_controller)
{
	uint64		now = 0, lapse = 0;
	static int	times = 0;
		
	if(disableVMU_LCD_val)
		return;

	if(lcd_vmu_update_time == 0)
		return;
	
	now = timer_ms_gettime64();
	lapse = now - lcd_vmu_update_time;
	
	dbglog(DBG_INFO, "Time from Controller swap after VMU update %"PRIu64" ms\n", lapse);
	
	if(lapse < 150)	// we got a bad LCD VMU...
	{
		dbglog(DBG_ERROR, "We got a bad LCD VMU, it is resetting the controller. DISABLED.\n");
		disableVMU_LCD_val = 1;
	}
	else if(lapse < 600)  // check if it is slower, but periodic
	{
		times++;
		if(times >= 3)
		{
			dbglog(DBG_ERROR, "We got a bad LCD VMU, it is resetting the controller. DISABLED.\n");
			disableVMU_LCD_val = 1;
		}
	}

	if(no_controller)
		lcd_vmu_update_time = 0;
}

int vmu_found_bad_lcd_vmu()
{
	static int reported = 0;
	
	if(reported)
		return 0;
	
	if(disableVMU_LCD_val)
	{
		reported = 1;
		return 1;
	}
	return 0;
}
