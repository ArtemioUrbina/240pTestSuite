#include "types.h"

const struct
{
    char console[16];               /* Console Name (16) */
    char copyright[16];             /* Copyright Information (16) */
    char title_local[48];           /* Domestic Name (48) */
    char title_int[48];             /* Overseas Name (48) */
    char serial[14];                /* Serial Number (2, 12) */
    u16 checksum;                   /* Checksum (2) */
    char IOSupport[16];             /* I/O Support (16) */
    u32 rom_start;                  /* ROM Start Address (4) */
    u32 rom_end;                    /* ROM End Address (4) */
    u32 ram_start;                  /* Start of RAM (4) */
    u32 ram_end;                    /* End of RAM (4) */
    char sram_sig[2];               /* "RA" for save ram (2) */
    u16 sram_type;                  /* 0xF820 for save ram on odd bytes (2) */
    u32 sram_start;                 /* SRAM start address - normally 0x200001 (4) */
    u32 sram_end;                   /* SRAM end address - start + 2*sram_size (4) */
    char modem_support[12];         /* Modem Support (24) */
    char notes[40];                 /* Memo (40) */
    char region[16];                /* Country Support (16) */
} rom_header = {
    "SEGA MEGA DRIVE ",	/* Console Name (16) */
    "(C) AUG 2022.JAN", /* Copyright Information (16) */
    "240P TEST SUITE                                 ", /* Domestic Name (48) */
    "240P TEST SUITE                                 ", /* Overseas Name (48) */
    "GM 00002501-23",	/* Serial Number (2, 12) */
    0x0000,				/* Checksum (2), fixed by tool in makefile */
    "J64             ",	/* I/O Support (16) */
    0x00000000,			/* ROM Start Address (4) */
    0x0003FFFF,			/* ROM End Address (4), fixed by tool in makefile */
    0x00FF0000,			/* Start of RAM (4) */
    0x00FFFFFF,			/* End of RAM (4) */
    "  ",				/* "RA" for save ram (2) */
    0x0000,				/* 0xF820 for save ram on odd bytes (2) */
    0x20202020,			/* SRAM start address - normally 0x200001 (4) */
    0x20202020,			/* SRAM end address - start + 2*sram_size (4) */
    "            ",		/* Modem Support (24) */
    "ARTEMIO URBINA 2022                     ",	/* Memo (40) */
    "JUE             "	/* Country Support (16) */
};
