#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>
#include "video.h"

sprite_t *LoadImage(char *name);
void FreeImage(sprite_t **image);

void rdp_start();
void rdp_end();
void HardDrawImage(int x, int y, sprite_t *image);
void FillScreenWithTexture(sprite_t *image);

void SoftDrawImage(int x, int y, sprite_t *image);
void SoftDrawImageSolid(int x, int y, sprite_t *image);
void drawImageDMA(int x, int y, sprite_t *backgd);

void FillScreen(int r, int g, int b);
void ClearScreen();

void drawPatchBackground(int x, int y, sprite_t *sprite, sprite_t *backgd);
void drawPatchBackgroundFromCapture(int x, int y, sprite_t *sprite);


