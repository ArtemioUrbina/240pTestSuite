********************* Vector Definitions *********************
_IRQ1	=	DAT_TIfunc
_IRQ2	=	DAT_vblankTI
_IRQ3	=	_irq3_handler
_ENTRY_USER	= _start
_ENTRY_PLAYER_START	= _dummy_config_handler
_ENTRY_DEMO_END	= _dummy_config_handler |; MVS game switch
_ENTRY_COIN_SOUND	= _dummy_config_handler |; coin sound

************************ Definitions *************************
_NGH	=	0x7777
_PROGRAM_SIZE	=	0x00100000
_WRK_BCKP_AREA = bkp_data
_WRK_BCKP_AREA_SIZE = 0x0100	|;256bytes
_EYE_CATCHER = 0x02	/*;eye catcher (0-common 1-custom 2-off)*/
_EYE_CATCHER_TILES = 0x01 /*;eye catcher start tiles (upper bits, 0x01 => 0x0100)*/

	.include	"common_crt0_cart.s"

* Names MUST be 16 characters long
*           <---------------->
JPconfig:
	.ascii	"HELLO WORLD JP  "
	.word	0xffff, 0xffff
	.byte		0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

NAconfig:
	.ascii	"HELLO WORLD US  "
	.word	0xffff, 0xffff
	.byte		0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	
EUconfig:
	.ascii	"HELLO WORLD EU  "
	.word	0xffff, 0xffff
	.byte		0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
