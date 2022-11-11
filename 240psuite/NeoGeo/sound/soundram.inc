; Neo-Geo freemlib Sound Driver RAM (edited for the 240p test Suite)
;==============================================================================;
; Usable RAM exists from 0xF800 to 0xFFFF.
	org $F800
;==============================================================================;
; <Sound Driver Section>
;==============================================================================;

; [Commands]
curCommand			ds 1		; Current command to handle

;==============================================================================;
; <YM2610 Section>
;==============================================================================;
; [System variables]
intStatus0			ds 1		; Status 0 (Timer flags and Busy)
intStatus1			ds 1		; Status 1 (ADPCM A and B status)

;------------------------------------------------------------------------------;
; [SSG Data]
SSG_FreqCoarse		ds 1		; Coarse SSG frequency for ramp
SSG_FreqFine		ds 1		; Fine SSG frequency for ramp
SSG_nextChannel		ds 1		; Next available SSG channel
SSG_currChannel		ds 1		; Currently used SSG channel

;==============================================================================;
; [ADPCM-A variables]
PCMnextChannel		ds 1		; Next available channel 0-5
PCMcurrChannel		ds 1		; Currently used channel 0-5

;==============================================================================;
; [ADPCM-B variables]
intPCMB_currSample	ds 1		; Current sample index
intPCMB_LeftRight	ds 1		; Left and Right channel output
intPCMB_SampRateL	ds 1		; Delta-N Sampling Rate (L)
intPCMB_SampRateH	ds 1		; Delta-N Sampling Rate (H)
intPCMB_Volume		ds 1		; Channel volume
intPCMB_Loop		ds 1		; loop B

usedRAM:

