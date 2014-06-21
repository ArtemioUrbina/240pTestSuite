/* 
 * 240p Test Suite
 * Copyright (C)2011 Artemio Urbina
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

#ifndef MENU_H
#define MENU_H

#define VERSION_NUMBER "Dreamcast Ver. 1.20"
#define VERSION_DATE "20/06/2014"

#define DEFAULT_OPTIONS { 0, 0, 0, 0 };

struct settings_st {
	int drawborder;
	int drawpvrbg;
	int EnablePAL;
	int PALStart;
};

extern struct settings_st settings;

extern uint8 EndProgram;
void ChangeOptions(ImagePtr screen);
void SelectVideoMode(ImagePtr screen);
void DrawShowMenu();
void ShowMenu(char *HelpData);
uint8 ReloadFBTexture();
void DrawCredits(ImagePtr back);
void DrawIntro();
void ShowHelpWindow(char *Data);

#endif
