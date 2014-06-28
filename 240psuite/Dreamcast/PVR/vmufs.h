/* KallistiOS ##version##

   vmu.c
   (c)2002 Dan Potter
*/

/* This simple example shows how to use the vmu_pkg_* functions to write
   a file to a VMU with a DC-compatible header so it can be managed like
   any other VMU file from the BIOS menus. */

#include <kos.h>
#include <kos/string.h>

int readvmu();
int writevmu(int icon);


