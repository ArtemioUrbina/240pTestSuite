#ifndef MENU_H
#define MENU_H

#include "system.h"
#include "video.h"
#include "font.h"
#include "image.h"
#include "help.h"
#include "controller.h"

void checkMenu(char *help, int *reload);
void checkStart(joypad_buttons_t keys);
void showMenu();
void selectVideoMode();

#define MENU_CANCEL -1

typedef struct menuData {
    unsigned int	optionValue;
	char			*optionText;
} fmenuData;

int selectMenu(char *title, fmenuData *menu_data, int numOptions, int selectedOption);
int selectMenuEx(char *title, fmenuData *menu_data, int numOptions, int selectedOption, char *helpFile);

void drawMessageBox(char *msg);

#endif