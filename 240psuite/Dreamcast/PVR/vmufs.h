/* KallistiOS ##version##

   vmu.c
   (c)2002 Dan Potter
*/

/* This simple example shows how to use the vmu_pkg_* functions to write
   a file to a VMU with a DC-compatible header so it can be managed like
   any other VMU file from the BIOS menus. */

#include <kos.h>
#include <kos/string.h>

#define VMU_NAME 		"240PTEST"
#define SAVE_NUM		120			// We have DATA_SIZE/32 bytes free and zeroed until we need to change this
#define	DATA_SIZE		32
#define ICON_ARRAY_SIZE 5

int LoadVMUSave(char *error);
int WriteVMUSave(int icon, char *error);

