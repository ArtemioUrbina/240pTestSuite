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

#endif