/* 
 * 240p Test Suite
 * Copyright (C)2016 Artemio Urbina (N64)
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

#include "utils.h"
#include "help.h"

void showMenu();
void SelectVideoMode(int usebuffer);
void DrawCredits(int usebuffer);
void ShowOptions(int usebuffer);

void CheckMenu(char *help);
void CheckStart(struct controller_data keys);

extern int ChangeVideoEnabled; 
extern int ShowMenu; 
extern int ChangedVideoFormat;
extern int ChangedResolution;