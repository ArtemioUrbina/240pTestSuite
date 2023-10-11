/* 
 * 240p Test Suite
 * Copyright (C)2011-2014 Artemio Urbina
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


#include "genesis.h" 
#include "res.h"
#include "patterns.h"
#include "tests.h"
#include "help.h"
#include "myvdp.h"
#include "myvdp.h"

extern u8		joytype;
extern u8		VDPChanged;
extern u8		intCancel;
extern u8		joytype2;

extern u8 		doZ80Lock;

extern u8		readDriveVer;
extern u16		DriveVersion;

#define FADE_TIME 10

#define SCDTEST

void ColorPatternMenu();
void GeometryPatternMenu();
void TestPatternMenu();
void DrawCredits();
void DrawIntro();
void Detect_MD(char *str);
void FadeAndCleanUp();
void CleanUp();
void VBlankIntCallback();
u8 CheckHelpAndVO(u16 *buttons, u16 *pressedButtons, int option);
void StopPSG();
void DrawHelpText();
void VBlankIntCallbackCancel();
u16 DrawMainBG();
u16 DrawMainBGwithGillian(u8 DrawGillian, u8 GillianX, u8 GillianY);
void DrawResolution();
void OptionsMenu();
void VideoTestsMenu();
void AudioTestsMenu();
void HardwareMenu();
void ControllerTest();
void TeamPlayerCheck();
void checkblink();

extern int gblink_count, is_gblinking;
extern u16 g_pos;

#ifdef SEGACD
void WarningFileNotFount();
void WarningWrongCDDA();
#endif

#ifndef MENUDATA
#define MENUDATA
typedef struct menu_data {
    u16		id;
	char	*name;
} fmenudata;
#endif

#define FLOAT_CANCEL	0
#define RES_320 		1
#define RES_256 		2
#define FLOAT_OPTION	3

u16 DrawFloatMenuRes(u16 def);
u16 DrawFloatMenuResExtra(u16 def, char *option);
u16 DrawFloatMenu(u16 def, fmenudata *data, u16 size);
void resetController(u16 *oldButtons);
u16 changeResMenu(u16 pressed, u16 button, u16 *oldButtons, u16 *type, u16 *loadvram);
u16 changeResMenuExtra(u16 pressed, u16 button, u16 *oldButtons, u16 *type, u16 *loadvram, char *option);
