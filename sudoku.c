#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "sudoku.h"

int board[SIZE][SIZE];
bool is_fixed[SIZE][SIZE];
int error_count = 0;
ConflictInfo last_conflict = {-1, -1, 0}; // Initialize with no conflict

void play_sudoku() {
    setbuf(stdout, 0);
    srand(time(NULL));

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            board[i][j] = 0;

    error_count = 0;
    last_conflict.row = -1;
    last_conflict.col = -1;
    last_conflict.conflict_type = 0;

    generate_full_board();

    int difficulty;
    printf("Seleccione el nivel de dificultad:\n");
    printf("1. Fácil\n");
    printf("2. Medio\n");
    printf("3. Difícil\n");
    printf("4. Avanzado\n");
    printf("5. Experto\n");
    printf("Ingrese un número (1-5): ");
    scanf("%d", &difficulty);

    int empty_cells;
    switch (difficulty) {
        case 1: empty_cells = 30 + (rand() % 6); break;
        case 2: empty_cells = 36 + (rand() % 6); break;
        case 3: empty_cells = 42 + (rand() % 6); break;
        case 4: empty_cells = 48 + (rand() % 6); break;
        case 5: empty_cells = 54 + (rand() % 6); break;
        default:
            printf("Dificultad inválida. Usando nivel Medio por defecto.\n");
            empty_cells = 36 + (rand() % 6);
            difficulty = 2;
            break;
    }

    remove_numbers(empty_cells, difficulty);

    while (1) {
        print_board();
        last_conflict.row = -1; // Clear conflict after displaying
        last_conflict.col = -1;
        last_conflict.conflict_type = 0;

        if (!find_empty(board, &(int){0}, &(int){0})) {
            printf("¡Felicidades! Completaste el Sudoku con %d errores.\n", error_count);
            break;
        }

        int row, col, val;
        printf("Ingrese fila (1-9), columna (1-9), y valor (1-9): ");
        int result = scanf("%d %d %d", &row, &col, &val);

        if (result != 3 || row < 1 || row > 9 || col < 1 || col > 9 || val < 1 || val > 9) {
            printf("Entrada inválida. Intente de nuevo.\n");
            while (getchar() != '\n');
            continue;
        }

        row--; col--;

        if (is_fixed[row][col]) {
            printf("No puede modificar una casilla ya colocada.\n");
            continue;
        }

        // Check for conflicts to set conflict_type
        bool row_conflict = false, col_conflict = false, subgrid_conflict = false;
        for (int i = 0; i < SIZE; i++) {
            if (board[row][i] == val) row_conflict = true;
            if (board[i][col] == val) col_conflict = true;
        }
        int box_start_row = (row / 3) * 3;
        int box_start_col = (col / 3) * 3;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (board[box_start_row + i][box_start_col + j] == val)
                    subgrid_conflict = true;

        if (row_conflict || col_conflict || subgrid_conflict) {
            error_count++;
            last_conflict.row = row;
            last_conflict.col = col;
            if (row_conflict) last_conflict.conflict_type = 1;
            else if (col_conflict) last_conflict.conflict_type = 2;
            else last_conflict.conflict_type = 3;
            printf("Movimiento inválido. El número se repite en %s. Errores: %d\n",
                   last_conflict.conflict_type == 1 ? "fila" :
                   last_conflict.conflict_type == 2 ? "columna" : "subcuadro",
                   error_count);
            continue;
        }

        board[row][col] = val;

        count_solutions();
        if (solution_count != 1) {
            error_count++;
            last_conflict.row = row;
            last_conflict.col = col;
            last_conflict.conflict_type = 0; // No specific conflict for invalid solution count
            printf("Movimiento inválido. Este movimiento lleva a un estado con %d soluciones (debe haber exactamente 1 solución). Errores: %d\n", solution_count, error_count);
            board[row][col] = 0;
            continue;
        }

        is_fixed[row][col] = true;
    }
}

int main() {
    play_sudoku();
    return 0;
}