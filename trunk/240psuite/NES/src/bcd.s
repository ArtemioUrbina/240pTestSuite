;
; Binary to decimal conversion for 8-bit and 24-bit numbers
; Copyright 2012-2015 Damian Yerrick
;
; Copying and distribution of this file, with or without
; modification, are permitted in any medium without royalty provided
; the copyright notice and this notice are preserved in all source
; code copies.  This file is offered as-is, without any warranty.
;
.export bcd8bit

; This file contains functions to convert binary numbers (base 2)
; to decimal representation (base 10).  Most of the hex to decimal
; routines on 6502.org rely on the MOS Technology 6502's decimal
; mode for ADC and SBC, which was removed from the Ricoh parts.
; These do not use decimal mode.

; The 8-bit routine ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.macro bcd8bit_iter value
  .local skip
  cmp value
  bcc skip
  sbc value
skip:
  rol highDigits
.endmacro

;;
; Converts a decimal number to two or three BCD digits
; in no more than 80 cycles.
; @param a the number to change
; @return a: low digit; 0: upper digits as nibbles
; No other memory or register is touched.
.proc bcd8bit
highDigits = 0

  ; First clear out two bits of highDigits.  (The conversion will
  ; fill in the other six.)
  asl highDigits
  asl highDigits

  ; Each iteration takes 11 if subtraction occurs or 10 if not.
  ; But if 80 is subtracted, 40 and 20 aren't, and if 200 is
  ; subtracted, then 100, 80, and one of 40 and 20 aren't.
  ; So this part takes up to 6*11-2 cycles.
  bcd8bit_iter #200
  bcd8bit_iter #100
  bcd8bit_iter #80
  bcd8bit_iter #40
  bcd8bit_iter #20
  bcd8bit_iter #10
  rts
.endproc

; 24-bit version ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.exportzp bcdNum, bcdResult
.export bcdConvert24, bcdWrite, bcdWrite0

BCD_BITS = 29
BCD_OUTSIZE = (BCD_BITS + 3) / 4

.segment "ZEROPAGE"
bcdResult: .res BCD_OUTSIZE
bcdNum = $00
curDigit = $03
bhi = bcdResult+0
blo = $04

.segment "CODE"
.proc bcdConvert24
  lda #$80 >> ((BCD_BITS - 1) & 3)
  sta curDigit
  ldx #BCD_OUTSIZE - 1
  ldy #BCD_BITS - 5

@loop:
  ; Trial subtract this bit to A:bhi:blo
  sec
  lda bcdNum
  sbc bcdTableLo,y
  sta blo
  lda bcdNum+1
  sbc bcdTableHi,y
  sta bhi
  lda bcdNum+2
  sbc bcdTableBank,y

  ; If A:b > bcdNum then bcdNum = A:bhi:blo
  bcc @trial_lower
  sta bcdNum+2
  lda bhi
  sta bcdNum+1
  lda blo
  sta bcdNum
@trial_lower:

  ; Copy bit from carry into digit and pick up 
  ; end-of-digit sentinel into carry
  rol curDigit
  dey
  bcc @loop

  ; Copy digit into result
  lda curDigit
  sta bcdResult,x
  lda #$10  ; Empty digit; sentinel at 4 bits
  sta curDigit
  ; If there are digits left, do those
  dex
  bne @loop
  lda bcdNum
  sta bcdResult
  rts
.endproc


;;
; Writes a decimal number to a string buffer.
; @param A high byte of destination address
; @param Y low byte of destination address
; @param X number of decimal places
; @return $00: start of string; A, Y: end of written string; X: length
.proc bcdWrite
dstlo = $00
dsthi = $01
  sta dsthi
  sty dstlo
.endproc
.proc bcdWrite0
dstlo = bcdWrite::dstlo
dsthi = bcdWrite::dsthi
decimals = $02
  dex
  stx decimals
  ldx #BCD_OUTSIZE-1
  ldy #0
loop:
  cpx decimals
  bne not_decimal_point
  lda #'.'
  sta (dstlo),y
  iny
not_decimal_point:
  lda bcdResult,x
  bne have_digit
  cpx #0
  beq have_digit
  cpy #0
  beq skip_digit
have_digit:
  ora #'0'
  sta (dstlo),y
  iny  
skip_digit:
  dex
  bpl loop
  tya
  tax
  clc
  adc dstlo
  tay
  lda dsthi
  adc #0
  rts
.endproc

.segment "RODATA"
.define bcdtens1 10,20,40,80,100,200,400,800,1000,2000,4000,8000
.define bcdtens2 10000,20000,40000,80000,100000,200000,400000,800000
.define bcdtens3 1000000,2000000,4000000,8000000,10000000

bcdTableLo:   .lobytes   bcdtens1, bcdtens2, bcdtens3
bcdTableHi:   .hibytes   bcdtens1, bcdtens2, bcdtens3
bcdTableBank: .bankbytes bcdtens1, bcdtens2, bcdtens3

