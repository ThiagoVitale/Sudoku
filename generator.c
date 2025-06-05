#include <stdbool.h>
#include <stdlib.h>
#include "sudoku.h"

void shuffle(int arr[], int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

bool is_valid(int (*current_board)[SIZE], int row, int col, int num) {
    for (int i = 0; i < SIZE; i++) {
        if (current_board[row][i] == num)
            return false;
    }
    for (int i = 0; i < SIZE; i++) {
        if (current_board[i][col] == num)
            return false;
    }
    int box_start_row = (row / 3) * 3;
    int box_start_col = (col / 3) * 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (current_board[box_start_row + i][box_start_col + j] == num)
                return false;
    return true;
}

bool find_empty(int (*current_board)[SIZE], int *row, int *col) {
    for (*row = 0; *row < SIZE; (*row)++)
        for (*col = 0; *col < SIZE; (*col)++)
            if (current_board[*row][*col] == 0)
                return true;
    return false;
}

bool generate_full_board() {
    int row, col;
    if (!find_empty(board, &row, &col))
        return true;

    int numbers[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    shuffle(numbers, 9);

    for (int i = 0; i < 9; i++) {
        int num = numbers[i];
        if (is_valid(board, row, col, num)) {
            board[row][col] = num;
            if (generate_full_board())
                return true;
            board[row][col] = 0;
        }
    }
    return false;
}

void remove_numbers(int empty_cells, int difficulty) {
    int pairs_to_remove = empty_cells / 2;
    bool remove_center = (empty_cells % 2 == 1);

    int possible_positions[41][2];
    int pos_index = 0;
    for (int row = 0; row <= 4; row++) {
        int col_start = (row == 4) ? 0 : 0;
        int col_end = (row == 4) ? 4 : 8;
        for (int col = col_start; col <= col_end; col++) {
            possible_positions[pos_index][0] = row;
            possible_positions[pos_index][1] = col;
            pos_index++;
        }
    }

    for (int i = pos_index - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp_row = possible_positions[i][0];
        int temp_col = possible_positions[i][1];
        possible_positions[i][0] = possible_positions[j][0];
        possible_positions[i][1] = possible_positions[j][1];
        possible_positions[j][0] = temp_row;
        possible_positions[j][1] = temp_col;
    }

    int pairs_removed = 0;
    bool difficulty_achieved = false;
    for (int i = 0; i < pos_index; i++) {
        int row = possible_positions[i][0];
        int col = possible_positions[i][1];
        int sym_row = 8 - row;
        int sym_col = 8 - col;

        if (board[row][col] == 0 || board[sym_row][sym_col] == 0)
            continue;

        int backup = board[row][col];
        int sym_backup = board[sym_row][sym_col];
        board[row][col] = 0;
        board[sym_row][sym_col] = 0;

        count_solutions();
        if (solution_count != 1) {
            board[row][col] = backup;
            board[sym_row][sym_col] = sym_backup;
            continue;
        }

        bool can_solve = can_solve_with_techniques(board, difficulty);
        bool has_one_empty_subgrid = (difficulty == 1) ? has_subgrid_with_one_empty(board) : true;

        if (difficulty == 1) {
            if (!can_solve || !has_one_empty_subgrid) {
                board[row][col] = backup;
                board[sym_row][sym_col] = sym_backup;
                continue;
            }
        } else if (difficulty == 2) {
            if (!can_solve) {
                board[row][col] = backup;
                board[sym_row][sym_col] = sym_backup;
                continue;
            }
        } else {
            if (can_solve) {
                pairs_removed++;
                continue;
            } else {
                difficulty_achieved = true;
            }
        }

        pairs_removed++;
        if (difficulty_achieved && pairs_removed >= pairs_to_remove)
            break;
    }

    if (remove_center && board[4][4] != 0) {
        int center_backup = board[4][4];
        board[4][4] = 0;
        count_solutions();
        bool can_solve = can_solve_with_techniques(board, difficulty);
        bool has_one_empty_subgrid = (difficulty == 1) ? has_subgrid_with_one_empty(board) : true;
        if (solution_count != 1 || 
            (difficulty == 1 && (!can_solve || !has_one_empty_subgrid)) ||
            (difficulty == 2 && !can_solve) ||
            (difficulty > 2 && can_solve)) {
            board[4][4] = center_backup;
        }
    }

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            is_fixed[i][j] = (board[i][j] != 0);
}