#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdbool.h>

#define SIZE 9

// Language enum
typedef enum {
    LANG_ENGLISH = 0,
    LANG_SPANISH = 1,
    LANG_COUNT = 2
} Language;

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
extern Language selected_language;
extern int solution_count;

// Message indices
enum {
    MSG_PROGRESS_ERRORS,
    MSG_SELECT_DIFFICULTY,
    MSG_EASY,
    MSG_MEDIUM,
    MSG_HARD,
    MSG_ADVANCED,
    MSG_EXPERT,
    MSG_ENTER_DIFFICULTY,
    MSG_INVALID_DIFFICULTY,
    MSG_DEFAULT_MEDIUM,
    MSG_CONGRATULATIONS,
    MSG_ENTER_ROW_COL_VALUE,
    MSG_INVALID_INPUT,
    MSG_CANNOT_MODIFY,
    MSG_INVALID_MOVE_REPEATED,
    MSG_ROW,
    MSG_COLUMN,
    MSG_SUBGRID,
    MSG_INVALID_MOVE_SOLUTIONS,
    MSG_SELECT_LANGUAGE,
    MSG_ENGLISH,
    MSG_SPANISH,
    MSG_COUNT
};

// Message array (extern declaration)
extern const char *messages[LANG_COUNT][MSG_COUNT];

// Function declarations (sudoku.c)
void play_sudoku();

// Function declarations (generator.c)
bool generate_full_board();
void remove_numbers(int empty_cells, int difficulty);
bool is_valid(int (*current_board)[SIZE], int row, int col, int num);
bool find_empty(int (*current_board)[SIZE], int *row, int *col);
void shuffle(int arr[][2], int size);
void shuffle_1d(int arr[], int size);

// Function declarations (solutions.c)
void count_solutions();

// Function declarations (solver.c)
bool can_solve_with_techniques(int (*current_board)[SIZE], int difficulty);

// Function declarations (candidates.c)
void initialize_candidates(int (*current_board)[SIZE], CellCandidates (*candidates)[SIZE]);
bool apply_naked_singles(int (*current_board)[SIZE], CellCandidates (*candidates)[SIZE]);
bool apply_hidden_singles(int (*current_board)[SIZE], CellCandidates (*candidates)[SIZE]);

// Function declarations (advanced_techniques.c)
bool apply_locked_candidates(int (*current_board)[SIZE], CellCandidates (*candidates)[SIZE]);
bool apply_x_wing(int (*current_board)[SIZE], CellCandidates (*candidates)[SIZE]);
bool apply_swordfish(int (*current_board)[SIZE], CellCandidates (*candidates)[SIZE]);

// Function declarations (utils.c)
bool has_subgrid_with_one_empty(int (*current_board)[SIZE]);

// Function declarations (display.c)
void print_board();
void print_horizontal_line();

#endif
