/*
**  CRC.H - header file for SNIPPETS CRC and checksum functions
**  original licence is public domain, modifications for public domain by Artemio Urbina
*/

#ifndef CRC__H
#define CRC__H

#include <stdlib.h>           /* For size_t                 */
typedef unsigned char  BYTE;
typedef unsigned long  DWORD;
typedef unsigned short WORD;

/*
**  File: CRC_32.C
*/

#define UPDC32(octet,crc) (crc_32_tab[((crc)\
     ^ ((BYTE)octet)) & 0xff] ^ ((crc) >> 8))

DWORD crc32buf(char *buf, size_t len);


#endif /* CRC__H */