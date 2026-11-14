initial_game_info:
    .byte $50, $50 ; spawn position x and y


palette_data:
	.byte $3F, $3F, $3F, $3F 	;background palettes
    .byte $00, $0A, $15, $01 	
    .byte $00, $29, $28, $27 	
    .byte $00, $34, $24, $14 	

	.byte $3E, $14, $37, $2C	;sprite palettes
    .byte $00, $0F, $11, $30	
    .byte $00, $0F, $30, $27	
    .byte $00, $3C, $2C, $1C 	