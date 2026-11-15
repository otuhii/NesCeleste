initialize_player:
; setting up position to spawn point
    lda game_info + SPAWN_POSITION_X 
    sta player + PLAYER_X
    lda game_info + SPAWN_POSITION_Y
    sta player + PLAYER_Y


    ;reseting player stuff
    lda #STATE_IDLE
    sta player + PLAYER_CURRENT_STATE
    lda #$ff
    sta player + PLAYER_PREVIOUS_STATE ; so we can use update_player_state_info and it wont skip the initialization
                                        ;because state comparisons wont succeed for sure
    lda #$00
    sta player + PLAYER_FRAME
    sta player + PLAYER_VELOCITY_X
    sta player + PLAYER_VELOCITY_Y
    sta player + PLAYER_FLAGS
    sta player + PLAYER_ANIM_TIMER


    jsr update_player_state_info
    jsr select_player_animation
    jsr update_player_oam
    
    rts





update_player:
    jsr update_player_facing


    jsr update_player_state
    jsr update_player_state_info

    jsr advance_player_animation

    jsr select_player_animation
    jsr select_player_hair_animation


    jsr update_player_oam

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



select_player_hair_animation:
    lda player + PLAYER_CURRENT_STATE

    cmp #STATE_DASHING
    beq @dashing

    cmp #STATE_FALLING
    beq @falling

    @idle: 
        lda #<idle_hair_frames
        sta selected_hair_animation
        lda #>idle_hair_frames
        sta selected_hair_animation+1
        rts

    @dashing: ;not handled now
        lda #<dash_hair_frames
        sta selected_hair_animation
        lda #>dash_hair_frames
        sta selected_hair_animation+1
        rts

    @falling: ;not handled now
        lda #<fall_hair_frames
        sta select_player_animation
        lda #>fall_hair_frames
        sta select_player_animation+1
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


update_player_facing:
    lda player + PLAYER_VELOCITY_X

    beq @done
    
    and #%10000000 ;checking sign of the velocity: if 1 it is negative value
    bne @left 

    @right:
        lda player + PLAYER_FLAGS
        and #%11111110
        sta player + PLAYER_FLAGS
        rts

    @left:
        lda player + PLAYER_FLAGS
        ora #%00000001
        sta player + PLAYER_FLAGS
    @done:
        rts 





update_player_oam:
    ;oam contains this order [top left] [top right] [bottom left] [bottom right]
    jsr update_player_oam_positions
    jsr update_player_oam_attributes

    jsr update_player_oam_tiles
    jsr update_hair_oam_tiles

    rts



update_player_oam_positions:
    ;left column
    lda player + PLAYER_X

    ;body
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 3
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 11
    ;hair
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 19
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 27

    ;right column
    clc
    adc #$8

    ;body
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 7
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 15
    ;hair
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 23
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 31



    ;top row
    lda player + PLAYER_Y

    ;body
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 0
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 4
    ;hair
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 16
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 20
    


    ;bottom row
    clc
    adc #8


    ;body
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 8
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 12
    ;hair
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 24
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 28

    rts



update_player_oam_attributes:
    ;;NEED TO TAKE PALLETES INTO ACCOUNT! 
    ; for now only changing horizontal flipping so just hardcoding the flipping if facing is left
    lda player + PLAYER_FLAGS
    and #%00000001
    beq @right

    @left:
        lda #%01000000
        jmp @store
    @right:
        lda #$0

    @store:
        ;body attributes
        sta OAM_BUFFER_RAM+OAM_PLAYER_POS+2
        sta OAM_BUFFER_RAM+OAM_PLAYER_POS+6
        sta OAM_BUFFER_RAM+OAM_PLAYER_POS+14
        sta OAM_BUFFER_RAM+OAM_PLAYER_POS+10

        ora #%00000001 ; storing index 1 for pallete for hair tiles

        ;hair attributes
        sta OAM_BUFFER_RAM+OAM_PLAYER_POS+18
        sta OAM_BUFFER_RAM+OAM_PLAYER_POS+22
        sta OAM_BUFFER_RAM+OAM_PLAYER_POS+26
        sta OAM_BUFFER_RAM+OAM_PLAYER_POS+30

        rts



update_player_oam_tiles:
; indices for facing right are these
; [0] [1]
; [2] [3]
; and indices for facing left are these
; [1] [0]
; [3] [2]
; with horizontal flipping in attributes
; attribute flipping happens in update_player_oam_attributes
; and here we swap tiles
    lda player + PLAYER_FRAME
                    ;as one frame is 4 bytes of info, our offset per one anim frame 
                    ;should be frame counter * 4
    asl             ; A *= 2
    asl             ; A *= 2 
    tay             ; X = frame * 4 

    lda player + PLAYER_FLAGS
    and #%00000001 ; because last bit of flags is responsible for flipping
    bne @facing_left


    @facing_right:
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
        sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 13; Bottom-right tile index

        rts

    @facing_left:
        ; Start from tile 1 instead of tile 0
        iny 
        lda (selected_animation), y
        sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 1

        dey
        lda (selected_animation), y
        sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 5

        iny 
        iny
        iny
        lda (selected_animation), y
        sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 9

        dey
        lda (selected_animation), y
        sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 13

        rts



update_hair_oam_tiles:
;same as for regular player tiles but without
; animation frame processing as no animation for hair
    ldy #$00

    lda player + PLAYER_FLAGS
    and #%00000001
    bne @left

    @right:
        lda (selected_hair_animation), y
        sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 17

        iny 
        lda (selected_hair_animation), y
        sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 21

        iny
        lda (selected_hair_animation), y
        sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 25

        iny 
        lda (selected_hair_animation), y
        sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 29

        rts
    @left:
        iny
        lda (selected_hair_animation),y
        sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 17

        dey
        lda (selected_hair_animation),y
        sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 21

        iny 
        iny
        iny
        lda (selected_hair_animation),y
        sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 25

        dey
        lda (selected_hair_animation),y
        sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 29

        rts