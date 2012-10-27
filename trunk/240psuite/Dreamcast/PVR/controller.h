#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <kos.h>

typedef cont_state_t controller;

uint16 InitController(uint16 num);
cont_state_t *ReadController(uint16 num);

#endif
