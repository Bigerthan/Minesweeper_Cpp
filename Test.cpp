#include <iostream>
#include <string>
#include <raylib.h>
#include <vector>
#include <utility>
#include <tuple>

#include "GameLogic.h"
#include "Debug.h"

static AllocationMetrics s_AllocationMetrics; //Allocaion info

int main() {
    //Screen Settings
    int screenWidth = 600, screenHeight = 700;
    InitWindow(screenWidth, screenHeight, "Minesweeper");
    SetTargetFPS(60);

    //Game Settings
    

    //Creating Elements
    BOARD Board{};
    GRID Grid;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GRAY);

        Board.draw();
        
        for (const auto& cell: Grid) {
            cell.draw();
        }

        //std::cout << "Allocating " << s_AllocationMetrics.CurrentMemoryUsage() << "Bytes\n";
        
        auto [isHit, targetCell_r, targetCell_c] = GetTargetCell(Board, Grid);
        if (isHit)
        {
            Cell& targetCell = Grid.at(targetCell_r * 8 + targetCell_c);  //8 = row_num

            std::cout << "Currently hovering at Cell " << targetCell_r << "_" << targetCell_c << "\n";

            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
                targetCell.makeFlagged();
        }

        auto cursor = GetMousePosition();
        DrawText(TextFormat("Pos: %.1f, %.1f", cursor.x, cursor.y), 25, 25, 40, GREEN);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}