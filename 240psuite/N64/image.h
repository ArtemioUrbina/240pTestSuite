#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>
#include "video.h"

sprite_t *LoadImage(char *name);
void FreeImage(sprite_t **image);

void rdp_texture_start();
void rdp_fill_start();
void rdp_rectangle(int x0, int y0, int x1, int y1, int r, int g, int b);
void rdp_DrawImage(int x, int y, sprite_t *image);
void rdp_DrawImageClipped(int x, int y, sprite_t *image, int CX, int CY);
void rdp_FillScreenWithTexture(sprite_t *image);
void rdp_FillScreenWithTextureXY(int x, int y, sprite_t *image);
void rdp_end();
void rdp_updatecache(int set);

void SoftDrawImage(int x, int y, sprite_t *image);
void SoftDrawImageSolid(int x, int y, sprite_t *image);
void drawImageDMA(int x, int y, sprite_t *backgd);
void drawScreenBufferDMA(int x, int y);

void ClearScreen();

void drawPatchBackground(int x, int y, sprite_t *sprite, sprite_t *backgd);
void drawPatchBackgroundFromCapture(int x, int y, sprite_t *sprite);

#ifdef DEBUG_BENCHMARK
void start_counter();
long long end_counter();
void Draw_counter(int x, int y, long long len);
#endif



