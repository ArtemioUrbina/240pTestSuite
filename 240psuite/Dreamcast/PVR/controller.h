#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <kos.h>

#define CONT_LTRIGGER	CONT_Z
#define CONT_RTRIGGER	CONT_D

extern uint8 isFishingRod, isMaracas, isStockController, isArcade, isKeyboard, isLightGun;

typedef cont_state_t controller;

void InitController();
cont_state_t *ReadController(uint16 num, uint16 *pressed);
void DetectControllerType(maple_device_t *dev);
void JoystickMenuMove(controller *st, int *sel, int maxsel, int *joycnt);
void JoystickDirections(controller *st, uint16 *pressed, int *joycntx, int *joycnty);

int rumble_puru(maple_device_t *dev, int type);
int stop_puru(maple_device_t *dev);
#endif
