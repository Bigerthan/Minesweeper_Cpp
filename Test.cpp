#include <iostream>
#include <string>
#include <raylib.h>
#include <vector>
#include <utility>
#include <tuple>
#include <random>

#include "Debug.h"
#include "PRNG.h"
#include "GameLogic.h"


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

        //auto cursor = GetMousePosition();
        //DrawText(TextFormat("Pos: %.1f, %.1f", cursor.x, cursor.y), 25, 25, 40, GREEN);
        //
        //std::cout << "Allocating " << s_AllocationMetrics.CurrentMemoryUsage() << "Bytes\n";

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
