#include <stdbool.h>
#include "sudoku.h"

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