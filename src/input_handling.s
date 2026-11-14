
;modified nes hacker way - https://www.youtube.com/watch?v=nAStgQzPrAQ
read_controller:
    lda current_frame_controller
    sta previous_frame_controller

    lda #$01
    sta current_frame_controller
    sta CONTROLLER1 ; 1 - enter data collection mode
    lda #$00
    sta CONTROLLER1 ; 0 - enter data out mode - it freezes current state so we can read

    @loop:
        lda CONTROLLER1
        lsr a ; place our just read value into carry
        rol current_frame_controller ; move value from carry into our input buffer, 
                                        ;thus transfering data that we just read
        bcc @loop ;when we will read every value, our initial stored 1 would be in the carry thus we will exit
    
    rts