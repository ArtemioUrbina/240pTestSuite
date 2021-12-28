#include <kos.h>
#include <stdlib.h>
#include "controller.h"

uint16 OldButtonsInternal = 0;
uint8 isFishingRod = 0;
char oldControllerName[256] = { 'I', 'n', 'i', 't', '\0' };

#ifdef DCLOAD
#define  SCREENSHOTMODE
#endif

#define FISHING	"Dreamcast Fishing Controller "

cont_state_t *ReadController(uint16 num, uint16 *pressed)
{
#ifdef DCLOAD
	static int init = 0;
#endif
#ifdef SCREENSHOTMODE
	static int timeout = 0;
#endif

	cont_state_t *st = NULL;
	maple_device_t *dev = NULL;
	
	dev = maple_enum_type(num, MAPLE_FUNC_CONTROLLER);
	if(!dev)
		return NULL;
	
	if(strcmp(oldControllerName, dev->info.product_name) != 0)
	{
#ifdef DCLOAD
		if(!init)
			dbglog(DBG_INFO, "Input Device is \"%s\"\n", 
				dev->info.product_name);
		else
			dbglog(DBG_INFO, "Device changed from \"%s\" to \"%s\"\n", 
				oldControllerName,
				dev->info.product_name);
		dbglog(DBG_INFO, "Functions %08lx: %s\n", 
			dev->info.functions, maple_pcaps(dev->info.functions));
		init = 1;
#endif
		strcpy(oldControllerName, dev->info.product_name);
		if(strcmp(FISHING, dev->info.product_name) == 0)
			isFishingRod = 1;
		else
			isFishingRod = 0;
	}
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

#ifdef DCLOAD
	if(isFishingRod)
	{
		if(abs(st->joy2x) > 3)
		{
			dbglog(DBG_INFO, "X: %d\n", st->joy2x);
			st->joyx = -1*st->joy2x*20;
		}
		
		if(abs(st->joy2y) > 3)
		{
			dbglog(DBG_INFO, "Y: %d\n", st->joy2y);
			st->joyy = -1*st->joy2y*20;
		}
	}
#endif

#ifdef SCREENSHOTMODE
	if(timeout > 0)
		timeout --;
	if(!isFishingRod && !timeout && pressed && st->buttons & CONT_LTRIGGER && st->buttons & CONT_RTRIGGER)
	{
		char name[200];
		static int screen = 0;

		sprintf(name, "/pc/screen%d.ppm", screen++); ;
		if(vid_screen_shot(name) < 0)
			dbglog(DBG_ERROR, "Could not save screenshot to %s\n", name);
		else
		{
			dbglog(DBG_INFO, "Screenshot saved to %s\n", name);
			timeout = 60;
		}
	}
#endif
	return st;
}
