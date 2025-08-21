#include "game-state.h"
#include "config.h"
#include "safe-memory.h"
#include "bit-sweep.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

GameStateManager* createGameStateManager(void) {
    GameStateManager* manager = safeMalloc(sizeof(GameStateManager));
    resetGameState(manager);
    return manager;
}

void freeGameStateManager(GameStateManager* manager) {
    if (manager) {
        free(manager);
    }
}

void resetGameState(GameStateManager* manager) {
    if (!manager) return;
    
    manager->currentState = GAME_STATE_PLAYING;
    manager->revealedCells = 0;
    manager->flaggedMines = 0;
    manager->remainingMines = BOMB_COUNT;
    manager->startTime = time(NULL);
    manager->pausedTime = 0;
    manager->isPaused = false;
}

void updateGameTime(GameStateManager* manager) {
    if (!manager || manager->isPaused) return;
    // Time is automatically updated by system time
}

GameTime getGameTime(GameStateManager* manager) {
    GameTime gameTime = {0, 0, 0};
    if (!manager) return gameTime;
    
    time_t currentTime = time(NULL);
    time_t elapsedTime;
    
    if (manager->isPaused) {
        elapsedTime = manager->pausedTime - manager->startTime;
    } else {
        elapsedTime = currentTime - manager->startTime;
    }
    
    gameTime.hours = (int)(elapsedTime / 3600);
    gameTime.minutes = (int)((elapsedTime % 3600) / 60);
    gameTime.seconds = (int)(elapsedTime % 60);
    
    return gameTime;
}

void pauseGame(GameStateManager* manager) {
    if (!manager || manager->currentState != GAME_STATE_PLAYING) return;
    
    manager->currentState = GAME_STATE_PAUSED;
    manager->pausedTime = time(NULL);
    manager->isPaused = true;
}

void resumeGame(GameStateManager* manager) {
    if (!manager || manager->currentState != GAME_STATE_PAUSED) return;
    
    time_t currentTime = time(NULL);
    time_t pauseDuration = currentTime - manager->pausedTime;
    manager->startTime += pauseDuration;
    
    manager->currentState = GAME_STATE_PLAYING;
    manager->isPaused = false;
}

void setGameOver(GameStateManager* manager) {
    if (!manager) return;
    manager->currentState = GAME_STATE_GAME_OVER;
}

void setGameVictory(GameStateManager* manager) {
    if (!manager) return;
    manager->currentState = GAME_STATE_VICTORY;
}

UIComponents* createUIComponents(void) {
    UIComponents* components = safeMalloc(sizeof(UIComponents));

    // Initialize with modern theme
    setTheme(createModernTheme());

    // Setup button positions and sizes
    int panelX = COLS * CELL_SIZE + GAME_AREA_MARGIN * 2 + UI_PANEL_MARGIN;
    int buttonWidth = 140;
    int buttonHeight = 45;
    int buttonSpacing = 55;
    int startY = 70;

    components->resetButton = createButton(
        (Rectangle){panelX, startY, buttonWidth, buttonHeight},
        "New Game", "R"
    );

    components->pauseButton = createButton(
        (Rectangle){panelX, startY + buttonSpacing, buttonWidth, buttonHeight},
        "Pause", "P"
    );

    components->exitButton = createButton(
        (Rectangle){panelX, startY + buttonSpacing * 2, buttonWidth, buttonHeight},
        "Exit", "X"
    );

    // Setup panels
    components->gamePanel = createPanel(
        (Rectangle){GAME_AREA_MARGIN, GAME_AREA_MARGIN, COLS * CELL_SIZE, ROWS * CELL_SIZE},
        "Minesweeper"
    );

    components->infoPanel = createPanel(
        (Rectangle){panelX - UI_PANEL_MARGIN, GAME_AREA_MARGIN, UI_PANEL_WIDTH - UI_PANEL_MARGIN, ROWS * CELL_SIZE},
        "Game Info"
    );

    // Setup progress bar for time visualization
    components->timeBar = createProgressBar(
        (Rectangle){panelX, startY + buttonSpacing * 3 + 20, buttonWidth, 20},
        ""
    );

    // Setup text components with better spacing
    components->timeText = createText(
        (Vector2){panelX, startY + buttonSpacing * 3 + 60},
        "Time: 00:00:00", 18, getCurrentTheme().primaryText
    );

    components->mineText = createText(
        (Vector2){panelX, startY + buttonSpacing * 3 + 90},
        "Mines: 50", 18, getCurrentTheme().primaryText
    );

    components->statusText = createText(
        (Vector2){panelX, startY + buttonSpacing * 3 + 120},
        "Playing", 18, getCurrentTheme().playingStatus
    );
    components->statusText.hasShadow = true;
    components->statusText.isAnimated = true;

    return components;
}

void freeUIComponents(UIComponents* components) {
    if (components) {
        free(components);
    }
}

// Helper function to count flagged cells
static int countFlaggedCells(BitSweep* bitSweep) {
    if (!bitSweep) return 0;

    int flaggedCount = 0;
    unsigned width = bitSweepWidth(bitSweep);
    unsigned height = bitSweepHeight(bitSweep);
    Cell*** cells = bitSweepCells(bitSweep);

    for (unsigned i = 0; i < width; i++) {
        for (unsigned j = 0; j < height; j++) {
            if (cellIsFlagged(cells[i][j])) {
                flaggedCount++;
            }
        }
    }

    return flaggedCount;
}

void updateGameStats(GameStateManager* manager, void* bitSweep) {
    if (!manager || !bitSweep) return;

    BitSweep* bs = (BitSweep*)bitSweep;

    // Update revealed cells count
    manager->revealedCells = bitSweepOpenedCellsCount(bs);

    // Update flagged mines count using our helper function
    manager->flaggedMines = countFlaggedCells(bs);

    // Update remaining mines
    manager->remainingMines = BOMB_COUNT - manager->flaggedMines;
}

void updateUIComponents(UIComponents* components, GameStateManager* manager) {
    if (!components || !manager) return;

    float deltaTime = GetFrameTime();

    // Update buttons
    updateButton(&components->resetButton);
    updateButton(&components->pauseButton);
    updateButton(&components->exitButton);

    // Update pause button text based on game state
    if (manager->currentState == GAME_STATE_PAUSED) {
        components->pauseButton.text = "Resume";
        components->pauseButton.icon = ">";
    } else {
        components->pauseButton.text = "Pause";
        components->pauseButton.icon = "||";
    }

    // Update time progress bar (max 10 minutes for visualization)
    GameTime gameTime = getGameTime(manager);
    int totalSeconds = gameTime.hours * 3600 + gameTime.minutes * 60 + gameTime.seconds;
    float timeProgress = fminf((float)totalSeconds / 600.0f, 1.0f); // 10 minutes max
    updateProgressBar(&components->timeBar, timeProgress);

    // Update text components
    char timeStr[32];
    sprintf(timeStr, "Time: %02d:%02d:%02d", gameTime.hours, gameTime.minutes, gameTime.seconds);
    components->timeText.text = timeStr;

    char mineStr[32];
    sprintf(mineStr, "Mines: %d", manager->remainingMines);
    components->mineText.text = mineStr;

    // Update status text with colors and animation
    const char* statusText = "";
    Color statusColor = getCurrentTheme().primaryText;

    switch (manager->currentState) {
        case GAME_STATE_PLAYING:
            statusText = "Playing";
            statusColor = getCurrentTheme().playingStatus;
            break;
        case GAME_STATE_PAUSED:
            statusText = "Paused";
            statusColor = getCurrentTheme().pausedStatus;
            break;
        case GAME_STATE_GAME_OVER:
            statusText = "Game Over!";
            statusColor = getCurrentTheme().gameOverStatus;
            components->statusText.isAnimated = true;
            break;
        case GAME_STATE_VICTORY:
            statusText = "Victory!";
            statusColor = getCurrentTheme().victoryStatus;
            components->statusText.isAnimated = true;
            break;
    }

    components->statusText.text = statusText;
    components->statusText.color = statusColor;
    updateText(&components->statusText, deltaTime);
}

void drawUIComponents(UIComponents* components, GameStateManager* manager) {
    if (!components || !manager) return;

    // Draw panels
    drawPanel(&components->infoPanel);

    // Draw buttons
    drawButton(&components->resetButton);
    drawButton(&components->pauseButton);
    drawButton(&components->exitButton);

    // Draw progress bar
    drawProgressBar(&components->timeBar);

    // Draw text components
    drawText(&components->timeText);
    drawText(&components->mineText);
    drawText(&components->statusText);

    // Draw additional decorative elements
    ColorTheme theme = getCurrentTheme();

    // Draw separator line
    int panelX = COLS * CELL_SIZE + GAME_AREA_MARGIN * 2 + UI_PANEL_MARGIN;
    DrawRectangle(panelX, 320, 140, 2, theme.borderColor);

    // Draw game statistics with better formatting
    Vector2 statsPos = {panelX, 340};
    char statsText[64];

    // Statistics title
    DrawText("Statistics:", statsPos.x, statsPos.y, 16, theme.primaryText);

    // Revealed cells
    sprintf(statsText, "Revealed: %d", manager->revealedCells);
    DrawText(statsText, statsPos.x, statsPos.y + 25, 15, theme.secondaryText);

    // Flagged cells
    sprintf(statsText, "Flagged: %d", manager->flaggedMines);
    DrawText(statsText, statsPos.x, statsPos.y + 45, 15, theme.secondaryText);

    // Progress percentage
    int totalCells = COLS * ROWS;
    int nonMineCells = totalCells - BOMB_COUNT;
    float progress = nonMineCells > 0 ? (float)manager->revealedCells / nonMineCells * 100.0f : 0.0f;
    sprintf(statsText, "Progress: %.1f%%", progress);
    DrawText(statsText, statsPos.x, statsPos.y + 65, 15, theme.successText);
}

bool isResetPressed(UIComponents* components) {
    return components && isButtonClicked(&components->resetButton);
}

bool isPausePressed(UIComponents* components) {
    return components && isButtonClicked(&components->pauseButton);
}

bool isExitPressed(UIComponents* components) {
    return components && isButtonClicked(&components->exitButton);
}
