#pragma once

#include "raylib.h"

// Modern color palette
typedef struct {
    // Background colors
    Color background;
    Color panelBackground;
    Color cellBackground;
    Color cellHover;
    Color cellPressed;
    
    // Button colors
    Color buttonNormal;
    Color buttonHover;
    Color buttonPressed;
    Color buttonText;
    
    // Game colors
    Color mineColor;
    Color flagColor;
    Color revealedCell;
    Color hiddenCell;
    Color borderColor;
    
    // Text colors
    Color primaryText;
    Color secondaryText;
    Color successText;
    Color warningText;
    Color errorText;
    
    // Number colors for mine counts
    Color numbers[9]; // 0-8 surrounding mines
    
    // Status colors
    Color playingStatus;
    Color pausedStatus;
    Color gameOverStatus;
    Color victoryStatus;
} ColorTheme;

// Theme functions
ColorTheme createModernTheme(void);

// Current active theme
extern ColorTheme currentTheme;

// Theme management
void setTheme(ColorTheme theme);
ColorTheme getCurrentTheme(void);

// Utility functions
Color lightenColor(Color color, float factor);
Color darkenColor(Color color, float factor);
Color blendColors(Color color1, Color color2, float factor);
