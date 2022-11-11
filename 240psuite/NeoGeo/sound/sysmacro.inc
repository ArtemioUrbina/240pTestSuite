; 240p Test Suite Sound Driver
; Based on freem ADPCM example at: http://ajworld.net/neogeodev/
; Modifications made by Artemio Urbina for the 240p Test Suite and MDFourier
;==============================================================================;
; SetBank
; A macro for setting one of the Z80 banks.

; Params:
; \1			Bank number
; \2			Bank slot (8,9,0xA,0xB)

	macro	SetBank
	ld		a,#\1
	in		a,(\2)
	endm

;==============================================================================;
; FM Instrument Definition Macros
;==============================================================================;
; fmInst_DetuneMult
; A macro for writing a combined detune/multiple value.

; Params:
; \1			Detune (0-7)
; \2			Multiple (0-15)

	macro	fmInst_DetuneMult
	byte	(\1&7)<<4|\2&0xF
	endm

;==============================================================================;
; fmInst_TotalLevel
; A macro for writing a FM instrument's Total Level (volume).

; Params:
; \1			Total Level ($00-$7F, 0-127)

	macro	fmInst_TotalLevel
	byte	\1&0x7F
	endm

;==============================================================================;
; fmInst_KeyScaleAtkRate
; A macro for writing a FM instrument's Key Scale and Attack Rate.

; Params:
; \1			Key Scale (0-3)
; \2			Attack Rate (0-31)

	macro	fmInst_KeyScaleAtkRate
	byte	(\1&3)<<6|\2&31
	endm

;==============================================================================;
; fmInst_AMOnDecayRate
; A macro for writing a FM instrument's AM On and Decay Rate.

; Params:
; \1			AM On (0 or 1)
; \2			Decay Rate (0-31)

	macro	fmInst_AMOnDecayRate
	byte		(\1&1)<<7|\2&0x1F
	endm

;==============================================================================;
; fmInst_SustainRate
; A macro for writing a FM instrument's Sustain Rate.

; Param:
; \1			Sustain Rate (0-31)

	macro	fmInst_SustainRate
	byte	\1&0x1F
	endm

;==============================================================================;
; fmInst_SustLevelRelRate
; A macro for writing a FM instrument's Sustain Level and Release Rate.

; Params:
; \1			Sustain Level (0-15)
; \2			Release Rate (0-15)

	macro	fmInst_SustLevelRelRate
	byte	(\1&0xF)<<4|\2&0xF
	endm

;==============================================================================;
; fmInst_SSGEG
; A macro for writing a FM instrument's SSG-EG value

; Param:
; \1			SSG-EG value (0-15)

	macro	fmInst_SSGEG
	byte	\1&0xF
	endm

;==============================================================================;
; fmInst_FeedbackAlgo
; A macro for writing a FM instrument's Feedback and Algorithm values.

; Params:
; \1			Feedback (0-7)
; \2			Algorithm (0-7)

	macro	fmInst_FeedbackAlgo
	byte	(\1&7)<<3|\2&7
	endm

;==============================================================================;
; fmInst_AMSPMS
; A macro for writing a FM instrument's AM Sense and PM Sense.
; (Always assumes left and right output are on.)

; Params:
; \1			AM Sense (0-3)
; \2			PM Sense (0-7)

	macro	fmInst_AMSPMS
	byte	0xC0|(\1&3)<<5|(\2&7)
	endm

;==============================================================================;
; fmInst_AMSPMSLR
; A macro for writing a FM instrument's AM Sense and PM Sense values, and
; default left/right output.

; Params:
; \1			AM Sense (0-3)
; \2			PM Sense (0-7)
; \3			Left/Right output (0=none,1=left,2=right,3=both)

	macro	fmInst_AMSPMSLR
	byte	(\3&0x3)<<6|(\1&3)<<5|(\2&7)
	endm
