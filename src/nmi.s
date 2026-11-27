nmi:
    pha 
    txa 
    pha 
    tya 
    pha 

    lda #$01
    sta new_frame_starts

    lda #OAM_PAGE
    sta $4014

    jsr write_scroll
    
    lda ppuctrl_copy
    sta PPUCTRL

    pla 
    tay 
    pla 
    tax 
    pla 

    rti 