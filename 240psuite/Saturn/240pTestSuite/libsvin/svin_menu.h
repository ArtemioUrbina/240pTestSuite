#ifndef _SVIN_MENU_H_
#define _SVIN_MENU_H_

#include <yaul.h>

#include <assert.h>
#include <stdlib.h>

#include "svin.h"

#include <mcufont.h>

typedef struct {
    char *line;
    uint32_t jump;
    uint8_t id;
} _svin_menu_item_type;

void _svin_menu_init(); //prepare menu

void _svin_menu_populate(int jump, const char * text);

int _svin_menu_activate();

void _svin_menu_clear();

void _svin_menu_init_palette();

#endif
