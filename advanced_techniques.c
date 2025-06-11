#include <stdbool.h>
#include <stdio.h> 
#include "sudoku.h"

bool apply_locked_candidates(int (*current_board)[SIZE], CellCandidates (*candidates)[SIZE]) {
    bool progress = false;
    for (int row = 0; row < SIZE; row++) {
        for (int num = 1; num <= 9; num++) {
            int box_cols[3] = {0, 0, 0};
            for (int col = 0; col < SIZE; col++) {
                if (current_board[row][col] == 0 && candidates[row][col].candidates[num - 1]) {
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
                                if (current_board[r][c] == 0 && candidates[r][c].candidates[num - 1]) {
                                    candidates[r][c].candidates[num - 1] = false;
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
                if (current_board[row][col] == 0 && candidates[row][col].candidates[num - 1]) {
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
                                if (current_board[r][c] == 0 && candidates[r][c].candidates[num - 1]) {
                                    candidates[r][c].candidates[num - 1] = false;
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
                        if (current_board[row][col] == 0 && candidates[row][col].candidates[num - 1]) {
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
                                if (current_board[row][c] == 0 && candidates[row][c].candidates[num - 1]) {
                                    candidates[row][c].candidates[num - 1] = false;
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
                                if (current_board[r][col] == 0 && candidates[r][col].candidates[num - 1]) {
                                    candidates[r][col].candidates[num - 1] = false;
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
    if (progress) {
        initialize_candidates(current_board, candidates);
    }
    return progress;
}

bool apply_x_wing(int (*current_board)[SIZE], CellCandidates (*candidates)[SIZE]) {
    bool progress = false;
    int max_checks = 1000; // Prevent excessive computation
    int check_count = 0;

    // Check rows
    for (int num = 1; num <= 9; num++) {
        for (int row1 = 0; row1 < SIZE - 1; row1++) {
            int col_positions1[SIZE], col_count1 = 0;
            for (int col = 0; col < SIZE; col++) {
                if (current_board[row1][col] == 0 && candidates[row1][col].candidates[num - 1]) {
                    col_positions1[col_count1++] = col;
                }
            }
            if (col_count1 != 2) continue;

            for (int row2 = row1 + 1; row2 < SIZE; row2++) {
                int col_positions2[SIZE], col_count2 = 0;
                for (int col = 0; col < SIZE; col++) {
                    if (current_board[row2][col] == 0 && candidates[row2][col].candidates[num - 1]) {
                        col_positions2[col_count2++] = col;
                    }
                }
                if (col_count2 != 2) continue;

                if (col_positions1[0] == col_positions2[0] && col_positions1[1] == col_positions2[1]) {
                    int col1 = col_positions1[0], col2 = col_positions1[1];
                    for (int row = 0; row < SIZE; row++) {
                        if (row == row1 || row == row2) continue;
                        if (current_board[row][col1] == 0 && candidates[row][col1].candidates[num - 1]) {
                            candidates[row][col1].candidates[num - 1] = false;
                            candidates[row][col1].count--;
                            progress = true;
                        }
                        if (current_board[row][col2] == 0 && candidates[row][col2].candidates[num - 1]) {
                            candidates[row][col2].candidates[num - 1] = false;
                            candidates[row][col2].count--;
                            progress = true;
                        }
                    }
                }
                check_count++;
                if (check_count >= max_checks) {
                    return progress;
                }
            }
        }
    }

    // Check columns
    for (int num = 1; num <= 9; num++) {
        for (int col1 = 0; col1 < SIZE - 1; col1++) {
            int row_positions1[SIZE], row_count1 = 0;
            for (int row = 0; row < SIZE; row++) {
                if (current_board[row][col1] == 0 && candidates[row][col1].candidates[num - 1]) {
                    row_positions1[row_count1++] = row;
                }
            }
            if (row_count1 != 2) continue;

            for (int col2 = col1 + 1; col2 < SIZE; col2++) {
                int row_positions2[SIZE], row_count2 = 0;
                for (int row = 0; row < SIZE; row++) {
                    if (current_board[row][col2] == 0 && candidates[row][col2].candidates[num - 1]) {
                        row_positions2[row_count2++] = row;
                    }
                }
                if (row_count2 != 2) continue;

                if (row_positions1[0] == row_positions2[0] && row_positions1[1] == row_positions2[1]) {
                    int row1 = row_positions1[0], row2 = row_positions1[1];
                    for (int col = 0; col < SIZE; col++) {
                        if (col == col1 || col == col2) continue;
                        if (current_board[row1][col] == 0 && candidates[row1][col].candidates[num - 1]) {
                            candidates[row1][col].candidates[num - 1] = false;
                            candidates[row1][col].count--;
                            progress = true;
                        }
                        if (current_board[row2][col] == 0 && candidates[row2][col].candidates[num - 1]) {
                            candidates[row2][col].candidates[num - 1] = false;
                            candidates[row2][col].count--;
                            progress = true;
                        }
                    }
                }
                check_count++;
                if (check_count >= max_checks) {
                    return progress;
                }
            }
        }
    }

    if (progress) {
        initialize_candidates(current_board, candidates);
    }
    return progress;
}

bool apply_swordfish(int (*current_board)[SIZE], CellCandidates (*candidates)[SIZE]) {
    bool progress = false;

    // Check rows
    for (int num = 1; num <= 9; num++) {
        for (int row1 = 0; row1 < SIZE - 2; row1++) {
            int col_positions1[SIZE], col_count1 = 0;
            for (int col = 0; col < SIZE; col++) {
                if (current_board[row1][col] == 0 && candidates[row1][col].candidates[num - 1]) {
                    col_positions1[col_count1++] = col;
                }
            }
            if (col_count1 < 2 || col_count1 > 3) continue;

            for (int row2 = row1 + 1; row2 < SIZE - 1; row2++) {
                int col_positions2[SIZE], col_count2 = 0;
                for (int col = 0; col < SIZE; col++) {
                    if (current_board[row2][col] == 0 && candidates[row2][col].candidates[num - 1]) {
                        col_positions2[col_count2++] = col;
                    }
                }
                if (col_count2 < 2 || col_count2 > 3) continue;

                for (int row3 = row2 + 1; row3 < SIZE; row3++) {
                    int col_positions3[SIZE], col_count3 = 0;
                    for (int col = 0; col < SIZE; col++) {
                        if (current_board[row3][col] == 0 && candidates[row3][col].candidates[num - 1]) {
                            col_positions3[col_count3++] = col;
                        }
                    }
                    if (col_count3 < 2 || col_count3 > 3) continue;

                    int cols[SIZE], col_count = 0;
                    for (int i = 0; i < col_count1; i++) {
                        int col = col_positions1[i];
                        bool found = false;
                        for (int j = 0; j < col_count; j++) {
                            if (cols[j] == col) {
                                found = true;
                                break;
                            }
                        }
                        if (!found) cols[col_count++] = col;
                    }
                    for (int i = 0; i < col_count2; i++) {
                        int col = col_positions2[i];
                        bool found = false;
                        for (int j = 0; j < col_count; j++) {
                            if (cols[j] == col) {
                                found = true;
                                break;
                            }
                        }
                        if (!found) cols[col_count++] = col;
                    }
                    for (int i = 0; i < col_count3; i++) {
                        int col = col_positions3[i];
                        bool found = false;
                        for (int j = 0; j < col_count; j++) {
                            if (cols[j] == col) {
                                found = true;
                                break;
                            }
                        }
                        if (!found) cols[col_count++] = col;
                    }

                    if (col_count == 3) {
                        for (int row = 0; row < SIZE; row++) {
                            if (row == row1 || row == row2 || row == row3) continue;
                            for (int i = 0; i < col_count; i++) {
                                int col = cols[i];
                                if (current_board[row][col] == 0 && candidates[row][col].candidates[num - 1]) {
                                    candidates[row][col].candidates[num - 1] = false;
                                    candidates[row][col].count--;
                                    progress = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Check columns
    for (int num = 1; num <= 9; num++) {
        for (int col1 = 0; col1 < SIZE - 2; col1++) {
            int row_positions1[SIZE], row_count1 = 0;
            for (int row = 0; row < SIZE; row++) {
                if (current_board[row][col1] == 0 && candidates[row][col1].candidates[num - 1]) {
                    row_positions1[row_count1++] = row;
                }
            }
            if (row_count1 < 2 || row_count1 > 3) continue;

            for (int col2 = col1 + 1; col2 < SIZE - 1; col2++) {
                int row_positions2[SIZE], row_count2 = 0;
                for (int row = 0; row < SIZE; row++) {
                    if (current_board[row][col2] == 0 && candidates[row][col2].candidates[num - 1]) {
                        row_positions2[row_count2++] = row;
                    }
                }
                if (row_count2 < 2 || row_count2 > 3) continue;

                for (int col3 = col2 + 1; col3 < SIZE; col3++) {
                    int row_positions3[SIZE], row_count3 = 0;
                    for (int row = 0; row < SIZE; row++) {
                        if (current_board[row][col3] == 0 && candidates[row][col3].candidates[num - 1]) {
                            row_positions3[row_count3++] = row;
                        }
                    }
                    if (row_count3 < 2 || row_count3 > 3) continue;

                    int rows[SIZE], row_count = 0;
                    for (int i = 0; i < row_count1; i++) {
                        int row = row_positions1[i];
                        bool found = false;
                        for (int j = 0; j < row_count; j++) {
                            if (rows[j] == row) {
                                found = true;
                                break;
                            }
                        }
                        if (!found) rows[row_count++] = row;
                    }
                    for (int i = 0; i < row_count2; i++) {
                        int row = row_positions2[i];
                        bool found = false;
                        for (int j = 0; j < row_count; j++) {
                            if (rows[j] == row) {
                                found = true;
                                break;
                            }
                        }
                        if (!found) rows[row_count++] = row;
                    }
                    for (int i = 0; i < row_count3; i++) {
                        int row = row_positions3[i];
                        bool found = false;
                        for (int j = 0; j < row_count; j++) {
                            if (rows[j] == row) {
                                found = true;
                                break;
                            }
                        }
                        if (!found) rows[row_count++] = row;
                    }

                    if (row_count == 3) {
                        for (int col = 0; col < SIZE; col++) {
                            if (col == col1 || col == col2 || col == col3) continue;
                            for (int i = 0; i < row_count; i++) {
                                int row = rows[i];
                                if (current_board[row][col] == 0 && candidates[row][col].candidates[num - 1]) {
                                    candidates[row][col].candidates[num - 1] = false;
                                    candidates[row][col].count--;
                                    progress = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (progress) {
        initialize_candidates(current_board, candidates);
    }
    return progress;
}
