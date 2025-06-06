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