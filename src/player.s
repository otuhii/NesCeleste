initialize_player:
; setting up position to spawn point
    lda game_info + SPAWN_POSITION_X 
    sta player + PLAYER_X
    lda game_info + SPAWN_POSITION_Y
    sta player + PLAYER_Y


    ;reseting player stuff
    lda #STATE_IDLE
    sta player + PLAYER_CURRENT_STATE
    lda #$00
    sta player + PLAYER_FRAME
    ;initial animation info set
    lda idle_frame_count
    sta player + PLAYER_MAX_FRAME_COUNT
    lda idle_frame_duration
    sta player + PLAYER_ANIM_DURATION


    jsr initialize_base_player_oam
    
    rts


update_player_state:
    ;simple either staying or walking, no jump crouch and other physics shit
    lda player + PLAYER_CURRENT_STATE
    sta player + PLAYER_PREVIOUS_STATE

    lda #STATE_IDLE
    sta player + PLAYER_CURRENT_STATE
    
    lda current_frame_controller
    and #(BUTTON_LEFT_MASK | BUTTON_RIGHT_MASK)
    beq @done

    lda #STATE_RUNNING
    sta player + PLAYER_CURRENT_STATE
    rts


    @done:
        rts




update_player_state_info:
    lda player + PLAYER_CURRENT_STATE
    cmp player + PLAYER_PREVIOUS_STATE
    beq @done


    ; State changed, restart the animation
    lda #$00
    sta player + PLAYER_FRAME
    sta player + PLAYER_ANIM_TIMER
    sta player + PLAYER_ANIM_DURATION
    sta player + PLAYER_MAX_FRAME_COUNT

    lda player + PLAYER_CURRENT_STATE

    cmp #STATE_RUNNING
    beq @running
    cmp #STATE_DASHING
    beq @dashing
    cmp #STATE_CROUCHING
    beq @crouching

    @staying:
        lda idle_frame_count
        sta player + PLAYER_MAX_FRAME_COUNT
        lda idle_frame_duration
        sta player + PLAYER_ANIM_DURATION
        rts
    @running:
        lda run_frame_count
        sta player + PLAYER_MAX_FRAME_COUNT
        lda run_frame_duration
        sta player + PLAYER_ANIM_DURATION
        rts
    @dashing:
        ;;some pallete update and stuff        
        rts
    @crouching:
        ;;later will also have animation
        rts

    @done:
        rts


update_player_physics:

    rts





select_player_animation:
    lda player + PLAYER_CURRENT_STATE

    cmp #STATE_RUNNING
    beq @running
    cmp #STATE_DASHING
    beq @dashing
    cmp #STATE_JUMPING
    beq @jumping
    cmp #STATE_CROUCHING
    beq @crouching
    cmp #STATE_FALLING
    beq @falling

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
    @dashing:
        lda #<dash_frames
        sta selected_animation
        lda #>dash_frames
        sta selected_animation+1
        rts
    @jumping:
        lda #<jump_frames
        sta selected_animation
        lda #>jump_frames   
        sta selected_animation+1
        rts
    @crouching:
        lda #<crouch_frames
        sta selected_animation
        lda #>crouch_frames
        sta selected_animation+1
        rts
    @falling:
        lda #<fall_frames
        sta selected_animation
        lda #>fall_frames
        sta selected_animation+1
        rts


advance_player_animation:
    ;;only for idle and running state animations are supported now
    lda player + PLAYER_CURRENT_STATE
    cmp #STATE_IDLE
    beq @advance_frame
    cmp #STATE_RUNNING
    bne @done

    @advance_frame:
        inc player + PLAYER_ANIM_TIMER

        lda player + PLAYER_ANIM_TIMER
        cmp player + PLAYER_ANIM_DURATION
        bcc @done ; register ≥ operand	z - x	c - 1	

        lda #$00
        sta player + PLAYER_ANIM_TIMER

        inc player + PLAYER_FRAME
        lda player + PLAYER_FRAME
        cmp player + PLAYER_MAX_FRAME_COUNT
        bcc @done
        ;;if our frame is the last one we are resetting it to 0
        lda #$00
        sta player + PLAYER_FRAME

    @done:

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




initialize_base_player_oam:
    ;;NEED TO TAKE PALLETES INTO ACCOUNT! 
    ;oam contains this order [top left] [top right] [bottom left] [bottom right]
    

    ;loading empty attribute
    lda #$00
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+2
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+6
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+14
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+10

    ;loading tile indices
    lda idle_frames
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+1
    lda idle_frames+1
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+5
    lda idle_frames+2
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+9
    lda idle_frames+3
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+13

    ;top left
    lda player+PLAYER_X
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+3

    lda player+PLAYER_Y
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS

    ;;;;;;;;;;;;;;;;;;;;;;bottom left
    clc
    adc #$8 ; a already contains y
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+8

    lda player + PLAYER_X
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+11




    ;;;;;;;;;;;;;;;;;;;;top right
    clc
    adc #$8 ; a already contains x
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+7
    lda #$00
    
    lda player + PLAYER_Y
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+4
    


    ;;;;;;;;;;;;;;;;;bottom right
    clc
    adc #$8 ; a lready contains y
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+12

    lda player + PLAYER_X
    clc
    adc #$8
    sta OAM_BUFFER_RAM+OAM_PLAYER_POS+15

    rts