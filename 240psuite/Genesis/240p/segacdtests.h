//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "genesis.h"
#include "types.h"
#include "main.h"

#define MEMORY_OK 0xFFFFFFFF

void SegaCDMenu();
int DetectSCDBIOS();
void Z80RamTest();

void CRC32_reset();
void CRC32_update(uint8_t data);
uint32_t CRC32_finalize();
void MemViewer(uint32_t address);

