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
 
#include <snes.h>

// taken from http://helmet.kafuka.org/msu1.htm
#define MSU1SEEK	*(unsigned long*)0x2000
#define MSU1TRACK	*(unsigned short*)0x2004
#define MSU1VOLUME	*(unsigned char*)0x2006
#define MSU1PLAY	*(unsigned char*)0x2007
#define MSU1STATUS	*(unsigned char*)0x2000
#define MSU1DATA	*(unsigned char*)0x2001
//A.N.: I don't even remember why I did it this way.
#define MSU1IDENT0	*(char*)0x2002
#define MSU1IDENT1	*(char*)0x2003
#define MSU1IDENT2	*(char*)0x2004
#define MSU1IDENT3	*(char*)0x2005
#define MSU1IDENT4	*(char*)0x2006
#define MSU1IDENT5	*(char*)0x2007

int msu1available();
void msu1play(unsigned int track);