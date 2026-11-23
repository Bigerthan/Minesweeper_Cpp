#pragma once
#include "PRNG.h"

//class RESET_BTN {
//private:
//    Color m_boxColor = RED, m_textColor = WHITE;
//    int m_textFontSize = 25;
//
//    std::pair<int, int> m_startingCoords = { 50, 175 };
//    int m_width = 75, m_height = 25;
//
//public:
//    RESET_BTN() {
//        DrawRectangle(m_startingCoords.first, m_startingCoords.second, m_width, m_height, m_boxColor);
//        DrawText("Reset", m_startingCoords.first, m_startingCoords.second, m_textFontSize, m_textColor);
//    }
//};
//
//class GAMEMODE_SELECTOR {
//private:
//    Color m_boxColor = GRAY, m_textColor = WHITE;
//    int m_textFontSize = 25;
//
//    std::pair<int, int> m_startingCoords = { 150, 175 };
//    int m_width = 45, m_height = 25;
//
//public:
//    GAMEMODE_SELECTOR() {
//        DrawRectangle(m_startingCoords.first, m_startingCoords.second, m_width, m_height, m_boxColor);
//        DrawText("Reset", m_startingCoords.first, m_startingCoords.second, m_textFontSize, m_textColor);
//    }
//};
//
//class EASY_MODE : public GAMEMODE_SELECTOR {
//private:
//
//public:
//    EASY_MODE() {
//
//    }
//};

class BOARD {
private:
    Color m_color = { 148, 158, 173, 255 };
    int m_startingX, m_startingY, m_finishX, m_finishY;
    int m_width, m_height;

public:
    BOARD(int startingX, int startingY, int finishX, int finishY):
        m_startingX(startingX), m_startingY(startingY), m_finishX(finishX), m_finishY(finishY)
    {
        m_width = m_finishX - m_startingX;
        m_height = m_finishY - m_startingY;
        draw();
        std::cout << "The Board is created!\n";
    }

    void draw() const {
        DrawRectangleLines(m_startingX - 1, m_startingY - 1, m_width + 2, m_height + 2, BLACK); //Board Border
        DrawRectangle(m_startingX, m_startingY, m_width, m_height, m_color); // Board
    }
};

class CELL{
private:
    Color m_borderColor = BLACK, m_hoverColor = { 168, 178, 194, 255 }, m_openColor = { 208, 214, 224, 255 }; //cell color = board color
    int m_size, m_startX, m_startY;
    int m_column, m_row;

    bool isBomb = false, isFlagged = false, isOpened = false, isHovering = false;
    int m_value = 0;

public:
    CELL(int column, int row, int size, int startX, int startY):
        m_column(column), m_row(row), m_size(size), m_startX(startX), m_startY(startY)
    {
        draw();
        std::cout << "Created cell " << m_column << "_" << m_row << " !\n";
    }

    void draw() const {
        DrawRectangleLines((m_startX + m_size * m_column), (m_startY + m_size * m_row), m_size, m_size, m_borderColor);

        if (isHovering) {
            DrawRectangle((m_startX + m_size * m_column) + 1, (m_startY + m_size * m_row) + 1, m_size - 2, m_size - 2, m_hoverColor);
        }

        if (isFlagged)
            DrawText("!", (m_startX - 2 + (m_column + 0.5) * m_size), (m_startY + 5 + m_row * m_size), 50, RED); // -2 and +5 = to center
        
        if (isOpened) {
            DrawRectangle((m_startX + m_size * m_column) + 1, (m_startY + m_size * m_row) + 1, m_size - 2, m_size - 2, m_openColor);
            const char* text = !m_value ? TextFormat(" ") : m_value == -1 ? TextFormat("%d", m_value) : TextFormat(" %d", m_value);
            DrawText(text, (m_startX + (m_column + 0.15) * m_size), (m_startY + 7.5 + m_row * m_size), 35, GREEN);
        }
    }

    void Flag() {
        if (isOpened)
            return;

        if (isFlagged) {
            isFlagged = false;
            return;
        }

        DrawText("!", 100 - 2 + (m_column + 0.5) * m_size, 200 + 5 + m_row * m_size, 50, RED); // -2 and +5 = to center 
        isFlagged = true;
    }

    void Open() {
        if (isOpened || isFlagged)
            return;

        isOpened = true;
        std::cout << " OPEN ONE! \n";
    }

    void OpenAll() {
        if (isFlagged)
            return;
        std::cout << " OPEN ALL! \n";
    }

    bool MakeBomb() {
        if (isBomb) 
            return 0;

        isBomb = true;
        m_value = -1;
        return 1;
    }

    void Hovering(bool enable) {
        if (enable)
            isHovering = true;
        else
            isHovering = false;
    }

    void setToDefault() {
        isBomb = false; isFlagged = false; isOpened = false; isHovering = false; m_value = 0;
    }

    void setValue(int value) {
        m_value = value;
    }

    bool CheckBomb() const {
        return isBomb;
    }
};

class GRID {
private:
    std::vector<CELL> Grid;
public:
    GRID(int rowNum, int columnNum, int cellSize, int cellstartX, int cellstartY) {
        Grid.reserve(rowNum * columnNum);
        
        for (int r = 0; r < rowNum; r++) {
            for (int c = 0; c < columnNum; c++) {
                Grid.emplace_back(c, r, cellSize, cellstartX, cellstartY);
            }
        }
    }

    void draw() const {
        for (const auto& cell : Grid) {
            cell.draw();
        }
    }

    CELL& getCellAt(int index) { //index = row * colNum + col
        return Grid.at(index);
    }

    CELL& getCellAt(int row, int col, int colNum) {
        return Grid.at(row * colNum + col);
    }

    void ResetGrid() {
        for (auto& cell : Grid) {
            cell.setToDefault();
        }
    }

    void SetCellValues(int rowNum, int colNum) {
        for (int r = 0; r < rowNum; r++) {
            for (int c = 0; c < colNum; c++) // EVERY CELL
            {
                int value = 0;

                if (getCellAt(r, c, colNum).CheckBomb()) {
                    value = -1;
                }
                else {
                    for (int i = -1; i <= 1; i++) {
                        for (int j = -1; j <= 1; j++)
                        {
                            if (r + i >= 0 && r + i < rowNum && //out of border
                                c + j >= 0 && c + j < colNum && //out of border
                                !(i == 0 && j == 0)) //if the cell is itself
                            {
                                if (getCellAt(r + i, c + j, colNum).CheckBomb()) {
                                    value += 1;
                                }
                            }
                        }
                    }
                }
                getCellAt(r, c, colNum).setValue(value);
            }
        }
    }
};

class GAME {
private:
    PRNG_Engine Random;

    //Cell stuff
    const int cellSize = 50;
    int gridCols = 8, gridRows = 8, numOfBombs = 10, currentBombs = 0;
    int targetCell_r = -1, targetCell_c = -1;

    //Border stuff
    std::pair<int, int> borderStartCoords = { 100, 200 };
    std::pair<int, int> borderFinishCoords = { borderStartCoords.first + cellSize * gridCols ,borderStartCoords.second + cellSize * gridRows };

    //Elements
    BOARD m_Board;
    GRID m_Grid;
    //RESET_BTN m_ResetBtn;
    //GAMEMODE_SELECTOR m_GamemodeSelector;

    //Other
    bool m_isGameFinish = 0; // type= 0: Normal, 1: GameEnd
    bool m_gameFinishType;  // type 0: Dead, 1: Won

    enum MOUSE_BTN
    {
        NONE = 0, OUT_MOUSE_BTN_R, OUT_MOUSE_BTN_L, OUT_MOUSE_BUT_M, OUT_MOUSE_BTN_B
    };
    int MouseEventListener() {
        bool RandL = (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) ||
                     (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT));

        if (RandL) { return OUT_MOUSE_BTN_B; }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) { return OUT_MOUSE_BTN_R; }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { return OUT_MOUSE_BTN_L; }
        if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) { return OUT_MOUSE_BUT_M; }
        return NONE;
    }
    
    void TitleDraw() const {
        DrawText("MINESWEEPER", GetScreenWidth() / 6 + 10, borderStartCoords.second / 2, 50, { 38, 46, 61, 255 });
    }
    void GameFinish_Draw() const {
        const char* bigText;
        Color boxColor;
        if (m_gameFinishType) { // WON
            bigText = "YOU  WON!";
            boxColor = LIME;
        }
        else {
            bigText = "YOU LOSE!";
            boxColor = RED;
        }
        DrawRectangle((borderStartCoords.first * 3 / 4), (borderStartCoords.second * 11 / 8), (cellSize * gridCols + borderStartCoords.first * 2 / 4), (cellSize * gridRows * 5 / 8), boxColor);
        DrawText(bigText, borderStartCoords.first * 9 / 8, borderStartCoords.second * 7 / 4, 75, BLACK);
        DrawText("Press 'R' to restart...", borderStartCoords.first * 13 / 8, borderStartCoords.second * 9 / 4, 25, BLACK);
    }

public:
    GAME():
        m_Board(borderStartCoords.first, borderStartCoords.second, borderFinishCoords.first, borderFinishCoords.second),
        m_Grid(gridRows, gridCols, cellSize, borderStartCoords.first, borderStartCoords.second)
    {}

    void Draw() {
        TitleDraw();
        m_Board.draw();
        m_Grid.draw();

        if (m_isGameFinish){
            GameFinish_Draw();
        }
    }

    void Refresh() {
        if (targetCell_c == -1 || targetCell_r == -1) { return; }

        CELL& targetCell = m_Grid.getCellAt(targetCell_r * gridCols + targetCell_c);
        targetCell.Hovering(false);
    }

    void HoveringCell() {
        if (m_isGameFinish)
            return;

        auto coord = GetMousePosition();
        bool isCursorOnGrid = borderStartCoords.first  < coord.x && borderStartCoords.second  < coord.y && //Whater the cursor on the grid
                              borderFinishCoords.first > coord.x && borderFinishCoords.second > coord.y; 

        if (!isCursorOnGrid)
            return;

        targetCell_c = (coord.x - borderStartCoords.first) / cellSize;
        targetCell_r = (coord.y - borderStartCoords.second) / cellSize;
        CELL& targetCell = m_Grid.getCellAt(targetCell_r * gridCols + targetCell_c);
        
        targetCell.Hovering(true);

        if (int act = MouseEventListener()) { //Action
            switch (act) {
            case 1:
                targetCell.Flag(); break;

            case 2:
                targetCell.Open(); break;

            case 3: case 4:
                targetCell.OpenAll(); break;
            }
        }
    }

    void GenerateBombs() {
        while (currentBombs < numOfBombs) {
            int result = Random.GenerateRandom(gridRows * gridCols);

            if (m_Grid.getCellAt(result).MakeBomb()) { //If the cell is NOT a bomb
                currentBombs += 1;
                std::cout << "Bomb placed on Cell " << result / gridCols << "_" << result % gridCols << "\n";
            }
            else {
                std::cout << "Bomb COULDN'T placed on Cell " << result / gridCols << "_" << result % gridCols << "\n";
            }
        }
    }

    void StartGame() {
        GenerateBombs();
        m_Grid.SetCellValues(gridRows, gridCols);
    }

    void FinishGame(bool type) {  // type 0: Dead, 1: Won
        m_isGameFinish = 1;
        m_gameFinishType = type ? 1 : 0;
    }

    void ResetGame() {
        m_Grid.ResetGrid();
        m_isGameFinish = 0;
        currentBombs = 0;

        StartGame();
    }
};

/*
TODOS:
FIRST CLICK NOT BOMB, if so RESET ONLY 1 BOMB BUT OTHER THİNGS ALSO CHANGE ( MAYBE OPTİMİZED MAYBE NOT) 
IF ALL BOMBS= FLAG, USEABLE OPEN ALL
İF CLİCK cell have neighbor 0's also open them.
*/
