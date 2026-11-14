initial_game_info:
    .byte $50, $50 ; spawn position x and y


palette_data:
	.byte $3F, $3F, $3F, $3F 	;background palettes
    .byte $00, $0A, $15, $01 	
    .byte $00, $29, $28, $27 	
    .byte $00, $34, $24, $14 	

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


idle_hair_frames:
    .byte TILE_HAIR_IDLE_TL, TILE_HAIR_IDLE_TR
    .byte TILE_HAIR_IDLE_BL, TILE_HAIR_IDLE_BR

fall_hair_frames:
    .byte TILE_HAIR_FALL_TL, TILE_HAIR_FALL_TR
    .byte TILE_HAIR_FALL_BL, TILE_HAIR_FALL_BR

dash_hair_frames:
    .byte TILE_HAIR_DASH_TL, TILE_HAIR_DASH_TR
    .byte TILE_HAIR_DASH_BL, TILE_HAIR_DASH_BR