; helpes to address different fields in player - player part in zero page
;       index via player + PLAYER_X to get access to x position e.g
PLAYER_X      = 0
PLAYER_Y      = 1
PLAYER_STATE  = 2
PLAYER_FRAME  = 3



; STATE_STAYING   = 0
; STATE_RUNNING   = 1
; STATE_JUMPING   = 2
; STATE_FALLING   = 3
; STATE_DASHING   = 4
; STATE_CROUCHING = 5




;constant memory for player in oam
OAM_PLAYER_POS = 0

;game_info part in zero page
;           index via game_info + SPAWN_POSITION_X to get access to x spawn positoon e.g
SPAWN_POSITION_X = 0
SPAWN_POSITION_Y = 1

OAM_PAGE = $02
OAM_BUFFER_RAM = $0200