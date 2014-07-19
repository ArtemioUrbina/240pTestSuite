/* 
 * 240p Test Suite
 * Copyright (C)2014 Artemio Urbina (SNES)
 *
 * This file is part of the 240p Test Suite
 *
 * The 240p Test Suite is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The 240p Test Suite is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 
.MEMORYMAP                      ; Begin describing the system architecture.
  SLOTSIZE $8000                ; The slot is $8000 bytes in size. More details on slots later.
  DEFAULTSLOT 0                 ; There's only 1 slot in SNES, there are more in other consoles.
  SLOT 0 $8000                  ; Defines Slot 0's starting address.
  SLOT 1 $0 $2000
  SLOT 2 $2000 $E000
  SLOT 3 $0 $10000
.ENDME          ; End MemoryMap definition

.ROMBANKSIZE $8000              ; Every ROM bank is 32 KBytes in size
.ROMBANKS 16                     ; 2 Mbits - Tell WLA we want to use 8 ROM Banks

.SNESHEADER
  ID "SNES"                     ; 1-4 letter string, just leave it as "SNES"

  NAME "240p Test Suite SNES "  ; Program Title - can't be over 21 bytes,
  ;    "123456789012345678901"  ; use spaces for unused bytes of the name.

  SLOWROM
  LOROM

  CARTRIDGETYPE $00             ; $02 = ROM+SRAM, see WLA documentation for others
  ROMSIZE $09                   ; $08 = 2 Mbits,  see WLA doc for more..
  SRAMSIZE $00                  ; $01 = 16 kbits, see WLA doc for more..
  COUNTRY $01                   ; $01 = U.S.  $00 = Japan, that's all I know
  LICENSEECODE $00              ; Just use $00
  VERSION $01                   ; $00 = 1.00, $01 = 1.01, etc.
.ENDSNES

.SNESNATIVEVECTOR               ; Define Native Mode interrupt vector table
  COP EmptyHandler
  BRK EmptyHandler
  ABORT EmptyHandler
  NMI VBlank
  IRQ EmptyHandler
.ENDNATIVEVECTOR

.SNESEMUVECTOR                  ; Define Emulation Mode interrupt vector table
  COP EmptyHandler
  ABORT EmptyHandler
  NMI EmptyHandler
  RESET tcc__start                   ; where execution starts
  IRQBRK EmptyHandler
.ENDEMUVECTOR
