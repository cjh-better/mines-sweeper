#include "cell.h"
#include "safe-memory.h"
#include <stdio.h>

#define NEIGHBOURS_COUNT 8

struct Cell
{
    int i;
    int j;
    bool isOpened;
    bool isMarked;
    CellValue value;
    CellState state;
    Cell** neighbours;
};

Cell* allocateCell(void)
{
    Cell* cell = safeMalloc(sizeof(struct Cell));

    cell->i = 0;
    cell->j = 0;
    cell->isOpened = false;
    cell->isMarked = false;
    cell->value = ZERO;
    cell->state = CELL_STATE_HIDDEN;
    cell->neighbours = safeMalloc(sizeof(Cell*) * (NEIGHBOURS_COUNT + 1));

    for (int i = 0; i < NEIGHBOURS_COUNT + 1; i++)
        cell->neighbours[i] = NULL;

    return cell;
}

void freeCell(Cell* const cell)
{
    if (!cell)
        return;

    free(cell->neighbours);
    free(cell);
}

int cellI(const Cell* const cell)
{
    return cell->i;
}

int cellJ(const Cell* const cell)
{
    return cell->j;
}

void setCellIndexes(Cell* const cell, int i, int j)
{
    cell->i = i;
    cell->j = j;
}

bool cellIsOpened(const Cell* const cell)
{
    return cellIsRevealed(cell);
}

void openCell(Cell* cell)
{
    if (cellContainsBomb(cell)) {
        setCellState(cell, CELL_STATE_MINE_EXPLODED);
    } else {
        setCellState(cell, CELL_STATE_REVEALED);
    }
}

bool cellIsMarked(const Cell* const cell)
{
    return cellIsFlagged(cell);
}

void toggleCellMark(Cell* cell)
{
    if (cellIsOpened(cell)) return;  // 不能标记已打开的单元格
    
    if (cell->state == CELL_STATE_FLAGGED) {
        setCellState(cell, CELL_STATE_HIDDEN);
    } else if (cell->state == CELL_STATE_HIDDEN) {
        setCellState(cell, CELL_STATE_FLAGGED);
    }
}

bool cellContainsBomb(const Cell* const cell)
{
    return cell->value == BOMB;
}

CellValue cellValue(const Cell* const cell)
{
    return cell->value;
}

void setCellValue(Cell* const cell, CellValue value)
{
    cell->value = value;
}

Cell** cellNeighbours(const Cell* const cell)
{
    return cell->neighbours;
}

void addCellNeighbour(Cell* const cell, Cell* const neighbour)
{
    Cell** ptr = cell->neighbours;
    int nCount = 0;

    while (*ptr && nCount < NEIGHBOURS_COUNT)
    {
        if (*ptr == neighbour)
            return;

        ptr++;
        nCount++;
    }

    if (nCount == NEIGHBOURS_COUNT)
        return;

    *ptr = neighbour;
}

void calculateCellValue(Cell* const cell)
{
    if (cellContainsBomb(cell))
        return;

    CellValue value = 0;

    for (int i = 0; i < NEIGHBOURS_COUNT; i++)
        if (cell->neighbours[i] && cellContainsBomb(cell->neighbours[i]))
            value++;

    cell->value = value;
}

// New functions for enhanced cell state management
CellState getCellState(const Cell* const cell)
{
    return cell->state;
}

void setCellState(Cell* const cell, CellState state)
{
    cell->state = state;

    // Update legacy flags for compatibility
    switch (state) {
        case CELL_STATE_REVEALED:
        case CELL_STATE_MINE_EXPLODED:
            cell->isOpened = true;
            cell->isMarked = false;
            break;
        case CELL_STATE_FLAGGED:
            cell->isOpened = false;
            cell->isMarked = true;
            break;
        case CELL_STATE_HIDDEN:
        default:
            cell->isOpened = false;
            cell->isMarked = false;
            break;
    }
}

bool cellIsHidden(const Cell* const cell)
{
    return cell->state == CELL_STATE_HIDDEN;
}

bool cellIsRevealed(const Cell* const cell)
{
    return cell->state == CELL_STATE_REVEALED || cell->state == CELL_STATE_MINE_EXPLODED;
}

bool cellIsFlagged(const Cell* const cell)
{
    return cell->state == CELL_STATE_FLAGGED;
}

bool cellIsMineExploded(const Cell* const cell)
{
    return cell->state == CELL_STATE_MINE_EXPLODED;
}