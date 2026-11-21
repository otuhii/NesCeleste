.include "collisions.s"



update_physics:
    jsr update_player_physics
    ; will combine some falling surroudings and stuff too
    rts



update_player_physics:
    jsr update_player_velocity

    jsr player_try_jump

    jsr player_apply_gravity


    jsr move_player_horizontal
    jsr resolve_horizontal_env_collisions ;using nametable index check from collision table in rom_data, 
                                            ;2 bit per cell for fell possible variants of the cell

    jsr move_player_vertical
    jsr resolve_vertical_env_collisions
    
    jsr check_player_objects_collisions ;using aabb

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



player_try_jump:
    lda current_frame_controller
    and #BUTTON_A_MASK
    beq @done

    lda previous_frame_controller
    and #BUTTON_A_MASK
    bne @done

    lda player + PLAYER_FLAGS
    and #%00000010
    beq @done


    lda #JUMP_VELOCITY
    sta player + PLAYER_VELOCITY_Y

    lda player + PLAYER_FLAGS
    and #%11111101
    sta player + PLAYER_FLAGS

    @done:
        rts


player_apply_gravity:
    lda player + PLAYER_VELOCITY_Y
    cmp #MAX_FALL_SPEED
    beq @done

    clc 
    adc #GRAVITY
    sta player + PLAYER_VELOCITY_Y

    @done:
        rts



move_player_horizontal:
    lda player + PLAYER_LOW_X
    clc
    adc player + PLAYER_VELOCITY_X
    sta player + PLAYER_LOW_X
    
    lda player + PLAYER_VELOCITY_X ; if a velocity would be negative we need to account for moving backwards
                                    ;if position is 0100 and velocity is FF(-1) we dont want to have 01FF but we want to have 00FF
    bmi @negative
    @positive:
        lda player + PLAYER_HIGH_X
        adc #$00
        jmp @done
    @negative:
        lda player + PLAYER_HIGH_X
        adc #$ff ;if carry is 1 we would get 0
                ;if carry is 0 we would get -1(ff)
    @done:
        sta player + PLAYER_HIGH_X
        rts


move_player_vertical:
    clc
    lda player + PLAYER_Y
    adc player + PLAYER_VELOCITY_Y
    sta player + PLAYER_Y

    rts