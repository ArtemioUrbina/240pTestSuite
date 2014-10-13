/* 
 * 240p Test Suite
 * Copyright (C)2014 Artemio Urbina (PC Engine/TurboGrafx-16)
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
 
/*
            DEFINES
*/

#define FONT            "graphics/font.pcx"
#define MAIN_BACKGROUND "graphics/mainbg.pcx"

/*
            FONT
*/

#incchr(my_font, FONT, 32, 3);

/*
            GRAPHICS
*/

#incchr(MB_bg,MAIN_BACKGROUND,32,28);
#incpal(MB_pal,MAIN_BACKGROUND);
#incbat(MB_bat,MAIN_BACKGROUND,0x1000,32,28);
