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
#define	VMU_CTRL_NAME	"240PCTRL"
#define VMU_NAME_LEN	8
#define SAVE_NUM		120			// We have DATA_SIZE/32 bytes free and zeroed until we need to change this
#define	DATA_SIZE		32
#define ICON_ARRAY_SIZE 5

#define VMU_OTHER_GAME	-7
#define VMU_LOADERR		-6
#define VMU_SMALLBUFF	-5
#define VMU_ERPARAM		-4
#define VMU_NOUNIT		-3
#define VMU_NOSPACE		-2
#define VMU_ERROR		-1
#define VMU_NOSAVE	 	0
#define VMU_OK	 		1
#define VMU_SAVEEXISTS	2

int isMemCardPresent();
int ListMemCardFiles(char *buffer, int buffsize);
int MemcardSaveExists(char *filename, int *blocks, int *port, int *unit);
int LoadMemCardSave(char *error);
int WriteMemCardSave(int icon, char *error);
int WriteMemCardControlTest(char *error);
int MemcardOtherGameExists(char *filename, int *port, int *unit);

