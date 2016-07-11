#include <libdragon.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "image.h"
#include "controller.h"
#include "video.h"

#ifndef UTILS_H
#define UTILS_H

#define VERSION_NUMBER "N64 Beta 0.1"
#define VERSION_DATE "2016/07/11"

typedef unsigned char byte;

extern int fh;

void init_n64();
int DetectRamSize();

void DrawString(int x, int y, int r, int g, int b, char *str);
void DrawStringS(int x, int y, int r, int g, int b, char *str);
void DrawStringB(int x, int y, int r, int g, int b, char *str);

// from n64memory
#ifdef USE_N64MEM
extern void *n64_malloc(size_t size_to_alloc);
extern void *n64_memcpy(void *d, const void *s, size_t n);
extern void *n64_memset(void *p, int v, size_t n);
extern void n64_free(void *buf);
#else
#include <malloc.h>
#endif

#endif