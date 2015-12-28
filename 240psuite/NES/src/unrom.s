;
; UNROM driver for NES
; Copyright 2011-2015 Damian Yerrick
;
; Copying and distribution of this file, with or without
; modification, are permitted in any medium without royalty provided
; the copyright notice and this notice are preserved in all source
; code copies.  This file is offered as-is, without any warranty.
;

.import nmi_handler, reset_handler, irq_handler, bankcall_table

.segment "INESHDR"
  .byt "NES",$1A  ; magic signature
  .byt 4          ; size of PRG ROM in 16384 byte units
  .byt 0          ; size of CHR ROM in 8192 byte units
  .byt $21        ; lower mapper nibble, vertical mirroring
  .byt $00        ; upper mapper nibble
  
.segment "ZEROPAGE"
lastPRGBank: .res 1
lastBankMode: .res 1
bankcallsaveA: .res 1

; On most discrete logic mappers (AOROM 7, BNROM 34, GNROM 66, and
; Crazy Climber UNROM 180), writing a value with bits 5-0 true
; (that is, $3F, $7F, $BF, $FF) switches in the last PRG bank, but
; it has to be written to a ROM address that has the same value.
.macro resetstub_in segname
.segment segname
.scope
resetstub_entry:
  sei
  ldx #$FF
  txs
  stx $FFF2  ; Writing $80-$FF anywhere in $8000-$FFFF resets MMC1
  jmp reset_handler
  .addr nmi_handler, resetstub_entry, irq_handler
.endscope
.endmacro

resetstub_in "STUB15"

.segment "CODE"

;;
; Changes $8000-$BFFF to point to a 16384 byte chunk of PRG ROM
; starting at $4000 * A.
.proc setPRGBank
  sta lastPRGBank
  tay
  sta identity16,y
  rts
.endproc

.segment "RODATA"
; To avoid bus conflicts, bankswitch needs to write a value
; to a ROM address that already contains that value.
identity16:
  .repeat 16, I
    .byte I
  .endrepeat
