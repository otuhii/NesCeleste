initialize_player:
; setting up position to spawn point
    lda game_info + SPAWN_POSITION_X 
    sta player + PLAYER_X
    lda game_info + SPAWN_POSITION_Y
    sta player + PLAYER_Y


    ;reseting player stuff
    lda #STATE_IDLE
    sta player + PLAYER_STATE
    lda #$00
    sta player + PLAYER_FRAME



    ;oam contains this order [top left] [top right] [bottom left] [bottom right]
    ;top left
    lda #$00
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+2
    lda idle_frames
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+1
    lda player+PLAYER_X
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+3
    lda player+PLAYER_Y
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS
    ;;NEED TO TAKE PALLETES INTO ACCOUNT

    ;bottom left
    clc
    adc #$8 ; a already contains y
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+8

    lda #$00
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+10
    lda idle_frames+2
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+9
    lda player + PLAYER_X
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+11

    ;top right
    clc
    adc #$8 ; a already contains x
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+7
    lda #$00
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+6
    lda idle_frames+1
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+5
    lda player + PLAYER_Y
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+4
    
    ;bottom right
    clc
    adc #$8 ; a lready contains y
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+12
    lda player + PLAYER_X
    clc
    adc #$8
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+15
    lda #$00
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+14
    lda idle_frames+3
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+13

    rts


update_player_state:
    ;simple either staying or walking, no jump crouch and other physics shit
    lda #STATE_IDLE
    sta player + PLAYER_STATE
    
    lda current_frame_controller
    and #(BUTTON_LEFT_MASK | BUTTON_RIGHT_MASK)
    beq @done

    lda #STATE_RUNNING
    sta player + PLAYER_STATE
    rts


    @done:
        rts




update_player_physics:
    rts





select_player_animation:
    lda player + PLAYER_STATE

    cmp #STATE_RUNNING
    beq @running

    @staying:
        lda #<idle_frames ; loading low byte of the address
        sta selected_animation
        lda #>idle_frames ;loading high byte of the address
        sta selected_animation+1
        rts
    @running:
        lda #<run_frames
        sta selected_animation
        lda #>run_frames
        sta selected_animation+1
        rts






update_player_animation:
;for any state other than idle and running anim frame should be 0 !!
    lda player + PLAYER_FRAME
                    ;as one frame is 4 bytes of info, our offset per one anim frame 
                    ;should be frame counter * 4
    asl             ; A *= 2
    asl             ; A *= 2 
    tay             ; X = frame * 4 

    lda (selected_animation),y
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 1; Top-left tile index

    iny
    lda (selected_animation),y
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 5; Top-right tile index

    iny
    lda (selected_animation),y
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 9; Bottom-left tile index

    iny
    lda (selected_animation),y
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + $D; Bottom-right tile index

    rts
