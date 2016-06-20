#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>
#include "video.h"

sprite_t *LoadImage(char *name);
void rdp_start();
void rdp_end();
void HardDrawImage(int x, int y, sprite_t *image);
void SoftDrawImage(int x, int y, sprite_t *image);

