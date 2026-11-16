; helpes to address different fields in player - player part in zero page
;       index via player + PLAYER_X to get access to x position e.g
;memory offsets for zero page player
PLAYER_X                = 0
PLAYER_Y                = 1
;bautiful two's complement - https://www.youtube.com/watch?v=mEsAsRjeqto
PLAYER_VELOCITY_X       = 2 ;signed integer - need to move later to fixed point numbers 
                                                ;that would be stored in two bytes i guess: one for frac and one for int 
PLAYER_VELOCITY_Y       = 3 ;signed integer
PLAYER_PREVIOUS_STATE   = 4
PLAYER_CURRENT_STATE    = 5
PLAYER_FRAME            = 6
PLAYER_MAX_FRAME_COUNT  = 7
PLAYER_ANIM_TIMER       = 8
PLAYER_ANIM_DURATION    = 9
PLAYER_FLAGS            = 10

;;player_aabb defined locally
PLAYER_AABB_MIN_X       = 11 
PLAYER_AABB_MIN_Y       = 12
PLAYER_AABB_MAX_X       = 13
PLAYER_AABB_MAX_Y       = 14
;0      0       0       0       0       0       0       0
;                                                      facing dir: 1 - facing left, 0 - facing right


STATE_IDLE      = 0
STATE_RUNNING   = 1
STATE_JUMPING   = 2
STATE_FALLING   = 3
STATE_DASHING   = 4
STATE_CROUCHING = 5



PLAYER_AABB_LOCAL_OFFSET_X = 2
PLAYER_AABB_LOCAL_OFFSET_Y = 1
PLAYER_AABB_WIDTH = 12
PLAYER_AABB_HEIGHT = 14



;;; AABB parameter offsets for zero page collision_a and collision_b
AABB_MIN_X = 0
AABB_MIN_Y = 1
AABB_MAX_X = 2
AABB_MAX_Y = 3




;;;;SOME PHYSICS CONSTANTS
GRAVITY = 1
MAX_FALL_SPEED = 4






;constant memory for player in oam
OAM_PLAYER_POS = 0

;game_info part in zero page
;           index via game_info + SPAWN_POSITION_X to get access to x spawn positoon e.g
SPAWN_POSITION_X = 0
SPAWN_POSITION_Y = 1

OAM_PAGE = $02
OAM_BUFFER_RAM = $0200



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

