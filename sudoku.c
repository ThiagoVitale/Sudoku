#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // For getpid
#include <stdbool.h>
#include "sudoku.h"

int board[SIZE][SIZE];
bool is_fixed[SIZE][SIZE];
int error_count = 0;
ConflictInfo last_conflict = {-1, -1, 0}; // Initialize with no conflict
Language selected_language = LANG_SPANISH; // Default language

// Define all messages for English and Spanish
const char *messages[LANG_COUNT][MSG_COUNT] = {
    // English
    {
        [MSG_PROGRESS_ERRORS] = "Progress: %.1f%% Errors: %d\n",
        [MSG_SELECT_DIFFICULTY] = "Select difficulty level:\n",
        [MSG_EASY] = "1. Easy\n",
        [MSG_MEDIUM] = "2. Medium\n",
        [MSG_HARD] = "3. Hard\n",
        [MSG_ADVANCED] = "4. Advanced\n",
        [MSG_EXPERT] = "5. Expert\n",
        [MSG_ENTER_DIFFICULTY] = "Enter a number (1-5): ",
        [MSG_INVALID_DIFFICULTY] = "Invalid difficulty. Using Medium level by default.\n",
        [MSG_DEFAULT_MEDIUM] = "Medium",
        [MSG_CONGRATULATIONS] = "Congratulations! You completed the Sudoku with 0 errors.\n",
        [MSG_ENTER_ROW_COL_VALUE] = "Enter row (1-9), column (1-9), and value (1-9): ",
        [MSG_INVALID_INPUT] = "Invalid input. Try again.\n",
        [MSG_CANNOT_MODIFY] = "Cannot modify a fixed cell.\n",
        [MSG_INVALID_MOVE_REPEATED] = "Invalid move. The number is repeated in %s. Errors: %d\n",
        [MSG_ROW] = "row",
        [MSG_COLUMN] = "column",
        [MSG_SUBGRID] = "subgrid",
        [MSG_INVALID_MOVE_SOLUTIONS] = "Invalid move. This move leads to a state with %d solutions (must have exactly 1 solution). Errors: %d\n",
        [MSG_SELECT_LANGUAGE] = "Select language / Seleccione idioma:\n",
        [MSG_ENGLISH] = "1. English / Inglés\n",
        [MSG_SPANISH] = "2. Spanish / Español\n"
    },
    // Spanish
    {
        [MSG_PROGRESS_ERRORS] = "Progreso: %.1f%% Errores: %d\n",
        [MSG_SELECT_DIFFICULTY] = "Seleccione el nivel de dificultad:\n",
        [MSG_EASY] = "1. Fácil\n",
        [MSG_MEDIUM] = "2. Medio\n",
        [MSG_HARD] = "3. Difícil\n",
        [MSG_ADVANCED] = "4. Avanzado\n",
        [MSG_EXPERT] = "5. Experto\n",
        [MSG_ENTER_DIFFICULTY] = "Ingrese un número (1-5): ",
        [MSG_INVALID_DIFFICULTY] = "Dificultad inválida. Usando nivel Medio por defecto.\n",
        [MSG_DEFAULT_MEDIUM] = "Medio",
        [MSG_CONGRATULATIONS] = "¡Felicidades! Completaste el Sudoku con %d errores.\n",
        [MSG_ENTER_ROW_COL_VALUE] = "Ingrese fila (1-9), columna (1-9), y valor (1-9): ",
        [MSG_INVALID_INPUT] = "Entrada inválida. Intente de nuevo.\n",
        [MSG_CANNOT_MODIFY] = "No puede modificar una casilla ya colocada.\n",
        [MSG_INVALID_MOVE_REPEATED] = "Movimiento inválido. El número se repite en %s. Errores: %d\n",
        [MSG_ROW] = "fila",
        [MSG_COLUMN] = "columna",
        [MSG_SUBGRID] = "subcuadro",
        [MSG_INVALID_MOVE_SOLUTIONS] = "Movimiento inválido. Este movimiento lleva a un estado con %d soluciones (debe haber exactamente 1 solución). Errores: %d\n",
        [MSG_SELECT_LANGUAGE] = "Select language / Seleccione idioma:\n",
        [MSG_ENGLISH] = "1. English / Inglés\n",
        [MSG_SPANISH] = "2. Spanish / Español\n"
    }
};

void play_sudoku() {
    setbuf(stdout, 0);
    srand(time(NULL) ^ getpid());

    printf("%s", messages[LANG_ENGLISH][MSG_SELECT_LANGUAGE]);
    printf("%s", messages[LANG_ENGLISH][MSG_ENGLISH]);
    printf("%s", messages[LANG_ENGLISH][MSG_SPANISH]);
    printf("Enter a number (1-2) / Ingrese un número (1-2): ");
    int lang_choice;
    scanf("%d", &lang_choice);
    if (lang_choice == 1) {
        selected_language = LANG_ENGLISH;
    } else {
        selected_language = LANG_SPANISH;
    }
    while (getchar() != '\n');

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            board[i][j] = 0;

    error_count = 0;
    last_conflict.row = -1;
    last_conflict.col = -1;
    last_conflict.conflict_type = 0;

    generate_full_board();

    int difficulty;
    printf("%s", messages[selected_language][MSG_SELECT_DIFFICULTY]);
    printf("%s", messages[selected_language][MSG_EASY]);
    printf("%s", messages[selected_language][MSG_MEDIUM]);
    printf("%s", messages[selected_language][MSG_HARD]);
    printf("%s", messages[selected_language][MSG_ADVANCED]);
    printf("%s", messages[selected_language][MSG_EXPERT]);
    printf("%s", messages[selected_language][MSG_ENTER_DIFFICULTY]);
    scanf("%d", &difficulty);

    int empty_cells;
    switch (difficulty) {
        case 1: empty_cells = 23 + (rand() % 5); break; // Easy: 23-27 empty cells
        case 2: empty_cells = 33 + (rand() % 5); break; // Medium: 33-37 empty cells
        case 3: empty_cells = 43 + (rand() % 5); break; // Hard: 43-47 empty cells
        case 4: empty_cells = 50 + (rand() % 5); break; // Advanced: 50-54 empty cells
        case 5: empty_cells = 58 + (rand() % 5); break; // Expert: 58-62 empty cells
        default:
            printf("%s", messages[selected_language][MSG_INVALID_DIFFICULTY]);
            empty_cells = 33 + (rand() % 5); // Default to Medium
            difficulty = 2;
            break;
    }


    remove_numbers(empty_cells, difficulty);

    while (1) {
        // Debug: Print empty cell count before printing board
        int empty_count = 0;
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                if (board[i][j] == 0) empty_count++;

        print_board();
        last_conflict.row = -1;
        last_conflict.col = -1;
        last_conflict.conflict_type = 0;

        if (!find_empty(board, &(int){0}, &(int){0})) {
            printf(messages[selected_language][MSG_CONGRATULATIONS], error_count);
            break;
        }

        printf("%s", messages[selected_language][MSG_ENTER_ROW_COL_VALUE]);
        int row, col, val;
        int result = scanf("%d %d %d", &row, &col, &val);

        if (result != 3 || row < 1 || row > 9 || col < 1 || col > 9 || val < 1 || val > 9) {
            printf("%s", messages[selected_language][MSG_INVALID_INPUT]);
            while (getchar() != '\n');
            continue;
        }

        row--; col--;

        if (is_fixed[row][col]) {
            printf("%s", messages[selected_language][MSG_CANNOT_MODIFY]);
            continue;
        }

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
            const char *conflict_area = (last_conflict.conflict_type == 1) ? messages[selected_language][MSG_ROW] :
                                        (last_conflict.conflict_type == 2) ? messages[selected_language][MSG_COLUMN] :
                                        messages[selected_language][MSG_SUBGRID];
            printf(messages[selected_language][MSG_INVALID_MOVE_REPEATED], conflict_area, error_count);
            continue;
        }

        board[row][col] = val;

        count_solutions();
        if (solution_count != 1) {
            error_count++;
            last_conflict.row = row;
            last_conflict.col = col;
            last_conflict.conflict_type = 0;
            printf(messages[selected_language][MSG_INVALID_MOVE_SOLUTIONS], solution_count, error_count);
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
