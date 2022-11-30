/* 
 * 240p Test Suite for the Neo Geo
 * by Artemio Urbina and Dustin Dembrosky (Dasutin)
 * Copyright (C)2011-2022 Artemio Urbina
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

//#include <stddef.h>
#include <stdarg.h>
#include <DATlib.h>
#include "types.h"
#include "ng.h"
#include "string_ng.h"

#define P01 10
#define P02 100
#define P03 1000
#define P04 10000
#define P05 100000
#define P06 1000000
#define P07 10000000
#define P08 100000000
#define P09 1000000000
#define P10 10000000000

static const char digits[] =
	"0001020304050607080910111213141516171819"
	"2021222324252627282930313233343536373839"
	"4041424344454647484950515253545556575859"
	"6061626364656667686970717273747576777879"
	"8081828384858687888990919293949596979899";

static u16 digits10(const u16 v);
static u16 uint16ToStr(u16 value, char *str, u16 minsize);

/* size_t strlen(const char *str)
{
	const char *src = str;
	while(*src++);
	return (src - str) - 1;
}

char* strcpy(char *to, const char *from)
{
	const char *src;
	char *dst;

	src = from;
	dst = to;
	while ((*dst++ = *src++));

	return to;
}

char* strcat(char *to, const char *from)
{
	const char *src;
	char *dst;

	src = from;
	dst = to;
	while (*dst++);

	--dst;
	while ((*dst++ = *src++));

	return to;
}

void* memcpy (volatile void *dest, const void *src, size_t len)
{
	char *d = dest;
	const char *s = src;
	while (len--)
	*d++ = *s++;
	return dest;
}

void memset(void* str, char ch, size_t n)
{
	int i;
	char *s = (char*) str;
	for(i=0; i<n; i++)
		s[i]=ch;
} */


u16 intToStr(s32 value, char *str, u16 minsize)
{
	if (value < -500000000)
	{
		*str = '-';
		return intToHex(-value, str + 1, minsize) + 1;
	}

	if (value < 0)
	{
		*str = '-';
		return uintToStr(-value, str + 1, minsize) + 1;
	}
	else return uintToStr(value, str, minsize);
}

u32 intToHex(u32 value, char *str, u16 minsize)
{
	u32 res;
	u16 cnt;
	u16 left;
	char data[16];
	char *src;
	char *dst;
	const u16 maxsize = 16;

	src = &data[16];
	res = value;
	left = minsize;

	cnt = 0;
	while (res)
	{
		u8 c;

		c = res & 0xF;

		if (c >= 10) c += ('A' - 10);
		else c += '0';

		*--src = c;
		cnt++;
		left--;
		res >>= 4;
	}
	while (left > 0)
	{
		*--src = '0';
		cnt++;
		left--;
	}

	if (cnt > maxsize) cnt = maxsize;

	dst = str;
	while(cnt--) *dst++ = *src++;
	*dst = 0;

	return 0;
}

u32 byteToBin(u8 value, char *str)
{
	u8 res, cnt;
	char data[10];
	char *src;
	char *dst;

	src = &data[9];
	res = value;
	
	for(cnt = 0; cnt < 8; cnt++)
	{
		u8 c;

		c = res & 0x01;

		if (c) c = '1';
		else c = '0';

		*--src = c;
		res >>= 1;

		if(cnt == 3)
			*--src = ' ';
	}
	
	cnt++;
	dst = str;
	while(cnt--) *dst++ = *src++;
	*dst = 0;

	return 0;
}

u16 uintToStr(u32 value, char *str, u16 minsize)
{
	u16 len;

	if (value > 500000000)
		return intToHex(value, str, minsize);

	if (value > 10000)
	{
		const u16 v1 = value / (u16) 10000;
		const u16 v2 = value % (u16) 10000;

		len = uint16ToStr(v1, str, (minsize > 4)?(minsize - 4):1);
		len += uint16ToStr(v2, str + len, 4);
	}
	else len = uint16ToStr(value, str, minsize);

	return len;
}

static u16 uint16ToStr(u16 value, char *str, u16 minsize)
{
	u16 length;
	char *dst;
	u16 v;

	length = digits10(value);
	if (length < minsize) length = minsize;
	dst = &str[length];
	*dst = 0;
	v = value;

	while (v >= 100)
	{
		const u16 quot = v / 100;
		const u16 remain = v % 100;

		const u16 i = remain * 2;
		v = quot;

		*--dst = digits[i + 1];
		*--dst = digits[i + 0];
	}

	if (v < 10) *--dst = '0' + v;
	else
	{
		const u16 i = v * 2;

		*--dst = digits[i + 1];
		*--dst = digits[i + 0];
	}

	while(dst != str) *--dst = '0';

	return length;
}

static u16 digits10(const u16 v)
{
	if (v < P02)
	{
		if (v < P01) return 1;
		return 2;
	}
	else
	{
		if (v < P03) return 3;
		if (v < P04) return 4;
		return 5;
	}
}

void fix32ToStr(fix32 value, char *str, u16 numdec)
{
	char *dst = str;
	fix32 v = value;
	u16 frac;
	u16 len;

	if (v < 0)
	{
		v = -v;
		*dst++ = '-';
	}

	dst += uintToStr(fix32ToInt(v), dst, 1);
	*dst++ = '.';

	// get fractional part
	frac = (((u16) fix32Frac(v)) * (u16) 1000) / ((u16) 1 << FIX32_FRAC_BITS);
	len = uint16ToStr(frac, dst, 3);

	if (len < numdec)
	{
		// need to add ending '0'
		dst += len;
		while(len++ < numdec) *dst++ = '0';
		// mark end here
		*dst = 0;
	}
	else dst[numdec] = 0;
}
