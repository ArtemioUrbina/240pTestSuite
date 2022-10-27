; Neo-Geo freemlib Sound Driver Defines
;==============================================================================;
; [SSG address (port 4)]
SSG_FineA			equ 0			; bottom 8 bits of SSG Channel A Tone Period
SSG_CoarseA			equ 1			; top 4 bits of SSG Channel A Tone Period
SSG_FineB			equ 2			; bottom 8 bits of SSG Channel B Tone Period
SSG_CoarseB			equ 3			; top 4 bits of SSG Channel B Tone Period
SSG_FineC			equ 4			; bottom 8 bits of SSG Channel C Tone Period
SSG_CoarseC			equ 5			; top 4 bits of SSG Channel C Tone Period
SSG_Noise			equ 6			; SSG Noise Channel Period
SSG_Mixer			equ 7			; SSG Enable Flags
SSG_VolumeA			equ 8			; SSG Channel A Volume and Mode
SSG_VolumeB			equ 9			; SSG Channel B Volume and Mode
SSG_VolumeC			equ 0xA			; SSG Channel C Volume and Mode
SSG_EnvFine			equ 0xB			; Envelope Period Fine Tune
SSG_EnvCoarse		equ 0xC			; Envelope Period Coarse Tune
SSG_EnvShape		equ 0xD			; Envelope Shape Cycle

;==============================================================================;
; [ADPCM-A address (port 6)]
PCMA_Control		equ 0x00		; ADPCM-A ON
PCMA_MasterVol		equ 0x01		; Total Level
; -- Per-Channel Variables, starting values (Channel 1) only -- ;
PCMA_Chan1VolLR		equ 0x08		; Channel 1 Left/Right and Volume
PCMA_Start1Lo		equ 0x10		; Channel 1 Sample start address/256 (LSB)
PCMA_Start1Hi		equ 0x18		; Channel 1 Sample start address/256 (MSB)
PCMA_End1Lo			equ 0x20		; Channel 1 Sample end address/256 (LSB)
PCMA_End1Hi			equ 0x28		; Channel 1 Sample end address/256 (MSB)

;==============================================================================;
; [ADPCM-B address (port 4)] (Only available on cart systems.)
PCMB_Control		equ 0x10		; ADPCM-B Start/Repeat/Reset
PCMB_LeftRight		equ 0x11		; ADPCM-B Left/Right output
PCMB_StartLo		equ 0x12		; Sample start address/256 (LSB)
PCMB_StartHi		equ 0x13		; Sample start address/256 (MSB)
PCMB_EndLo			equ 0x14		; Sample end address/256 (LSB)
PCMB_EndHi			equ 0x15		; Sample end address/256 (MSB)
PCMB_SampRateLo		equ 0x19		; Delta-N Sampling rate (Lo)
PCMB_SampRateHi		equ 0x1A		; Delta-N Sampling rate (Hi)
PCMB_Volume			equ 0x1B		; ADPCM-B Total Level/Volume
PCMB_Flags			equ 0x1C		; ADPCM-A and B Flag control

;==============================================================================;
; [FM address ports]
; -- Shared FM (ports 4/5) --;
FM_LFOFreq			equ 0x22		; LFO Frequency Control
FM_TimerALo			equ 0x24		; Timer A lower 2 bits
FM_TimerAHi			equ 0x25		; Timer A upper 8 bits
FM_TimerB			equ 0x26		; Timer B
FM_TimerMode		equ 0x27		; Timer A/B Control and 2CH Mode
FM_KeyOnOff			equ 0x28		; Key On/Off (and Slot)

; -- Per-Channel FM --;
; Data values used with address 0x28:
FM_Chan1			equ 0x01		; Physical Channel 1/6; Real Channel 1/4
FM_Chan2			equ 0x02		; Physical Channel 2/6; Real Channel 2/4
FM_Chan3			equ 0x05		; Physical Channel 5/6; Real Channel 3/4
FM_Chan4			equ 0x06		; Physical Channel 6/6; Real Channel 4/4

; Data values used with addresses 0xB1/0xB2:
FM_Algo1			equ 0			; Four serial connection mode
FM_Algo2			equ 1			; Three double modulation serial connection mode
FM_Algo3			equ 2			; Double modulation mode 1
FM_Algo4			equ 3			; Double modulation mode 2
FM_Algo5			equ 4			; Two serial connection and two parallel modes
FM_Algo6			equ 5			; Common modulation 3 parallel mode
FM_Algo7			equ 6			; Two serial connection and two sine mode
FM_Algo8			equ 7			; Four parallel sine synthesis mode
;====================================;
; "FM1" is used for Channel 1 (ports 4/5) and 3 (ports 6/7).

; -- FM1 Channel Variables -- ;
FM1_FreqHi			equ 0xA5		; Block and Frequency upper 3 bits
FM1_FreqLo			equ 0xA1		; Frequency lower 8 bits
FM1_Freq2Hi			equ 0xAD		; 2CH*Block and 2CH*Frequency upper 3 bits
FM1_Freq2Lo			equ 0xA9		; 2CH * Frequency lower 8 bits
FM1_FeedbackAlgo	equ 0xB1		; Feedback and Algorithm
FM1_LeftRightAMPM	equ 0xB5		; Left/Right, AM Change, PM Change

; -- FM1 Operators -- ;
FM1_DetuneMult_Op1	equ 0x31		; DT/MULT	_DDDMMMM (D=detune, M=multiple)
FM1_DetuneMult_Op2	equ 0x35		; DT/MULT
FM1_DetuneMult_Op3	equ 0x39		; DT/MULT
FM1_DetuneMult_Op4	equ 0x3D		; DT/MULT
;---------------------------;
FM1_TotalLevel_Op1	equ 0x41		; TL		_VVVVVVV (V=total level/volume)
FM1_TotalLevel_Op2	equ 0x45		; TL
FM1_TotalLevel_Op3	equ 0x49		; TL
FM1_TotalLevel_Op4	equ 0x4D		; TL
;---------------------------;
FM1_KeyScalAtk_Op1	equ 0x51		; KS/AR		SS_RRRRR (S=key scale, R=attack rate)
FM1_KeyScalAtk_Op2	equ 0x55		; KS/AR
FM1_KeyScalAtk_Op3	equ 0x59		; KS/AR
FM1_KeyScalAtk_Op4	equ 0x5D		; KS/AR
;---------------------------;
FM1_DecayAMOn_Op1	equ 0x61		; AM/DR		A__DDDDD (A=AM on, D=decay rate)
FM1_DecayAMOn_Op2	equ 0x65		; AM/DR
FM1_DecayAMOn_Op3	equ 0x69		; AM/DR
FM1_DecayAMOn_Op4	equ 0x6D		; AM/DR
;---------------------------;
FM1_Sustain_Op1		equ 0x71		; SR		___SSSSS (S=sustain rate)
FM1_Sustain_Op2		equ 0x75		; SR
FM1_Sustain_Op3		equ 0x79		; SR
FM1_Sustain_Op4		equ 0x7D		; SR
;---------------------------;
FM1_SusLevRel_Op1	equ 0x81		; SL/RR		SSSSRRRR (S=sustain level, R=release rate)
FM1_SusLevRel_Op2	equ 0x85		; SL/RR
FM1_SusLevRel_Op3	equ 0x89		; SL/RR
FM1_SusLevRel_Op4	equ 0x8D		; SL/RR
;---------------------------;
FM1_SSGEG_Op1		equ 0x91		; SSG-EG	____CALH (C=Cont., A=Attack, L=Alternate, H=Hold)
FM1_SSGEG_Op2		equ 0x95
FM1_SSGEG_Op3		equ 0x99
FM1_SSGEG_Op4		equ 0x9D

;====================================;
; "FM2" is used for Channel 2 (ports 4/5) and 4 (ports 6/7).

; -- FM2 Channel Variables -- ;
FM2_FreqHi			equ 0xA6		; Block and Frequency upper 3 bits
FM2_FreqLo			equ 0xA2		; Frequency lower 8 bits
FM2_Freq2Hi			equ 0xAE		; 2CH*Block and 2CH*Frequency upper 3 bits
FM2_Freq2Lo			equ 0xAA		; 2CH * Frequency lower 8 bits
FM2_FeedbackAlgo	equ 0xB2		; Feedback and Algorithm
FM2_LeftRightAMPM	equ 0xB6		; Left/Right, AM Change, PM Change

; -- FM2 Operators -- ;
FM2_DetuneMult_Op1	equ 0x32		; DT/MULT
FM2_DetuneMult_Op2	equ 0x36		; DT/MULT
FM2_DetuneMult_Op3	equ 0x3A		; DT/MULT
FM2_DetuneMult_Op4	equ 0x3E		; DT/MULT
;---------------------------;
FM2_TotalLevel_Op1	equ 0x42		; TL
FM2_TotalLevel_Op2	equ 0x46		; TL
FM2_TotalLevel_Op3	equ 0x4A		; TL
FM2_TotalLevel_Op4	equ 0x4E		; TL
;---------------------------;
FM2_KeyScalAtk_Op1	equ 0x52		; KS/AR
FM2_KeyScalAtk_Op2	equ 0x56		; KS/AR
FM2_KeyScalAtk_Op3	equ 0x5A		; KS/AR
FM2_KeyScalAtk_Op4	equ 0x5E		; KS/AR
;---------------------------;
FM2_DecayAMOn_Op1	equ 0x62		; AM/DR
FM2_DecayAMOn_Op2	equ 0x66		; AM/DR
FM2_DecayAMOn_Op3	equ 0x6A		; AM/DR
FM2_DecayAMOn_Op4	equ 0x6E		; AM/DR
;---------------------------;
FM2_Sustain_Op1		equ 0x72		; SR
FM2_Sustain_Op2		equ 0x76		; SR
FM2_Sustain_Op3		equ 0x7A		; SR
FM2_Sustain_Op4		equ 0x7E		; SR
;---------------------------;
FM2_SusLevRel_Op1	equ 0x82		; SL/RR
FM2_SusLevRel_Op2	equ 0x86		; SL/RR
FM2_SusLevRel_Op3	equ 0x8A		; SL/RR
FM2_SusLevRel_Op4	equ 0x8E		; SL/RR
;---------------------------;
FM2_SSGEG_Op1		equ 0x92		; SSG-EG	____CALH (C=Cont., A=Attack, L=Alternate, H=Hold)
FM2_SSGEG_Op2		equ 0x96
FM2_SSGEG_Op3		equ 0x9A
FM2_SSGEG_Op4		equ 0x9E

;==============================================================================;
; YM2610 ports
YM_Status0			equ 4			; YM2610 Status 0 (read from port 4)
YM_Status1			equ 6			; YM2610 Status 1 (read from port 6)
