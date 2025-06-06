#include <stdbool.h>
#include "sudoku.h"

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