#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <kos.h>

#define CONT_LTRIGGER	CONT_Z
#define CONT_RTRIGGER	CONT_D

typedef cont_state_t controller;

cont_state_t *ReadController(uint16 num, uint16 *pressed);

#endif
