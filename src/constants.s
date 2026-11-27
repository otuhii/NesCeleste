; helpes to address different fields in player - player part in zero page
;       index via player + PLAYER_LOW_X to get access to x position e.g
;memory offsets for zero page player
PLAYER_LOW_X            = 0 ; because of scrolling i need to store world player position(0-511)
PLAYER_HIGH_X           = 1    
PLAYER_Y                = 2
;bautiful two's complement - https://www.youtube.com/watch?v=mEsAsRjeqto
PLAYER_VELOCITY_X       = 3 ;signed integer - need to move later to fixed point numbers 
                                                ;that would be stored in two bytes i guess: one for frac and one for int 
PLAYER_VELOCITY_Y       = 4 ;signed integer
PLAYER_PREVIOUS_STATE   = 5
PLAYER_CURRENT_STATE    = 6
PLAYER_FRAME            = 7
PLAYER_MAX_FRAME_COUNT  = 8
PLAYER_ANIM_TIMER       = 9
PLAYER_ANIM_DURATION    = 10
PLAYER_FLAGS            = 11

;;player aabb values stored which define local offset from player position
PLAYER_AABB_MIN_X       = 12 
PLAYER_AABB_MIN_Y       = 13
PLAYER_AABB_MAX_X       = 14
PLAYER_AABB_MAX_Y       = 15
;0      0       0       0       0       0       0       0
;                                          grounded   facing dir: 
;                                           flag      1 - facing left 
;                                                     0 - facing right


STATE_IDLE      = 0
STATE_RUNNING   = 1
STATE_JUMPING   = 2
STATE_FALLING   = 3
STATE_DASHING   = 4
STATE_CROUCHING = 5



PLAYER_AABB_LOCAL_OFFSET_X = 2
PLAYER_AABB_LOCAL_OFFSET_Y = 0
PLAYER_AABB_WIDTH = 12
PLAYER_AABB_HEIGHT = 16



;;; AABB parameter offsets for zero page collision_a and collision_b
AABB_MIN_X = 0
AABB_MIN_Y = 1
AABB_MAX_X = 2
AABB_MAX_Y = 3




;;;;SOME PHYSICS CONSTANTS
GRAVITY = 1
MAX_FALL_SPEED = 2
JUMP_VELOCITY = 245




;constant memory for player in oam
OAM_PLAYER_POS = 0

;game_info part in zero page
;           index via game_info + SPAWN_POSITION_X to get access to x spawn positoon e.g
SPAWN_POSITION_X = 0
SPAWN_POSITION_Y = 1

OAM_PAGE = $02
OAM_BUFFER_RAM = $0200

;PPU registers
PPUCTRL = $2000
PPUMASK = $2001
PPUSCROLL = $2005
PPUADDR = $2006
PPUDATA = $2007

;scrolling
SCROLL_X_COPY = 0
SCROLL_Y_COPY = 1

SCROLL_THRESHOLD = 128

;;;;; will be storing environment map per nametable tile
; on collisions for environment we just check the index input of that tile to find out 
;                       if it is a solid or we die(spikes) or we just can continue moving
TILE_EMPTY   = %00
TILE_SOLID   = %01
TILE_SPIKES  = %10
TILE_NOSTATENOW = %11 ;not supported


;;;;;;;;;;;;;;;;;;;;;
;       SYSTEM      ;
;;;;;;;;;;;;;;;;;;;;;
CONTROLLER1 = $4016

BUTTON_A_MASK       = %10000000
BUTTON_B_MASK       = %01000000
BUTTON_SELECT_MASK  = %00100000
BUTTON_START_MASK   = %00010000
BUTTON_UP_MASK      = %00001000
BUTTON_DOWN_MASK    = %00000100
BUTTON_LEFT_MASK    = %00000010
BUTTON_RIGHT_MASK   = %00000001






;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;       FAMISTUDIO     ;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
FAMISTUDIO_CFG_EXTERNAL          = 1
FAMISTUDIO_CFG_NTSC_SUPPORT      = 1
FAMISTUDIO_CFG_SFX_SUPPORT       = 0
FAMISTUDIO_CFG_DPCM_SUPPORT      = 0

FAMISTUDIO_USE_FAMITRACKER_TEMPO = 1
FAMISTUDIO_USE_VOLUME_TRACK      = 1
FAMISTUDIO_USE_PITCH_TRACK       = 1
FAMISTUDIO_USE_SLIDE_NOTES       = 1
FAMISTUDIO_USE_DUTYCYCLE_EFFECT  = 1

.define FAMISTUDIO_CA65_ZP_SEGMENT   ZEROPAGE ; famistudio's zero-page variables
.define FAMISTUDIO_CA65_RAM_SEGMENT  BSS      ; famistudio's regular ram variables
.define FAMISTUDIO_CA65_CODE_SEGMENT CODE     ; famistudio's engine code in prg-rom