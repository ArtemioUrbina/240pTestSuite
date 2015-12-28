;
; Multiply and divide routines
; Copyright 2012-2015 Damian Yerrick
;
; Copying and distribution of this file, with or without
; modification, are permitted in any medium without royalty provided
; the copyright notice and this notice are preserved in all source
; code copies.  This file is offered as-is, without any warranty.
;
.exportzp prodlo, prodmlo, prodmhi, prodhi, fac1lo, fac1hi, fac2lo, fac2hi
.export mul16x16, div32x16

prodlo  = $00
prodmlo = $01
prodmhi = $02
prodhi  = $03
fac1lo  = prodlo
fac1hi  = prodmlo
fac2lo  = $04
fac2hi  = $05

.proc mul16x16
  lda #$00
  sta prodmhi
  sta prodhi
  ldy #16
  
  ; Prime the carry bit loop
  lsr prodmlo
  ror prodlo
loop:
  bcc noadd
  clc
  lda fac2lo
  adc prodmhi
  sta prodmhi
  lda fac2hi
  adc prodhi
  sta prodhi
noadd:
  ror prodhi
  ror prodmhi
  ror prodmlo
  ror prodlo
  dey
  bne loop
  rts
.endproc


.proc div32x16
  ldy #16
  ; prod is the dividend
  asl prodlo
  rol prodmlo
loop:
  rol prodmhi
  rol prodhi
  bcs already_greater
  lda prodmhi
  cmp fac2lo
  lda prodhi
  sbc fac2hi
  bcc not_greater
already_greater:
  lda prodmhi
  sbc fac2lo
  sta prodmhi
  lda prodhi
  sbc fac2hi
  sta prodhi
  sec
not_greater:
  rol prodlo
  rol prodmlo
  dey
  bne loop
  rts
.endproc
