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

void initializeBoard(char board[GRID_SIZE][GRID_SIZE]);
void interactiveShipPlacement(char board[GRID_SIZE][GRID_SIZE], vector<Ship>& ships);
void placeAllComputerShips(char board[GRID_SIZE][GRID_SIZE]);
bool takeShot(char board[GRID_SIZE][GRID_SIZE], char shots[GRID_SIZE][GRID_SIZE], const string& shot);
bool checkAllShipsSunk(char board[GRID_SIZE][GRID_SIZE]);
void computerShot(char board[GRID_SIZE][GRID_SIZE], char shots[GRID_SIZE][GRID_SIZE]);
void displayBoard(const char board[GRID_SIZE][GRID_SIZE]);
bool validShipPlacement(char board[GRID_SIZE][GRID_SIZE], int x, int y, int size, char orientation);
void placeShip(char board[GRID_SIZE][GRID_SIZE], int x, int y, int size, char orientation);

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
        }
        else {
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
// pull in the board and the vector of ships 
void interactiveShipPlacement(char board[GRID_SIZE][GRID_SIZE], vector<Ship>& ships) 
{
    int count = 0;

    // loop over all the ships in the vector
    for (Ship& ship : ships) 
    {
        // boolean value to store if the ship has been placed or not 
        bool placed = false;

        // keep looping while the ship isn't placed 
        while (!placed) 
        {
            // this will store the A1 index value from the user input for the starting point 
            string startingPoint;

            // this will store direction 
            char orientation;
            if (count == 0)
            {
                displayBoard(board);

            }
            cout << "Place " << ship.name << " of size " << ship.size << ".\n";

            
            cout << "Enter the coordinates of the starting position (e.g., A1) \nand orientation direction (H for horizontal, V for vertical): ";
            
            

            // user input A1 and H for example 
            cin >> startingPoint >> orientation;

            // convert any lowercase to upper case then take the first element of the which is the row index value 
            // subtract A from each value to set A as the zero index element 
            int x = toupper(startingPoint[0]) - 'A';

            // convert to integer 
            //take the input second element which is the number and subtract 1 to set 1 as the zero index 
            int y = stoi(startingPoint.substr(1)) - 1;
            // now we have x and y coordinates for our grid 


            // input validation for the users input 
            if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE && (toupper(orientation) == 'H' || toupper(orientation) == 'V')) {
                // check the size and location of ship on the board and if it can be placed
                if (validShipPlacement(board, x, y, ship.size, orientation)) 
                {
                    placeShip(board, x, y, ship.size, orientation);
                    ship.placed = true;
                    placed = true;
                }
                else {
                    cout << "Cannot place ship there. Try again.\n";
                }
            }
            else {
                cout << "Invalid input. Try again.\n";
            }
            displayBoard(board);
            count++; 
        }
    }
}

bool validShipPlacement(char board[GRID_SIZE][GRID_SIZE], int x, int y, int size, char orientation) {
    // check for horizontal orientation 
    if (orientation == 'H') 
    {
        // add the length of the ship to the starting grid position and check if it can be less the the external boundaries
        if (y + size > GRID_SIZE)
        {
            // too big
            return false;
        }
        // check to make sure each element the program will try to place the ship on is already empty
        for (int i = 0; i < size; ++i) 
        {
            if (board[x][y + i] != EMPTY)
            {
                return false;
            }
        }
    }
    // check vertical condition 
    else 
    {
        if (x + size > GRID_SIZE) 
        { 
            return false; 
        }

        for (int i = 0; i < size; ++i) 
        {
            if (board[x + i][y] != EMPTY)
            {
                return false;
            }
        }
    }
    // last condition is it fits and can be placed
    return true;
}
// function to place the ship after a valid index is captured 
void placeShip(char board[GRID_SIZE][GRID_SIZE], int x, int y, int size, char orientation) 
{
    // change elements to ship value rather than empty
    if (orientation == 'H') {
        for (int i = 0; i < size; ++i) {
            board[x][y + i] = SHIP;
        }
    }
    // do the same for the vertical case 
    else {
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

void computerShot(char board[GRID_SIZE][GRID_SIZE], char shots[GRID_SIZE][GRID_SIZE]) {
    int x, y;
    bool shotTaken = false;
    while (!shotTaken) {
        x = rand() % GRID_SIZE;
        y = rand() % GRID_SIZE;
        if (shots[x][y] == EMPTY) {
            if (board[x][y] == SHIP) {
                cout << "Computer hit your ship at (" << char('A' + x) << y + 1 << ")!\n";
                board[x][y] = HIT;
                shots[x][y] = HIT;
            }
            else {
                cout << "Computer missed at (" << char('A' + x) << y + 1 << ").\n";
                board[x][y] = MISS;
                shots[x][y] = MISS;
            }
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
