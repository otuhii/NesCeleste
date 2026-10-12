
;some of these defined variables are not used
CONTROL_OUTPUT = $20
COLOR_VALUE = $21
TEST_VALUE = $13
PPUCTRL   = $2000
PPUMASK   = $2001
PPUSTATUS = $2002
PPUADDR   = $2006
PPUDATA   = $2007

;video helper https://www.youtube.com/watch?v=V5uWqdK92i0&t=47s




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


.segment "CODE"

reset:
    sei
    cld
    
    ldx #$40
    stx $4017 ; disbling sound irq
    ldx #$00
    stx $4010

    ldx #$ff
    txs
    
    ;clearing ppu registers, tells ppu 'to hide' everything
    ldx #$00
    stx $2000
    stx $2001

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;      WAITING FOR VBLANK         ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
:
    ;reading vblank flag into a register, if it is one it gets stored in 7 bit
                    ; accumulator which then is interpreted as negative value
                    ;so we are checking if there is no vblank thus bpl    
    bit $2002
    bpl :- ; we are coming back to the current anonymous label

;;;;;;;;;;;;;;;;;;;;;;;;;;;;



    txa
ClearMemory: ;0000 - 07ff
    sta $0000, x
    sta $0100, x
    sta $0300, x
    sta $0400, x
    sta $0500, x
    sta $0600, x
    sta $0700, x
    

    ;0200 - very frequently used for storing OAM data so in order to have it resetted
    ;it needs to contain FF and not 00 because 0 0 means 0 for x pos and y pos
    ;but ff gets handled that if x or y is ff then it is not rendered
    ; https://www.nesdev.org/wiki/PPU_OAM
    lda #$ff
    sta $0200, x
    lda #$00


    inx 
    cpx #$00
    bne ClearMemory
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;      WAITING FOR VBLANK         ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
:
    ;reading vblank flag into a register, if it is one it gets stored in 7 bit
                    ; accumulator which then is interpreted as negative value
                    ;so we are checking if there is no vblank thus bpl    
    bit $2002
    bpl :- ; we are coming back to the current anonymous label

;;;;;;;;;;;;;;;;;;;;;;;;;;;;


; setting up sprite range
;4014 - highest byte of the oam ram address, which tells the ppu that
;starting from that address, copy next 256 bytes, which in this case is $0200 - $02ff
lda #$02
sta $4014



; setting up color pallete info
lda #$3f ;$3f00 - starting address of pallete ppu memory region
sta $2006 ;2006 - ppu read/write address - it requires two writes
lda #$00                                  ; order - most and then least
sta $2006


;loading palette data onto ppu
ldx #$00
LoadPalettes:
    lda PaletteData, x
    sta $2007 ; data register for ppu writes
                ;every time we read/write from/into the ppu address that we specified
                ;this address gets increased by one byte so second write cycle we will write into 3f01
    inx
    cpx #$20 ; 32 possible palettes
    bne LoadPalettes

;loading object attribute memory data into our ram
ldx #$00
LoadSprites:
    lda SpriteData, x
    sta $0200, x
    inx
    cpx #$10 ; i am loading 4 sprite objects, one sprite object is 4 bytes so 16 bytes in totale 
    bne LoadSprites

; proceed with setting up some other values for ppu
cli

lda #%10010000
sta $2000 ;PPUCTRL

lda #%00011110
sta $2001



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

.segment "VECTORS"

    .word nmi
    .word reset
    .word irq


.segment "CHARS"
    .incbin "visual_data/rom.chr"
