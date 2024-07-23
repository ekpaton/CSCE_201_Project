#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

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

struct Shot {
    string coordinates;
    bool isHit;
};

void initializeBoard(char board[GRID_SIZE][GRID_SIZE]);
void interactiveShipPlacement(char board[GRID_SIZE][GRID_SIZE], vector<Ship>& ships);
void placeAllComputerShips(char board[GRID_SIZE][GRID_SIZE]);
bool takeShot(char board[GRID_SIZE][GRID_SIZE], char shots[GRID_SIZE][GRID_SIZE], const string& shot);
bool checkAllShipsSunk(char board[GRID_SIZE][GRID_SIZE]);
void computerShot(char board[GRID_SIZE][GRID_SIZE], char shots[GRID_SIZE][GRID_SIZE], vector<Shot>& cpuShotHistory);
void displayBoard(const char board[GRID_SIZE][GRID_SIZE]);
bool validShipPlacement(char board[GRID_SIZE][GRID_SIZE], int x, int y, int size, char orientation);
void placeShip(char board[GRID_SIZE][GRID_SIZE], int x, int y, int size, char orientation);
void displayCpuShotHistory(const vector<Shot>& cpuShotHistory);

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

    vector<Shot> cpuShotHistory;
    bool gameOver = false;
    while (!gameOver) {
        string shot;
        displayBoard(playerShots);
        displayCpuShotHistory(cpuShotHistory);
        cout << "Enter your shot (e.g., A1): ";
        cin >> shot;
        if (takeShot(computerShips, playerShots, shot)) {
            cout << "Hit!\n";
        }
        else {
            cout << "Miss!\n";
        }

        gameOver = checkAllShipsSunk(computerShips);
        if (gameOver) {
            cout << "Player wins!\n";
            break;
        }

        computerShot(playerShips, computerShots, cpuShotHistory);

        gameOver = checkAllShipsSunk(playerShips);
        if (gameOver) {
            cout << "Computer wins!\n";
        }
    }

    return 0;
}

void initializeBoard(char board[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            board[i][j] = EMPTY;
        }
    }
}

void interactiveShipPlacement(char board[GRID_SIZE][GRID_SIZE], vector<Ship>& ships) {
    for (Ship& ship : ships) {
        bool placed = false;
        while (!placed) {
            string startingPoint;
            char orientation;
            cout << "Place " << ship.name << " of size " << ship.size << ".\n";
            displayBoard(board);
            cout << "Enter the coordinates of the starting position (e.g., A1) \nand orientation direction (H for horizontal, V for vertical): ";
            cin >> startingPoint >> orientation;

            int x = toupper(startingPoint[0]) - 'A';
            int y = stoi(startingPoint.substr(1)) - 1;

            if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE && (orientation == 'H' || orientation == 'V')) {
                if (validShipPlacement(board, x, y, ship.size, orientation)) {
                    placeShip(board, x, y, ship.size, orientation);
                    ship.placed = true;
                    placed = true;
                } else {
                    cout << "Cannot place ship there. Try again.\n";
                }
            } else {
                cout << "Invalid input. Try again.\n";
            }
        }
    }
}

bool validShipPlacement(char board[GRID_SIZE][GRID_SIZE], int x, int y, int size, char orientation) {
    if (orientation == 'H') {
        if (y + size > GRID_SIZE) {
            return false;
        }
        for (int i = 0; i < size; ++i) {
            if (board[x][y + i] != EMPTY) {
                return false;
            }
        }
    } else {
        if (x + size > GRID_SIZE) {
            return false;
        }
        for (int i = 0; i < size; ++i) {
            if (board[x + i][y] != EMPTY) {
                return false;
            }
        }
    }
    return true;
}

void placeShip(char board[GRID_SIZE][GRID_SIZE], int x, int y, int size, char orientation) {
    if (orientation == 'H') {
        for (int i = 0; i < size; ++i) {
            board[x][y + i] = SHIP;
        }
    } else {
        for (int i = 0; i < size; ++i) {
            board[x + i][y] = SHIP;
        }
    }
}

void placeAllComputerShips(char board[GRID_SIZE][GRID_SIZE]) {
    vector<Ship> ships = {
        {"Carrier", 5, false},
        {"Battleship", 4, false},
        {"Cruiser", 3, false},
        {"Submarine", 3, false},
        {"Destroyer", 2, false}
    };

    for (Ship& ship : ships) {
        bool placed = false;
        while (!placed) {
            int x = rand() % GRID_SIZE;
            int y = rand() % GRID_SIZE;
            char orientation = (rand() % 2 == 0) ? 'H' : 'V';
            if (validShipPlacement(board, x, y, ship.size, orientation)) {
                placeShip(board, x, y, ship.size, orientation);
                ship.placed = true;
                placed = true;
            }
        }
    }
}

bool takeShot(char board[GRID_SIZE][GRID_SIZE], char shots[GRID_SIZE][GRID_SIZE], const string& shot) {
    if (shot.length() < 2) {
        return false;
    }

    int row = toupper(shot[0]) - 'A';
    int col = stoi(shot.substr(1)) - 1;

    if (row < 0 || row >= GRID_SIZE || col < 0 || col >= GRID_SIZE) {
        return false;
    }

    if (board[row][col] == SHIP) {
        board[row][col] = HIT;
        shots[row][col] = HIT;
        return true;
    } else {
        shots[row][col] = MISS;
        return false;
    }
}

bool checkAllShipsSunk(char board[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (board[i][j] == SHIP) {
                return false;
            }
        }
    }
    return true;
}

void computerShot(char board[GRID_SIZE][GRID_SIZE], char shots[GRID_SIZE][GRID_SIZE], vector<Shot>& cpuShotHistory) {
    int x, y;
    bool shotTaken = false;
    while (!shotTaken) {
        x = rand() % GRID_SIZE;
        y = rand() % GRID_SIZE;
        if (shots[x][y] == EMPTY) {
            Shot shot;
            shot.coordinates = string(1, char('A' + x)) + to_string(y + 1);
            if (board[x][y] == SHIP) {
                cout << "Computer hit your ship at (" << char('A' + x) << y + 1 << ")!\n";
                board[x][y] = HIT;
                shots[x][y] = HIT;
                shot.isHit = true;
            } else {
                cout << "Computer missed at (" << char('A' + x) << y + 1 << ").\n";
                board[x][y] = MISS;
                shots[x][y] = MISS;
                shot.isHit = false;
            }
            cpuShotHistory.push_back(shot);
            shotTaken = true;
        }
    }
}

void displayBoard(const char board[GRID_SIZE][GRID_SIZE]) {
    cout << "  ";
    for (int i = 0; i < GRID_SIZE; ++i) {
        cout << i + 1 << " ";
    }
    cout << endl;

    for (int i = 0; i < GRID_SIZE; ++i) {
        cout << char('A' + i) << " ";
        for (int j = 0; j < GRID_SIZE; ++j) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

void displayCpuShotHistory(const vector<Shot>& cpuShotHistory) {
    cout << "CPU Shot History:\n";
    for (const Shot& shot : cpuShotHistory) {
        cout << shot.coordinates << " - " << (shot.isHit ? "Hit" : "Miss") << "\n";
    }
}



