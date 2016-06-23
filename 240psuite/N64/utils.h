#include <libdragon.h>
#include "image.h"
#include "video.h"

typedef unsigned char byte;

extern int fh;

void init_n64();
int DetectRamSize();

void DrawString(int x, int y, int r, int g, int b, char *str);
void DrawStringS(int x, int y, int r, int g, int b, char *str);

// from n64memory
#ifdef USE_N64MEM
extern void *n64_malloc(size_t size_to_alloc);
extern void *n64_memcpy(void *d, const void *s, size_t n);
extern void *n64_memset(void *p, int v, size_t n);
extern void n64_free(void *buf);
#else
#include <malloc.h>
#endif