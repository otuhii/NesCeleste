load_palettes:
    bit $2002 ; reset latch
    lda #$3f  ; load high byte of #$3f00
    sta PPUADDR ; $2006 PPUADDR sets the Vram address for when $2007 will be used
    lda #$00  ; load low byte of #$3f00
    sta PPUADDR 
        
    ;loading palette data onto ppu
    ldx #$00
    @loop:
    lda palette_data, x
    sta PPUDATA ; data register for ppu writes
                ;every time we read/write from/into the ppu address that we specified
                ;this address gets increased by one byte so second write cycle we will write into 3f01
    inx ; x is an index, like i in for loop
    cpx #$20 ; 32 possible palettes
    bne @loop
    rts 

title_screen:
    jsr load_palettes

    bit $2002 ; reset latch
    lda #$20 ; load high byte of #$2000
    sta PPUADDR ; in $2006 you lode new Vram adress that you will write to using $2007
    lda #$00 ; load low byte of #$2000
    sta PPUADDR 

    lda #<title_screen_nametable_info ; < means low byte or lower 8 bits that will make up the nametable_ptr
    sta nametable_read_ptr ; pointer to tile data in ROM, 16 bits, or 2 bytes
    lda #>title_screen_nametable_info ; > means high byte the upper 8 bits or thje last byte that will make up the nametable_ptr
    sta nametable_read_ptr + 1 ; stores the next 8 bits into the address 1 after the nametable_read_ptr

    ldx #$04 ; how many times you have to wrap around y until you copy the whole nametable (1024/256 = 4)
    ldy #$00 ; current byte of part of the current nametable thats being copied
@name_table_loop:
    lda (nametable_read_ptr), y ; reads byte at pointer + y
    sta PPUDATA ; write the data into the vram
    iny ; increase the offset
    bne @name_table_loop ; if y = 255 and after the iny it wraps around becomes 0. Z = 1, zero flag gets triggered and bne does not branch
    inc nametable_read_ptr+1 ; adds 1 to high byte of pointer which makes the pointer move 256 bytes
    dex ; decreses the amount of times y has to wrap around until the nametable is finished being copied to vram
    bne @name_table_loop ; if the x reaches 0, z=1 bne does not branch

bit $2002 ; reset latch
lda #$00 ; default pos 0
sta PPUSCROLL ; reset x pos to 0, do this because writing to $2006 and $2007 scrambles up ppu scroll pos
sta PPUSCROLL ; reset y pos to 0
lda #%00010000 ; make it so that the tiles are being selected from the second pattern table(bit 4 in ppuctrl)
sta PPUCTRL 
lda #%00001010 ; bit 3 of PPUMASK turns on background rendering, so that you can see the title screen, bit 1 makes it so that first 8 pixels from left are shown
sta PPUMASK 
@wait_press:
    lda #1
    sta $4016
    lda #0
    sta $4016 ; this sequence basically tels the controller to latch all current buttons in the shift register

    lda $4016 ; loads the bits from shift register 1 by 1. since button A comes first, we only need to do it 1 time per loop
    and #%00000001 ; check if the A is pressed
    beq @wait_press ; if A = 00000001, if A not pressed, controller returns #0, so the and fails, and Z=1 so it branches, when A pressed, %00000001 returns, with and Z = 0, so it doesnt branches

@wait_release: ; i don't understand why but for some reason it does not work, maybe just on my laptop, need to check on NES
    lda #1
    sta $4016
    lda #0
    sta $4016 ; forces the controller to latch all buttons to shift register
    lda $4016 ; takes the first button from shift register which is a which is what we need
    and #%00000001 ; checks if its being pressed
    bne @wait_release ; if is pressed, the loop and wait until it is not

lda #00
sta PPUMASK 

rts


load_rom_data:
    ;loading initial game info from rom to zero page ram game_info, why do that? idk
    lda initial_game_info + SPAWN_POSITION_X
    sta game_info + SPAWN_POSITION_X
    lda initial_game_info + SPAWN_POSITION_Y
    sta game_info + SPAWN_POSITION_Y

    

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;loading nametables and attibute tables onto ppu;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;maybe rewrite because i am not sure if writing this way is the best way
    lda $2002 ; read resets write pair for $2005/$2006
    lda #$20 ;high byte of address
    sta $2006
    lda #$00 ;low byte of address
    sta $2006
    ldx #$00

    ;setting up zero page pointer to current write page
    lda #<initial_nametable_info
    sta nametable_read_ptr
    lda #>initial_nametable_info
    sta nametable_read_ptr+1

    ldy #$00
    ldx #$00
    @load_background_loop:
        lda (nametable_read_ptr), y
        sta $2007

        iny
        bne @load_background_loop

        inc nametable_read_ptr+1
        inx
        ldy #$00
        
        cpx #$8
        bne @load_background_loop

    @end:
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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;      TITLE SCREEN STUFF         ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

jsr title_screen ; do title screen stuff

; Initialize audio hardware
ldx #<music_data_eb_02_2014b
ldy #>music_data_eb_02_2014b
lda #1                         ; 1 = NTSC
jsr famistudio_init

lda #0                         ; song index
jsr famistudio_music_play
; setting up sprite range
;4014 - highest byte of the oam ram address, which tells the ppu that
;starting from that address, copy next 256 bytes, which in this case is $0200 - $02ff
lda #OAM_PAGE
sta $4014


; setting up color pallete info
lda #$3f ;$3f00 - starting address of pallete ppu memory region
sta $2006 ;2006 - ppu read/write address - it requires two writes
lda #$00                                  ; order - most and then least
sta $2006

jsr load_rom_data

lda #0
sta PPUSCROLL ; stores x
sta PPUSCROLL ; stores y
sta scroll + SCROLL_X_COPY
sta scroll + SCROLL_Y_COPY

jsr initialize_player

lda #$00
sta new_frame_starts

; proceed with setting up some other values for ppu
cli

lda #%10010000
sta PPUCTRL 
sta ppuctrl_copy

lda #%00011110
sta $2001
