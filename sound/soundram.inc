; Neo-Geo freemlib Sound Driver RAM (edited for the 240p test Suite)
;==============================================================================;
; Usable RAM exists from 0xF800 to 0xFFFF.
	org $F800
;==============================================================================;
; <Sound Driver Section
;==============================================================================;
dummyByte			ds 1		; dummy byte used for RAM clearing, etc.
;------------------------------------------------------------------------------;
; [Commands]
curCommand			ds 1		; Current command to handle

;==============================================================================;
; <YM2610 Section>
;==============================================================================;
; [System variables]	00
intStatus0			ds 1		; Status 0 (Timer flags and Busy)
intStatus1			ds 1		; Status 1 (ADPCM A and B status)

;==============================================================================;
; [Timer variables]		02
intTimerControl		ds 1		; Timer Control and Mode
intTimerA_Hi		ds 1		; Timer A value, upper 8 bits
intTimerA_Lo		ds 1		; Timer A value, lower 2 bits
intTimerB			ds 1		; Timer B value

;------------------------------------------------------------------------------;
; [SSG Data]			06
SSG_FreqFine		ds 1		; Previous command handled
SSG_FreqCoarse		ds 1		; Previous command handled

;==============================================================================;
; [ADPCM-A variables ]	08
intPCMA_Dump		ds 1		; Dump (Key On/Off) and Channel toggle
intPCMA_MasterVol	ds 1		; Master Volume
PCMnextChannel		ds 1		; Next available channel 0-5
PCMcurrChannel		ds 1		; Currently used channel 0-5

;------------------------------------------------------------------------------;
; * ADPCM-A channels	0C
intPCMA1_Volume		ds 1		; Channel volume
intPCMA1_StartL		ds 1		; Sample Start Address/256 (LSB)
intPCMA1_StartH		ds 1		; Sample Start Address/256 (MSB)
intPCMA1_EndL		ds 1		; Sample End Address/256 (xSB)
intPCMA1_EndH		ds 1		; Sample End Address/256 (xSB)

;==============================================================================;
; [ADPCM-B variables]	11
intPCM_ChanFlags	ds 1		; *ADPCM-A and B flags control*

;------------------------------------------------------------------------------;
; * ADPCM-B				12
intPCMB_Status		ds 1		; Start, Repeat, Reset
intPCMB_LeftRight	ds 1		; Left and Right channel output
intPCMB_StartLo		ds 1		; Sample Start Address/256 (LSB)
intPCMB_StartHi		ds 1		; Sample Start Address/256 (MSB)
intPCMB_EndLo		ds 1		; Sample End Address/256 (LSB)
intPCMB_EndHi		ds 1		; Sample End Address/256 (MSB)
intPCMB_SampRateL	ds 1		; Delta-N Sampling Rate (L)
intPCMB_SampRateH	ds 1		; Delta-N Sampling Rate (H)
intPCMB_Volume		ds 1		; Channel volume
intPCMB_Loop		ds 1		; loop B

