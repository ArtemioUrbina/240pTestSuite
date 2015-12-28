;
; Init code for 240p test suite
; Copyright 2015 Damian Yerrick
;
; Copying and distribution of this file, with or without
; modification, are permitted in any medium without royalty provided
; the copyright notice and this notice are preserved in all source
; code copies.  This file is offered as-is, without any warranty.
;
.include "nes.inc"
.include "global.inc"

.segment "CODE"
.proc reset_handler
  ; The very first thing to do when powering on is to put all sources
  ; of interrupts into a known state.
  sei             ; Disable interrupts
  ldx #$00
  stx PPUCTRL     ; Disable NMI and set VRAM increment to 32
  stx PPUMASK     ; Disable rendering
  stx $4010       ; Disable DMC IRQ
  dex             ; Subtracting 1 from $00 gives $FF, which is a
  txs             ; quick way to set the stack pointer to $01FF
  bit PPUSTATUS   ; Acknowledge stray vblank NMI across reset
  bit SNDCHN      ; Acknowledge DMC IRQ
  lda #$40
  sta P2          ; Disable APU Frame IRQ
  lda #$0F
  sta SNDCHN      ; Disable DMC playback, initialize other channels

vwait1:
  bit PPUSTATUS   ; It takes one full frame for the PPU to become
  bpl vwait1      ; stable.  Wait for the first frame's vblank.

  ; We have about 29700 cycles to burn until the second frame's
  ; vblank.  Use this time to get most of the rest of the chipset
  ; into a known state.

  ; Disable (normally nonfunctional) BCD mode for compatibility
  ; with debuggers and post-patent famiclones
  cld

  ; Clear OAM and the zero page here.
  ; We don't copy the cleared OAM to the PPU until later.
  ldx #0
  jsr ppu_clear_oam  ; clear out OAM from X to end and set X to 0

  txa
clear_zp:
  sta $00,x
  inx
  bne clear_zp
  
  ; Other things that can be done here (not shown):
  ; Set up PRG RAM
  ; Copy initial high scores, bankswitching trampolines, etc. to RAM
  ; Set up your sound engine
  ; Set up the mapper

vwait2:
  bit PPUSTATUS  ; After the second vblank, we know the PPU has
  bpl vwait2     ; fully stabilized.
  
  ; There are two ways to wait for vertical blanking: spinning on
  ; bit 7 of PPUSTATUS (as seen above) and waiting for the NMI
  ; handler to run.  Before the PPU has stabilized, you want to use
  ; the PPUSTATUS method because NMI might not be reliable.  But
  ; afterward, you want to use the NMI method because if you read
  ; PPUSTATUS at the exact moment that the bit turns on, it'll flip
  ; from off to on to off faster than the CPU can see.

  jmp main
.endproc

