#include <libdragon.h>
#include "image.h"
#include "video.h"

extern int fh;

void init_n64();
int DetectExpansionPack();

void DrawString(int x, int y, int r, int g, int b, char *str);
void DrawStringS(int x, int y, int r, int g, int b, char *str);