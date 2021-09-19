#include <kos.h>
#include "controller.h"

uint16 OldButtonsInternal = 0;

#ifdef SERIAL
#define  SCREENSHOTMODE
#endif

cont_state_t *ReadController(uint16 num, uint16 *pressed)
{
#ifdef SCREENSHOTMODE
	static int timeout = 0;
#endif

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
	if(timeout > 0)
		timeout --;
	if(!timeout && pressed && st->buttons & CONT_LTRIGGER && st->buttons & CONT_RTRIGGER)
	{
		char name[200];
		static int screen = 0;

		sprintf(name, "/pc/screen%d.ppm", screen++); ;
		if(vid_screen_shot(name) < 0)
			fprintf(stderr, "Could not save screenshot to %s\n", name);
		else
		{
			printf("Screenshot saved to %s\n", name);
			timeout = 60;
		}
	}
#endif
	return st;
}
