#pragma once

#include "raylib.h"
#include "theme.h"
#include <stdbool.h>

// Button states
typedef enum {
    BUTTON_STATE_NORMAL,
    BUTTON_STATE_HOVER,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_DISABLED
} ButtonState;

// Enhanced button structure
typedef struct {
    Rectangle bounds;
    const char* text;
    const char* icon; // Unicode icon or emoji
    ButtonState state;
    bool isPressed;
    bool wasPressed;
    float animationTime;
    Color currentColor;
} EnhancedButton;

// Panel structure for organized UI
typedef struct {
    Rectangle bounds;
    const char* title;
    Color backgroundColor;
    Color borderColor;
    float borderRadius;
    bool hasShadow;
} UIPanel;

// Progress bar for time visualization
typedef struct {
    Rectangle bounds;
    float progress; // 0.0 to 1.0
    Color fillColor;
    Color backgroundColor;
    const char* label;
} ProgressBar;

// Enhanced text display
typedef struct {
    Vector2 position;
    const char* text;
    int fontSize;
    Color color;
    bool hasShadow;
    bool isAnimated;
    float animationTime;
} EnhancedText;

// Function declarations
EnhancedButton createButton(Rectangle bounds, const char* text, const char* icon);
void updateButton(EnhancedButton* button);
void drawButton(const EnhancedButton* button);
bool isButtonClicked(const EnhancedButton* button);

UIPanel createPanel(Rectangle bounds, const char* title);
void drawPanel(const UIPanel* panel);

ProgressBar createProgressBar(Rectangle bounds, const char* label);
void updateProgressBar(ProgressBar* bar, float progress);
void drawProgressBar(const ProgressBar* bar);

EnhancedText createText(Vector2 position, const char* text, int fontSize, Color color);
void updateText(EnhancedText* text, float deltaTime);
void drawText(const EnhancedText* text);

// Utility drawing functions
void drawRoundedRectangleWithShadow(Rectangle rect, float roundness, Color color, Color shadowColor);
void drawGradientRectangle(Rectangle rect, Color topColor, Color bottomColor);
void drawIconText(Vector2 position, const char* icon, const char* text, int fontSize, Color color);

// Animation helpers
float easeInOutQuad(float t);
float easeOutBounce(float t);
Color animateColor(Color from, Color to, float t);
