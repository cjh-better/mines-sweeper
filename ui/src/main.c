#include "bit-sweep-widget.h"
#include "bit-sweep.h"
#include "cell-widget.h"
#include "config.h"
#include "game-state.h"
#include "raylib.h"
#include "time.h"
#include <math.h>

BitSweepWidget* widget;
GameStateManager* gameState;
UIComponents* uiComponents;

static void createNewGame(void);
static void handleInput(void);
static void updateGame(void);
static void drawGame(void);

void main(void)
{
    InitWindow(COLS * CELL_SIZE + UI_PANEL_WIDTH + GAME_AREA_MARGIN * 2, ROWS * CELL_SIZE + GAME_AREA_MARGIN * 2, "Enhanced Minesweeper");
    SetTargetFPS(60);

    // Initialize game components
    gameState = createGameStateManager();
    uiComponents = createUIComponents();
    createNewGame();

    while (!WindowShouldClose())
    {
        handleInput();
        updateGame();
        drawGame();
    }

    // Cleanup
    CloseWindow();
    freeBitSweepWidget(widget);
    freeGameStateManager(gameState);
    freeUIComponents(uiComponents);
}

void createNewGame(void)
{
    BitSweepParams params = { time(NULL), COLS, ROWS, BOMB_COUNT };

    if (widget) {
        freeBitSweepWidget(widget);
    }
    widget = allocateBitSweepWidget(params);

    if (gameState) {
        resetGameState(gameState);
    }
}

static void handleInput(void)
{
    if (!gameState || !uiComponents) return;

    // Handle button presses
    if (isResetPressed(uiComponents)) {
        createNewGame();
    }

    if (isPausePressed(uiComponents)) {
        if (gameState->currentState == GAME_STATE_PLAYING) {
            pauseGame(gameState);
        } else if (gameState->currentState == GAME_STATE_PAUSED) {
            resumeGame(gameState);
        }
    }

    if (isExitPressed(uiComponents)) {
        CloseWindow();
    }

    // Handle game input only if playing
    if (gameState->currentState == GAME_STATE_PLAYING && widget) {
        handleBitSweepWidgetInput(widget);

        // Check for game state changes
        if (bitSweepIsFinished(widget->bitSweep)) {
            if (bitSweepIsWon(widget->bitSweep)) {
                setGameVictory(gameState);
            } else {
                setGameOver(gameState);
            }
        }
    }

    // Handle restart with R key
    if (IsKeyPressed(KEY_R) && (gameState->currentState == GAME_STATE_GAME_OVER ||
                                gameState->currentState == GAME_STATE_VICTORY)) {
        createNewGame();
    }
}

static void updateGame(void)
{
    if (!gameState || !uiComponents) return;

    updateGameTime(gameState);

    // Update game statistics if widget exists
    if (widget && widget->bitSweep) {
        updateGameStats(gameState, widget->bitSweep);
    }

    updateUIComponents(uiComponents, gameState);
}

static void drawGame(void)
{
    BeginDrawing();

    // Use theme background color
    ColorTheme theme = getCurrentTheme();
    ClearBackground(theme.background);

    // Draw game board with panel background
    if (widget && gameState->currentState != GAME_STATE_PAUSED) {
        drawBitSweepWidget(widget);
    } else if (gameState->currentState == GAME_STATE_PAUSED) {
        // Draw paused overlay with modern styling
        Rectangle pauseOverlay = {
            GAME_AREA_MARGIN + 5,
            GAME_AREA_MARGIN + 5,
            COLS * CELL_SIZE - 10,
            ROWS * CELL_SIZE - 10
        };
        DrawRectangleRounded(pauseOverlay, 0.05f, 8, (Color){0, 0, 0, 180});

        // Animated pause text
        const char* pauseText = "PAUSED";
        int textWidth = MeasureText(pauseText, 32);
        Vector2 pauseTextPos = {
            pauseOverlay.x + (pauseOverlay.width - textWidth) / 2,
            pauseOverlay.y + pauseOverlay.height / 2 - 16
        };

        // Add subtle animation
        float bounce = sinf(GetFrameTime() * 50.0f) * 2.0f;
        pauseTextPos.y += bounce;

        DrawText(pauseText, pauseTextPos.x, pauseTextPos.y, 32, theme.pausedStatus);

        // Instructions
        const char* instruction = "Click Resume to continue";
        int instrWidth = MeasureText(instruction, 16);
        Vector2 instrPos = {
            pauseOverlay.x + (pauseOverlay.width - instrWidth) / 2,
            pauseTextPos.y + 50
        };
        DrawText(instruction, instrPos.x, instrPos.y, 16, theme.secondaryText);
    }

    // Draw UI components
    if (uiComponents && gameState) {
        drawUIComponents(uiComponents, gameState);
    }

    EndDrawing();
}