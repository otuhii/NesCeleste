update_scroll:
    lda player + PLAYER_VELOCITY_X
    bmi @reverse_scrolling

    @normal_scrolling:
        sec
        lda player + PLAYER_LOW_X ; load player x coordinate
        sbc scroll + SCROLL_X_COPY ; subtract it with the x of the viewport to get how far away from the left side the player is
        cmp #SCROLL_THRESHOLD; player x needs to be bigger than threshold to start scrolling horizontaly
        bcc @return
        
        cmp #255 ; max value for viewport so it does not wrap to the first nametable
        beq @return
        inc scroll + SCROLL_X_COPY
        
        bne @return
        lda ppuctrl_copy
        eor #%00000001
        sta ppuctrl_copy
        jmp @return

    @reverse_scrolling:
        sec
        lda player + PLAYER_LOW_X
        sbc scroll + SCROLL_X_COPY
        cmp #SCROLL_THRESHOLD
        bcs @return

        lda scroll + SCROLL_X_COPY
        beq @return ;min value 00 for viewport so it does not wrap to second nametable

        dec scroll + SCROLL_X_COPY
    
    @return:
        rts


write_scroll:
    lda $2002 ; resets the ppuscroll latch to 0 so that the x value is written to x
    lda scroll + SCROLL_X_COPY
    sta PPUSCROLL ;stores x and y into ppuscroll and copies it
    lda scroll + SCROLL_Y_COPY
    sta PPUSCROLL 
    rts