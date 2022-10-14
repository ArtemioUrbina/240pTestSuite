********************* Vector Definitions *********************
_IRQ1	=	DAT_TIfunc
_IRQ2	=	DAT_vblankTI
_IRQ3	=	_irq3_handler
_ENTRY_USER	= _start
_ENTRY_PLAYER_START	= _240p_mvs_player_start
_ENTRY_DEMO_END	= _240p_mvs_game_change |; MVS game switch
_ENTRY_COIN_SOUND	= _dummy_config_handler |; coin sound

************************ Definitions *************************
_NGH	=	0x2501
_PROGRAM_SIZE	=	0x00100000
_WRK_BCKP_AREA = bkp_data
_WRK_BCKP_AREA_SIZE = 0x0100	|;256bytes
_EYE_CATCHER = 0x01	/*;eye catcher (0-common 1-custom 2-off)*/
_EYE_CATCHER_TILES = 0x00 /*;eye catcher start tiles (upper bits, 0x01 => 0x0100)*/

	.include	"common_crt0_cart.s"

* Names MUST be 16 characters long
*           <---------------->
JPconfig:
	.ascii	"240P TEST SUITE "
	.word	0xffff, 0xffff
	.byte	0xff, 0xff, 0x12, 0x12, 0x02, 0x02, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00
	.ascii	"MVS DEMO    "
	.ascii	"WITHOUT     "
	.ascii	"WITH        "
	.ascii	"CREDIT DISP."
	.ascii	"WITHOUT     "
	.ascii	"WITH        "
	.ascii	"4P CONTROL  "
	.ascii	"FOLLOW DIP 2"
	.ascii	"WITH        "
	.ascii	"HORIZ. RES  "
	.ascii	"USE BIOS 304"
	.ascii	"USE FULL 320"
	.ascii	"WARNINGS    "
	.ascii	"DISABLED    "
	.ascii	"ENABLED     "
	
NAconfig:
	.ascii	"240P TEST SUITE "
	.word	0xffff, 0xffff
	.byte	0xff, 0xff, 0x12, 0x12, 0x02, 0x02, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00
	.ascii	"MVS DEMO    "
	.ascii	"WITHOUT     "
	.ascii	"WITH        "
	.ascii	"CREDIT DISP."
	.ascii	"WITHOUT     "
	.ascii	"WITH        "
	.ascii	"4P CONTROL  "
	.ascii	"FOLLOW DIP 2"
	.ascii	"WITH        "
	.ascii	"HORIZ. RES  "
	.ascii	"USE BIOS 304"
	.ascii	"USE FULL 320"
	.ascii	"WARNINGS    "
	.ascii	"DISABLED    "
	.ascii	"ENABLED     "
	
EUconfig:
	.ascii	"240P TEST SUITE "
	.word	0xffff, 0xffff
	.byte	0xff, 0xff, 0x12, 0x12, 0x02, 0x02, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00
	.ascii	"MVS DEMO    "
	.ascii	"WITHOUT     "
	.ascii	"WITH        "
	.ascii	"CREDIT DISP."
	.ascii	"WITHOUT     "
	.ascii	"WITH        "
	.ascii	"4P CONTROL  "
	.ascii	"FOLLOW DIP 2"
	.ascii	"WITH        "
	.ascii	"HORIZ. RES  "
	.ascii	"USE BIOS 304"
	.ascii	"USE FULL 320"
	.ascii	"WARNINGS    "
	.ascii	"DISABLED    "
	.ascii	"ENABLED     "
