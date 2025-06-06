#include <stdbool.h>
#include <stdio.h> // For debugging
#include "sudoku.h"

bool can_solve_with_techniques(int (*current_board)[SIZE], int difficulty) {
    int work_board[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            work_board[i][j] = current_board[i][j];

    CellCandidates candidates[SIZE][SIZE];
    initialize_candidates(work_board, candidates);

    bool progress;
    bool solved_with_naked_singles = true;
    int max_iterations = 100; // Prevent infinite loops
    int iteration_count = 0;

    do {
        progress = apply_naked_singles(work_board, candidates);
        iteration_count++;
    } while (progress && iteration_count < max_iterations);

    bool solved = true;
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (work_board[i][j] == 0) {
                solved = false;
                solved_with_naked_singles = false;
                break;
            }

    if (difficulty == 1) {
        return solved;
    }

    bool used_hidden_singles = false; // Changed from requires_hidden_singles
    if (!solved) {
        iteration_count = 0;
        do {
            progress = apply_hidden_singles(work_board, candidates);
            if (progress) {
                used_hidden_singles = true;
                do {
                    progress = apply_naked_singles(work_board, candidates);
                    iteration_count++;
                } while (progress && iteration_count < max_iterations);
            }
            iteration_count++;
        } while (progress && iteration_count < max_iterations);

        solved = true;
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                if (work_board[i][j] == 0) {
                    solved = false;
                    break;
                }
    }
    if (difficulty == 2) {
        return solved; // Removed requires_hidden_singles condition
    }

    bool used_locked_candidates = false; // Changed from requires_locked_candidates
    if (!solved) {
        iteration_count = 0;
        do {
            progress = apply_locked_candidates(work_board, candidates);
            if (progress) {
                used_locked_candidates = true;
                bool inner_progress;
                do {
                    inner_progress = apply_hidden_singles(work_board, candidates);
                    if (inner_progress) {
                        do {
                            inner_progress = apply_naked_singles(work_board, candidates);
                        } while (inner_progress);
                    }
                } while (inner_progress);
            }
            iteration_count++;
        } while (progress && iteration_count < max_iterations);

        solved = true;
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                if (work_board[i][j] == 0) {
                    solved = false;
                    break;
                }
    }
    if (difficulty == 3) {
        return solved; // Removed requires_locked_candidates condition
    }

    bool used_x_wing = false; // Changed from requires_x_wing
    if (!solved) {
        iteration_count = 0;

        do {
            progress = apply_x_wing(work_board, candidates);
            if (progress) {
                used_x_wing = true;
                bool inner_progress;
                do {
                    inner_progress = apply_locked_candidates(work_board, candidates);
                    if (inner_progress) {
                        do {
                            inner_progress = apply_hidden_singles(work_board, candidates);
                            if (inner_progress) {
                                do {
                                    inner_progress = apply_naked_singles(work_board, candidates);
                                } while (inner_progress);
                            }
                        } while (inner_progress);
                    }
                } while (inner_progress);
            }
            iteration_count++;
            if (iteration_count % 10 == 0) {

            }
        } while (progress && iteration_count < max_iterations);

        solved = true;
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                if (work_board[i][j] == 0) {
                    solved = false;
                    break;
                }
    }
    if (difficulty == 4) {

        return solved; // Removed requires_x_wing condition
    }

    bool used_swordfish = false;
    if (!solved) {
        iteration_count = 0;
        do {
            progress = apply_swordfish(work_board, candidates);
            if (progress) {
                used_swordfish = true;
                bool inner_progress;
                do {
                    inner_progress = apply_x_wing(work_board, candidates);
                    if (inner_progress) {
                        do {
                            inner_progress = apply_locked_candidates(work_board, candidates);
                            if (inner_progress) {
                                do {
                                    inner_progress = apply_hidden_singles(work_board, candidates);
                                    if (inner_progress) {
                                        do {
                                            inner_progress = apply_naked_singles(work_board, candidates);
                                        } while (inner_progress);
                                    }
                                } while (inner_progress);
                            }
                        } while (inner_progress);
                    }
                } while (inner_progress);
            }
            iteration_count++;
        } while (progress && iteration_count < max_iterations);

        solved = true;
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                if (work_board[i][j] == 0) {
                    solved = false;
                    break;
                }
    }
    if (difficulty == 5) {
        return solved && used_swordfish;
    }

    return solved;
}
