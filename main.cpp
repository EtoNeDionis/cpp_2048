#include <conio.h>
#include <windows.h>
 
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
 
using namespace std;
 
static const int MAPSIZE = 4;
static const int TOTALGOAL = 2048;
 
class Cell {
   private:
   public:
    bool hasCollapsedThisTurn;
    int value;
 
    Cell() {
        this->hasCollapsedThisTurn = false;
        this->value = 0;
    }
};
 
class Board {
   private:
   public:
    Cell cells[MAPSIZE][MAPSIZE];
    int score;
 
    Board() { this->score = 0; }
 
 	void printCells() {
        system("cls");
        cout << "Score: " << score << "\n";
        string decorLine(16 * MAPSIZE, '-');
 
        for (int i = 0; i < MAPSIZE; ++i) {
            cout << decorLine << "\n";
            for (int j = 0; j < MAPSIZE; ++j) {
                if (cells[i][j].value == 0) {
                    cout << "|       \t";
                } else {
                    cout << "|       " << cells[i][j].value << "\t";
                }
            }
            cout << "|" << "\n";
        }
        cout << decorLine << "\n";
    }
 
 
    pair<int, int> getRandomCoords() {
        random_device rd;   // obtain a random number from hardware
        mt19937 gen(rd());  // seed the generator
        uniform_int_distribution<> distrX(
            0, MAPSIZE - 1);  // define the range
        uniform_int_distribution<> distrY(
            0, MAPSIZE - 1);  // define the range
        int x = distrX(gen);
        int y = distrY(gen);
 
        return make_pair(x, y);
    }
 
    void insertNewValue() {
        pair<int, int> p = getRandomCoords();
 
       //Since a random pair of numbers can be a busy cell, we must repeat the random calculation until our random cell is empty
        while (cells[p.first][p.second].value != 0) {
            p = getRandomCoords();
        }
 
        cells[p.first][p.second].value = 2;
    }
 
    void initcells() {
        for (int i = 0; i < MAPSIZE; i++) {
            for (int j = 0; j < MAPSIZE; j++) {
                cells[i][j].value = 0;
            }
        }
 
        //  init first 2 twos
        for (int i = 0; i < 2; i++) {
            insertNewValue();
        }
    }
 
    bool hasFreeSpace() {
        for (int i = 0; i < MAPSIZE; i++) {
            for (int j = 0; j < MAPSIZE; j++) {
                if (cells[i][j].value == 0) return true;
            }
        }
 
        return false;
    }
 
    void moveUp() {
        for (int i = 0; i < MAPSIZE; i++) {
            for (int j = 0; j < MAPSIZE; j++) {
                if (cells[i][j].value == 0 ||
                    cells[i - 1][j].hasCollapsedThisTurn)
                    continue;
 
                if (cells[i - 1][j].value == 0) {
                    cells[i - 1][j].value = cells[i][j].value;
                    cells[i][j].value = 0;
                }
 
                if (cells[i - 1][j].value == cells[i][j].value) {
                    cells[i - 1][j].value = 2 * cells[i - 1][j].value;
                    score += 2 * cells[i - 1][j].value;
 
                    cells[i][j].value = 0;
                    cells[i - 1][j].hasCollapsedThisTurn = true;
                }
            }
        }
    }
    void moveLeft() {
        for (int i = 0; i < MAPSIZE; i++) {
            for (int j = 1; j < MAPSIZE; j++) {
                if (cells[i][j].value == 0 ||
                    cells[i][j - 1].hasCollapsedThisTurn)
                    continue;
 
                if (cells[i][j - 1].value == 0) {
                    cells[i][j - 1].value = cells[i][j].value;
                    cells[i][j].value = 0;
                }
 
                if (cells[i][j - 1].value == cells[i][j].value) {
                    cells[i][j - 1].value = 2 * cells[i][j - 1].value;
                    score += 2 * cells[i][j - 1].value;
 
                    cells[i][j].value = 0;
                    cells[i][j - 1].hasCollapsedThisTurn = true;
                }
            }
        }
    }
    void moveDown() {
        for (int i = MAPSIZE - 2; i >= 0; i--) {
            for (int j = 0; j < MAPSIZE; j++) {
 
                    if (cells[i][j].value == 0 ||
                    cells[i + 1][j].hasCollapsedThisTurn)
                    continue;
 
               
                if (cells[i + 1][j].value == 0) {
                    cells[i + 1][j].value = cells[i][j].value;
                    cells[i][j].value = 0;
                }
 
          
                if (cells[i + 1][j].value == cells[i][j].value) {
                    cells[i + 1][j].value = 2 * cells[i + 1][j].value;
                    score += 2 * cells[i + 1][j].value;
 
                    cells[i][j].value = 0;
                    cells[i + 1][j].hasCollapsedThisTurn = true;
                }
            }
        }
    }
 
    void moveRight() {
        for (int i = 0; i < MAPSIZE; i++) {
            for (int j = MAPSIZE - 2; j >= 0; j--) {
                if (cells[i][j].value == 0 ||
                    cells[i][j + 1].hasCollapsedThisTurn)
                    continue;
 
                if (cells[i][j + 1].value == 0) {
                    cells[i][j + 1].value = cells[i][j].value;
                    cells[i][j].value = 0;
                }
 
                if (cells[i][j + 1].value == cells[i][j].value) {
                    cells[i][j + 1].value = 2 * cells[i][j + 1].value;
                    score += 2 * cells[i][j + 1].value;
 
                    cells[i][j].value = 0;
                    cells[i][j + 1].hasCollapsedThisTurn = true;
                }
            }
        }
    }
 
    Board getCopyBoard() {
        Board newBoard;
        newBoard.score = this->score;
        for (int i = 0; i < MAPSIZE; i++) {
            for (int j = 0; j < MAPSIZE; j++) {
                newBoard.cells[i][j].value = cells[i][j].value;
            }
        }
        return newBoard;
    }
 
    bool isBoardSame(Board _board) {
        for (int i = 0; i < MAPSIZE; i++) {
            for (int j = 0; j < MAPSIZE; j++) {
                if (_board.cells[i][j].value != cells[i][j].value) return false;
            }
        }
        return true;
    }
};
 
class Game {
   private:
    char inputCh;
    bool skipTurn;
 
    void save() {
        vector<int> map;
        string savedMap;
 
        for (int i = 0; i < MAPSIZE; i++) {
            for (int j = 0; j < MAPSIZE; j++) {
                int elem = board.cells[i][j].value;
                savedMap += to_string(elem);
                savedMap += " ";
            }
        }
        ofstream saveFile("save.txt");
 
        if (saveFile.is_open()) {
            saveFile << savedMap;
            saveFile << board.score;
        }
        system("cls");
        saveFile.close();
    }
 
    void load() {
        ifstream saveFile("./save.txt");
        if (saveFile.is_open()) {
            string line;
            vector<int> map;
 
            while (saveFile >> line) {
                map.push_back(stoi(line));
            }
 
            system("cls");
 
            if (map.size() == 0 || MAPSIZE * MAPSIZE != map.size() - 1) {
                cout << "Please check either map size is correct or save file "
                        "exists\n\n";
                return;
            }
 
            int index = 0;
            for (int i = 0; i < MAPSIZE; i++) {
                for (int j = 0; j < MAPSIZE; j++) {
                    board.cells[i][j].value = map.at(index++);
                }
            }
 
            board.score = map.back();
        }
        saveFile.close();
    }
 
    void cinInputCh() {
        inputCh = getch();
 
        Board copyBoard;
        switch (inputCh) {
            case 'w':
                while (!board.isBoardSame(copyBoard)) {
                    copyBoard = board.getCopyBoard();
                    board.moveUp();
                }
                break;
 
            case 'a':
                while (!board.isBoardSame(copyBoard)) {
                    copyBoard = board.getCopyBoard();
                    board.moveLeft();
                }
                break;
            case 's':
                while (!board.isBoardSame(copyBoard)) {
                    copyBoard = board.getCopyBoard();
                    board.moveDown();
                }
                break;
            case 'd':
                while (!board.isBoardSame(copyBoard)) {
                    copyBoard = board.getCopyBoard();
                    board.moveRight();
                }
                break;
            case 'q':
                hasEnded = true;
                break;
            case 'o':
                save();
                break;
            case 'l':
                load();
                break;
            case 'r':
                playGame();
                break;
            default:
                skipTurn = true;
                break;
        }
    }
 
    bool isGameEnded() {
    	for (int i = 0; i < MAPSIZE; i++) {
            for (int j = 0; j < MAPSIZE; j++) {
                if (board.cells[i][j].value == TOTALGOAL) {
                    hasWon = true;
                    hasEnded = true;
                    return true;
                }
            }
        }
 
        for (int i = 0; i < MAPSIZE; i++) {
            for (int j = 0; j < MAPSIZE; j++) {
                int value = board.cells[i][j].value;
                if (j != MAPSIZE - 1 && board.cells[i][j + 1].value == value ||
                    j != 0 && board.cells[i][j - 1].value == value ||
                    i != MAPSIZE - 1 && board.cells[i + 1][j].value == value ||
                    i != 0 && board.cells[i - 1][j].value == value ||
                    j != MAPSIZE - 1 && board.cells[i][j + 1].value == 0 ||
                    j != 0 && board.cells[i][j - 1].value == 0 ||
                    i != MAPSIZE - 1 && board.cells[i + 1][j].value == 0 ||
                    i != 0 && board.cells[i - 1][j].value == 0) {
                    return false;
                }
            }
        }
 
        hasEnded = true;
        return true;
    }
 
   public:
    bool isStarted;
    bool hasEnded;
    bool hasWon;
    Board board;
 
    Game() {
        this->isStarted = false;
        this->hasWon = false;
        this->hasEnded = false;
        this->board = *new Board();
    }
 
    void playGame() {
        board = *new Board();
        board.initcells();
 
        isStarted = true;
        cout << "2048 Puzzle Game\n";
 
        while (!hasEnded) {
            skipTurn = false;
            board.printCells();
            cout
                << "w: up, a: left, s: down, d: right, r: restart, o: save, q: "
                   "quit, l: load save\n\n";
 
            cinInputCh();
            if (inputCh == 'o' || inputCh == 'l' || skipTurn) continue;
 
            //  clean cells from property "hasCollapsedThisTurn"
            board = board.getCopyBoard();
 
            if (board.hasFreeSpace()) {
                board.insertNewValue();
            }
 
            isGameEnded();
        }
 
        board.printCells();
        hasWon ? cout << "You Won!\n" : cout << "You Lost\n";
 
        Sleep(2000);
    }
};
 
int main() {
    Game game;
    game.playGame();
    return 0;
}
