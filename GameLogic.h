#pragma once

class Game
{
protected:
    int cellSize = 50;
    size_t columnNum = 8, rowNum = 8;

public:
	Game();
	~Game();

    std::tuple<bool, int, int> GetTargetCell(Board& BOARD, std::vector<Cell>& Grid) {
        auto coord = GetMousePosition();
        auto boardArena = BOARD.getAreaCoord();

        if (get<0>(boardArena) < coord.x && get<1>(boardArena) < coord.y &&
            get<2>(boardArena) > coord.x && get<3>(boardArena) > coord.y)
        {
            int targetCell_c = (coord.x - get<0>(boardArena)) / 50;    //50 = cell width
            int targetCell_r = (coord.y - get<1>(boardArena)) / 50;    //50 = cell height

            return std::tuple<bool, int, int>(true, targetCell_r, targetCell_c);
        }

        else { return std::tuple<bool, int, int>(false, -1, -1); }
    }
};

class BOARD : public Game{
private:
    Color m_color = RAYWHITE;
    int m_startingX = 100, m_startingY = 200;
    int m_width = 400, m_height = 400;

public:
    BOARD()
    {
        draw();
        std::cout << "The Board is created!\n";
    }

    void draw() const {
        DrawRectangleLines(m_startingX - 1, m_startingY - 1, m_width + 2, m_height + 2, BLACK); //Board Border
        DrawRectangle(m_startingX, m_startingY, m_width, m_height, m_color); // Board
    }

    std::tuple<int, int, int, int> getAreaCoord() const {
        return std::tuple<int, int, int, int>(m_startingX, m_startingY, m_startingX + m_width, m_startingY + m_height);
    }
};

class CELL : public Game{
private:
    Color m_color = BLACK;
    int m_width = 50, m_height = 50;
    int m_column, m_row;
    bool isBomb = false, isFlagged = false, isOpened = false;

public:
    CELL(int column, int row) :
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

class GRID : public Game {
private:
    std::vector<CELL> Grid;
public:
    GRID() {
        Grid.reserve(rowNum * columnNum);
        
        for (int r = 0; r < rowNum; r++) {
            for (int c = 0; c < columnNum; c++) {
                Grid.emplace_back(c, r);
            }
        }
    }

    std::vector<CELL> GetGridElement() {
        return Grid;
    }
};