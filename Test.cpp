#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <tuple>
#include <raylib.h>
#include <random>
#include <chrono>

#include "Engines.h"
#include "GameLogic.h"

#ifdef _DEBUG
#include "Debug.h"
#define SHOW_MOUSE_COORDS auto cursor = GetMousePosition(); DrawText(TextFormat("Pos: %.1f, %.1f", cursor.x, cursor.y), 25, 25, 40, LIME)
#define LOG_ALLOCATING_MEMORY std::cout << "Allocating " << s_AllocationMetrics.CurrentMemoryUsage() << "Bytes\n"

#else
#define SHOW_MOUSE_COORDS
#define LOG_ALLOCATING_MEMORY
#endif

int main() {
    //Screen Settings
    int screenWidth = 600, screenHeight = 700;
    InitWindow(screenWidth, screenHeight, "Minesweeper");
    SetTargetFPS(60);

    //Creating Elements
    GAME Game;
    Game.StartGame();

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_R)) {
            Game.ResetGame();
        }

        BeginDrawing();
        ClearBackground({ 176, 184, 196, 255 });

        Game.HoveringCell();
        Game.Draw();
        Game.Refresh();
        
        //Debug stuff
        SHOW_MOUSE_COORDS;
        //LOG_ALLOCATING_MEMORY;

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
