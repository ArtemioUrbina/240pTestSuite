; =======================================================================================
;  Sega CD Boot loader by Luke Usher/SoullessSentinel (c) 2010
;  Used with permission. This code is not released under the GPL.
; =======================================================================================
align macro
	cnop 0,\1
	endm
; =======================================================================================
;  Sega CD Header (Based on Sonic CD's header)
; =======================================================================================
DiscType:	dc.b 'SEGADISCSYSTEM  '		; Disc Type (Must be SEGADISCSYSTEM)
VolumeName:	dc.b 'SEGACDGAME ',0		; Disc ID
VolumeSystem:	dc.w $100, $1			; System ID, Type
SystemName:	dc.b 'SEGACDGAME ',0		; System Name
SystemVersion:	dc.w 0,0			; System Version, Type
IP_Addr:	dc.l $800			; IP Start Address (Must be $800)
IP_Size:	dc.l $800			; IP End Address (Must be $800)
IP_Entry:	dc.l 0
IP_WorkRAM:	dc.l 0
SP_Addr:	dc.l $1000			; SP Start Address (Must be $1000)
SP_Size:	dc.l $7000			; SP End Address (Must be $7000)
SP_Entry:	dc.l 0
SP_WorkRAM:	dc.l 0
		align $100			; Pad to $100
		
; =======================================================================================
;  Game Header
; =======================================================================================	
HardwareType:	dc.b 'SEGA MEGA DRIVE '
Copyright:	dc.b '(C)ARTEMIO 2011 '
NativeName:	dc.b '240P TEST SUITE                                 '
OverseasName:	dc.b '240P TEST SUITE                                 '
DiscID:		dc.b 'GM 00-0000-00   '
IO:		dc.b 'J               '
		; Modem information, notes, and padding, left undefined as it is not used
		; Padded to $1F0 instead (Start of Region Code)
		align $1F0
Region:		dc.b 'E               '
; ========================================================================================
;  IP (Includes security sector)
; ========================================================================================	
		incbin "_boot\ip-jp.bin";
; =======================================================================================
;  Sub CPU Program (SP)
; =======================================================================================
		align	$1000
		incbin	"_boot\sp.bin"
; =======================================================================================
;  Padding, to $8000 (Size of boot area of iso)
; =======================================================================================	
		align	$8000
; =======================================================================================
;  FileSystem:
; =======================================================================================
		incbin	"filesystem.bin"
		align	$2000000
