#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <limits>

using namespace std;

const int GRID_SIZE = 10;
const int DEMO_GRID_SIZE = 5;  // For demo mode
const char EMPTY = '.';
const char SHIP = 'S';
const char HIT = 'X';
const char MISS = 'O';

struct Ship {
    string name;
    int size;
    bool placed;
    int hitCount;
};

struct Shot {
    string coordinates;
    bool isHit;
};

void initializeBoard(char board[GRID_SIZE][GRID_SIZE], int gridSize);
void interactiveShipPlacement(char board[GRID_SIZE][GRID_SIZE], vector<Ship>& ships, int gridSize);
void placeAllComputerShips(char board[GRID_SIZE][GRID_SIZE], vector<Ship>& cpuShips, int gridSize);
bool takeShot(char board[GRID_SIZE][GRID_SIZE], char shots[GRID_SIZE][GRID_SIZE], vector<Ship>& ships, const string& shot, int gridSize);
bool checkAllShipsSunk(char board[GRID_SIZE][GRID_SIZE]);
void computerShot(char board[GRID_SIZE][GRID_SIZE], char shots[GRID_SIZE][GRID_SIZE], vector<Shot>& cpuShotHistory, int gridSize);
void displayBoard(const char board[GRID_SIZE][GRID_SIZE], int gridSize);
void displayCpuGrid(const char board[GRID_SIZE][GRID_SIZE], int gridSize);
void clearConsole();
bool validShipPlacement(char board[GRID_SIZE][GRID_SIZE], int x, int y, int size, char orientation, int gridSize);
void placeShip(char board[GRID_SIZE][GRID_SIZE], int x, int y, int size, char orientation);
void randomizeShipPlacement(char board[GRID_SIZE][GRID_SIZE], vector<Ship>& ships, int gridSize);

int main() {
    srand(time(0));
    char choice;

    while (true) {
        clearConsole();
        cout << "Main Menu\n";
        cout << "1. Demo Game\n";
        cout << "2. Full Game\n";
        cout << "3. Quit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == '3') break;

        int gridSize = (choice == '1') ? DEMO_GRID_SIZE : GRID_SIZE;
        char playerShips[GRID_SIZE][GRID_SIZE];
        char computerShips[GRID_SIZE][GRID_SIZE];
        char playerShots[GRID_SIZE][GRID_SIZE];
        char computerShots[GRID_SIZE][GRID_SIZE];

        initializeBoard(playerShips, gridSize);
        initializeBoard(computerShips, gridSize);
        initializeBoard(playerShots, gridSize);
        initializeBoard(computerShots, gridSize);

        vector<Ship> playerShipsList = {
            {"Carrier", 5, false, 0},
            {"Battleship", 4, false, 0},
            {"Cruiser", 3, false, 0},
            {"Submarine", 3, false, 0},
            {"Destroyer", 2, false, 0}
        };

        vector<Ship> cpuShipsList = {
            {"Carrier", 5, false, 0},
            {"Battleship", 4, false, 0},
            {"Cruiser", 3, false, 0},
            {"Submarine", 3, false, 0},
            {"Destroyer", 2, false, 0}
        };

        if (choice == '1') {
            playerShipsList = { {"Cruiser", 3, false, 0}, {"Destroyer", 2, false, 0} };
            cpuShipsList = { {"Cruiser", 3, false, 0}, {"Destroyer", 2, false, 0} };
        }

        clearConsole();
        cout << "Player, place your ships:\n";
        interactiveShipPlacement(playerShips, playerShipsList, gridSize);

        placeAllComputerShips(computerShips, cpuShipsList, gridSize);

        vector<Shot> cpuShotHistory;
        bool gameOver = false;
        while (!gameOver) {
            cin.get();
            cin.get();
            clearConsole();
            cout << "Player's Shots on CPU Board:\n";
            displayBoard(playerShots, gridSize);
            cout << "\nCPU Shots on Your Board:\n";
            displayCpuGrid(computerShots, gridSize);

            string shot;
            cout << "Enter your shot (e.g., A1): ";
            cin >> shot;
            if (takeShot(computerShips, playerShots, cpuShipsList, shot, gridSize)) {
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

            computerShot(playerShips, computerShots, cpuShotHistory, gridSize);

            gameOver = checkAllShipsSunk(playerShips);
            if (gameOver) {
                cout << "Computer wins!\n";
            }
        }

        cout << "\nGame over. Returning to main menu...\n";
        cin.ignore();
        cin.get();
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

void initializeBoard(char board[GRID_SIZE][GRID_SIZE], int gridSize) {
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            board[i][j] = EMPTY;
        }
    }
}

void interactiveShipPlacement(char board[GRID_SIZE][GRID_SIZE], vector<Ship>& ships, int gridSize) {
    int count = 0;
    for (Ship& ship : ships) {
        bool placed = false;
        while (!placed) {
            if (count != 0) {

                cin.get();
                cin.get();
                clearConsole();
            }
            string startingPoint;
            char orientation;
            cout << "Place " << ship.name << " of size " << ship.size << ".\n";
            displayBoard(board, gridSize);
            cout << "Enter the coordinates of the starting position (e.g., A1), orientation direction (H for horizontal, V for vertical), or R for random: ";
            cin >> startingPoint >> orientation;

            if (toupper(orientation) == 'R') {
                randomizeShipPlacement(board, ships, gridSize);
                displayBoard(board, gridSize);
                cout << "Press enter to continue:";
                return;
            }

            int x = toupper(startingPoint[0]) - 'A';
            int y = stoi(startingPoint.substr(1)) - 1;

            if (x >= 0 && x < gridSize && y >= 0 && y < gridSize && (toupper(orientation) == 'H' || toupper(orientation) == 'V')) {
                if (validShipPlacement(board, x, y, ship.size, orientation, gridSize)) {
                    placeShip(board, x, y, ship.size, orientation);
                    ship.placed = true;
                    placed = true;
                    displayBoard(board, gridSize);
                    cout << "Press enter to continue:";
                }
                else {
                    cout << "Cannot place ship there. Try again.\n";
                }
            }
            else {
                cout << "Invalid input. Try again.\n";
            }
            count++;
        }
    }
}

bool validShipPlacement(char board[GRID_SIZE][GRID_SIZE], int x, int y, int size, char orientation, int gridSize) {
    if (toupper(orientation) == 'H') {
        if (y + size > gridSize) return false;
        for (int i = 0; i < size; ++i) {
            if (board[x][y + i] != EMPTY) return false;
        }
    }
    else {
        if (x + size > gridSize) return false;
        for (int i = 0; i < size; ++i) {
            if (board[x + i][y] != EMPTY) return false;
        }
    }
    return true;
}

void placeShip(char board[GRID_SIZE][GRID_SIZE], int x, int y, int size, char orientation) {
    if (toupper(orientation) == 'H') {
        for (int i = 0; i < size; ++i) {
            board[x][y + i] = SHIP;
        }
    }
    else {
        for (int i = 0; i < size; ++i) {
            board[x + i][y] = SHIP;
        }
    }
}

void randomizeShipPlacement(char board[GRID_SIZE][GRID_SIZE], vector<Ship>& ships, int gridSize) {
    for (Ship& ship : ships) {
        bool placed = false;
        while (!placed) {
            int x = rand() % gridSize;
            int y = rand() % gridSize;
            char orientation = (rand() % 2 == 0) ? 'H' : 'V';
            if (validShipPlacement(board, x, y, ship.size, orientation, gridSize)) {
                placeShip(board, x, y, ship.size, orientation);
                ship.placed = true;
                placed = true;
            }
        }
    }
}

bool takeShot(char board[GRID_SIZE][GRID_SIZE], char shots[GRID_SIZE][GRID_SIZE], vector<Ship>& ships, const string& shot, int gridSize) {
    if (shot.length() < 2) {
        cout << "Invalid input. Try again.\n";
        return false;
    }

    int row = toupper(shot[0]) - 'A';
    int col = stoi(shot.substr(1)) - 1;

    if (row < 0 || row >= gridSize || col < 0 || col >= gridSize) {
        cout << "Coordinates out of bounds. Try again.\n";
        return false;
    }

    if (shots[row][col] == HIT || shots[row][col] == MISS) {
        cout << "You already shot there. Try again.\n";
        return false;
    }

    if (board[row][col] == SHIP) {
        board[row][col] = HIT;
        shots[row][col] = HIT;
        for (auto& ship : ships) {
            if (ship.placed && board[row][col] == SHIP) {
                ship.hitCount++;
                break;
            }
        }
        return true;
    }
    else {
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

void computerShot(char board[GRID_SIZE][GRID_SIZE], char shots[GRID_SIZE][GRID_SIZE], vector<Shot>& cpuShotHistory, int gridSize) {
    int x, y;
    bool shotTaken = false;
    while (!shotTaken) {
        x = rand() % gridSize;
        y = rand() % gridSize;
        if (shots[x][y] == EMPTY) {
            Shot shot;
            shot.coordinates = string(1, char('A' + x)) + to_string(y + 1);
            if (board[x][y] == SHIP) {
                cout << "Computer hit your ship at (" << char('A' + x) << y + 1 << ")!\n";
                cout << "Press enter to continue making shots:";
                board[x][y] = HIT;
                shots[x][y] = HIT;
                shot.isHit = true;
            }
            else {
                cout << "Computer missed at (" << char('A' + x) << y + 1 << ").\n";
                cout << "Press enter to continue making shots:";
                board[x][y] = MISS;
                shots[x][y] = MISS;
                shot.isHit = false;
            }
            cpuShotHistory.push_back(shot);
            if (cpuShotHistory.size() > 10) {
                cpuShotHistory.erase(cpuShotHistory.begin());
            }
            shotTaken = true;
        }
    }
}

void placeAllComputerShips(char board[GRID_SIZE][GRID_SIZE], vector<Ship>& cpuShips, int gridSize) {
    for (Ship& ship : cpuShips) {
        bool placed = false;
        while (!placed) {
            int x = rand() % gridSize;
            int y = rand() % gridSize;
            char orientation = (rand() % 2 == 0) ? 'H' : 'V';
            if (validShipPlacement(board, x, y, ship.size, orientation, gridSize)) {
                placeShip(board, x, y, ship.size, orientation);
                ship.placed = true;
                placed = true;
            }
        }
    }
}

void displayBoard(const char board[GRID_SIZE][GRID_SIZE], int gridSize) {
    cout << "  ";
    for (int i = 0; i < gridSize; ++i) {
        cout << i + 1 << " ";
    }
    cout << endl;

    for (int i = 0; i < gridSize; ++i) {
        cout << char('A' + i) << " ";
        for (int j = 0; j < gridSize; ++j) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

void displayCpuGrid(const char board[GRID_SIZE][GRID_SIZE], int gridSize) {
    cout << "  ";
    for (int i = 0; i < gridSize; ++i) {
        cout << i + 1 << " ";
    }
    cout << endl;

    for (int i = 0; i < gridSize; ++i) {
        cout << char('A' + i) << " ";
        for (int j = 0; j < gridSize; ++j) {
            if (board[i][j] == HIT || board[i][j] == MISS) {
                cout << board[i][j] << " ";
            }
            else {
                cout << EMPTY << " ";
            }
        }
        cout << endl;
    }
}