/* 
 * 240p Test Suite
 * Copyright (C)2014 Artemio Urbina (Wii GX)
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
 
#ifdef WII_VERSION
#define VERSION_NUMBER "Wii GX Ver. 1.8"
#else
#define VERSION_NUMBER "GC GX Ver. 1.8"
#endif

#define VERSION_DATE "12/11/2014"

extern u8 DrawMenu;
extern char **HelpData;
extern u8 EndProgram;
extern u8 ChangeVideoEnabled;

void ShowMenu();
void SelectVideoMode(ImagePtr title);
void ChangeOptions(ImagePtr Back);
void ChangePALBackgroundColor(ImagePtr Back);
void ShowVideoData();
void ShowVideoWarning(ImagePtr screen);

void DrawCredits(ImagePtr Back);
void DrawIntro();

#ifdef WII_VERSION	
void GetWiiRegion();
extern char wiiregion[100];
#endif