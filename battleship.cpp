#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <limits>

using namespace std;

const int GRID_SIZE = 10;
const int DEMO_GRID_SIZE = 5;
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
bool takeShot(char board[GRID_SIZE][GRID_SIZE], char shots[GRID_SIZE][GRID_SIZE], vector<Ship>& ships, const string& shot, int gridSize, string& shotMessage);
bool checkAllShipsSunk(const vector<Ship>& ships);
void computerShot(char board[GRID_SIZE][GRID_SIZE], char shots[GRID_SIZE][GRID_SIZE], vector<Shot>& cpuShotHistory, int gridSize, string& shotMessage);
void displayBoard(const char board[GRID_SIZE][GRID_SIZE], int gridSize);
void displayCpuGrid(const char board[GRID_SIZE][GRID_SIZE], int gridSize);
void clearConsole();
bool validShipPlacement(char board[GRID_SIZE][GRID_SIZE], int x, int y, int size, char orientation, int gridSize);
void placeShip(char board[GRID_SIZE][GRID_SIZE], int x, int y, int size, char orientation);
void randomizeShipPlacement(char board[GRID_SIZE][GRID_SIZE], vector<Ship>& ships, int gridSize);
void mainMenu();
void demoMode();
void fullGameMode();

int main() {
    srand(time(0));
    char choice;

    while (true) {
        mainMenu();

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
            playerShipsList = {{"Cruiser", 3, false, 0}, {"Destroyer", 2, false, 0}};
            cpuShipsList = {{"Cruiser", 3, false, 0}, {"Destroyer", 2, false, 0}};
        }

        clearConsole();
        cout << "Player, place your ships:\n";
        interactiveShipPlacement(playerShips, playerShipsList, gridSize);

        placeAllComputerShips(computerShips, cpuShipsList, gridSize);

        vector<Shot> cpuShotHistory;
        string playerShotMessage = "";
        string computerShotMessage = "";
        bool gameOver = false;

        while (!gameOver) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            clearConsole();
            cout << "Player's Shots on CPU Board:\n";
            displayBoard(playerShots, gridSize);
            cout << "\nCPU Shots on Your Board:\n";
            displayCpuGrid(computerShots, gridSize);
            cout << playerShotMessage << endl;
            cout << computerShotMessage << endl;

            string shot;
            cout << "Enter your shot (e.g., A1): ";
            cin >> shot;
            if (takeShot(computerShips, playerShots, cpuShipsList, shot, gridSize, playerShotMessage)) {
                playerShotMessage = "Hit!";
            } else {
                playerShotMessage = "Miss!";
            }

            if (checkAllShipsSunk(cpuShipsList)) {
                cout << "Player wins!\n";
                gameOver = true;
                break;
            }

            computerShot(playerShips, computerShots, cpuShotHistory, gridSize, computerShotMessage);

            if (checkAllShipsSunk(playerShipsList)) {
                cout << "Computer wins!\n";
                gameOver = true;
                break;
            }
        }

        // Post-game options
        cout << "\nGame over. What would you like to do next?\n";
        cout << "1. Play Again\n";
        cout << "2. Return to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == '1') {
            continue;  // Start a new game
        } else {
            clearConsole();
        }
    }

    return 0;
}

void mainMenu() {
    clearConsole();
    cout << "Main Menu\n";
    cout << "1. Demo Game\n";
    cout << "2. Full Game\n";
    cout << "3. Quit\n";
    cout << "Enter your choice: ";
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
    for (Ship& ship : ships) {
        bool placed = false;
        while (!placed) {
            string startingPoint;
            char orientation;
            cout << "Place " << ship.name << " of size " << ship.size << ".\n";
            displayBoard(board, gridSize);
            cout << "Enter the coordinates of the starting position (e.g., A1), orientation direction (H for horizontal, V for vertical), or R for random: ";
            cin >> startingPoint >> orientation;

            if (toupper(orientation) == 'R') {
                randomizeShipPlacement(board, ships, gridSize);
                displayBoard(board, gridSize);
                return;
            }

            int x = toupper(startingPoint[0]) - 'A';
            int y = stoi(startingPoint.substr(1)) - 1;

            if (x >= 0 && x < gridSize && y >= 0 && y < gridSize && (toupper(orientation) == 'H' || toupper(orientation) == 'V')) {
                if (validShipPlacement(board, x, y, ship.size, orientation, gridSize)) {
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

bool validShipPlacement(char board[GRID_SIZE][GRID_SIZE], int x, int y, int size, char orientation, int gridSize) {
    if (toupper(orientation) == 'H') {
        if (y + size > gridSize) return false;
        for (int i = 0; i < size; ++i) {
            if (board[x][y + i] != EMPTY) return false;
        }
    } else {
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
    } else {
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

bool takeShot(char board[GRID_SIZE][GRID_SIZE], char shots[GRID_SIZE][GRID_SIZE], vector<Ship>& ships, const string& shot, int gridSize, string& shotMessage) {
    if (shot.length() < 2) {
        shotMessage = "Invalid input. Try again.";
        return false;
    }

    int row = toupper(shot[0]) - 'A';
    int col = stoi(shot.substr(1)) - 1;

    if (row < 0 || row >= gridSize || col < 0 || col >= gridSize) {
        shotMessage = "Coordinates out of bounds. Try again.";
        return false;
    }

    if (shots[row][col] == HIT || shots[row][col] == MISS) {
        shotMessage = "You already shot there. Try again.";
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
        shotMessage = "Hit!";
        return true;
    } else {
        shots[row][col] = MISS;
        shotMessage = "Miss!";
        return false;
    }
}

bool checkAllShipsSunk(const vector<Ship>& ships) {
    for (const auto& ship : ships) {
        if (ship.hitCount < ship.size) {
            return false;
        }
    }
    return true;
}

void computerShot(char board[GRID_SIZE][GRID_SIZE], char shots[GRID_SIZE][GRID_SIZE], vector<Shot>& cpuShotHistory, int gridSize, string& shotMessage) {
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
                board[x][y] = HIT;
                shots[x][y] = HIT;
                shot.isHit = true;
                shotMessage = "Computer hit at (" + string(1, 'A' + x) + to_string(y + 1) + ")";
            } else {
                cout << "Computer missed at (" << char('A' + x) << y + 1 << ").\n";
                board[x][y] = MISS;
                shots[x][y] = MISS;
                shot.isHit = false;
                shotMessage = "Computer missed at (" + string(1, 'A' + x) + to_string(y + 1) + ")";
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
            } else {
                cout << EMPTY << " ";
            }
        }
        cout << endl;
    }
}