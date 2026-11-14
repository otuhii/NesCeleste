initialize_player:
; setting up position to spawn point
    lda game_info + SPAWN_POSITION_X 
    sta player + PLAYER_X
    lda game_info + SPAWN_POSITION_Y
    sta player + PLAYER_Y

    
    lda player + PLAYER_Y
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS
    lda jump_frames+0
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 1
    lda #$00
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 2
    lda player + PLAYER_X
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 3


    lda player + PLAYER_Y
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 4 
    
    lda jump_frames+1
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 1 + 4
    lda #$00
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 2 + 4


    lda player + PLAYER_X
    clc
    adc #$8
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 3 + 4


    lda player + PLAYER_Y
    clc
    adc #$8
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 8
    lda jump_frames+2
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 1 + 8
    lda #$00
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 2 + 8
    lda player + PLAYER_X
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 3 + 8


    lda player + PLAYER_Y
    clc
    adc #$8
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 12
    lda jump_frames+3
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 1 + 12
    lda #$00
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 2 + 12
    lda player + PLAYER_X
    clc 
    adc #$8
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 3 + 12


    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;HAIR

    lda player + PLAYER_Y
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 16
    lda idle_hair_frames+0
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 1 + 16
    lda #$01
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 2 + 16
    lda player + PLAYER_X
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 3 + 16


    lda player + PLAYER_Y
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 20
    
    lda idle_hair_frames+1
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 1+ 20
    lda #$01
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 2+ 20


    lda player + PLAYER_X
    clc
    adc #$8
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 3 + 20


    lda player + PLAYER_Y
    clc
    adc #$8
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 24
    lda idle_hair_frames+2
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 1 +24
    lda #$01
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 2 + 24
    lda player + PLAYER_X
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 3 + 24


    lda player + PLAYER_Y
    clc
    adc #$8
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS +  28
    lda idle_hair_frames+3
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 1 + 28
    lda #$01
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 2 +  28
    lda player + PLAYER_X
    clc 
    adc #$8
    sta OAM_BUFFER_RAM + OAM_PLAYER_POS + 3 + 28



    rts


; update_player_animation:
; ;for any state other than idle and running anim frame should be 0 !!
;     lda player + PLAYER_FRAME

;                     ;as one frame is 4 bytes of info, our offset per one anim frame 
;                     ;should be frame counter * 4
;     asl             ; A *= 2
;     asl             ; A *= 2 
;     tax             ; X = frame * 4 



;     lda idle_frames,x
;     sta $0201       ; Top-left tile index

;     lda idle_frames+1,x
;     sta $0205       ; Top-right tile index

;     lda idle_frames+2,x
;     sta $0209       ; Bottom-left tile index

;     lda idle_frames+3,x
;     sta $020D       ; Bottom-right tile index

;     rts
