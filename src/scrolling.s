update_scroll:
    sec
    lda player + PLAYER_X ; load player x coordinate
    sbc scroll + SCROLL_X_COPY ; subtract it with the x of the viewport to get how far away from the left side the player is
    cmp #100 ; player x needs to be 100 to start scrolling horizontaly
    bcc @return
        inc scroll + SCROLL_X_COPY
        bne @return
            lda ppuctrl_copy
            eor #%00000001
            sta ppuctrl_copy
@return:
    rts


write_scroll:
    lda $2002 ; resets the ppuscroll latch to 0 so that the x value is written to x
    lda scroll + SCROLL_X_COPY
    sta PPUSCROLL ;stores x and y into ppuscroll and copies it
    lda scroll + SCROLL_Y_COPY
    sta PPUSCROLL 
    rts