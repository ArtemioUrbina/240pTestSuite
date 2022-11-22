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

;------------------------------------------------------------------------------;
; [FM Data]
FM_Octave			ds 1		; Octave for FM
FM_Note				ds 1		; Note for FM
FM_ChannCycle		ds 1		; Toggle for FM channels

;------------------------------------------------------------------------------;
; [SSG Data]
SSG_FreqCoarse		ds 1		; Coarse SSG frequency for ramp
SSG_FreqFine		ds 1		; Fine SSG frequency for ramp
SSG_NoiseTune		ds 1		; Noise SSG frequency for ramp

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

