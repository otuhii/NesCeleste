; helpes to address different fields in player - player part in zero page
;       index via player + PLAYER_X to get access to x position e.g
PLAYER_X                = 0
PLAYER_Y                = 1
;bautiful two's complement - https://www.youtube.com/watch?v=mEsAsRjeqto
PLAYER_VELOCITY_X       = 2 ;signed integer
PLAYER_VELOCITY_Y       = 3 ;signed integer
PLAYER_PREVIOUS_STATE   = 4
PLAYER_CURRENT_STATE    = 5
PLAYER_FRAME            = 6
PLAYER_MAX_FRAME_COUNT  = 7
PLAYER_ANIM_TIMER       = 8
PLAYER_ANIM_DURATION    = 9
PLAYER_FLAGS            = 10
;0      0       0       0       0       0       0       0
;                                                      facing dir: 1 - facing left, 0 - facing right


STATE_IDLE      = 0
STATE_RUNNING   = 1
STATE_JUMPING   = 2
STATE_FALLING   = 3
STATE_DASHING   = 4
STATE_CROUCHING = 5




;constant memory for player in oam
OAM_PLAYER_POS = 0

;game_info part in zero page
;           index via game_info + SPAWN_POSITION_X to get access to x spawn positoon e.g
SPAWN_POSITION_X = 0
SPAWN_POSITION_Y = 1

OAM_PAGE = $02
OAM_BUFFER_RAM = $0200





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
