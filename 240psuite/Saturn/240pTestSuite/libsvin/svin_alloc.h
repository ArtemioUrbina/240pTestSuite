#ifndef _SVIN_ALLOC_H_
#define _SVIN_ALLOC_H_

#include "svin.h"

typedef struct {
    uint32_t address;
    uint32_t size;
} _svin_alloc_type;

void * _svin_alloc_lwram(int size, int address);
void _svin_free_lwram(int address);

#endif
