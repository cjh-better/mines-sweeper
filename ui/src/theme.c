#include "theme.h"

ColorTheme currentTheme;

ColorTheme createModernTheme(void) {
    ColorTheme theme = {0};
    
    theme.background = (Color){45, 52, 64, 255};
    theme.panelBackground = (Color){59, 66, 82, 255};
    theme.cellBackground = (Color){136, 192, 208, 255};
    theme.cellHover = (Color){129, 161, 193, 255};
    theme.cellPressed = (Color){94, 129, 172, 255};
    
    theme.buttonNormal = (Color){88, 101, 242, 255};
    theme.buttonHover = (Color){109, 120, 245, 255};
    theme.buttonPressed = (Color){67, 80, 239, 255};
    theme.buttonText = (Color){255, 255, 255, 255};
    
    theme.mineColor = (Color){191, 97, 106, 255};
    theme.flagColor = (Color){208, 135, 112, 255};
    theme.revealedCell = (Color){236, 239, 244, 255};
    theme.hiddenCell = (Color){136, 192, 208, 255};
    theme.borderColor = (Color){76, 86, 106, 255};
    
    theme.primaryText = (Color){236, 239, 244, 255};
    theme.secondaryText = (Color){129, 161, 193, 255};
    theme.successText = (Color){163, 190, 140, 255};
    theme.warningText = (Color){235, 203, 139, 255};
    theme.errorText = (Color){191, 97, 106, 255};
    
    theme.numbers[0] = (Color){76, 86, 106, 255};
    theme.numbers[1] = (Color){129, 161, 193, 255};
    theme.numbers[2] = (Color){163, 190, 140, 255};
    theme.numbers[3] = (Color){191, 97, 106, 255};
    theme.numbers[4] = (Color){180, 142, 173, 255};
    theme.numbers[5] = (Color){208, 135, 112, 255};
    theme.numbers[6] = (Color){235, 203, 139, 255};
    theme.numbers[7] = (Color){143, 188, 187, 255};
    theme.numbers[8] = (Color){136, 192, 208, 255};
    
    theme.playingStatus = (Color){163, 190, 140, 255};
    theme.pausedStatus = (Color){235, 203, 139, 255};
    theme.gameOverStatus = (Color){191, 97, 106, 255};
    theme.victoryStatus = (Color){208, 135, 112, 255};
    
    return theme;
}

// Removed unused theme implementations to simplify code

void setTheme(ColorTheme theme) {
    currentTheme = theme;
}

ColorTheme getCurrentTheme(void) {
    return currentTheme;
}

Color lightenColor(Color color, float factor) {
    return (Color){
        (unsigned char)(color.r + (255 - color.r) * factor),
        (unsigned char)(color.g + (255 - color.g) * factor),
        (unsigned char)(color.b + (255 - color.b) * factor),
        color.a
    };
}

Color darkenColor(Color color, float factor) {
    return (Color){
        (unsigned char)(color.r * (1.0f - factor)),
        (unsigned char)(color.g * (1.0f - factor)),
        (unsigned char)(color.b * (1.0f - factor)),
        color.a
    };
}

Color blendColors(Color color1, Color color2, float factor) {
    return (Color){
        (unsigned char)(color1.r * (1.0f - factor) + color2.r * factor),
        (unsigned char)(color1.g * (1.0f - factor) + color2.g * factor),
        (unsigned char)(color1.b * (1.0f - factor) + color2.b * factor),
        (unsigned char)(color1.a * (1.0f - factor) + color2.a * factor)
    };
}
