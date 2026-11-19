#include <iostream>
#include <string>
#include <raylib.h>
#include <vector>
#include <utility>
#include <tuple>

struct AllocationMetrics
{
    uint32_t TotalAllocated = 0;
    uint32_t TotalFreed = 0;

    uint32_t CurrentMemoryUsage() { return TotalAllocated - TotalFreed; }
};
static AllocationMetrics s_AllocationMetrics;

void* operator new(size_t size) {
    std::cout << "Allocated " << size << "bytes.\n";
    s_AllocationMetrics.TotalAllocated += size;
    return malloc(size);
}
void operator delete(void* memory, size_t size) {
    std::cout << "Freed " << size << "bytes.\n";
    s_AllocationMetrics.TotalFreed += size;
    free(memory);
}

class Board {
private:
    Color m_color = RAYWHITE;
    int m_startingX = 100, m_startingY = 200;
    int m_width = 400, m_height = 400;

public:
    Board()
    {
        draw();
        std::cout << "The Board is created!\n";
    }

    void draw() const {
        DrawRectangleLines(m_startingX - 1, m_startingY - 1, m_width + 2, m_height + 2, BLACK); //Board Border
        DrawRectangle(m_startingX, m_startingY, m_width, m_height, m_color); // Board
    }

    std::tuple<int,int,int,int> getAreaCoord() const {
        return std::tuple<int,int,int,int>(m_startingX, m_startingY, m_startingX + m_width, m_startingY + m_height);
    }
};

class Cell{
private:
    Color m_color = BLACK;
    int m_width = 50, m_height = 50;
    int m_column, m_row;
    bool isBomb = false, isFlagged = false, isOpened = false;

public:
    Cell(int column, int row) :
        m_column(column), m_row(row)
    {
        this->draw();
        std::cout << "Created cell " << m_column << "_" << m_row << " !\n";
    }

    void draw() const {
        DrawRectangleLines(100 + m_width * m_column, 200 + m_height * m_row, m_width, m_height, m_color);

        if (this->isFlagged)
            DrawText("!", 100 - 2 + (m_column + 0.5) * m_width, 200 + 5 + m_row * m_height, 50, RED); // -2 and +5 = to center 
    }

    void makeFlagged() {
        if (this->isOpened)
            return;

        if (this->isFlagged) {
            this->isFlagged = false;
            return;
        }

        DrawText("!", 100 - 2 + (m_column + 0.5) * m_width, 200 + 5 + m_row * m_height, 50, RED); // -2 and +5 = to center 
        this->isFlagged = true;
    }
};

std::tuple<bool,int,int> GetTargetCell(Board& BOARD, std::vector<Cell>& Grid) {
    auto coord = GetMousePosition();
    auto boardArena = BOARD.getAreaCoord();

    if (get<0>(boardArena) < coord.x && get<1>(boardArena) < coord.y &&
        get<2>(boardArena) > coord.x && get<3>(boardArena) > coord.y)
    {
        int targetCell_c = (coord.x - get<0>(boardArena)) / 50;    //50 = cell width
        int targetCell_r = (coord.y - get<1>(boardArena)) / 50;    //50 = cell height

        return std::tuple<bool,int,int> (true, targetCell_r, targetCell_c);
    }

    else {return std::tuple<bool, int, int>(false, -1, -1);}
}

int main() {
    //Screen Settings
    int screenWidth = 600, screenHeight = 700;
    InitWindow(screenWidth, screenHeight, "Minesweeper");
    SetTargetFPS(60);

    //Game Settings
    int const column_num = 8, row_num = 8;

    //Creating Elements
    Board BOARD{};
    std::vector<Cell> Grid;
    Grid.reserve(row_num * column_num);
    
    for (int i = 0; i < row_num; i++){
        for (int j = 0; j < column_num; j++){
            Grid.emplace_back(j, i);
        }
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GRAY);

        BOARD.draw();
        
        for (const auto& cell: Grid) {
            cell.draw();
        }

        //std::cout << "Allocating " << s_AllocationMetrics.CurrentMemoryUsage() << "Bytes\n";
        
        auto [isHit, targetCell_r, targetCell_c] = GetTargetCell(BOARD, Grid);
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
