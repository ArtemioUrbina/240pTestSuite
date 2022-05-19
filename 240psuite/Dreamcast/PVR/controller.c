#include <kos.h>
#include <stdlib.h>
#include "controller.h"

uint16 OldButtonsInternal = 0;
char oldControllerName[256] = { '\0' };
uint8 isFishingRod = 0, isMaracas = 0, isStockController = 0;

#ifdef DCLOAD
#define  SCREENSHOTMODE
void PrintDebugController(cont_state_t *st);
#endif

#define FISHING	"Dreamcast Fishing Controller "
#define MARACAS	"Maracas Controller           "
#define STANDRD	"Dreamcast Controller         "

cont_state_t *ReadController(uint16 num, uint16 *pressed)
{
#ifdef DCLOAD
	static int detected = 0;
#endif
#ifdef SCREENSHOTMODE
	static int timeout = 0;
#endif

	cont_state_t *st = NULL;
	maple_device_t *dev = NULL;
	
	dev = maple_enum_type(num, MAPLE_FUNC_CONTROLLER);
	if(!dev)
	{
#ifdef DCLOAD
		detected = 0;
#endif
		OldButtonsInternal = 0;
		oldControllerName[0] = '\0';
		return NULL;
	}
	
	if(strcmp(oldControllerName, dev->info.product_name) != 0)
	{
#ifdef DCLOAD
		if(!detected)
			dbglog(DBG_INFO, "Input Device is \"%s\"",
				dev->info.product_name);
		else
			dbglog(DBG_INFO, "Device changed from \"%s\" to \"%s\"", 
				oldControllerName,
				dev->info.product_name);
		dbglog(DBG_INFO, "\n  Functions 0x%08lx: %s\n  Power[%d-%d]mW",
			dev->info.functions, maple_pcaps(dev->info.functions),
			dev->info.standby_power, dev->info.max_power);
		dbglog(DBG_INFO, "\n  License: \"%s\"\n  Area Code: 0x%0x\n",
				dev->info.product_license, dev->info.area_code);
		detected = 1;
#endif		
		if(strcmp(STANDRD, dev->info.product_name) == 0)
			isStockController = 1;
		else
			isStockController = 0;
		
		if(strcmp(FISHING, dev->info.product_name) == 0)
			isFishingRod = 1;
		else
			isFishingRod = 0;
			
		if(strcmp(MARACAS, dev->info.product_name) == 0)
			isMaracas = 1;
		else
			isMaracas = 0;

		OldButtonsInternal = 0;
		strcpy(oldControllerName, dev->info.product_name);
	}
	st = (cont_state_t*)maple_dev_status(dev); 
	if(!st)
		return NULL;
	if(st && isStockController) // we map them to digital buttons Z and D
	{
		if (st->ltrig > 5)
			st->buttons |= CONT_LTRIGGER;
		if (st->rtrig > 5)
			st->buttons |= CONT_RTRIGGER;
	}
	if(pressed)
		*pressed = st->buttons & ~OldButtonsInternal & st->buttons;
	OldButtonsInternal = st->buttons;
#ifdef DCLOAD
	//PrintDebugController(st);
	
	if(isFishingRod)
	{
		if(st->ltrig && abs(st->joy2x) > 3)
			st->joyx = -1*st->joy2x*20;
		
		if(st->ltrig && abs(st->joy2y) > 3)
			st->joyy = -1*st->joy2y*20;
	}
#endif

#ifdef SCREENSHOTMODE
	if(timeout > 0)
		timeout --;
	if(!timeout && st->buttons & CONT_LTRIGGER && st->buttons & CONT_RTRIGGER)
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

#ifdef DCLOAD
void PrintDebugController(cont_state_t *st)
{
	if(!st)
		return;

	dbglog(DBG_INFO, "X1: %4d Y1: %4d X2: %4d Y2: %4d LT: %d RT: %d ",
		st->joyx, st->joyy, st->joy2x, st->joy2y, 
		st->ltrig, st->rtrig);
	dbglog(DBG_INFO, st->buttons & CONT_C ? "C" : " ");
	dbglog(DBG_INFO, st->buttons & CONT_B ? "B" : " ");
	dbglog(DBG_INFO, st->buttons & CONT_A ? "A" : " ");
	dbglog(DBG_INFO, st->buttons & CONT_START ? "S" : " ");
	dbglog(DBG_INFO, st->buttons & CONT_DPAD_UP ? "U" : " ");
	dbglog(DBG_INFO, st->buttons & CONT_DPAD_DOWN ? "D" : " ");
	dbglog(DBG_INFO, st->buttons & CONT_DPAD_LEFT ? "L" : " ");
	dbglog(DBG_INFO, st->buttons & CONT_DPAD_RIGHT ? "R" : " ");
	dbglog(DBG_INFO, st->buttons & CONT_Z ? "Z" : " ");
	dbglog(DBG_INFO, st->buttons & CONT_Y ? "Y" : " ");
	dbglog(DBG_INFO, st->buttons & CONT_X ? "X" : " ");
	dbglog(DBG_INFO, st->buttons & CONT_D ? "D" : " ");
	dbglog(DBG_INFO, st->buttons & CONT_DPAD2_UP ? "u" : " ");
	dbglog(DBG_INFO, st->buttons & CONT_DPAD2_DOWN ? "d" : " ");
	dbglog(DBG_INFO, st->buttons & CONT_DPAD2_LEFT ? "l" : " ");
	dbglog(DBG_INFO, st->buttons & CONT_DPAD2_RIGHT ? "r" : " ");
	dbglog(DBG_INFO, "\n");
}
#endif
