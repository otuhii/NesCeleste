nmi:
    lda #$02
    sta $4014

    rti ; return from interupt, like rts