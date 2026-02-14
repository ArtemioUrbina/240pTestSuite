/* 
 * 240p Test Suite for Nintendo 64
 * Copyright (C)2024 Artemio Urbina
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
 
#ifndef MENU_H
#define MENU_H

#include "system.h"
#include "video.h"
#include "font.h"
#include "image.h"
#include "help.h"
#include "controller.h"

#define VERSION_NUMBER "N64 0.92B"
#ifndef VERSION_DATE
#define VERSION_DATE "2026/02/13"
#endif

void checkMenu(char *help, int *reload);
void checkStart(joypad_buttons_t keys);
void showMenu();
void selectVideoMode(int useBack);
void drawCredits(int usebuffer);
void setMenuVideo(int showVideoOption);

void SD_blink_cycle(image *sd);
void SD_release();

#define MENU_CANCEL -1

typedef struct menuData {
    unsigned int	optionValue;
	char			*optionText;
} fmenuData;

int selectMenu(char *title, fmenuData *menu_data, int numOptions, int selectedOption);
int selectMenuEx(char *title, fmenuData *menu_data, int numOptions, int selectedOption, char *helpFile);

void drawMessageBox(char *msg);
int drawAskQuestion(char *msg);

#endif