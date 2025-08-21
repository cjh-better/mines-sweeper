#pragma once

#include "stdbool.h"

// Forward declarations to avoid circular dependencies
typedef enum {
    CELL_STATE_HIDDEN,
    CELL_STATE_REVEALED,
    CELL_STATE_FLAGGED,
    CELL_STATE_MINE_EXPLODED
} CellState;

typedef enum
{
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    BOMB
} CellValue;

typedef struct Cell Cell;

Cell* allocateCell(void);
void freeCell(Cell* const cell);

int cellI(const Cell* const cell);
int cellJ(const Cell* const cell);
void setCellIndexes(Cell* const cell, int i, int j);

bool cellIsOpened(const Cell* const cell);
void openCell(Cell* const cell);

bool cellIsMarked(const Cell* const cell);
void toggleCellMark(Cell* const cell);

bool cellContainsBomb(const Cell* const cell);

CellValue cellValue(const Cell* const cell);
void setCellValue(Cell* const cell, CellValue value);

Cell** cellNeighbours(const Cell* const cell);
void addCellNeighbour(Cell* const cell, Cell* const neighbour);

void calculateCellValue(Cell* const cell);

// New functions for enhanced cell state management
CellState getCellState(const Cell* const cell);
void setCellState(Cell* const cell, CellState state);
bool cellIsHidden(const Cell* const cell);
bool cellIsRevealed(const Cell* const cell);
bool cellIsFlagged(const Cell* const cell);
bool cellIsMineExploded(const Cell* const cell);