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

    int Open(bool FirstClick) { //Return= -4: Flagged/Unopenable, -3: Opened, -2: First click is Bomb, -1: Opened bomb, 0: Zero, 0<x: Opened cell with value of x;
        if (isFlagged)
            return -4;

        if (isOpened)
            return -3;

        if (isBomb) {
            if (FirstClick) {
                std::cout << " OPENED THE BOMB FIRST CLICK! \n";
                return -2;
            }
            std::cout << " OPENED THE BOMB! \n";
            isOpened = true;
            return -1;
        }
        isOpened = true;
        return m_value;
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

    template<typename T>
    T getData(char name = 'V') { // (int) V: value, (bool) B: bomb, (bool) F: flagged, (bool) O: opened, (bool) H: hovering, (int) R: row, (int) C: col 
        switch (name) {
        case 'V': return m_value; break;
        case 'B': return isBomb; break;
        case 'F': return isFlagged; break;
        case 'O': return isOpened; break;
        case 'H': return isHovering; break;
        case 'R': return m_row; break;
        case 'C': return m_column; break;
        default: std::cout << "Error: Unknown data name!\n"; break;
        }
    }
};

class GRID {
private:
    int rowNum, colNum;
    std::vector<CELL> Grid;

    template<typename Func>
    void NeighbourFunc(int r, int c, Func func) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++)
            {
                if ((i == 0 && j == 0)) { continue; } //If the cell is itself

                int n_r = r + i, n_c = c + j;
                if (n_r >= 0 && n_r < rowNum && n_c >= 0 && n_c < colNum) { //If NOT out of border
                    func(n_r, n_c);
                }
            }
        }
    }
public:
    GRID(int rowNUM, int colNUM, int cellSize, int cellstartX, int cellstartY) :
        rowNum(rowNUM), colNum(colNUM)
    {
        Grid.reserve(rowNum * colNum);

        for (int r = 0; r < rowNum; r++) {
            for (int c = 0; c < colNum; c++) {
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

    CELL& getCellAt(int row, int col) {
        return Grid.at(row * colNum + col);
    }

    void ResetGrid() {
        for (auto& cell : Grid) {
            cell.setToDefault();
        }
    }

    void SetCellValues() { // This is for initial set
        for (int r = 0; r < rowNum; r++) {
            for (int c = 0; c < colNum; c++) // Every cell
            {
                int value = 0;
                if (getCellAt(r, c).getData<bool>('B')) {
                    value = -1;
                }
                else {
                    NeighbourFunc(r, c, [&value, this](int n_r, int n_c) {
                        if (getCellAt(n_r, n_c).getData<bool>('B')) { value++; }
                    });
                }
                getCellAt(r, c).setValue(value);
            }
        }
    }

    void SetCellValues(int newBomb_r, int newBomb_c, int oldBomb_r, int oldBomb_c) { // This is for first click optimizasion set
        int oldBombValue = 0;
        std::cout << "Cell " << newBomb_r << "_" << newBomb_c << " is change to be a bomb!\n"; // Its already made before coming this function (in ReplaceBomb)

        NeighbourFunc(oldBomb_r, oldBomb_c, [this, &oldBombValue](int n_r, int n_c) {
            CELL& neighbourOldCell = getCellAt(n_r, n_c);
            if (neighbourOldCell.getData<bool>('B')) 
                oldBombValue++;
            else 
                neighbourOldCell.setValue(neighbourOldCell.getData<int>() - 1);;
        });

        NeighbourFunc(newBomb_r, newBomb_c, [this](int n_r, int n_c) {
            CELL& neighbourNewCell = getCellAt(n_r, n_c);
            if (!(neighbourNewCell.getData<bool>('B'))) { // If the neighbour is NOT a bomb
                neighbourNewCell.setValue(neighbourNewCell.getData<int>() + 1);}
        });

        CELL& oldBombCell = getCellAt(oldBomb_r, oldBomb_c);
        oldBombCell.setToDefault();
        oldBombCell.setValue(oldBombValue);
        oldBombCell.Open(false);
    }

    void OpenCellPlus(int r, int c, int& totalOpenedCells) {
        NeighbourFunc(r, c, [this, &totalOpenedCells](int n_r, int n_c) { // HAS A PROBLEM, DOESNT open outer things, have diffirent kinds. 
            CELL& neighbourCell = getCellAt(n_r, n_c);
            if (!neighbourCell.getData<bool>('O')) { 
                neighbourCell.Open(false);
                totalOpenedCells++;

                if (neighbourCell.getData<int>('V') == 0) {
                    OpenCellPlus(n_r, n_c, totalOpenedCells);
                }
            }
        });
    }

    bool OpenNeighbourCells(int r, int c, int& totalOpenedCells) {
        int FlaggedNeighbours = 0;
        bool ReturnResult = false;

        NeighbourFunc(r, c, [this, &FlaggedNeighbours](int n_r, int n_c) {
            if (getCellAt(n_r, n_c).getData<bool>('F'))
                FlaggedNeighbours++;
        });

        CELL& targetCell = getCellAt(r, c);
        if (targetCell.getData<int>('V') == FlaggedNeighbours) {
            NeighbourFunc(r, c, [this, &totalOpenedCells, &ReturnResult](int n_r, int n_c) {
                CELL& NeighbourCell = getCellAt(n_r, n_c);

                int result = NeighbourCell.getData<bool>('O') ? -5 : NeighbourCell.Open(false);
                if (result >= -1) {
                    if (result == -1) { ReturnResult = true; } // Opened a bomb
                    else {
                        if (result == 0) { OpenCellPlus(n_r, n_c, totalOpenedCells); }
                        totalOpenedCells++;
                    }
                }
            });
        }
        return ReturnResult;
    }
};

class GAME {
private:
    PRNG_Engine Random;

    //Cell stuff
    const int cellSize = 50;
    const int gridCols = 8, gridRows = 8, numOfCells = 8 * 8, numOfBombs = 10;
    int numOfOpenedCells = 0, currentBombs = 0;
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
    bool m_gameFinishType = 0;  // type 0: Dead, 1: Won
    bool m_isFirstClick = 1;

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
            case OUT_MOUSE_BTN_R:
                targetCell.Flag(); break;

            case OUT_MOUSE_BTN_L:
            {
                int result = targetCell.Open(m_isFirstClick);
                m_isFirstClick = false; 

                switch (result) {
                case -4: //Flagged
                    break;

                case -3: //Opened
                    if (m_Grid.OpenNeighbourCells(targetCell_r, targetCell_c, numOfOpenedCells)) //If opened a bomb
                        FinishGame(0);
                    else {
                        std::cout << numOfOpenedCells << " - " << numOfCells << " - " << numOfBombs << "\n";
                        if (numOfOpenedCells == numOfCells - numOfBombs)
                            FinishGame(1);
                    }
                    break;

                case -2: //FirstClick Bomb
                    ReplaceBomb(targetCell_r, targetCell_c);
                    if (targetCell.getData<int>('V') == 0) {
                        m_Grid.OpenCellPlus(targetCell_r, targetCell_c, numOfOpenedCells);
                    }

                    numOfOpenedCells++; // Initial opening
                    if (numOfOpenedCells == numOfCells - numOfBombs)
                        FinishGame(1);
                    break;

                case -1: //Opened Bomb
                    FinishGame(0);
                    break;

                default:
                    if (result == 0)
                        m_Grid.OpenCellPlus(targetCell_r, targetCell_c, numOfOpenedCells);

                    numOfOpenedCells++; // Initial opening
                    std::cout << numOfOpenedCells << " - " << numOfCells << " - " << numOfBombs << "\n";
                    if (numOfOpenedCells == numOfCells - numOfBombs)
                        FinishGame(1);
                }
                break;
            }

            case OUT_MOUSE_BUT_M: case OUT_MOUSE_BTN_B:
                if (m_Grid.getCellAt(targetCell_r,targetCell_c).getData<bool>('O')) {
                    if (m_Grid.OpenNeighbourCells(targetCell_r, targetCell_c, numOfOpenedCells)) //If opened a bomb
                        FinishGame(0);
                    else {
                        std::cout << numOfOpenedCells << " - " << numOfCells << " - " << numOfBombs << "\n";
                        if (numOfOpenedCells == numOfCells - numOfBombs)
                            FinishGame(1);
                    }
                }
                break;
            }
        }
    }

    void GenerateBombs() {
        while (currentBombs < numOfBombs) {
            int result = Random.GenerateRandom(gridRows * gridCols);

            if (m_Grid.getCellAt(result).MakeBomb()) { //If the cell is NOT a bomb
                currentBombs ++;
                std::cout << "Bomb placed on Cell " << result / gridCols << "_" << result % gridCols << "\n";
            }
            else {
                std::cout << "Bomb COULDN'T placed on Cell " << result / gridCols << "_" << result % gridCols << "\n";
            }
        }
    }

    void ReplaceBomb(int oldCell_r, int oldCell_c) {
        int result;
        do {
            result = Random.GenerateRandom(gridRows * gridCols);
        } while (result == oldCell_r * gridCols + oldCell_c || !(m_Grid.getCellAt(result).MakeBomb())); // While NOT the same cell AND the diffirent cell is NOT a bomb , make the result cell a bomb.
        m_Grid.SetCellValues(result / gridCols, result % gridCols, oldCell_r, oldCell_c);
    }

    void StartGame() {
        std::cout << "\n --------------------------------- \n\n";
        GenerateBombs();
        m_Grid.SetCellValues();
    }

    void FinishGame(bool type) {  // Type 0: Dead, 1: Won
        m_isGameFinish = 1;
        m_gameFinishType = type ? 1 : 0;
    }

    void ResetGame() {
        m_Grid.ResetGrid();
        m_isGameFinish = 0; m_isFirstClick = 1;
        currentBombs = 0; numOfOpenedCells = 0;
        StartGame();
    }
};

/*
TODOS:
A timer, display the final time when game end.
Remaining bomb counter.
*/
