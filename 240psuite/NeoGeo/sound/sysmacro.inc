; 240p Test Suite Sound Driver
; Based on freem ADPCM example at: http://ajworld.net/neogeodev/
;==============================================================================;
; SetBank
; A macro for setting one of the Z80 banks.

; Params:
; \1            Bank number
; \2            Bank slot (8,9,0xA,0xB)

    macro   SetBank
    ld      a,#\1
    in      a,(\2)
    endm
