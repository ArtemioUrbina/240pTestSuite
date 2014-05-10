/* 
 * 240p Test Suite
 * Copyright (C)2014 Artemio Urbina (Wii GX)
 *
 * This file is part of the 240p Test Suite
 *
 * The 240p Test Suite is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The 240p Test Suite is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 
#include "controller.h"
#ifdef WII_VERSION
#include <wiiuse/wpad.h>
#endif

s32 ControllerInit()
{
    s32 retval;

    retval = PAD_Init();
#ifdef WII_VERSION
    retval = WPAD_Init();
#endif    
    return retval;
}

s32 ControllerScan()
{
    s32 retval = 0;

    retval = PAD_ScanPads();
#ifdef WII_VERSION
    retval |= WPAD_ScanPads();
#endif        
    return retval;
}

u32 Controller_ButtonsDown(int chan)
{
    u32 retval = 0;

    retval = PAD_ButtonsDown(chan);
#ifdef WII_VERSION
    u32 wiistate;

	wiistate = WPAD_ButtonsDown(chan);

    if( wiistate & WPAD_BUTTON_LEFT )
        retval |= PAD_BUTTON_LEFT;
    if( wiistate & WPAD_BUTTON_RIGHT )
        retval |= PAD_BUTTON_RIGHT;
    if( wiistate & WPAD_BUTTON_DOWN )
        retval |= PAD_BUTTON_DOWN;
    if( wiistate & WPAD_BUTTON_UP )
        retval |= PAD_BUTTON_UP;

    if( wiistate & WPAD_BUTTON_HOME )
        retval |= PAD_BUTTON_START;
    if( wiistate & WPAD_BUTTON_A )
        retval |= PAD_BUTTON_A;
    if( wiistate & WPAD_BUTTON_B )
        retval |= PAD_BUTTON_B;
    if( wiistate & WPAD_BUTTON_1 )
        retval |= PAD_BUTTON_X;
    if( wiistate & WPAD_BUTTON_2 )
        retval |= PAD_BUTTON_Y;
    if( wiistate & WPAD_BUTTON_PLUS )
        retval |= PAD_TRIGGER_R;
    if( wiistate & WPAD_BUTTON_MINUS )
        retval |= PAD_TRIGGER_L;  
#endif    
    return retval;
}


u32 Controller_ButtonsHeld(int chan)
{
    u32 retval;

    retval = PAD_ButtonsHeld(chan);
#ifdef WII_VERSION    
	u32 wiistate;

	wiistate = WPAD_ButtonsHeld(chan);
	if( wiistate & WPAD_BUTTON_LEFT )
		retval |= PAD_BUTTON_LEFT;
	if( wiistate & WPAD_BUTTON_RIGHT )
		retval |= PAD_BUTTON_RIGHT;
	if( wiistate & WPAD_BUTTON_DOWN )
		retval |= PAD_BUTTON_DOWN;
	if( wiistate & WPAD_BUTTON_UP )
		retval |= PAD_BUTTON_UP;

	if( wiistate & WPAD_BUTTON_HOME )
		retval |= PAD_BUTTON_START;
	if( wiistate & WPAD_BUTTON_A )
		retval |= PAD_BUTTON_A;
	if( wiistate & WPAD_BUTTON_B )
		retval |= PAD_BUTTON_B;
	if( wiistate & WPAD_BUTTON_1 )
		retval |= PAD_BUTTON_X;
	if( wiistate & WPAD_BUTTON_2 )
		retval |= PAD_BUTTON_Y;
	if( wiistate & WPAD_BUTTON_PLUS )
		retval |= PAD_TRIGGER_R;
	if( wiistate & WPAD_BUTTON_MINUS )
		retval |= PAD_TRIGGER_L;  

#endif    
    return retval;
}
	