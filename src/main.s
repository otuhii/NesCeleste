.segment "HEADER";mesen helper, specifying memory layout and some other stuff

    ; iNES header
    .byte "NES", $1A

    .byte 2          ; 2 × 16 KB PRG = 32 KB
    .byte 1          ; 1 × 8 KB CHR = 8 KB

    .byte $00        ; mapper 0, horizontal mirroring
    .byte $00

    ; remaining bytes of 16-byte header
    .byte $00, $00, $00, $00
    .byte $00, $00, $00, $00



.segment "ZEROPAGE"
;intitial game data
;should remain constant over time, used for level reseting after player death
game_info: 
    .res 2

;SOME GENERAL STUFF
new_frame_starts:
    .res 1
player: 
    .res 15

;;;;INPUT
previous_frame_controller:
    .res 1
current_frame_controller:
    .res 1

;;;;ANIMATIONS
selected_animation: ; 16 bit address to rom predefined animation oam layouts
    .res 2
selected_hair_animation:
    .res 2
selected_hair_offsets:
    .res 2


;;COLLISIONS


;;;;;;;;;;;;;;;;;;;;;;;;
;aabb object collisions;
;;;;;;;;;;;;;;;;;;;;;;;;
collision_a: 
    .res 4
collision_b: 
    .res 4
collision_result: 
    .res 1
;;;;;;;;;;;;;;;;;;;;;;;;





;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;environment index collisions with collision table;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;vertical collision when y is fixed and we have range of x so we check all tiles spanned by player aabb
collision_start_tile_x: ;left corner x in tile indices range
    .res 1
collision_end_tile_x: ;right corner x in tile indices range
    .res 1
collision_tile_y: ;corner y that picks either top or bottom of aabb
    .res 1

;horizontal collision when x is fixed and we have range of y so we check all tiles spanned by player aabb
collision_start_tile_y: ;top corner y in tile indices range
    .res 1
collision_end_tile_y: ;bottom curner y in tile indices range
    .res 1
collision_tile_x: ; ;x value that picks either right or left of aabb
    .res 1


collision_byte_index: ; offset for collisition_table to get to byte that includes desired tile
                                    ;(because info for one tile is only 2 bit)
    .res 1
collision_packed_byte:
    .res 1
collision_tile_type:
    .res 1
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;




.segment "VECTORS"

    .addr nmi
    
    .addr reset

    .addr irq

.segment "CODE"

.include "constants.s"

.include "player.s"
.include "physics.s"
.include "input_handling.s"
.include "startup.s"


main_loop:
    jsr wait_for_frame ; uppdating everything once per frame

    jsr famistudio_update

    jsr read_controller

    jsr update_physics
    
    jsr update_player

    jmp main_loop




irq:
    rti ;nothing to do



.include "nmi.s"
.include "sync.s"

.include "rom_data.s"


;;;;;;;;;;;;;;;
;;;;;music;;;;;
.include "audio_data/famistudio_ca65.s"
.include "audio_data/jazz.s"
;;;;;;;;;;;;;;



.segment "CHARS"
    .incbin "visual_data/celeste.chr"
