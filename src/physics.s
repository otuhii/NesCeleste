update_physics:

    jsr update_player_physics
    ;will combine some falling surroudings and stuff too
    rts



update_player_physics:
    jsr update_player_velocity
    jsr player_apply_gravity

    jsr move_player_horizontal
    jsr move_player_vertical

    rts



update_player_velocity:
    lda current_frame_controller
    and #BUTTON_LEFT_MASK
    bne @left

    lda current_frame_controller
    and #BUTTON_RIGHT_MASK
    bne @right

    lda #$00
    jmp @store_velocity

    @left: 
        lda #$FF ;-1 in two's complement
        jmp @store_velocity

    @right:
        lda #$01

    @store_velocity:
        sta player+PLAYER_VELOCITY_X

    rts

player_apply_gravity:
    rts

move_player_horizontal:
    clc
    lda player + PLAYER_X
    adc player + PLAYER_VELOCITY_X
    sta player + PLAYER_X

    rts

move_player_vertical:
    rts