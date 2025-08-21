#pragma once

#define WIDTH 900
#define COLS 16
#define ROWS 16
#define BOMB_COUNT 50
#define CELL_SIZE 30
#define UI_PANEL_WIDTH 200
#define GAME_AREA_MARGIN 10
#define UI_PANEL_MARGIN 15

// Game states
typedef enum {
    GAME_STATE_PLAYING,
    GAME_STATE_PAUSED,
    GAME_STATE_GAME_OVER,
    GAME_STATE_VICTORY
} GameState;