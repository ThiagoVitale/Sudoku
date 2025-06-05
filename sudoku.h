#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdbool.h>

#define SIZE 9

// Structure to hold candidates for each cell
typedef struct {
    bool candidates[10]; // candidates[1] to candidates[9], candidates[0] unused
    int count; // Number of candidates
} CellCandidates;

// Structure to track invalid move conflicts
typedef struct {
    int row; // Row of the invalid move (-1 if no conflict)
    int col; // Column of the invalid move (-1 if no conflict)
    int conflict_type; // 0: none, 1: row, 2: column, 3: subgrid
} ConflictInfo;

// Global variables (extern declarations)
extern int board[SIZE][SIZE];
extern bool is_fixed[SIZE][SIZE];
extern int error_count;
extern ConflictInfo last_conflict;

// Function declarations (sudoku.c)
void play_sudoku();

// Function declarations (generator.c)
bool generate_full_board();
void remove_numbers(int empty_cells, int difficulty);
bool is_valid(int (*current_board)[SIZE], int row, int col, int num);
bool find_empty(int (*current_board)[SIZE], int *row, int *col);

// Function declarations (solver.c)
void count_solutions();
bool can_solve_with_techniques(int (*current_board)[SIZE], int difficulty);
void initialize_candidates(int (*current_board)[SIZE], CellCandidates (*candidates)[SIZE]);
bool apply_naked_singles(int (*current_board)[SIZE], CellCandidates (*candidates)[SIZE]);
bool apply_hidden_singles(int (*current_board)[SIZE], CellCandidates (*candidates)[SIZE]);
bool apply_locked_candidates(int (*current_board)[SIZE], CellCandidates (*candidates)[SIZE]);
bool has_subgrid_with_one_empty(int (*current_board)[SIZE]);

// Function declarations (display.c)
void print_board();
void print_horizontal_line();

extern int solution_count;

#endif