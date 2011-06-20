* ---------------------------
.global checkbleed_tile
checkbleed_tile:
* ---------------------------
* size:32 bytes


* ---------------------------
	dc.l 0x0F0F0F0F
	dc.l 0xF0F0F0F0
	dc.l 0x0F0F0F0F
	dc.l 0xF0F0F0F0
	dc.l 0x0F0F0F0F
	dc.l 0xF0F0F0F0
	dc.l 0x0F0F0F0F
	dc.l 0xF0F0F0F0
