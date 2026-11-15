wait_for_frame:
@wait:
    lda new_frame_starts

    beq @wait

    lda #$00
    sta new_frame_starts
    rts