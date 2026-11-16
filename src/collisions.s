;Two AABBs overlap when:
;   A.left < B.right
;   A.right > B.left
;   A.top < B.bottom
;   A.bottom > B.top
;All four conditions must hold.
;store one aabb for madeline.
;and check it against every aabb with some general routine
; use collision_a and collision_b, store result of collision: object type, collided in collision_result
check_aabb_collision:

    rts




resolve_horizontal_env_collisions:

    rts



check_player_objects_collisions:


    rts





resolve_vertical_env_collisions:
    lda player + PLAYER_VELOCITY_Y
    beq @done
    bmi @upmove

    ;downmove
    ;checking only two bottom points
    ;but we need to check every index between those two points
    ;because 
    ;Environment tiles (8 pixels each):
    ;|  Tile 0  |  Tile 1  |  Tile 2  |
    ;      [------ Player ------]
    ;          12 pixels wide
    lda player + PLAYER_X
    clc
    adc player + PLAYER_AABB_MIN_X
    
    lsr ; three shifts right divide value by 8 thus we will get current 
            ;index in which cell of nametable our player is remaining
    lsr
    lsr
    sta collision_start_tile_x ; storing first index of col in collision_start_tile_x

    lda player + PLAYER_X
    clc
    adc player + PLAYER_AABB_MAX_X
    sec 
    sbc #$1 ; aabb maximumm boundaries are exclusive so that is why subtracting one

    lsr ; three shifts right divide value by 8 thus we will get current 
            ;index in which cell of nametable our player is remaining
    lsr
    lsr
    sta  collision_end_tile_x ;storing last index of col in collision_end_tile_x zero page

    lda player + PLAYER_Y
    clc
    adc player + PLAYER_AABB_MAX_Y
    sec 
    sbc #$1 ; aabb maximum boundaries are exclusive so that is why subtracting ones

    lsr ; three shifts right divide value by 8 thus we will get current 
            ;index in which cell of nametable our player is remaining
    lsr
    lsr

    sta collision_tile_y


    jsr check_collision_result
    cmp #TILE_SOLID
    bne @done

    ;convering tile row to pixel so we can place him 
    ;on top of that platform that was hit
    lda collision_tile_y
    asl
    asl
    asl ; y(index [0; 30])

    sec
    sbc player + PLAYER_AABB_MAX_Y
    sta player + PLAYER_Y

    lda #$0
    sta player + PLAYER_VELOCITY_Y


    @upmove:


    @done:
        rts


; takes first xpos from collision_start_tile_x and iterates to last xpos from collision_end_tile_x register
; uses ypos from collision_tile_y register
; stores results in register a
check_collision_result:
    ldx collision_start_tile_x
    @loop:
        ldy collision_tile_y
        ;x = tile xpos
        ;y = tile ypos
        jsr get_collision_tile_type

        cmp #TILE_SOLID
        beq @solid

        ;if we reached last tile and still no collision just return no collision
        cpx collision_end_tile_x
        beq @no_collision

        inx
        jmp @loop

    @solid:
        lda #TILE_SOLID 
        rts
    @no_collision:
        lda #TILE_EMPTY
        rts



;writes tile type into a based on x and y register
get_collision_tile_type:
;one byte contains 4 tiles
;one row contains 32 tiles
;one row in collision table occupies 8 bytes
;thus offset with y is y * 8(which will be change to just 3 shifts to right of y value)
;and to get x offset we need to x/4 because when index is 0-3 it will be 0 and so on and it fits our representation in bytes
                                                        ;that first byte is 0-3
    tya 
    asl
    asl
    asl
    sta collision_byte_index; y * 8

    txa ; x / 4
    lsr
    lsr

    clc
    adc collision_byte_index
    tay ; we store our y * 8 + int(x / 4) in y register

    lda collision_table, y ; adding our y * 8 + int(x/4) offset to our table so we get byte that includes our desired tile
    sta collision_packed_byte ;byte contains values for 4 tiles
    
    ;;;;;;;;;;;;;;;
    ;;;;decoding;;;
    ;;;;;;;;;;;;;;


    ;;;;;determining tile position within the byte using % division
    ;      tile%4 == tile & 3 
    ; https://stackoverflow.com/questions/7876351/why-is-x-3-identical-to-x-mod-4?utm_source=chatgpt.com

    txa ;x contains x position to the thing
    and #%00000011 ;tileX % 4 - gives us local position in that bit with remainder
    tay

    lda collision_packed_byte

    cpy #0
    beq @tile0

    cpy #1
    beq @tile1

    cpy #2
    beq @tile2

    jmp @tile3

    ;our shifts accumulate based on where our values is
    ;for 0 6 shifs, for 1 we skip 2 shifts so we shift 4 times and so on
    @tile0:
        lsr
        lsr
    @tile1:
        lsr
        lsr
    @tile2:
        lsr
        lsr
    @tile3:
        and #%00000011
        rts

    rts