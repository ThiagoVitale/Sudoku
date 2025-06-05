#include <stdbool.h>
#include "sudoku.h"

static int temp_board[SIZE][SIZE];
int solution_count = 0;

void count_solutions_recursive() {
    int r, c;
    if (!find_empty(temp_board, &r, &c)) {
        solution_count++;
        return;
    }

    for (int num = 1; num <= 9; num++) {
        if (is_valid(temp_board, r, c, num)) {
            temp_board[r][c] = num;
            count_solutions_recursive();
            temp_board[r][c] = 0;
        }
        if (solution_count > 1)
            return;
    }
}

void count_solutions() {
    solution_count = 0;
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            temp_board[i][j] = board[i][j];
    count_solutions_recursive();
}

void initialize_candidates(int (*current_board)[SIZE], CellCandidates (*candidates)[SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (current_board[row][col] != 0) {
                candidates[row][col].count = 0;
                continue;
            }
            candidates[row][col].count = 9;
            for (int num = 1; num <= 9; num++) {
                candidates[row][col].candidates[num] = true;
            }
            for (int i = 0; i < SIZE; i++) {
                if (current_board[row][i] != 0) {
                    int num = current_board[row][i];
                    if (candidates[row][col].candidates[num]) {
                        candidates[row][col].candidates[num] = false;
                        candidates[row][col].count--;
                    }
                }
                if (current_board[i][col] != 0) {
                    int num = current_board[i][col];
                    if (candidates[row][col].candidates[num]) {
                        candidates[row][col].candidates[num] = false;
                        candidates[row][col].count--;
                    }
                }
            }
            int box_start_row = (row / 3) * 3;
            int box_start_col = (col / 3) * 3;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    int num = current_board[box_start_row + i][box_start_col + j];
                    if (num != 0 && candidates[row][col].candidates[num]) {
                        candidates[row][col].candidates[num] = false;
                        candidates[row][col].count--;
                    }
                }
            }
        }
    }
}

bool apply_naked_singles(int (*current_board)[SIZE], CellCandidates (*candidates)[SIZE]) {
    bool progress = false;
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (current_board[row][col] != 0) continue;
            if (candidates[row][col].count == 1) {
                for (int num = 1; num <= 9; num++) {
                    if (candidates[row][col].candidates[num]) {
                        current_board[row][col] = num;
                        initialize_candidates(current_board, candidates);
                        progress = true;
                        break;
                    }
                }
            }
        }
    }
    return progress;
}

bool apply_hidden_singles(int (*current_board)[SIZE], CellCandidates (*candidates)[SIZE]) {
    bool progress = false;
    for (int row = 0; row < SIZE; row++) {
        for (int num = 1; num <= 9; num++) {
            int count = 0;
            int last_col = -1;
            for (int col = 0; col < SIZE; col++) {
                if (current_board[row][col] == 0 && candidates[row][col].candidates[num]) {
                    count++;
                    last_col = col;
                }
            }
            if (count == 1) {
                current_board[row][last_col] = num;
                initialize_candidates(current_board, candidates);
                progress = true;
            }
        }
    }
    for (int col = 0; col < SIZE; col++) {
        for (int num = 1; num <= 9; num++) {
            int count = 0;
            int last_row = -1;
            for (int row = 0; row < SIZE; row++) {
                if (current_board[row][col] == 0 && candidates[row][col].candidates[num]) {
                    count++;
                    last_row = row;
                }
            }
            if (count == 1) {
                current_board[last_row][col] = num;
                initialize_candidates(current_board, candidates);
                progress = true;
            }
        }
    }
    for (int box_row = 0; box_row < 3; box_row++) {
        for (int box_col = 0; box_col < 3; box_col++) {
            for (int num = 1; num <= 9; num++) {
                int count = 0;
                int last_row = -1, last_col = -1;
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        int row = box_row * 3 + i;
                        int col = box_col * 3 + j;
                        if (current_board[row][col] == 0 && candidates[row][col].candidates[num]) {
                            count++;
                            last_row = row;
                            last_col = col;
                        }
                    }
                }
                if (count == 1) {
                    current_board[last_row][last_col] = num;
                    initialize_candidates(current_board, candidates);
                    progress = true;
                }
            }
        }
    }
    return progress;
}

bool apply_locked_candidates(int (*current_board)[SIZE], CellCandidates (*candidates)[SIZE]) {
    bool progress = false;
    for (int row = 0; row < SIZE; row++) {
        for (int num = 1; num <= 9; num++) {
            int box_cols[3] = {0, 0, 0};
            for (int col = 0; col < SIZE; col++) {
                if (current_board[row][col] == 0 && candidates[row][col].candidates[num]) {
                    int box = col / 3;
                    box_cols[box]++;
                }
            }
            for (int box = 0; box < 3; box++) {
                if (box_cols[box] > 0) {
                    int other_boxes[2] = {(box + 1) % 3, (box + 2) % 3};
                    if (box_cols[other_boxes[0]] == 0 && box_cols[other_boxes[1]] == 0) {
                        int box_start_col = box * 3;
                        int box_start_row = (row / 3) * 3;
                        for (int i = 0; i < 3; i++) {
                            int r = box_start_row + i;
                            if (r == row) continue;
                            for (int j = 0; j < 3; j++) {
                                int c = box_start_col + j;
                                if (current_board[r][c] == 0 && candidates[r][c].candidates[num]) {
                                    candidates[r][c].candidates[num] = false;
                                    candidates[r][c].count--;
                                    progress = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    for (int col = 0; col < SIZE; col++) {
        for (int num = 1; num <= 9; num++) {
            int box_rows[3] = {0, 0, 0};
            for (int row = 0; row < SIZE; row++) {
                if (current_board[row][col] == 0 && candidates[row][col].candidates[num]) {
                    int box = row / 3;
                    box_rows[box]++;
                }
            }
            for (int box = 0; box < 3; box++) {
                if (box_rows[box] > 0) {
                    int other_boxes[2] = {(box + 1) % 3, (box + 2) % 3};
                    if (box_rows[other_boxes[0]] == 0 && box_rows[other_boxes[1]] == 0) {
                        int box_start_row = box * 3;
                        int box_start_col = (col / 3) * 3;
                        for (int j = 0; j < 3; j++) {
                            int c = box_start_col + j;
                            if (c == col) continue;
                            for (int i = 0; i < 3; i++) {
                                int r = box_start_row + i;
                                if (current_board[r][c] == 0 && candidates[r][c].candidates[num]) {
                                    candidates[r][c].candidates[num] = false;
                                    candidates[r][c].count--;
                                    progress = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    for (int box_row = 0; box_row < 3; box_row++) {
        for (int box_col = 0; box_col < 3; box_col++) {
            for (int num = 1; num <= 9; num++) {
                int row_counts[3] = {0, 0, 0};
                int col_counts[3] = {0, 0, 0};
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        int row = box_row * 3 + i;
                        int col = box_col * 3 + j;
                        if (current_board[row][col] == 0 && candidates[row][col].candidates[num]) {
                            row_counts[i]++;
                            col_counts[j]++;
                        }
                    }
                }
                for (int i = 0; i < 3; i++) {
                    if (row_counts[i] > 0) {
                        int other_rows[2] = {(i + 1) % 3, (i + 2) % 3};
                        if (row_counts[other_rows[0]] == 0 && row_counts[other_rows[1]] == 0) {
                            int row = box_row * 3 + i;
                            for (int c = 0; c < SIZE; c++) {
                                if (c / 3 == box_col) continue;
                                if (current_board[row][c] == 0 && candidates[row][c].candidates[num]) {
                                    candidates[row][c].candidates[num] = false;
                                    candidates[row][c].count--;
                                    progress = true;
                                }
                            }
                        }
                    }
                }
                for (int j = 0; j < 3; j++) {
                    if (col_counts[j] > 0) {
                        int other_cols[2] = {(j + 1) % 3, (j + 2) % 3};
                        if (col_counts[other_cols[0]] == 0 && col_counts[other_cols[1]] == 0) {
                            int col = box_col * 3 + j;
                            for (int r = 0; r < SIZE; r++) {
                                if (r / 3 == box_row) continue;
                                if (current_board[r][col] == 0 && candidates[r][col].candidates[num]) {
                                    candidates[r][col].candidates[num] = false;
                                    candidates[r][col].count--;
                                    progress = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (progress) initialize_candidates(current_board, candidates);
    return progress;
}

bool has_subgrid_with_one_empty(int (*current_board)[SIZE]) {
    for (int box_row = 0; box_row < 3; box_row++) {
        for (int box_col = 0; box_col < 3; box_col++) {
            int empty_count = 0;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    int row = box_row * 3 + i;
                    int col = box_col * 3 + j;
                    if (current_board[row][col] == 0) {
                        empty_count++;
                    }
                }
            }
            if (empty_count == 1) {
                return true;
            }
        }
    }
    return false;
}

bool can_solve_with_techniques(int (*current_board)[SIZE], int difficulty) {
    int work_board[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            work_board[i][j] = current_board[i][j];

    CellCandidates candidates[SIZE][SIZE];
    initialize_candidates(work_board, candidates);

    while (apply_naked_singles(work_board, candidates)) {
        // Continue until no more Naked Singles
    }

    if (difficulty == 1) {
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                if (work_board[i][j] == 0)
                    return false;
        return true;
    }

    if (difficulty >= 2) {
        while (apply_hidden_singles(work_board, candidates)) {
            while (apply_naked_singles(work_board, candidates)) {}
        }
    }

    if (difficulty == 2) {
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                if (work_board[i][j] == 0)
                    return false;
        return true;
    }

    if (difficulty >= 3) {
        while (apply_locked_candidates(work_board, candidates)) {
            while (apply_naked_singles(work_board, candidates)) {}
            while (apply_hidden_singles(work_board, candidates)) {
                while (apply_naked_singles(work_board, candidates)) {}
            }
        }
    }

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (work_board[i][j] == 0)
                return false;
    return true;
}