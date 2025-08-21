#include "cell-widget.h"
#include "safe-memory.h"
#include "theme.h"
#include "stdio.h"

static void drawCellBackground(const CellWidget* const widget, const Cell* const cell);
static void drawCellMark(const Rectangle rect, const Cell* const cell);
static void drawCellValue(const Rectangle rect, const Cell* const cell);

CellWidget* allocateCellWidget(
    const Rectangle rect,
    Cell* const cell,
    BitSweep* const bitSweep)
{
    CellWidget* widget = safeMalloc(sizeof(struct CellWidget));

    widget->rect = rect;
    widget->cell = cell;
    widget->bitSweep = bitSweep;

    return widget;
}

void freeCellWidget(CellWidget* const widget)
{
    free(widget);
}

void handleCellWidgetInput(CellWidget* const widget)
{
    const Vector2 mousePos = GetMousePosition();
    const Rectangle rect = widget->rect;

    Cell* cell = widget->cell;

    const bool mouseCollides = CheckCollisionPointRec(mousePos, rect);

    widget->isClicked = IsMouseButtonDown(MOUSE_LEFT_BUTTON) && mouseCollides;

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && mouseCollides)
        openCellAt(widget->bitSweep, cellI(cell), cellJ(cell));

    if (IsMouseButtonReleased(MOUSE_RIGHT_BUTTON) && mouseCollides)
        toggleCellMarkAt(widget->bitSweep, cellI(cell), cellJ(cell));
}

void drawCellWidget(const CellWidget* const widget)
{
    const Rectangle rect = widget->rect;
    const Cell* cell = widget->cell;

    drawCellBackground(widget, cell);

    if (cellIsFlagged(cell))
        drawCellMark(rect, cell);

    if (cellIsOpened(cell))
        drawCellValue(rect, cell);
}

static void drawCellBackground(const CellWidget* const widget, const Cell* const cell)
{
    const bool gameIsFinished = bitSweepIsFinished(widget->bitSweep);
    ColorTheme theme = getCurrentTheme();
    Color cellColor;

    if (cellIsOpened(cell) && cellContainsBomb(cell)) {
        cellColor = theme.mineColor;
    } else if (cellIsOpened(cell) || (widget->isClicked && !gameIsFinished)) {
        cellColor = theme.revealedCell;
    } else if (!cellIsOpened(cell)) {
        cellColor = theme.hiddenCell;

        // Hover effect
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, widget->rect)) {
            cellColor = theme.cellHover;
        }
    }

    // Draw cell with shadow
    Rectangle shadowRect = {
        widget->rect.x + 1,
        widget->rect.y + 1,
        widget->rect.width,
        widget->rect.height
    };
    DrawRectangleRounded(shadowRect, 0.2f, 8, (Color){0, 0, 0, 50});
    DrawRectangleRounded(widget->rect, 0.2f, 8, cellColor);

    // Draw border
    DrawRectangleLinesEx(widget->rect, 1, theme.borderColor);
}

static void drawCellMark(const Rectangle rect, const Cell* cell)
{
    ColorTheme theme = getCurrentTheme();
    const float x = rect.x + rect.width / 2.75;
    const float y = rect.y + rect.width / 5;
    const int fontSize = rect.width / 1.4;

    // Use flag symbol
    DrawText("F", x, y, fontSize, theme.flagColor);
}

static void drawCellValue(const Rectangle rect, const Cell* const cell)
{
    ColorTheme theme = getCurrentTheme();
    float x = rect.x + rect.width / 2.75;
    const float y = rect.y + rect.width / 5;
    const int fontSize = rect.width / 1.4;
    const CellValue value = cellValue(cell);

    char str[4];

    if (value == 1)
        x = rect.x + rect.width / 2.2;

    if (cellContainsBomb(cell)) {
        sprintf(str, "*");
        DrawText(str, x, y, fontSize, theme.mineColor);
    } else if (value > 0) {
        sprintf(str, "%d", value);
        Color numberColor = theme.numbers[value];
        DrawText(str, x, y, fontSize, numberColor);
    }
}
