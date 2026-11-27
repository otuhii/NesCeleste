initial_game_info:
    .byte $60, $50 ; spawn position x and y


palette_data:
	.byte $2F, $30, $21, $11 	;background palettes
    .byte $0F, $30, $3d, $2d 	
    .byte $0f, $39, $29, $19 	
    .byte $0f, $37, $16, $06 	

	.byte $3E, $14, $37, $2C	;sprite palettes
    .byte $0F, $06, $36, $16	
    .byte $00, $0F, $30, $27	
    .byte $00, $3C, $2C, $1C 	


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;           ANIMATION           ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.include "pattern_indices.s" 
idle_frames:
    .byte TILE_IDLE_0_TL, TILE_IDLE_0_TR
    .byte TILE_IDLE_0_BL, TILE_IDLE_0_BR
    
    .byte TILE_IDLE_1_TL, TILE_IDLE_1_TR
    .byte TILE_IDLE_1_BL, TILE_IDLE_1_BR

run_frames:
    .byte TILE_RUN_0_TL, TILE_RUN_0_TR
    .byte TILE_RUN_0_BL, TILE_RUN_0_BR

    .byte TILE_RUN_1_TL, TILE_RUN_1_TR
    .byte TILE_RUN_1_BL, TILE_RUN_1_BR

    .byte TILE_RUN_2_TL, TILE_RUN_2_TR
    .byte TILE_RUN_2_BL, TILE_RUN_2_BR

    .byte TILE_RUN_3_TL, TILE_RUN_3_TR
    .byte TILE_RUN_3_BL, TILE_RUN_3_BR

jump_frames:
    .byte TILE_JUMP_TL, TILE_JUMP_TR
    .byte TILE_JUMP_BL, TILE_JUMP_BR

fall_frames:
    .byte TILE_FALL_TL, TILE_FALL_TR
    .byte TILE_FALL_BL, TILE_FALL_BR

dash_frames:
    .byte TILE_DASH_TL, TILE_DASH_TR
    .byte TILE_DASH_BL, TILE_DASH_BR

crouch_frames:
    .byte TILE_CROUCH_TL, TILE_CROUCH_TR
    .byte TILE_CROUCH_BL, TILE_CROUCH_BR

idle_frame_count:
    .byte 2
idle_frame_duration:
    .byte 20

run_frame_count:
    .byte 4
run_frame_duration:
    .byte 10






;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;       HAIR            ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

idle_hair_frames:
    .byte TILE_HAIR_IDLE_TL, TILE_HAIR_IDLE_TR
    .byte TILE_HAIR_IDLE_BL, TILE_HAIR_IDLE_BR

fall_hair_frames:
    .byte TILE_HAIR_FALL_TL, TILE_HAIR_FALL_TR
    .byte TILE_HAIR_FALL_BL, TILE_HAIR_FALL_BR

dash_hair_frames:
    .byte TILE_HAIR_DASH_TL, TILE_HAIR_DASH_TR
    .byte TILE_HAIR_DASH_BL, TILE_HAIR_DASH_BR

;;more offset may be added as i will use other animation states
idle_hair_offsets:
    .byte 0, 0      ;idle frame 0 
    .byte 0, 1   ;idle frame 1, y in two's complement so 255 is -1

run_hair_offsets:
    .byte 255, 255
    .byte 255, 254
    .byte 255, 0
    .byte 255, 254



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;       COLLISION DATA          ;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

collision_table_0:
    .incbin "visual_data/level_info/collisions.bin", 0, 240

collision_table_1:
    .incbin "visual_data/level_info/collisions.bin", 240, 240

initial_nametable_info:
    .incbin "visual_data/level_info/nametables.bin"