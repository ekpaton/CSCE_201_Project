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

void initializeBoard(char board[GRID_SIZE][GRID_SIZE]);
void interactiveShipPlacement(char board[GRID_SIZE][GRID_SIZE], vector<Ship>& ships);
void placeAllComputerShips(char board[GRID_SIZE][GRID_SIZE]);
bool takeShot(char board[GRID_SIZE][GRID_SIZE], char shots[GRID_SIZE][GRID_SIZE], const string& shot);
bool checkAllShipsSunk(char board[GRID_SIZE][GRID_SIZE]);
void computerShot(char board[GRID_SIZE][GRID_SIZE], char shots[GRID_SIZE][GRID_SIZE]);
void displayBoard(const char board[GRID_SIZE][GRID_SIZE]);

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

void initializeBoard(char board[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            board[i][j] = EMPTY;
        }
    }
}

void interactiveShipPlacement(char board[GRID_SIZE][GRID_SIZE], vector<Ship>& ships) {
}

void placeAllComputerShips(char board[GRID_SIZE][GRID_SIZE]) {
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

void computerShot(char board[GRID_SIZE][GRID_SIZE], char shots[GRID_SIZE][GRID_SIZE]) {
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

