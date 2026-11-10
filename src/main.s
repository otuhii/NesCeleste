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


.segment "VECTORS"

    .addr nmi
    
    .addr reset

    .addr irq

.segment "CODE"

.include "globals.s"
.include "startup.s"



forever:
    jmp forever


nmi:
    lda #$02
    sta $4014

    rti ; return from interupt, like rts

irq:
    rti ; return from interupt, like rts

PaletteData:
	.byte $00, $0F, $00, $10, 	$00, $0A, $15, $01, 	$00, $29, $28, $27, 	$00, $34, $24, $14 	;background palettes
	.byte $31, $0F, $15, $30, 	$00, $0F, $11, $30, 	$00, $0F, $30, $27, 	$00, $3C, $2C, $1C 	;sprite palettes


;y, tile index from pattern table, attributes, X
SpriteData:
    .byte $40, $d1, $00, $40
    .byte $40, $d2, $00, $48
    .byte $48, $e1, $00, $40
    .byte $48, $e2, $00, $48




.segment "CHARS"
    .incbin "visual_data/rom.chr"
