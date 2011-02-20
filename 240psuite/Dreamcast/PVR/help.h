#ifndef HELP_H
#define HELP_H

#include "image.h"

#define GENERALHELP			"/rd/help/general.txt"
#define PLUGEHELP				"/rd/help/pluge.txt"
#define SOUNDHELP				"/rd/help/sound.txt"

uint16 HelpWindow(char *filename, ImagePtr screen, uint16 main);
char *LoadHelpFile(char *filename, char ***pages, int *npages);

#endif

