#include "ui-components.h"
#include <math.h>
#include <string.h>

EnhancedButton createButton(Rectangle bounds, const char* text, const char* icon) {
    EnhancedButton button = {0};
    button.bounds = bounds;
    button.text = text;
    button.icon = icon;
    button.state = BUTTON_STATE_NORMAL;
    button.isPressed = false;
    button.wasPressed = false;
    button.animationTime = 0.0f;
    button.currentColor = getCurrentTheme().buttonNormal;
    return button;
}

void updateButton(EnhancedButton* button) {
    if (!button) return;
    
    Vector2 mousePos = GetMousePosition();
    bool mouseOver = CheckCollisionPointRec(mousePos, button->bounds);
    bool mousePressed = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    bool mouseClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    
    button->wasPressed = button->isPressed;
    
    // Update button state
    if (mouseOver) {
        if (mousePressed) {
            button->state = BUTTON_STATE_PRESSED;
            button->isPressed = mouseClicked;
        } else {
            button->state = BUTTON_STATE_HOVER;
            button->isPressed = false;
        }
    } else {
        button->state = BUTTON_STATE_NORMAL;
        button->isPressed = false;
    }
    
    // Update animation
    float deltaTime = GetFrameTime();
    button->animationTime += deltaTime * 8.0f; // Animation speed
    
    // Animate color based on state
    ColorTheme theme = getCurrentTheme();
    Color targetColor;
    
    switch (button->state) {
        case BUTTON_STATE_HOVER:
            targetColor = theme.buttonHover;
            break;
        case BUTTON_STATE_PRESSED:
            targetColor = theme.buttonPressed;
            break;
        case BUTTON_STATE_DISABLED:
            targetColor = darkenColor(theme.buttonNormal, 0.5f);
            break;
        default:
            targetColor = theme.buttonNormal;
            break;
    }
    
    // Smooth color transition
    float t = fminf(button->animationTime, 1.0f);
    button->currentColor = animateColor(button->currentColor, targetColor, t * 0.2f);
}

void drawButton(const EnhancedButton* button) {
    if (!button) return;
    
    ColorTheme theme = getCurrentTheme();
    
    // Draw shadow
    Rectangle shadowRect = {
        button->bounds.x + 2,
        button->bounds.y + 2,
        button->bounds.width,
        button->bounds.height
    };
    DrawRectangleRounded(shadowRect, 0.3f, 8, (Color){0, 0, 0, 50});
    
    // Draw button background with gradient effect
    Color topColor = lightenColor(button->currentColor, 0.1f);
    Color bottomColor = darkenColor(button->currentColor, 0.1f);
    
    if (button->state == BUTTON_STATE_PRESSED) {
        // Swap colors for pressed effect
        Color temp = topColor;
        topColor = bottomColor;
        bottomColor = temp;
    }
    
    drawGradientRectangle(button->bounds, topColor, bottomColor);
    
    // Draw border
    DrawRectangleLinesEx(button->bounds, 2, darkenColor(button->currentColor, 0.3f));
    
    // Calculate text position with better font size
    int fontSize = 18;
    int iconSize = 20;
    int textWidth = MeasureText(button->text, fontSize);
    int iconWidth = button->icon ? MeasureText(button->icon, iconSize) : 0;
    int totalWidth = iconWidth + (iconWidth > 0 ? 10 : 0) + textWidth;

    Vector2 textPos = {
        button->bounds.x + (button->bounds.width - totalWidth) / 2,
        button->bounds.y + (button->bounds.height - fontSize) / 2
    };

    // Draw icon and text with better spacing
    if (button->icon) {
        DrawText(button->icon, textPos.x, textPos.y - 1, iconSize, theme.buttonText);
        textPos.x += iconWidth + 10;
    }

    DrawText(button->text, textPos.x, textPos.y, fontSize, theme.buttonText);
}

bool isButtonClicked(const EnhancedButton* button) {
    return button && button->isPressed && !button->wasPressed;
}

UIPanel createPanel(Rectangle bounds, const char* title) {
    UIPanel panel = {0};
    panel.bounds = bounds;
    panel.title = title;
    panel.backgroundColor = getCurrentTheme().panelBackground;
    panel.borderColor = getCurrentTheme().borderColor;
    panel.borderRadius = 8.0f;
    panel.hasShadow = true;
    return panel;
}

void drawPanel(const UIPanel* panel) {
    if (!panel) return;
    
    // Draw shadow
    if (panel->hasShadow) {
        Rectangle shadowRect = {
            panel->bounds.x + 4,
            panel->bounds.y + 4,
            panel->bounds.width,
            panel->bounds.height
        };
        DrawRectangleRounded(shadowRect, panel->borderRadius / panel->bounds.width, 8, (Color){0, 0, 0, 30});
    }
    
    // Draw panel background
    DrawRectangleRounded(panel->bounds, panel->borderRadius / panel->bounds.width, 8, panel->backgroundColor);
    
    // Draw border
    DrawRectangleLinesEx(panel->bounds, 2, panel->borderColor);
    
    // Draw title if provided
    if (panel->title) {
        ColorTheme theme = getCurrentTheme();
        int titleFontSize = 20;
        int titleWidth = MeasureText(panel->title, titleFontSize);
        Vector2 titlePos = {
            panel->bounds.x + (panel->bounds.width - titleWidth) / 2,
            panel->bounds.y + 12
        };

        // Title background with better styling
        Rectangle titleBg = {
            titlePos.x - 10,
            titlePos.y - 3,
            titleWidth + 20,
            titleFontSize + 6
        };
        DrawRectangleRounded(titleBg, 0.4f, 6, lightenColor(panel->backgroundColor, 0.15f));

        // Add subtle border to title
        DrawRectangleLinesEx(titleBg, 1, theme.borderColor);

        DrawText(panel->title, titlePos.x, titlePos.y, titleFontSize, theme.primaryText);
    }
}

ProgressBar createProgressBar(Rectangle bounds, const char* label) {
    ProgressBar bar = {0};
    bar.bounds = bounds;
    bar.progress = 0.0f;
    bar.fillColor = getCurrentTheme().successText;
    bar.backgroundColor = darkenColor(getCurrentTheme().panelBackground, 0.2f);
    bar.label = label;
    return bar;
}

void updateProgressBar(ProgressBar* bar, float progress) {
    if (!bar) return;
    bar->progress = fmaxf(0.0f, fminf(1.0f, progress));
}

void drawProgressBar(const ProgressBar* bar) {
    if (!bar) return;
    
    // Draw background
    DrawRectangleRounded(bar->bounds, 0.5f, 4, bar->backgroundColor);
    
    // Draw fill
    Rectangle fillRect = {
        bar->bounds.x + 2,
        bar->bounds.y + 2,
        (bar->bounds.width - 4) * bar->progress,
        bar->bounds.height - 4
    };
    
    if (fillRect.width > 0) {
        DrawRectangleRounded(fillRect, 0.5f, 4, bar->fillColor);
    }
    
    // Draw border
    DrawRectangleLinesEx(bar->bounds, 1, getCurrentTheme().borderColor);
    
    // Draw label
    if (bar->label) {
        int labelWidth = MeasureText(bar->label, 14);
        Vector2 labelPos = {
            bar->bounds.x + (bar->bounds.width - labelWidth) / 2,
            bar->bounds.y + (bar->bounds.height - 14) / 2
        };
        DrawText(bar->label, labelPos.x, labelPos.y, 14, getCurrentTheme().primaryText);
    }
}

EnhancedText createText(Vector2 position, const char* text, int fontSize, Color color) {
    EnhancedText enhancedText = {0};
    enhancedText.position = position;
    enhancedText.text = text;
    enhancedText.fontSize = fontSize;
    enhancedText.color = color;
    enhancedText.hasShadow = false;
    enhancedText.isAnimated = false;
    enhancedText.animationTime = 0.0f;
    return enhancedText;
}

void updateText(EnhancedText* text, float deltaTime) {
    if (!text || !text->isAnimated) return;
    text->animationTime += deltaTime;
}

void drawText(const EnhancedText* text) {
    if (!text || !text->text) return;
    
    Vector2 pos = text->position;
    
    // Apply animation offset if animated
    if (text->isAnimated) {
        float bounce = sinf(text->animationTime * 3.0f) * 2.0f;
        pos.y += bounce;
    }
    
    // Draw shadow if enabled
    if (text->hasShadow) {
        Vector2 shadowPos = {pos.x + 1, pos.y + 1};
        DrawText(text->text, shadowPos.x, shadowPos.y, text->fontSize, (Color){0, 0, 0, 100});
    }
    
    // Draw main text
    DrawText(text->text, pos.x, pos.y, text->fontSize, text->color);
}

// Utility functions
void drawRoundedRectangleWithShadow(Rectangle rect, float roundness, Color color, Color shadowColor) {
    Rectangle shadowRect = {rect.x + 2, rect.y + 2, rect.width, rect.height};
    DrawRectangleRounded(shadowRect, roundness, 8, shadowColor);
    DrawRectangleRounded(rect, roundness, 8, color);
}

void drawGradientRectangle(Rectangle rect, Color topColor, Color bottomColor) {
    // Simple gradient simulation with multiple rectangles
    int steps = 10;
    float stepHeight = rect.height / steps;
    
    for (int i = 0; i < steps; i++) {
        float t = (float)i / (steps - 1);
        Color stepColor = animateColor(topColor, bottomColor, t);
        
        Rectangle stepRect = {
            rect.x,
            rect.y + i * stepHeight,
            rect.width,
            stepHeight + 1 // +1 to avoid gaps
        };
        
        DrawRectangleRec(stepRect, stepColor);
    }
}

void drawIconText(Vector2 position, const char* icon, const char* text, int fontSize, Color color) {
    if (icon) {
        DrawText(icon, position.x, position.y, fontSize + 4, color);
        position.x += MeasureText(icon, fontSize + 4) + 8;
    }
    if (text) {
        DrawText(text, position.x, position.y, fontSize, color);
    }
}

// Animation helpers
float easeInOutQuad(float t) {
    return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
}

float easeOutBounce(float t) {
    if (t < 1.0f / 2.75f) {
        return 7.5625f * t * t;
    } else if (t < 2.0f / 2.75f) {
        t -= 1.5f / 2.75f;
        return 7.5625f * t * t + 0.75f;
    } else if (t < 2.5f / 2.75f) {
        t -= 2.25f / 2.75f;
        return 7.5625f * t * t + 0.9375f;
    } else {
        t -= 2.625f / 2.75f;
        return 7.5625f * t * t + 0.984375f;
    }
}

Color animateColor(Color from, Color to, float t) {
    t = fmaxf(0.0f, fminf(1.0f, t));
    return (Color){
        (unsigned char)(from.r + (to.r - from.r) * t),
        (unsigned char)(from.g + (to.g - from.g) * t),
        (unsigned char)(from.b + (to.b - from.b) * t),
        (unsigned char)(from.a + (to.a - from.a) * t)
    };
}
