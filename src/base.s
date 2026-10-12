CONTROL_OUTPUT = $20
COLOR_VALUE = $21
TEST_VALUE = $13
PPUCTRL   = $2000
PPUMASK   = $2001
PPUSTATUS = $2002
PPUADDR   = $2006
PPUDATA   = $2007


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
    lda #$de
    ldx #$ad

forever:
    jmp forever

nmi:
    rti ; return from interupt, like rts

irq:
    rti ; return from interupt, like rts


.segment "VECTORS"

    .word nmi
    .word reset
    .word irq


.segment "CHARS"

    .res $2000, $00
