# Try to find raylib using pkg-config first
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(raylib QUIET raylib)
endif()

# If not found via pkg-config, try find_package
if(NOT raylib_FOUND)
    find_package(raylib QUIET)
endif()

# Try to find raylib in common installation paths
if(NOT raylib_FOUND)
    # Common raylib installation paths
    set(RAYLIB_SEARCH_PATHS
        "D:/site/raylib-5.0_win64_mingw-w64/raylib-5.0_win64_mingw-w64"
        "D:/site/raylib-5.0_win64_mingw-w64"
        "C:/raylib"
        "C:/Program Files/raylib"
        "C:/msys64/ucrt64"
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/raylib"
    )

    foreach(SEARCH_PATH ${RAYLIB_SEARCH_PATHS})
        if(EXISTS "${SEARCH_PATH}/include/raylib.h")
            message(STATUS "Found raylib at: ${SEARCH_PATH}")
            add_library(raylib STATIC IMPORTED)
            set_target_properties(raylib PROPERTIES
                IMPORTED_LOCATION "${SEARCH_PATH}/lib/libraylib.a"
                INTERFACE_INCLUDE_DIRECTORIES "${SEARCH_PATH}/include"
                INTERFACE_LINK_LIBRARIES "winmm;gdi32;opengl32"
            )
            set(raylib_FOUND TRUE)
            break()
        endif()
    endforeach()
endif()

# If still not found, try to use FetchContent to download raylib
if(NOT raylib_FOUND)
    message(STATUS "raylib not found locally, attempting to download...")

    include(FetchContent)

    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.0
        GIT_SHALLOW TRUE
    )

    # Set raylib options
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(raylib)

    if(raylib_POPULATED)
        message(STATUS "raylib downloaded and configured successfully")
    else()
        message(WARNING "Failed to download raylib, falling back to stub library")

        # Create raylib stub directory and files
        file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/raylib_stub)

        # Create a minimal raylib.h stub
        file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/raylib_stub/raylib.h "
#ifndef RAYLIB_H
#define RAYLIB_H

#include <stdbool.h>

typedef struct Color {
    unsigned char r, g, b, a;
} Color;

typedef struct Rectangle {
    float x, y, width, height;
} Rectangle;

typedef struct Vector2 {
    float x, y;
} Vector2;

// Color constants
#define BLANK (Color){0, 0, 0, 0}
#define WHITE (Color){255, 255, 255, 255}
#define BLACK (Color){0, 0, 0, 255}
#define RED (Color){255, 0, 0, 255}
#define GREEN (Color){0, 255, 0, 255}
#define BLUE (Color){0, 0, 255, 255}
#define YELLOW (Color){255, 255, 0, 255}
#define ORANGE (Color){255, 165, 0, 255}
#define PINK (Color){255, 192, 203, 255}
#define PURPLE (Color){128, 0, 128, 255}
#define SKYBLUE (Color){135, 206, 235, 255}
#define GRAY (Color){128, 128, 128, 255}

// Key constants
#define KEY_R 82

// Mouse button constants
#define MOUSE_LEFT_BUTTON 0
#define MOUSE_RIGHT_BUTTON 1

// Function declarations
void InitWindow(int width, int height, const char *title);
void CloseWindow(void);
bool WindowShouldClose(void);
void SetTargetFPS(int fps);
void BeginDrawing(void);
void EndDrawing(void);
void ClearBackground(Color color);
bool IsKeyDown(int key);
bool IsKeyPressed(int key);
Vector2 GetMousePosition(void);
bool IsMouseButtonDown(int button);
bool IsMouseButtonPressed(int button);
bool IsMouseButtonReleased(int button);
bool CheckCollisionPointRec(Vector2 point, Rectangle rec);
void DrawRectangle(int posX, int posY, int width, int height, Color color);
void DrawRectangleRec(Rectangle rec, Color color);
void DrawRectangleLinesEx(Rectangle rec, float lineThick, Color color);
void DrawRectangleRounded(Rectangle rec, float roundness, int segments, Color color);
void DrawText(const char *text, int posX, int posY, int fontSize, Color color);
int MeasureText(const char *text, int fontSize);
float GetFrameTime(void);

#endif
")

        # Create a minimal raylib.c stub
        file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/raylib_stub/raylib.c "
#include \"raylib.h\"
#include <stdio.h>
#include <string.h>

static bool windowShouldClose = false;
static int frameCount = 0;

void InitWindow(int width, int height, const char *title) {
    printf(\"InitWindow: %dx%d - %s\\n\", width, height, title);
}

void CloseWindow(void) {
    printf(\"CloseWindow\\n\");
    windowShouldClose = true;
}

bool WindowShouldClose(void) {
    frameCount++;
    if (frameCount > 100) { // Auto close after 100 frames for demo
        return true;
    }
    return windowShouldClose;
}

void SetTargetFPS(int fps) {
    printf(\"SetTargetFPS: %d\\n\", fps);
}

void BeginDrawing(void) {
    printf(\"Frame %d: BeginDrawing\\n\", frameCount);
}

void EndDrawing(void) {
    printf(\"Frame %d: EndDrawing\\n\", frameCount);
}

void ClearBackground(Color color) {
    printf(\"ClearBackground: R=%d G=%d B=%d A=%d\\n\", color.r, color.g, color.b, color.a);
}

bool IsKeyDown(int key) {
    return false;
}

bool IsKeyPressed(int key) {
    return false;
}

Vector2 GetMousePosition(void) {
    return (Vector2){100.0f, 100.0f}; // Simulate mouse position
}

bool IsMouseButtonDown(int button) {
    return false;
}

bool IsMouseButtonPressed(int button) {
    return false;
}

bool IsMouseButtonReleased(int button) {
    return false;
}

bool CheckCollisionPointRec(Vector2 point, Rectangle rec) {
    return (point.x >= rec.x) && (point.x <= (rec.x + rec.width)) &&
           (point.y >= rec.y) && (point.y <= (rec.y + rec.height));
}

void DrawRectangle(int posX, int posY, int width, int height, Color color) {
    printf(\"DrawRectangle: (%d,%d,%d,%d) Color(%d,%d,%d,%d)\\n\",
           posX, posY, width, height, color.r, color.g, color.b, color.a);
}

void DrawRectangleRec(Rectangle rec, Color color) {
    printf(\"DrawRectangleRec: (%.1f,%.1f,%.1f,%.1f) Color(%d,%d,%d,%d)\\n\",
           rec.x, rec.y, rec.width, rec.height, color.r, color.g, color.b, color.a);
}

void DrawRectangleLinesEx(Rectangle rec, float lineThick, Color color) {
    printf(\"DrawRectangleLinesEx: (%.1f,%.1f,%.1f,%.1f) thick=%.1f Color(%d,%d,%d,%d)\\n\",
           rec.x, rec.y, rec.width, rec.height, lineThick, color.r, color.g, color.b, color.a);
}

void DrawRectangleRounded(Rectangle rec, float roundness, int segments, Color color) {
    printf(\"DrawRectangleRounded: (%.1f,%.1f,%.1f,%.1f) Color(%d,%d,%d,%d)\\n\",
           rec.x, rec.y, rec.width, rec.height, color.r, color.g, color.b, color.a);
}

void DrawText(const char *text, int posX, int posY, int fontSize, Color color) {
    printf(\"DrawText: '%s' at (%d,%d) size=%d Color(%d,%d,%d,%d)\\n\",
           text, posX, posY, fontSize, color.r, color.g, color.b, color.a);
}

int MeasureText(const char *text, int fontSize) {
    return strlen(text) * fontSize / 2; // Simple approximation
}

float GetFrameTime(void) {
    return 0.016f; // 60 FPS approximation
}
")

        # Create raylib stub library
        add_library(raylib STATIC ${CMAKE_CURRENT_BINARY_DIR}/raylib_stub/raylib.c)
        target_include_directories(raylib PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/raylib_stub)
    endif()
endif()

#ui-lib
file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

add_library(bitsweeper-ui STATIC
    ${CORE_SOURCES}
)

target_link_libraries(bitsweeper-ui PRIVATE
	raylib
	bitsweeper-core
)
 
target_include_directories(bitsweeper-ui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

file(GLOB_RECURSE INCLUDE_DIRS LIST_DIRECTORIES true "${CMAKE_CURRENT_SOURCE_DIR}/src/*")

foreach(dir ${INCLUDE_DIRS})
	if(IS_DIRECTORY ${dir})
		target_include_directories(bitsweeper-ui PUBLIC ${dir})
	endif()
endforeach()

#game
add_executable(bitsweeper-game
	${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
)

target_link_libraries(bitsweeper-game PRIVATE
	bitsweeper-ui
	bitsweeper-core
	raylib
)