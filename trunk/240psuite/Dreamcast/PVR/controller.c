#include <kos.h>
#include "controller.h"

uint16 InitController(uint16 num)
{
	uint16 buttons = 0xffff;
	cont_state_t *st;

	st = ReadController(num);
	if(st)
		buttons = st->buttons;
	return buttons;
}

cont_state_t *ReadController(uint16 num)
{
	cont_state_t *st;
	maple_device_t *dev;
	
	dev = maple_enum_type(num, MAPLE_FUNC_CONTROLLER);
	if(!dev)
		return NULL;
	st = (cont_state_t*)maple_dev_status(dev); 
	if(!st)
		return NULL;
	return st;
}
