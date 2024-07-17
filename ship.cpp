#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

const int GRID_SIZE = 10;
const char EMPTY = '.';
const char SHIP = 'S';
const char HIT = 'X';
const char MISS = 'O';

struct Ship {
    string name;
    int size;
    bool placed;
};

void clearConsole();
void initializeBoard(char board[GRID_SIZE][GRID_SIZE]);
void displayBoard(const char board[GRID_SIZE][GRID_SIZE]);
void parsePosition(const string &pos, int &row, int &col);
bool placeShip(char board[GRID_SIZE][GRID_SIZE], const string &start, const string &end);
bool placeComputerShip(char board[GRID_SIZE][GRID_SIZE], int size);
void placeAllComputerShips(char board[GRID_SIZE][GRID_SIZE]);
bool takeShot(char board[GRID_SIZE][GRID_SIZE], char shots[GRID_SIZE][GRID_SIZE], const string &pos);
void computerShot(char playerBoard[GRID_SIZE][GRID_SIZE], char computerShots[GRID_SIZE][GRID_SIZE]);
bool checkAllShipsSunk(const char board[GRID_SIZE][GRID_SIZE]);
void interactiveShipPlacement(char board[GRID_SIZE][GRID_SIZE], vector<Ship> &ships);

int main() {
    srand(time(0));

    char playerShips[GRID_SIZE][GRID_SIZE];
    char computerShips[GRID_SIZE][GRID_SIZE];
    char playerShots[GRID_SIZE][GRID_SIZE];
    char computerShots[GRID_SIZE][GRID_SIZE];

    initializeBoard(playerShips);
    initializeBoard(computerShips);
    initializeBoard(playerShots);
    initializeBoard(computerShots);

    vector<Ship> playerShipsList = {
        {"Carrier", 5, false},
        {"Battleship", 4, false},
        {"Cruiser", 3, false},
        {"Submarine", 3, false},
        {"Destroyer", 2, false}
    };

    cout << "Player, place your ships:\n";
    interactiveShipPlacement(playerShips, playerShipsList);

    placeAllComputerShips(computerShips);

    bool gameOver = false;
    while (!gameOver) {

        string shot;
        displayBoard(playerShots);
        cout << "Enter your shot (e.g., A1): ";
        cin >> shot;
        if (takeShot(computerShips, playerShots, shot)) {
            cout << "Hit!\n";
        } else {
            cout << "Miss!\n";
        }

        gameOver = checkAllShipsSunk(computerShips);
        if (gameOver) {
            cout << "Player wins!\n";
            break;
        }

        computerShot(playerShips, computerShots);

        gameOver = checkAllShipsSunk(playerShips);
        if (gameOver) {
            cout << "Computer wins!\n";
        }
    }

    return 0;
}

void clearConsole() {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}

void initializeBoard(char board[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            board[i][j] = EMPTY;
        }
    }
}

void displayBoard(const char board[GRID_SIZE][GRID_SIZE]) {
    cout << "   A B C D E F G H I J\n";
    for (int i = 0; i < GRID_SIZE; i++) {
        if (i < 9) {
            cout << " " << i + 1 << " ";
        } else {
            cout << i + 1 << " ";
        }
        for (int j = 0; j < GRID_SIZE; j++) {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }
}

void parsePosition(const string &pos, int &row, int &col) {
    col = pos[0] - 'A';
    row = stoi(pos.substr(1)) - 1;
}

bool placeShip(char board[GRID_SIZE][GRID_SIZE], const string &start, const string &end) {
    int startRow, startCol, endRow, endCol;
    parsePosition(start, startRow, startCol);
    parsePosition(end, endRow, endCol);

    if (startRow == endRow) {
        for (int i = startCol; i <= endCol; i++) {
            if (board[startRow][i] == SHIP) return false;
            board[startRow][i] = SHIP;
        }
    } else if (startCol == endCol) {
        for (int i = startRow; i <= endRow; i++) {
            if (board[i][startCol] == SHIP) return false;
            board[i][startCol] = SHIP;
        }
    } else {
        return false;
    }
    return true;
}

bool placeComputerShip(char board[GRID_SIZE][GRID_SIZE], int size) {
    bool placed = false;
    while (!placed) {
        int orientation = rand() % 2;
        int row = rand() % GRID_SIZE;
        int col = rand() % GRID_SIZE;

        if (orientation == 0 && col + size <= GRID_SIZE) { // Horizontal
            bool valid = true;
            for (int i = 0; i < size; ++i) {
                if (board[row][col + i] == SHIP) {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                for (int i = 0; i < size; ++i) {
                    board[row][col + i] = SHIP;
                }
                placed = true;
            }
        } else if (orientation == 1 && row + size <= GRID_SIZE) { // Vertical
            bool valid = true;
            for (int i = 0; i < size; ++i) {
                if (board[row + i][col] == SHIP) {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                for (int i = 0; i < size; ++i) {
                    board[row + i][col] = SHIP;
                }
                placed = true;
            }
        }
    }
    return placed;
}

void placeAllComputerShips(char board[GRID_SIZE][GRID_SIZE]) {
    vector<int> shipSizes = {5, 4, 3, 3, 2};
    for (int size : shipSizes) {
        placeComputerShip(board, size);
    }
}

bool takeShot(char board[GRID_SIZE][GRID_SIZE], char shots[GRID_SIZE][GRID_SIZE], const string &pos) {
    int row, col;
    parsePosition(pos, row, col);
    if (board[row][col] == SHIP) {
        shots[row][col] = HIT;
        board[row][col] = HIT;
        return true; // Hit
    } else {
        shots[row][col] = MISS;
        board[row][col] = MISS;
        return false; // Miss
    }
}

void computerShot(char playerBoard[GRID_SIZE][GRID_SIZE], char computerShots[GRID_SIZE][GRID_SIZE]) {
    bool shotTaken = false;
    while (!shotTaken) {
        int row = rand() % GRID_SIZE;
        int col = rand() % GRID_SIZE;
        if (computerShots[row][col] == EMPTY) {
            if (playerBoard[row][col] == SHIP) {
                computerShots[row][col] = HIT;
                playerBoard[row][col] = HIT;
                cout << "Computer hit at " << static_cast<char>('A' + col) << row + 1 << "!\n";
            } else {
                computerShots[row][col] = MISS;
                playerBoard[row][col] = MISS;
                cout << "Computer missed at " << static_cast<char>('A' + col) << row + 1 << ".\n";
            }
            shotTaken = true;
        }
    }
}

bool checkAllShipsSunk(const char board[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (board[i][j] == SHIP) {
                return false;
            }
        }
    }
    return true;
}

void interactiveShipPlacement(char board[GRID_SIZE][GRID_SIZE], vector<Ship> &ships) {
    string start, end;
    bool validPlacement;

    for (Ship &ship : ships) {
        do {
            clearConsole();
            displayBoard(board);
            cout << "Enter the start and end positions for your " << ship.name << " (size " << ship.size << ") (e.g., A1 A5): ";
            cin >> start >> end;
            validPlacement = placeShip(board, start, end);
            if (!validPlacement) {
                cout << "Invalid placement. Try again.\n";
            } else {
                ship.placed = true;
            }
        } while (!validPlacement);
    }
}
