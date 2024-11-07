#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <yaul.h>
#include "font.h"
#include "video_vdp2.h"

void get_digital_keypress_anywhere(smpc_peripheral_digital_t *digital)
{
    //smpc_peripheral_digital_port(1, digital);
    //return;
	//cycling through all peripherals until we meet one with digital key pressed
	smpc_peripheral_port_t * _port;
	smpc_peripheral_t *peripheral;
	smpc_peripheral_t *tmp_peripheral;
	digital->pressed.raw = 0;
    int y=30;
    char buf[32];
	for (int port = 1; port <3; port++)
	{
		_port = smpc_peripheral_raw_port(port);
        //first checking single value
        peripheral = _port->peripheral;
		//checking if the peripheral is actually a digital device
		//if (TYPE_DIGITAL == peripheral->type)
        //sprintf(buf,"%d type = %d (%d)",port,peripheral->type,peripheral->connected);
        //DrawString(buf, 10, y, FONT_WHITE);
        y+=10;
		if (ID_DIGITAL == peripheral->type)
		{
			smpc_peripheral_digital_get(peripheral,digital);
			//if the key is pressed, searching no more
			if (digital->pressed.raw)
				return;
		}
		//now the list	
		for (peripheral = TAILQ_FIRST(&_port->peripherals);
			(peripheral != NULL) && (tmp_peripheral = TAILQ_NEXT(peripheral, peripherals), 1);
           	peripheral = tmp_peripheral) {
				//checking if the peripheral is actually a digital device
                //sprintf(buf,"%dL type = %d (%d)",port,peripheral->type,peripheral->connected);
                //DrawString(buf, 10, y, FONT_WHITE);
                y+=10;
				if (ID_DIGITAL == peripheral->type)
				{
					smpc_peripheral_digital_get(peripheral,digital);
					//if the key is pressed, searching no more
					if (digital->pressed.raw)
						return;
				}
			}
	}
}