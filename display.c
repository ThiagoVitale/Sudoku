#include <stdio.h>
#include <unistd.h> // For isatty
#include "sudoku.h"

// ANSI escape codes for colors and formatting
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define BLUE "\033[34m"
#define RED "\033[31m"
#define BG_WHITE "\033[47m"
#define BG_GRAY "\033[100m"

// Fallback for non-Unicode terminals
#define USE_UNICODE 1 // Set to 0 to use ASCII borders
#define HORIZ_LINE (USE_UNICODE ? "═══" : "---")
#define VERT_LINE (USE_UNICODE ? "║" : "|")
#define CROSS (USE_UNICODE ? "╬" : "+")
#define TOP_LEFT (USE_UNICODE ? "╔" : "+")
#define TOP_RIGHT (USE_UNICODE ? "╗" : "+")
#define BOTTOM_LEFT (USE_UNICODE ? "╚" : "+")
#define BOTTOM_RIGHT (USE_UNICODE ? "╝" : "+")
#define TOP_CROSS (USE_UNICODE ? "╦" : "+")
#define BOTTOM_CROSS (USE_UNICODE ? "╩" : "+")
#define LEFT_CROSS (USE_UNICODE ? "╠" : "+")
#define RIGHT_CROSS (USE_UNICODE ? "╣" : "+")

void print_horizontal_line(int is_top, int is_bottom) {
    int use_colors = isatty(STDOUT_FILENO);
    printf("    ");
    for (int i = 0; i < SIZE; i++) {
        printf("%s", HORIZ_LINE);
        if ((i + 1) % 3 == 0 && i != SIZE - 1)
            printf("%s", CROSS);
    }
    printf("\n");
}

void print_board() {
    int use_colors = isatty(STDOUT_FILENO); // Check if output is a terminal
    // Calculate filled cells for progress
    int filled_cells = 0;
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (board[i][j] != 0)
                filled_cells++;
    float progress = (float)filled_cells / (SIZE * SIZE) * 100;

    // Print progress
    printf("\nProgreso: %.1f%% Errores: %d\n", progress, error_count);

    // Print column headers
    printf("     ");
    for (int i = 0; i < SIZE; i++) {
        printf(" %d ", i + 1);
        if ((i + 1) % 3 == 0 && i != SIZE - 1)
            printf(" ");
    }
    printf("\n");

    // Top border
    printf("   %s", TOP_LEFT);
    for (int i = 0; i < SIZE; i++) {
        printf("%s", HORIZ_LINE);
        if ((i + 1) % 3 == 0 && i != SIZE - 1)
            printf("%s", TOP_CROSS);
    }
    printf("%s\n", TOP_RIGHT);

    for (int row = 0; row < SIZE; row++) {
        printf(" %d %s", row + 1, VERT_LINE);
        for (int col = 0; col < SIZE; col++) {
            // Determine subgrid background color (alternating white/gray)
            int box_row = row / 3;
            int box_col = col / 3;
            const char * bg_color = (box_row + box_col) % 2 ? BG_GRAY : BG_WHITE;

            // Check if cell is part of a conflict
            int is_conflict = 0;
            if (last_conflict.row != -1 && last_conflict.col != -1) {
                if (last_conflict.conflict_type == 1 && row == last_conflict.row)
                    is_conflict = 1;
                else if (last_conflict.conflict_type == 2 && col == last_conflict.col)
                    is_conflict = 1;
                else if (last_conflict.conflict_type == 3 &&
                         row / 3 == last_conflict.row / 3 &&
                         col / 3 == last_conflict.col / 3)
                    is_conflict = 1;
            }

            // Print cell with appropriate formatting
            if (use_colors) {
                printf("%s", is_conflict ? RED : bg_color);
                if (is_fixed[row][col])
                    printf("%s", BOLD BLUE);
            }

            if (board[row][col] == 0)
                printf(" . ");
            else
                printf(" %d ", board[row][col]);

            if (use_colors)
                printf("%s", RESET);

            if ((col + 1) % 3 == 0 && col != SIZE - 1)
                printf("%s", VERT_LINE);
        }
        printf("%s\n", VERT_LINE);

        if ((row + 1) % 3 == 0 && row != SIZE - 1) {
            printf("   %s", LEFT_CROSS);
            for (int i = 0; i < SIZE; i++) {
                printf("%s", HORIZ_LINE);
                if ((i + 1) % 3 == 0 && i != SIZE - 1)
                    printf("%s", CROSS);
            }
            printf("%s\n", RIGHT_CROSS);
        }
    }

    // Bottom border
    printf("   %s", BOTTOM_LEFT);
    for (int i = 0; i < SIZE; i++) {
        printf("%s", HORIZ_LINE);
        if ((i + 1) % 3 == 0 && i != SIZE - 1)
            printf("%s", BOTTOM_CROSS);
    }
    printf("%s\n", BOTTOM_RIGHT);
}