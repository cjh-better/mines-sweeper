#pragma once

#include "config.h"
#include "raylib.h"
#include "ui-components.h"
#include "theme.h"
#include <time.h>
#include <stdbool.h>

typedef struct {
    GameState currentState;
    int revealedCells;
    int flaggedMines;
    int remainingMines;
    time_t startTime;
    time_t pausedTime;
    bool isPaused;
} GameStateManager;

typedef struct {
    EnhancedButton resetButton;
    EnhancedButton pauseButton;
    EnhancedButton exitButton;
    UIPanel gamePanel;
    UIPanel infoPanel;
    ProgressBar timeBar;
    EnhancedText timeText;
    EnhancedText mineText;
    EnhancedText statusText;
} UIComponents;

typedef struct {
    int hours;
    int minutes;
    int seconds;
} GameTime;

// Function declarations
GameStateManager* createGameStateManager(void);
void freeGameStateManager(GameStateManager* manager);
void resetGameState(GameStateManager* manager);
void updateGameTime(GameStateManager* manager);
GameTime getGameTime(GameStateManager* manager);
void pauseGame(GameStateManager* manager);
void resumeGame(GameStateManager* manager);
void setGameOver(GameStateManager* manager);
void setGameVictory(GameStateManager* manager);

UIComponents* createUIComponents(void);
void freeUIComponents(UIComponents* components);
void updateGameStats(GameStateManager* manager, void* bitSweep);
void updateUIComponents(UIComponents* components, GameStateManager* manager);
void drawUIComponents(UIComponents* components, GameStateManager* manager);
bool isResetPressed(UIComponents* components);
bool isPausePressed(UIComponents* components);
bool isExitPressed(UIComponents* components);
