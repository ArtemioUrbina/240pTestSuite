/* 
 * 240p Test Suite
 * Copyright (C)2014 Artemio Urbina (SNES)
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
 
 // taken from http://helmet.kafuka.org/msu1.htm
 // Code by qwertymodo

#include "msu.h"

int msu1available()
{
    if( MSU1IDENT0 == 'S' &&
        MSU1IDENT1 == '-' &&
        MSU1IDENT2 == 'M' &&
        MSU1IDENT3 == 'S' &&
        MSU1IDENT4 == 'U' &&
        MSU1IDENT5 == '1' )
        return 1;
    return 0;
}

void msu1play(unsigned int track)
{
    MSU1TRACK = track;
    
    while (MSU1STATUS & 0x40);    // Wait for track to load
    if (MSU1STATUS & 0x04)
        return;    // Track missing, exit
    
    /* TODO: edit SMO-ckery to load a message from somewhere not 0x0000
     * and as such have a proper busy loop. Confirm if the below would
     * do.
     */
    /* POSSIBILITY: for version 2 systems, let this function return bool.
     * If so, switch the results around and have true mean "it worked"? */
	MSU1VOLUME = 255;
    MSU1PLAY = 1;

    while (MSU1STATUS & 0x10);    // Wait for track to finish playing
}

