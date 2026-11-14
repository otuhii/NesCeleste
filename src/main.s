.include "globals.s"


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
player: 
    .res 4


.segment "VECTORS"

    .addr nmi
    
    .addr reset

    .addr irq

.segment "CODE"


.include "player.s"
.include "startup.s"



main_loop:

    jmp main_loop



irq:
    rti ;nothing to do



.include "nmi.s"

.include "rom_data.s"

.segment "CHARS"
    .incbin "visual_data/celeste.chr"
