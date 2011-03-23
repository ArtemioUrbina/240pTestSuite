.include "header.inc"
.include "Snes_Init.asm"
VBlank:    ; Needed to satisfy interrupt definition in "header.inc"
    RTI

.bank 0
.section "MainCode"

Start:
    ; Initialize the SNES.
    Snes_Init

    

    ; Loop forever.
Forever:
    jmp Forever

.ends