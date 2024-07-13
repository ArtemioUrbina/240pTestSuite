/* 
 * 240p Test Suite for Nintendo 64
 * Copyright (C)2024 Artemio Urbina
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	02111-1307	USA
 */
 
 #include "image.h"
 #include "video.h"
 
 void rdpqStart()
 {
 	rdpq_attach(__dc, NULL);
	rdpq_set_mode_copy(true);
 }
 
 void rdpqEnd()
 {
	rdpq_detach_wait();
 }
 
 void rdpqDrawImage(sprite_t* tiles, float x, float y)
 {			
	rdpq_sprite_blit(tiles, x, y, NULL);
 }
 
 void rdpqClearScreen()
 {
	rdpq_set_mode_fill(RGBA32(0, 0, 0, 0xff));
	rdpq_fill_rectangle(0, 0, dW, dH);
	rdpq_set_mode_copy(true);
 }
 
 void freeImage(sprite_t **image)
{
	if(*image)
	{
		free(*image);
		*image = NULL;
	}
}
 
 