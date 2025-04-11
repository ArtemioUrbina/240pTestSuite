.text
/* from Mike j Moffitt code at https://github.com/Mikejmoffitt/LICS/ */
| ---------------------------------------------------------------------------
| short set_sr(short new_sr);
| set SR, return previous SR
| entry: arg = SR value
| exit:  d0 = previous SR value
| ---------------------------------------------------------------------------
    .globl set_sr
    .type  set_sr, @function
set_sr:
    moveq   #0,%d0
    move.w  %sr,%d0
    move.l  4(%sp),%d1
    move.w  %d1,%sr
    rts

/* from Chilly Willy example at https://gendev.spritesmind.net/forum/viewtopic.php?t=1018 */
| ---------------------------------------------------------------------------
| Kosinski decompression subroutine
| void Kos_Decomp(uint8_t *src, uint8_t *dst)
| Inputs:
| 4(%sp) = compressed data location
| 8(%sp) = destination
| ---------------------------------------------------------------------------

    .globl Kos_Decomp
    .type  Kos_Decomp, @function
Kos_Decomp:
    movea.l 4(%sp),%a0
    movea.l 8(%sp),%a1
    movm.l %d2-%d6,-(%sp)
    subq.l  #2,%sp          /* make %space for two bytes on the stack */
    move.b  (%a0)+,1(%sp)
    move.b  (%a0)+,(%sp)
    move.w  (%sp),%d5       /* copy first description field */
    moveq   #15,%d4         /* 16 bits in a byte */

Kos_Decomp_Loop:
    lsr.w   #1,%d5          /* bit which is shifted out goes into C flag */
    move    %sr,%d6
    dbra    %d4,Kos_Decomp_ChkBit
    move.b  (%a0)+,1(%sp)
    move.b  (%a0)+,(%sp)
    move.w  (%sp),%d5       /* get next description field if needed */
    moveq   #15,%d4         /* reset bit counter */

Kos_Decomp_ChkBit:
    move    %d6,%ccr            /* was the bit set? */
    bcc.b   Kos_Decomp_RLE      /* if not, branch (C flag clear means bit was clear) */
    move.b  (%a0)+,(%a1)+       /* otherwise, copy byte as-is */
    bra.b   Kos_Decomp_Loop

| ---------------------------------------------------------------------------

Kos_Decomp_RLE:
    moveq   #0,%d3
    lsr.w   #1,%d5          /* get next bit */
    move    %sr,%d6
    dbra    %d4,Kos_Decomp_ChkBit2
    move.b  (%a0)+,1(%sp)
    move.b  (%a0)+,(%sp)
    move.w  (%sp),%d5
    moveq   #15,%d4

Kos_Decomp_ChkBit2:
    move    %d6,%ccr            /* was the bit set? */
    bcs.b   Kos_Decomp_SeparateRLE  /* if it was, branch */
    lsr.w   #1,%d5          /* bit which is shifted out goes into X flag */
    dbra    %d4,1f
    move.b  (%a0)+,1(%sp)
    move.b  (%a0)+,(%sp)
    move.w  (%sp),%d5
    moveq   #15,%d4
1:
    roxl.w  #1,%d3          /* get high repeat count bit (shift X flag in) */
    lsr.w   #1,%d5
    dbra    %d4,2f
    move.b  (%a0)+,1(%sp)
    move.b  (%a0)+,(%sp)
    move.w  (%sp),%d5
    moveq   #15,%d4
2:
    roxl.w  #1,%d3          /* get low repeat count bit */
    addq.w  #1,%d3          /* increment repeat count */
    moveq   #-1,%d2
    move.b  (%a0)+,%d2      /* calculate offset */
    bra.b   Kos_Decomp_RLELoop

| ---------------------------------------------------------------------------

Kos_Decomp_SeparateRLE:
    move.b  (%a0)+,%d0      /* get first byte */
    move.b  (%a0)+,%d1      /* get second byte */
    moveq   #-1,%d2
    move.b  %d1,%d2
    lsl.w   #5,%d2
    move.b  %d0,%d2         /* calculate offset */
    andi.w  #7,%d1          /* does a third byte need to be read? */
    beq.b   Kos_Decomp_SeparateRLE2 /* if it does, branch */
    move.b  %d1,%d3         /* copy repeat count */
    addq.w  #1,%d3          /* and increment it */

Kos_Decomp_RLELoop:
    move.b  (%a1,%d2.w),%d0
    move.b  %d0,(%a1)+      /* copy appropriate byte */
    dbra    %d3,Kos_Decomp_RLELoop  /* and repeat the copying */
    jmp     Kos_Decomp_Loop

| ---------------------------------------------------------------------------

Kos_Decomp_SeparateRLE2:
    move.b  (%a0)+,%d1
    beq.b   Kos_Decomp_Done     /* 0 indicates end of compressed data */
    cmpi.b  #1,%d1
    beq.w   Kos_Decomp_Loop     /* 1 indicates a new description needs to be read */
    move.b  %d1,%d3         /* otherwise, copy repeat count */
    bra.b   Kos_Decomp_RLELoop

| ---------------------------------------------------------------------------

Kos_Decomp_Done:
    addq.l  #2,%sp          /* restore stack pointer to original state */
    movm.l (%sp)+,%d2-%d6
    rts

| End of function Kos_Decomp

/* Helpers */

/* This is the vsync int for Sega CD */
/* Called from VBlankIntCallback */
.globl vblank_scd
vblank_scd:
    movem.l %d0/%a0,-(%sp)
    lea     0xA12000,%a0
    move.w  (%a0),%d0
    ori.w   #0x0100,%d0
    move.w  %d0,(%a0)
    movem.l (%sp)+,%d0/%a0
    rts
