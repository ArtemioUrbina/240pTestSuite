#include <kos.h>
#include "controller.h"

uint16 OldButtonsInternal = 0;

cont_state_t *ReadController(uint16 num, uint16 *pressed)
{
	cont_state_t *st;
	maple_device_t *dev;
	
	dev = maple_enum_type(num, MAPLE_FUNC_CONTROLLER);
	if(!dev)
		return NULL;
	st = (cont_state_t*)maple_dev_status(dev); 
	if(!st)
		return NULL;
	if(pressed)
		*pressed = st->buttons & ~OldButtonsInternal & st->buttons;
	OldButtonsInternal = st->buttons;
	return st;
}
