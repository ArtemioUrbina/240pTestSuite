#include <kos.h>
#include <stdlib.h>
#include "controller.h"
#include "vmu.h"

uint16	OldButtonsInternal = 0;
char	oldControllerName[256] = { '\0' };
uint8 	isFishingRod = 0,
		isMaracas = 0,
		isStockController = 0,
		isArcade = 0,
		isKeyboard = 0,
		isLightGun = 0;

#ifdef DCLOAD
#define  SCREENSHOTMODE
void PrintDebugController(cont_state_t *st);
#endif

#define STANDRD	"Dreamcast Controller         "
#define ARCADE	"Arcade Stick                 "
#define FISHING	"Dreamcast Fishing Controller "
#define MARACAS	"Maracas Controller           "
#define KEYBRD	"Keyboard                     "
#define LIGHTGN	"Dreamcast Gun                "

#define	MARACAS_DEATH_ZONE	20
#define	MARACAS_RATE		120
#define	MARACAS_MODULO		9
#define	MARACAS_MAX_RATE	128

#define FISHING_DEATH_ZONE	60
#define	FISHING_MAX_RATE	128

void InitController()
{
	maple_device_t *dev = NULL;
	
	dev = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
	if(!dev)
		return;
	
	DetectControllerType(dev);
	OldButtonsInternal = 0;
	strcpy(oldControllerName, dev->info.product_name);
}

cont_state_t *ReadController(uint16 num, uint16 *pressed)
{
#ifdef SCREENSHOTMODE
	static int timeout = 0;
#endif
	cont_state_t *st = NULL;
	maple_device_t *dev = NULL;
	
	if(pressed)
		*pressed = 0;

	dev = maple_enum_type(num, MAPLE_FUNC_CONTROLLER);
	if(!dev)
	{
		dev = maple_enum_type(num, MAPLE_FUNC_KEYBOARD);
		if(!dev)
		{
			OldButtonsInternal = 0;
			oldControllerName[0] = '\0';
			
			// check if bad VMU caused a reset issue
			vmu_report_controller_swap(1);
			return NULL;
		}
	}
	
	if(strcmp(oldControllerName, dev->info.product_name) != 0)
	{
#ifdef DCLOAD
		dbglog(DBG_INFO, "Input Device is \"%s\"",
				dev->info.product_name);

		dbglog(DBG_INFO, "\n  Functions 0x%08lx: %s\n  Power[%d-%d]mW",
			dev->info.functions, maple_pcaps(dev->info.functions),
			dev->info.standby_power, dev->info.max_power);
		dbglog(DBG_INFO, "\n  License: \"%s\"\n  Area Code: 0x%0x\n",
				dev->info.product_license, dev->info.area_code);
#endif		
		DetectControllerType(dev);

		OldButtonsInternal = 0;
		strcpy(oldControllerName, dev->info.product_name);
		
		// check if bad VMU caused a reset issue
		vmu_report_controller_swap(0);
	}
	st = (cont_state_t*)maple_dev_status(dev); 
	if(!st)
		return NULL;
		
#ifdef DCLOAD
	// Unmapped
	//PrintDebugController(st);
#endif

	if(st && isStockController)
	{
		/*
			========================================================================
			Official Controller                                 HKT-7700 315-6211-AB
			========================================================================
			Functions     : 0x01000000  Function int 0: 0xfe060f00
			Function int 1: 0x00000000  Function int 2: 0x00000000
			Region:         0xff        Connection:     0x00
			Product Name & License: Dreamcast Controller
			  Produced By or Under License From SEGA ENTERPRISES,LTD.
			Standby power: 0x01ae (430mW) Max: 0x01f4 (500mW)
			Extra data:
			000 | 56 65 72 73 69 6f 6e 20 31 2e 30 31 30 2c 31 39 | Version 1.010,19
			010 | 39 38 2f 30 39 2f 32 38 2c 33 31 35 2d 36 32 31 | 98/09/28,315-621
			020 | 31 2d 41 42 20 20 20 2c 41 6e 61 6c 6f 67 20 4d | 1-AB   ,Analog M
			030 | 6f 64 75 6c 65 20 3a 20 54 68 65 20 34 74 68 20 | odule : The 4th
			040 | 45 64 69 74 69 6f 6e 2e 35 2f 38 20 20 2b 44 46 | Edition.5/8  +DF
			050 | 6f 64 75 6c 6f 64 75 6c 6f 64 75 6c 6f 64 75 6c | odulodulodulodul
			060 | 6f 64 75 6c 6f 64 75 6c 6f 64 75 6c 00 00 00 00 | odulodulodul
			070 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
			080 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
			End of Extra data
		*/
		
		/*
		 * We map triggers to digital buttons Z and D for internal use
		 * doens't affect controller test
		 */
		 
		if (st->ltrig > 5)
			st->buttons |= CONT_LTRIGGER;
		if (st->rtrig > 5)
			st->buttons |= CONT_RTRIGGER;
	}
	
	if(st && isArcade)
	{
		/*
			========================================================================
			Official Arcade Controller                          HKT-7300 315-6125-AC
			========================================================================
			Functions     : 0x01000000  Function int 0: 0xff070000
			Function int 1: 0x00000000  Function int 2: 0x00000000
			Region:         0xff        Connection:     0x00
			Product Name & License: Arcade Stick
			  Produced By or Under License From SEGA ENTERPRISES,LTD.
			Standby power: 0x00aa (170mW) Max: 0x012c (300mW)
			Extra data:
			000 | 56 65 72 73 69 6f 6e 20 31 2e 30 30 30 2c 31 39 | Version 1.000,19
			010 | 39 38 2f 30 36 2f 30 39 2c 33 31 35 2d 36 31 32 | 98/06/09,315-612
			020 | 35 2d 41 43 20 20 20 2c 44 69 72 65 63 74 69 6f | 5-AC   ,Directio
			030 | 6e 20 4b 65 79 20 26 20 41 2c 42 2c 43 2c 58 2c | n Key & A,B,C,X,
			040 | 59 2c 5a 2c 53 74 61 72 74 20 42 75 74 74 6f 6e | Y,Z,Start Button
			050 | 6e 20 4b 65 6e 20 4b 65 6e 20 4b 65 6e 20 4b 65 | n Ken Ken Ken Ke
			060 | 6e 20 4b 65 6e 20 4b 65 6e 20 4b 65 00 00 00 00 | n Ken Ken Ke
			070 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
			080 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
			End of Extra data
		*/
		/*
		 * We map triggers to digital buttons Z and D for internal use
		 * doens't affect controller test
		 */
		if (st->buttons & CONT_Z)
			st->buttons |= CONT_LTRIGGER;
		if (st->buttons & CONT_C)
			st->buttons |= CONT_RTRIGGER;
	}

	if(st && isFishingRod)
	{
		static int fishing_frames_up = 0;
		static int fishing_frames_down = 0;
		static int fishing_frames_left = 0;
		static int fishing_frames_right = 0;
		/*
			========================================================================
			Official ASCII Fishing Controller                   HKT-8700 315-6125-AJ
			========================================================================
			Functions     : 0x01000000  Function int 0: 0x00000000
			Function int 1: 0x00000000  Function int 2: 0x00000000
			Region:         0xff        Connection:     0x00
			Product Name & License: Dreamcast Fishing Controller
			  Produced By or Under License From SEGA ENTERPRISES,LTD.
			Standby power: 0x0258 (600mW) Max: 0x0960 (2400mW)
			Extra data:
			000 | 56 65 72 73 69 6f 6e 20 31 2e 30 30 33 2c 31 39 | Version 1.003,19
			010 | 39 38 2f 31 32 2f 30 31 2c 33 31 35 2d 36 31 32 | 98/12/01,315-612
			020 | 35 2d 41 4a 20 20 20 20 41 64 64 20 52 58 42 20 | 5-AJ    Add RXB
			030 | 54 69 6d 65 6f 75 74 20 43 68 65 63 6b 20 20 20 | Timeout Check
			040 | 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 |
			050 | 54 69 6d 65 54 69 6d 65 54 69 6d 65 54 69 6d 65 | TimeTimeTimeTime
			060 | 54 69 6d 65 54 69 6d 65 54 69 6d 65 00 00 00 00 | TimeTimeTime
			070 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
			080 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
			End of Extra data
		*/

		if(st->joy2y < -1*FISHING_DEATH_ZONE)
		{
			fishing_frames_up++;
			fishing_frames_down = 0;
			if(fishing_frames_up > FISHING_MAX_RATE)
				fishing_frames_up = 1;
			st->buttons |= CONT_DPAD_UP;
		}
		else if(st->joy2y > FISHING_DEATH_ZONE)
		{
			fishing_frames_up = 0;
			fishing_frames_down++;
			if(fishing_frames_down > FISHING_MAX_RATE)
				fishing_frames_down = 1;
			st->buttons |= CONT_DPAD_DOWN;
		}

		if(st->joy2x < -1*FISHING_DEATH_ZONE)
		{
			fishing_frames_left++;
			fishing_frames_right = 0;
			if(fishing_frames_left > FISHING_MAX_RATE)
				fishing_frames_left = 1;
			st->buttons |= CONT_DPAD_LEFT;
		}
		else if(st->joy2x > FISHING_DEATH_ZONE)
		{
			fishing_frames_left = 0;
			fishing_frames_right++;
			if(fishing_frames_right > FISHING_MAX_RATE)
				fishing_frames_right = 1;
			st->buttons |= CONT_DPAD_RIGHT;
		}

	}
	
	// Give them a death zone and filter them slightly
	if(st && isMaracas)
	{
		static int maracas_frames_up = 0;
		static int maracas_frames_down = 0;
		static int maracas_frames_left = 0;
		static int maracas_frames_right = 0;
		static int maracas_frames_lost = 0;
		
		/*
			========================================================================
			Official Maracas Controller                        HKT- 9700 315-6211-AS
			========================================================================
			Functions     : 0x01000000  Function int 0: 0x0f093c00
			Function int 1: 0x00000000  Function int 2: 0x00000000
			Region:         0xff        Connection:     0x00
			Product Name & License: Maracas Controller
			  Produced By or Under License From SEGA ENTERPRISES,LTD.
			Standby power: 0x044c (1100mW) Max: 0x0546 (1350mW)
			Extra data:
			000 | 56 65 72 73 69 6f 6e 20 31 2e 30 31 30 2c 32 30 | Version 1.010,20
			010 | 30 30 2f 30 32 2f 30 33 2c 33 31 35 2d 36 32 31 | 00/02/03,315-621
			020 | 31 2d 41 53 20 20 20 2c 41 74 74 65 6e 74 69 6f | 1-AS   ,Attentio
			030 | 6e 20 3a 20 44 2c 5a 20 62 75 74 74 6f 6e 20 69 | n : D,Z button i
			040 | 73 20 4c 6f 73 74 20 46 6c 61 67 2e 20 6e 79 6f | s Lost Flag. nyo
			050 | 6e 20 3a 20 6e 20 3a 20 6e 20 3a 20 6e 20 3a 20 | n : n : n : n :
			060 | 6e 20 3a 20 6e 20 3a 20 6e 20 3a 20 00 00 00 00 | n : n : n :
			070 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
			080 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
			End of Extra data
		*/
		
		/* 	
		 *	Z button is undetected maraca left as seen above
		 */
		if(st->buttons & CONT_Z)
		{
			maracas_frames_lost++;
			if(maracas_frames_lost > 30)
			{
				st->joyx = 0;
				st->joyy = 0;
			}
		}
		else
			maracas_frames_lost = 0;
		
		if(st->joyy < -1*MARACAS_DEATH_ZONE)
		{
			maracas_frames_up++;
			maracas_frames_down = 0;
			if(maracas_frames_up > MARACAS_MAX_RATE)
				maracas_frames_up = 1;
			st->joyy = maracas_frames_up % MARACAS_MODULO ? -1*MARACAS_RATE : 0;
		}
		else if(st->joyy > MARACAS_DEATH_ZONE)
		{
			maracas_frames_up = 0;
			maracas_frames_down++;
			if(maracas_frames_down > MARACAS_MAX_RATE)
				maracas_frames_down = 1;
			st->joyy =maracas_frames_down % MARACAS_MODULO ? MARACAS_RATE : 0;
		}
		else
			st->joyy = 0;
		
		if(st->joyx < -1*MARACAS_DEATH_ZONE)
		{
			maracas_frames_left++;
			maracas_frames_right = 0;
			if(maracas_frames_left > MARACAS_MAX_RATE)
				maracas_frames_left = 1;
			st->joyx = maracas_frames_left % MARACAS_MODULO ? -1*MARACAS_RATE : 0;
		}
		else if(st->joyx > MARACAS_DEATH_ZONE)
		{
			maracas_frames_left = 0;
			maracas_frames_right++;
			if(maracas_frames_right > MARACAS_MAX_RATE)
				maracas_frames_right = 1;
			st->joyx = maracas_frames_right % MARACAS_MODULO ? MARACAS_RATE : 0;
		}
		else
			st->joyx = 0;
	}
	
	if(st && isKeyboard)
	{
		int region = 0, rcv = 0;	
		/*
			========================================================================
			Official Keyboard US                                HKT-7620 315-6211-AM
			========================================================================
			Functions     : 0x40000000  Function int 0: 0x80000502
			Function int 1: 0x00000000  Function int 2: 0x00000000
			Region:         0x01        Connection:     0x00
			Product Name & License: Keyboard
			  Produced By or Under License From SEGA ENTERPRISES,LTD.
			Standby power: 0x012c (300mW) Max: 0x0190 (400mW)
			Extra data:
			000 | 56 65 72 73 69 6f 6e 20 31 2e 30 31 30 2c 31 39 | Version 1.010,19
			010 | 39 39 2f 30 34 2f 32 37 2c 33 31 35 2d 36 32 31 | 99/04/27,315-621
			020 | 31 2d 41 4d 20 20 20 2c 4b 65 79 20 53 63 61 6e | 1-AM   ,Key Scan
			030 | 20 4d 6f 64 75 6c 65 20 3a 20 54 68 65 20 32 6e |  Module : The 2n
			040 | 64 20 45 64 69 74 69 6f 6e 2e 20 30 34 2f 32 35 | d Edition. 04/25
			050 | 20 4d 6f 64 20 4d 6f 64 20 4d 6f 64 20 4d 6f 64 |  Mod Mod Mod Mod
			060 | 20 4d 6f 64 20 4d 6f 64 20 4d 6f 64 00 00 00 00 |  Mod Mod Mod
			070 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
			080 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
			End of Extra data
		*/
	
		/*
		 * We map all controller inputs to some keys
		 * doens't affect controller test
		 */
		 
		if(dev->info.area_code	== 0x01)
			region = KBD_REGION_US;
		else
			region = KBD_REGION_JP;
		
		rcv = kbd_queue_pop(dev, region);
		if(rcv != -1)
		{
			if (rcv == 0x5200)
				st->buttons |= CONT_DPAD_UP;
			if (rcv == 0x5100)
				st->buttons |= CONT_DPAD_DOWN;
			if (rcv == 0x5000)
				st->buttons |= CONT_DPAD_LEFT;
			if (rcv == 0x4F00)
				st->buttons |= CONT_DPAD_RIGHT;
			
			if (rcv == 'z')
				st->buttons |= CONT_A;
			if (rcv == 'x')
				st->buttons |= CONT_B;
			if (rcv == 'a')
				st->buttons |= CONT_X;
			if (rcv == 's')
				st->buttons |= CONT_Y;
			if (rcv == 'q')
				st->buttons |= CONT_LTRIGGER;
			if (rcv == 'w')
				st->buttons |= CONT_RTRIGGER;
			
			if (rcv == 0x1B)	// ESC
				st->buttons |= CONT_B;
			if (rcv == 0x0A)	// ENTER
				st->buttons |= CONT_START;
			
		}
	}
	
#ifdef DCLOAD
	// Mapped above
	//PrintDebugController(st);
#endif	
	
	if(OldButtonsInternal != st->buttons)
		resetSleep();
		
	if(pressed)
		*pressed = st->buttons & ~OldButtonsInternal;
	OldButtonsInternal = st->buttons;

#ifdef SCREENSHOTMODE
	if(timeout > 0)
		timeout --;
	if(!timeout && isStockController && 
		st->buttons & CONT_LTRIGGER && 
		st->buttons & CONT_RTRIGGER)
	{
		char name[200];
		static int screen = 0;

		sprintf(name, "/pc/screen%.2d.ppm", screen++); ;
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

void DetectControllerType(maple_device_t *dev)
{
	if(!dev)
		return;
		
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
		
	if(strcmp(ARCADE, dev->info.product_name) == 0)
		isArcade = 1;
	else
		isArcade = 0;
		
	if(strcmp(KEYBRD, dev->info.product_name) == 0)
		isKeyboard = 1;
	else
		isKeyboard = 0;
	
	if(strcmp(LIGHTGN, dev->info.product_name) == 0)
		isLightGun = 1;
	else
		isLightGun = 0;
}

void JoystickMenuMove(controller *st, int *sel, int maxsel, int *joycnt)
{
	if( st && st->joyy != 0)
	{
		if(++(*joycnt) > 5)
		{
			if(st && st->joyy > 0 && 
				!(st->buttons & CONT_DPAD_DOWN))	// don't duplicate on those controllers that do both at once
				(*sel) ++;
			if(st && st->joyy < 0 && 
				!(st->buttons & CONT_DPAD_UP))		// don't duplicate on those controllers that do both at once
				(*sel) --;

			if(*sel < 1)
				*sel = maxsel;
			if(*sel > maxsel)
				*sel = 1;
			*joycnt = 0;
			
			resetSleep();
		}
	}
	else
		*joycnt = 0;
}

void JoystickDirections(controller *st, uint16 *pressed, int *joycntx, int *joycnty)
{
	if( st && abs(st->joyx) > 50)
	{
		if(++(*joycntx) > 2)
		{
			if(st && st->joyx > 0)
				(*pressed) |= CONT_DPAD_RIGHT;
			if(st && st->joyx < 0)
				(*pressed) |= CONT_DPAD_LEFT;

			*joycntx = 0;
			resetSleep();
		}
	}
	else
		*joycntx = 0;
		
	if( st && abs(st->joyy) > 50)
	{
		if(++(*joycnty) > 2)
		{
			if(st && st->joyy < 0)
				(*pressed) |= CONT_DPAD_UP;
			if(st && st->joyy > 0)
				(*pressed) |= CONT_DPAD_DOWN;

			*joycnty = 0;
			resetSleep();
		}
	}
	else
		*joycnty = 0;
}

/*
	========================================================================
	Official Rumble Pack                                HKT-8600 315-6211-AH
	========================================================================
	Functions     : 0x00010000  Function int 0: 0x00000101
	Function int 1: 0x00000000  Function int 2: 0x00000000
	Region:         0xff        Connection:     0x00
	Product Name & License: Puru Puru Pack
	  Produced By or Under License From SEGA ENTERPRISES,LTD.
	Standby power: 0x00c8 (200mW) Max: 0x0640 (1600mW)
	Extra data:
	000 | 56 65 72 73 69 6f 6e 20 31 2e 30 30 30 2c 31 39 | Version 1.000,19
	010 | 39 38 2f 31 31 2f 31 30 2c 33 31 35 2d 36 32 31 | 98/11/10,315-621
	020 | 31 2d 41 48 20 20 20 2c 56 69 62 72 61 74 69 6f | 1-AH   ,Vibratio
	030 | 6e 20 4d 6f 74 6f 72 3a 31 20 2c 20 46 6d 3a 34 | n Motor:1 , Fm:4
	040 | 20 2d 20 33 30 48 7a 20 2c 50 6f 77 3a 37 20 20 |  - 30Hz ,Pow:7
	050 | 6e 20 4d 6f 6e 20 4d 6f 6e 20 4d 6f 6e 20 4d 6f | n Mon Mon Mon Mo
	060 | 6e 20 4d 6f 6e 20 4d 6f 6e 20 4d 6f 00 00 00 00 | n Mon Mon Mo
	070 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
	080 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
	End of Extra data

	========================================================================
	Performance Jump Pack with Memory Card (Black)
	========================================================================
	Functions     : 0x02010000  Function int 0: 0x00000101
	Function int 1: 0x00410f00  Function int 2: 0x00000000
	Region:         0xff        Connection:     0x00
	Product Name & License: Memory Card wIth rumble
	  Produced By or Under License From SEGA ENTERPRISES,LTD.
	Standby power: 0x00c8 (200mW) Max: 0x0708 (1800mW)
	Extra data:
	000 | 56 33 31 32 20 76 65 72 20 31 2e 30 20 20 31 2f | V312 ver 1.0  1/
	010 | 4a 75 6e 65 2f 39 39 20 20 20 20 20 20 20 20 20 | June/99
	020 | 20 20 20 20 20 20 20 20 20 20 20 20 00 00 00 00 |
	030 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
	040 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
	050 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
	End of Extra data

	========================================================================
	DC HYPER PAK NYKO (Switch set to Jump Pack)
	========================================================================
	Functions     : 0x00010000  Function int 0: 0x00000101
	Function int 1: 0x00000000  Function int 2: 0x00000000
	Region:         0xff        Connection:     0x00
	Product Name & License: Puru Puru Pack
	  Produced By or Under License From SEGA ENTERPRISES,LTD.
	Standby power: 0x00c8 (200mW) Max: 0x0640 (1600mW)
*/

int rumble_puru(maple_device_t *dev, int type)
{
	if(!(dev->info.functions & MAPLE_FUNC_PURUPURU))
		return 0;
	
	// Functions from the "performance" and official rumble
	if(dev->info.function_data[0] & 0x00000001)	
	{
		purupuru_effect_t	effect;
		
		effect.duration = 20;
		effect.effect2 = PURUPURU_EFFECT2_UINTENSITY(1)|PURUPURU_EFFECT2_LINTENSITY(1);
		effect.effect1 = PURUPURU_EFFECT1_INTENSITY(7);
		if(type)
			effect.special = PURUPURU_SPECIAL_MOTOR1;
		else
			effect.special = PURUPURU_SPECIAL_MOTOR2;
		return(purupuru_rumble(dev, &effect) == MAPLE_EOK);
	}
	
	// Functions from the Fishing Rod
	if(dev->info.function_data[0] & 0x00000002)
	{
		// these values were found at random and
		// tuned for length
		return(purupuru_rumble_raw(dev, 
				type ? 0x030FF21E : 0x0355612D) == MAPLE_EOK);
	}
	
	return 0;
}

int stop_puru(maple_device_t *dev)
{
	if(!(dev->info.functions & MAPLE_FUNC_PURUPURU))
		return 0;
		
	if(dev->info.function_data[0] & 0x00000001)	
	{
		if(purupuru_rumble_raw(dev, 0x00000000) != MAPLE_EOK)
			return 0;
		return 1;
	}
	
	if(dev->info.function_data[0] & 0x00000002)
	{
		// disable pulse directly
		if(purupuru_rumble_raw(dev, 0x030F001E) != MAPLE_EOK)
			return 0;
		if(purupuru_rumble_raw(dev, 0x0355002D) != MAPLE_EOK)
			return 0;
		return 1;
	}
	return 0;
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
