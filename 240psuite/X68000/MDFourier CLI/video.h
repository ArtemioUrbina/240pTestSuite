/* 
 * 240p Test Suite
 * Copyright (C)2021 Artemio Urbina
 *
 * This file is part of the 240p Test Suite
 *
 * The 240p Test Suite is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The 240p Test Suite is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 
#include <stdint.h>
#include "types.h"

// Original Code by	lydux
// https://nfggames.com/forum2/index.php?topic=5115.0

#ifndef __X68K_VIDEO
#define __X68K_VIDEO

/* MFP address */
#define MFP_BASE  0xe88000

struct MFP
{
	u16 gpdr; 	/* General Purpose I/O line Data Register (used as interrupt sources on x68000) */
	u16 aer;	/* Active Edge Register (The signal edge the interrupt will be trigerred on) */
	u16 ddr;	/* Data Direction Register (only input on x68000) */
	u16 iera; 	/* Interrupt Enable Register A */
	u16 ierb; 	/* Interrupt Enable Register B */
	u16 ipra; 	/* Interrupt Pending Register A */
	u16 iprb; 	/* Interrupt Pending Register B */
	u16 isra; 	/* Interrupt In-Service Register A */
	u16 isrb; 	/* Interrupt In-Service Register B */
	u16 imra; 	/* Interrupt Mask Register A */
	u16 imrb; 	/* Interrupt Mask Register B */
	u16 vr;		/* Vector Register */
	u16 tacr;
	u16 tbcr;
	u16 tcdcr;
	u16 tadr;
	u16 tbdr;
	u16 tcdr;
	u16 tddr;
	u16 scr;
	u16 ucr;
	u16 rsr;
	u16 tsr;
	u16 udr;
};
#define mfp ((*(volatile struct MFP *)MFP_BASE))

/* Bits signification on registers GPDR, AER and DDR */
#define DR_ALARM    0
#define DR_EXPON    1
#define DR_POWSW    2
#define DR_OPMIRQ   3
#define DR_VDISP    4
#define DR_CRTC     6
#define DR_HSYNC    7

/* Bits signification on interrupt registers A (IERA, IPRA, ISRA and IMRA) */
#define IA_TIMERB   0
#define IA_TXERR    1
#define IA_TXEMPTY  2
#define IA_RXERR    3
#define IA_RXFULL   4
#define IA_TIMERA   5
#define IA_CRTC     6
#define IA_HSYNC    7

/* Bits signification on interrupt registers B (IERB, IPRB, ISRB and IMRB) */
#define IB_ALARM    0
#define IB_EXPON    1
#define IB_POWSW    2
#define IB_OPMIRQ   3
#define IB_TIMERD   4
#define IB_TIMERC   5
#define IB_VDISP    6
#define IB_GPIP5    7

/* MFP vectors (CPU user vectors) */
#define MFP_VEC_ALARM             64    /* RTC Alarm interrupt */
#define MFP_VEC_EXPON             65    /* Expansion card power off */
#define MFP_VEC_POWSW             66    /* Front power switch */
#define MFP_VEC_OPMIRQ            67    /* OPM interrupt */
#define MFP_VEC_TIMERD            68    /* MFP internal timer D */
#define MFP_VEC_TIMERC            69    /* MFP internal timer C */
#define MFP_VEC_VDISP             70    /* CRTC VDisp interrupt */
#define MFP_VEC_RTC_CLOCK         71    /* RTC clock */
#define MFP_VEC_TIMERB            72    /* MFP internal timer B */
#define MFP_VEC_USART_TX_ERROR    73    /* MFP internal USART transmitter error */
#define MFP_VEC_USART_TX_EMPTY    74    /* MFP internal USART transmitter buffer empty */
#define MFP_VEC_USART_RX_ERROR    75    /* MFP internal USART receiver error */
#define MFP_VEC_USART_RX_EMPTY    76    /* MFP internal USART receiver buffer empty */
#define MFP_VEC_TIMERA            77    /* MFP internal timer A */
#define MFP_VEC_CRTC              78    /* CRTC interrupt (?) */
#define MFP_VEC_HSYNC             79    /* CRTC VSync interrupt */

/* GCC and m68k specific. */
#define ISR  __attribute__((__interrupt_handler__))

/* CPU interrupts managment */
#define DISABLE_ALL_INTERRUPTS() asm("ori.w #0x0700, %sr")
#define ENABLE_ALL_INTERRUPTS() asm("andi.w #0xf8ff, %sr")

/* Registers bits access helper */
#define bset(reg,bit)   (reg |= (1<<bit))
#define bclr(reg,bit)   (reg &= ~(1<<bit))

#define GPIP_ALARM	  (1 << 0)
#define GPIP_EXPON	  (1 << 1)
#define GPIP_POWSW	  (1 << 2)
#define GPIP_OPMIRQ   (1 << 3)
#define GPIP_VDISP	  (1 << 4)
#define GPIP_CRTC	  (1 << 6)
#define GPIP_HSYNC	  (1 << 7)

extern int video_count;

#define CRTC_under_vdisp (mfp.gpdr & GPIP_VDISP)
#define CRTC_under_vblank !(mfp.gpdr & GPIP_VDISP)

static void inline wait_for_vblank	(void)
{
	/* CRTC under vdisp */
	while(CRTC_under_vdisp);
}

static void inline wait_for_vdisp  (void)
{
	/* CRTC under vblank */
	while(CRTC_under_vblank);
}

static void inline wait_vblank (void)
{
	// were we called during vblank? (most probably the same one due to our short cycles)
	if(CRTC_under_vblank)
		wait_for_vdisp();
	wait_for_vblank ();

	video_count++;
}

#endif
