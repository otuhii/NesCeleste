load_rom_data:
    ;loading initial game info from rom to zero page ram game_info
    lda initial_game_info + SPAWN_POSITION_X
    sta game_info + SPAWN_POSITION_X
    lda initial_game_info + SPAWN_POSITION_Y
    sta game_info + SPAWN_POSITION_Y



    ;loading palette data onto ppu
    ldx #$00
    @load_palettes:
        lda palette_data, x
        sta $2007 ; data register for ppu writes
                ;every time we read/write from/into the ppu address that we specified
                ;this address gets increased by one byte so second write cycle we will write into 3f01
        inx
        cpx #$20 ; 32 possible palettes
        bne @load_palettes

    rts



reset:
    sei
    cld
    
    ldx #$40
    stx $4017 ; disbling sound irq
    ldx #$00
    stx $4010

    ldx #$ff
    txs ;;making sure stack pointer is defined
    
    ;clearing ppu registers, tells ppu 'to hide' everything
    ldx #$00
    stx $2000 ;disabling nmi
    stx $2001 ;disabling rendering

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;      WAITING FOR VBLANK         ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
:
    ;reading vblank flag into a register, if it is one it gets stored in 7 bit
                    ; accumulator which then is interpreted as negative value
                    ;so we are checking if there is no vblank thus bpl    
    bit $2002
    bpl :- 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;we need to wait for the PPU to finish warming up anyway for 30000 cycles or something, so we use some of that waiting time to do useful work 
;that is why we clear zero our memory between two first blanks
lda #$00
clear_memory: ;0000 - 07ff
    sta $0000, x
    sta $0100, x
    sta $0300, x
    sta $0400, x
    sta $0500, x
    sta $0600, x
    sta $0700, x
    
    ;zeroing with ff because when y is bigger\
    ; than 240 in dec it hides the sprite from the screen
    lda #$ff
    sta $0200, x
    lda #$00


    inx 
    cpx #$00
    bne clear_memory


;The first VBlank alone isn't a sufficient guarantee that the PPU is fully initialized
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;      WAITING FOR VBLANK         ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
:
    bit $2002
    bpl :- 
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


jsr load_rom_data

jsr initialize_player



; proceed with setting up some other values for ppu
cli

lda #%10010000
sta $2000 ;PPUCTRL

lda #%00011110
sta $2001
