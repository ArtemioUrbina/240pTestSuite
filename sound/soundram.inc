; Neo-Geo freemlib Sound Driver RAM (edited for the 240p test Suite)
;==============================================================================;
; Usable RAM exists from 0xF800 to 0xFFFF.
	org $F800
;==============================================================================;
; <Sound Driver Section. Currently 27 bytes used
;==============================================================================;
dummyByte			ds 1		; dummy byte used for RAM clearing, etc.
;------------------------------------------------------------------------------;
; [Commands]
curCommand			ds 1		; Current command to handle

;==============================================================================;
; <YM2610 Section>
;==============================================================================;
; [System variables]
intStatus0			ds 1		; Status 0 (Timer flags and Busy)
intStatus1			ds 1		; Status 1 (ADPCM A and B status)

;==============================================================================;
; [Timer variables]
intTimerControl		ds 1		; ($27) Timer Control and Mode
intTimerA_Hi		ds 1		; ($24) Timer A value, upper 8 bits
intTimerA_Lo		ds 1		; ($25) Timer A value, lower 2 bits
intTimerB			ds 1		; ($26) Timer B value

;------------------------------------------------------------------------------;
; [SSG Data]

SSG_FreqFine		ds 1		; Previous command handled
SSG_FreqCoarse		ds 1		; Previous command handled

;==============================================================================;
; [ADPCM-A variables ](Ports 6/7)
intPCMA_Dump		ds 1		; ($00) Dump (Key On/Off) and Channel toggle
intPCMA_MasterVol	ds 1		; ($01) Master Volume

;------------------------------------------------------------------------------;
; * ADPCM-A channels (only ch.1 is used in this demo)
intPCMA1_Volume		ds 1		; ($08) Channel volume
intPCMA1_StartL		ds 1		; ($10) Sample Start Address/256 (LSB)
intPCMA1_StartH		ds 1		; ($18) Sample Start Address/256 (MSB)
intPCMA1_EndL		ds 1		; ($20) Sample End Address/256 (xSB)
intPCMA1_EndH		ds 1		; ($28) Sample End Address/256 (xSB)

;==============================================================================;
; [ADPCM-B variables] (ports 4/5) (Only available on cart systems.)
intPCM_ChanFlags	ds 1		; ($1C) *ADPCM-A and B flags control*

;------------------------------------------------------------------------------;
; * ADPCM-B
intPCMB_Status		ds 1		; ($10) Start, Repeat, Reset
intPCMB_LeftRight	ds 1		; ($11) Left and Right channel output
intPCMB_StartLo		ds 1		; ($12) Sample Start Address/256 (LSB)
intPCMB_StartHi		ds 1		; ($13) Sample Start Address/256 (MSB)
intPCMB_EndLo		ds 1		; ($14) Sample End Address/256 (LSB)
intPCMB_EndHi		ds 1		; ($15) Sample End Address/256 (MSB)
intPCMB_SampRateL	ds 1		; ($19) Delta-N Sampling Rate (L)
intPCMB_SampRateH	ds 1		; ($1A) Delta-N Sampling Rate (H)
intPCMB_Volume		ds 1		; ($1B) Channel volume

