#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h> // For debugging
#include "sudoku.h"

void remove_numbers_easy(int target_empty_cells);
void remove_numbers_medium(int target_empty_cells);
void remove_numbers_hard(int target_empty_cells);
void remove_numbers_advanced(int target_empty_cells);
void remove_numbers_expert(int target_empty_cells);

// Shuffle function for 1D arrays
void shuffle_1d(int arr[], int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// Shuffle function for 2D arrays
void shuffle(int arr[][2], int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp_row = arr[i][0];
        int temp_col = arr[i][1];
        arr[i][0] = arr[j][0];
        arr[i][1] = arr[j][1];
        arr[j][0] = temp_row;
        arr[j][1] = temp_col;
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
    for (*row = 0; (*row) < SIZE; (*row)++)
        for (*col = 0; (*col) < SIZE; (*col)++)
            if (current_board[*row][*col] == 0)
                return true;
    return false;
}

bool generate_full_board() {
    int row, col;
    if (!find_empty(board, &row, &col))
        return true;

    int numbers[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    shuffle_1d(numbers, 9); // Use shuffle_1d for 1D array

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

void initialize_positions(int possible_positions[81][2], int *pos_index) {
    *pos_index = 0;
    for (int row = 0; row <= 4; row++) {
        int col_start = 0;
        int col_end = (row == 4) ? 4 : 8;
        for (int col = col_start; col <= col_end; col++) {
            possible_positions[*pos_index][0] = row;
            possible_positions[*pos_index][1] = col;
            (*pos_index)++;
        }
    }
    shuffle(possible_positions, *pos_index); // Use shuffle for 2D array
}

void remove_numbers_easy(int target_empty_cells) {
    int pairs_to_remove = target_empty_cells / 2;
    bool remove_center = (target_empty_cells % 2 == 1);

    int possible_positions[81][2];
    int pos_index;
    initialize_positions(possible_positions, &pos_index);

    int pairs_removed = 0;
    const int max_attempts = 100;
    int attempts = 0;
    const int max_resets = 3;
    int reset_count = 0;
    int pos_reset_count = 0;

    int removed_positions[81][2];
    int backup_values[81][2];
    int removed_count = 0;

    while (pairs_removed < pairs_to_remove && reset_count < max_resets) {
        if (attempts >= max_attempts) {
            if (removed_count > 0) {
                removed_count--;
                int row = removed_positions[removed_count][0];
                int col = removed_positions[removed_count][1];
                int sym_row = 8 - row;
                int sym_col = 8 - col;
                board[row][col] = backup_values[removed_count][0];
                board[sym_row][sym_col] = backup_values[removed_count][1];
                pairs_removed--;
                attempts = 0;
            } else {
                generate_full_board();
                pairs_removed = 0;
                removed_count = 0;
                attempts = 0;
                reset_count++;
                initialize_positions(possible_positions, &pos_index);
                pos_reset_count = 0;
                continue;
            }
        }

        if (pos_index == 0) {
            pos_reset_count++;
            if (pos_reset_count >= max_resets) break;
            pos_index = 0;
            for (int row = 0; row < SIZE; row++) {
                int col_start = (row <= 4) ? 0 : (8 - row);
                int col_end = (row <= 4) ? (row == 4 ? 4 : 8) : 8;
                for (int col = col_start; col <= col_end; col++) {
                    if (board[row][col] != 0) {
                        possible_positions[pos_index][0] = row;
                        possible_positions[pos_index][1] = col;
                        pos_index++;
                    }
                }
            }
            if (pos_index == 0) break;
            shuffle(possible_positions, pos_index); // Use shuffle for 2D array
        }

        int i = rand() % pos_index;
        int row = possible_positions[i][0];
        int col = possible_positions[i][1];
        int sym_row = 8 - row;
        int sym_col = 8 - col;

        possible_positions[i][0] = possible_positions[pos_index - 1][0];
        possible_positions[i][1] = possible_positions[pos_index - 1][1];
        pos_index--;

        if (board[row][col] == 0 || board[sym_row][sym_col] == 0) {
            attempts++;
            continue;
        }

        int backup = board[row][col];
        int sym_backup = board[sym_row][sym_col];
        board[row][col] = 0;
        board[sym_row][sym_col] = 0;

        count_solutions();
        if (solution_count != 1) {
            board[row][col] = backup;
            board[sym_row][sym_col] = sym_backup;
            attempts++;
            continue;
        }

        removed_positions[removed_count][0] = row;
        removed_positions[removed_count][1] = col;
        backup_values[removed_count][0] = backup;
        backup_values[removed_count][1] = sym_backup;
        removed_count++;

        pairs_removed++;
        attempts = 0;
    }

    bool meets_criteria = false;
    int final_attempts = 0;
    while (removed_count > 0 && final_attempts < max_attempts) {
        bool can_solve = can_solve_with_techniques(board, 1);
        bool has_one_empty_subgrid = has_subgrid_with_one_empty(board);
        if (can_solve && has_one_empty_subgrid) {
            meets_criteria = true;
            break;
        }
        removed_count--;
        int row = removed_positions[removed_count][0];
        int col = removed_positions[removed_count][1];
        int sym_row = 8 - row;
        int sym_col = 8 - col;
        board[row][col] = backup_values[removed_count][0];
        board[sym_row][sym_col] = backup_values[removed_count][1];
        pairs_removed--;
        final_attempts++;
    }

    if (!meets_criteria && removed_count == 0) {
        generate_full_board();
        remove_numbers(target_empty_cells - 2, 1);
        return;
    }

    if (remove_center && board[4][4] != 0) {
        int center_backup = board[4][4];
        board[4][4] = 0;
        count_solutions();
        bool can_solve = can_solve_with_techniques(board, 1);
        bool has_one_empty_subgrid = has_subgrid_with_one_empty(board);
        if (solution_count != 1 || !(can_solve && has_one_empty_subgrid)) {
            board[4][4] = center_backup;
        }
    }

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            is_fixed[i][j] = (board[i][j] != 0);
}

void remove_numbers_medium(int target_empty_cells) {
    int pairs_to_remove = target_empty_cells / 2;
    bool remove_center = (target_empty_cells % 2 == 1);

    int possible_positions[81][2];
    int pos_index;
    initialize_positions(possible_positions, &pos_index);

    int pairs_removed = 0;
    const int max_attempts = 100;
    int attempts = 0;
    const int max_resets = 3;
    int reset_count = 0;
    int pos_reset_count = 0;

    int removed_positions[81][2];
    int backup_values[81][2];
    int removed_count = 0;

    while (pairs_removed < pairs_to_remove && reset_count < max_resets) {
        if (attempts >= max_attempts) {
            if (removed_count > 0) {
                removed_count--;
                int row = removed_positions[removed_count][0];
                int col = removed_positions[removed_count][1];
                int sym_row = 8 - row;
                int sym_col = 8 - col;
                board[row][col] = backup_values[removed_count][0];
                board[sym_row][sym_col] = backup_values[removed_count][1];
                pairs_removed--;
                attempts = 0;
            } else {
                generate_full_board();
                pairs_removed = 0;
                removed_count = 0;
                attempts = 0;
                reset_count++;
                initialize_positions(possible_positions, &pos_index);
                pos_reset_count = 0;
                continue;
            }
        }

        if (pos_index == 0) {
            pos_reset_count++;
            if (pos_reset_count >= max_resets) break;
            pos_index = 0;
            for (int row = 0; row < SIZE; row++) {
                int col_start = (row <= 4) ? 0 : (8 - row);
                int col_end = (row <= 4) ? (row == 4 ? 4 : 8) : 8;
                for (int col = col_start; col <= col_end; col++) {
                    if (board[row][col] != 0) {
                        possible_positions[pos_index][0] = row;
                        possible_positions[pos_index][1] = col;
                        pos_index++;
                    }
                }
            }
            if (pos_index == 0) break;
            shuffle(possible_positions, pos_index); // Use shuffle for 2D array
        }

        int i = rand() % pos_index;
        int row = possible_positions[i][0];
        int col = possible_positions[i][1];
        int sym_row = 8 - row;
        int sym_col = 8 - col;

        possible_positions[i][0] = possible_positions[pos_index - 1][0];
        possible_positions[i][1] = possible_positions[pos_index - 1][1];
        pos_index--;

        if (board[row][col] == 0 || board[sym_row][sym_col] == 0) {
            attempts++;
            continue;
        }

        int backup = board[row][col];
        int sym_backup = board[sym_row][sym_col];
        board[row][col] = 0;
        board[sym_row][sym_col] = 0;

        count_solutions();
        if (solution_count != 1) {
            board[row][col] = backup;
            board[sym_row][sym_col] = sym_backup;
            attempts++;
            continue;
        }

        removed_positions[removed_count][0] = row;
        removed_positions[removed_count][1] = col;
        backup_values[removed_count][0] = backup;
        backup_values[removed_count][1] = sym_backup;
        removed_count++;

        pairs_removed++;
        attempts = 0;
    }

    bool meets_criteria = false;
    int final_attempts = 0;
    while (removed_count > 0 && final_attempts < max_attempts) {
        bool can_solve = can_solve_with_techniques(board, 2);
        if (can_solve) {
            meets_criteria = true;
            break;
        }
        removed_count--;
        int row = removed_positions[removed_count][0];
        int col = removed_positions[removed_count][1];
        int sym_row = 8 - row;
        int sym_col = 8 - col;
        board[row][col] = backup_values[removed_count][0];
        board[sym_row][sym_col] = backup_values[removed_count][1];
        pairs_removed--;
        final_attempts++;
    }

    if (!meets_criteria && removed_count == 0) {
        generate_full_board();
        remove_numbers(target_empty_cells - 2, 2);
        return;
    }

    if (remove_center && board[4][4] != 0) {
        int center_backup = board[4][4];
        board[4][4] = 0;
        count_solutions();
        bool can_solve = can_solve_with_techniques(board, 2);
        if (solution_count != 1 || !can_solve) {
            board[4][4] = center_backup;
        }
    }

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            is_fixed[i][j] = (board[i][j] != 0);
}

void remove_numbers_hard(int target_empty_cells) {
    int pairs_to_remove = target_empty_cells / 2;
    bool remove_center = (target_empty_cells % 2 == 1);

    int possible_positions[81][2];
    int pos_index;
    initialize_positions(possible_positions, &pos_index);

    int pairs_removed = 0;
    const int max_attempts = 100;
    int attempts = 0;
    const int max_resets = 3;
    int reset_count = 0;
    int pos_reset_count = 0;

    int removed_positions[81][2];
    int backup_values[81][2];
    int removed_count = 0;

    while (pairs_removed < pairs_to_remove && reset_count < max_resets) {
        if (attempts >= max_attempts) {
            if (removed_count > 0) {
                removed_count--;
                int row = removed_positions[removed_count][0];
                int col = removed_positions[removed_count][1];
                int sym_row = 8 - row;
                int sym_col = 8 - col;
                board[row][col] = backup_values[removed_count][0];
                board[sym_row][sym_col] = backup_values[removed_count][1];
                pairs_removed--;
                attempts = 0;
            } else {
                generate_full_board();
                pairs_removed = 0;
                removed_count = 0;
                attempts = 0;
                reset_count++;
                initialize_positions(possible_positions, &pos_index);
                pos_reset_count = 0;
                continue;
            }
        }

        if (pos_index == 0) {
            pos_reset_count++;
            if (pos_reset_count >= max_resets) break;
            pos_index = 0;
            for (int row = 0; row < SIZE; row++) {
                int col_start = (row <= 4) ? 0 : (8 - row);
                int col_end = (row <= 4) ? (row == 4 ? 4 : 8) : 8;
                for (int col = col_start; col <= col_end; col++) {
                    if (board[row][col] != 0) {
                        possible_positions[pos_index][0] = row;
                        possible_positions[pos_index][1] = col;
                        pos_index++;
                    }
                }
            }
            if (pos_index == 0) break;
            shuffle(possible_positions, pos_index); // Use shuffle for 2D array
        }

        int i = rand() % pos_index;
        int row = possible_positions[i][0];
        int col = possible_positions[i][1];
        int sym_row = 8 - row;
        int sym_col = 8 - col;

        possible_positions[i][0] = possible_positions[pos_index - 1][0];
        possible_positions[i][1] = possible_positions[pos_index - 1][1];
        pos_index--;

        if (board[row][col] == 0 || board[sym_row][sym_col] == 0) {
            attempts++;
            continue;
        }

        int backup = board[row][col];
        int sym_backup = board[sym_row][sym_col];
        board[row][col] = 0;
        board[sym_row][sym_col] = 0;

        count_solutions();
        if (solution_count != 1) {
            board[row][col] = backup;
            board[sym_row][sym_col] = sym_backup;
            attempts++;
            continue;
        }

        removed_positions[removed_count][0] = row;
        removed_positions[removed_count][1] = col;
        backup_values[removed_count][0] = backup;
        backup_values[removed_count][1] = sym_backup;
        removed_count++;

        pairs_removed++;
        attempts = 0;
    }

    bool meets_criteria = false;
    int final_attempts = 0;
    while (removed_count > 0 && final_attempts < max_attempts) {
        bool can_solve = can_solve_with_techniques(board, 3);
        if (can_solve) {
            meets_criteria = true;
            break;
        }
        removed_count--;
        int row = removed_positions[removed_count][0];
        int col = removed_positions[removed_count][1];
        int sym_row = 8 - row;
        int sym_col = 8 - col;
        board[row][col] = backup_values[removed_count][0];
        board[sym_row][sym_col] = backup_values[removed_count][1];
        pairs_removed--;
        final_attempts++;
    }

    if (!meets_criteria && removed_count == 0) {
        generate_full_board();
        remove_numbers(target_empty_cells - 2, 3);
        return;
    }

    if (remove_center && board[4][4] != 0) {
        int center_backup = board[4][4];
        board[4][4] = 0;
        count_solutions();
        bool can_solve = can_solve_with_techniques(board, 3);
        if (solution_count != 1 || !can_solve) {
            board[4][4] = center_backup;
        }
    }

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            is_fixed[i][j] = (board[i][j] != 0);
}

void remove_numbers_advanced(int target_empty_cells) {

    int pairs_to_remove = target_empty_cells / 2;
    bool remove_center = (target_empty_cells % 2 == 1);

    int possible_positions[81][2];
    int pos_index;
    initialize_positions(possible_positions, &pos_index);

    int pairs_removed = 0;
    const int max_attempts = 300; // Increased from 200
    const int max_resets = 7; // Increased from 5
    int attempts = 0;
    int reset_count = 0;
    int pos_reset_count = 0;

    int removed_positions[81][2];
    int backup_values[81][2];
    int removed_count = 0;

    while (pairs_removed < pairs_to_remove && reset_count < max_resets) {
        if (attempts >= max_attempts) {

            if (removed_count > 0) {
                removed_count--;
                int row = removed_positions[removed_count][0];
                int col = removed_positions[removed_count][1];
                int sym_row = 8 - row;
                int sym_col = 8 - col;
                board[row][col] = backup_values[removed_count][0];
                board[sym_row][sym_col] = backup_values[removed_count][1];
                pairs_removed--;
                attempts = 0;
            } else {

                generate_full_board();
                pairs_removed = 0;
                removed_count = 0;
                attempts = 0;
                reset_count++;
                initialize_positions(possible_positions, &pos_index);
                pos_reset_count = 0;
                continue;
            }
        }

        if (pos_index == 0) {
            pos_reset_count++;

            if (pos_reset_count >= max_resets) break;
            pos_index = 0;
            for (int row = 0; row < SIZE; row++) {
                int col_start = (row <= 4) ? 0 : (8 - row);
                int col_end = (row <= 4) ? (row == 4 ? 4 : 8) : 8;
                for (int col = col_start; col <= col_end; col++) {
                    if (board[row][col] != 0) {
                        possible_positions[pos_index][0] = row;
                        possible_positions[pos_index][1] = col;
                        pos_index++;
                    }
                }
            }
            if (pos_index == 0) break;
            shuffle(possible_positions, pos_index); // Use shuffle for 2D array
        }

        int i = rand() % pos_index;
        int row = possible_positions[i][0];
        int col = possible_positions[i][1];
        int sym_row = 8 - row;
        int sym_col = 8 - col;

        possible_positions[i][0] = possible_positions[pos_index - 1][0];
        possible_positions[i][1] = possible_positions[pos_index - 1][1];
        pos_index--;

        if (board[row][col] == 0 || board[sym_row][sym_col] == 0) {
            attempts++;
            continue;
        }

        int backup = board[row][col];
        int sym_backup = board[sym_row][sym_col];
        board[row][col] = 0;
        board[sym_row][sym_col] = 0;

        count_solutions();
        if (solution_count != 1) {
            board[row][col] = backup;
            board[sym_row][sym_col] = sym_backup;
            attempts++;
            continue;
        }

        removed_positions[removed_count][0] = row;
        removed_positions[removed_count][1] = col;
        backup_values[removed_count][0] = backup;
        backup_values[removed_count][1] = sym_backup;
        removed_count++;

        pairs_removed++;
        attempts = 0;

    }

    bool meets_criteria = false;
    int final_attempts = 0;
    while (removed_count > 0 && final_attempts < max_attempts) {

        bool can_solve = can_solve_with_techniques(board, 4);

        if (can_solve) {
            meets_criteria = true;
            break;
        }
        removed_count--;
        int row = removed_positions[removed_count][0];
        int col = removed_positions[removed_count][1];
        int sym_row = 8 - row;
        int sym_col = 8 - col;
        board[row][col] = backup_values[removed_count][0];
        board[sym_row][sym_col] = backup_values[removed_count][1];
        pairs_removed--;
        final_attempts++;
    }

    if (!meets_criteria && removed_count == 0) {

        remove_numbers_hard(target_empty_cells); // Fallback to difficulty 3
        return;
    }

    if (remove_center && board[4][4] != 0) {

        int center_backup = board[4][4];
        board[4][4] = 0;
        count_solutions();
        bool can_solve = can_solve_with_techniques(board, 4);

        if (solution_count != 1 || !can_solve) {
            board[4][4] = center_backup;
        }
    }


    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            is_fixed[i][j] = (board[i][j] != 0);
}

void remove_numbers_expert(int target_empty_cells) {
    int pairs_to_remove = target_empty_cells / 2;
    bool remove_center = (target_empty_cells % 2 == 1);

    int possible_positions[81][2];
    int pos_index;
    initialize_positions(possible_positions, &pos_index);

    int pairs_removed = 0;
    const int max_attempts = 100;
    int attempts = 0;
    const int max_resets = 3;
    int reset_count = 0;
    int pos_reset_count = 0;

    int removed_positions[81][2];
    int backup_values[81][2];
    int removed_count = 0;

    while (pairs_removed < pairs_to_remove && reset_count < max_resets) {
        if (attempts >= max_attempts) {
            if (removed_count > 0) {
                removed_count--;
                int row = removed_positions[removed_count][0];
                int col = removed_positions[removed_count][1];
                int sym_row = 8 - row;
                int sym_col = 8 - col;
                board[row][col] = backup_values[removed_count][0];
                board[sym_row][sym_col] = backup_values[removed_count][1];
                pairs_removed--;
                attempts = 0;
            } else {
                generate_full_board();
                pairs_removed = 0;
                removed_count = 0;
                attempts = 0;
                reset_count++;
                initialize_positions(possible_positions, &pos_index);
                pos_reset_count = 0;
                continue;
            }
        }

        if (pos_index == 0) {
            pos_reset_count++;
            if (pos_reset_count >= max_resets) break;
            pos_index = 0;
            for (int row = 0; row < SIZE; row++) {
                int col_start = (row <= 4) ? 0 : (8 - row);
                int col_end = (row <= 4) ? (row == 4 ? 4 : 8) : 8;
                for (int col = col_start; col <= col_end; col++) {
                    if (board[row][col] != 0) {
                        possible_positions[pos_index][0] = row;
                        possible_positions[pos_index][1] = col;
                        pos_index++;
                    }
                }
            }
            if (pos_index == 0) break;
            shuffle(possible_positions, pos_index); // Use shuffle for 2D array
        }

        int i = rand() % pos_index;
        int row = possible_positions[i][0];
        int col = possible_positions[i][1];
        int sym_row = 8 - row;
        int sym_col = 8 - col;

        possible_positions[i][0] = possible_positions[pos_index - 1][0];
        possible_positions[i][1] = possible_positions[pos_index - 1][1];
        pos_index--;

        if (board[row][col] == 0 || board[sym_row][sym_col] == 0) {
            attempts++;
            continue;
        }

        int backup = board[row][col];
        int sym_backup = board[sym_row][sym_col];
        board[row][col] = 0;
        board[sym_row][sym_col] = 0;

        removed_positions[removed_count][0] = row;
        removed_positions[removed_count][1] = col;
        backup_values[removed_count][0] = backup;
        backup_values[removed_count][1] = sym_backup;
        removed_count++;

        pairs_removed++;
        attempts = 0;
    }

    count_solutions();
    int min_empty_cells = 58;
    int current_empty_cells = pairs_removed * 2 + (remove_center ? 1 : 0);
    while (solution_count != 1 && current_empty_cells > min_empty_cells && removed_count > 0) {
        removed_count--;
        int row = removed_positions[removed_count][0];
        int col = removed_positions[removed_count][1];
        int sym_row = 8 - row;
        int sym_col = 8 - col;
        board[row][col] = backup_values[removed_count][0];
        board[sym_row][sym_col] = backup_values[removed_count][1];
        pairs_removed--;
        current_empty_cells = pairs_removed * 2 + (remove_center ? 1 : 0);
        count_solutions();
    }

    if (remove_center && board[4][4] != 0) {
        int center_backup = board[4][4];
        board[4][4] = 0;
        count_solutions();
        if (solution_count != 1) {
            board[4][4] = center_backup;
        }
    }

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            is_fixed[i][j] = (board[i][j] != 0);
}

void remove_numbers(int target_empty_cells, int difficulty) {
    static int recursion_depth = 0;
    if (recursion_depth++ > 10) { // Prevent infinite recursion
        recursion_depth--;

        return;
    }


    switch (difficulty) {
        case 1: remove_numbers_easy(target_empty_cells); break;
        case 2: remove_numbers_medium(target_empty_cells); break;
        case 3: remove_numbers_hard(target_empty_cells); break;
        case 4: remove_numbers_advanced(target_empty_cells); break;
        case 5: remove_numbers_expert(target_empty_cells); break;
        default: remove_numbers_medium(target_empty_cells); break;
    }
    recursion_depth--;

}
