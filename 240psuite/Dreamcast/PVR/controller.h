#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <kos.h>

#define CONT_LTRIGGER	CONT_Z
#define CONT_RTRIGGER	CONT_D

typedef cont_state_t controller;

cont_state_t *ReadController(uint16 num, uint16 *pressed);
void JoystickMenuMove(controller *st, int *sel, int maxsel, int *joycnt);
void JoystickDirectios(controller *st, uint16 *pressed, int *joycntx, int *joycnty);

#endif
