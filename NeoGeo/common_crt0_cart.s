	* ++====================================================================++
	* || common_crt0_cart.s - C Run Time Startup Code for Neo Geo Cartridge	||
	* ++--------------------------------------------------------------------++
	* || $Id: common_crt0_cart.s,v 1.5 2001/07/13 14:46:31 fma Exp $		||
	* ++--------------------------------------------------------------------++
	* || This is the startup code needed by programs compiled with GCC		||
	* ++--------------------------------------------------------------------||
	* || BGM: Guitar Vader - S.P.Y.											||
	* ++====================================================================++

_ZERO_DIVIDE	=	0x00c00426
_CHK_CMD	=	0x00c00426
_TRAPV_CMD	=	0x00c00426
_NPC_1010	=	0x00c00426
_NPC_1111	=	0x00c00426
_IRQ4	=	0x00c00426
_IRQ5	=	0x00c00426
_IRQ6	=	0x00c00426
_IRQ7	=	0x00c00426

********************** Exported Symbols **********************
	.globl	_start
	.globl	atexit

********************** Imported Symbols **********************
	.globl	__do_global_ctors
	.globl	__do_global_dtors
	.globl	main
	.globl	memset
	.globl	__bss_start	
	.globl	_end

********************** Program Start *************************

** NOTE: Cartridge systems have swapped IRQ1 and IRQ2

	.org	0x0000

	.long	0x0010f300	|;reset stack ptr
	.long	0x00c00402	/*;reset ptr*/
	.long	0x00c00408	/*;bus error*/
	.long	0x00c0040e	/*;address error*/

	.long	0x00c00414	/*;illegal instruction*/
	.long	_ZERO_DIVIDE	/*;division by 0*/
	.long	_CHK_CMD	/*;CHK command*/
	.long	_TRAPV_CMD	/*;TRAPV command*/

	.long	0x00c0041a	/*;illegal privilege*/
	.long	0x00c00420	/*;trace exception handling*/
	.long	_NPC_1010	/*;no package command (1010)*/
	.long	_NPC_1111	/*;no package command (1111)*/

	.long	0x00c00426, 0x00c00426, 0x00c00426	/*;unused*/
	.long	0x00c0042c	/*;uninitialized interrupt*/

	.long	0x00c00426,	0x00c00426,	0x00c00426,	0x00c00426	/*;unused*/
	.long	0x00c00426,	0x00c00426,	0x00c00426,	0x00c00426	/*;unused*/
	.long	0x00c00432	/*;virtual interrupt*/
	|; 0x64
	.long	_IRQ2, _IRQ1, _IRQ3, _IRQ4, _IRQ5, _IRQ6, _IRQ7	/*;4-7 unused*/

__security_code:
	.long	0x76004a6d, 0x0a146600, 0x003c206d, 0x0a043e2d
	.long	0x0a0813c0, 0x00300001, 0x32100c01, 0x00ff671a
	.long	0x30280002, 0xb02d0ace, 0x66103028, 0x0004b02d
	.long	0x0acf6606, 0xb22d0ad0, 0x67085088, 0x51cfffd4
	.long	0x36074e75, 0x206d0a04, 0x3e2d0a08, 0x3210e049
	.long	0x0c0100ff, 0x671a3010, 0xb02d0ace, 0x66123028
	.long	0x0002e048, 0xb02d0acf, 0x6606b22d, 0x0ad06708
	.long	0x588851cf, 0xffd83607
	.word	0x4e75

	.org	0x100
	.ascii	"NEO-GEO\0"
	.word	_NGH
	.long	_PROGRAM_SIZE
	.long	_WRK_BCKP_AREA
	.word	_WRK_BCKP_AREA_SIZE
	
	.byte	_EYE_CATCHER
	.byte	_EYE_CATCHER_TILES
	
	.long	JPconfig
	.long	NAconfig
	.long	EUconfig
	
	jmp	_ENTRY_USER
	jmp	_ENTRY_PLAYER_START
	jmp	_ENTRY_DEMO_END
	jmp	_ENTRY_COIN_SOUND

_irq3_handler:
	move.w	#1, 0x3C000C
_dummy_exc_handler:
	rte
atexit:	|;Dummy atexit (does nothing for now)
	moveq	#0, d0
_dummy_config_handler:
	rts

_irq1_handler:	|;Standard IRQ1 handler
	move.w	#2, 0x3C000C
	rte

	.org	0x182
	.long	__security_code

_dummyTIdata:
	.word	0x0000

|;* Entry point of our program
_start:
	|;* Setup stack pointer and 'system' pointer
	|;lea		0x10F300,a7
	|;lea		0x108000,a5

	|;* Reset watchdog
	move.b	d0, 0x300001

	moveq	#0, d0
	move.l	d0, TInextTable
	move.l	d0, VBL_callBack
	move.l	d0, VBL_skipCallBack

	|;* Flush interrupts
	move.b	#7, 0x3C000C
	
	|;* Enable interrupts
	move.w	#0x2000,sr
	
	|;* Initialize BSS section
	move.l	#_end, d0
	sub.l	#__bss_start, d0
	move.l	d0, -(a7)
	clr.l	-(a7)
	pea		__bss_start
	jbsr	memset
	
	jsr	0xc004c2	|; FIX_CLEAR
	jsr	0xc004c8	|; LSP_1st
	
	|;* Jump to main
	jbsr	main

	|;* Call global destructors
	jbsr	__do_global_dtors

	|;* For cart systems, infinite loop
9:
	jmp	9b(pc)

