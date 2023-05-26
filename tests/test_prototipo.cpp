#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

const int BOARD_SIZE = 15;
const int NUM_SHIPS = 5;
const char SHIP_SYMBOL = 'S';
const char HIT_SYMBOL = 'X';
const char MISS_SYMBOL = 'O';

enum class CellState { Empty, Ship, Hit, Miss };

struct Cell {
    CellState state;
    bool revealed;
};

typedef std::vector<std::vector<Cell>> Board;

// Función para imprimir el tablero
void printBoard(const Board& board) {
    std::cout << "   ";
    for (int col = 0; col < BOARD_SIZE; ++col) {
        std::cout << col << " ";
    }
    std::cout << std::endl;

    for (int row = 0; row < BOARD_SIZE; ++row) {
        std::cout << row << "  ";
        for (int col = 0; col < BOARD_SIZE; ++col) {
            const Cell& cell = board[row][col];
            if (cell.revealed) {
                switch (cell.state) {
                    case CellState::Empty:
                        std::cout << "~ ";
                        break;
                    case CellState::Ship:
                        std::cout << SHIP_SYMBOL << " ";
                        break;
                    case CellState::Hit:
                        std::cout << HIT_SYMBOL << " ";
                        break;
                    case CellState::Miss:
                        std::cout << MISS_SYMBOL << " ";
                        break;
                }
            } else {
                std::cout << "~ ";
            }
        }
        std::cout << std::endl;
    }
}

// Función para colocar los barcos en el tablero
void placeShips(Board& board) {
    std::srand(std::time(0));  // Semilla para la generación de números aleatorios

    int shipsPlaced = 0;
    while (shipsPlaced < NUM_SHIPS) {
        int row = std::rand() % BOARD_SIZE;
        int col = std::rand() % BOARD_SIZE;

        if (board[row][col].state == CellState::Empty) {
            board[row][col].state = CellState::Ship;
            ++shipsPlaced;
        }
    }
}

// Función para verificar si se ha hundido la flota
bool allShipsSunk(const Board& board) {
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (board[row][col].state == CellState::Ship && !board[row][col].revealed) {
                return false;
            }
        }
    }
    return true;
}

// Función para que la computadora realice un disparo
void computerShoot(Board& board) {
    int row, col;
    do {
        row = std::rand() % BOARD_SIZE;
        col = std::rand() % BOARD_SIZE;
    } while (board[row][col].revealed);

    board[row][col].revealed = true;

    if (board[row][col].state == CellState::Ship) {
        std::cout << "La computadora ha acertado un disparo!" << std::endl;
        board[row][col].state = CellState::Hit;
    } else {
        std::cout << "La computadora ha fallado el disparo." << std::endl;
        board[row][col].state = CellState::Miss;
    }
}

int main() {
    Board playerBoard(BOARD_SIZE, std::vector<Cell>(BOARD_SIZE, { CellState::Empty, false }));
    Board computerBoard(BOARD_SIZE, std::vector<Cell>(BOARD_SIZE, { CellState::Empty, false }));

    placeShips(computerBoard);

    while (true) {
        std::cout << "Tablero del Jugador:" << std::endl;
        printBoard(playerBoard);
        std::cout << "Tablero de la Computadora:" << std::endl;
        printBoard(computerBoard);

        // Turno del jugador
        int row, col;
        std::cout << "Ingresa las coordenadas de tu disparo (fila columna): ";
        std::cin >> row >> col;

        if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
            std::cout << "Coordenadas inválidas. Intenta de nuevo." << std::endl;
            continue;
        }

        if (playerBoard[row][col].revealed) {
            std::cout << "Ya has disparado en estas coordenadas. Intenta de nuevo." << std::endl;
            continue;
        }

        playerBoard[row][col].revealed = true;

        if (computerBoard[row][col].state == CellState::Ship) {
            std::cout << "Has acertado un disparo!" << std::endl;
            computerBoard[row][col].state = CellState::Hit;
        } else {
            std::cout << "Has fallado el disparo." << std::endl;
            computerBoard[row][col].state = CellState::Miss;
        }

        if (allShipsSunk(computerBoard)) {
            std::cout << "¡Felicidades! Has hundido todos los barcos de la computadora. ¡Ganaste!" << std::endl;
            break;
        }

        // Turno de la computadora
        computerShoot(playerBoard);

        if (allShipsSunk(playerBoard)) {
            std::cout << "La computadora ha hundido todos tus barcos. ¡Perdiste!" << std::endl;
            break;
        }
    }

    return 0;
}