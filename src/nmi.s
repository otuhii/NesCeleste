nmi:
    pha

    lda #$01
    sta new_frame_starts

    lda #OAM_PAGE
    sta $4014

    pla
    rti 