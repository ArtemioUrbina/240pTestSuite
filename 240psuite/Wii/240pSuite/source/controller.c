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
#include "options.h"

#define JOYTHSHLD 10			// min value for joystick to be a read
#define MINFRAMES 4				// Frames to count joystick helps as pad

u8 JoyCountX = 0;
u8 JoyCountY = 0;


s32 ControllerInit()
{
    s32 retval;

    retval = PAD_Init();
#ifdef WII_VERSION
    retval = WPAD_Init();
    WPAD_SetIdleTimeout(60);
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
	int x, y;

    retval = PAD_ButtonsDown(chan);
    
    x  = PAD_StickX(0);
    y  = PAD_StickY(0);
    if (x > JOYTHSHLD)
	{
		JoyCountX++;
		if(JoyCountX > MINFRAMES)			
		{
			retval |= PAD_BUTTON_RIGHT;
			JoyCountX = 0;
		}
	}
	if (x < -JOYTHSHLD) 
	{
		JoyCountX++;
		if(JoyCountX > MINFRAMES)			
		{
			retval |= PAD_BUTTON_LEFT;
			JoyCountX = 0;
		}
	}
	if (y > JOYTHSHLD) 
	{ 
		JoyCountY++;
		if(JoyCountY > MINFRAMES)			
		{
			retval |= PAD_BUTTON_UP;
			JoyCountY = 0;
		}
	}
	if (y < -JOYTHSHLD) 
	{
		JoyCountY++;
		if(JoyCountY > MINFRAMES)			
		{
			retval |= PAD_BUTTON_DOWN;
			JoyCountY = 0;
		}
	}
	 
#ifdef WII_VERSION
    u32 wiistate;

// WiiPad
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

// Wii Classic Controller
    WPADData *data = WPAD_Data(0);
    if (data && data->exp.type == WPAD_EXP_CLASSIC)
    {
        int x, y, overflow;        

        if( wiistate & WPAD_CLASSIC_BUTTON_LEFT )
            retval |= PAD_BUTTON_LEFT;
        if( wiistate & WPAD_CLASSIC_BUTTON_RIGHT )
            retval |= PAD_BUTTON_RIGHT;
        if( wiistate & WPAD_CLASSIC_BUTTON_DOWN )
            retval |= PAD_BUTTON_DOWN;
        if( wiistate & WPAD_CLASSIC_BUTTON_UP )
            retval |= PAD_BUTTON_UP;
    
		if(Options.SFCClassicController)
		{
			if( wiistate & WPAD_CLASSIC_BUTTON_PLUS )
				retval |= PAD_BUTTON_START;
			if( wiistate & WPAD_CLASSIC_BUTTON_FULL_R )
				retval |= PAD_TRIGGER_R;
			if( wiistate & WPAD_CLASSIC_BUTTON_FULL_L )
				retval |= PAD_TRIGGER_L;  
		}		
		else // Normal Classic Controller
		{
			if( wiistate & WPAD_CLASSIC_BUTTON_HOME )
				retval |= PAD_BUTTON_START;
			if( wiistate & WPAD_CLASSIC_BUTTON_PLUS )
				retval |= PAD_TRIGGER_R;
			if( wiistate & WPAD_CLASSIC_BUTTON_MINUS )
				retval |= PAD_TRIGGER_L;  
		}
			        
        if( wiistate & WPAD_CLASSIC_BUTTON_A )
            retval |= PAD_BUTTON_A;
        if( wiistate & WPAD_CLASSIC_BUTTON_B )
            retval |= PAD_BUTTON_B;
        if( wiistate & WPAD_CLASSIC_BUTTON_X )
            retval |= PAD_BUTTON_X;
        if( wiistate & WPAD_CLASSIC_BUTTON_Y )
            retval |= PAD_BUTTON_Y;        
        
		overflow = 0;		
		
        x = data->exp.classic.ljs.pos.x;    		
        if(x > data->exp.classic.ljs.max.x)
		{
			overflow = 1;
			JoyCountX++;
			if(JoyCountX > MINFRAMES)			
			{
				retval |= PAD_BUTTON_RIGHT;
				JoyCountX = 0;
			}
		}
        if(x < data->exp.classic.ljs.min.x)
		{
			overflow = 1;
			JoyCountX++;
			if(JoyCountX > MINFRAMES)			
			{
				retval |= PAD_BUTTON_LEFT;
				JoyCountX = 0;
			}
		}		
			
		if(!overflow)
		{
			x -= data->exp.classic.ljs.center.x;
			if(x > JOYTHSHLD)
			{
				JoyCountX++;
				if(JoyCountX > MINFRAMES)			
				{
					retval |= PAD_BUTTON_RIGHT;
					JoyCountX = 0;
				}
			}
			if(x < -1*JOYTHSHLD)
			{
				JoyCountX++;
				if(JoyCountX > MINFRAMES)			
				{
					retval |= PAD_BUTTON_LEFT;
					JoyCountX = 0;
				}
			}
		}
        
		overflow = 0;
		
        y = data->exp.classic.ljs.pos.y;         
		
        if(y > data->exp.classic.ljs.max.y)
		{
			overflow = 1;
			JoyCountY++;
			if(JoyCountY > MINFRAMES)
			{
				retval |= PAD_BUTTON_UP;
				JoyCountY = 0;
			}
		}
        if(y < data->exp.classic.ljs.min.y)
		{
			overflow = 1;
			JoyCountY++;
			if(JoyCountY > MINFRAMES)
			{
				retval |= PAD_BUTTON_DOWN;
				JoyCountY = 0;
			}
		}		
			
		if(!overflow)
		{
			y -= data->exp.classic.ljs.center.y;
			if(y > JOYTHSHLD)
			{
				JoyCountY++;
				if(JoyCountY > MINFRAMES)
				{
					retval |= PAD_BUTTON_UP;
					JoyCountY = 0;
				}
			}
			if(y < -1*JOYTHSHLD)
			{			
				JoyCountY++;
				if(JoyCountY > MINFRAMES)
				{	
					retval |= PAD_BUTTON_DOWN;
					JoyCountY = 0;
				}
			}
		}
    }

    if (data && data->exp.type == WPAD_EXP_NUNCHUK)
    {
        int x, y, overflow;               
		
		if( wiistate & WPAD_NUNCHUK_BUTTON_C )
            retval |= PAD_BUTTON_A;        
    
        if( wiistate & WPAD_NUNCHUK_BUTTON_Z )
            retval |= PAD_BUTTON_B;                
        
		overflow = 0;		
		
        x = data->exp.nunchuk.js.pos.x;    		
        if(x > data->exp.nunchuk.js.max.x)
		{
			overflow = 1;
			JoyCountX++;
			if(JoyCountX > MINFRAMES)			
			{
				retval |= PAD_BUTTON_RIGHT;
				JoyCountX = 0;
			}
		}
        if(x < data->exp.nunchuk.js.min.x)
		{
			overflow = 1;
			JoyCountX++;
			if(JoyCountX > MINFRAMES)			
			{
				retval |= PAD_BUTTON_LEFT;
				JoyCountX = 0;
			}
		}		
			
		if(!overflow)
		{
			x -= data->exp.nunchuk.js.center.x;
			if(x > JOYTHSHLD)
			{
				JoyCountX++;
				if(JoyCountX > MINFRAMES)			
				{
					retval |= PAD_BUTTON_RIGHT;
					JoyCountX = 0;
				}
			}
			if(x < -1*JOYTHSHLD)
			{
				JoyCountX++;
				if(JoyCountX > MINFRAMES)			
				{
					retval |= PAD_BUTTON_LEFT;
					JoyCountX = 0;
				}
			}
		}
        
		overflow = 0;
		
        y = data->exp.nunchuk.js.pos.y;         
		
        if(y > data->exp.nunchuk.js.max.y)
		{
			overflow = 1;
			JoyCountY++;
			if(JoyCountY > MINFRAMES)
			{
				retval |= PAD_BUTTON_UP;
				JoyCountY = 0;
			}
		}
        if(y < data->exp.nunchuk.js.min.y)
		{
			overflow = 1;
			JoyCountY++;
			if(JoyCountY > MINFRAMES)
			{
				retval |= PAD_BUTTON_DOWN;
				JoyCountY = 0;
			}
		}		
			
		if(!overflow)
		{
			y -= data->exp.nunchuk.js.center.y;
			if(y > JOYTHSHLD)
			{
				JoyCountY++;
				if(JoyCountY > MINFRAMES)
				{
					retval |= PAD_BUTTON_UP;
					JoyCountY = 0;
				}
			}
			if(y < -1*JOYTHSHLD)
			{			
				JoyCountY++;
				if(JoyCountY > MINFRAMES)
				{	
					retval |= PAD_BUTTON_DOWN;
					JoyCountY = 0;
				}
			}
		}
    }
#endif    
    return retval;
}


u32 Controller_ButtonsHeld(int chan)
{
    u32 retval;
	int x, y;

    retval = PAD_ButtonsHeld(chan);
	x  = PAD_StickX(0);
    y  = PAD_StickY(0);
    if (x > JOYTHSHLD)
       retval |= PAD_BUTTON_RIGHT;
	if (x < -JOYTHSHLD) 
		retval |= PAD_BUTTON_LEFT;
	if (y > JOYTHSHLD)  
		retval |= PAD_BUTTON_UP;
	if (y < -JOYTHSHLD) 
		retval |= PAD_BUTTON_DOWN;
		
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

    WPADData *data = WPAD_Data(0);
    if (data && data->exp.type == WPAD_EXP_CLASSIC)
    {
        int x, y, overflow;        

        if( wiistate & WPAD_CLASSIC_BUTTON_LEFT )
            retval |= PAD_BUTTON_LEFT;
        if( wiistate & WPAD_CLASSIC_BUTTON_RIGHT )
            retval |= PAD_BUTTON_RIGHT;
        if( wiistate & WPAD_CLASSIC_BUTTON_DOWN )
            retval |= PAD_BUTTON_DOWN;
        if( wiistate & WPAD_CLASSIC_BUTTON_UP )
            retval |= PAD_BUTTON_UP;
    
        if( wiistate & WPAD_CLASSIC_BUTTON_HOME )
            retval |= PAD_BUTTON_START;
        if( wiistate & WPAD_CLASSIC_BUTTON_A )
            retval |= PAD_BUTTON_A;
        if( wiistate & WPAD_CLASSIC_BUTTON_B )
            retval |= PAD_BUTTON_B;
        if( wiistate & WPAD_CLASSIC_BUTTON_X )
            retval |= PAD_BUTTON_X;
        if( wiistate & WPAD_CLASSIC_BUTTON_Y )
            retval |= PAD_BUTTON_Y;
        if( wiistate & WPAD_CLASSIC_BUTTON_PLUS )
            retval |= PAD_TRIGGER_R;
        if( wiistate & WPAD_CLASSIC_BUTTON_MINUS )
            retval |= PAD_TRIGGER_L;  
       
		overflow = 0;
		
        x = data->exp.classic.ljs.pos.x;    		
        if(x > data->exp.classic.ljs.max.x)
		{
			overflow = 1;
            retval |= PAD_BUTTON_RIGHT;
		}
        if(x < data->exp.classic.ljs.min.x)
		{
			overflow = 1;
            retval |= PAD_BUTTON_LEFT;
		}		
			
		if(!overflow)
		{
			x -= data->exp.classic.ljs.center.x;
			if(x > JOYTHSHLD)
				retval |= PAD_BUTTON_RIGHT;
			if(x < -1*JOYTHSHLD)
				retval |= PAD_BUTTON_LEFT;
		}
		
		overflow = 0;
		
        y = data->exp.classic.ljs.pos.y;         
		
        if(y > data->exp.classic.ljs.max.y)
		{
			overflow = 1;
            retval |= PAD_BUTTON_UP;
		}
        if(y < data->exp.classic.ljs.min.y)
		{
			overflow = 1;
            retval |= PAD_BUTTON_DOWN;
		}		
			
		if(!overflow)
		{
			y -= data->exp.classic.ljs.center.y;
			if(y > JOYTHSHLD)
				retval |= PAD_BUTTON_UP;
			if(y < -1*JOYTHSHLD)
				retval |= PAD_BUTTON_DOWN;
		}
    }

	if (data && data->exp.type == WPAD_EXP_NUNCHUK)
    {
        int x, y, overflow;        
        		
        overflow = 0;
		
		x = data->exp.nunchuk.js.pos.x;    		
        if(x > data->exp.nunchuk.js.max.x)
		{
			overflow = 1;
            retval |= PAD_BUTTON_RIGHT;
		}
        if(x < data->exp.nunchuk.js.min.x)
		{
			overflow = 1;
            retval |= PAD_BUTTON_LEFT;
		}		
			
		if(!overflow)
		{
			x -= data->exp.nunchuk.js.center.x;
			if(x > JOYTHSHLD)
				retval |= PAD_BUTTON_RIGHT;
			if(x < -1*JOYTHSHLD)
				retval |= PAD_BUTTON_LEFT;
		}
        
		overflow = 0;
		
        y = data->exp.nunchuk.js.pos.y;         
		
        if(y > data->exp.nunchuk.js.max.y)
		{
			overflow = 1;
            retval |= PAD_BUTTON_UP;
		}
        if(y < data->exp.nunchuk.js.min.y)
		{
			overflow = 1;
            retval |= PAD_BUTTON_DOWN;
		}		
			
		if(!overflow)
		{
			y -= data->exp.nunchuk.js.center.y;
			if(y > JOYTHSHLD)
				retval |= PAD_BUTTON_UP;
			if(y < -1*JOYTHSHLD)
				retval |= PAD_BUTTON_DOWN;
		}
    }

#endif    
    return retval;
}

u8 ControllerBattery(int chan)
{
	u8 level = 0;
	
#ifdef WII_VERSION   
	level = WPAD_BatteryLevel(chan);	
#endif
	return(level);
}
	
void ControllerRumble(int chan, int rumble)
{	
	PAD_ControlMotor(chan, rumble);
#ifdef WII_VERSION   
    WPAD_Rumble(chan, rumble);
#endif
}