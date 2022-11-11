;******************************************************************************
; Perspective - A VMU Graphics Library
;******************************************************************************
; Version: 0.1
; ------------
; Perspective is a drawing library for the VMU which uses WRAM to allow for
; fast, artifact-free drawing to VMU XRAM.
;
; (C) Kresna Susila - 2018
;******************************************************************************
; Resource Usage:
; ---------------
;       RAM1:   $F2-$FF         (Adjustable, be cautious of sizes)
;       WRAM1:  $00-$BF         (Adjustable with User Constants)
;
;------------------------------------------------------------------------------
; User Macros:
; ------------
;
; Background Drawing Macros:
; --------------------------
;
;       P_Draw_Background
;       -----------------
;               Args: 1
;               Accepts: Background Image Address (2 bytes)
;               Destroys: ACC, C, TRL, TRH, VRMAD1, VRMAD2, VSEL, PSW
;               Copies a bitmap 48x32 pixels in size. Uses standard Perspective
;       image format, but ignores size bits. Accepts only an address from RAM.
;
;       P_Draw_Background_Constant
;       -----------------
;               Args: 1
;               Accepts: Background Image Address (16bit Constant)
;               Destroys: ACC, C, TRL, TRH, VRMAD1, VRMAD2, VSEL, PSW
;               Copies a bitmap 48x32 pixels in size. Uses standard Perspective
;       image format, but ignores size bits. Accepts only constants.
;
;
; Sprite Drawing Macros:
; ----------------------
;
;       P_Draw_Sprite
;       ----------------------
;               Args: 3
;               Accepts: Sprite Address (1 bytes)
;                        Sprite X Coordinate (1 byte)
;                        Sprite Y Coordinate (1 byte)
;               Destroys: ACC, B, C, TRL, TRH, VRMAD1, VRMAD2, VSEL, PSW,
;                         @R0 Pointer
;       Draws a sprite directly onto the screen without a sprite mask. Sprites
;       use the standard format. Accepts only RAM addresses for all arguments.
;
;       P_Draw_Sprite_Constant
;       ----------------------
;               Args: 3
;               Accepts: Sprite Address (16bit Constant)
;                        Sprite X Coordinate (8bit Constant)
;                        Sprite Y Coordinate (8bit Constant)
;               Destroys: ACC, B, C, TRL, TRH, VRMAD1, VRMAD2, VSEL, PSW,
;                         @R0 Pointer
;       Draws a sprite directly onto the screen without a sprite mask. Sprites
;       use the standard format. Accepts only constants for all arguments.
;
;       P_Draw_Sprite_Mask
;       ---------------------------
;               Args: 3
;               Accepts: Sprite Address (2 bytes)
;                        Sprite X Coordinate (1 byte)
;                        Sprite Y Coordinate (1 byte)
;               Destroys: ACC, B, C, TRL, TRH, VRMAD1, VRMAD2, VSEL, PSW,
;                         @R0 Pointer
;       Draws a sprite onto the screen with a sprite mask. Sprites use the
;       masked sprite format. Accepts only RAM addresses for all arguments.
;
;       P_Draw_Sprite_Mask_Constant
;       ---------------------------
;               Args: 3
;               Accepts: Sprite Address (16bit Constant)
;                        Sprite X Coordinate (8bit Constant)
;                        Sprite Y Coordinate (8bit Constant)
;               Destroys: ACC, B, C, TRL, TRH, VRMAD1, VRMAD2, VSEL, PSW,
;                         @R0 Pointer
;       Draws a sprite onto the screen with a sprite mask. Sprites use the
;       masked sprite format. Accepts only constants for all arguments.
;
;
; Screen Macros:
; --------------
;
;       P_Blit_Screen
;       -------------
;               Args: 0
;               Accepts: None
;               Destroys: ACC, C, VRMAD1, VRMAD2, VSEL, PSW, @R2 Pointer
;               Alias for calling the P_Blit_Screen function. Blits the WRAM
;       buffer to XRAM. Call this once per frame to update the XRAM
;       framebuffer.
;
;       P_Fill_Screen
;       -------------
;               Args: 1
;               Accepts: Fill Value (8bit Constant)
;               Destroys: ACC, C, VRMAD1, VRMAD2, VSEL
;               Fills the screen with the given value. Any value is valid.
;       Built-in constants are P_WHITE and P_BLACK.
;
;       P_Mask_Fill_Screen
;       -------------
;               Args: 1
;               Accepts: Mask Value (1 byte)
;               Destroys: ACC, B, C, VRMAD1, VRMAD2, VSEL
;               Masks the screen with a value from RAM.
;
;       P_Mask_Fill_Screen_Constant
;       -------------
;               Args: 1
;               Accepts: Mask Value (8bit Constant)
;               Destroys: ACC, B, C, VRMAD1, VRMAD2, VSEL
;               Masks the screen with the given constant value.
;
;       P_Clear_Screen
;       -------------
;               Args: 0
;               Accepts: None
;               Destroys: ACC, C, VRMAD1, VRMAD2, VSEL
;               Alias for 'P_Fill_Screen P_WHITE'. Clears the WRAM framebuffer.
;
;       P_Invert_Screen
;       ---------------
;               Args: 0
;               Accepts: None
;               Destroys: ACC, C, VRMAD1, VRMAD2, VSEL
;               Alias for calling the P_Invert_Screen function. Inverts all
;               bits of the WRAM framebuffer.
;
; User Functions:
; ---------------
;       P_Blit_Screen
;       -----------
;               Accepts: None
;               Destroys: ACC, C, VRMAD1, VRMAD2, VSEL, PSW, @R2 Pointer
;               Blits the WRAM buffer to XRAM. Call this once per frame to
;       update the XRAM framebuffer.
;
;       P_Invert_Screen
;       ---------------
;               Accepts: None
;               Destroys: ACC, C, VRMAD1, VRMAD2, VSEL
;               Inverts all bits of the WRAM framebuffer.
;
;------------------------------------------------------------------------------
; Internal Functions:
; -------------------
;       _P_Draw_Background
;       _P_Fill_Screen
;       _P_Mask_Fill_Screen
;       _P_Draw_Sprite_No_Mask
;       _P_Draw_Sprite_Mask
;       _P_Sprite_Buffer_Common
;       _P_Blit_Pass_Unrolled
;
; Internal Macros:
; ----------------
;       None
;
;------------------------------------------------------------------------------
; Example Sprite Format (No Mask):
; --------------------------------
; Sprites that have no mask are drawn approximately twice as fast as those
; which use a mask. Backgrounds follow the same format as a non-masked sprite,
; but have a fixed size of 48x32 pixels. Even though the size is fixed, the
; pixel dimensions must still be declared as if it was a large sprite.
;
; Sprite:
;       .byte   4, 4            ; Sprite X and Y size in pixels.
;       .byte   %11110000       ; Sprite bitmap, a sprite larger than 8 pixels
;       .byte   %10010000       ; will span across multiple bytes. Sprites must
;       .byte   %10010000       ; be byte aligned.
;       .byte   %11110000
;
; Example Sprite Format (With Mask):
; ----------------------------------
; Masked_Sprite:
;       .byte   4, 4            ; Sprite X and Y size in pixels.
;       .byte   %00001111       ; Sprite mask
;       .byte   %00001111
;       .byte   %00001111
;       .byte   %00001111
;       .byte   %11110000       ; Sprite bitmap
;       .byte   %10010000
;       .byte   %10010000
;       .byte   %11110000
;
;******************************************************************************

;------------------------------------------------------------------------------
; Library Variables
;------------------------------------------------------------------------------
p_spr_x                 =       $ff             ; 1 byte
p_spr_y                 =       $fe             ; 1 byte
p_spr_flag              =       $fd             ; 1 byte
p_spr_size_x            =       $fc             ; 1 byte
p_spr_size_y            =       $fb             ; 1 byte
p_spr_size_x_remainder  =       $fa             ; 1 byte
p_spr_counter           =       $f9             ; 1 byte
p_spr_offset_x          =       $f8             ; 1 byte
p_byte_buffer           =       $f2             ; 6 bytes

;------------------------------------------------------------------------------
; Library Constants
;------------------------------------------------------------------------------
;User Constants:
P_WRAM_BANK             EQU     1               ; WRAM Bank to use for buffer
P_WRAM_ADDR             EQU     0               ; WRAM buffer start address
P_PSW_IRBKX             EQU     %00000010       ; PSW IRBK setting
P_INDR_SFR              EQU     2               ; Address for @R2
P_INDR_MEM              EQU     0               ; Address for @R0
P_BYTE_BUFFER_LOC       EQU     $f2             ; Location of byte buffer

; Library Constants:
P_WHITE                 EQU     $00
P_BLACK                 EQU     $FF
;------------------------------------------------------------------------------
%macro  P_Draw_Sprite_Mask %spr_addr, %spr_x, %spr_y
        ;----------------------------------------------------------------------
        ; Draw a sprite to the background with a mask using stored RAM values
        ;----------------------------------------------------------------------
        ; Draws a sprite onto the screen with a sprite mask.
        ; Sprites use the standard format. Accepts only RAM addresses for all
        ; arguments
        ;
        ; Accepts:      Sprite Address (2 bytes)
        ;               Sprite X Coordinate (1 byte)
        ;               Sprite Y Coordinate (1 byte)
        ; Destroys:     ACC, B, C, TRL, TRH, VRMAD1, VRMAD2, VSEL, PSW
        ;----------------------------------------------------------------------
        ; Preload RAM
        ld      %spr_x
        st      p_spr_x
        ld      %spr_y
        st      p_spr_y
        ld      %spr_addr
        st      trl
        ld      %spr_addr+1
        st      trh
        call    _P_Draw_Sprite_Mask
%end

%macro  P_Draw_Sprite_Mask_Constant %spr_addr, %spr_x, %spr_y
        ;----------------------------------------------------------------------
        ; Draw a sprite to the background with a mask using constant values
        ;----------------------------------------------------------------------
        ; Draws a sprite onto the screen with a sprite mask.
        ; Sprites use the standard format. Accepts only constant values for
        ; all arguments.
        ;
        ; Accepts:      Sprite Address (16bit Constant)
        ;               Sprite X Coordinate (8bit Constant)
        ;               Sprite Y Coordinate (8bit Constant)
        ; Destroys:     ACC, B, C, TRL, TRH, VRMAD1, VRMAD2, VSEL, PSW
        ;----------------------------------------------------------------------
        ; Preload RAM
        mov     #%spr_x, p_spr_x
        mov     #%spr_y, p_spr_y
        mov     #<%spr_addr, trl
        mov     #>%spr_addr, trh
        call    _P_Draw_Sprite_Mask
%end

%macro  P_Draw_Sprite %spr_addr, %spr_x, %spr_y
        ;----------------------------------------------------------------------
        ; Draw a sprite to the background with no mask using stored RAM values
        ;----------------------------------------------------------------------
        ; Draws a sprite directly onto the screen without a sprite mask.
        ; Sprites use the standard format. Accepts only constant values for
        ; all arguments.
        ;
        ; Accepts:      Sprite Address (16bit Constant)
        ;               Sprite X Coordinate (8bit Constant)
        ;               Sprite Y Coordinate (8bit Constant)
        ; Destroys:     ACC, B, C, TRL, TRH, VRMAD1, VRMAD2, VSEL, PSW
        ;----------------------------------------------------------------------
        ; Preload RAM
        ld      %spr_x
        st      p_spr_x
        ld      %spr_y
        st      p_spr_y
        ld      %spr_addr
        st      trl
        ld      %spr_addr+1
        st      trh
        call    _P_Draw_Sprite_No_Mask
%end

%macro  P_Draw_Sprite_Constant %spr_addr, %spr_x, %spr_y
        ;----------------------------------------------------------------------
        ; Draw a sprite to the background with no mask using constant values
        ;----------------------------------------------------------------------
        ; Draws a sprite directly onto the screen without a sprite mask.
        ; Sprites use the standard format. Accepts only constant values for
        ; all arguments.
        ;
        ; Accepts:      Sprite Address (16bit Constant)
        ;               Sprite X Coordinate (8bit Constant)
        ;               Sprite Y Coordinate (8bit Constant)
        ; Destroys:     ACC, B, C, TRL, TRH, VRMAD1, VRMAD2, VSEL, PSW
        ;----------------------------------------------------------------------
        ; Preload RAM
        mov     #%spr_x, p_spr_x
        mov     #%spr_y, p_spr_y
        mov     #<%spr_addr, trl
        mov     #>%spr_addr, trh
        call    _P_Draw_Sprite_No_Mask
%end

%macro  P_Blit_Screen
        ;----------------------------------------------------------------------
        ; Blit WRAM to XRAM
        ;----------------------------------------------------------------------
        ; Accepts:      None
        ; Destroys:     ACC, C, VRMAD1, VRMAD2, VSEL, PSW
        ;
        ; Alias for calling the P_Blit_Screen function.
        ;----------------------------------------------------------------------
        call    P_Blit_Screen
%end

%macro  P_Draw_Background %bgaddr
        ;----------------------------------------------------------------------
        ; Draw a background image to WRAM
        ;----------------------------------------------------------------------
        ; Accepts:      Background Image address (16bit Constant)
        ; Destroys:     ACC, C, TRL, TRH, VRMAD1, VRMAD2, VSEL
        ;
        ; Copies a bitmap 48x32 pixels in size. Uses standard Perspective image
        ; format, but ignores size bits. Controls _P_Draw_Background.
        ;----------------------------------------------------------------------
        ld      %bgaddr+1
        st      trh
        ld      %bgaddr
        add     #2
        st      trl
        bn      psw, 7, .no_overflow%
        inc     trh
.no_overflow%
        call    _P_Draw_Background
%end

%macro  P_Draw_Background_Constant %bgaddr
        ;----------------------------------------------------------------------
        ; Draw a background image to WRAM
        ;----------------------------------------------------------------------
        ; Accepts:      Background Image address (16bit Constant)
        ; Destroys:     ACC, C, TRL, TRH, VRMAD1, VRMAD2, VSEL
        ;
        ; Copies a bitmap 48x32 pixels in size. Uses standard Perspective image
        ; format, but ignores size bits. Controls _P_Draw_Background.
        ;----------------------------------------------------------------------
        mov     #<(%bgaddr+2), trl
        mov     #>(%bgaddr+2), trh
        call    _P_Draw_Background
%end

%macro  P_Fill_Screen %value
        ;----------------------------------------------------------------------
        ; Fill the screen with a value
        ;----------------------------------------------------------------------
        ; Accepts:      Fill Value (8bit Constant)
        ; Destroys:     ACC, C, VRMAD1, VRMAD2, VSEL
        ;
        ; Fills the screen with the value of ACC.
        ;----------------------------------------------------------------------
        mov     #%value, acc
        call    _P_Fill_Screen
%end

%macro  P_Mask_Fill_Screen %value
        ;----------------------------------------------------------------------
        ; Mask the screen with a value from RAM
        ;----------------------------------------------------------------------
        ; Accepts:      Fill Value (1 byte)
        ; Destroys:     ACC, B, C, VRMAD1, VRMAD2, VSEL
        ;
        ; Mask the screen with a RAM value.
        ;----------------------------------------------------------------------
        ld      %value
        call    _P_Mask_Fill_Screen
%end

%macro  P_Mask_Fill_Screen_Constant %value
        ;----------------------------------------------------------------------
        ; Mask the screen with a constant value
        ;----------------------------------------------------------------------
        ; Accepts:      Fill Value (8bit Constant)
        ; Destroys:     ACC, B, C, VRMAD1, VRMAD2, VSEL
        ;
        ; Mask the screen with an 8bit constant.
        ;----------------------------------------------------------------------
        mov     #%value, acc
        call    _P_Mask_Fill_Screen
%end

%macro  P_Clear_Screen
        ;----------------------------------------------------------------------
        ; Clears the screen
        ;----------------------------------------------------------------------
        ; Accepts:      None
        ; Destroys:     ACC, C, VRMAD1, VRMAD2, VSEL
        ;
        ; Alias for 'P_Fill_Screen P_WHITE'.
        ;----------------------------------------------------------------------
        mov     #P_WHITE, acc
        call    _P_Fill_Screen
%end

%macro P_Invert_Screen
        ;----------------------------------------------------------------------
        ; Invert the screen colours
        ;----------------------------------------------------------------------
        ; Accepts:      None
        ; Destroys:     ACC, C, VRMAD1, VRMAD2, VSEL
        ;
        ; Alias for calling the P_Invert_Screen function.
        ;----------------------------------------------------------------------
        call    P_Invert_Screen
%end

P_Invert_Screen:
        ;----------------------------------------------------------------------
        ; Invert the screen colours
        ;----------------------------------------------------------------------
        ; Accepts:      None
        ; Destroys:     ACC, C, VRMAD1, VRMAD2, VSEL
        ;
        ; Inverts the screen colours
        ;----------------------------------------------------------------------
        clr1    ocr, 5
        ; Prepare the frame buffer address
        mov     #P_WRAM_BANK, vrmad2
        mov     #P_WRAM_ADDR, vrmad1
        mov     #%00000000, vsel

        mov     #48, c
.loop
        ld      vtrbf
        xor     #%11111111
        st      vtrbf
        inc     vrmad1
        ld      vtrbf
        xor     #%11111111
        st      vtrbf
        inc     vrmad1
        ld      vtrbf
        xor     #%11111111
        st      vtrbf
        inc     vrmad1
        ld      vtrbf
        xor     #%11111111
        st      vtrbf
        inc     vrmad1
        dbnz    c, .loop
        set1    ocr, 5
        ret

P_Blit_Screen:
        ;----------------------------------------------------------------------
        ; Blit WRAM to XRAM
        ;----------------------------------------------------------------------
        ; Accepts:      None
        ; Destroys:     ACC, C, VRMAD1, VRMAD2, VSEL, PSW
        ;
        ; Blits the WRAM buffer to XRAM. Call this once per frame to update the
        ; XRAM framebuffer.
        ;----------------------------------------------------------------------
        clr1    ocr, 5
        ; Prepare the frame buffer address
        mov     #P_WRAM_BANK, vrmad2
        mov     #P_WRAM_ADDR, vrmad1
        mov     #%00010000, vsel

        ; Copy the frame buffer to XRAM quickly
        ; Configure indirect addressing mode
        mov     #0, xbnk
        mov     #P_PSW_IRBKX, psw

        ; Fast XRAM Copy
        ; ---------------------------------------------------------------------
        ; Pass 1, Bank 0
        mov     #$80, P_INDR_SFR
        call    _P_Blit_Pass_Unrolled

        ; Pass 2, Bank 0
        mov     #$90, P_INDR_SFR
        call    _P_Blit_Pass_Unrolled

        ; Pass 3, Bank 0
        mov     #$A0, P_INDR_SFR
        call    _P_Blit_Pass_Unrolled

        ; Pass 4, Bank 0
        mov     #$B0, P_INDR_SFR
        call    _P_Blit_Pass_Unrolled

        ; Pass 5, Bank 0
        mov     #$C0, P_INDR_SFR
        call    _P_Blit_Pass_Unrolled

        ; Pass 6, Bank 0
        mov     #$D0, P_INDR_SFR
        call    _P_Blit_Pass_Unrolled

        ; Pass 7, Bank 0
        mov     #$E0, P_INDR_SFR
        call    _P_Blit_Pass_Unrolled

        ; Pass 8, Bank 0
        mov     #$F0, P_INDR_SFR
        call    _P_Blit_Pass_Unrolled

        ; Next Bank
        mov     #1, xbnk

        ; Pass 9, Bank 1
        mov     #$80, P_INDR_SFR
        call    _P_Blit_Pass_Unrolled

        ; Pass 10, Bank 1
        mov     #$90, P_INDR_SFR
        call    _P_Blit_Pass_Unrolled

        ; Pass 11, Bank 1
        mov     #$A0, P_INDR_SFR
        call    _P_Blit_Pass_Unrolled

        ; Pass 12, Bank 1
        mov     #$B0, P_INDR_SFR
        call    _P_Blit_Pass_Unrolled

        ; Pass 13, Bank 1
        mov     #$C0, P_INDR_SFR
        call    _P_Blit_Pass_Unrolled

        ; Pass 14, Bank 1
        mov     #$D0, P_INDR_SFR
        call    _P_Blit_Pass_Unrolled

        ; Pass 15, Bank 1
        mov     #$E0, P_INDR_SFR
        call    _P_Blit_Pass_Unrolled

        ; Pass 16, Bank 1
        mov     #$F0, P_INDR_SFR
        call    _P_Blit_Pass_Unrolled

        ; Done
        set1    ocr, 5
        ret

;------------------------------------------------------------------------------
; Internal Library Functions
;------------------------------------------------------------------------------
_P_Draw_Background:
        ;----------------------------------------------------------------------
        ; Draw a background image to WRAM
        ;----------------------------------------------------------------------
        ; Accepts:      TRL, TRH - Background image address
        ; Destroys:     ACC, C, TRL, TRH, VRMAD1, VRMAD2, VSEL, PSW
        ;
        ; Copies a bitmap 48x32 pixels in size. Uses standard Perspective image
        ; format, but relies on macro to skip size bits. Controlled by
        ; P_Draw_Background Macro.
        ;----------------------------------------------------------------------
        clr1    ocr, 5
        ; Prepare the frame buffer address
        mov     #P_WRAM_BANK, vrmad2
        mov     #P_WRAM_ADDR, vrmad1
        mov     #%00010000, vsel

        mov     #0, c
.loop
        ld      c
        ldc
        st      vtrbf
        inc     c
        ld      c
        ldc
        st      vtrbf
        inc     c
        ld      c
        ldc
        st      vtrbf
        inc     c
        ld      c
        ldc
        st      vtrbf
        inc     c
        ld      c
        ldc
        st      vtrbf
        inc     c
        ld      c
        ldc
        st      vtrbf
        inc     c
        ld      c
        sub     #192
        bnz     .loop
        set1    ocr, 5
        ret

_P_Mask_Fill_Screen:
        ;----------------------------------------------------------------------
        ; Mask the screen with a value
        ;----------------------------------------------------------------------
        ; Accepts:      ACC - Fill value
        ; Destroys:     ACC, B, C, VRMAD1, VRMAD2, VSEL
        ;
        ; Masks the screen with the value of ACC. Controlled by the macro
        ; P_Mask_Fill_Screen and various alias macros.
        ;----------------------------------------------------------------------
        clr1    ocr, 5
        ; Prepare the frame buffer address
        mov     #P_WRAM_BANK, vrmad2
        mov     #P_WRAM_ADDR, vrmad1
        mov     #%00000000, vsel

        mov     #32, c
        st      b
.loop
        ld      vtrbf
        or      b
        st      vtrbf
        inc     vrmad1
        ld      vtrbf
        or      b
        st      vtrbf
        inc     vrmad1
        ld      vtrbf
        or      b
        st      vtrbf
        inc     vrmad1
        ld      vtrbf
        or      b
        st      vtrbf
        inc     vrmad1
        ld      vtrbf
        or      b
        st      vtrbf
        inc     vrmad1
        ld      vtrbf
        or      b
        st      vtrbf
        inc     vrmad1
        dbnz    c, .loop
        set1    ocr, 5
        ret

_P_Fill_Screen:
        ;----------------------------------------------------------------------
        ; Fill the screen with a value
        ;----------------------------------------------------------------------
        ; Accepts:      ACC - Fill value
        ; Destroys:     ACC, C, VRMAD1, VRMAD2, VSEL
        ;
        ; Fills the screen with the value of ACC. Controlled by the macro
        ; P_Fill_Screen and various alias macros.
        ;----------------------------------------------------------------------
        ; Prepare the frame buffer address
        clr1    ocr, 5
        mov     #P_WRAM_BANK, vrmad2
        mov     #P_WRAM_ADDR, vrmad1
        mov     #%00010000, vsel

        mov     #8, c
.loop
        st      vtrbf
        st      vtrbf
        st      vtrbf
        st      vtrbf
        st      vtrbf
        st      vtrbf
        st      vtrbf
        st      vtrbf
        st      vtrbf
        st      vtrbf
        st      vtrbf
        st      vtrbf
        st      vtrbf
        st      vtrbf
        st      vtrbf
        st      vtrbf
        st      vtrbf
        st      vtrbf
        st      vtrbf
        st      vtrbf
        st      vtrbf
        st      vtrbf
        st      vtrbf
        st      vtrbf
        dbnz    c, .loop
        set1    ocr, 5
        ret

_P_Blit_Pass_Unrolled:
        ; This is much faster
        ld      vtrbf
        st      @R2
        inc     P_INDR_SFR
        ld      vtrbf
        st      @R2
        inc     P_INDR_SFR
        ld      vtrbf
        st      @R2
        inc     P_INDR_SFR
        ld      vtrbf
        st      @R2
        inc     P_INDR_SFR
        ld      vtrbf
        st      @R2
        inc     P_INDR_SFR
        ld      vtrbf
        st      @R2
        inc     P_INDR_SFR
        ld      vtrbf
        st      @R2
        inc     P_INDR_SFR
        ld      vtrbf
        st      @R2
        inc     P_INDR_SFR
        ld      vtrbf
        st      @R2
        inc     P_INDR_SFR
        ld      vtrbf
        st      @R2
        inc     P_INDR_SFR
        ld      vtrbf
        st      @R2
        inc     P_INDR_SFR
        ld      vtrbf
        st      @R2
        ret

_P_Draw_Sprite_No_Mask:
        ;----------------------------------------------------------------------
        ; Draw a sprite onto the screen without a mask
        ;----------------------------------------------------------------------
        clr1    ocr, 5
        ; Prepare what we can of the frame buffer address
        mov     #P_WRAM_BANK, vrmad2
        mov     #%00010000, vsel

        ; Initialise used RAM
        mov     #0, p_spr_flag
        mov     #P_PSW_IRBKX, psw

        ; Store the sprite x size in bytes+1 and y size
        ; eg, 10 = 1r2
        ; Store the remainder into the extra byte
        xor     acc
        ldc
        and     #%00000111
        st      p_spr_size_x_remainder

        xor     acc
        ldc
        and     #%11111000
        ror
        ror
        ror
        ; If the remainder is not zero, add one FIXME: IM UGLY
        st      p_spr_size_x
        ld      p_spr_size_x_remainder
        bz      .skip_add_1
        inc     p_spr_size_x
.skip_add_1

        mov     #1, acc
        ldc
        st      p_spr_size_y

        ; Store sprite x offset
        ld      p_spr_x
        and     #%00000111
        st      p_spr_offset_x

        ; Use the offset and remainder to calculate if there is an extra byte
        ld      p_spr_size_x_remainder
        add     p_spr_offset_x
        and     #%11111000
        ror
        ror
        ror
        st      p_spr_size_x_remainder

        ; p_spr_y * 6 + P_WRAM_ADDR + p_spr_x / 8
        ; p_spr_y * 6
        ld      p_spr_y
        and     #%00111111
        rol
        st      c
        rol
        add     c
        st      c

        ; p_spr_x / 8
        ld      p_spr_x
        and     #%11111000
        ror
        ror
        ror
        st      p_spr_x
        add     c

        ; + P_WRAM_ADDR
        add     #P_WRAM_ADDR
        ; ---
        ; acc now contains correct wram address to start drawing sprite
        ; ---
        ; Prepare the frame buffer address
        st      vrmad1

        ; ---
        ; For each line:
        ; ---
        ld      p_spr_size_y
        st      c
        mov     #2, p_spr_counter
.next_line
        call    _P_Sprite_Buffer_Common
        ; RORC across buffer
        ld      p_spr_offset_x
        bz      .skip_rorc
        st      b
.rorc_buffer
        clr1    psw, 7
        mov     #P_BYTE_BUFFER_LOC, P_INDR_MEM
        ld      @R0
        rorc
        st      @R0
        inc     P_INDR_MEM
        ld      @R0
        rorc
        st      @R0
        inc     P_INDR_MEM
        ld      @R0
        rorc
        st      @R0
        inc     P_INDR_MEM
        ld      @R0
        rorc
        st      @R0
        inc     P_INDR_MEM
        ld      @R0
        rorc
        st      @R0
        inc     P_INDR_MEM
        ld      @R0
        rorc
        st      @R0
        dbnz    b, .rorc_buffer
.skip_rorc

        ; Write buffer to WRAM
        push    vrmad1
        mov     #P_BYTE_BUFFER_LOC, P_INDR_MEM
        mov     #0, b

.write_buffer
        ld      @R0
        st      vtrbf
        inc     P_INDR_MEM

        ; If we have reached the edge of the screen
        ld      b
        add     p_spr_x
        sub     #5
        bz      .write_buffer_complete

        ; If we have drawn all the blocks of this line
        ld      p_spr_size_x
        add     p_spr_size_x_remainder
        inc     b
        sub     b
        bnz     .write_buffer
.write_buffer_complete
        pop     vrmad1
        ; ---
        ; Line write complete
        ; ---

        ; If there are no more lines, finish
        dec     c
        ld      c
        bz      .sprite_drawing_complete

        ; If we are at the bottom of the screen, finish
        mov     #32, acc
        sub     p_spr_y
        sub     p_spr_size_y
        add     c
        bz      .sprite_drawing_complete

        ; Else, prepare next line
        ld      vrmad1
        add     #6
        st      vrmad1
        jmp     .next_line
.sprite_drawing_complete
        set1    ocr, 5
        ret

_P_Sprite_Buffer_Common:
        ; Clear byte buffer so as to not draw artifacts
        mov     #P_BYTE_BUFFER_LOC, P_INDR_MEM
        mov     #0, @R0
        inc     P_INDR_MEM
        mov     #0, @R0
        inc     P_INDR_MEM
        mov     #0, @R0
        inc     P_INDR_MEM
        mov     #0, @R0
        inc     P_INDR_MEM
        mov     #0, @R0
        inc     P_INDR_MEM
        mov     #0, @R0

        ; Fill byte buffer with p_spr_size_x bytes
        mov     #P_BYTE_BUFFER_LOC, P_INDR_MEM
        ld      p_spr_size_x

        st      b
.fill_buffer
        ld      p_spr_counter
        inc     p_spr_counter
        ldc
        st      @R0
        inc     P_INDR_MEM
        dbnz    b, .fill_buffer
        ret

_P_Draw_Sprite_Mask:
        ;----------------------------------------------------------------------
        ; Draw a sprite onto the screen with a mask
        ;----------------------------------------------------------------------
        clr1    ocr, 5
        ; Prepare what we can of the frame buffer address
        mov     #P_WRAM_BANK, vrmad2
        mov     #%00010000, vsel
        ; Initialise used RAM
        mov     #0, p_spr_flag
        mov     #P_PSW_IRBKX, psw

        ; Store the sprite x size in bytes+1 and y size
        ; eg, 10 = 1r2
        ; Store the remainder into the extra byte
        xor     acc
        ldc
        and     #%00000111
        st      p_spr_size_x_remainder

        xor     acc
        ldc
        and     #%11111000
        ror
        ror
        ror
        ; If the remainder is not zero, add one
        st      p_spr_size_x
        ld      p_spr_size_x_remainder
        bz      .skip_add_1
        inc     p_spr_size_x
.skip_add_1

        mov     #1, acc
        ldc
        st      p_spr_size_y

        ; Store sprite x offset
        ld      p_spr_x
        and     #%00000111
        st      p_spr_offset_x

        ; Use the offset and remainder to calculate if there is an extra byte
        ld      p_spr_size_x_remainder
        add     p_spr_offset_x
        and     #%11111000
        ror
        ror
        ror
        st      p_spr_size_x_remainder

        ; p_spr_y * 6 + P_WRAM_ADDR + p_spr_x / 8
        ; p_spr_y * 6
        ld      p_spr_y
        and     #%00111111
        rol
        st      c
        rol
        add     c
        st      c

        ; p_spr_x / 8
        ld      p_spr_x
        and     #%11111000
        ror
        ror
        ror
        st      p_spr_x
        add     c

        ; + P_WRAM_ADDR
        add     #P_WRAM_ADDR
        ; ---
        ; acc now contains correct wram address to start drawing sprite
        ; ---

        ; Prepare the frame buffer address
        st      vrmad1

        ; PRESERVE IMPORTANT INFO
        push    vrmad1

        ; ----------------
        ; DRAW SPRITE MASK
        ; ----------------

        ; ---
        ; For each line:
        ; ---
        ld      p_spr_size_y
        st      c
        mov     #2, p_spr_counter
.next_line
        mov     #P_BYTE_BUFFER_LOC, P_INDR_MEM
        mov     #$FF, @R0
        inc     P_INDR_MEM
        mov     #$FF, @R0
        inc     P_INDR_MEM
        mov     #$FF, @R0
        inc     P_INDR_MEM
        mov     #$FF, @R0
        inc     P_INDR_MEM
        mov     #$FF, @R0
        inc     P_INDR_MEM
        mov     #$FF, @R0

        ; Fill byte buffer with p_spr_size_x bytes
        mov     #P_BYTE_BUFFER_LOC, P_INDR_MEM
        ld      p_spr_size_x

        st      b
.fill_buffer
        ld      p_spr_counter
        inc     p_spr_counter
        ldc
        st      @R0
        inc     P_INDR_MEM
        dbnz    b, .fill_buffer
        ; RORC across buffer
        ld      p_spr_offset_x
        bz      .skip_rorc
        st      b
.rorc_buffer
        set1    psw, 7
        mov     #P_BYTE_BUFFER_LOC, P_INDR_MEM
        ld      @R0
        rorc
        st      @R0
        inc     P_INDR_MEM
        ld      @R0
        rorc
        st      @R0
        inc     P_INDR_MEM
        ld      @R0
        rorc
        st      @R0
        inc     P_INDR_MEM
        ld      @R0
        rorc
        st      @R0
        inc     P_INDR_MEM
        ld      @R0
        rorc
        st      @R0
        inc     P_INDR_MEM
        ld      @R0
        rorc
        st      @R0
        dbnz    b, .rorc_buffer
.skip_rorc

        ; Write buffer to WRAM
        push    vrmad1
        mov     #P_BYTE_BUFFER_LOC, P_INDR_MEM
        mov     #0, b

.write_buffer
        mov     #%00000000, vsel
        ld      vtrbf
        and     @R0
        mov     #%00010000, vsel
        st      vtrbf
        inc     P_INDR_MEM

        ; If we have reached the edge of the screen
        ld      b
        add     p_spr_x
        sub     #5
        bz      .write_buffer_complete_mask

        ; If we have drawn all the blocks of this line
        ld      p_spr_size_x
        add     p_spr_size_x_remainder
        inc     b
        sub     b
        bnz     .write_buffer
.write_buffer_complete_mask
        pop     vrmad1
        ; ---
        ; Line write complete
        ; ---

        ; If there are no more lines, finish
        dec     c
        ld      c
        bz      .mask_drawing_complete

        ; If we are at the bottom of the screen, finish
        mov     #32, acc
        sub     p_spr_y
        sub     p_spr_size_y
        add     c
        bz      .mask_drawing_complete

        ; Else, prepare next line
        ld      vrmad1
        add     #6
        st      vrmad1
        jmp     .next_line
.mask_drawing_complete

        ; RESTORE IMPORTANT INFO
        pop     vrmad1

        ; ---------------------
        ; MASK SPRITE TO SCREEN
        ; ---------------------
        ; First, calculate the new location in ROM to get sprite data
        ld      trl
        add     p_spr_counter
        st      trl
        bn      psw, 7, .no_overflow
        inc     trh
.no_overflow

        ; ---
        ; For each line:
        ; ---
        ld      p_spr_size_y
        st      c
        mov     #0, p_spr_counter
.next_line_sprite
        call    _P_Sprite_Buffer_Common
        ; RORC across buffer
        ld      p_spr_offset_x
        bz      .skip_rorc_sprite
        st      b
.rorc_buffer_sprite
        clr1    psw, 7
        mov     #P_BYTE_BUFFER_LOC, P_INDR_MEM
        ld      @R0
        rorc
        st      @R0
        inc     P_INDR_MEM
        ld      @R0
        rorc
        st      @R0
        inc     P_INDR_MEM
        ld      @R0
        rorc
        st      @R0
        inc     P_INDR_MEM
        ld      @R0
        rorc
        st      @R0
        inc     P_INDR_MEM
        ld      @R0
        rorc
        st      @R0
        inc     P_INDR_MEM
        ld      @R0
        rorc
        st      @R0
        dbnz    b, .rorc_buffer_sprite
.skip_rorc_sprite

        ; Write buffer to WRAM
        push    vrmad1
        mov     #P_BYTE_BUFFER_LOC, P_INDR_MEM
        mov     #0, b

.write_buffer_sprite
        mov     #%00000000, vsel
        ld      vtrbf
        or      @R0
        mov     #%00010000, vsel
        st      vtrbf
        inc     P_INDR_MEM

        ; If we have reached the edge of the screen
        ld      b
        add     p_spr_x
        sub     #5
        bz      .write_buffer_complete_sprite

        ; If we have drawn all the blocks of this line
        ld      p_spr_size_x
        add     p_spr_size_x_remainder
        inc     b
        sub     b
        bnz     .write_buffer_sprite
.write_buffer_complete_sprite
        pop     vrmad1
        ; ---
        ; Line write complete
        ; ---

        ; If there are no more lines, finish
        dec     c
        ld      c
        bz      .drawing_complete

        ; If we are at the bottom of the screen, finish
        mov     #32, acc
        sub     p_spr_y
        sub     p_spr_size_y
        add     c
        bz      .drawing_complete

        ; Else, prepare next line
        ld      vrmad1
        add     #6
        st      vrmad1
        jmp     .next_line_sprite
.drawing_complete
        set1    ocr, 5
        ret

;------------------------------------------------------------------------------
; Internal Library Macros
;------------------------------------------------------------------------------
