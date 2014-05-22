#include <kos.h>
#include "controller.h"

uint16 OldButtonsInternal = 0;

//#define  SCREENSHOTMODE

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
	if (st->ltrig > 5)
		st->buttons |= CONT_LTRIGGER;
	if (st->rtrig > 5)
		st->buttons |= CONT_RTRIGGER;
	if(pressed)
		*pressed = st->buttons & ~OldButtonsInternal & st->buttons;
	OldButtonsInternal = st->buttons;

#ifdef SCREENSHOTMODE
	if(pressed && st->buttons & CONT_LTRIGGER && st->buttons & CONT_RTRIGGER)
	{
		char name[200];
		static int screen = 0;

		sprintf(name, "/pc/home/aurbina/DC/screens/screen%d.ppm", screen++); 
		vid_screen_shot(name);
	}
#endif
	return st;
}
